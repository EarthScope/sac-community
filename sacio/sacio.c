/**
 * @file
 * @brief sac I/O and manipulation
 */
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
#include <float.h>
#include <sys/stat.h>
#include <errno.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/** @cond NO_DOCS */
#define SAC_NULL_HEADER_REQUIRED /**< @private Define a fully NULL sac header */
/** @endcond */
#include <sacio.h>

#include "time64.h"
#include "strip.h"
#include "defs.h"

#include "geodesic.h"

#define ERROR_NOT_A_SAC_FILE                1317 /**< @brief Not a sac file */
#define ERROR_OVERWRITE_FLAG_IS_OFF         1303 /**< @brief Overwrite flag, lovrok is set to 0 */
#define ERROR_WRITING_FILE                  115 /**< @brief Error writing sac file */
#define ERROR_READING_FILE                  114 /**< @brief Error reading sac file */
#define ERROR_FILE_DOES_NOT_EXIST           108 /**< @brief Error file does not exist */
#define ERROR_OPENING_FILE                  101 /**< @brief Error opening sac file */
#define SAC_OK                              0 /**< @brief Success, everything is ok */

#define SAC_HEADER_SIZEOF_NUMBER          (sizeof(float))  /**< @brief Size of header values in bytes */
#define SAC_DATA_SIZE                     SAC_HEADER_SIZEOF_NUMBER /**< @brief Size of data values in bytes */
#define SAC_HEADER_VERSION_6              6 /**< @brief Sac version no 6 */
#define SAC_HEADER_VERSION_7              7 /**< @brief Sac version no 7 */
#define SAC_HEADER_MAJOR_VERSION          SAC_HEADER_VERSION_6 /**< @brief Current Major sac version number */
#define SAC_HEADER_MAX_VERSION            SAC_HEADER_VERSION_7 /**< @brief Current Maximum sac version number */
#define SAC_VERSION_LOCATION              76 /**< @brief Offset in 4-byte words of the header version */

/**
 * @defgroup sac sac
 * @brief sac file I/O and manipulation
 *
 */


/**
 * @brief   Minimum of two values
 * @private
 */
#define MIN(a,b) ((a < b) ? a : b )
/**
 * @brief   Maximum of two values
 * @private
 */
#define MAX(a,b) ((a > b) ? a : b )

/** \cond NO_DOCS */
sacmeta * sac_meta_new();
sac * sac_read_internal(char *filename, int read_data, int *nerr);
double calc_e_even(sac *s);
void sac_write_internal(sac *s, char *filename, int write_data, int swap, int *nerr);
static float array_max(float *y, int n);
static float array_min(float *y, int n);
static float array_mean(float *y, int n);
sac_hdr * sac_hdr_new();
int sac_get_time_ref(sac *s, timespec64 *t);
/** \endcond */

/**
 * @brief Get a sac header floating value. Error are returned as SAC_FLOAT_UNDEFINED
 * @private
 */
static double
sac_float(sac *s, int id) {
    double v = 0;
    if(!sac_get_float(s, id, &v)) {
        return SAC_FLOAT_UNDEFINED;
    }
    return v;
}

/**
 * @brief Get begin value
 * @private
 */
#define  B(s) sac_float(s, SAC_B)
/**
 * @brief Get end value
 * @private
 */
#define  E(s) sac_float(s, SAC_E)
/**
 * @brief Get delta time value
 * @private
 */
#define DT(s) sac_float(s, SAC_DELTA)

/**
 * @param SAC_HEADER_FLOATS
 *    Number of Floating point values in the SAC Header
 *    Size: \p SAC_HEADER_SIZEOF_NUMBER bytes
 *
 */
#define SAC_HEADER_FLOATS                 70    /* 4 bytes  (real or float)    */
/**
 * @param SAC_HEADER_INTEGERS
 *    Number of Integer values in the SAC Header
 *    Size: \p SAC_HEADER_SIZEOF_NUMBER bytes
 *
 */
#define SAC_HEADER_INTEGERS               15

/**
 * @param SAC_HEADER_ENUMS
 *    Number of Enumerated values in the SAC Header
 *    Size: \p SAC_HEADER_SIZEOF_NUMBER bytes
 *
 */
#define SAC_HEADER_ENUMS                  20
/**
 * @param SAC_HEADER_LOGICALS
 *    Number of Logical values in the SAC Header
 *    Size: \p SAC_HEADER_SIZEOF_NUMBER bytes
 *
 */
#define SAC_HEADER_LOGICALS               5
/**
 * @param SAC_HEADER_STRINGS
 *    Number of strings in the SAC Header.  The number here is in reality
 *      one less as the number below as of header version 6.  The second
 *      value, the event name is twice as long as any other characer string
 *
 */
#define SAC_HEADER_STRINGS                24    /* 9 bytes (characters) actually 23 + 1 */

/**
 * @param SAC_HEADER_NUMBERS
 *    Number of numeric values in the SAC Header
 *    Size: \p SAC_HEADER_SIZEOF_NUMBER bytes
 *
 */
#define SAC_HEADER_NUMBERS                ( SAC_HEADER_FLOATS + SAC_HEADER_INTEGERS + SAC_HEADER_ENUMS + SAC_HEADER_LOGICALS )
/** @cond NO_DOCS */
/**
 * @param SAC_HEADER_STRINGS_SIZE_FILE
 *    Size of file character header data block in four byte increments
 * @note 8 * \p SAC_HEADER_STRINGS
 */
#define SAC_HEADER_STRINGS_SIZE_FILE    ( 2 * SAC_HEADER_STRINGS )

/**
 * @param SAC_HEADER_STRING_LENGTH_FILE
 *    Size of a character string stored on disk for a SAC header
 *    Strings are stored without the C string termination character
 */
#define SAC_HEADER_STRING_LENGTH_FILE     8
/**
 * @param SAC_HEADER_STRING_SIZE_BYTES_FILE
 *     Size of string header in file measured in bytes
 *     SAC_HEADER_STRINGS * SAC_HEADER_STRINGS_LENGTH_FILE
 */
#define SAC_HEADER_STRINGS_SIZE_BYTES_FILE ( SAC_HEADER_STRINGS * SAC_HEADER_STRING_LENGTH_FILE )

/**
 * @param SAC_HEADER_NUMBERS_SIZE_BYTES_FILE
 *     Size of numbers header in file measured in bytes
 *     SAC_HEADER_NUMBERS * SAC_HEADER_SIZEOF_NUMBER
 */
#define SAC_HEADER_NUMBERS_SIZE_BYTES_FILE ( SAC_HEADER_NUMBERS * SAC_HEADER_SIZEOF_NUMBER )
/** \endcond */

/**
 * @brief      Read a sac file
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Read a sac file, data and header
 *
 * @param      filename   file to read data and header from
 * @param      nerr       status code, 0 on success, non-zero on header
 *
 * @return     sac file structure, NULL on failure
 */
sac *
sac_read(char *filename, int *nerr) {
    return sac_read_internal(filename, 1, nerr);
}


/**
 * @brief      Read a sac file header
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Read a sac file header
 *
 * @param      filename    file to read sac header from
 * @param      nerr        status code, 0 on success, non-zero on failure
 *
 * @return     sac file structure, NULL on failure
 */
sac *
sac_read_header(char *filename, int *nerr) {
    return sac_read_internal(filename, 0, nerr);
}



/**
 * @brief      write a sac file to disk
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    write a sac file to dist, header and data
 *
 * @param      s         sac file to write to disk
 * @param      filename  file to write to
 * @param      nerr      status code, 0 on success, non-zero on error
 *
 */
void
sac_write(sac *s, char *filename, int *nerr) {
    sac_write_internal(s, filename, SAC_WRITE_HEADER_AND_DATA, s->m->swap, nerr);
}

void
sac_write_header(sac *s, char *filename, int *nerr) {
    sac_write_internal(s, filename, SAC_WRITE_HEADER, s->m->swap, nerr);
}


int
sac_is_timeval(int hid) {
    switch(hid) {
    case SAC_B: case SAC_E: case SAC_O: case SAC_A: case SAC_F:
    case SAC_T0: case SAC_T1: case SAC_T2: case SAC_T3:
    case SAC_T4: case SAC_T5: case SAC_T6: case SAC_T7:
    case SAC_T8: case SAC_T9:
        return 1;
        break;
    default:
        break;
    }
    return 0;
}

/**
 * @brief    X-Macro for sac_f64_new()
 * @private
 */
#define X(name,key)  z->key = SAC_FLOAT_UNDEFINED;

/**
 * @brief      create a new 64 bit float sac header value
 *
 * @ingroup    sac
 * @memberof   sac
 * @private
 *
 * @details    create a new 64 bit float sac header value and set all values to SAC_FLOAT_UNDEFINED
 *
 * @return     newly created 64 bit sac header
 */
sac_f64 *
sac_f64_new() {
    sac_f64 *z;
    z = (sac_f64 *) malloc(sizeof(sac_f64));
    if(z) {
  SAC_F64
    }
    return z;
}
#undef X

/**
 * @brief   X-Macro for sac_f32_new()
 * @private
 */
#define X(name,key)  case SAC_##name: s->h->key = (float) value; break;

/**
 * @brief      set a 32-bit float value
 *
 * @ingroup    sac
 * @memberof   sac
 * @private
 *
 * @details    set a 32-bit float value in the sac header
 *
 * @param      s      sac file
 * @param      n      sac header id
 * @param      value  value to set (value is converted from double to float)
 *
 * @return     1 on success, 0 on error
 */
int
sac_set_f32(sac *s, int n, double value) {
    switch(n) {
  SAC_F32
    default:
        fprintf(stderr, "Error in sac_set_f32(): Unknown type: %d\n", n);
        return 0;
    }
    return 1;
}
#undef X

/**
 * @brief      set a 64-bit float value
 *
 * @ingroup    sac
 * @memberof   sac
 * @private
 *
 * @details    set a 64-bit float value in the sac header, values not in the 64-bit header
 *             are set in the 32-bit header
 *
 * @param      s      sac file
 * @param      n      sac header id
 * @param      value  value to set
 *
 * @return     return type
 */
int
sac_set_f64(sac *s, int n, double value) {
    switch(n) {
#define X(name,key)  case SAC_##name: s->z->key = value; s->h->key = (float) value; break;
        SAC_F64
#undef X
    default:
        return sac_set_f32(s, n, (float) value);
        //fprintf(stderr, "Error in sac_set_f64(): Unknown type: %d\n", n);
        break;
    }
    return 1;
}

/**
 * @brief      get a 32-bit float value
 *
 * @ingroup    sac
 * @memberof   sac
 * @private
 *
 * @details    get a 32-bit float value from the sac header
 *
 * @param      s   sac file
 * @param      n   sac header id
 * @param      v   value to get (must be double precision / 64-bit)
 *
 * @return     1 on success, 0 on failure
 */
int
sac_get_f32(sac *s, int n, double *v) {
    switch(n) {
#define X(name,key) case SAC_##name: *v = s->h->key; break;
        SAC_F32
#undef X
    default:
        fprintf(stderr, "Error in sac_get_f32(): Unknown type: %d\n", n);
        return 0;
    }
    return 1;
}

/**
 * @brief      get a 64-bit float value
 *
 * @ingroup    sac
 * @memberof   sac
 * @private
 *
 * @details    get a 64-bit float value from the sac header.  If the value is not in
 *             the 64-bit header, value is taken from the 32-bit header
 *
 * @param      s   sac file
 * @param      n   sac header id
 * @param      v   value to get
 *
 * @return     return type
 */
int
sac_get_f64(sac *s, int n, double *v) {
    switch(n) {
#define X(name,key)  case SAC_##name: *v = s->z->key; break;
        SAC_F64
#undef X
    default:
        return sac_get_f32(s, n, v);
        break;
    }
    return 1;
}

/**
 * @brief      Copy all values from the 64-bit to 32-bit float header
 *
 * @ingroup    sac
 * @memberof   sac
 * @private
 *
 * @details    Copy all values from the 64-bit to 32-bit float header
 *
 * @param      s  sac file
 *
 */
void
sac_copy_f64_to_f32(sac *s) {
#define X(name,key) s->h->key = (float) s->z->key;
    SAC_F64
#undef X
}
/**
 * @brief      Copy all values from the 32-bit to 64-bit float header
 *
 * @ingroup    sac
 * @memberof   sac
 * @private
 *
 * @details    Copy all values from the 32-bit to 64-bit float header
 *
 * @param      s  sac file
 *
 */
