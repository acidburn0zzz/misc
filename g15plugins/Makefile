CC = gcc
CXX = g++
CFLAGS = -Wall -g
CXXFLAGS = $(CFLAGS)

PROGRAMS = g15music g15hella

all:	$(PROGRAMS)

g15music:	g15music.o pid.o
	$(CC) $(CFLAGS) g15music.o pid.o -o $@ -lg15render -lg15daemon_client -lpcre -lpthread

g15hella:	g15hella.o pid.o
	$(CC) $(CFLAGS) g15hella.o pid.o -o $@ -lg15render -lg15daemon_client -lpcre -lpthread -lxmlrpc -lxmlrpc_client

g15music.o:	g15music.c
	$(CC) $(CFLAGS) -c $< -o $@

g15hella.o:	g15hella.c
	$(CC) $(CFLAGS) -c $< -o $@

pid.o:	pid.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) *.o

distclean:	clean
	$(RM) $(PROGRAMS)
