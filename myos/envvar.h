#ifndef __ENVVAR_H__
#define __ENVVAR_H__

struct envvar_s {
    char key[256];
    char val[256];
    struct envvar_s *next;
};

struct envvar_s *addEnv(char *str, struct envvar_s *list);
struct envvar_s *rmEnv(char *key, struct envvar_s *list);
char *getEnv(char *key, struct envvar_s *list);
void cleanEnv(struct envvar_s *list);

#endif
