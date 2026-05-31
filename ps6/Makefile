# Boukdad PS6 Makefile

CC = g++
CFLAGS = -std=c++20 -Wall -Werror -Wextra -pedantic -g
LIB = -lboost_unit_test_framework

PROGRAM = ps6

.PHONY: all clean lint

all: TextWriter TextWriter.a test

TextWriter: TextWriter.o RandWriter.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

# static library
TextWriter.a: RandWriter.o
	ar rcs $@ $^

# test exectutable
test: test.o RandWriter.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

TextWriter.o: TextWriter.cpp RandWriter.hpp
	$(CC) $(CFLAGS) -c $<

RandWriter.o: RandWriter.cpp RandWriter.hpp
	$(CC) $(CFLAGS) -c $<

test.o: test.cpp RandWriter.hpp
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o TextWriter TextWriter.a test

lint:
	cpplint --filter=-build/include_subdir TextWriter.cpp RandWriter.cpp RandWriter.hpp test.cpp