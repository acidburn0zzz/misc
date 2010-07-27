CC=gcc
CFLAGS=-Wall -O2 -pedantic -ansi -g
CXXFLAGS=$(CFLAGS)
LDFLAGS=-lsqlite3 -lpcre -lFLAC -lid3

PROGRAM=scanner
SOURCES=main.c
OBJS=$(SOURCES:.c=.o)

all:	$(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CC) $(CFLAGS) -o $(PROGRAM) $(OBJS) $(LDFLAGS)

.c.o :
	$(CC) $(CFLAGS) -c -o $@ $<

install:
	$(INSTALL) -D $(PROGRAM) $(PREFIX)/bin/$(PROGRAM)

clean:
	$(RM) $(OBJS) *~

distclean:	clean
	$(RM) $(PROGRAM)
