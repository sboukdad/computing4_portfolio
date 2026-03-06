# Boukdad - ps1a Makefile

CC = g++
CFLAGS = --std=c++20 -Wall -Werror -pedantic -g -I..
LIB = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system -lboost_unit_test_framework

# your .hpp files 
DEPS = SpriteSheet.hpp

# your compiled .o files
OBJECTS = SpriteSheet.o TextureView.o

# declare command names
.PHONY: all clean lint

# default target - builds everything
all: SpriteBuilder SpriteBuilder.a test

# obj files
SpriteSheet.o: SpriteSheet.hpp $(DEPS)
	$(CC) $(CFLAGS) -c SpriteSheet.cpp -o SpriteSheet.o

TextureView.o: TextureView.cpp $(DEPS)
	$(CC) $(CFLAGS) -c TextureView.cpp -o TextureView.o

test.o: test.cpp $(DEPS)
	$(CC) $(CFLAGS) -c test.cpp -o test.o

main.o: main.cpp $(DEPS)
	$(CC) $(CFLAGS) -c main.cpp -o main.o

# exe files
SpriteBuilder: main.o $(OBJECTS)
	$(CC) $(CFLAGS) -o SpriteBuilder main.o $(OBJECTS) $(LIB)

SpriteBuilder.a: $(OBJECTS)
	ar rcs SpriteBuilder.a $(OBJECTS)	


test: test.o $(OBJECTS)
	$(CC) $(CFLAGS) -o test test.o $(OBJECTS) $(LIB)

clean: 
	rm -f *.o SpriteBuilder SpriteBuilder.a test

lint:
	cpplint --filter=-build/include_subdir *.cpp *.hpp