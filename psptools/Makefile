CC=gcc
CFLAGS=-Wall -Werror -O2 -g -pedantic -ansi -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE
CXXFLAGS=$(CFLAGS)
INCLUDES=
LDFLAGS=-lz

PROGRAMS=pbp-pack pbp-unpack popstrip
SOURCES=pbp-pack.c pbp-unpack.c popstrip.c
OBJS=$(SOURCES:.c=.o)

RM=rm -f

all:	$(PROGRAMS)

pbp-pack:	pbp-pack.o
	$(CC) -o $@ $<

pbp-unpack:	pbp-unpack.o
	$(CC) -o $@ $<

popstrip:	popstrip.o
	$(CC) -o $@ $< $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) *~

distclean:	clean
	$(RM) $(PROGRAMS)
