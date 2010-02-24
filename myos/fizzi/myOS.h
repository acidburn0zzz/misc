#ifndef __MYOS_H__
#define __MYOS_H__

#define RAMSIZE 1000

char *RAM[RAMSIZE];
FILE *HDD;

int boot();
int main(int argc, char **argv);

#endif
