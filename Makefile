CC=gcc
CFLAGS=-Wall -Werror -g
LIBS= set.o graph.o readData.o
MIBS= set.o
TESTS=ts tq tk tg
searchPagerank : searchPagerank.o $(MIBS)
	gcc -g -o searchPagerank searchPagerank.o $(MIBS) -lcurl


pagerank : pagerank.o $(LIBS)
	gcc -g -o pagerank pagerank.o $(LIBS) -lcurl


searchPagerank : searchPagerank.o $(MIBS)
	gcc -g -o searchPagerank searchPagerank.o $(MIBS) -lcurl


set.o : set.c set.h
graph.o : graph.c graph.h
readData.o : readData.c readData.h
