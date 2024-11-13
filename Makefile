
OBJS = src/main.cpp src/utils.cpp
OBJ_NAME = main

CC = g++
COMPILER_FLAGS = -w
LINKER_FLAGS = -lSDL2 #-lSDL2_image


all: $(OBJ)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)


clean:
	rm -f $(OBJ_NAME)

