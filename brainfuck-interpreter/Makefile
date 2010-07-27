CC=gcc
CFLAGS=-Wall -O2 -pedantic -ansi -g
CXXFLAGS=$(CFLAGS)
LDFLAGS=

PROGRAM=bfint
SOURCES=main.c
OBJS=$(SOURCES:.c=.o)

PREFIX=/usr/local

all:	$(PROGRAM)

bfint: $(OBJS)
	$(CC) -o $(PROGRAM) $(OBJS) $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

install:
	$(INSTALL) -D $(PROGRAM) $(PREFIX)/bin/$(PROGRAM)

clean:
	$(RM) $(OBJS) *~
