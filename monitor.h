#ifndef _monitor_h
#define _monitor_h
#include <stdbool.h>
#include "snake.h"
#include "food.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct Monitor{
  GRID width;
  GRID height;

  void (*draw)(struct Monitor*);

  Snake *snake;
  Food *food;

} Monitor;

typedef enum {
  Symbol_Empty,
  Symbol_Snake,
  Symbol_Food,
  Symbol_Border,
} SymbolType;

typedef void (*MonitorDraw)(Monitor*);

Monitor *Monitor_new(Snake *, GRID, GRID, MonitorDraw);
void Monitor_Draw(Monitor*);
void Monitor_Clear(void);


#ifdef __cplusplus
}
#endif
#endif