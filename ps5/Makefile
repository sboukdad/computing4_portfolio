# Boukdad ps5 Makefile

CC = g++
CFLAGS = -std=c++20 -Wall -Werror -Wextra -pedantic -g -O0
LIB = -lsfml-system -lboost_unit_test_framework

# .hpp files
DEPS = EDistance.hpp

# compile .o files
OBJECTS = EDistance.o

PROGRAM = EDistance

.PHONY: all clean lint test EDistance

%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c $<

$(PROGRAM): main.o $(OBJECTS)
	$(CC) $(CFLAGS) -o $@	$^ $(LIB)

EDistance.a: $(OBJECTS)
	ar rcs EDistance.a $(OBJECTS)

all: $(PROGRAM) EDistance.a

clean:
	rm -f *.o $(PROGRAM) EDistance.a test.o test

lint:
	cpplint --filter=-build/include_subdir *.cpp *.hpp

test: test.o $(OBJECTS)
	$(CC) $(CFLAGS) -o test test.o $(OBJECTS) $(LIB)