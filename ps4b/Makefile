CC = g++
CCFLAGS = -std=c++17 -Wall -Werror -Wextra -pedantic -g
LIBS = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system -lboost_unit_test_framework

.PHONY: all clean lint test

all: AniPlayer AniPlayer.a test

# The main executable
AniPlayer: main.o AniPlayer.o Component.o KeyFrame.o
	$(CC) $(CCFLAGS) $^ -o AniPlayer $(LIBS)

# Static library containing class logic
AniPlayer.a: AniPlayer.o Component.o KeyFrame.o
	ar rcs AniPlayer.a $^

# Unit test executable
test: test.o AniPlayer.o Component.o KeyFrame.o
	$(CC) $(CCFLAGS) $^ -o test $(LIBS)

main.o: main.cpp AniPlayer.hpp Component.hpp KeyFrame.hpp
	$(CC) $(CCFLAGS) -c main.cpp

AniPlayer.o: AniPlayer.cpp AniPlayer.hpp Component.hpp KeyFrame.hpp
	$(CC) $(CCFLAGS) -c AniPlayer.cpp

Component.o: Component.cpp Component.hpp KeyFrame.hpp
	$(CC) $(CCFLAGS) -c Component.cpp

KeyFrame.o: KeyFrame.cpp KeyFrame.hpp
	$(CC) $(CCFLAGS) -c KeyFrame.cpp

test.o: test.cpp KeyFrame.hpp
	$(CC) $(CCFLAGS) -c test.cpp

lint:
	cpplint --filter=-build/include_subdir *.cpp *.hpp

clean:
	rm -f *.o AniPlayer AniPlayer.a test