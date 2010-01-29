CC=gcc
CFLAGS=-Wall -O2 -g -pedantic -ansi
CXXFLAGS=$(CFLAGS)

PROGRAM=myos
SOURCES=main.c boot.c envvar.c panic.c shell.c
OBJS=$(SOURCES:.c=.o)

INCLUDES=
LDFLAGS=

RM=rm -f

all:	$(PROGRAM)

myos: $(OBJS)
	$(CC) -o $(PROGRAM) $(OBJS) $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJS)

distclean:  clean
	$(RM) $(PROGRAM)
