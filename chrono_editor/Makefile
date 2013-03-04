CC=g++
CFLAGS=-pedantic -ansi -Wall -Wshadow -Wpointer-arith -Wcast-qual
CXXFLAGS=$(CFLAGS)
INCLUDES=
LDFLAGS=

CFLAGS+=-g -DDEBUG

PROGRAM=test
SOURCES=test.cpp sramfile.cpp character.cpp game.cpp
OBJS=$(SOURCES:.cpp=.o)

all:	$(PROGRAM)

test:  $(OBJS)
	$(CC) -o $(PROGRAM) $(OBJS) $(LDFLAGS)

.cpp.o:
	$(CC) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJS)

distclean:  clean
	$(RM) $(PROGRAM)
