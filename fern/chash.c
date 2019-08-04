
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "chash.h"
#include "defs.h"

/**
 * @defgroup dict dict
 * @brief Dictionary / Hash Table
 */

static int primes[] = { 3, 5, 11, 23, 47, 97,
    191, 383, 769, 1531, 3067, 6143, 12289,
    24571, 49157, 98299, 196613, 393209,
    786431, 1572869, 3145721
};

typedef struct dict_entry dict_entry;
/**
 * @brief Dictionary entry
 *
 * @memberof dict
 * @ingroup dict
 *
 * @private
 *
 */
struct dict_entry {            /* table entry: */
    dict_entry *next;           /* next entry in chain */
    char *name;                 /* defined name */
    char *data;                 /* replacement text */
};
/**
 * @brief Dictionary or Hash Table
 *
 * @memberof dict
 * @ingroup dict
 *
 */
struct dict {
    int hashsize;    /**< \private */
    int used;        /**< \private */
    float resize;     /**< \private */
    dict_entry **hashtab; /**< \private */
};

/**
 * @brief Find prime integer larger than input
 *
 * @memberof dict
 * @private
 *
 * @param v  value to check
 *
 * @return smallest prime number larger than v
 *
 */
int
prime_larger(int v) {
    int i = 0;
    while (primes[i] <= v) {
        i++;
    }
    return primes[i];
}

/**
 * @brief hash function: form hash value for string s 
 *
 * @memberof dict
 * @ingroup dict
 *
 * @param d  dict
 * @param s  value to hash
 *
 * @return hashed version of s
 *
 */
unsigned int
hash(dict * d, char *s) {
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval % d->hashsize;
}

/**
 * @brief Allocate a number of dict entries
 *
 * @memberof dict
 * @ingroup dict
 * @private
 *
 * @param size number of entries
 *
 * @return array of allocated dict_entry's
 *
 */
dict_entry **
dict_entry_alloc(int size) {
    dict_entry **de = (dict_entry **) malloc(sizeof(dict_entry *) * size);
    memset(de, 0, sizeof(dict_entry *) * size);
    return de;
}

/**
 * @brief Create a new dict with an initial length
 *
 * @memberof dict
 * @ingroup dict
 *
 * @param n   Initial length of the dict
 *
 * @return new dict
 *
 */
dict *
dict_new_with_length(int n) {
    dict *d;
    d = (dict *) malloc(sizeof(dict));
    if (d) {
        d->used = 0;
        d->hashsize = prime_larger(n);
        d->resize = 0.80;
        d->hashtab = dict_entry_alloc(d->hashsize);
    }
    return d;
}
/**
 * @brief Crate a new dict
 *
 * @memberof dict
 * @ingroup dict
 *
 * @return new dict
 *
 * @note Initial size is 10
 *
 */
dict *
dict_new() {
    return dict_new_with_length(10);
}

/**
 * @brief Set the resize limit for a dict
 *
 * @memberof dict
 * @ingroup dict
 *
 * @param d dict
 *
 * @param limit size between 0.0 and 1.0 when to resize to dict
 *
 * 
 */
void
dict_resize_limit(dict * d, float limit) {
    if (d && limit > 0.0) {
        d->resize = limit;
    }
}

/**
 * @brief Free a dict
 *
 * @memberof dict
 * @ingroup dict
 *
 * @param d          dict to free
 * @param free_data  function to free the underlying values
 *
 */
void
dict_free(dict * d, void (*free_data) (void *)) {
    int i;
    dict_entry *e, *p;
    for (i = 0; i < d->hashsize; i++) {
        e = d->hashtab[i];
        while (e) {
            p = e;
            e = e->next;
            if (p->data && free_data) {
                free_data(p->data);
                p->data = NULL;
            }
            FREE(p->name);
            FREE(p);
        }
        d->hashtab[i] = NULL;
    }
    FREE(d->hashtab);
    FREE(d);
}

/**
 * @brief Remove an entry from a dict
 *
 * @memberof dict
 * @ingroup dict
 *
 * @param d          dict
 * @param s          key to remove
 * @param free_data  function to free the entry
 *
 * @return 1 on success, 0 on failure
 *
 */
int
dict_remove(dict * d, char *s, void (*free_data) (void *)) {
    unsigned int h;
    dict_entry *np, *last;
    last = NULL;
    h = hash(d, s);

    for (np = d->hashtab[h]; np != NULL; np = np->next) {
        if (strcmp(s, np->name) == 0) {
            break;
        }
        last = np;
    }
    if (np == NULL) {
        return 0;
    }
    if (last == NULL) {
        d->hashtab[h] = np->next;
    } else {
        last->next = np->next;
    }
    if (np->data && free_data) {
        free_data(np->data);
        np->data = NULL;
    }
    FREE(np->name);
    FREE(np);
    return 1;
}

/**
 * @brief Resize a dict
 *
 * @memberof dict
 * @ingroup dict
 *
 * @private
 *
 * @param d  dict to resize
 *
 */
void
dict_resize(dict * d) {
    int i, n;
    dict_entry **oldhash, *e, *p;

    oldhash = d->hashtab;
    n = d->hashsize;

    d->hashsize = prime_larger(d->hashsize);
    d->hashtab = dict_entry_alloc(d->hashsize);

    d->used = 0;
    for (i = 0; i < n; i++) {
        e = oldhash[i];
        while (e) {
            dict_put(d, e->name, e->data);
            p = e;
            e = e->next;
            FREE(p->name);
            FREE(p);
        }
    }
    FREE(oldhash);
}

