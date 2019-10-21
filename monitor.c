#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "monitor.h"

static void Monitor_Print(GRID *, GRID, GRID);
static void Monitor_PrintLine(GRID *, GRID, GRID, int);
static Food *Monitor_CreateValidFood(Snake *, GRID, GRID);
static bool Monitor_IsFoodValid(Snake *, Food *);
static bool Monitor_IsFoodEaten(Snake *, Food *);

static void Monitor_Print(GRID *buf, GRID width, GRID height) {
  for(int i = 0; i < height; i++) {
    Monitor_PrintLine(buf + width * i, width, height, i);
  }
}

static void Monitor_PrintLine(GRID *buf, GRID width, GRID height, int i) {
  GRID *line = calloc(sizeof(GRID), width);
  assert(line);
  for(int i = 0; i < width; i++) {
    if(*(buf + i) == Symbol_Snake)
      *(line + i) = '*';
    else if(*(buf + i) == Symbol_Border)
      *(line + i) = '#';
    else if(*(buf + i) == Symbol_Food)
      *(line + i) = '@';
    else
      *(line + i) = ' ';
  }
  printf("   %s\n", line);
  free(line);
}

static Food *Monitor_CreateValidFood(Snake *snake, GRID w, GRID h) {
    Food *f = NULL;
    while(true) {
      f = Food_new(w, h);
      if(Monitor_IsFoodValid(snake, f)) 
        break;
      else 
        Food_delete(f);
    }
    return f;
}

static bool Monitor_IsFoodValid(Snake *snake, Food *food) {
  bool result = true;
  Node *node = snake->head;
  while (node != NULL) {
    if (node->x == food->x && node->y == food->y) {
      result = false;
      break;
    }
    else {
      node = node->next;
    }
  }
  return result;
}

static bool Monitor_IsFoodEaten(Snake *snake, Food *food) {
  if(snake->head->x == food->x && snake->head->y == food->y)
    return true;
  else
    return false;
}

Monitor *Monitor_new(Snake *snake, GRID w, GRID h, MonitorDraw draw) {
  Monitor *m = calloc(1, sizeof(Monitor));
  if(m) {
    m->width = w;
    m->height = h;
    m->draw = draw;
    m->snake = snake;
    m->food = Monitor_CreateValidFood(snake, w, h);
    return m;
  }
  return NULL;
}

void Monitor_delete(Snake *monitor) {
    if(monitor) {
      free(monitor);
      monitor = NULL;
  }
}

void Monitor_Draw(Monitor* monitor) {
  Monitor_Clear();
  GRID fullScreen[monitor->height][monitor->width];
  memset(fullScreen, 0, sizeof(fullScreen));

  int h = 0;
  while(h < monitor->height) {
    if(h == 0 || h == monitor->height - 1) {
      for(int w = 0; w < monitor->width; w++) {
        fullScreen[h][w] = Symbol_Border;
      }
    }
    else{
      fullScreen[h][0] = Symbol_Border;
      fullScreen[h][monitor->width - 1] = Symbol_Border;
    }
    h++;
  }

  GRID newX = 0, newY = 0;
  Snake_FindNextStep(monitor->snake, &newX, &newY);
  if(newX <= 0 || newX >= monitor->snake->liveInBoxWidth - 1 || newY <= 0 || newY >= monitor->snake->liveInBoxHeight - 1) {
    monitor->snake->alive = false;
  }else{
    if(!Snake_MoveOneStep(monitor->snake, newX, newY))
      monitor->snake->alive = false;

    if(monitor->snake->totalNode == monitor->snake->highestPoint)
      monitor->snake->finished = true;
  }

  if(Monitor_IsFoodEaten(monitor->snake, monitor->food)) {
    monitor->snake->snakeIsFull = true;
    monitor->food = Monitor_CreateValidFood(monitor->snake, monitor->width, monitor->height);
  }
  fullScreen[monitor->food->y][monitor->food->x] = Symbol_Food;

  Node *node = monitor->snake->head;
  while (node != NULL) {
    fullScreen[node->y][node->x] = Symbol_Snake;
    node = node->next;
  }

  Monitor_Print((GRID *)fullScreen, monitor->width, monitor->height);
  printf("monitor w: %d, h: %d\n", monitor->width, monitor->height);
  printf("food: %d, %d\n", monitor->food->x, monitor->food->y);
}

void Monitor_Clear(void) {
  printf("\x1b[H\x1b[J"); 
}
