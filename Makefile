CC=gcc

CFLAGS=-Wall -O2 -std=c99

all: hospital

debug: hospital.dbg

hospital: hospital.o parse.o structure.o
	$(CC) -o hospital hospital.o parse.o structure.o $(CFLAGS)

hospital.o: hospital.c
	$(CC) -c hospital.c $(CFLAGS) 

parse.o: parse.h parse.c
	$(CC) -c parse.c $(CFLAGS)

structure.o: structure.h structure.c
	$(CC) -c structure.c $(CFLAGS)

hospital.dbg: hospital.o parse.o structure.o
	$(CC) -g -o hospital.dbg hospital.o parse.o structure.o $(CFLAGS)

.PHONY: clean

clean:
	-rm -f *.o *.dbg hospital
