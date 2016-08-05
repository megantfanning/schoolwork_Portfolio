# Megan Fanning
# makefile lab1

CC=g++ -std=c++11
CFLAGS= -o
all: clean gameOfLife.o

gameOfLife.o: gameOfLifeMain.cpp board.o
	${CC} gameOfLifeMain.cpp -o gameOfLife.o

clean:
	rm -rf *.o all
