#ifndef _GOL_DATA_H
#define _GOL_DATA_H

int * createStartBoard(int);
int * createSecondBoard(int);
void printBoard(int *, int);
void freeBoards(int *, int *);
int computeNeighbours(int *, int, int, int*);
void updateNextBoard(int *, int, int, int);

#endif