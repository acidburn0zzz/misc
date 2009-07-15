CC=g++
CFLAGS=-Wall -W -O2 -pedantic -ansi
CXXFLAGS=$(CFLAGS)
INCLUDES=
LDFLAGS=

#~ CFLAGS+=-g -DDEBUG
CFLAGS+=-s

PROGRAM=string
SOURCES=main.cpp string.cpp
OBJS=$(SOURCES:.cpp=.o)

all:	$(PROGRAM)

$(PROGRAM):  $(OBJS)
	$(CC) -o $(PROGRAM) $(OBJS) $(LDFLAGS)

.cpp.o:
	$(CC) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJS)

distclean:  clean
	$(RM) $(PROGRAM)
