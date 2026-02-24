# Boukdad ps3a Makefile

CC = g++
CFLAGS = -std=c++20 -Wall -Werror -Wextra -pedantic -g
LIB = -lsfml-graphics -lsfml-window -lsfml-system

# .hpp files
DEPS = Sokoban.hpp SpriteSheet.hpp

# comiles .o files
OBJECTS = Sokoban.o SpriteSheet.o TextureView.o

# name of program
PROGRAM = Sokoban

.PHONY: all clean lint

all: $(PROGRAM) Sokoban.a

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c $<

$(PROGRAM): main.o $(OBJECTS)
	$(CC) $(CFLAGS) -o $@	 $^ $(LIB)

Sokoban.a: $(OBJECTS)
	ar rcs Sokoban.a $(OBJECTS)

clean: 
	rm -f *.o $(PROGRAM) Sokoban.a

lint:
	cpplint --filter=-build/include_subdir *.cpp *.hpp