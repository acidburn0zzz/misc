CC=gcc
CFLAGS=-Wall -O2 -g -pedantic -ansi -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE
CXXFLAGS=$(CFLAGS)
INCLUDES=
LDFLAGS=

PROGRAM=libcrc32.a
SOURCES=crc32.c
OBJS=$(SOURCES:.c=.o)

AR=ar rcs
RM=/bin/rm

all:	$(PROGRAM)

$(PROGRAM): $(OBJS)
	$(AR) $(PROGRAM) $(OBJS)

.c.o:
	$(CC) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) -f $(OBJS) *~ *core

distclean:	clean
	$(RM) -f $(PROGRAM)

test:	$(PROGRAM)
	time ./crc32 /home/mathieu/porn/films/galitsin_079_-_tender_touches_alice_krista.avi
