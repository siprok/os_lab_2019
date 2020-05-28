CC=gcc

lib: header.c
	$(CC) -c header.c header.h
	ar rcs libhead.a header.o

clean:
	 rm header.h.gch header.o libhead.a
