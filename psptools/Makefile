CC = gcc
CFLAGS = -Wall -O2 -g -pedantic -ansi -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE
CXXFLAGS = $(CFLAGS)
INCLUDES =
LDFLAGS =

PROGRAMS = pbp-pack pbp-unpack popstrip
SOURCES = pbp-pack.c pbp-unpack.c popstrip.c
OBJS = $(SOURCES:.c=.o)

RM = /bin/rm

all:	$(PROGRAMS)

pbp-pack:	pbp-pack.o
	$(CC) $(LDFLAGS) -o $@ $<

pbp-unpack:	pbp-unpack.o
	$(CC) $(LDFLAGS) -o $@ $<

popstrip:	popstrip.o
	$(CC) $(LDFLAGS) -lz -o $@ $<

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) -f $(OBJS) *~
	
distclean:	clean
	$(RM) -f $(PROGRAMS)
