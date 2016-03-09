OBJS = ./src/main.cpp

CC = g++

COMPILER_FLAGS = -w

LINKER_FLAGS = -L/usr/local/lib -lSDL2

OBJ_NAMES = Asuna

all: $(OBJS)
	$(CC) $(OBJS)  $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAMES)