void
sac_copy_f32_to_f64(sac *s) {
#define X(name,key) s->z->key = s->h->key;
    SAC_F64
#undef X
}

/**
 * @brief      Set a time pick value
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Set a time pick value in a sac header
 *
 * @param      s       sac file
 * @param      n       pick number
 * @param      value   timing value to set (64 bit value)
 *
 * @return     1 on success, 0 on failure
 */
int
sac_set_pick(sac *s, int n, double value) {
    switch(n) {
    case 0: return sac_set_float(s, SAC_T0, value); break;
    case 1: return sac_set_float(s, SAC_T1, value); break;
    case 2: return sac_set_float(s, SAC_T2, value); break;
    case 3: return sac_set_float(s, SAC_T3, value); break;
    case 4: return sac_set_float(s, SAC_T4, value); break;
    case 5: return sac_set_float(s, SAC_T5, value); break;
    case 6: return sac_set_float(s, SAC_T6, value); break;
    case 7: return sac_set_float(s, SAC_T7, value); break;
    case 8: return sac_set_float(s, SAC_T8, value); break;
    case 9: return sac_set_float(s, SAC_T9, value); break;
    default:
        fprintf(stderr, "Error in sac_set_pick(): Unknown pick: %d\n", n);
        return 0;
        break;
    }
    return 1;
}

/**
 * @brief      Get a timing pick
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Get a timing pick from the sac header
 *
 * @param      s   sac file
 * @param      n   pick number
 * @param      t   return value (64-bit value)
 *
 * @return     1 on success, 0 on failure
 */
int
sac_get_pick(sac *s, int n, double *t) {
    switch(n) {
    case 0: return sac_get_float(s, SAC_T0, t); break;
    case 1: return sac_get_float(s, SAC_T1, t); break;
    case 2: return sac_get_float(s, SAC_T2, t); break;
    case 3: return sac_get_float(s, SAC_T3, t); break;
    case 4: return sac_get_float(s, SAC_T4, t); break;
    case 5: return sac_get_float(s, SAC_T5, t); break;
    case 6: return sac_get_float(s, SAC_T6, t); break;
    case 7: return sac_get_float(s, SAC_T7, t); break;
    case 8: return sac_get_float(s, SAC_T8, t); break;
    case 9: return sac_get_float(s, SAC_T9, t); break;
    default:
        fprintf(stderr, "Error in sac_get_pick(): Unknown pick: %d\n", n);
        return 0;
        break;
    }
    return 1;
}

/**
 * @brief      Set the header version to 7
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @param      s   sac file
 *
 * @details    Set the header version to `7`.
 *             See sac::sac_get_float() and sac::sac_set_float() for the details
 *             about how the different header version impact which floating point
 *             values are set and get.
 */
void
sac_set_v7(sac *s) {
    s->h->nvhdr = SAC_HEADER_VERSION_7;
}
/**
 * @brief      Set the header version to 6
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @param      s   sac file
 *
 * @details    Set the header version to `6`.
 *             See sac::sac_get_float() and sac::sac_set_float() for the details
 *             about how the different header version impact which floating point
 *             values are set and get.
 */
void
sac_set_v6(sac *s) {
    s->h->nvhdr = SAC_HEADER_VERSION_6;
}

/**
 * @brief      create a new sac file structure
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    create a new sac file structure. Header and meta data are allocated.
 *             Data components are not allocated and should be done with sac_alloc()
 *             after setting \p the type of file and the number of data points
 *
 * @return     newly created sac file structure
 */
sac *
sac_new() {
    sac *s;
    s = (sac *) malloc(sizeof(sac));
    if (s) {
        s->h = sac_hdr_new();
        if (!s->h) {
            goto ERROR;
        }
        s->m = sac_meta_new();
        if (!s->m) {
            goto ERROR;
        }
        s->z = sac_f64_new();
        if(!s->z) {
            goto ERROR;
        }
        s->n = 1;
        s->y = NULL;
        s->x = NULL;
        s->sddhdr = NULL;
    }

    return s;
  ERROR:
    sac_free(s);
    return NULL;
}


/**
 * @brief      free a sac file structure
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    free a sac file structure.  Free any data components, meta
 *             data, header and other associated components
 *
 * @param      s   sac file struture to free
 *
 */
void
sac_free(sac * s) {
    if (s) {
        FREE(s->h);
        FREE(s->x);
        FREE(s->y);
        FREE(s->z);
        if (s->m) {
            FREE(s->m->filename);
        }
        FREE(s->m);
        FREE(s->sddhdr);
        FREE(s);
    }
}

/**
 * @brief      allocate the sac data portion of a sac strucutre
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    allocate the sac data poriton of a sac structure.  The memory
 *             size the data and components are determined by sac_comps() and
 *             the number of points \p npts. Any previous data is freed 
 *             before allocation
 *
 * @param      s   sac file to allocate the data for
 *
 */
void
sac_alloc(sac * s) {
    size_t n = 0;
    if (!s) {
        return;
    }
    FREE(s->y);
    FREE(s->x);
    if(s->h->npts <= 0) {
        return;
    }
    n = sizeof(float) * (size_t) s->h->npts;
    s->y = (float *) malloc(n);
    memset(s->y, 0, n);
    if (sac_comps(s) == 2) {
        s->x = (float *) malloc(n);
        memset(s->x, 0, n);
    }
}

#define COPY_MOVE(dst, src, n) do {   \
        memcpy(dst, src, n);          \
        dst[n] = 0;                   \
        src += n;                     \
        dst += (n+1);                 \
    } while(0) ;

#define SAC_ALPHA_FLOAT_LINES  14
#define SAC_ALPHA_INT_LINES     8
#define SAC_ALPHA_STRING_LINES  8
#define SAC_ALPHA_FLOAT_FMT    "%#15.7g"
#define SAC_ALPHA_INT_FMT      "%10d"
#define SAC_ALPHA_STRING_FMT   "%8s"
#define SAC_ALPHA_DOUBLE_FMT   "%.17g" // https://stackoverflow.com/a/21162120

void
sac_write_alpha(sac *s, char *filename, int *nerr) {
    FILE *fp = NULL;
    float *f = NULL;
    double *d = NULL;
    char *c = NULL;
    int *v = NULL;
    int i = 0, j = 0;
    if(!s || !filename) {
        *nerr = 1301;
        goto error;
    }
    if(!(fp = fopen(filename, "w"))) {
        *nerr = 101;
        goto error;
    }
    f = &(s->h->_delta);
    for(j = 0; j < SAC_ALPHA_FLOAT_LINES; j++) {
        for(i = 0; i < 5; i++) {
            fprintf(fp, SAC_ALPHA_FLOAT_FMT, *f);
            f++;
        }
        fprintf(fp, "\n");
    }
    v = &(s->h->nzyear);
    for(j = 0; j < SAC_ALPHA_INT_LINES; j++) {
        for(i = 0; i < 5; i++) {
            fprintf(fp, SAC_ALPHA_INT_FMT,  *v);
            v++;
        }
        fprintf(fp, "\n");
    }
    c = s->h->kstnm;
    fprintf(fp, "%8s%16s\n", s->h->kstnm, s->h->kevnm);
    c += (8+1)*3;
    for(i = 1; i < SAC_ALPHA_STRING_LINES; i++) {
        for(j = 0; j < 3; j++) {
            fprintf(fp, SAC_ALPHA_STRING_FMT, c); c += 9;
        }
        fprintf(fp, "\n");
    }
    for(j = 0; j < sac_comps(s); j++) {
        f = (j == 0) ? s->y : s->x;
        for(i = 0; i < s->h->npts; i++) {
            fprintf(fp, SAC_ALPHA_FLOAT_FMT, f[i]);
            if(i % 5 == 4 && i+1 != s->h->npts) {
                fprintf(fp,"\n");
            }
        }
        fprintf(fp, "\n");
    }
    if(s->h->nvhdr == SAC_HEADER_VERSION_7) {
        // Version 7 Header
        d = &(s->z->_delta);
        for(i = 0; i < (int)(sizeof(sac_f64)/sizeof(double)); i++) {
            fprintf(fp, SAC_ALPHA_DOUBLE_FMT "\n", *d);
            d++;
        }
    }
 error:
    if(fp) {
        fclose(fp);
    }
}

sac *
sac_read_alpha(char *filename, int *nerr) {
    FILE *fp = NULL;
    int i = 0, j = 0;
    float *f = NULL;
    int *v = NULL;
    char *c = NULL;
    sac *s = NULL;
    char line[256] = {0};
    char *p = NULL;
    double *d = NULL;

    *nerr = 0;

    if(!(fp = fopen(filename, "r"))) {
        *nerr = 101;
        goto error;
    }
    s = sac_new();
    s->m->filename = strdup(filename);

    f = &(s->h->_delta);
    for(j = 0; j < 14; j++) {
        if(fgets(line, sizeof(line), fp) == NULL) { // Read a Line
            *nerr = 1319;
            goto error;
        }
        if(sscanf(line, "%15g%15g%15g%15g%15g", &f[0],&f[1],&f[2],&f[3],&f[4]) != 5) {
            printf("Error reading float: %d,%d\n", j,i);
            *nerr = 1319;
            goto error;
        }
        f+=5;
    }
    v = &(s->h->nzyear);
    for(j = 0; j < 8; j++) {
        if(fgets(line, sizeof(line), fp) == NULL) { // Read a Line
            *nerr = 1319;
            goto error;
        }
        if(sscanf(line, "%10d%10d%10d%10d%10d", &v[0],&v[1],&v[2],&v[3],&v[4]) != 5) {
            printf("Error reading int: %d,%d\n", j,i);
            *nerr = 1319;
            goto error;
        }
        v+=5;
    }
    c = s->h->kstnm;
    for(i = 0; i < 8; i++) {
        if(fgets(line, sizeof(line), fp) == NULL) { // Read a Line
            *nerr = 1319;
            goto error;
        }
        p = line;
        COPY_MOVE(c, p, 8);
        if(i == 0) {
            memcpy(c, p, 16); c[16] = 0; c += 18;
        } else {
            COPY_MOVE(c, p, 8);
            COPY_MOVE(c, p, 8);
        }
    }

    sac_alloc(s);
    for(j = 0; j < sac_comps(s); j++) {
        f = (j == 0) ? s->y : s->x;
        for(i = 0; i < s->h->npts; i++) {
            if(fscanf(fp, "%f", &f[i]) != 1) {
                *nerr = 1319;
                goto error;
            }
        }
        if(fgets(line, sizeof(line), fp) == NULL) {
            *nerr = 1319;
            goto error;
        }
    }
    if(s->h->nvhdr == 6) {
        sac_copy_f32_to_f64(s);
        return s;
    }
    /* Version 7 Header */
    d = &(s->z->_delta);
    for(i = 0; i < (int)(sizeof(sac_f64)/sizeof(double)); i++) {
        if(fscanf(fp, "%lf", d) != 1) {
            *nerr = 1319;
            goto error;
        }
        d++;
    }
    return s;
 error:
    if(s) {
        sac_free(s);
        s = NULL;
    }
    return NULL;
}

/**
 * @brief      Set the beginning and end time for a sac file
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Set the beginning and end time for a sac file
 *
 * @param      s   sac file
 *
 */
void
sac_be(sac *s) {
    if(s->h->leven) {
        sac_set_float(s, SAC_E, calc_e_even(s));
    } else {
        if(s->x) {
            sac_set_float(s, SAC_B, array_min(s->x, s->h->npts));
            sac_set_float(s, SAC_E, array_max(s->x, s->h->npts));
        }
    }
}

#ifdef USE_GEOGRAPHICLIB

/**
 * @brief      Update the dist, az, gcarc, baz header fields
 *
 * @details    Update the dist, az, gcarc, baz header fields of a sac file
 *             using the geographiclib v 1.49.  Header updates are performed
 *             if stlo, stla, evlo, evla are defined. Radius and flattening
 *             used are defined from WGS84:
 *             - radius:     6378137 m
 *             - flattening: 1.0 / 298.257223563
 *             See https://geographiclib.sourceforge.io/
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @param      s  sac file to update
 *
 */
