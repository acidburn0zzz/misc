CC = g++
CFLAGS = -Wall -O2 -s
CXXFLAGS = $(CFLAGS)
INCLUDES =
LDFLAGS =

PROGRAMS = sha256crypt
SOURCES = sha256crypt.cpp
OBJS = $(SOURCES:.cpp=.o) sha2.o

RM = /bin/rm

all:	$(PROGRAMS)

sha256crypt:	sha256crypt.o sha2.o
	$(CC) $(LDFLAGS) -o sha256crypt sha256crypt.o sha2.o

sha2.o:
	$(CC) $(CXXFLAGS) $(INCLUDES) -c ../sha2.c -o $@

.cpp.o:
	$(CC) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) -f $(OBJS) *~
	
distclean:	clean
	$(RM) -f $(PROGRAMS)

#~ pbp-unpack:	pbp-unpack.o
	#~ $(CC) $(LDFLAGS) -o $@ $<
