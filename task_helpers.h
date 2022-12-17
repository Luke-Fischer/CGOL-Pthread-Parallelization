#ifndef _GOL_TASK_H
#define _GOL_TASK_H
#define SUCCESS 1
#define FAILURE 0


int * createStartBoard(int);
int * createSecondBoard(int);
void printBoard(int *, int);
void freeBoards(int *, int *);
int computeNeighbours(int *, int, int, int*);
int computeState(int, int);
void updateNextBoard(int *, int, int);

typedef struct NODE{
  int location;
  struct NODE * next;
}Node;

typedef struct QUEUE{
  int size;
  Node * head;
  Node * tail;
}Queue;

Queue * create_queue();
int size(Queue *);
int isempty(Queue *);
int enque(Queue *, int);
Node * dequeue(Queue *);


#endif



