CC=gcc
CXX=g++
CFLAGS=-g -O2 -pedantic -ansi -Wall -Wshadow -Wpointer-arith -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes
CXXFLAGS=$(CFLAGS)

PROGRAMS=bin2iso chuck_norris_facts dlink format fibonacci ips keygen quicksort

all: $(PROGRAMS)

$(PROGRAMS): %: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(PROGRAMS)
