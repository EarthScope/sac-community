
#ifndef _CHASH_H_
#define _CHASH_H_

typedef struct dict dict;

dict *  dict_new             ( );
dict *  dict_new_with_length (int n);
void *  dict_get             (dict *d, char *s);
int     dict_put             (dict *d, char *name, void *data);
char ** dict_keys            (dict *d);
int     dict_remove          (dict *d, char *s, void (*free_data)(void *));
void    dict_free            (dict *d, void (*free_data)(void *));
void    dict_status          (dict *d);

void    dict_keys_free(char **keys);


#endif /* _CHASH_H_ */
