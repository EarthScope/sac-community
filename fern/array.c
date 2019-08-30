// LICENSE: MIT

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>

#ifdef WIN32
#define pointer char *
#else
#define pointer void *
#endif

/**
 * @defgroup array array
 * @brief Growable vector
 *
 * @code{.c}
 * // Create a vector of ints 'i'
 *    int *v = xarray_new('i');
 *    v = xarray_append(v, 0);
 *    v = xarray_append(v, 1);
 *    v = xarray_append(v, 2);
 *    assert(xarray_length(v) == 3);
 *    assert(v == {0, 1, 2});
 *
 * // Remove the last item
 *    xarray_pop(v);
 *    assert(xarray_length(v) == 2);
 *    assert(v == {0, 1});
 *
 * // Add two more items
 *    v = xarray_append(v, 3);
 *    v = xarray_append(v, 4);
 *    assert(v == {0, 1, 3, 4});
 *
 * // Remove the item at index 2, i.e. the number 3 (index starts at 0)
 *    xarray_delete(v, 2);
 *    assert(v == {0, 1, 4});
 *
 * // Remove the item at index 0, i.e. the number 0
 *    xarray_delete(v, 0);
 *    assert(v == {1, 4});
 *
 * // Create a vector of pointers with length 10
 *    sac **p = xarray_new_with_len('p', 3);
 *    p = xarray_append(p, sac_new()));
 *    p = xarray_append(p, sac_new()));
 *    p = xarray_append(p, sac_new()));
 *
 * // Accesss is most easily done directly
 *    for(int i = 0; i < xarray_length(p); i++) {
 *        printf("%p\n", p[i]);
 *    }
 *
 * @endcode
 */

/**
 * @brief xarray header structure
 * @private
 * @ingroup array
 */
struct xarray_header {
    size_t len;    /** number of items in array */
    size_t nalloc; /** number of items allocated */
    char type;     /** type of array */
    size_t size;   /** size of item */
    char buf[];    /** data storage */
};

/**
 * @brief Growable list or vector
 * @ingroup array
 *
 * @details This really does not exist, but allows the grouping of similar itesm
 *
 */
struct xarray { };

/**
 * @brief Get the xarray header from an xarray list
 *
 * @private
 * @ingroup array
 *
 * @memberof   xarray
 *
 * @param      a   xarray list
 *
 * @return     xarray_header
 */
struct xarray_header *
xarray_header(void *a) {
    struct xarray_header *ah;
    if (!a ||
        !(ah =
          (struct xarray_header *) ((pointer) a -
                                    offsetof(struct xarray_header, buf)))) {
        return NULL;
    }
    return ah;
}

/**
 * @brief Get the length of an xarray list
 *
 * @memberof   xarray
 * @ingroup array
 *
 * @param      a   xarray list
 *
 * @return     length of the list
 */
int
xarray_length(void *a) {
    struct xarray_header *ah;
    if (!(ah = xarray_header(a))) {
        return 0;
    }
    return (int) ah->len;
}
/**
 * @brief Create a new xarray list with a length of type t
 *
 * @memberof   xarray
 * @ingroup array
 *
 * @param      t    type of list
 * @param      n    length of list
 *
 * @return     new xarray list of type t, items are not initialized
 */
void *
xarray_new_with_len(char t, size_t n) {
    struct xarray_header *ah;
    size_t size;
    size_t nalloc = 2;
    switch (t) {
        case 'p':
            size = sizeof(void *);
            break;
        case 'i':
            size = sizeof(int);
            break;
        case 'f':
            size = sizeof(float);
            break;
        case 'd':
            size = sizeof(double);
            break;
        case 'c':
            size = sizeof(char);
            break;
        default:
            size = 0;
            break;
    }
    if (size == 0) {
        printf("arr invalid element type\n");
        return NULL;
    }
    while (n > nalloc) {
        nalloc *= 2;
    }
    ah = malloc(sizeof *ah + size * nalloc);
    if (!ah) {
        return NULL;
    }
    ah->len = n;
    ah->nalloc = nalloc;
    ah->type = t;
    ah->size = size;
    return (void *) ah->buf;
}
/**
 * @brief      create a new xarray vector with type t
 *
 * @details    create a new xarray vector with type t
 *
 * @memberof xarray
 * @ingroup array
 *
 * @param      t - Type of thing to store
 *             - 'p' - pointers
 *             - 'i' - integers
 *             - 'f' - floats, single precision floating point
 *             - 'd' - doubles, double precision floating point
 *             - 'c' - characters, probably best to use an string handling routine
 *
 * @note       Two items are initially allocated
 *
 * @return     pointer to memory of allocated memory
 *
 */
void *
xarray_new(char t) {
    return xarray_new_with_len(t, 0);
}

/**
 * @brief Free an xarray list
 *
 * @memberof xarray
 * @ingroup array
 *
 * @param    a   list to free
 *
 */
void
xarray_free(void *a) {
    struct xarray_header *ah;
    if (!(ah = xarray_header(a))) {
        return;
    }
    free(ah);
}

