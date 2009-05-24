CC=gcc
CFLAGS=-Wall -O2 -s -pedantic -ansi -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE
AR=ar rsc

LIB=libsha1.a
LIBSRC=sha1.c
LIBOBJ=$(LIBSRC:.c=.o)

all:	libsha1.a

libsha1.a: $(LIBOBJ)
	$(AR) libsha1.a $(LIBOBJ)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(LIBOBJ)

distclean:	clean
	rm libsha1.a
