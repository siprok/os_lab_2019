CC=gcc
CFLAGS=-I. -std=c99

all : task1 task3

task1 : find_min_max.o utils.o parallel_min_max.o
	$(CC) -o parallel_min_max parallel_min_max.o find_min_max.o utils.o $(CFLAGS)

task3 : process_memory.o process_memory.c
	$(CC) -o process_memory process_memory.o $(CFLAGS)

utils.o : utils.c utils.h
	$(CC) -c utils.c utils.h $(CFLAGS)

find_min_max.o : find_min_max.c
	$(CC) -c find_min_max.c $(CFLAGS)

parallel_min_max.o : parallel_min_max.c
	$(CC) -c parallel_min_max.c $(CFLAGS)

process_memory.o : process_memory.c
	$(CC) -o process_memory.o -c process_memory.c $(CFLAGS)


clean :
	rm process_memory.o process_memory find_min_max.o utils.o parallel_min_max.o parallel_min_max 
