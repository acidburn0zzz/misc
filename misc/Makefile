CC = gcc
CXX = g++
CFLAGS = -Wall -O2 -g -pedantic -ansi
CXXFLAGS = $(CFLAGS)

PROGRAMS = bin2iso chuck_norris_facts fgets format fibonacci keygen millis quicksort shacrypt

all:	$(PROGRAMS)

bin2iso:	bin2iso.c
	$(CC) $(CFLAGS) $< -o $@

chuck_norris_facts:	chuck_norris_facts.c
	$(CC) $(CFLAGS) $< -o $@

fgets:	fgets.c
	$(CC) $(CFLAGS) $< -o $@

format:	format.c
	$(CC) $(CFLAGS) $< -o $@

fibonacci:	fibonacci.c
	$(CC) $(CFLAGS) $< -o $@

keygen:	keygen.c
	$(CC) $(CFLAGS) $< -o $@

millis:	millis.c
	$(CC) $(CFLAGS) $< -o $@

quicksort:	quicksort.c
	$(CC) $(CFLAGS) $< -o $@

shacrypt:	shacrypt.c
	$(CC) $(CFLAGS) -I../libs/libhash $< -o $@ -L../libs/libhash -lhash

clean:
	rm -rf $(PROGRAMS)
