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

#ifndef _ARRAY_H_
#define _ARRAY_H_

#ifndef FALSE 
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE  1
#endif

typedef struct _array_t array_t;
struct _array_t {
  void **data;
  int    n;
  int    alloc;
  int    esize;
  void   (*print)(void *a);
  int    (*compare)(void *a, void *b);
  void   (*free)(void *a);
};
typedef enum _vtype vtype;
enum _vtype {
    va_char,
    va_uchar,
    va_int,
    va_uint,
    va_float,
    va_double,
};
typedef struct _varray_t varray_t;
struct _varray_t {
    char  *data;
    int    n;
    int    alloc;
    size_t esize;
    vtype  type;
};

int       array_grow(array_t *a, int len);
array_t * array_new();
int       array_element_set(array_t *a, int where, void *data);
void *    array_element(array_t *a, int which);
void      array_dump(array_t *a);
int       array_element_set_n(array_t *a, int where, void **data, int len);
int       array_length(array_t *a);
void      array_free(array_t **pa);
int       array_append(array_t *a, void *data);
int       array_insert_n(array_t *a, void **data, int len, int where);
int       array_append_n(array_t *a, void **data, int len);
int       array_copy_array(array_t *to, array_t *from);
int       array_append_array(array_t *to, array_t *from);
int       array_insert(array_t *a, void *data, int where);
int       array_remove_value(array_t *a, void *value);
void *    array_pop(array_t *a);
int       array_push(array_t *a, void *data);
int       array_remove(array_t *a, int where);
int       array_find_value(array_t *a, void *value);
int       array_count_value(array_t *a, void *value);
int       array_reverse(array_t *a);
array_t * array_slice(array_t *a, int start, int stop, int step, int len);

varray_t *    varray_new(vtype t);
int           varray_length(varray_t *v);
int           varray_grow(varray_t *v, int len);
int           varray_append(varray_t *v, ...);
int           varray_get_int(varray_t *v, int i);
unsigned int  varray_get_uint(varray_t *v, int i);
char          varray_get_char(varray_t *v, int i);
unsigned char varray_get_uchar(varray_t *v, int i);

#endif /* __ARRAY_H__ */
