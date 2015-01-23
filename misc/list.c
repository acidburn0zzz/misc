#include <stdio.h>
#include <stdlib.h>

typedef struct element {
    int value;
    struct element *next;
} Element;

typedef struct list {
    int count;
    Element *first;
    Element *last;
} List;

typedef union foo {
    unsigned int i[2];
    unsigned long l;
} Union;

void list_init(List *list) {
    list->count = 0;
    list->first = NULL;
    list->last = NULL;
}

void list_free(List *list) {
    Element *elem, *tmp;
    elem = list->first;

    while (elem != NULL) {
        tmp = elem;
        elem = elem->next;
        free(tmp);
    }

    free(list);
}


void list_prepend(List *list, int value) {
    Element *elem;

    elem = malloc(sizeof(Element));
    elem->value = value;

    if (list->count == 0) {
        elem->next = NULL;
        list->first = elem;
        list->last = elem;
    } else {
        elem->next = list->first;
        list->first = elem;
    }

    list->count++;
}

void list_append(List *list, int value) {
    Element *elem;

    elem = malloc(sizeof(Element));
    elem->value = value;

    if (list->count == 0) {
        elem->next = NULL;
        list->first = elem;
        list->last = elem;
    } else {
        list->last->next = elem;
        list->last = elem;
    }

    list->count++;
}

void list_print(List *list) {
    Element *elem = list->first;

    while (elem != NULL) {
        printf("%d ", elem->value);
        elem = elem->next;
    }

    printf("\n");
}

void list_remove(List *list, int pos) {
    Element *elem, *tmp;

    if (list->count <= pos) {
        return;
    }

    if (pos == 0) {
        tmp = list->first;
        list->first = list->first->next;
        free(tmp);
    } else {
        elem = list->first;

        while (--pos) {
            elem = elem->next;
        }

        tmp = elem->next;
        elem->next = tmp->next;
        free(tmp);
    }

    list->count--;
}

int main(int argc, char **argv) {
    List *list = malloc(sizeof(List));
    Union u;

    list_init(list);

    list_append(list, 2);
    list_append(list, 3);
    list_append(list, 4);
    list_prepend(list, 1);

    list_print(list);
    printf("%d\n", list->count);

    list_remove(list, 0);
    list_remove(list, 1);

    list_print(list);
    printf("%d\n", list->count);

    list_remove(list, 0);
    list_remove(list, 0);

    list_prepend(list, 42);
    list_print(list);
    /*list_remove(list, 0);*/
    /*list_print(list);*/
    list_free(list);

    u.i[0] = 0xffffffff;
    u.i[1] = 0xffffffff;
    printf("%lu\n", u.l);

    return 0;
}
