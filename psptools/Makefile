CC=gcc
CFLAGS=-Wall -O2 -g -pedantic -ansi -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE
CXXFLAGS=$(CFLAGS)
INCLUDES=
LDFLAGS=-lz

PROGRAMS=pbp-pack pbp-unpack popstrip
SOURCES=pbp-pack.c pbp-unpack.c popstrip.c
OBJS=$(SOURCES:.c=.o)

RM=/bin/rm

all:	$(PROGRAMS)

pbp-pack:	pbp-pack.o
	$(CC) -o $@ $<

pbp-unpack:	pbp-unpack.o
	$(CC) -o $@ $<

popstrip:	popstrip.o
	$(CC) -o $@ $< $(LDFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) -f $(OBJS) *~
	
distclean:	clean
	$(RM) -f $(PROGRAMS)
