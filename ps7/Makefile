# Boukdad PS7 Makefile

CC = g++
CFLAGS = -std=c++20 -Wall -Werror -Wextra -pedantic -g
LIB = -lboost_date_time

DEPS =
OBJECTS =

PROGRAM = ps7

.PHONY: all clean lint

all: $(PROGRAM)

$(PROGRAM): main.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

main.o: main.cpp
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o $(PROGRAM) *.rpt

lint:
	cpplint main.cpp