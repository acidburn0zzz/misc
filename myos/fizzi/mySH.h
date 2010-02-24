#ifndef __MYSH_H__
#define __MYSH_H__

char verbose;

void shhelp();
int interpret(char *input, char *memory[]);
int showprompt();
void mysh(int bose);

#endif
