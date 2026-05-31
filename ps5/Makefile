# Boukdad ps5 Makefile

CC = g++
CFLAGS = -std=c++20 -Wall -Werror -Wextra -pedantic -g
LIB = -lsfml-system -lboost_unit_test_framework

# .hpp files
DEPS = EDistance.hpp

# compile .o files
OBJECTS = EDistance.o

PROGRAM = EDistance

.PHONY: all clean lint test

all: $(PROGRAM) EDistance.a test

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c $<

test.o: test.cpp $(DEPS)
	$(CC) $(CFLAGS) -c test.cpp

$(PROGRAM): main.o $(OBJECTS)
	$(CC) $(CFLAGS) -o $@	$^ $(LIB)

EDistance.a: $(OBJECTS)
	ar rcs EDistance.a $(OBJECTS)

clean:
	rm -f *.o $(PROGRAM) EDistance.a test.o test

lint:
	cpplint --filter=-build/include_subdir *.cpp *.hpp

test: test.o $(OBJECTS)
	$(CC) $(CFLAGS) -o test test.o $(OBJECTS) $(LIB)