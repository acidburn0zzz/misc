#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include "envvar.h"

void free_item(item_t *item) {
	free(item->key);
	free(item->val);
	free(item);
}

item_t *add_item(item_t *list, char *key, void *value) {
    item_t *new_item, *cur, *prev;

    new_item = malloc(sizeof(item_t));

    new_item->key = key;
    new_item->val = value;
    new_item->next = NULL;

    if (list == NULL) {
        list = new_item;
        return list;
    }

    cur = list;
    if (strcmp(cur->key, new_item->key) == 0) {
        new_item->next = cur->next;
        free_item(cur);
        return new_item;
    }

    while (cur != NULL) {
        prev = cur;
        cur = cur->next;

        if (cur && strcmp(cur->key, new_item->key) == 0) {
            new_item->next = cur->next;
            prev->next = new_item;
            free_item(cur);
            return list;
        }
    }

    prev->next = new_item;

    return list;
}


item_t *del_item(item_t *list, char *key) {
    item_t *cur, *prev, *tmp;

    if (list == NULL)
        return NULL;

    /* TODO: I can do better than this */
    if (key == NULL)
        return list;

    cur = list;
    if (strcmp(cur->key, key) == 0) {
        tmp = cur->next;
        free_item(cur);
        return tmp;
    }

    prev = cur;
    cur = cur->next;

    while (cur) {
        if (strcmp(cur->key, key) == 0) {
            prev->next = cur->next;
            free_item(cur);

            return list;
        }
        
        prev = cur;
        cur = cur->next;
    }

    return list;
}

void *get_item(item_t *list, char *key) {
    while (list != NULL) {
        if (strcmp(list->key, key) == 0)
            return list->val;
        list = list->next;
    }

    return NULL;
}

void delete_list(item_t *list) {
    item_t *cur, *next;

    cur = list;
    while (cur != NULL) {
        next = cur->next;
        free_item(cur);
        cur = next;
    }
}