/**
 * @brief Clear an xarray
 *
 * @memberof   xarray
 * @ingroup array
 *
 * @param      a   list to clear
 *
 */
void
xarray_clear(void *a) {
    struct xarray_header *ah;
    if (!(ah = xarray_header(a))) {
        return;
    }
    memset(a, 0, ah->size * ah->nalloc);
    ah->len = 0;
}

/**
 * @brief Grow an array if needed, expand by factors of 2, memory is reallocated
 *
 * @memberof   xarray
 * @ingroup array
 * @private
 *
 * @param      ah   xarray header 
 * @param      n    new size
 *
 * @return     new xarray header, different if reallocated
 */
struct xarray_header *
xarray_grow(struct xarray_header *ah, size_t n) {
    struct xarray_header *new;
    if (n < ah->nalloc) {
        return ah;
    }
    while (n >= ah->nalloc) {
        ah->nalloc *= 2;
    }
    new = realloc(ah, sizeof(struct xarray_header) + ah->size * ah->nalloc);
    if (!new) {
        return NULL;
    }
    return new;
}

/**
 * @brief Append to a xarray list
 *
 * @memberof   xarray
 * @ingroup array
 *
 * @param      a      xarray list to append to
 * @param      ...    value to append
 *
 * @return     possibly new updated version of the xarray, if reallocated
 */
void *
xarray_append(void *a, ...) {
    struct xarray_header *ah;
    va_list ap;
    if (!(ah = xarray_header(a))) {
        return NULL;
    }
    ah = xarray_grow(ah, ah->len + 1);
    va_start(ap, a);
    switch (ah->type) {
        case 'p':
            ((void **) (ah->buf))[ah->len] = va_arg(ap, void *);
            break;
        case 'i':
            ((int *) (ah->buf))[ah->len] = va_arg(ap, int);
            break;
        case 'f':
            ((float *) (ah->buf))[ah->len] = (float) va_arg(ap, double);
            break;
        case 'd':
            ((double *) (ah->buf))[ah->len] = va_arg(ap, double);
            break;
        case 'c':
            ((char *) (ah->buf))[ah->len] = (char) va_arg(ap, int);
            break;
        default:
            break;
    }
    va_end(ap);
    ah->len++;
    return ah->buf;
}

/**
 * @brief Delete a value from the xarray at index i
 *
 * @memberof   xarray
 * @ingroup array
 *
 * @param      a   xarray list to remove item from
 * @param      i   index to remove
 *
 */
void
xarray_delete(void *a, int i) {
    size_t n = 0;
    size_t ui = 0;
    struct xarray_header *ah;
    if (!(ah = xarray_header(a))) {
        return;
    }
    if (i < 0 || (size_t) i > ah->len) {
        printf("xarray: attempt to delete a non-existant value\n");
        return;
    }
    ui = (size_t) i;
    n = ah->len - 1 - ui;
    memmove((pointer) a + ah->size * (size_t) i, (pointer) a + ah->size * (ui + 1),
            ah->size * n);
    ah->len--;
}

/**
 * @brief Get an item at index i
 *
 * @param      a  xarray list to get item from
 * @param      i  index to get item
 *
 * @return     value at a[i]
 */
void *
xarray_index(void *a, size_t i) {
    struct xarray_header *ah;
    if (!(ah = xarray_header(a))) {
        return NULL;
    }
    if(i >= ah->len) {
        printf("xarray: attempt to index for non-existant value\n");
        return NULL;
    }
    return (pointer) a + (ah->size * i);
}

/**
 * @brief Remove last item in a xarray
 *
 * @memberof   xarray
 * @ingroup array
 *
 * @param      a   xarray to remove last item from
 *
 */
void
xarray_pop(void *a) {
    struct xarray_header *ah;
    if (!(ah = xarray_header(a))) {
        return;
    }
    xarray_delete(a, (int)ah->len - 1);
}
/**
 * @brief Sort a xarray list base on the compare function
 *
 * @memberof xarray
 * @ingroup array

 * @param      a        xarray list to sort
 * @param      compare  comparison function
 *
 */
void
xarray_sort(void *a, int (*compare) (const void *a, const void *b)) {
    struct xarray_header *ah;
    if (!(ah = xarray_header(a))) {
        return;
    }
    qsort(a, ah->len, ah->size, compare);
}

/**
 * @brief Free items in an enclosed xarray list
 *
 * @memberof xarray
 * @ingroup array
 *
 * @param a          xarray list
 * @param free_data  how to free indiviual items
 *
 * @note only useful for list of pointers
 *
 */
void
xarray_free_items(void *a, void (*free_data) (void *) ) {
    struct xarray_header *ah = NULL;
    if(!a) {
        return;
    }
    if (!(ah = xarray_header(a))) {
        return;
    }
    if(ah->type != 'p') {
        return;
    }
    for(size_t i = 0; i < (size_t) xarray_length(a); i++) {
        free_data( *(void **) xarray_index(a, i) );
    }
}
