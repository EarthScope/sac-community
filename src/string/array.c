/*
Copyright (c) 2011, Brian Savage
All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, 
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the 
      documentation and/or other materials provided with the distribution.
    * The names of the contributors may be used to endorse or promote products 
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
POSSIBILITY OF SUCH DAMAGE.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>

#include "array.h"
#include "debug.h"

int
array_slide_up_n(array_t *a, int n, int len) {
  int rem;

  if(len < 0) {
    return FALSE;
  }
  if(n <= 0) {
    n = 0;
  }
  if(n >= a->n && a->n != 0) {
    n = a->n - 1;
  }
  rem = a->n - n;
  memmove(&(a->data[n+len]), &(a->data[n]), (a->esize * rem));
  return TRUE;
}

int
array_slide_up(array_t *a, int n) {
  return array_slide_up_n(a, n, 1);
}

int
array_slide_down_n(array_t *a, int n, int len) {
  if(len <= 0) {
    return TRUE;
  }
  if(n < 0) {
    n = 0;
  }
  if(n >= a->n && a->n != 0) {
    n = a->n - 1;
  }
  memmove(&(a->data[n]), &(a->data[n+len]), a->esize * (a->n - (n + len)) );
  return TRUE;
}

int
array_slide_down(array_t *a, int n) {
  return array_slide_down_n(a, n, 1);
}

array_t * 
array_new() {
  array_t *a;

  a = (array_t *)malloc(sizeof(array_t));
  if(!a) {
    fprintf(stderr, "Error allocating space for array\n");
    return NULL;
  }
  a->data  = NULL;
  a->n     = 0;
  a->alloc = 0;
  a->esize = sizeof(void *);
  return a;
}

int
array_length(array_t *a) {
  return a->n;
}

int 
array_grow(array_t *a, int len) {
  void **tmp;
  a->alloc = 2;
  while(a->alloc <= len) {
    a->alloc *= 2;
  }
  tmp = (void **) realloc(a->data, (size_t) (a->alloc * a->esize));
  if(!tmp) {
    return FALSE;
  }
  a->data = tmp;
  memset(& (a->data[a->n]), 0, (a->alloc - a->n) * a->esize);
  return TRUE;
}

void
array_free(array_t **pa) {
  int i;
  array_t *a = *pa;

  if(pa == NULL) {
    return;
  }
  if(a == NULL) {
    return;
  }
  for(i = 0; i < a->n; i++) {
    a->free(a->data[i]);
  }

  a->n = 0;
  a->alloc = 0;
  a->esize = 0;

  free(a->data);
  a->data = NULL;

  free(a);
  pa = NULL;
  return;
}

int
array_insert_n(array_t *a, void **data, int len, int where) {
  int n;

  if(where < 0) {
    where = where + a->n;
  }
  if(where < 0 || where > a->n) {
    return FALSE;
  }
  if(len <= 0) {
    return TRUE;
  }
  n = a->n + len;
  if(n > a->alloc) {
    if(!array_grow(a, n)) {
      return FALSE;
    }
  }
  array_slide_up_n(a, where, len);
  a->n = n;
  if(! array_element_set_n(a, where, data, len)) {
    return FALSE;
  }
  return TRUE;
}

int
array_insert(array_t *a, void *data, int where) {
  int n;

  if(where < 0) {
    where = where + a->n;
  }
  if(where < 0 || where > a->n) {
    return FALSE;
  }
  n = a->n + 1;

  if(n >= a->alloc) {
    if(!array_grow(a, n)) {
      return FALSE;
    }
  }

  array_slide_up(a, where);
  a->n = n;
  if(! array_element_set(a, where, data) ) {
    return FALSE;
  }
  return TRUE;
}

int 
array_push(array_t *a, void *data) {
  return array_insert(a, data, a->n);
}

int 
array_append(array_t *a, void *data) {
  return array_insert(a, data, a->n);
}

int
array_append_n(array_t *a, void **data, int len) {
  if(!a) {
    return FALSE;
  }
  return array_insert_n(a, data, len, a->n);
}

int
array_prepend(array_t *a, void *data) {
  return array_insert(a, data, 0);
}

int
array_clear(array_t *a) {
  if(!a) {
    return FALSE;
  }
  a->n = 0;
  return TRUE;
}

int
array_remove_n(array_t *a, int where, int len) {
  int i;
  if(where < 0) {
    where = where + a->n;
  }
  if(where < 0 || where >= a->n) {
    return FALSE;
  }
  for(i = where; i < where + len; i++) {
    a->free( a->data[i] );
  }
  array_slide_down_n(a, where, len);
  a->n = a->n - len;
  return TRUE;
}

int
array_remove(array_t *a, int where) {
  return array_remove_n(a, where, 1);
}

void *
array_shift(array_t *a) {
  void *d;
  d = array_element(a, 0);
  array_slide_down(a, -1);
  a->n = a->n - 1;
  return d;
}

int
array_element_set(array_t *a, int where, void *data) {
  if(where < 0) {
    where = where + a->n;
  }
  if(where < 0 || where >= a->n) {
    return FALSE;
  }
  a->data[where] = data;
  return TRUE;
}

int
array_element_set_n(array_t *a, int where, void **data, int len) {
  if(len <= 0) {
    return FALSE;
  }
  if(where < 0) {
    where = where + a->n;
  }
  if(where < 0 || where >= a->n) {
    return FALSE;
  }
  memcpy(&(a->data[where]), data, (len * a->esize));
  return TRUE;
}

void *
array_element(array_t *a, int where) {
 if(where < 0) {
    where = where + a->n;
  }
  if(where >= a->n || where < 0) {
    return (void *) 0;
  }
  return a->data[where];
}

void *
array_pop(array_t *a) {
  void *d;
  d = array_element(a, -1);
  a->n = a->n - 1;
  return d;
}

int
array_append_array(array_t *to, array_t *from) {
  return array_append_n(to, from->data, from->n);
}

int
array_copy_array(array_t *to, array_t *from) {
  array_clear(to);
  return array_append_n(to, from->data, from->n);
}

array_t *
array_copy(array_t *a) {
  array_t *new;
  new = array_new();
  array_copy_array(new, a);
  return new;
}

int
array_compare(array_t *a, int i, void *value) {
  return a->compare(a->data[i], value);
}

int
array_find_value(array_t *a, void *value) {
  int i;
  for(i = 0; i < a->n; i++) {
    if(array_compare(a, i, value)) {
      return i;
    }
  }
  return -1;
}

int
array_count_value(array_t *a, void *value) {
  int i;
  int count;
  count = 0;
  for(i = 0; i < a->n; i++) {
    count += array_compare(a, i, value);
  }
  return count;
}

int
array_remove_value(array_t *a, void *value) {
  int index;
  if((index = array_find_value(a, value)) < 0) {
    return FALSE;
  }
  return array_remove(a, index);
}

int
array_reverse(array_t *a) {
  int i;
  void *tmp;
  for(i = 0; i < a->n/2; i++) {
    tmp = a->data[i];
    a->data[i] = a->data[ a->n - 1 - i ];
    a->data[ a->n - 1 - i ] = tmp;
  }
  return TRUE;
}

array_t *
array_slice(array_t *a, int start, int stop, int step, int len) {
  int i, k;
  array_t *new;
  UNUSED(stop);
  new = array_new();
  k = start;
  for(i = 0; i < len; i++) {
    array_append(new, a->data[k]);
    k += step;
  }
  return new;
}


#ifdef __UNIT_TESTING_ARRAY__

void
array_dump(array_t *a) {
  int i;
  for(i = 0; i < a->n; i++) {
    a->print(a->data[i]);
  }
}

void 
int_print(void *pa) {
  int *a = (int *) pa;
  fprintf(stderr, "%d\n", *a);
}
int 
int_compare(void *pa, void *pb) {
  int *a = (int *)pa;
  int *b = (int *)pb;
  return *a <= *b;
}
void
int_free(void *pa) {
  int *a = (int *)pa;
  if(!a) {
    return;
  }
  free(a);
  a = NULL;
}

void
thing_print(void *pt) {
  thing *t = (thing *)pt;
  fprintf(stderr,"%d\n", t->id);
}

int
thing_compare(void *pa, void *pb) {
  thing *a = (thing *)pa;
  thing *b = (thing *)pb;
  return (a->id <= b->id);
}

void
thing_free(void *pa) {
  thing *a = (thing *)pa;
  if(!a) {
    return;
  }
  free(a);
  a = NULL;
}

thing *
tn(int id) {  
  thing *t = (thing *) malloc(sizeof(thing));
  t->id = id;
  return t;
}

int
thing_main() {
  thing **f;
  thing *t;
  array_t *a;


  a = array_new();
  a->print   = thing_print;
  a->compare = thing_compare;
  a->free    = thing_free;

  array_push(a, tn(1));
  array_push(a, tn(2));
  array_push(a, tn(3));
  array_push(a, tn(4));
  
  array_dump(a);

  array_append(a, tn(5));
  array_append(a, tn(6));

  array_dump(a);

  array_prepend(a, tn(0));
  array_prepend(a, tn(-1));
  array_prepend(a, tn(-2));
  array_prepend(a, tn(-3));

  array_dump(a);

  array_remove(a, 0);
  array_remove(a, 2);
  array_remove(a, 4);
  array_remove(a, -1);
  array_remove(a, 50);

  array_dump(a);

  t = array_shift(a);
  thing_free(t);
  t = array_shift(a);
  thing_free(t);
  t = array_shift(a);
  thing_free(t);  

  array_dump(a);

  array_prepend(a, tn(0));
  array_insert(a, tn(1), 1);
  array_insert(a, tn(3), 3);

  array_dump(a);

  t = array_pop(a);
  thing_free(t);  
  t = array_pop(a);
  thing_free(t);  
  t = array_pop(a);
  thing_free(t);  

  array_dump(a);

  array_insert(a, tn(3), a->n);
  array_insert(a, tn(-1), 0);

  array_dump(a);


  array_remove(a, -1);
  array_remove(a, -1);
  array_remove(a, 50);

  array_dump(a);

  f = (thing **) malloc(sizeof(thing *) * 4);
  f[0] = tn(10);
  f[1] = tn(11);
  f[2] = tn(12);
  f[3] = tn(13);

  array_dump(a);

  array_insert_n(a, (void **) f, 4, 3);
  f[0] = tn(10);
  f[1] = tn(11);
  f[2] = tn(12);
  f[3] = tn(13);
  array_insert_n(a, (void **) f, 4, 2);
  f[0] = tn(10);
  f[1] = tn(11);
  f[2] = tn(12);
  f[3] = tn(13);
  array_insert_n(a, (void **) f, 4, 1);
  free(f);

  array_dump(a);

  array_remove_n(a, 1, 4);
  array_remove_n(a, 2, 4);
  array_remove_n(a, 3, 4);
  
  array_dump(a);

  array_free(&a);

  return 0;
}

int 
main() {
  thing_main();
  return 0;
}

#endif /* __UNIT_TESTING_ARRAY__ */