/**
 * @brief Find a key in a dict
 *
 * @memberof dict
 * @ingroup dict
 *
 * @private
 *
 * @param d  dict to search in
 * @param key key to search for
 *
 * @return entry if found, NULL if not found
 *
 */
dict_entry *
dict_get_internal(dict * d, char *s) {
    dict_entry *np;
    for (np = d->hashtab[hash(d, s)]; np != NULL; np = np->next) {
        if (strcmp(s, np->name) == 0) {
            return np;          /* found */
        }
    }
    return NULL;                /* not found */
}

/**
 * @brief Print status of dict, with size and number occupied
 *
 * @memberof dict
 * @ingroup dict
 *
 * @param d  dict to get status of
 *
 */
void
dict_status(dict *d) {
    if(d) {
        fprintf(stdout, "Dict: %p Size: %d Used: %d\n", d, d->hashsize, d->used);
    }
}

/**
 * @brief Get an entry from a dict
 *
 * @memberof dict
 * @ingroup dict
 *
 * @param d     dict to search
 * @param s     key to search for
 *
 * @return value associated with key if found, NULL if not found
 *
 * @warning memory is owned by the dict and should not be modified
 *
 */
void *
dict_get(dict * d, char *s) {
    dict_entry *np;
    if (!d) {
        fprintf(stderr, "dictionary not defined\n");
        return NULL;
    }
    if ((np = dict_get_internal(d, s))) {
        return np->data;
    }
    return NULL;
}

/**
 * @brief Insert an key/value in a dict
 *
 * @memberof dict
 * @ingroup dict
 *
 * @param d     dict to insert key/value into
 * @param name  name of the entry
 * @param data  pointer to data to store
 *
 * @return 1 on success, 0 in failure
 *
 * @warning data ownership is transfered to the dict
 * @warning name is cloned
 *
 */
int
dict_put(dict * d, char *name, void *data) {
    dict_entry *np;
    unsigned hashval;
    if ((np = dict_get_internal(d, name)) == NULL) {    /* not found */
        np = (dict_entry *) malloc(sizeof(*np));
        if (np == NULL || (np->name = strdup(name)) == NULL)
            return 0;
        hashval = hash(d, name);
        np->next = d->hashtab[hashval];
        d->hashtab[hashval] = np;
    } else {                    /* already there */
        //free((void *) np->data); /*free previous data */
    }
    if (data) {
        np->data = data;
    } else {
        return 0;
    }
    d->used++;
    if ((float) d->used / (float) d->hashsize > d->resize) {
        dict_resize(d);
    }
    return 1;
}

/**
 * @brief Get keys for all entries
 *
 * @memberof dict
 * @ingroup dict
 *
 * @param d dict to get keys for
 *
 * @return all keys assocaited with dict
 *
 * @warning keys are owned by the user and the user is responsible for freeing the
 *    underlying memory with \ref dict_keys_free
 * @note collection of keys is terminated with a NULL value
 */
char **
dict_keys(dict * d) {
    int i, j;
    dict_entry *e;
    char **keys;
    if (!d) {
        return NULL;
    }
    keys = (char **) malloc(sizeof(char *) * (d->used + 1));
    for (j = 0, i = 0; i < d->hashsize; i++) {
        e = d->hashtab[i];
        while (e) {
            keys[j++] = strdup(e->name);
            e = e->next;
        }
    }
    keys[j] = NULL;
    return keys;
}

/**
 * @brief Free a collection of keys
 *
 * @memberof dict
 *
 * @param keys  key collection to free
 * 
 */
void
dict_keys_free(char **keys) {
    if(keys) {
        for(size_t i = 0; keys[i]; i++) {
            FREE(keys[i]);
        }
        FREE(keys);
    }
}


#ifdef __TESTING__

int
main() {
    char c;
    dict *d;
    char s[10], s2[3], *v;

    d = dict_new();
    dict_put(d, "a", "a:data");
    dict_put(d, "b", "b:data");
    dict_put(d, "c", "c:data");
    dict_put(d, "d", "d:data");
    dict_put(d, "e", "e:data");
    dict_put(d, "f", "f:data");
    dict_put(d, "g", "g:data");
    dict_put(d, "h", "h:data");
    dict_put(d, "i", "i:data");

    for (c = 'a'; c <= 'j'; c++) {
        sprintf(s, "%c:data", c);
        sprintf(s2, "%c", c);
        if ((v = (char *) dict_get(d, s2))) {
            if (strcmp(v, s) != 0) {
                fprintf(stderr, "%c %s != %s\n", c, v, s);
            }
        } else {
            fprintf(stderr, "%c does not exist\n", c);
        }
    }

    dict_remove(d, "e", NULL);

    for (c = 'a'; c <= 'j'; c++) {
        sprintf(s, "%c:data", c);
        sprintf(s2, "%c", c);
        if ((v = (char *) dict_get(d, s2))) {
            if (strcmp(v, s) != 0) {
                fprintf(stderr, "%c %s != %s\n", c, v, s);
            }
        } else {
            fprintf(stderr, "%c does not exist\n", c);
        }
    }

    v = strdup("aaa:data");

    dict_put(d, "a", v);
    fprintf(stderr, "a: %s\n", (char *) dict_get(d, "a"));

    dict_remove(d, "a", free);

    dict_free(d, NULL);
    return 0;
}


#endif /* __TESTING__ */
