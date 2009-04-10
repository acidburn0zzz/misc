CC = gcc
CFLAGS = -Wall -O2 -pedantic -ansi -g
CXXFLAGS = $(CFLAGS)

PROGRAM = bfint
SOURCES = main.c
OBJS = $(SOURCES:.c=.o)

RM = /bin/rm
INSTALL = /usr/bin/install
PREFIX = /usr/local

all:	$(PROGRAM)

bfint: $(OBJS)
	$(CC) $(CFLAGS) -g -o $(PROGRAM) $(OBJS)

release: $(OBJS)
	$(CC) $(CFLAGS) -s -o $(PROGRAM) $(OBJS)

install:
	$(INSTALL) -D $(PROGRAM) $(PREFIX)/bin/$(PROGRAM)

clean:
	$(RM) -f $(OBJS) *~ *core
