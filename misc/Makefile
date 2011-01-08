CC = gcc
CXX = g++
CFLAGS = -Wall -O2 -g -pedantic
CXXFLAGS = $(CFLAGS)

PROGRAMS = bin2iso chuck_norris_facts format fibonacci ips keygen millis quicksort

all: $(PROGRAMS)

$(PROGRAMS): %: %.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(PROGRAMS)
