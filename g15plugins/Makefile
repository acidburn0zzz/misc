CC = gcc
CXX = g++
CFLAGS = -Wall -O2 -g
CXXFLAGS = $(CFLAGS)

PROGRAMS = g15music

all:	$(PROGRAMS)

g15music:	g15music.o pid.o
	$(CC) $(CFLAGS) g15music.o pid.o -o $@ -lg15render -lg15daemon_client -lpcre -lpthread

g15music.o:	g15music.c
	$(CC) $(CFLAGS) -c $< -o $@

pid.o:	pid.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) *.o

distclean:	clean
	$(RM) $(PROGRAMS)
