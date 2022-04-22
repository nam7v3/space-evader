OBJS= main.cpp Game.cpp Timer.cpp Player.cpp Asteroid.cpp
CC = g++
FLAGS = -Wall -w -g
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf
OBJ_NAME = space-evader


all: $(OBJS)
	$(CC) $(FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME) $(OBJS)
	./space-evader
