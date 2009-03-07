CC = gcc
CXX = g++
CFLAGS = -Wall -O2 -s -pedantic -ansi
CXXFLAGS = $(CFLAGS)

PROGRAMS = bin2iso chuck_norris_facts fgets fibonacci wii_key_gen

all:	$(PROGRAMS)

bin2iso:	bin2iso.c
	$(CC) $(CFLAGS) bin2iso.c -o $@

chuck_norris_facts:	chuck_norris_facts.c
	$(CC) $(CFLAGS) chuck_norris_facts.c -o $@

fgets:	fgets.c
	$(CC) $(CFLAGS) fgets.c -o $@

fibonacci:	fibonacci.c
	$(CC) $(CFLAGS) fibonacci.c -o $@

wii_key_gen:	wii_key_gen.c
	$(CC) $(CFLAGS) wii_key_gen.c -o $@

clean:
	rm $(PROGRAMS)
