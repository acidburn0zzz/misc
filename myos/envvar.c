#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include "envvar.h"

typedef struct envvar_s envvar_t;

envvar_t *addEnv(char *str, envvar_t *list) {
    envvar_t *newVar = malloc(sizeof(envvar_t));

    strtok(str, " ");

    strcpy(newVar->key, str);
    strcpy(newVar->val, str + strlen(str) + 1);
    newVar->next = NULL;

    if (list == NULL) {
        list = newVar;
    } else {
        envvar_t *tmp = list;
        while (tmp->next != NULL)
            tmp = tmp->next;

        tmp->next = newVar;
    }

    return list;
}

char *getEnv(char *key, envvar_t *list) {
    while (list != NULL) {
        if (strcmp(list->key, key) == 0)
            return list->val;
        list = list->next;
    }

    return NULL;
}
