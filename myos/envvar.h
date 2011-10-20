#ifndef __ENVVAR_H__
#define __ENVVAR_H__

struct item_s {
    char *key;
    void *val;
    struct item_s *next;
};

typedef struct item_s item_t;

struct item_s *add_item(struct item_s *list, char *key, void *value);
struct item_s *del_item(struct item_s *list, char *key);
void *get_item(struct item_s *list, char *key);
void delete_list(struct item_s *list);

#endif