void
update_distaz(sac *s) {
    double lat1 = 0.0, lon1 = 0.0, lat2 = 0.0, lon2 = 0.0;
    double degrees = 0.0, meters = 0.0, az1 = 0.0, az2 = 0.0;
    double a = 6378137, f = 1/298.257223563; /* WGS84 */
    struct geod_geodesic g;
    if(!s->h->lcalda) {
        return;
    }
    if(!sac_hdr_defined(s, SAC_STLA, SAC_STLO, SAC_EVLA, SAC_EVLO, NULL)) {
        return;
    }
    geod_init(&g, a, f);
    sac_get_float(s, SAC_EVLA, &lat1);
    sac_get_float(s, SAC_EVLO, &lon1);
    sac_get_float(s, SAC_STLA, &lat2);
    sac_get_float(s, SAC_STLO, &lon2);
    degrees = geod_geninverse(&g, lat1, lon1, lat2, lon2,
                              &meters, &az1, &az2,
                              0, 0, 0, 0);
    sac_set_float(s, SAC_GCARC, degrees);
    sac_set_float(s, SAC_DIST, meters/1e3);
    sac_set_float(s, SAC_AZ, az1);
    az2 = az2 - 180.0;
    if(az2 <= 0.0) {
        az2 += 360.0;
    }
    sac_set_float(s, SAC_BAZ, az2);
}
#else
/**
 * @brief Function prototype for sac version of computing distance and azimuth
 * @private
 */
void distaz(double the, double phe, float *ths, float *phs,
            int ns, float *dist, float *az, float *baz, float *xdeg,
            int *nerr);
/**
 * @brief      Update the dist, az, gcarc, baz header fields
 *
 * @details    Update the dist, az, gcarc, baz header fields of a sac file
 *             using the sac's method.  Header updates are performed
 *             if stlo, stla, evlo, evla are defined.
 *
 * @note       This is only used if the actual sacio library is available
 *             from the IRIS SAC distribution.
 *
 * @ingroup    sac
 * @memberof   sac
 * @private
 *
 * @param      s  sac file to update
 *
 */
void
update_distaz(sac * s) {
    float d,a,b,g;
    int ndaerr = 0;
    //DEBUG("\n");
    double sx, sy, ex, ey;
    float fsx, fsy;
    sac_get_float(s, SAC_STLO, &sx);
    sac_get_float(s, SAC_STLA, &sy);
    sac_get_float(s, SAC_EVLO, &ex);
    sac_get_float(s, SAC_EVLA, &ey);
    if (s->h->lcalda && sx != SAC_FLOAT_UNDEFINED &&
        sy != SAC_FLOAT_UNDEFINED && ex != SAC_FLOAT_UNDEFINED
        && ey != SAC_FLOAT_UNDEFINED) {
        //DEBUG("compute\n");
        /* These temporary values are necessary as distaz() will not
         * calculate an output if the input is < 0
         */
        d = a = b = g = 0;
        fsx = (float) sx;
        fsy = (float) sy;
        distaz(ey, ex,
               (float *) &fsy, (float *) &fsx,
               1,
               (float *) &d, (float *) &a, (float *) &b, (float *) &g,
               &ndaerr);
        sac_set_float(s, SAC_DIST, d);
        sac_set_float(s, SAC_GCARC, g);
        sac_set_float(s, SAC_AZ, a);
        sac_set_float(s, SAC_BAZ, b);
        if (sx == ex && sy == ey) {
            sac_set_float(s, SAC_AZ, 0.0);
            sac_set_float(s, SAC_BAZ, 0.0);
        }
        if (ndaerr) {
            sac_set_float(s, SAC_DIST, SAC_FLOAT_UNDEFINED);
            sac_set_float(s, SAC_GCARC, SAC_FLOAT_UNDEFINED);
            sac_set_float(s, SAC_AZ, SAC_FLOAT_UNDEFINED);
            sac_set_float(s, SAC_BAZ, SAC_FLOAT_UNDEFINED);
        }
    }
    return;
}
#endif

/**
 * @brief      copy the sac header
 *
 * @details    copy the sac header from \p from to \p to
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @param      to     destination
 * @param      from   source
 *
 */
void
sac_header_copy(sac * to, sac * from) {
    memmove(to->h, from->h, sizeof(sac_hdr));
    memmove(to->z, from->z, sizeof(sac_f64));
}
/**
 * @brief      copy the sac meta header
 *
 * @details    copy the sac meta header from \p from to \p to
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @param      to     destination
 * @param      from   source
 *
 */
void
sac_meta_copy(sac *to, sac *from) {
    to->m->swap      = from->m->swap;
    to->m->filename  = strdup(from->m->filename);
    to->m->data_read = from->m->data_read;
    to->m->nstop     = from->m->nstop;
    to->m->nstart    = from->m->nstart;
    to->m->nfillb    = from->m->nfillb;
    to->m->nfille    = from->m->nfille;
    to->m->ntotal    = from->m->ntotal;
}

/**
 * @brief      copy sac data
 *
 * @details    copy sac data from one file to another
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @param      to     destination
 * @param      from   source
 *
 * @return     return type
 */
void
sac_data_copy(sac *to, sac *from) {
    size_t n = 0;
    if(from->h->npts <= 0) {
        return;
    }
    if(from->y) {
        sac_alloc(to);
        n = sizeof(float) * (size_t) from->h->npts;
        memcpy(to->y, from->y, n);
        if(sac_comps(to) == 2 && from->x) {
            memcpy(to->x, from->x, n);
        }
    }
}

/**
 * @brief      copy a sac file
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    copy a sac file: header, meta, and data
 *
 * @param      s    sac file to copy
 *
 * @return     copy of sac file
 */
sac *
sac_copy(sac *s) {
    sac *new;
    new = sac_new();
    sac_header_copy(new, s);
    sac_meta_copy(new, s);
    sac_data_copy(new, s);
    return new;
}


/**
 * @brief      Check the number of npts in a sac file
 *
 * @details    Check the number of npts in a sac file
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @param      npts   Number of points
 *
 * @return     - 0 if  npts > 0
 *             - 115 if the npts <= 0
 */
int
sac_check_npts(int npts) {
    if (npts <= 0) {
        return ERROR_WRITING_FILE;
    }
    return SAC_OK;
}

/**
 * @brief Size of header
 */
#define SAC_HEADER_SIZE 632

/**
 * @brief      Get the size of a sac file in bytes
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Get the size of a sac file in bytes if stored on disk
 *
 * @param      s    sac file
 *
 * @return     size of file in bytes
 */
off_t
sac_size(sac *s) {
    off_t size = 0;
    if(s->h->npts < 0) {
        return 0;
    }
    size = SAC_HEADER_SIZE + MAX(0, 4 * (off_t) s->h->npts * (off_t) sac_comps(s));
    if(s->h->nvhdr == SAC_HEADER_VERSION_7) {
        size += sizeof(sac_f64);
    }
    return size;
}


/**
 * @brief      Check the overwrite flag
 *
 * @details    Check the overwrite flat
 *
 * @param      lovrok  - overwrite flag (L - Logical, ovr - OVerwRite, ok - OK)
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @return     - 0 if overwrite flag is != 0
 *             - 1303 if overwrite flag is 0
 */
int
sac_check_lovrok(int lovrok) {
    if (!lovrok) {
        return ERROR_OVERWRITE_FLAG_IS_OFF;
    }
    return SAC_OK;
}

/**
 * @brief      Get the number of data components
 *
 * @details    Get the number of data components:
 *             1 - Evenly spaced time (ITIME), general xy (IXY)
 *                 or unknown (IUKN), xyz (IXYZ)
 *             2 - Unevenly spcaed time (ITIME), real-imag (IRLIM),
 *                 amp-phase (IAMPH)
 *
 * @param      s   sac file
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @return     Number of data components
 */
int
sac_comps(sac * s) {
    int n = 0;
    switch (s->h->iftype) {
        case ITIME:
        case IXY:
        case IUNKN:
            n = (s->h->leven) ? 1 : 2;
            break;
        case IXYZ:
            n = 1;
            break;
        case IRLIM:
        case IAMPH:
            n = 2;
            break;
        default:
            fprintf(stderr, "unknown sac data type: %d\n", s->h->iftype);
            n = 2;
            break;
    }
    return n;
}

/**
 * @brief      Get the minimum value in an array
 *
 * @details    Get the minimum value in an array
 *
 * @param      y   array
 * @param      n   length of \p y
 *
 * @return     min value in array
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 */
static float
array_min(float *y, int n) {
    int i = 0;
    float v = y[0];
    for(i = 0; i < n; i++) {
        v = fminf(v, y[i]);
    }
    return v;
}
/**
 * @brief      Get the maximum value in an array
 *
 * @details    Get the maximum value in an array
 *
 * @param      y   array
 * @param      n   length of \p y
 *
 * @return     max value in array
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 */
static float
array_max(float *y, int n) {
    int i = 0;
    float v = y[0];
    for(i = 0; i < n; i++) {
        v = fmaxf(v, y[i]);
    }
    return v;
}
/**
 * @brief      Get the mean value in an array
 *
 * @details    Get the mean value in an array
 *
 * @param      y   array
 * @param      n   length of \p y
 *
 * @return     mean value in array
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 */

static float
array_mean(float *y, int n) {
    int i = 0;
    double v = 0.0;
    for(i = 0; i < n; i++) {
        v += y[i];
    }
    return (float) (v / n);
}

/**
 * @brief      check if min and max values are valid floating point numbers
 *
 * @details    check if min and max values are valid floating point numbers
 *
 * @param      vmin    miniumum value
 * @param      vmax    maxiumum value
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 */
static void
check_value(float vmin, float vmax) {
    if(! isfinite(vmin) || ! isfinite(vmax) || vmin < -3.40282e38 || vmax > 3.40282e38) {
        printf(" WARNING: Data value outside system storage bounds\n");
        printf(" Maxvalue = %.5g ", vmax);
        printf(" Minvalue = %.5g\n", vmin);
    }
}

/**
 * @brief      compute depmin, depmax, and depmen in sac header
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    compute depmin, depmax and depmen value for the sac header
 *             report if any of the values are not finite numbers, i.e.
 *             inf or nan
 *
 * @param      s   sac file
 *
 * @note       This sets the following sac header values:
 *             - depmin
 *             - depmax
 *             - depmen
 *
 */
void
sac_extrema(sac * s) {
    s->h->depmin = array_min(s->y, s->h->npts);
    s->h->depmax = array_max(s->y, s->h->npts);
    s->h->depmen = array_mean(s->y, s->h->npts);
    check_value(s->h->depmin, s->h->depmax);
}

/**
 * @brief      Get a reference to a character string sac header value
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Return a pointer to the first character of a particular
 *             sac header value.
 *
 * @param      s   sac file
 * @param      k   character string header id. Conversion from header id
 *                 is done by \f$kid = hid - SAC_STA + 1\f$
 *                 - kstnm  = 1
 *                 - kevnm  = 2
 *                 - khole  = 4
 *                 - ko     = 5
 *                 - ka     = 6
 *                 - kt0    = 7
 *                 - kt1    = 8
 *                 - kt2    = 9
 *                 - kt3    = 10
 *                 - kt4    = 11
 *                 - kt5    = 12
 *                 - kt6    = 13
 *                 - kt7    = 14
 *                 - kt8    = 15
 *                 - kt9    = 16
 *                 - kf     = 17
 *                 - kuser0 = 18
 *                 - kuser1 = 19
 *                 - kuser2 = 20
 *                 - kcmpnm = 21
 *                 - knetwk = 22
 *                 - kdatrd = 23
 *                 - kinst  = 24
 *
 * @return     return type
 */
char *
khdr(sac * s, int k) {
    char *p;
    switch (k) {
        case 1:
            p = s->h->kstnm;
            break;
        case 2:
        case 3:
            p = s->h->kevnm;
            break;
        case 4:
            p = s->h->khole;
            break;
        case 5:
            p = s->h->ko;
            break;
        case 6:
            p = s->h->ka;
            break;
        case 7:
            p = s->h->kt0;
            break;
        case 8:
            p = s->h->kt1;
            break;
        case 9:
            p = s->h->kt2;
            break;
        case 10:
            p = s->h->kt3;
            break;
        case 11:
            p = s->h->kt4;
            break;
        case 12:
            p = s->h->kt5;
            break;
        case 13:
            p = s->h->kt6;
            break;
        case 14:
            p = s->h->kt7;
            break;
        case 15:
            p = s->h->kt8;
            break;
        case 16:
            p = s->h->kt9;
            break;
        case 17:
            p = s->h->kf;
            break;
        case 18:
            p = s->h->kuser0;
            break;
        case 19:
            p = s->h->kuser1;
            break;
        case 20:
            p = s->h->kuser2;
            break;
        case 21:
            p = s->h->kcmpnm;
            break;
        case 22:
            p = s->h->knetwk;
            break;
        case 23:
            p = s->h->kdatrd;
            break;
        case 24:
            p = s->h->kinst;
            break;
        default:
            p = NULL;
            break;
    }
    return p;
}

