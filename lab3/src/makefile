CC=gcc
CFLAGS=-I. -std=c99


all : utils.o find_min_max.o sequential_min_max parallel_min_max 
	$(CC) -o task5 task5.c $(CFLAGS)

sequential_min_max : sequential_min_max.o utils.o find_min_max.o utils.h find_min_max.h
	$(CC) -o sequential_min_max find_min_max.o utils.o sequential_min_max.o $(CFLAGS)

parallel_min_max : utils.o find_min_max.o utils.h find_min_max.h parallel_min_max.o
	$(CC) -o parallel_min_max utils.o find_min_max.o parallel_min_max.o $(CFLAGS)

parallel_min_max.o : 
	$(CC) -o parallel_min_max.o -c parallel_min_max.c $(CFLAGS)

sequential_min_max.o : 
	$(CC) -o sequential_min_max.o -c sequential_min_max.c $(CFLAGS)

utils.o : utils.h
	$(CC) -o utils.o -c utils.c $(CFLAGS)

find_min_max.o : utils.h find_min_max.h
	$(CC) -o find_min_max.o -c find_min_max.c $(CFLAGS)

clean :
	rm utils.o find_min_max.o sequential_min_max parallel_min_max
