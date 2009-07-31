CC=gcc
CFLAGS=-Wall -O2 -pedantic -ansi -g -I../libs/libcrc32
CXXFLAGS=$(CFLAGS)
LDFLAGS=-L../libs/libcrc32 -lsqlite3

PROGRAM=mp3_scanner
SOURCES=main.c id3.c
OBJS=$(SOURCES:.c=.o)

all:	$(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CC) $(CFLAGS) -o $(PROGRAM) $(OBJS) $(LDFLAGS)

.c.o :
	$(CC) $(CFLAGS) -c -o $@ $<

install:
	$(INSTALL) -D $(PROGRAM) $(PREFIX)/bin/$(PROGRAM)

clean:
	$(RM) $(OBJS) *~ *core

distclean:	clean
	$(RM) $(PROGRAM)