/**
 * @brief      check the precision of a value against the time sampling
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    check the precision of a value against the time sampling
 *             Check is made by computing the difference between \p val
 *             and the next floating value and that difference to the
 *             time sampling.  Uses the nextafterf from
 *             https://en.cppreference.com/w/c/numeric/math/nextafter
 *
 * @param      dt   time sampling
 * @param      val  value to check
 *
 * @return     difference between floating point values if larger than
 *             time sampling, 0.0 otherwise
 *
 */
double
check_precision(float dt, float val) {
    float df = val - nextafterf(val,0.0);
    if(df > dt) {         // Return difference if larger than sampling rate
        return df;
    }
    return 0.0;           // Otherwise return 0.0
}

/**
 * @brief      check the time precision of all timing values in the sac header
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    check all time related values in the sac header for loss of
 *             precision.  If the delta is undefined of the file type is
 *             \p ITIME, the function returns immediately. This routine only
 *             prints out a message if precision loss is detected.  Header
 *             values checked are b, e, a, o, f, t0 - t9
 *
 * @param      h     pointer to the sac header
 *
 *
 */
void
sac_check_time_precision(sac *s) {
    int i = 0;
    double df, v, dt;
    char *names[] = {"b","e","a","o","fmt",
                     "t0","t1","t2","t3","t4","t5","t6","t7","t8","t9","f"};
    if(s->h->nvhdr == SAC_HEADER_VERSION_7 ||
       s->h->_delta == SAC_FLOAT_UNDEFINED ||
       s->h->iftype != ITIME) {
        return;
    }
    sac_get_float(s, SAC_DELTA, &dt);
    for(i = SAC_B; i <= SAC_T9; i++) {
        if(i == SAC_FMT) { continue; }
        sac_get_float(s, i, &v);
        if(v == SAC_FLOAT_UNDEFINED) {
            continue;
        }
        if((df = check_precision((float) dt, (float) v)) != 0) {
            printf(" WARNING:  minimum precision > sampling rate: %s = %f\n"
                "       sampling rate (delta):      %f\n"
                "       32-bit minimum precision:   %f\n",
                names[i-SAC_B], v, dt, df);
            //outmsg();
            //clrmsg();
        }
    }
}

/**
 * @brief      byteswap data
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    byteswap data of 2, 4, or 8 bytes in place
 *
 * @param      v    data to be swapped
 * @param      n    size of data
 *
 */
static void
byteswap_bsd(void *v, size_t n) {
    size_t i = 0;
    char t = 0;
    char *p = v;
    for(i = 0; i < n/2; i++) {
        size_t j = n-i-1;
        t = p[i];
        p[i] = p[j];
        p[j] = t;
    }
}

/**
 * @brief Swap the Sac Header
 *
 * @details Swap all floating point and integer values of the sac header
 *
 * @private
 * @memberof sac
 * @ingroup  sac
 *
 * @param hdr    Sac Header to Swap, packed
 *
 * @date July 01, 2007 Initial Version -- B. Savage
 *
 */
void
sac_header_swap(void *hdr) {
    int i;
    float *ptr;
    for (i = 0, ptr = hdr; i < SAC_HEADER_NUMBERS; i++, ptr++) {
        byteswap_bsd(ptr, SAC_HEADER_SIZEOF_NUMBER);
    }
}

/**
 * @brief      Swap an array of data in place
 *
 * @details    Swap an array of data in place,
 *             typically the data component of a sac file
 *
 * @private
 * @memberof sac
 * @ingroup  sac
 *
 * @param      y   array to to be swapped in place
 * @param      n   length of \p y
 *
 */
void
sac_data_swap(float *y, int n) {
    int i;
    float *ptr;
    for (i = 0, ptr = y; i < n; i++, ptr++) {
        byteswap_bsd((void *) ptr, SAC_HEADER_SIZEOF_NUMBER);
    }
}

/**
 * @brief      Copy strings from a sac file to a sac structure in memory
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Copy character string from a sac file to a sac structure
 *             in memory, appending C-termination characters at the end
 *             This is the inverse of sac_copy_strings_strip_terminator().
 *             Input \p src data is assumed to be concatenated unterminated
 *             character strings as exists for a sac file saved to disk
 *
 *
 * @param      s     Where to place the character strings with terminators
 * @param      src   Packed Input character strings without terminators
 *
 */
void
sac_copy_strings_add_terminator(sac *s, char *src) {
    int i = 0;
    void *p = src;
    for(i = SAC_STA; i <= SAC_INST; i++) {
        if(i == SAC_EVENT2) {
            continue;
        }
        char *dst = khdr(s, i-SAC_STA+1);
        size_t n = (i == SAC_EVENT) ? 16 : 8;
        memcpy(dst, p, n);
        dst[n] = 0;
        p += n;
    }
}

/**
 * @brief      Copy strings from a sac file to a packed string
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Copy character strings from a sac structure in memory
 *             to a concatenated character string as would exist in a sac
 *             file on disk.  Terminators are removed and strings are appended
 *             in order as they appear on disk.  This is the inverse operation
 *             of sac_copy_strings_add_terminator().  Order is important.
 *
 * @param      s     sac file to take strings from
 * @param      dst   concatenated character string to place unterminated
 *                   character string into.  
 *
 */
void
sac_copy_strings_strip_terminator(sac *s, char *dst) {
    int i = 0;
    void *p = dst;
    for(i = SAC_STA; i <= SAC_INST; i++) {
        if(i == SAC_EVENT2) {
            continue;
        }
        size_t n = (i == SAC_EVENT) ? 16 : 8;
        memcpy(p, khdr(s, i-SAC_STA+1), n);
        p += n;
    }
}

/**
 * @brief Write a Sac Header
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @param nun   Logical file unite to write SAC Header To
 * @param nerr  Statue code, 0 on success, non-zero on failure
 *
 * @date November 7, 2010
 */
void
sac_header_write(sac *s, int nun, int swap, int *nerr) {
    ssize_t n;
    char temp2[256]; // 8*24 = 192; Should be big enough

    if (swap) {
        sac_header_swap((void *) s->h);
    }
    /* Write the numerical values */
    n = write(nun, (void *) s->h, SAC_HEADER_NUMBERS_SIZE_BYTES_FILE);

    if (n != SAC_HEADER_NUMBERS_SIZE_BYTES_FILE) {
        *nerr = ERROR_WRITING_FILE;
        return;
    }
    if (swap) {
        sac_header_swap((void *) s->h);
    }

    sac_copy_strings_strip_terminator(s, temp2);
    n = write(nun, temp2, SAC_HEADER_STRINGS_SIZE_BYTES_FILE);
    if (n != SAC_HEADER_STRINGS_SIZE_BYTES_FILE) {
        *nerr = ERROR_WRITING_FILE;
        return;
    }
}

/**
 * @brief      Write a single data component
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Write a single data component to a file descriptor
 *
 * @param      nun    File descriptor to write to
 * @param      data   Data to write
 * @param      npts   Length of data to write
 * @param      swap   To swap dat before writing
 * @param      nerr   Status code, 0 on success, non-zero on error
 *
 */
void
sac_data_write1(int nun, float *data, int npts, int swap, int *nerr) {
    ssize_t n;
    if (swap) {
        sac_data_swap(data, npts);
    }
    n = write(nun, data, (size_t) npts * SAC_DATA_SIZE);
    if (n == -1 || n != npts * (int) SAC_DATA_SIZE) {
        *nerr = ERROR_WRITING_FILE;
        return;
    }
    if(swap) {
        sac_data_swap(data, npts);
    }
}

/**
 * @brief      Write two data components to a file descriptor
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Write two data components to a file descriptor,
 *             calls sac_data_write1()
 *
 * @param      nun    file descriptor to write to
 * @param      y      first data component to write
 * @param      x      second data component to write
 * @param      npts   length of \p y and \p x
 * @param      swap   to swap data before writing
 * @param      nerr   status code, 0 on succes, non-zero on failure
 *
 */
void
sac_data_write2(int nun, float *y, float *x, int npts, int swap, int *nerr) {
    sac_data_write1(nun, y, npts, swap, nerr);
    if (*nerr) {
        return;
    }
    sac_data_write1(nun, x, npts, swap, nerr);
}

/**
 * @brief      Write data components to a file descriptor
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Write 1 or 2 data components to a file descriptor, calls
 *             sac_data_write1() or sac_data_write2()
 *
 * @param      nun    file descriptor to write to
 * @param      y      first data component to write
 * @param      x      second data component to write, maybe NULL
 * @param      comps  number of data components to write
 * @param      npts   length of \p y and \p x
 * @param      swap   to swap data before writing
 * @param      nerr   status code, 0 on success, non-zero on failure
 *
 */
void
sac_data_write(int nun, float *y, float *x, int comps, int npts, int swap,
               int *nerr) {
    if (comps == 1) {
        sac_data_write1(nun, y, npts, swap, nerr);
    } else if (comps == 2) {
        sac_data_write2(nun, y, x, npts, swap, nerr);
    } else {
        *nerr = ERROR_WRITING_FILE;
    }
    return;
}

static int v7_keys[] = {
#define X(name,key) SAC_##name ,
    SAC_F64
#undef X
};
static size_t v7_keys_length = sizeof(v7_keys) / sizeof(int);
/**
 * Write the sac header version 7
 *
 * # Arguments
 * - `nun` - Negative file id number (yes, it is negative)
 * - `s` - sac file structure to write out
 * - `nerr` - Error reporting value
 *
 * V7 of the header is a additional set of data at the end of the file
 * following the data.  This routine only reads the this "footer" of
 * metadata, the v6 header I/O routines are still required
 *
 */
void
sac_header_write_v7(int nun, sac *s, int *nerr) {
    ssize_t n;
    double buffer[v7_keys_length];
    *nerr = SAC_OK;

    for(size_t i = 0; i < v7_keys_length; i++) {
        sac_get_f64(s, v7_keys[i], &buffer[i]);
    }

    n = write(nun, buffer, sizeof(buffer));
    if(n == -1 || (size_t) n != sizeof(buffer)) {
        *nerr = ERROR_WRITING_FILE;
    }
}

/**
 * @brief     Read the sac header version 7
 *
 * @details   V7 of the header is a additional set of data at the end of the file
 *            following the data.  This routine only reads the this "footer" of
 *            metadata, the v6 header I/O routines are still required
 *
 *            Function places the file pointer at the beginning of the v7 header,
 *            completes the read and then places the file pointer at its previous
 *            position.  It is safe to call before reading data components.
 *
 * @ingroup    sac
 * @memberof   sac
 * @private
 *
 * @param  fp    File Pointer to open sac file for reading
 * @param  s     sac file structure to fill
 * @param  nerr  Error reporting value
 *
 */
void
sac_header_read_v7(FILE *fp, sac *s, int *nerr) {
    size_t i = 0;
    long offset = 0;
    size_t n = 0;
    double buffer[v7_keys_length];
    *nerr = SAC_OK;

    offset = ftell(fp);
    fseek(fp, -1 * (int)(sizeof(double) * v7_keys_length), SEEK_END);

    n = fread(buffer, sizeof(double), v7_keys_length, fp);
    if(n != v7_keys_length) {
        *nerr = ERROR_READING_FILE;
        return;
    }
    for(i = 0; i < v7_keys_length; i++) {
        sac_set_f64(s, v7_keys[i], buffer[i]);
    }
    fseek(fp, offset, SEEK_SET);
}

/**
 * @brief      Fill the v7 header
 *
 * @details    Fill the v7 header through either reading the v7 header
 *             or copying v6 header values to the v7 header.  Reading of
 *             the v7 header will only occur if the header version equals 7.
 *
 * @ingroup    sac
 * @memberof   sac
 * @private
 *
 * @param      s      sac file
 * @param      fp     file begin read from
 * @param      nerr   0 on success, non-zero on error
 *
 * @return     return type
 */
