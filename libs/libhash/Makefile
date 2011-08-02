CC=gcc
CFLAGS=-g -O2 -pedantic -ansi -Wall -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE
CXXFLAGS=$(CFLAGS)
INCLUDES=
LDFLAGS=

PROGRAM=libhash.a
SOURCES=crc32.c md5.c sha1.c sha2.c
OBJS=$(SOURCES:.c=.o)

AR=ar rcs

all:	$(PROGRAM)

$(PROGRAM): $(OBJS)
	$(AR) $(PROGRAM) $(OBJS)

.c.o:
	$(CC) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJS)

distclean:	clean
	$(RM) $(PROGRAM)
