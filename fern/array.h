
#include <stdlib.h>

void *xarray_new(char t);
void *xarray_new_with_len(char t, int n);
void xarray_free(void *a);
void xarray_clear(void *a);
void *xarray_append(void *a, ...);
size_t xarray_length(void *a);
void xarray_delete(void *a, int i);
void xarray_pop(void *a);

void xarray_sort(void *a, int (*compare) (const void *a, const void *b));
void * xarray_index(void *a, size_t i);
void xarray_free_items(void *p, void (*free_data) (void *) );