void
sac_header_v7_fill(sac *s, FILE *fp, int *nerr) {
    switch(s->h->nvhdr) {
    case SAC_HEADER_VERSION_7:
        sac_header_read_v7(fp, s, nerr);
        sac_copy_f64_to_f32(s);
        break;
    case SAC_HEADER_VERSION_6:
        sac_copy_f32_to_f64(s);
        break;
    }

}

/**
 * @brief      internal sac data writing function
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    internal sac data writing function.
 *             - Checks the number of points
 *             - checks if the overwrite flag is "ok"
 *             - updates the extra values (depmin, depmax, depmen)
 *             - updates the distance fields (dist, az, baz, gcarc)
 *             - checks and reports any time precison issues
 *             - create a new file if writing data or update the header of
 *               and existing file
 *             - write the header
 *             - write the data
 *
 * @param      s           sac file to write
 * @param      filename    filename to write to
 * @param      write_data  whether to write data or just the header
 * @param      swap        whether to swap the data and header before writing
 * @param      nerr        status code, 0 on success, non-zero on failure
 *
 */
void
sac_write_internal(sac *s, char *filename, int write_data, int swap, int *nerr) {
    int nin = 0;
    if(write_data && ((*nerr = sac_check_npts(s->h->npts)) != SAC_OK)) {
        return;
    }
    if((*nerr = sac_check_lovrok(s->h->lovrok)) != SAC_OK) {
        return;
    }
    if(write_data) {
        sac_extrema(s);
    }
    update_distaz(s);

    sac_check_time_precision(s);

    if(write_data) {
        nin = creat(filename, 0666);
    } else {
        nin = open(filename, O_RDWR, 0666);
        swap = s->m->swap;
    }
    if(nin < 0) {
        if(!write_data) {
            *nerr = ERROR_FILE_DOES_NOT_EXIST;
        } else {
            *nerr = ERROR_OPENING_FILE;
        }
        return;
    }
    switch(s->h->nvhdr) {
    case SAC_HEADER_VERSION_7:  sac_copy_f64_to_f32(s); break;
    case SAC_HEADER_VERSION_6: break;
    }
    sac_header_write(s, nin, swap, nerr);
    if(*nerr != SAC_OK) {
        return;
    }
    if(write_data) {
        sac_data_write(nin, s->y, s->x, sac_comps(s), s->h->npts, swap, nerr);
        if(*nerr != SAC_OK) {
            return;
        }
    } else {
        lseek(nin,
              SAC_HEADER_SIZE +
              4 * (off_t) s->h->npts * sac_comps(s),
              SEEK_SET);
    }
    if(s->h->nvhdr == SAC_HEADER_VERSION_7) {
        sac_header_write_v7(nin, s, nerr);
    }

    close(nin);
}
/**
 * @brief      read sac data from a file pointer
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    read sac data from a file pointer.  Memory should already
 *             be allocated and data will be swapped if the \p swap flag
 *             is defined in the meta data
 *
 * @param      s    sac data to read data into
 * @param      fp   file pointer to read data from
 *
 * @return     0 on success, -1 if npts < 0, ERROR_READING_FILE on read error
 */
int
sac_data_read(sac *s, FILE *fp) {
    float *p;
    int i;
    size_t n;
    if(s->h->npts <= 0) {
        return -1;
    }
    n = (size_t) s->h->npts;
    for(i = 0; i < sac_comps(s); i++) {
        p = (i == 0) ? s->y : s->x ;
        if(fread(p, sizeof(float), n, fp) != n) {
            return ERROR_READING_FILE;
        }
        if(s->m->swap) {
            sac_data_swap(p, s->h->npts);
        }
    }
    return 0;
}

/**
 * @brief Check the Sac Header Version
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details This routine checks the header version for a number between
 *          1 and \p SAC_HEADER_MAJOR_VERSION.  It check both the swapped
 *          and non-swapped version to determine the byte-order of the file
 *
 * @param hdr    Sac Header to Check
 * @param nerr   status code, 0 on success, non-zero on failure
 *
 * @return if header needs to be swapped or not or is not a sac file
 *    - 1 if file needs to be swapped
 *    - 0 if file does not need to be swapped
 *    - -1 if there was an error checking the header
 *
 */
int
sac_check_header_version(float *hdr, int *nerr) {
    int lswap;
    int *ver;

    lswap = FALSE;
    *nerr = SAC_OK;
    /* determine if the data needs to be swapped. */
    ver = (int *) (hdr + SAC_VERSION_LOCATION);
    if (*ver < 1 || *ver > SAC_HEADER_MAX_VERSION) {
        byteswap_bsd((void *) ver, SAC_HEADER_SIZEOF_NUMBER);

        if (*ver < 1 || *ver > SAC_HEADER_MAX_VERSION) {
            *nerr = ERROR_NOT_A_SAC_FILE;
            printf("not in sac format, nor byteswapped sac format.");
            return -1;
        } else {
            /* swap back, so it can be */
            byteswap_bsd((void *) ver, SAC_HEADER_SIZEOF_NUMBER);
            lswap = TRUE;
        }
    }
    return lswap;
}


/**
 * @brief      Read a sac header from a file pointer
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Read a sac header from a file pointer
 *
 * @param      s   sac file to place header into
 * @param      fp  file pointer to read header from
 *
 * @return     status code, 0 on success, non-zero on failure
 */
int
sac_header_read(sac *s, FILE *fp) {
    int nerr;
    size_t n;
    char str[SAC_HEADER_STRINGS * 8];

    n = SAC_HEADER_NUMBERS;
    if(fread((char *) s->h, sizeof(float), n, fp) != n) {
        return ERROR_NOT_A_SAC_FILE;
    }
    s->m->swap = sac_check_header_version((float *) s->h, &nerr);
    if(nerr) {
        return ERROR_NOT_A_SAC_FILE;
    }
    if(s->m->swap) {
        sac_header_swap((float *) s->h);
    }

    if(fread(str, 8, SAC_HEADER_STRINGS, fp) != SAC_HEADER_STRINGS) {
        return ERROR_READING_FILE;
    }
    sac_copy_strings_add_terminator(s, str);

    return 0;
}

static sac *
sac_read_header_internal(char *filename, int *nerr, FILE **fp) {
    off_t size = 0;
    sac *s = NULL;
    struct stat stbuf;

    if(!filename) {
        *nerr = ERROR_FILE_DOES_NOT_EXIST;
        return NULL;
    }

    if(stat(filename, &stbuf) != 0) {
        switch(errno) {
        case EACCES:       /* Permission         */
        case EIO:          /* I/O Error          */
        case ELOOP:        /* Too many sym links */
        case ENOTDIR:      /* Not a Directory    */
        case ENAMETOOLONG: /* Name too long      */
        case EFAULT:       /* Invalid Address    */
            *nerr = ERROR_READING_FILE;
            break;
        case ENOENT:       /* File does not exit */
            *nerr = ERROR_FILE_DOES_NOT_EXIST;
        }
        return NULL;
    }

    if(!(*fp = fopen(filename, "rb"))) {
        *nerr = ERROR_FILE_DOES_NOT_EXIST;
        return NULL;
    }

    size = stbuf.st_size;

    s = sac_new();

    s->m->filename = strdup(filename);
    *nerr = sac_header_read(s, *fp);
    if(*nerr) {
        goto error;
    }

    // Check actual size versus expected size of the file
    if(sac_size(s) != size) {
        printf("sacio: Actual file size %lld != expected file size: %lld [npts: %d]\n",
               (long long int) size, (long long int) sac_size(s), s->h->npts);
        *nerr = ERROR_NOT_A_SAC_FILE;
        goto error;
    }

    return s;
 error:
    if(s) {
        sac_free(s);
        s = NULL;
    }
    return NULL;
}

static void
sac_read_post(sac *s, int read_data) {
    sac_be(s);
    update_distaz(s);
    if(read_data) {
        sac_extrema(s);
    }
    sac_check_time_precision(s);
}

/**
 * @brief      Read a sac file
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Read a sac file. Header values b, e, dist, az, baz, gcarc
 *             updated after read.  Data is read from either byte order
 *             and converted to the system's byte order
 *
 * @param      filename   file to read sac file from
 * @param      read_data  whether to read the header and data or just the header
 * @param      nerr       status code, 0 on success, non-zero on failure
 *
 * @return     sac file structure, NULL on failure
 */
sac *
sac_read_internal(char *filename, int read_data, int *nerr) {
    FILE *fp = NULL;
    sac *s = NULL;

    *nerr = 0;

    if(!(s = sac_read_header_internal(filename, nerr, &fp))) {
        goto error;
    }

    if(read_data) {
        sac_alloc(s);
        s->m->nstart = 1;
        s->m->nstop  = s->h->npts;
        s->m->ntotal = s->h->npts;
        s->m->nfillb = 0;
        s->m->nfille = 0;
        if((*nerr = sac_data_read(s, fp))) {
            goto error;
        }
    }

    sac_header_v7_fill(s, fp, nerr);

    fclose(fp);

    sac_read_post(s, read_data);

    return s;

 error:
    if(s) {
        sac_free(s);
        s = NULL;
    }
    if(fp) {
        fclose(fp);
    }
    return NULL;
}

/**
 * @brief      initialize / fill a sac header
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    initialize / fill a sac header with default values
 *             - nvhdr  = \p SAC_HEADER_MAJOR_VERSION
 *             - level  = TRUE
 *             - lpspol = FALSE
 *             - lovrok = TRUE
 *             - lcalda = TRUE
 *             - level  = ITIME
 *             - all other values are set to undefined, -12345
 *
 * @param      sh   pointer sac header
 *
 */
void
sac_hdr_init(sac_hdr *sh) {
    if (sh) {
        memcpy(sh, &NullSacHeader, sizeof(NullSacHeader));
        sh->nvhdr = SAC_HEADER_MAJOR_VERSION;

        sh->leven = TRUE;
        sh->lpspol = FALSE;
        sh->lovrok = TRUE;
        sh->lcalda = TRUE;
        sh->unused27 = TRUE;
        sh->iftype = ITIME;
    }
}

/**
 * @brief      Convert a time to index
 *
 * @details    Convert a time value to a data point index. All values are
 *             relative to the `b` value 
 *
 * @param      s   sac file
 * @param      t   time value
 *
 * @return     index of data point associated with the time
 */
int
sac_time_to_index(sac *s, double t) {
    double b, dt;
    sac_get_float(s, SAC_B, &b);
    sac_get_float(s, SAC_DELTA, &dt);
    return (int) lround((t-b)/dt);
}



/**
 * @brief    Start Time is before Begin value (B)
 * @private
 */
#define START_BEFORE 1<<0
/**
 * @brief    Start Time is between Begin (B) and End Value (E)
 * @private
 */
#define START_INSIDE 1<<1
/**
 * @brief    Start Time is after End value (E)
 * @private
 */
#define START_AFTER  1<<2
/**
 * @brief    End Time is before Start time (B)
 * @private
 */
#define END_BEFORE   1<<3
/**
 * @brief    End Time is between Begin (B) and End Value (E)
 * @private
 */
#define END_INSIDE   1<<4
/**
 * @brief    End Time is after End Value (E)
 * @private
 */
#define END_AFTER    1<<5

/**
 * @brief      characterize a cut window
 *
 * @details    characterize a cut window by determining the the location
 *             of the start and end of the cut window with the data
 *
 * @param      s   sac file
 *
 * @return     (START_BEFORE | START_INSIDE | START_AFTER) &
 *             (END_BEFORE | END_INSIDE | END_AFTER)
 */
static int
window_overlap(sac *s) {
    int f = 0;
    if(s->m->nstart < 1) {
        f |= START_BEFORE;
    } else if(s->m->nstart > s->h->npts) {
        f |= START_AFTER;
    } else {
        f |= START_INSIDE;
    }
    if(s->m->nstop < 1) {
        f |= END_BEFORE;
    } else if(s->m->nstop > s->h->npts) {
        f |= END_AFTER;
    } else {
        f |= END_INSIDE;
    }
    return f;
}

/**
 * @brief      get time value
 *
 * @details    get time value from the sac header
 *
 * @param      s     sac file
 * @param      c     time pick or "Z"
 *                   Z, B, E, O, A, F, T0 ... T9
 * @param      nerr  1301 on error, 0 on success
 *
 * @return     time value
 */
