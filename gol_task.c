#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include "task_helpers.h"
void swapBoards();
int gridSize;
int numThreads;
int numIterations;
int displayOutput;
int *nextBoard;
int *currentBoard;
void * queueThread(void *arg);
void * readLiveQueue(void *arg);
void * readDeadQueue(void *arg);
int totalSize;
Queue * liveQueue;
Queue * deadQueue;
int deadWrites;
int aliveWrites;
int iterSynch[10000];
int iterCounter;
pthread_mutex_t liveMutex;
pthread_mutex_t deadMutex;
pthread_cond_t liveCond;
pthread_cond_t deadCond;
double calcTime(struct timeval start);
struct timeval	start;
double			elapsed;

int main(int argc, char *argv[]){
    pthread_t tid1;
    pthread_t tid2;
    pthread_t tid3;
    aliveWrites = 0;
    deadWrites = 0;
    iterCounter = 0;
    // initialize the mutex
    pthread_mutex_init(&liveMutex, NULL);
    pthread_mutex_init(&deadMutex, NULL);
    pthread_cond_init(&liveCond, NULL);
    pthread_cond_init(&deadCond, NULL);

    if(argc < 3 || argc > 4){
        printf("Incorrect arguments - usage: ./gol_task gridSize nIterations -d\n");
        exit(1);
    }

    //set global variables
    gridSize = atoi(argv[1]);
    numIterations = atoi(argv[2]);

    if(gridSize < 1){
        printf("Grid size must be greater than 0\n");
        exit(1);
    }
    if(numIterations > 10000){
        printf("Max iterations is 10000\n");
        exit(1);
    }

    if(argv[3] != NULL){
        if(strcmp(argv[3],"-d") == 0){
            displayOutput = 1;
        }
        else{
            printf("%s is not a valid flag - to select output display the correct flag is -d\n", argv[4]);
            exit(1);
        }
    }
    else{
        displayOutput = 0;
    }

    //Create the starting two boards
    currentBoard = createStartBoard(gridSize);
    nextBoard = createSecondBoard(gridSize);
    totalSize = gridSize * gridSize;

    if(displayOutput == 1){
        printf("\nStarting board\n");
        printBoard(currentBoard, gridSize);
    }

    liveQueue = create_queue();
    deadQueue = create_queue();

    /*
     * Get timing data
     */
	gettimeofday(&start, NULL);

    //First thread writes to queues
    pthread_create(&tid1, NULL, queueThread, NULL);
    pthread_create(&tid2, NULL, readLiveQueue, NULL);
    pthread_create(&tid3, NULL, readDeadQueue, NULL);

        //Driving code to generate boards
    for(int j = 0; j < numIterations; j++){    
        //Handle board swaps - busy wait
        while(1){
            if((aliveWrites + deadWrites) == totalSize){
                swapBoards();
                deadWrites = 0;
                aliveWrites = 0;
                iterSynch[iterCounter] = 1;
                iterCounter++;
                break;
            }
        }
    }

    pthread_cancel(tid1); 
    pthread_cancel(tid2); 
    pthread_cancel(tid3); 
    pthread_mutex_destroy(&liveMutex);
    pthread_mutex_destroy(&deadMutex);
    pthread_cond_destroy(&liveCond);
    pthread_cond_destroy(&deadCond);
    
    //Wait for pthreads
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    
    //Free mallocd data
    freeBoards(currentBoard, nextBoard);

    free(liveQueue);
    free(deadQueue);

    elapsed = calcTime(start);
	printf("\nTask parallelizing took %.4f seconds\n", elapsed);

    return 0;
}

//Driving code to write to queue
void * queueThread(void *arg){
    for(int j = 0; j < numIterations; j++){
        for(int i = 0; i < totalSize; i++){
            int live;
            int numNeighbours = computeNeighbours(currentBoard, gridSize, i, &live);
            int result = computeState(numNeighbours, live);
            if(result == 0){
                pthread_mutex_lock(&deadMutex);
                enque(deadQueue, i);
                pthread_mutex_unlock(&deadMutex);
                pthread_cond_signal(&deadCond);
            }
            else{
                pthread_mutex_lock(&liveMutex);
                enque(liveQueue, i);
                pthread_mutex_unlock(&liveMutex);
                pthread_cond_signal(&liveCond);
            }
        }
        while(iterSynch[j] != 1){
            //busy wait
        }
    }

    return((void *)0);
}

void * readLiveQueue(void * arg){
    for(int j = 0; j < numIterations; j++){
        while((aliveWrites + deadWrites) < totalSize){
            //queue is empty
            pthread_mutex_lock(&liveMutex);
            while(isempty(liveQueue) == 1){
                pthread_cond_wait(&liveCond, &liveMutex);
            }
            Node * node = dequeue(liveQueue);
            updateNextBoard(nextBoard, node -> location, 1);
            free(node);
            aliveWrites++;
            pthread_mutex_unlock(&liveMutex);
            
        }
        while(iterSynch[j] != 1){
            //busy wait
        }
    }
    return((void *)0);
}

void * readDeadQueue(void * arg){
    for(int j = 0; j < numIterations; j++){
        while((aliveWrites + deadWrites) < totalSize){
            //queue is empty
            pthread_mutex_lock(&deadMutex);
            while(isempty(deadQueue) == 1){
                pthread_cond_wait(&deadCond, &deadMutex);
            }
            Node * node = dequeue(deadQueue);
            updateNextBoard(nextBoard, node -> location, 0);
            free(node);
            deadWrites++;
            pthread_mutex_unlock(&deadMutex);
        }
        while(iterSynch[j] != 1){
            //busy wait
        }
    }
    return((void *)0);
}

void swapBoards(){
    //Swap boards to get ready for next iteration
    int* temp = nextBoard;
    nextBoard = currentBoard;
    currentBoard = temp;

    if(displayOutput == 1){
        printf("\n\nIteration: %d\n", iterCounter + 1);
        printBoard(currentBoard, gridSize);
    }
}

double calcTime(struct timeval start){
    
    long long		startusec, endusec;
    struct timeval	end;
    
    gettimeofday(&end, NULL);
    startusec = start.tv_sec * 1000000 + start.tv_usec;
    endusec = end.tv_sec * 1000000 + end.tv_usec;
    return (double)(endusec - startusec) / 1000000.0;
}