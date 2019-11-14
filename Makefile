CC = gcc
EXEC = snake 
CFLAGS = -std=gnu99 -Wall
OBJS = ./main.o ./food.o ./monitor.o ./snake.o

all: $(OBJS)
	$(CC) $(OBJS) -o $(EXEC) -lpthread
	strip $(EXEC)

%.o: %.c
	$(CC) -c -o $*.o $(CFLAGS) $<

.PHONY: clean
clean:
	-rm -f $(OBJS) $(EXEC)