double
sac_pick_ref_time(sac *s, char *c, int *nerr) {
    double r = 0.0;
    *nerr = 0;
    switch(c[0]) {
    case 'Z': r = 0.0; break;
    case 'B': r = sac_float(s, SAC_B); break;
    case 'E': r = sac_float(s, SAC_E); break;
    case 'O': r = sac_float(s, SAC_O); break;
    case 'A': r = sac_float(s, SAC_A); break;
    case 'F': r = sac_float(s, SAC_F); break;
    case 'T':
        switch(c[1]) {
        case '0': r = sac_float(s, SAC_T0); break;
        case '1': r = sac_float(s, SAC_T1); break;
        case '2': r = sac_float(s, SAC_T2); break;
        case '3': r = sac_float(s, SAC_T3); break;
        case '4': r = sac_float(s, SAC_T4); break;
        case '5': r = sac_float(s, SAC_T5); break;
        case '6': r = sac_float(s, SAC_T6); break;
        case '7': r = sac_float(s, SAC_T7); break;
        case '8': r = sac_float(s, SAC_T8); break;
        case '9': r = sac_float(s, SAC_T9); break;
        default: *nerr = 1301; break;
        }
        break;
    default: *nerr = 1301; break;
    }
    return r;
}

/**
 * @brief      calculate read window specifics
 *
 * @details    calculate read window values including:
 *             - nread, offt, and skip
 *             - nfille, nfillb, nstart, nstop (in sac meta)
 *
 * @param      s       sac file
 * @param      c1      reference time pick for start
 * @param      t1      relative time from time pick `c1`
 * @param      c2      reference time pick for end
 * @param      t2      relative time ffrom time pick `c2`
 * @param      cutact  Behavior of cut
 *                     - CutNone = 0
 *                     - CutFatal = 1
 *                     - CutUseBe = 2
 *                     - CutFillZero = 3
 * @param      nread   Number of point to read from file
 * @param      offt    Points to fill with zero before writing
 * @param      skip    Points to skip before reading
 * @param      nerr    Status code
 *
 * @return     1 on success, 0 on failure
 */
static int
sac_calc_read_window(sac *s, char *c1, double t1, char *c2, double t2, enum CutAction cutact, int *nread, int *offt, int *skip, int *nerr) {
    *nerr = 0;
    if(cutact == CutNone) {
        s->m->nfillb = 0;
        s->m->nfillb = 0;
        s->m->nstart = 1;
        s->m->nstop  = s->h->npts;
        *nread = s->h->npts;
        *offt  = 0;
        *skip  = 0;
    } else {
        double r1 = sac_pick_ref_time(s, c1, nerr);
        if(*nerr) {
            goto error;
        }
        double r2 = sac_pick_ref_time(s, c2, nerr);
        if(*nerr) {
            goto error;
        }

        if(r1 == SAC_FLOAT_UNDEFINED) {
            printf(" WARNING: Undefined starting cut for file %s\n"
                   " Corrected by using file begin.\n", s->m->filename);
        }
        if(r2 == SAC_FLOAT_UNDEFINED) {
            printf(" WARNING: Undefined starting cut for file %s\n"
                   " Corrected by using file end.\n", s->m->filename);
        }
        t1 = (r1 == SAC_FLOAT_UNDEFINED) ? B(s) : r1 + t1;
        t2 = (r2 == SAC_FLOAT_UNDEFINED) ? E(s) : r2 + t2;

        s->m->nstart = sac_time_to_index(s, t1) + 1;
        s->m->nstop  = sac_time_to_index(s, t2) + 1;

        if(t1 >= t2) {
            printf(" WARNING: Start cut greater than stop cut for file %s\n"
                   "\ttime:  %f >= %f\n"
                   "\tindex: %d >= %d\n", s->m->filename,
                   t1, t2,
                   s->m->nstart, s->m->nstop);
            *nerr = ERROR_START_TIME_GREATER_THAN_STOP;
            goto error;
        }

        int f = window_overlap(s);
        if(cutact == CutUseBE) {
            s->m->nfillb = 0;
            s->m->nfille = 0;
            if(s->m->nstart < 1) {
                s->m->nstart = 1;
                printf(" WARNING: Start cut less than file begin for file %s\n"
                       " Corrected by using file begin.\n", s->m->filename);
            }
            if(s->m->nstop > s->h->npts) {
                s->m->nstop = s->h->npts;
                printf(" WARNING: Stop cut greater than file end for file %s\n"
                       " Corrected by using file end.\n", s->m->filename);
            }
            *nread = s->m->nstop - s->m->nstart + 1;
            *offt  = 0;
            *skip  = MAX(s->m->nstart, 1) - 1;
        } else if(cutact == CutFillZero) {
            s->m->nfillb = MAX(1 - s->m->nstart, 0);
            s->m->nfille = s->m->nstop - s->h->npts;
            *nread = MIN(s->m->nstop, s->h->npts) - MAX(s->m->nstart, 1) + 1;
            *offt  = s->m->nfillb;
            *skip  = MAX(s->m->nstart, 1) - 1;
        } else if(cutact == CutFatal) {
            *nread = s->m->nstop - s->m->nstart + 1;
            *skip  = s->m->nstart-1;
            *offt  = 0;
        }
        if(cutact != CutFillZero) {
            switch (f) {
            case START_BEFORE | END_BEFORE:  *nerr = ERROR_STOP_TIME_LESS_THAN_BEGIN;    goto error; break; // |---| {-------}
            case START_BEFORE | END_INSIDE:  *nerr = ERROR_START_TIME_LESS_THAN_BEGIN;   break; // |-----{---|---}
            case START_BEFORE | END_AFTER:   *nerr = ERROR_CUT_TIMES_BEYOND_DATA_LIMITS; break; // |-----{-------}---|
            case START_INSIDE | END_BEFORE:  *nerr = ERROR_START_TIME_GREATER_THAN_STOP; goto error; break;
            case START_INSIDE | END_INSIDE:  *nerr = SAC_OK;                             break; //       {-|---|-}
            case START_INSIDE | END_AFTER:   *nerr = ERROR_STOP_TIME_GREATER_THAN_END;   break; //       {---|---}---|
            case START_AFTER  | END_BEFORE:  *nerr = ERROR_START_TIME_GREATER_THAN_STOP; goto error; break;
            case START_AFTER  | END_INSIDE:  *nerr = ERROR_START_TIME_GREATER_THAN_STOP; goto error; break;
            case START_AFTER  | END_AFTER:   *nerr = ERROR_START_TIME_GREATER_THAN_END;  break; //       {-------}  |-----|
            }
        }
        if(cutact == CutFatal && (f != (START_INSIDE | END_INSIDE))) {
            switch(*nerr) {
            case ERROR_CUT_TIMES_BEYOND_DATA_LIMITS:
            case ERROR_START_TIME_LESS_THAN_BEGIN:
                printf(" WARNING: Start cut less than file begin for file %s\n", s->m->filename);
                break;
            case ERROR_STOP_TIME_GREATER_THAN_END:
                printf(" WARNING: Stop cut greater than file end for file %s\n", s->m->filename);
                break;
            }
            goto error;
        }
        if(*nread <= 0 && cutact != CutFillZero) {
            goto error;
        }

        sac_set_float(s, SAC_B, B(s) + DT(s) * (double) (s->m->nstart - 1));
        int npts = s->m->nstop - s->m->nstart + 1;
        if(npts <= 0) {
            goto error;
        }
        sac_set_int(s, SAC_NPTS, npts);
        sac_be(s);
    }
    return 1;
 error:
    return 0;

}

/**
 * @brief      read a sac file while cutting
 *
 * @details    read a sac file while cutting
 *
 * @param      filename  sac file to read
 * @param      c1        reference time pick for start
 * @param      t1        relative time from time pick `c1`
 * @param      c2        reference time pick for end
 * @param      t2        relative time ffrom time pick `c2`
 * @param      cutact    Behavior of cut
 *                       - CutNone = 0
 *                       - CutFatal = 1
 *                       - CutUseBe = 2
 *                       - CutFillZero = 3
 * @param      nerr      Status code, 0 on success, non-zero on Error
 *
 * @return     read and cut file on success, NULL on error
 */
sac *
sac_read_with_cut(char *filename,
                  char* c1, double t1,
                  char* c2, double t2,
                  enum CutAction cutact, int *nerr) {
    FILE *fp = NULL;
    sac *s = NULL;
    int nread = 0, offt = 0;
    int skip = 0;
    if(cutact != CutNone && (!isfinite(t1) || !isfinite(t2))) {
        *nerr = ERROR_START_TIME_GREATER_THAN_STOP;
        goto error;
    }

    if(!(s = sac_read_header_internal(filename, nerr, &fp))) {
        goto error;
    }
    if(! s->h->leven ) {
        *nerr = ERROR_CANT_CUT_UNEVENLY_SPACED_FILE;
        goto error;
    }
    if(s->h->iftype != ITIME) {
        *nerr = ERROR_CANT_CUT_SPECTRAL_FILE;
        goto error;
    }

    sac_header_v7_fill(s, fp, nerr);

    if(!sac_calc_read_window(s, c1, t1, c2, t2, cutact,
                             &nread, &offt, &skip, nerr)) {
        goto error;
    }

    sac_alloc(s);

    if(skip > 0) {
        fseek(fp,  skip * (int) SAC_DATA_SIZE, SEEK_CUR);
    }
    if(nread > 0) {
        size_t nr = (size_t) nread;
        if(fread(s->y + offt, SAC_DATA_SIZE, nr, fp) != nr) {
            *nerr = ERROR_READING_FILE;
            goto error;
        }
    }
    fclose(fp);

    sac_read_post(s, TRUE);

    return s;
 error:
    if(fp) {
        fclose(fp);
    }
    if(s) {
        sac_free(s);
        s = NULL;
    }
    return NULL;
}

/**
 * @brief      cut raw data
 *
 * @details    cut raw data from `in` to `out`.
 *             Output data if overwritten.
 *
 * @param      in       input data to cut
 * @param      nstart   Where to start cut, can be < 0
 * @param      nstop    Where to stop cut, can be >= npts
 * @param      nfillb   Number of points to zero fill before first point
 * @param      nfille   Number of points to zero fill after last point
 * @param      out      output cut data
 *
 */
static void
cut_data(float *in, int nstart, int nstop, int nfillb, int nfille, float *out) {
    int out_offset, in_offset;
    int n;
    /* Number of data points to cut */
    n = nstop - nstart + 1 - MAX(0, nfillb) - MAX(0, nfille);

    /* Fill full array with zeros */
    if(nstop - nstart + 1 > 0) {
        memset(out, 0, sizeof(float) * (size_t) (nstop - nstart) + 1);
    }

    out_offset = nfillb;
    if (n > 0) {
        in_offset = nstart - 1 + nfillb;
        memcpy(out + out_offset, in + in_offset, (size_t) n * sizeof(float));
    }
}

/**
 * @brief      cut a sac file
 *
 * @details    cut a sac file and return a new sac file
 *
 * @param      sin     sac file
 * @param      c1      reference time pick for start time
 * @param      t1      relative time from `c1`
 * @param      c2      reference time pick for end time
 * @param      t2      relative time from `c2`
 * @param      cutact  Behavior of cut
 *                     - CutNone = 0
 *                     - CutFatal = 1
 *                     - CutUseBe = 2
 *                     - CutFillZero = 3
 * @param      nerr    status code
 *                     - 0 on success
 *                     - Might be non-zero when using CutUseBE
 *
 * @return     newly cut sac file
 */
sac *
sac_cut(sac *sin, char *c1, double t1, char *c2, double t2, enum CutAction cutact, int *nerr) {
    int j = 0;
    int nread = 0, offt = 0, skip = 0;
    sac *s = NULL;
    if(!sin) {
        return NULL;
    }
    if(cutact != CutNone && (!isfinite(t1) || !isfinite(t2))) {
        *nerr = ERROR_START_TIME_GREATER_THAN_STOP;
        goto error;
    }
    if(! sin->h->leven ) {
        *nerr = ERROR_CANT_CUT_UNEVENLY_SPACED_FILE;
        goto error;
    }
    if(sin->h->iftype != ITIME) {
        *nerr = ERROR_CANT_CUT_SPECTRAL_FILE;
        goto error;
    }

    s = sac_new();
    sac_header_copy(s, sin);
    sac_meta_copy(s, sin);

    if(!sac_calc_read_window(s, c1, t1, c2, t2, cutact,
                             &nread, &offt, &skip, nerr)) {
        goto error;
    }
    sac_alloc(s);
    for(j = 0; j < sac_comps(s); j++) {
        float *oldy = (j == 0) ? sin->y : sin->x;
        float *newy = (j == 0) ? s->y   : s->x;
        cut_data(oldy, s->m->nstart, s->m->nstop, s->m->nfillb, s->m->nfille, newy);
    }
    sac_extrema(s);
    sac_be(s);
    return s;

 error:
    if(s) {
        sac_free(s);
        s = NULL;
    }
    return NULL;
}

