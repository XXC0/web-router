SRC = $(wildcard *.c)
OBJ = $(patsubst %*.c,%.o,$(SRC))
TARGET=router
FLAGS = -Wall
CC = gcc 

$(TARGET):$(OBJ)
	$(CC)  $^ -o $@ $(FLAGS) -lpthread -levent

%.o:%.c
	$(CC) -c $< -o $@
.PHONY:clean
clean:
	rm *.o $(TARGET)
	