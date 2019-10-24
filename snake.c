#include <unistd.h>

#include "snake.h"
#include "monitor.h"

static int getch (void);
static void *Snake_RecieveCMD(void *);
static void Snake_AddHead(Snake *, GRID, GRID);
static bool Snake_CheckSnakeValidAndRemoveTail(Snake *, GRID, GRID);
static bool Snake_CheckSnakeValid(Snake *, GRID, GRID);

static int getch (void)
{
  int ch;
  struct termios oldt, newt;
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON|ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  ch = getchar();
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

  return ch;
}

static void *Snake_RecieveCMD(void *param) {
  pthread_detach(pthread_self());
  Snake *snake = (Snake *)param;
  
  while(true) {
    char ch = getch();
    if(ch == 27) {
      ch = getch();
      if(ch == 91) {
        ch = getch();
        if(ch == 65) {
          if(snake->current_orient == ORIENTATION_right || snake->current_orient == ORIENTATION_left)
            snake->next_orient = ORIENTATION_down;
          else
            snake->next_orient = snake->current_orient;
        }
        else if(ch == 66) {
          if(snake->current_orient == ORIENTATION_right || snake->current_orient == ORIENTATION_left) 
            snake->next_orient = ORIENTATION_up;
          else
            snake->next_orient = snake->current_orient;
        }
        else if(ch == 67) {
          if(snake->current_orient == ORIENTATION_up || snake->current_orient == ORIENTATION_down)
            snake->next_orient = ORIENTATION_right;
          else
            snake->next_orient = snake->current_orient;
        }
        else if(ch == 68) {
          if(snake->current_orient == ORIENTATION_up || snake->current_orient == ORIENTATION_down)
            snake->next_orient = ORIENTATION_left;
          else
            snake->next_orient = snake->current_orient;
        }
      }
    }
  }
  return NULL;
}

static void Snake_AddHead(Snake *snake, GRID x, GRID y) {
  Node *newNode = calloc(1, sizeof(Node));
  assert(newNode);
  newNode->x = x;
  newNode->y = y;
  newNode->next = snake->head;
  snake->head = newNode;
}

static bool Snake_CheckSnakeValidAndRemoveTail(Snake *snake, GRID x, GRID y) {
  Node *node = snake->head;
  Node *pre = snake->head;

  while(node->next != NULL) {
    pre = node;
    node = node->next;
    if(node->x == x && node->y == y) 
      return false;
  }
  free(node);
  pre->next = NULL;
  return true;
}

static bool Snake_CheckSnakeValid(Snake *snake, GRID x, GRID y) {
  Node *node = snake->head;
  Node *pre = snake->head;

  while(node->next != NULL) {
    pre = node;
    node = node->next;
    if(node->x == x && node->y == y) 
      return false;
  }
  return true;
}

Snake *Snake_new(GRID width, GRID height) {
  Snake *snake = calloc(1, sizeof(Snake));
  if(snake) {
    snake->liveInBoxWidth = width;
    snake->liveInBoxHeight = height;
    snake->totalNode = 1;
    snake->highestPoint = (width - 2) * (height - 2);
    snake->head = calloc(1, sizeof(Node));
    snake->head->x = (snake->liveInBoxWidth - 2)/3;
    snake->head->y = (snake->liveInBoxHeight - 2)/3;
    snake->head->next = NULL;

    snake->alive = true;
    snake->finished = false;
    snake->current_orient = ORIENTATION_right;
    snake->next_orient = ORIENTATION_right;
    snake->snakeIsFull = false;
    tcgetattr(STDIN_FILENO, &snake->oldt);
    
    printf("snake init position (%d, %d)\n", snake->head->x, snake->head->y);
    pthread_create(&snake->thread, NULL, Snake_RecieveCMD, snake);

    return snake;
  }
  return NULL;
}

void Snake_delete(Snake *snake) {
  if(snake) {
    if(snake->head) {
      free(snake->head);
      snake->head = NULL;
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &snake->oldt);
    free(snake);
    snake = NULL;
  }
}

void Snake_FindNextStep(Snake *snake, GRID *newX, GRID *newY) {
  GRID old_x = *newX = snake->head->x;
  GRID old_y = *newY = snake->head->y;
  if(snake->next_orient == ORIENTATION_up) {
    *newY = old_y + 1;
    snake->current_orient = ORIENTATION_up;
  } else if (snake->next_orient == ORIENTATION_down) {
    *newY = old_y - 1;
    snake->current_orient = ORIENTATION_down;
  } else if (snake->next_orient == ORIENTATION_left) {
    *newX = old_x - 1;
    snake->current_orient = ORIENTATION_left;
  } else if (snake->next_orient == ORIENTATION_right) {
    *newX = old_x + 1;
    snake->current_orient = ORIENTATION_right;
  }
  return;
}

bool Snake_MoveOneStep(Snake *snake, GRID x, GRID y) {
  Snake_AddHead(snake, x, y);

  if(snake->snakeIsFull) {
    if(!Snake_CheckSnakeValid(snake, x, y))
      return false;
    snake->snakeIsFull = false;
    snake->totalNode += 1;
  }
  else {
    if(!Snake_CheckSnakeValidAndRemoveTail(snake, x, y))
      return false;
  }
  return true;
}
