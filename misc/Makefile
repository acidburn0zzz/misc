CC = gcc
CXX = g++
CFLAGS = -Wall -O2 -g -pedantic -ansi
CXXFLAGS = $(CFLAGS)

PROGRAMS = bin2iso chuck_norris_facts fgets fibonacci quicksort shacrypt wii_key_gen

all:	$(PROGRAMS)

bin2iso:	bin2iso.c
	$(CC) $(CFLAGS) $< -o $@

chuck_norris_facts:	chuck_norris_facts.c
	$(CC) $(CFLAGS) $< -o $@

fgets:	fgets.c
	$(CC) $(CFLAGS) $< -o $@

fibonacci:	fibonacci.c
	$(CC) $(CFLAGS) $< -o $@

quicksort:	quicksort.c
	$(CC) $(CFLAGS) $< -o $@

shacrypt:	shacrypt.c
	$(CC) $(CFLAGS) -I../libsha2 $< -o $@ -L../libsha2 -lsha2

wii_key_gen:	wii_key_gen.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm $(PROGRAMS)
