CC=gcc
CFLAGS=-Wall -O2 -g -pedantic
CXXFLAGS=$(CFLAGS)
LDFLAGS=-lncurses

PROGRAM=pendu
SOURCES=anim.c fichiers.c modifications.c pendu.c
OBJS=$(SOURCES:.c=.o)

all:	$(PROGRAM)

pendu: $(OBJS)
	$(CC) -o $(PROGRAM) $(OBJS) $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

distclean:  clean
	$(RM) $(PROGRAM)
