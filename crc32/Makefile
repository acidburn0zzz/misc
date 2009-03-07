CC = g++
CFLAGS = -Wall -O2 -g -pedantic -ansi -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE
CXXFLAGS = $(CFLAGS)
INCLUDES =
LDFLAGS =

PROGRAM = crc32
SOURCES = main.cpp crc32.cpp sfvfile.cpp
OBJS = $(SOURCES:.cpp=.o)

RM = /bin/rm
INSTALL = /usr/bin/install
PREFIX = /usr/local

all:	$(PROGRAM)

crc32: $(OBJS)
	$(CC) $(LDFLAGS) -o $(PROGRAM) $(OBJS)

.cpp.o:
	$(CC) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

install:
	$(INSTALL) -m 755 $(PROGRAM) $(PREFIX)/bin/$(PROGRAM)

clean:
	$(RM) -f $(OBJS) $(PROGRAM) *~ *core

test:	$(PROGRAM)
#	./$(PROGRAM) -v /home/mathieu/musique/portal_still_alive.wav
	./$(PROGRAM) -v /home/mathieu/midgar/pure_18_-_maddison.wmv
