#ifndef _snake_h
#define _snake_h
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  ORIENTATION_up,
  ORIENTATION_down,
  ORIENTATION_left,
  ORIENTATION_right,
} ORIENTATION;

typedef enum {
  LEVEL_easy,
  LEVEL_medium,
  LEVEL_hard,
} LEVEL;

typedef struct node{
    GRID x;
    GRID y;
    struct node *next;
} Node;

typedef struct Snake{
  Node *head;
  u_int16_t totalNode;
  u_int16_t highestPoint;

  GRID liveInBoxWidth;
  GRID liveInBoxHeight;
  LEVEL level;

  volatile bool alive;
  volatile bool finished;
  volatile ORIENTATION current_orient;
  volatile ORIENTATION next_orient;
  volatile bool snakeIsFull;

  pthread_t thread;
} Snake;

Snake *Snake_new(GRID, GRID);
void Snake_delete(Snake *);
void Snake_FindNextStep(Snake *, GRID *, GRID *);
bool Snake_MoveOneStep(Snake *, GRID, GRID);

#ifdef __cplusplus
}
#endif
#endif
