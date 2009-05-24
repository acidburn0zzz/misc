CC=gcc
CFLAGS=-Wall -O2 -g -pedantic -ansi -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE
CXXFLAGS=$(CFLAGS)
INCLUDES=
LDFLAGS=

PROGRAM=libmd5.a
SOURCES=md5.c
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
