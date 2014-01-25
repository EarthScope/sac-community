
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "chash.h"


#define FREE(x) do {  \
  if(x) {             \
    free(x);          \
    x = NULL;         \
  }                   \
} while(0);

#ifdef __TESTING__
#define DEBUG(fmt, args...) debug("%s:%d "fmt, __FUNCTION__, __LINE__, ##args)
void
debug(char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);
}
#else 
#define DEBUG(fmt, args...)
#endif

int primes[] = { 3,5,11,23,47,97,
                 191,383,769,1531,3067,6143,12289,
                 24571,49157,98299,196613,393209,
                 786431,1572869,3145721
};


typedef struct _dict_entry dict_entry;
struct _dict_entry { /* table entry: */
    dict_entry *next; /* next entry in chain */
    char *name; /* defined name */
    char *data; /* replacement text */
};
struct _dict {
  int    hashsize;
  int    used;
  float  resize;
  dict_entry **hashtab;
};

int
prime_larger(int v) {
  int i = 0;
  while(primes[i] <= v) { i++; }
  return primes[i];
}

/* hash: form hash value for string s */
unsigned int
hash(dict *d, char *s) {
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++)
      hashval = *s + 31 * hashval;
    return hashval % d->hashsize;
}

dict_entry **
dict_entry_alloc(int size) {
  dict_entry **de = (dict_entry **) malloc(sizeof(dict_entry *) * size);
  memset(de, 0, sizeof(dict_entry*) * size);
  return de;
}

dict *
dict_new() {
  dict *d;
  DEBUG("\n");
  d = (dict *) malloc(sizeof(dict));
  if(d) {
    d->used     = 0;
    d->hashsize = prime_larger(10); 
    d->resize   = 0.80;
    d->hashtab  = dict_entry_alloc(d->hashsize);
  }
  return d;
}

void
dict_resize_limit(dict *d, float limit) {
  if(d && limit > 0.0) {
    d->resize = limit;
  }
}

void
dict_free(dict *d, void (*free_data)(void *)) {
  int i;
  dict_entry *e, *p;
  for(i = 0; i < d->hashsize; i++) {
    e = d->hashtab[i];
    while(e) {
      p = e;
      e = e->next;
      if(p->data && free_data) {
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

int
dict_remove(dict *d, char *s, void (*free_data)(void *)) {
  unsigned int h;
  dict_entry *np, *last;
  last = NULL;
  h = hash(d,s);

  DEBUG("remove: '%s'\n", s);
  for(np = d->hashtab[h]; np != NULL; np = np->next) {
    if(strcmp(s, np->name) == 0) {
      break;
    }
    last = np;
  }
  if(np == NULL) {
    return 0;
  }
  if(last == NULL) {
    d->hashtab[h] = np->next;
  } else {
    last->next = np->next;
  }
  if(np->data && free_data) {
    free_data(np->data);
    np->data = NULL;
  }
  FREE(np->name);
  FREE(np);
  return 1;
}

void
dict_resize(dict *d) {
  int i, n;
  dict_entry **oldhash, *e, *p;

  DEBUG("\n");
  oldhash = d->hashtab;
  n       = d->hashsize;

  DEBUG("used: %f (%d/%d)\n", (float)d->used / (float)d->hashsize, d->used, d->hashsize);
    
  d->hashsize = prime_larger(d->hashsize);
  d->hashtab  = dict_entry_alloc(d->hashsize);

  d->used = 0;
  for(i = 0; i < n; i++) {
    e = oldhash[i];
    while(e) {
      dict_put(d, e->name, e->data);
      p = e;
      e = e->next;
      FREE(p->name);
      FREE(p);
    }
  }
  DEBUG("used: %f (%d/%d) [ resized ]\n", (float)d->used / (float)d->hashsize, d->used, d->hashsize);
  FREE(oldhash);
}


/* lookup: look for s in hashtab */
dict_entry *
dict_get_internal(dict *d, char *s)
{
    dict_entry *np;
    for (np = d->hashtab[hash(d,s)]; np != NULL; np = np->next) {
      if (strcmp(s, np->name) == 0) {
        return np; /* found */
      }
    }
    return NULL; /* not found */
}

void *
dict_get(dict *d, char *s) {
  dict_entry *np;
  if(!d) {
    fprintf(stderr, "dictionary not defined\n");
    return NULL;
  }
  if((np = dict_get_internal(d,s))) {
    return np->data;
  }
  return NULL;
}

int
dict_put(dict *d, char *name, void *data) {
    dict_entry *np;
    unsigned hashval;
    if ((np = dict_get_internal(d, name)) == NULL) { /* not found */
      np = (dict_entry *) malloc(sizeof(*np));
      if (np == NULL || (np->name = strdup(name)) == NULL)
        return 0;
      DEBUG("add: '%s' '%p'\n", name, data);
      hashval = hash(d, name);
      np->next = d->hashtab[hashval];
      d->hashtab[hashval] = np;
    } else { /* already there */
      DEBUG("rep: '%s' '%p'\n", name, data);
      //free((void *) np->data); /*free previous data */
    }
    if(data) {
      DEBUG("set: '%s' '%p'\n", name, data);
      np->data = data;
    } else {
      return 0;
    }
    d->used++;
    if((float) d->used  / (float)d->hashsize > d->resize) {
      dict_resize(d);
    }
    return 1;
}

char ** 
dict_keys(dict *d) {
  int i, j;
  dict_entry *e;
  char **keys;
  if(!d) {
    return NULL;
  }
  keys = (char **) malloc(sizeof(char *) * (d->used + 1));
  for(j = 0, i = 0; i < d->hashsize; i++) {
    e = d->hashtab[i];
    while(e) {
      keys[j++] = strdup(e->name);
      e = e->next;
    }
  }
  keys[j] = NULL;
  return keys;
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

  for(c = 'a'; c <= 'j'; c++) {
    sprintf(s, "%c:data", c);
    sprintf(s2, "%c", c);
    if((v = (char *)dict_get(d,s2))) {
      if(strcmp(v, s) != 0) {
        fprintf(stderr, "%c %s != %s\n", c, v, s);
      }
    } else {
      fprintf(stderr, "%c does not exist\n", c);
    }
  }

  dict_remove(d, "e", NULL);

  for(c = 'a'; c <= 'j'; c++) {
    sprintf(s, "%c:data", c);
    sprintf(s2, "%c", c);
    if((v = (char *)dict_get(d,s2))) {
      if(strcmp(v, s) != 0) {
        fprintf(stderr, "%c %s != %s\n", c, v, s);
      }
    } else {
      fprintf(stderr, "%c does not exist\n", c);
    }
  }

  v = strdup("aaa:data");

  dict_put(d, "a", v);
  fprintf(stderr, "a: %s\n", (char *) dict_get(d,"a"));
  
  dict_remove(d, "a", free);

  dict_free(d, NULL);
  return 0;
}

#endif /* __TESTING__ */
