CC=gcc
CFLAGS=-O2 -g -Wall -ansi -pedantic

all : id3v1

id3v1 : main.o
	$(CC) $(CFLAGS) -o id3v1 main.o

main.o : main.c
	$(CC) $(CFLAGS) -c -o main.o main.c

clean :
	rm -rf *.o

distclean : clean
	rm -rf id3v1
