CC=g++
CFLAGS=-Wall -W -O2 -pedantic -ansi
CXXFLAGS=$(CFLAGS)
INCLUDES=
LDFLAGS=

#CFLAGS+=-g -DDEBUG
CFLAGS+=-s

PROGRAM=poker
SOURCES=main.cpp carte.cpp jeu.cpp paquet.cpp simulation.cpp
OBJS=$(SOURCES:.cpp=.o)

RM=rm
INSTALL=install
PREFIX=/usr/local

all:	$(PROGRAM)

poker:  $(OBJS)
	$(CC) -o $(PROGRAM) $(OBJS) $(LDFLAGS)

.cpp.o:
	$(CC) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

install:
	$(INSTALL) -D $(PROGRAM) $(PREFIX)/bin/$(PROGRAM)

clean:
	$(RM) -f $(OBJS)

distclean:  clean
	$(RM) -f $(PROGRAM)
