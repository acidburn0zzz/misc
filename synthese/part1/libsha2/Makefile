CC=gcc
CFLAGS=-Wall -O2 -s -pedantic -ansi -static -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE
AR=ar rcs

LIB=libsha2.a
LIBSRC=sha2.c
LIBOBJ=$(LIBSRC:.c=.o)

all:	libsha2.a

libsha2.a: $(LIBOBJ)
	$(AR) libsha2.a $(LIBOBJ)

.c.o:
	$(CC) $(CFLAGS) -static -c $< -o $@

clean:
	rm $(LIBOBJ)

distclean:	clean
	rm libsha2.a
