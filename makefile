CFLAGS = -std=c99 -g -pedantic -Wall
CC = gcc

all: buildidx key2val val2key actors

util.o: util.c util.h
	$(CC) $(CFLAGS) -c util.c -o util.o

hashfn.o: hashfn.c hashfn.h
	$(CC) $(CFLAGS) -c hashfn.c -o hashfn.o

buildidx.o: buildidx.c
	$(CC) $(CFLAGS) -c buildidx.c -o buildidx.o

buildidx: buildidx.o util.o hashfn.o
	$(CC) $(CFLAGS) util.o hashfn.o buildidx.o -o buildidx

key2val.o: key2val.c
	$(CC) $(CFLAGS) -c key2val.c -o key2val.o

key2val: key2val.o util.o hashfn.o
	$(CC) $(CFLAGS) util.o hashfn.o key2val.o -o key2val

val2key.o: val2key.c
	$(CC) $(CFLAGS) -c val2key.c -o val2key.o

val2key: val2key.o util.o hashfn.o
	$(CC) $(CFLAGS) util.o hashfn.o val2key.o -o val2key

actors.o: actors.c
	$(CC) $(CFLAGS) -c actors.c -o actors.o

actors: actors.o util.o hashfn.o
	$(CC) $(CFLAGS) util.o hashfn.o actors.o -o actors

clean:
	rm *.o buildidx key2val val2key
