# Boukdad PS2 Makefile

CC = g++
CFLAGS = -std=c++20 -Wall -Werror -Wextra -pedantic -g
LIB = -lsfml-graphics -lsfml-window -lsfml-system

# .hpp files
DEPS = PTree.hpp

# compiles .o files
OBJECTS = PTree.o

# name of program
PROGRAM = PTree

.PHONY: all clean lint

all: $(PROGRAM)

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c $<

$(PROGRAM): main.o $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)
	
clean:
	rm -f *.o $(PROGRAM)

lint: 
	cpplint --filter=-legal/copyright,-build/include_subdir,-build/header_guard *.cpp *.hpp



