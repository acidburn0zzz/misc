CC = g++
CFLAGS = -Wall -W -O2 -pedantic -ansi
CXXFLAGS = $(CFLAGS)
INCLUDES =
LDFLAGS =

#CFLAGS += -g -DDEBUG
CFLAGS += -s

PROGRAM = poker
SOURCES = main.cpp carte.cpp jeu.cpp paquet.cpp simulation.cpp
OBJS = $(SOURCES:.cpp=.o)

RM = /bin/rm
INSTALL = /usr/bin/install
PREFIX = /usr/local

all:	$(PROGRAM)

poker: $(OBJS)
	$(CC) $(LDFLAGS) -o $(PROGRAM) $(OBJS)

.cpp.o:
	$(CC) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

install:
	strip $(PROGRAM)
	upx -9 $(PROGRAM)
	$(INSTALL) -D $(PROGRAM) $(PREFIX)/bin/$(PROGRAM)

clean:
	$(RM) -f $(OBJS) $(PROGRAM) *~ *core
