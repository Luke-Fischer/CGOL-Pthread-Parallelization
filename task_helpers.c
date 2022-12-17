#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "task_helpers.h"
#include <string.h>

int * createStartBoard(int gridSize){
    //initialize board of size gridSize x gridSize
    int * grid = malloc((gridSize * gridSize) * sizeof(int));
    int totalSize = gridSize * gridSize;

    //Randomly fill board with live cells and dead cells - the probability of a cell being live is 1 in every 10
    srand(time(NULL));
    
    for(int i = 0; i < totalSize; i++){
        if((rand() % 3) == 0){
            grid[i] = 1;
        }
        else{
            grid[i] = 0;
        }
    }   
    return grid;
}

int * createSecondBoard(int gridSize){
    //initialize board of size gridSize x gridSize
    int totalSize = gridSize * gridSize;
    int * grid = malloc((totalSize) * sizeof(int));
    for(int i = 0; i < totalSize; i++){
        grid[i] = 0;
    }
    return grid;
}

void printBoard(int * board, int gridSize){
    int totalSize = gridSize * gridSize;
    
    for(int i = 0; i < totalSize; i++){
        //Make 1D array display in 2D
        if(i % gridSize == 0){
            printf("\n");
        }
        printf("%d  ", board[i]);
    }
    printf("\n");
}

int computeNeighbours(int * board1, int gridSize, int index, int * live){
    int i = index;
    int totalSize = gridSize * gridSize;
    //Check each 8 neighboring cells of all cells in the grid - compute number of neighbours
 
    int neighboringCells = 0;
    //top left
    if(i % gridSize != 0){
        int topLeft = i - gridSize - 1;
        if(topLeft >= 0){
            if(board1[topLeft] == 1){
                neighboringCells++;
            }
        }
    }
    //top middle
    int topMiddle = i - gridSize;
    if(topMiddle >= 0){
        if(board1[topMiddle] == 1){
            neighboringCells++;
        }
    }
    //top right
    if((i + 1) % gridSize != 0){
        int topRight = i - gridSize + 1;
        if(topRight >= 0){
            if(board1[topRight] == 1){
                neighboringCells++;
            }
        }
    }
    //middle left
    if(i % gridSize != 0){
        int middleLeft = i - 1;
        if(board1[middleLeft] == 1){
            neighboringCells++;
        }
    }
    //middle right
    if((i + 1) % gridSize != 0){
        int middleRight = i + 1;
        if(board1[middleRight] == 1){
            neighboringCells++;
        }
    }
    
    //bottom left
    int bottomLeft = i + gridSize - 1;
    if(bottomLeft < totalSize){
        if(i % gridSize != 0){
            if(board1[bottomLeft] == 1){
                neighboringCells++;
            }
        }
    }
    //bottom middle
    int bottomMiddle = i + gridSize;
    if(bottomMiddle < totalSize){
        if(board1[bottomMiddle] == 1){
            neighboringCells++;
        }
    }
    //bottom right
    int bottomRight = i + gridSize + 1;
    if(bottomRight < totalSize){
        if((i + 1) % gridSize != 0){
            if(board1[bottomRight] == 1){
                neighboringCells++;
            }
        }
    }

    if(board1[i] == 0){
        *live = 0;
    }
    else{
        *live = 1;
    }
    return neighboringCells;
}

//Update board according to neighbours and cgol rules
int computeState(int numNeighbours, int live){
    if(live == 1){
        if(numNeighbours == 2 || numNeighbours == 3){
            return 1;
        }
        else{
            return 0;
        }
    }
    else{
        if(numNeighbours == 3){
            return 1;
        }
        else{
            return 0;
        }
    }
}

void updateNextBoard(int * nextBoard, int location, int status){
    nextBoard[location] = status;
}

void freeBoards(int * board1, int * board2){
    free(board1);
    free(board2);
}

Queue * create_queue(){
  Queue * queue = malloc(sizeof(Queue));
  queue -> head = NULL;
  queue -> tail = NULL;
  queue -> size = 0;
  return queue;
}

int isempty(Queue * queue){
  if((queue -> size) == 0){
    return SUCCESS;
  }
  else{
    return FAILURE;
  }
}

int enque(Queue * queue, int location){
  Node * new_node = malloc(sizeof(Node));
  Node * tail = queue -> tail;
  new_node -> location = location;

  if(isempty(queue) == SUCCESS){
    new_node -> next = NULL;
    queue -> head = new_node;
    queue -> tail = new_node;
  }
  else{
    tail -> next = new_node;
    new_node -> next = NULL;
    queue -> tail = new_node;
  }
  queue -> size++;
  return SUCCESS;
}

Node * dequeue(Queue * queue){
  Node * node = queue -> head;
  if(isempty(queue) == SUCCESS){
    return FAILURE;
  }
  else{
    queue -> head = node -> next;
    node -> next = NULL;
  }
  queue -> size--;
  return node;
}

void print_all(Queue * queue){
  if(isempty(queue) == SUCCESS){
    printf("QUEUE IS EMPTY\n");
  }
  else{
    Node * node = queue -> head;
    while(node != NULL){
      printf("%d ", node -> location);
      node = node -> next;
    }
  }
}