/**
 * @brief      Create a new sac meta component
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Create and initialize a new sac meta component
 *
 * @return     sac meta component
 */
sacmeta *
sac_meta_new() {
    sacmeta *m;
    m = (sacmeta *) malloc(sizeof(sacmeta));
    if (m) {
        m->swap = FALSE;
        m->filename = NULL;
        m->data_read = TRUE;
        m->nstop = 0;
        m->nstart = 0;
        m->nfillb = 0;
        m->nfille = 0;
        m->ntotal = 0;
    }
    return m;
}

/**
 * @brief      Calculate the end time for an evenly spaced file
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Calculate the end time for an evenly spaced file.
 *             For a time domain file, \f$ e = b + dt * (npts-1) \f$
 *             For a frequency domain file, \f$ e = b + nfreq * df\f$
 *             where nfreq = npts/2 or (npts-1)/2 if npts is even or odd
 *
 * @param      s   sac file
 *
 * @return     end time
 */
double
calc_e_even(sac *s) {
    double b, dt;
    sac_get_float(s, SAC_B, &b);
    sac_get_float(s, SAC_DELTA, &dt);
    switch (s->h->iftype) {
    case ITIME:
    case IXY:
    case IUNKN:
        return b + dt * (double)(s->h->npts - 1);
        break;
    case IRLIM:
    case IAMPH: {
        int nfreq = 0;
        if(s->h->npts % 2 == 0) {
            nfreq = s->h->npts / 2;
        } else {
            nfreq = (s->h->npts-1) / 2;
        }
        return b + (double)nfreq * dt;
    }
        break;
    case IXYZ:
        break;
    }
    return SAC_FLOAT_UNDEFINED;
}

/**
 * @brief      Set a character string in a sac file
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Set a character stirng in a sac file
 *
 * @param      s     sac file
 * @param      hdr   Header ID
 * @param      v     character string to set
 *
 * @return     success code, 1 on success, 0 on failure
 */
int
sac_set_string(sac *s, int hdr, char *v) {
    char *k = NULL;
    if(!s || hdr < SAC_STA || hdr > SAC_INST || !v) {
        return 0;
    }
    if(!(k = khdr(s, hdr-SAC_STA+1))) {
        return 0;
    }
    sacio_strlcpy(k, v, (hdr == SAC_EVENT) ? 17 : 9);
    return 1;
}

/**
 * @brief      format a timing mark with name
 *
 * @details    format a timing mark with its name
 *
 * @param      s         sac file
 * @param      time_id   time mark id
 * @param      name_id   name id
 * @param      dst       output character string
 * @param      n         length of \p dst
 *
 * @private
 * @memberof sac
 * @ingroup sac
 */
void
sac_timing_mark(sac *s, int time_id, int name_id, char *dst, size_t n) {
    if(sac_hdr_defined(s, time_id, NULL)) {
        double time = 0.0;
        char name[32] = {0};
        sac_get_float(s, time_id, &time);
        if(sac_hdr_defined(s, name_id, NULL)) {
            sac_get_string(s, name_id, name, sizeof(name));
            snprintf(dst, n, "%16.5g (%s)", time, name);
        } else {
            snprintf(dst, n, "%16.5g", time);
        }
    }
}

/**
 * @brief      Get a character string from a sac file
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Get a character string from a sac file
 *
 * @param      s    sac file
 * @param      hdr  Header ID
 * @param      v    output character string
 * @param      n    length of \p v
 *
 * @return     status code, 0 on failure, 1 on success
 */
int
sac_get_string(sac *s, int hdr, char *v, size_t n) {
    char *k = NULL;
    if(!s || hdr < SAC_STA || hdr > SAC_T9MARKER || !v) {
        return 0;
    }
    if(hdr >= SAC_STA && hdr <= SAC_INST) {
        if(!(k = khdr(s, hdr-SAC_STA+1))) {
            return 0;
        }
        sacio_strlcpy(v, k, n);
    } else {
        switch(hdr) {
        case SAC_DATE: {
            timespec64 t = {0,0};
            sac_get_time_ref(s, &t);
            strftime64t(v, n, "%b %d (%j), %Y", &t);
        } break;
        case SAC_TIME: {
            timespec64 t = {0,0};
            sac_get_time_ref(s, &t);
            strftime64t(v, n, "%H:%M:%S.%03f", &t);
        } break;
        case SAC_FILENAME:
            sacio_strlcpy(v, s->m->filename, n);
            break;
        case SAC_AMARKER: sac_timing_mark(s, SAC_A, SAC_KA, v, n); break;
        case SAC_OMARKER: sac_timing_mark(s, SAC_O, SAC_KO, v, n); break;
        case SAC_T0MARKER: sac_timing_mark(s, SAC_T0, SAC_KT0, v, n); break;
        case SAC_T1MARKER: sac_timing_mark(s, SAC_T1, SAC_KT1, v, n); break;
        case SAC_T2MARKER: sac_timing_mark(s, SAC_T2, SAC_KT2, v, n); break;
        case SAC_T3MARKER: sac_timing_mark(s, SAC_T3, SAC_KT3, v, n); break;
        case SAC_T4MARKER: sac_timing_mark(s, SAC_T4, SAC_KT4, v, n); break;
        case SAC_T5MARKER: sac_timing_mark(s, SAC_T5, SAC_KT5, v, n); break;
        case SAC_T6MARKER: sac_timing_mark(s, SAC_T6, SAC_KT6, v, n); break;
        case SAC_T7MARKER: sac_timing_mark(s, SAC_T7, SAC_KT7, v, n); break;
        case SAC_T8MARKER: sac_timing_mark(s, SAC_T8, SAC_KT8, v, n); break;
        case SAC_T9MARKER: sac_timing_mark(s, SAC_T9, SAC_KT9, v, n); break;
        case SAC_STCMP:
            if(!sac_hdr_defined(s, SAC_STA, NULL)) {
                sacio_strlcpy(v, "UNDEFINED", n);
            }
            if(sac_hdr_defined(s, SAC_CHA, NULL)) {
                sac_fmt(v, n, "%S %C", s);
            } else {
                sac_fmt(v, n, "%S %c", s);
            }
            break;
        }
    }
    return 1;
}

/**
 * @brief      Set a floating point value in a sac file
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Set a floating point value in a sac file.  If the value is in the 64-bit
 *             header, the value is copied into the 32-bit and 64-bit header. If the value
 *             is only in the 32-bit header, the value is only copied there
 *
 * @param      s     sac file
 * @param      hdr   Header ID
 * @param      v     floating point to set
 *
 * @return     success code, 1 on success, 0 on failure
 */
int
sac_set_float(sac *s, int hdr, double v) {
    if(!s || hdr < SAC_DELTA || hdr > SAC_UN70) {
        return 0;
    }
    return sac_set_f64(s, hdr, v);
}
/**
 * @brief      Get a floating point value from a sac file
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Get a floating point value from a sac file. If the header version
 *             is `6`, values are always from the 32-bit header.  If the header
 *             version is `7`, values from the 64-bit header are preferred,
 *             otherwise the 32-bit header values are used.
 *
 * @param      s    sac file
 * @param      hdr  Header ID
 * @param      v    output floating point value
 *
 * @return     status code, 0 on failure, 1 on success
 */
int
sac_get_float(sac *s, int hdr, double *v) {
    if(!s || hdr < SAC_DELTA || hdr > SAC_UN70 || !v) {
        return 0;
    }
    switch(s->h->nvhdr) {
    case SAC_HEADER_VERSION_6: return sac_get_f32(s, hdr, v); break;
    case SAC_HEADER_VERSION_7: return sac_get_f64(s, hdr, v); break;
    default:
        printf("Unknown header version: %d, expected 6 or 7\n", s->h->nvhdr);
        break;
    }
    return 0;
}
/**
 * @brief      Set a integer value in a sac file
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Set a integer in a sac file
 *
 * @param      s     sac file
 * @param      hdr   Header ID
 * @param      v     integer value to set
 *
 * @return     success code, 1 on success, 0 on failure
 */
int
sac_set_int(sac *s, int hdr, int v) {
    if(!s || hdr < SAC_YEAR || hdr > SAC_UN110) {
        return 0;
    }
    int *ip = (int *) (&(s->h->nzyear));
    ip[hdr - SAC_YEAR] = v;
    return 1;
}
/**
 * @brief      Get a integer value from a sac file
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Get a integer value from a sac file
 *
 * @param      s    sac file
 * @param      hdr  Header ID
 * @param      v    output integer value
 *
 * @return     status code, 0 on failure, 1 on success
 */
int
sac_get_int(sac *s, int hdr, int *v) {
    if(!s || hdr < SAC_YEAR || hdr > SAC_UN110 || !v) {
        return 0;
    }
    int *ip = (int *) (&(s->h->nzyear));
    *v = ip[hdr - SAC_YEAR];
    return 1;
}

/** \cond NO_DOCS */
#define is_float(x) ( x >= SAC_DELTA && h <= SAC_UN70 )
#define is_int(x)   ( x >= SAC_YEAR && h <= SAC_UN105 )
#define is_str(x)   ( x >= SAC_STA && h <= SAC_INST )
/** \endcond */

/**
 * @brief      Check if multiple header values are defined
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Check if mulutple header values are defined
 *
 * @param      s    sac file
 * @param      ...  Header id, NULL terminatated
 *
 * @return     0 if any are undefined, 1 if all are defined
 */
int
sac_hdr_defined(sac *s, ...) {
    int h = 0;
    va_list ap;
    if(!s) {
        return 0;
    }
    va_start(ap, s);
    while((h = va_arg(ap, int)) > 0) {
        if( is_float(h) ) {
            double v = 0.0;
            sac_get_float(s, h, &v);
            if(v == SAC_FLOAT_UNDEFINED) {
                return 0;
            }
        }
        else if( is_int(h) ) {
            int v = 0;
            sac_get_int(s, h, &v);
            if(v == SAC_INT_UNDEFINED) {
                return 0;
            }
        }
        else if( is_str(h) ) {
            char v[20] = {0};
            sac_get_string(s, h, v, sizeof(v));
            if(h == SAC_EVENT) {
                if(strcmp(v, SAC_CHAR_UNDEFINED_2) == 0) {
                    return 0;
                }
            } else {
                if(strcmp(v, SAC_CHAR_UNDEFINED) == 0) {
                    return 0;
                }
            }
        } else {
            printf("Warning: unknown header id: %d\n", h);
        }
    }
    va_end(ap);
    return 1;
}

/**
 * @brief     Get the reference time from a sac file
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @details   Get the refereence time from a sac file
 *
 * @param s   sac file
 * @param t   timespec64 output value
 *
 * @return 1 on success, 0 in failure
 */
int
sac_get_time_ref(sac *s, timespec64 *t) {
    if( sac_hdr_defined(s,
                        SAC_YEAR, SAC_DAY, SAC_HOUR, SAC_MIN, SAC_SEC, SAC_MSEC, -1)) {
         *t = timespec64_from_yjhmsf(s->h->nzyear, s->h->nzjday,
                                    s->h->nzhour, s->h->nzmin, s->h->nzsec,
                                    s->h->nzmsec * 1000000);
        return 1;
    }
    return 0;
}

/**
 * @brief      Get time value from sac file
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Get time value from sac file
 *
 *
 * @param s    sac file
 * @param hdr  header index value
 * @param t    timespec64 output value
 *
 * @return 1 in success, 0 on failure
 *
 */
int
sac_get_time(sac *s, int hdr, timespec64 *t) {
    double v = 0.0;
    double fract = 0.0, ipart = 0.0;
    if(!sac_get_time_ref(s, t)) {
        return 0;
    }
    if(!sac_get_float(s, hdr, &v)) {
        return 0;
    }
    fract = modf(v, &ipart);
    t->tv_sec +=  (int64_t) ipart;
    t->tv_nsec += (int) (fract * 1e9);
    return 1;
}

