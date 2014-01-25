
#ifndef _CHASH_H_
#define _CHASH_H_

typedef struct _dict dict;

dict *  dict_new    ( );
void *  dict_get    (dict *d, char *s);
int     dict_put    (dict *d, char *name, void *data);
char ** dict_keys   (dict *d);
int     dict_remove (dict *d, char *s, void (*free_data)(void *));
void    dict_free   (dict *d, void (*free_data)(void *));

#endif /* _CHASH_H_ */
