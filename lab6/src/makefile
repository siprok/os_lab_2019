CC=gcc
CFLAGS=-I.

server: server.c include/libhead.a
	$(CC) -o server.o -c server.c $(CFLAGS)
	$(CC) -o server server.o -Linclude -lhead -pthread  $(CFLAGS)

client: client.c include/libhead.a
	$(CC) -o client.o -c client.c $(CFLAGS)
	$(CC) -o client client.o -Linclude -lhead $(CFLAGS)

clean:
	rm client server client.o server.o 
