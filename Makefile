CC=gcc
CFLAGS=-Wall -g
LOADLIBES=-ljq

all: statj

clean:
	rm -f statj *.o

statj: statj.o
