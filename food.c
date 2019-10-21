#include <stdbool.h>
#include <time.h>
#include <assert.h>

#include "food.h"
#include "monitor.h"

static int RANDOMNUMBER[10] = {1000, 1001, 1002, 1003, 1004, 1005, 1006, 1007, 1008, 1009};
static int RANDOMINDEX = 0;

static GRID randomNumber(GRID max, GRID min);

Food *Food_new(GRID w, GRID h) {
  Food *food = calloc(1, sizeof(Food));
  assert(food);
  food->x = randomNumber(w - 2, 1);
  food->y = randomNumber(h - 2, 1);
  return food;
}

static GRID randomNumber(GRID max, GRID min) {
  assert(max >= min);
  time_t seed = time(0) + RANDOMNUMBER[RANDOMINDEX];
  RANDOMINDEX = (RANDOMINDEX + 1) % sizeof(RANDOMNUMBER);
  srand(seed);
  return min + rand() % (max - min + 1);
}

void Food_delete(Food *food) {
  if(food) {
    free(food);
    food = NULL;
  }
}

