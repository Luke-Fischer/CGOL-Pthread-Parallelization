#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include "data_helpers.h"

struct boundary {
   int startIndex;
   int endIndex;
};
double calcTime(struct timeval start);
struct timeval	start;
double			elapsed;
pthread_mutex_t mutex;
void swapBoards();
void * threadFunction(void *arg);
int gridSize;
int numThreads;
int numIterations;
int displayOutput;
int *nextBoard;
int *currentBoard;
int progress;
int goNext;
int iterSynch[10000];
int countThreads;

int main(int argc, char *argv[]){
    if(argc < 4 || argc > 5){
        printf("Incorrect arguments - usage: ./gol_serial nThreads gridSize nIterations -d\n");
        exit(1);
    }
    progress = 0;
    goNext = 0;
    //set global variables
    numThreads = atoi(argv[1]);
    gridSize = atoi(argv[2]);
    numIterations = atoi(argv[3]);
    pthread_t tid[numThreads];
    struct boundary boundArr[numThreads];
    int err;
    int totalSize = gridSize * gridSize;
    countThreads = 0;

    // initialize the mutex
    pthread_mutex_init(&mutex, NULL);

    if(numThreads < 1){
        printf("Number of threads must be greater than 0\n");
        exit(1);
    }

    if(numThreads > totalSize){
        printf("Threads must be less than or equal to the number of cells\n");
        exit(0);
    }
    if(gridSize < 1){
        printf("Grid size must be greater than 0\n");
        exit(1);
    }
    if(numIterations > 10000){
        printf("Max iterations is 10000\n");
        exit(1);
    }

    if(argv[4] != NULL){
        if(strcmp(argv[4],"-d") == 0){
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

    if(displayOutput == 1){
        printf("\nStarting Board\n");
        printBoard(currentBoard, gridSize);
    }

    /*
     * Get timing data
     */
	gettimeofday(&start, NULL);

    //Create and send threads to working code
    for (int i = 0; i < numThreads; i++) {
        if(i == 0){
            boundArr[0].startIndex = 0;
            boundArr[0].endIndex = totalSize / numThreads - 1;
            if(((float)totalSize / numThreads - 1) != (float)boundArr[0].endIndex){
                boundArr[0].endIndex = boundArr[0].endIndex + 1;
            }
        }
        else{
            boundArr[i].startIndex = boundArr[i - 1].endIndex + 1;
            boundArr[i].endIndex = boundArr[i].startIndex + boundArr[0].endIndex;
            if(boundArr[i].endIndex >= totalSize){
                boundArr[i].endIndex = totalSize - 1;
            }
            if(boundArr[i].startIndex >= totalSize){
                break;
            }
        }
        countThreads++;
        err = pthread_create(&tid[i], NULL, threadFunction, (void*)&boundArr[i]);
        if (err != 0){
            printf("Cannot create thread, error: %d", err);
            exit(-1);
        }
    }
    int iterCounter = 0;
    //Handle board swaps - busy wait
    while(1){
        if(progress == totalSize){
            if(displayOutput == 1){
                printf("\nIteration: %d\n", iterCounter + 1);
            }
            swapBoards();
            progress = 0;
            iterSynch[iterCounter] = 1;
            iterCounter++;
            if(iterCounter + 1 > numIterations){
                break;
            }
        }
    }

    //Cleanup pthreads
    for (int i = 0; i < countThreads; i++){
        pthread_join(tid[i], NULL);
    }

    //Free mallocd data
    freeBoards(currentBoard, nextBoard);

    elapsed = calcTime(start);
	printf("\nData parallelizing took %.4f seconds\n", elapsed);

    return 0;
}

//Driving code to generate boards
void * threadFunction(void * bound){
    struct boundary *struct_ptr = (struct boundary*) bound;
    //printf("Starting: %d, Ending: %d\n", struct_ptr -> startIndex, struct_ptr -> endIndex);
    for(int j = 0; j < numIterations; j++){  
        int live;
        int index = struct_ptr -> startIndex;
        int endIndex = struct_ptr -> endIndex;
        for(index = index; index <= endIndex; index++){
            int numNeighbours = computeNeighbours(currentBoard, gridSize, index, &live);
            updateNextBoard(nextBoard, numNeighbours, index, live);
            pthread_mutex_lock(&mutex);
            progress++;
            pthread_mutex_unlock(&mutex);
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
