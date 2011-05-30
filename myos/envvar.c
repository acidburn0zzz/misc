#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include "envvar.h"

typedef struct envvar_s envvar_t;

envvar_t *addEnv(char *str, envvar_t *list) {
    envvar_t *newVar, *cur, *prev;

    newVar = malloc(sizeof(envvar_t));

    strtok(str, " ");

    strcpy(newVar->key, str);
    strcpy(newVar->val, str + strlen(str) + 1);
    newVar->next = NULL;

    if (list == NULL) {
        list = newVar;
        return list;
    }

    cur = list;
    if (strcmp(cur->key, newVar->key) == 0) {
        newVar->next = cur->next;
        free(cur);
        return newVar;
    }

    while (cur != NULL) {
        prev = cur;
        cur = cur->next;

        if (cur && strcmp(cur->key, newVar->key) == 0) {
            newVar->next = cur->next;
            prev->next = newVar;
            free(cur);
            return list;
        }
    }

    prev->next = newVar;

    return list;
}


envvar_t *rmEnv(char *key, envvar_t *list) {
    envvar_t *cur, *prev, *tmp;

    if (list == NULL)
        return NULL;

    /* TODO: I can do better than this */
    if (key == NULL)
        return list;

    cur = list;
    if (strcmp(cur->key, key) == 0) {
        tmp = cur->next;
        free(cur);
        return tmp;
    }

    prev = cur;
    cur = cur->next;

    while (cur) {
        if (strcmp(cur->key, key) == 0) {
            prev->next = cur->next;
            free(cur);

            return list;
        }
        
        prev = cur;
        cur = cur->next;
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

void cleanEnv(envvar_t *list) {
    envvar_t *cur, *next;

    cur = list;
    while (cur != NULL) {
        next = cur->next;
        free(cur);
        cur = next;
    }
}
