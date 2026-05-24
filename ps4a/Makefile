# Boukdad ps4a Makefile
CC = g++
CFLAGS = -std=c++17 -Wall -Werror -Wextra -pedantic -g
LIB = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system -lboost_unit_test_framework
DEPS = AniPlayer.hpp Component.hpp KeyFrame.hpp
OBJECTS = AniPlayer.o Component.o KeyFrame.o
PROGRAM = AniPlayer
.PHONY: all clean lint
all: $(PROGRAM) AniPlayer.a
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c $<
$(PROGRAM): main.o $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)
AniPlayer.a: $(OBJECTS)
	ar rcs AniPlayer.a $(OBJECTS)
clean:
	rm -f *.o $(PROGRAM) AniPlayer.a
lint:
	cpplint --filter=-build/include_subdir *.cpp *.hpp