/**
 * Set the reference time for a sac file, assumed to be the origin time
 *
 * @memberof sac
 * @ingroup  sac
 *
 * @param s   sac file
 * @param t   origin time
 *
 * @return 1 on success, 0 on failure
 *
 * @note Reference time is set to the time provided, the time reference
 *   is set to ORIGIN, and all other times are references relative
 *
 * Effective sac commands are:
 *
 *    SAC> ch o gmt 1994 160 00 33 16 230
 *    SAC> ch iztype IO
 *    SAC> ch allt (-1.0 * &1,o)
 *
 */
int
sac_set_time(sac *s, timespec64 t) {
    int j = 0;
    struct TM tm = {0};
    timespec64 t0;
    double dt = 0.0;
    if(sac_get_time_ref(s, &t0)) {
        int64_t dtf = t.tv_sec - t0.tv_sec;
        int64_t dtp = t.tv_nsec - t0.tv_nsec;
        dt = (double) dtf + (double) dtp / 1e9;
    }
    gmtime64_r(&t.tv_sec, &tm);
    s->h->iztype = IO;
    s->h->nzyear = (int) (tm.tm_year + 1900);
    s->h->nzjday = tm.tm_yday + 1;;
    s->h->nzhour = tm.tm_hour;
    s->h->nzmin  = tm.tm_min;
    s->h->nzsec  = tm.tm_sec;
    s->h->nzmsec = (int) (t.tv_nsec / 1000000);
    sac_set_float(s, SAC_O, dt);
    for(j = SAC_B; j <= SAC_F; j++) {
        double v = 0.0;
        sac_get_float(s, j, &v);
        if(v != SAC_FLOAT_UNDEFINED) {
            sac_set_float(s, j, v - dt);
        }
    }
    return 1;
}


/**
 * @brief      Copy a sac character string or "" if undefined
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Copy a sac character string or "" if undefined.
 *             If the \p src value is defined, not "-12345   "
 *             then \p src is copied to \dst and trailing whitespace
 *             is removed
 *
 * @param      dst  Ouptut character string
 * @param      src  Input charater string
 * @param      n    Length of \p dst
 *
 * @return     full length of output dst
 */
size_t
sac_strlcat(char *dst, char *src, size_t n) {
    char tmp[20] = {0};
    if(strcmp(src, SAC_CHAR_UNDEFINED) != 0) {
        sacio_strlcpy(tmp, src, sizeof(tmp));
        sacio_rstrip(tmp);
        return sacio_strlcat(dst, tmp, n);
    }
    return strlen(dst);
}

/**
 * @brief      Copy an absolute time value to a string, if defined
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Copy an absolute time value in the form of
 *             1994/06/09T00:33:16.123 if a refernce time is defined in
 *             the sac file 
 *
 * @param      dst    Output character string
 * @param      s      sac file to get time value from
 * @param      hdr    header ID
 * @param      n      length of output \p dst
 *
 * @return     full length of \p dst
 */
size_t
sac_timelcat(char *dst, sac *s, int hdr, size_t n) {
    char tmp[64] = {0};
    timespec64 t = {0,0};
    if(sac_get_time(s, hdr, &t)) {
        strftime64t(tmp, sizeof(tmp), "%FT%T.%3f", &t);
        return sacio_strlcat(dst, tmp, n);
    }
    return strlen(dst);
}

/**
 * @brief      Copy a floating point value to a string
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    Copy a floating point value to a string using '%g'
 *
 * @param      dst   Output character string
 * @param      s     sac file to get header value from
 * @param      hdr   Header ID
 * @param      n     Length of \p dst
 *
 * @return     full length of \p dst
 */
size_t
sac_floatlcat(char *dst, sac *s, int hdr, size_t n) {
    double v = 0.0;
    char tmp[64] = {0};
    sac_get_float(s, hdr, &v);
    if(v != SAC_FLOAT_UNDEFINED) {
        if(snprintf(tmp, sizeof(dst), "%g", v) < 0) {
            printf("warning: inusfficient space for number format\n");
        }
    }
    return sacio_strlcat(dst, tmp, n);
}


/**
 * @brief      create a new sac header
 *
 * @private
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    create and initialize a new sac header
 *
 * @return     newly created sac header
 */
sac_hdr *
sac_hdr_new() {
    sac_hdr *sh;
    sh = (sac_hdr *) malloc(sizeof(sac_hdr));
    if (sh) {
        sac_hdr_init(sh);
    }
    return sh;
}

/**
 * @brief      format sac values into a string
 *
 * @ingroup    sac
 * @memberof   sac
 *
 * @details    format sac values into a string
 *             - %%  Output the '%' character
 *             - %%E Event Name (kevnm)
 *             - %%I Instrument (kinst)
 *             - %%N Network (knetwk)
 *             - %%S Station (kstnm)
 *             - %%C Channel (kcmpnm)
 *             - %%H Location or Hole (khole)
 *             - %%L Location or Hole (khole), if value is "" or undefined, use "--".
 *             - %%t Relative time formatting (seconds from reference) `12.64` .
 *               All relative timing uses lowercase characters
 *               - %%tb begin time
 *               - %%te end time
 *               - %%to origin time
 *               - %%ta arrival time
 *               - %%t0 .. %%t9 time picks 0 to 9
 *             - %%T Absolute time formatting, e.g. `1994/06/09T00:33:16.123` .
 *               All absolute timing uses uppercase characters
 *               - %%tB begin time
 *               - %%tE end time
 *               - %%tO origin time
 *               - %%tA arrival time
 *               - %%t0 .. %%t9 time picks 0 to 9
 *             - %%Z alias for `%%N.%%S.%%H.%%C`  net.sta.loc.cha
 *             - %%R alias for `%%N %%S %%L %%C %%TB %%TE` similar to a data request
 *
 * @param      dst  Output character string
 * @param      n    Length of \p dst
 * @param      fmt  Formatting string
 * @param      s    sac file to get values from
 *
 * @return     full length of \p dst, -1 on error
 */
int 
sac_fmt(char *dst, size_t n, const char *fmt, sac *s) {
    int i = 0;
    int j = 0;
    char c = 0;
    if(!dst || !fmt || n == 0) {
        return -1;
    }
    memset(dst, 0, n);
    while((c = *fmt) != 0) {
        if(i < 0) {
            return -1;
        }
        // Reglar Character
        if((c = *fmt++) != '%') {
            dst[i++] = c;
            continue;
        }
        // Format Character
        switch(c = *fmt++) {
        case 'E': i = (int) sac_strlcat(dst, s->h->kevnm, n);  break;
        case 'I': i = (int) sac_strlcat(dst, s->h->kinst, n);  break;
        case 'N': i = (int) sac_strlcat(dst, s->h->knetwk, n); break;
        case 'S': i = (int) sac_strlcat(dst, s->h->kstnm, n);  break;
        case 'C': i = (int) sac_strlcat(dst, s->h->kcmpnm, n); break;
        case 'H': i = (int) sac_strlcat(dst, s->h->khole, n);  break;
        case 'L': {
            if((j = (int) sac_strlcat(dst, s->h->khole, n)) == i) {
                // No chracters copied, either empty "" or -12345
                j = (int) sac_strlcat(dst, "--", n);
            }
            i = j;
            break;
        }
        case 'c': {
            if(s->h->cmpinc == 0.0) {
                i = (int) sac_strlcat(dst, "VERT", n);
            } else if(s->h->cmpinc == 90.0) {
                float az = (float)fmod((double)s->h->cmpaz + 360.0, 360.0);
                if(az < 0.0) { az += 360.0; }

                if(fabs(az - 0.0) < 0.1) {
                    i = (int) sac_strlcat(dst, "NORTH", n);
                } else if(fabs(az - 90.0) < 0.1) {
                    i = (int) sac_strlcat(dst, "EAST", n);
                } else if(fabs(az - 180.0) < 0.1) {
                    i = (int) sac_strlcat(dst, "SOUTH", n);
                } else if(fabs(az - 270.0) < 0.1) {
                    i = (int) sac_strlcat(dst, "WEST", n);
                } else if(sac_hdr_defined(s, SAC_CMPAZ, SAC_CMPINC, NULL)) {
                    snprintf(dst, n, "%s %4d %4d", dst, (int)round(s->h->cmpaz), (int)round(s->h->cmpinc));
                    i = (int) strlen(dst);
                }
            } else if(sac_hdr_defined(s, SAC_CMPAZ, SAC_CMPINC, NULL)) {
                snprintf(dst, n, "%s %4d %4d", dst, (int)round(s->h->cmpaz), (int)round(s->h->cmpinc));
                i = (int) strlen(dst);
            }
        } break;
        case 't': {
            if(*fmt == 0) {
                printf("Unexpected end of format, expected time specifier\n");
                return -1;
            }
            switch(c = *fmt++) {
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                i = (int) sac_floatlcat(dst, s, SAC_T0 + (c - '0'), n);
                break;
            case 'b': i = (int) sac_floatlcat(dst, s, SAC_B, n); break;
            case 'e': i = (int) sac_floatlcat(dst, s, SAC_E, n); break;
            case 'o': i = (int) sac_floatlcat(dst, s, SAC_O, n); break;
            case 'a': i = (int) sac_floatlcat(dst, s, SAC_A, n); break;
            default:
                printf("Unknown conversion character: %c\n", c);
                return -1;
            }
        }
            break;
        case '{': {
            char key[16] = {0};
            char *p = NULL;
            if(!(p = strchr(fmt, '}'))) {
                printf("Expected '}', not found\n");
                return -1;
            }
            memcpy(key, fmt, p-fmt);
            key[p-fmt] = 0;
            fmt = p;
            fmt++;
        }
            break;
        case 'Z':
            i = sac_fmt(dst+i, n - (size_t) i, "%N.%S.%H.%C", s);
            break;
        case 'R':
            i = sac_fmt(dst+i, n - (size_t) i, "%N %S %L %C %TB %TE", s);
            break;
        case 'T':
            if(*fmt == 0) {
                printf("Unexpected end of format, exptected time specifier\n");
                return -1;
            }
            switch(c = *fmt++) {
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                i = (int) sac_timelcat(dst, s, SAC_T0 + (c - '0'), n);
                break;
            case 'B': i = (int) sac_timelcat(dst, s, SAC_B, n); break;
            case 'E': i = (int) sac_timelcat(dst, s, SAC_E, n); break;
            case 'O': i = (int) sac_timelcat(dst, s, SAC_O, n); break;
            case 'A': i = (int) sac_timelcat(dst, s, SAC_A, n); break;
            default:
                printf("Unknown conversion character: %c\n", c);
                return -1;
            }
            break;
        default:
            printf("Unknown conversion character: %c\n", c);
            return -1;
        }

    }
    return i;
}



#ifdef __TESTING__

int
main() {
    int nerr = 0;
    char dst[128] = {0};
    sac *s = sac_new();
    timespec64 t = {0,0};
    timespec64 t2 = {0,0};
    s->h->npts = 10;
    s->h->delta = 1.0;
    sac_set_float(s, SAC_B, 0.0);
    s->h->b = 0.0;
    sac_alloc(s);
    for(int i = 0; i < 10; i++) {
        s->y[i] = i;
    }
    timespec64_parse("1976/01/27T03:12:00", &t);
    printf("%lld %lld\n", t.tv_sec, t.tv_nsec);
    sac_set_time(s, t);
    sac_be(s);
    sac_get_time(s, SAC_B, &t2);
    printf("%lld %lld\n", t2.tv_sec, t2.tv_nsec);
    sac_write(s, "bsd.sac", &nerr);
    printf("err: %d\n", nerr);
    sac_fmt(dst, sizeof(dst), "%TO", s);
    printf("%s\n", dst);
    sac_fmt(dst, sizeof(dst), "%TB", s);
    printf("%s\n", dst);
    sac_fmt(dst, sizeof(dst), "%TE", s);
    printf("%s\n", dst);

    s = sac_read("XE.DOOR..BHZ.M.1994.160.003321.sac", &nerr);
    printf("file: %s\n", s->m->filename);
    sac_fmt(dst, sizeof(dst), "%TO", s);
    printf("%s\n", dst);
    sac_fmt(dst, sizeof(dst), "%TB", s);
    printf("%s\n", dst);
    sac_fmt(dst, sizeof(dst), "%TE", s);
    printf("%s\n", dst);


    return 0;
}

#endif

