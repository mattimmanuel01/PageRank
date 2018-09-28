CC=gcc
CFLAGS=-Wall -Werror -g
LIBS= set.o graph.o 
TESTS=ts tq tk tg

readData : readData.o $(LIBS)
	gcc -g -o readData readData.o $(LIBS) -lcurl

set.o : set.c set.h
graph.o : graph.c graph.h
