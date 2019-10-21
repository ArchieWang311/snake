#ifndef _food_h
#define _food_h
#include <stdio.h>
#include <stdlib.h>
#include "snake.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  GRID x;
  GRID y;
} Food;

Food *Food_new(GRID, GRID);
void Food_delete(Food *);

#ifdef __cplusplus
}
#endif
#endif
