#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "snake.h"
#include "monitor.h"
#include "food.h"


int main(int argc, char *argv[]) {
  GRID borderWidth = 18;
  GRID borderHeight = 10;
  Snake *snake = Snake_new(borderWidth, borderHeight);
  Monitor *monitor = Monitor_new(snake, borderWidth, borderHeight, Monitor_Draw); 
  if(snake && monitor) {
    while(true)
    {
      if (snake->alive && !snake->finished) {
        monitor->draw(monitor);
        usleep(250000);
      }
      else {
        Monitor_Clear();
        if(snake->finished) {
          puts("Congrats!");
          printf("You got the highes points %d\n", snake->totalNode);
        }
        else if(!snake->alive) {
          puts("Game Over!");
          printf("you got %d points\n", snake->totalNode);
        }
        break;
      }
    }
    Snake_delete(snake);
  }
}