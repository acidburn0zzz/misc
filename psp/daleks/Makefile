CC=gcc
CFLAGS=-Wall -O2 -g -pedantic -ansi
CXXFLAGS=$(CFLAGS)

PROGRAM=daleks
SOURCES=main.c touches.c graphics.c partie.c dalek.c doc.c
OBJS=$(SOURCES:.c=.o)

INCLUDES=
LDFLAGS=-lSDL -lSDL_image -lSDL_ttf

all:	$(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CC) -o $(PROGRAM) $(OBJS) $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

distclean:  clean
	$(RM) $(PROGRAM)
