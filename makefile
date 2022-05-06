SRC_DIR = src
INCLUDE = include/
TARGET = space-evader
SRC = $(wildcard ${SRC_DIR}/*.cpp)
CC = g++
FLAGS = -w -Wall -g -O3 -I $(INCLUDE)
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf

all:
	$(CC) $(FLAGS) $(LINKER_FLAGS) -o $(TARGET) $(SRC)
	./space-evader
