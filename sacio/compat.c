/**
 * @file
 * @brief compatability sac I/O and manipulation
 */
#include <stdio.h>
#include <string.h>

#include "sacio.h"

#include "strip.h"
#include "defs.h"

#define ERROR_SAC_FILE_NOT_EVENLY_SPACED     801 /**< @brief sac file not evenly spaced */
#define ERROR_SAC_FILE_NOT_UNEVENLY_SPACED   802 /**< @brief sac file not unevenly spaced */
#define ERROR_SAC_DATA_TRUNCATED_ON_READ     803 /**< @brief sac file truncated during read */
#define ERROR_ILLEGAL_DATA_FILE_LIST_NUMBER 1301 /**< @brief no sac file was defined for operation */
#define ERROR_ILLEGAL_HEADER_FIELD_NAME     1337 /**< @brief unknown header keyword */
#define ERROR_UNDEFINED_HEADER_FIELD_VALUE  1336 /**< @brief header value was undefined */
#define ERROR_ILLEGAL_ENUMERATED_VALUE      1365 /**< @brief illegal enumerated value */

sac *current = NULL;


/**
 * @defgroup sac-iris sac-iris
 * @brief Compatability routines for fortran style sacio
 */


/**
 * Older sac I/O routines, written orignally for Fortran 77
 * @private
 */
struct sac_iris {};

/**
 * @brief      Copy a character string
 *
 * @details    Copy a string from src to dst assuming the src is from fortran
 *
 * @private
 * @ingroup    sac-iris
 * @memberof   sac_iris
 *
 * @param      dst    Output character string
 * @param      ndst   Length of dst
 * @param      src    Input character string
 * @param      nsrc   Length of src, if <= 0, assume a C string with terminator
 *                    otherwise assume a fortran string with length
 *
 */
static void
fstrcpy(char *dst, int ndst, char *src, int nsrc) {
    size_t n = 0;
    if(nsrc < 0) { // Likely C Path
        sacio_strlcpy(dst, src, ndst);
    } else { // Likely Fortran Path
        n = (nsrc < ndst) ? nsrc : ndst - 1;
        memset(dst, 0, sizeof(ndst));
        memcpy(dst, src, n);
    }
    sacio_rstrip(dst);
}

/**
 * @brief      Copy a character string right padded with spaces
 *
 * @details    Copy a character string right padded assuming the dst is
 *             a fortran string
 *
 * @private
 * @ingroup    sac-iris
 * @memberof   sac_iris
 *
 * @param      dst    Output character string, padded with spaces
 * @param      ndst   Length of dst
 * @param      src    Input character string
 * @param      nsrc   Length of src
 *
 */
static void
fstrput(char *dst, int ndst, char *src, int nsrc, int null_terminate) {
    size_t n = 0;
    if(ndst <= 0 || nsrc <= 0) {
        return;
    }
    memset(dst, ' ', ndst);
    n = (nsrc < ndst) ? nsrc : ndst;
    memcpy(dst, src, n);
    if(null_terminate) {
        n = (nsrc < ndst-1) ? nsrc : ndst-1;
        dst[n] = 0;
    }
}

/**
 * @brief      Convert a keyword to a Header Type and Location
 *
 * @details    Determine the Hedaer Type and location from a keyword
 *
 * @private
 * @ingroup    sac-iris
 * @memberof   sac_iris
 *
 * @param      kname    Keyword to find header location and type of
 * @param      nerr     Status code, 0 in success, non-zero on failure
 * @param      kname_s  Length of keyword
 *
 * @return     struct hid with name, type, and header id
 */
struct hid *
get_hid(char *kname, int *nerr, int kname_s) {
    struct hid *h = NULL;
    char name[32] = {0};
    *nerr = 0;
    fstrcpy(name, sizeof(name), kname, kname_s);
    if(!(h = sac_keyword_to_header(name, strlen(name)))) {
        *nerr = ERROR_ILLEGAL_HEADER_FIELD_NAME;
    }
    return h;
}

/**
 * @brief      Get the current old fortran style sac header
 *
 * @private
 * @ingroup sac_iris
 * @memberof sac-iris
 *
 * @details    Get the current old fortran style sac header
 *
 * @param      nerr  status code, 0 on success, non-zero on failure
 *
 * @return     sac   current sac header
 */
sac *
get_current(int *nerr) {
    if(current == NULL) {
        *nerr = ERROR_ILLEGAL_DATA_FILE_LIST_NUMBER;
    }
    return current;
}



/**
 * @brief      Read an evenly spaced sac file
 *
 * @details    Read an evenly spaced sac file
 *
 * @ingroup    sac-iris
 * @memberof   sac_iris
 *
 * @param      kname      Name of sac file to read
 * @param      yarray     Output dependent variable
 * @param      nlen       Output length of \p yarray. If the data length is
 *                        greater than the space specificed by \p max_,
 *                        nlen is set to \p max_, \p nerr is set to -803,
 *                        and only the first \p max_ points are returned
 * @param      beg        Output begin value
 * @param      del        Output sampling for the independent variable
 * @param      max_       Length of yarray
 * @param      nerr       Status code, 0 on success, non-zero on failure
 * @param      kname_s    Length of kname
 *
 */
void
rsac1(char      *kname,
      float      yarray[],
      int       *nlen,
      float     *beg,
      float     *del,
      int       *max_,
      int       *nerr,
      int        kname_s) {
    char name[4096] = {0};
    fstrcpy(name, sizeof(name), kname, kname_s);
    sac *s = sac_read(name, nerr);
    if(*nerr != 0) {
        return;
    }
    if(!s->h->leven) {
        *nerr = ERROR_SAC_FILE_NOT_EVENLY_SPACED;
        return;
    }
    *beg = s->h->b;
    *del = s->h->delta;

    if(s->h->npts <= *max_) {
        *nlen = s->h->npts;
    } else {
        *nlen = *max_;
        *nerr = -ERROR_SAC_DATA_TRUNCATED_ON_READ;
    }
    memcpy(yarray, s->y, *nlen * sizeof(float));
    current = s;
}

/**
 * @brief      Read an unevenly spaced sac file
 *
 * @details    Read an unevenly spaced sac file
 *
 * @ingroup    sac-iris
 * @memberof   sac_iris
 *
 * @param      kname      Name of sac file to read
 * @param      yarray     Output dependent variable
 * @param      nlen       Output length of \p yarray and \p xarray
 * @param      xarray     Output independent variable
 * @param      max_       Length of yarray
 * @param      nerr       Status code, 0 on success, non-zero on failure
 * @param      kname_s    Length of kname
 *
 */
void
rsac2(char  *kname,
      float *yarray,
      int   *nlen,
      float *xarray,
      int   *max_,
      int   *nerr,
      int   kname_s) {

    char name[4096] = {0};
    fstrcpy(name, sizeof(name), kname, kname_s);
    sac *s = sac_read(name, nerr);
    if(*nerr != 0) {
        return;
    }
    if(s->h->leven) {
        *nerr = ERROR_SAC_FILE_NOT_UNEVENLY_SPACED;
        return;
    }
    if(s->h->npts <= *max_) {
        *nlen = s->h->npts;
    } else {
        *nlen = *max_;
        *nerr = -ERROR_SAC_DATA_TRUNCATED_ON_READ;
    }
    memcpy(yarray, s->y, *nlen * sizeof(float));
    memcpy(xarray, s->x, *nlen * sizeof(float));
    s->h->npts = *nlen;
    sac_be(s);
    current = s;
}

/**
 * @brief      Create a new sac header to store in memory
 *
 * @details    Creates a new, empy sac header in memory
 *
 * @ingroup    sac-iris
 * @memberof   sac_iris
 *
 * @note       The header is held in memory and fields are accessible
 *             throught getfhv(), getihv(), getkhv(), getnhv(), getlhv()
 *             and settable from setfhv(), setihv(), setkhv(), setnhv(),
 *             setlhv(). This active header is used during writes in 
 *             wsac0(), wsac1(), wsac2(), and wsac3().
 */
void
newhdr() {
    sac *s = sac_new();
    s->m->filename = strdup("new_hdr_file.sac");
    if(current) {
        sac_free(current);
        current = NULL;
    }
    current = s;
}


/**
 * @brief      Read a sac file header
 *
 * @details    Read a sac file header
 *
 * @ingroup    sac-iris
 * @memberof   sac_iris
 *
 * @param      kname     Name of sac file header to read
 * @param      nerr      Status code, 0 on succes, non-zero on failure
 * @param      kname_s   Length of kname
 *
 */
void
rsach(char  *kname,
      int   *nerr,
      int    kname_s) {
    char name[4096] = {0};
    fstrcpy(name, sizeof(name), kname, kname_s);
    sac *s = sac_read_header(name, nerr);
    if(*nerr != 0) {
        return;
    }
    sac_be(s);
    current = s;
}

/**
 * @brief      Write a sac file
 *
 * @details    Write a sac file using the existing header in memory
 *
 * @ingroup    sac-iris
 * @memberof   sac_iris
 *
 * @param      kname     Name of sac file to write
 * @param      yarray    Input independent varaible, e.g. x, time, real, amp
 * @param      xarray    Input dependedent variable, e.g. amplitude, imag, phase
 * @param      nerr      Status code, 0 on success, non-zero on failue
 * @param      kname_s   Length of kname
 *
 */
void
wsac0(char   *kname,
      float  *xarray,
      float  *yarray,
      int    *nerr,
      int     kname_s) {

    char name[4096] = {0};
    float *x = NULL, *y = NULL;
    sac *s = NULL;
    if((s = get_current(nerr)) == NULL) {
        return;
    }
    fstrcpy(name, sizeof(name), kname, kname_s);
    x = s->x;
    y = s->y;
    s->x = xarray;
    s->y = yarray;
    sac_write(s, name, nerr);
    s->x = x;
    s->y = y;
}

void
wsac3(char   *kname,
      float  *xarray,
      float  *yarray,
      int    *nerr,
      int     kname_s) {
    wsac0(kname, xarray, yarray, nerr, kname_s);
}


/**
 * @brief      Write an evenly spaced sac file
 *
 * @details    Write an evenly spaced sac file using the header in memory
 *
 * @ingroup    sac-iris
 * @memberof   sac_iris
 *
 * @param      kname     Name of sac file to write
 * @param      yarray    Input dependent variable, e.g. amplitude
 * @param      nlen      Length of yarray
 * @param      beg       Begin value
 * @param      del       dependent variable sampling
 * @param      nerr      Status code, 0 on success, non-zero on failure
 * @param      kname_s   Length of kname
 *
 */
void
wsac1(char   *kname,
      float  *yarray,
      int    *nlen,
      float  *beg,
      float  *del,
      int    *nerr,
      int     kname_s) {
    char name[4096] = {0};
    float *y = NULL;
    sac *s = NULL;
    if((s = get_current(nerr)) == NULL) {
        return;
    }
    fstrcpy(name, sizeof(name), kname, kname_s);
    y = s->y;
    s->y = yarray;
    s->h->delta = *del;
    s->h->b     = *beg;
    s->h->npts  = *nlen;
    s->h->leven = TRUE;
    sac_be(s);
    sac_write(s, name, nerr);
    s->y = y;
}

/**
 * @brief      Write an unevenly spaced sac file
 *
 * @details    Write an unevenly spaced sac file using the header in memory
 *
 * @ingroup    sac-iris
 * @memberof   sac_iris
 *
 * @param      kname     Name of sac file to write
 * @param      yarray    Input dependent variable
 * @param      nlen      Length of \p yarray and \p xarray
 * @param      xarray    Input independent variable
 * @param      nerr      Status code, 0 on success, non-zero on failure
 * @param      kname_s   Length of kname
 *
 */
void
wsac2(char   *kname,
      float  *yarray,
      int    *nlen,
      float  *xarray,
      int    *nerr,
      int     kname_s) {
    char name[4096] = {0};
    float *y = NULL, *x = NULL;
    sac *s = NULL;
    if((s = get_current(nerr)) == NULL) {
        return;
    }
    fstrcpy(name, sizeof(name), kname, kname_s);
    y = s->y;
    x = s->x;
    s->y = yarray;
    s->x = xarray;
    s->h->npts  = *nlen;
    s->h->leven = FALSE;
    sac_be(s);
    sac_write(s, name, nerr);
    s->y = y;
    s->x = x;
}

/**
 * @brief      Get a floating point header value
 *
 * @details    Get a floating point header value from the sac header
 *
 * @ingroup    sac-iris
 * @memberof   sac_iris
 *
 * @param      kname    Name of header value
 * @param      fvalue   Output header value
 * @param      nerr     Status, 0 on success, non-zero on failure
 * @param      kname_s  Length of kname
 *
 * @note This routine requires an internal, global sac file from
 *       rsac1(), rsac2() or newhdr()
 */
void
getfhv(char *kname, float *fvalue, int *nerr, int kname_s) {
    sac *s = NULL;
    struct hid *h = NULL;

    *nerr = 0;
    if((s = get_current(nerr)) == NULL) {
        return;
    }
    if((h = get_hid(kname, nerr, kname_s)) == NULL) {
        return;
    }
    if(h->type != SAC_FLOAT_TYPE) {
        *nerr = ERROR_ILLEGAL_HEADER_FIELD_NAME;
        *fvalue = SAC_FLOAT_UNDEFINED;
        return;
    }
    sac_get_float(current, h->id, fvalue);
    if(*fvalue == SAC_FLOAT_UNDEFINED) {
        *nerr = ERROR_UNDEFINED_HEADER_FIELD_VALUE;
    }
}

/**
 * @brief Enumerated values to use in getihv() and setihv()
 *
 * @ingroup sac0iris
 * @memberof sac_iris
 *
 */
char *enum_values[] = {
    "ITIME   ",
    "IRLIM   ",
    "IAMPH   ",
    "IXY     ",
    "IUNKN   ",
    "IDISP   ",
    "IVEL    ",
    "IACC    ",
    "IB      ",
    "IDAY    ",
    "IO      ",
    "IA      ",
    "IT0     ",
    "IT1     ",
    "IT2     ",
    "IT3     ",
    "IT4     ",
    "IT5     ",
    "IT6     ",
    "IT7     ",
    "IT8     ",
    "IT9     ",
    "IRADNV  ",
    "ITANNV  ",
    "IRADEV  ",
    "ITANEV  ",
    "INORTH  ",
    "IEAST   ",
    "IHORZA  ",
    "IDOWN   ",
    "IUP     ",
    "ILLLBB  ",
    "IWWSN1  ",
    "IWWSN2  ",
    "IHGLP   ",
    "ISRO    ",
    "INUCL   ",
    "IPREN   ",
    "IPOSTN  ",
    "IQUAKE  ",
    "IPREQ   ",
    "IPOSTQ  ",
    "ICHEM   ",
    "IOTHER  ",
    "IGOOD   ",
    "IGLCH   ",
    "IDROP   ",
    "ILOWSN  ",
    "IRLDTA  ",
    "IVOLTS  ",
    "IXYZ    ",
    "IMB     ",
    "IMS     ",
    "IML     ",
    "IMW     ",
    "IMD     ",
    "IMX     ",
    "INEIC   ",
    "IPDEQ   ",
    "IPDEW   ",
    "IPDE    ",
    "IISC    ",
    "IREB    ",
    "IUSGS   ",
    "IBRK    ",
    "ICALTECH",
    "ILLNL   ",
    "IEVLOC  ",
    "IJSOP   ",
    "IUSER   ",
    "IUNKNOWN",
    "IQB     ",
    "IQB1    ",
    "IQB2    ",
    "IQBX    ",
    "IQMT    ",
    "IEQ     ",
    "IEQ1    ",
    "IEQ2    ",
    "IME     ",
    "IEX     ",
    "INU     ",
    "INC     ",
    "IO_     ",
    "IL      ",
    "IR      ",
    "IT      ",
    "IU      ",
    "IEQ3    ",
    "IEQ0    ",
    "IEX0    ",
    "IQC     ",
    "IQB0    ",
    "IGEY    ",
    "ILIT    ",
    "IMET    ",
    "IODOR   "};
/**
 * @brief      Get a enumerated header value
 *
 * @details    Get a enumerated header value from the sac header
 *
 * @ingroup    sac-iris
 * @memberof   sac_iris
 * @private
 *
 * @param      kname    Name of header value
 * @param      kvalue   Output enumerated value as a string
 * @param      nerr     Status, 0 on success, non-zero on failure
 * @param      kname_s  Length of kname
 * @param      kvalue_s Length of kvalue
 *
 * @note This routine requires an internal, global sac file from
 *       rsac1(), rsac2() or newhdr()
 */
void
getihv_internal(char *kname, char *kvalue, int *nerr, int kname_s, int kvalue_s, int null_terminate) {
    sac *s = NULL;
    int n = 0;
    struct hid *h = NULL;
    int ivalue = -1;

    *nerr = 0;
    if(kvalue_s <= 0) {
        *nerr = ERROR_ILLEGAL_HEADER_FIELD_NAME;
        return;
    }
    if((s = get_current(nerr)) == NULL) {
        return;
    }
    if((h = get_hid(kname, nerr, kname_s)) == NULL) {
        memset(kvalue, 0, kvalue_s);
        fstrput(kvalue, kvalue_s, "ILLEGAL", 7, null_terminate);
        return;
    }
    if(h->type != SAC_ENUM_TYPE) {
        *nerr = ERROR_ILLEGAL_HEADER_FIELD_NAME;
        fstrput(kvalue, kvalue_s, SAC_CHAR_UNDEFINED, 8, null_terminate);
    }
    sac_get_int(current, h->id, &ivalue);
    if(ivalue == SAC_INT_UNDEFINED) {
        *nerr = ERROR_UNDEFINED_HEADER_FIELD_VALUE;
        fstrput(kvalue, kvalue_s, "UNDEFINED", 9, null_terminate);
        return;
    }
    memset(kvalue, ' ', kvalue_s);
    n = strlen(enum_values[ivalue-1]);
    fstrput(kvalue, kvalue_s, enum_values[ivalue-1], n, null_terminate);
}
/**
 * @brief      Get a enumerated header value
 *
 * @details    Get a enumerated header value from the sac header
 *
 * @ingroup    sac-iris
 * @memberof   sac_iris
 *
 * @param      kname    Name of header value
 * @param      kvalue   Output enumerated value as a string
 * @param      nerr     Status, 0 on success, non-zero on failure
 * @param      kname_s  Length of kname
 * @param      kvalue_s Length of kvalue
 *
 * @note This routine requires an internal, global sac file from
 *       rsac1(), rsac2() or newhdr()
 */
void
getihv(char *kname, char *kvalue, int *nerr, int kname_s, int kvalue_s) {
    getihv_internal(kname, kvalue, nerr, kname_s, kvalue_s, 1);
}
void
getihv_(char *kname, char *kvalue, int *nerr, int kname_s, int kvalue_s) {
    getihv_internal(kname, kvalue, nerr, kname_s, kvalue_s, 0);
}
void
getihv__(char *kname, char *kvalue, int *nerr, int kname_s, int kvalue_s) {
    getihv_internal(kname, kvalue, nerr, kname_s, kvalue_s, 0);
}

/**
 * @brief      Get a character string header value
 *
 * @details    Get a character string header value from the sac header
 *
 * @ingroup    sac-iris
 * @memberof   sac_iris
 *
 * @param      kname    Name of header value
 * @param      kvalue   Output character string
 * @param      nerr     Status, 0 on success, non-zero on failure
 * @param      kname_s  Length of kname
 * @param      kvalue_s Length of kvalue
 *
 * @note This routine requires an internal, global sac file from
 *       rsac1(), rsac2() or newhdr()
 */
void
getkhv_internal(char *kname, char *kvalue, int *nerr, int kname_s, int kvalue_s, int null_terminate) {
    sac *s = NULL;
    struct hid *h = NULL;
    char v[32] = {0};

    if(kvalue_s <= 0) {
        *nerr = ERROR_ILLEGAL_HEADER_FIELD_NAME;
        return;
    }
    *nerr = 0;
    if((s = get_current(nerr)) == NULL) {
        return;
    }
    if((h = get_hid(kname, nerr, kname_s)) == NULL) {
        return;
    }
    if(h->type != SAC_STRING_TYPE && h->type != SAC_LONG_STRING_TYPE) {
        *nerr = ERROR_ILLEGAL_HEADER_FIELD_NAME;
        fstrput(kvalue, kvalue_s, SAC_CHAR_UNDEFINED, 8, null_terminate);
        return;
    }
    sac_get_string(current, h->id, v, sizeof(v));
    if((h->type == SAC_STRING_TYPE && strcmp(v, SAC_CHAR_UNDEFINED) == 0) ||
       (h->type == SAC_LONG_STRING_TYPE && strcmp(v, SAC_CHAR_UNDEFINED_2) == 0)) {
        *nerr = ERROR_UNDEFINED_HEADER_FIELD_VALUE;
        fstrput(kvalue, kvalue_s, SAC_CHAR_UNDEFINED, 8, null_terminate);
        return;
    }
    fstrput(kvalue, kvalue_s, v, strlen(v), null_terminate);
}
void
getkhv(char *kname, char *kvalue, int *nerr, int kname_s, int kvalue_s) {
    getkhv_internal(kname, kvalue, nerr, kname_s, kvalue_s, 1);
}
void
getkhv_(char *kname, char *kvalue, int *nerr, int kname_s, int kvalue_s) {
    getkhv_internal(kname, kvalue, nerr, kname_s, kvalue_s, 0);
}
void
getkhv__(char *kname, char *kvalue, int *nerr, int kname_s, int kvalue_s) {
    getkhv_internal(kname, kvalue, nerr, kname_s, kvalue_s, 0);
}

/**
 * @brief      Get a integer header value
 *
 * @details    Get a integer header value from the sac header
 *
 * @ingroup    sac-iris
 * @memberof   sac_iris
 *
 * @param      kname    Name of header value
 * @param      nvalue   Output header value
 * @param      nerr     Status, 0 on success, non-zero on failure
 * @param      kname_s  Length of kname
 *
 * @note This routine requires an internal, global sac file from
 *       rsac1(), rsac2() or newhdr()
 */
void
getnhv(char *kname, int *nvalue, int *nerr, int kname_s) {
    sac *s = NULL;
    struct hid *h = NULL;

    *nerr = 0;
    if((s = get_current(nerr)) == NULL) {
        return;
    }
    if((h = get_hid(kname, nerr, kname_s)) == NULL) {
        return;
    }

    if(h->type != SAC_INT_TYPE) {
        *nerr = ERROR_ILLEGAL_HEADER_FIELD_NAME;
        *nvalue = SAC_INT_UNDEFINED;
        return;
    }
    sac_get_int(current, h->id, nvalue);
    if(*nvalue == SAC_INT_UNDEFINED) {
        *nerr = ERROR_UNDEFINED_HEADER_FIELD_VALUE;
    }

}
/**
 * @brief      Get a logical/bool header value
 *
 * @details    Get a logical/bool header value from the sac header
 *
 * @ingroup    sac-iris
 * @memberof   sac_iris
 *
 * @param      kname    Name of header value
 * @param      nvalue   Output header value
 * @param      nerr     Status, 0 on success, non-zero on failure
 * @param      kname_s  Length of kname
 *
 * @note This routine requires an internal, global sac file from
 *       rsac1(), rsac2() or newhdr()
 */
void
getlhv(char *kname, int *nvalue, int *nerr, int kname_s) {
    sac *s = NULL;
    struct hid *h = NULL;

    *nerr = 0;
    if((s = get_current(nerr)) == NULL) {
        return;
    }
    if((h = get_hid(kname, nerr, kname_s)) == NULL) {
        return;
    }

    if(h->type != SAC_BOOL_TYPE) {
        *nerr = ERROR_ILLEGAL_HEADER_FIELD_NAME;
        *nvalue = SAC_INT_UNDEFINED;
        return;
    }
    sac_get_int(current, h->id, nvalue);
    if(*nvalue == SAC_INT_UNDEFINED) {
        *nerr = ERROR_UNDEFINED_HEADER_FIELD_VALUE;
    }
}

/**
 * @brief      Set a floating point header value
 *
 * @details    Set a floating point header value in the active sac header
 *
 * @ingroup    sac-iris
 * @memberof   sac_iris
 *
 * @param      kname      Name of header value to set
 * @param      fvalue     Input value to set
 * @param      nerr       Status code, 0 on success, non-zero on failure
 * @param      kname_s    Length of kname
 *
 * @note This routine requires an internal, global sac file from
 *       rsac1(), rsac2() or newhdr()
 */
void
setfhv(char *kname, float *fvalue, int *nerr, int kname_s) {
    sac *s = NULL;
    struct hid *h = NULL;

    *nerr = 0;
    if((s = get_current(nerr)) == NULL) {
        return;
    }
    if((h = get_hid(kname, nerr, kname_s)) == NULL) {
        return;
    }
    if(h->type != SAC_FLOAT_TYPE) {
        *nerr = ERROR_ILLEGAL_HEADER_FIELD_NAME;
        return;
    }
    sac_set_float(s, h->id, *fvalue);
}
/**
 * @brief      Set a integer header value
 *
 * @details    Set a integer header value in the active sac header
 *
 * @ingroup    sac-iris
 * @memberof   sac_iris
 *
 * @param      kname      Name of header value to set
 * @param      nvalue     Input value to set
 * @param      nerr       Status code, 0 on success, non-zero on failure
 * @param      kname_s    Length of kname
 *
 * @note This routine requires an internal, global sac file from
 *       rsac1(), rsac2() or newhdr()
 */
void
setnhv(char *kname, int *nvalue, int *nerr, int kname_s) {
    sac *s = NULL;
    struct hid *h = NULL;

    *nerr = 0;
    if((s = get_current(nerr)) == NULL) {
        return;
    }
    if((h = get_hid(kname, nerr, kname_s)) == NULL) {
        return;
    }
    if(h->type != SAC_INT_TYPE) {
        *nerr = ERROR_ILLEGAL_HEADER_FIELD_NAME;
        return;
    }
    sac_set_int(s, h->id, *nvalue);
}
/**
 * @brief      Set a logical/bool header value
 *
 * @details    Set a logical/bool header value in the active sac header
 *
 * @ingroup    sac-iris
 * @memberof   sac_iris
 *
 * @param      kname      Name of header value to set
 * @param      nvalue     Input value to set
 * @param      nerr       Status code, 0 on success, non-zero on failure
 * @param      kname_s    Length of kname
 *
 * @note This routine requires an internal, global sac file from
 *       rsac1(), rsac2() or newhdr()
 */
void
setlhv(char *kname, int *nvalue, int *nerr, int kname_s) {
    sac *s = NULL;
    struct hid *h = NULL;

    *nerr = 0;
    if((s = get_current(nerr)) == NULL) {
        return;
    }
    if((h = get_hid(kname, nerr, kname_s)) == NULL) {
        return;
    }
    if(h->type != SAC_BOOL_TYPE) {
        *nerr = ERROR_ILLEGAL_HEADER_FIELD_NAME;
        return;
    }
    sac_set_int(s, h->id, *nvalue);
}

/**
 * @brief      Set a character string header value
 *
 * @details    Set a character string header value in the active sac header
 *
 * @ingroup    sac-iris
 * @memberof   sac_iris
 *
 * @param      kname      Name of header value to set
 * @param      kvalue     Input value to set
 * @param      nerr       Status code, 0 on success, non-zero on failure
 * @param      kname_s    Length of kname
 * @param      kvalue_s   Length of kvalue
 *
 * @note This routine requires an internal, global sac file from
 *       rsac1(), rsac2() or newhdr()
 */
void
setkhv(char *kname, char *kvalue, int *nerr, int kname_s, int kvalue_s) {
    int n = 8;
    sac *s = NULL;
    struct hid *h = NULL;
    char v[32] = {0};

    *nerr = 0;
    if((s = get_current(nerr)) == NULL) {
        return;
    }
    if((h = get_hid(kname, nerr, kname_s)) == NULL) {
        return;
    }
    if(h->type != SAC_STRING_TYPE && h->type != SAC_LONG_STRING_TYPE) {
        *nerr = ERROR_ILLEGAL_HEADER_FIELD_NAME;
        return;
    }
    memset(v, ' ', sizeof(v));
    v[sizeof(v)-1] = 0;
    n = (h->type == SAC_STRING_TYPE) ? 8 : 16;
    if(kvalue_s <= 0) {
        kvalue_s = strlen(kvalue);
    }
    n = (kvalue_s < n) ? kvalue_s : n;
    memcpy(v, kvalue, n);
    sac_set_string(s, h->id, v);
}

/**
 * @brief      Set a enumerated header value
 *
 * @details    Set a enumerated header value in the active sac header
 *
 * @ingroup    sac-iris
 * @memberof   sac_iris
 *
 * @param      kname      Name of header value to set
 * @param      kvalue     Input value to set as a character string
 * @param      nerr       Status code, 0 on success, non-zero on failure
 * @param      kname_s    Length of kname
 * @param      kvalue_s   Length of kvalue
 *
 * @note This routine requires an internal, global sac file from
 *       rsac1(), rsac2() or newhdr()
 */
void
setihv(char *kname, char *kvalue, int *nerr, int kname_s, int kvalue_s) {
    size_t i = 0;
    size_t n = 0;
    sac *s = NULL;
    struct hid *h = NULL;
    char v[9] = {0};

    *nerr = 0;
    if((s = get_current(nerr)) == NULL) {
        return;
    }
    if((h = get_hid(kname, nerr, kname_s)) == NULL) {
        return;
    }
    if(h->type != SAC_ENUM_TYPE) {
        return;
    }
    memset(v, ' ', sizeof(v));
    v[8] = 0;
    if(kvalue_s <= 0) {
        kvalue_s = strlen(kvalue);
    }
    n = (kvalue_s < 8) ? kvalue_s : 8;
    memcpy(v, kvalue, n);
    for(i = 0; i < sizeof(enum_values)/sizeof(char *); i++) {
        if(strcasecmp(v, enum_values[i]) == 0) {
            sac_set_int(s, h->id, i+1);
            return;
        }
    }
    *nerr = ERROR_ILLEGAL_ENUMERATED_VALUE;
    return;
}

/**
 * @private
 * @see getfhv()
 */
void getfhv_(char *kname, float *fvalue, int *nerr, int kname_s) {
    getfhv(kname, fvalue, nerr, kname_s);
}
/**
 * @private
 * @see getfhv()
 */
void getfhv__(char *kname, float *fvalue, int *nerr, int kname_s) {
    getfhv_(kname, fvalue, nerr, kname_s);
}

void
wsac0_(char *kname, float *xarray, float *yarray, int *nerr, int kname_s) {
    wsac0(kname, xarray, yarray, nerr, kname_s);
}
void
wsac0__(char *kname, float *xarray, float *yarray, int *nerr, int kname_s) {
    wsac0(kname, xarray, yarray, nerr, kname_s);
}

void
getlhv_(char *kname, int *nvalue, int *nerr, int kname_s) {
    getlhv(kname, nvalue, nerr, kname_s);
}
void
getlhv__(char *kname, int *nvalue, int *nerr, int kname_s) {
    getlhv(kname, nvalue, nerr, kname_s);
}
void
getnhv_(char *kname, int *nvalue, int *nerr, int kname_s) {
    getnhv(kname, nvalue, nerr, kname_s);
}
void
getnhv__(char *kname, int *nvalue, int *nerr, int kname_s) {
    getnhv(kname, nvalue, nerr, kname_s);
}
void
setnhv_(char *kname, int *nvalue, int *nerr, int kname_s) {
    setnhv(kname, nvalue, nerr, kname_s);
}
void
setnhv__(char *kname, int *nvalue, int *nerr, int kname_s) {
    setnhv(kname, nvalue, nerr, kname_s);
}
void
setlhv_(char *kname, int *nvalue, int *nerr, int kname_s) {
    setlhv(kname, nvalue, nerr, kname_s);
}
void
setlhv__(char *kname, int *nvalue, int *nerr, int kname_s) {
    setlhv(kname, nvalue, nerr, kname_s);
}
void
setkhv_(char *kname, char *kvalue, int *nerr, int kname_s, int kvalue_s) {
    setkhv(kname, kvalue, nerr, kname_s, kvalue_s);
}
void
setkhv__(char *kname, char *kvalue, int *nerr, int kname_s, int kvalue_s) {
    setkhv(kname, kvalue, nerr, kname_s, kvalue_s);
}
void
setihv_(char *kname, char *kvalue, int *nerr, int kname_s, int kvalue_s) {
    setihv(kname, kvalue, nerr, kname_s, kvalue_s);
}
void
setihv__(char *kname, char *kvalue, int *nerr, int kname_s, int kvalue_s) {
    setihv(kname, kvalue, nerr, kname_s, kvalue_s);
}
void
setfhv_(char *kname, float *fvalue, int *nerr, int kname_s) {
    setfhv(kname, fvalue, nerr, kname_s);
}void
setfhv__(char *kname, float *fvalue, int *nerr, int kname_s) {
    setfhv(kname, fvalue, nerr, kname_s);
}
void
newhdr_() {
    newhdr();
}
void
newhdr__() {
    newhdr();
}

void
rsac1_(char *kname, float yarray[], int *nlen, float *beg, float *del, int *max_, int *nerr, int kname_s) {
    rsac1(kname, yarray, nlen, beg, del, max_, nerr, kname_s);
}
void
rsac1__(char *kname, float yarray[], int *nlen, float *beg, float *del, int *max_, int *nerr, int kname_s) {
    rsac1(kname, yarray, nlen, beg, del, max_, nerr, kname_s);
}

void
rsac2_(char *kname, float *yarray, int *nlen, float *xarray, int *max_, int *nerr, int kname_s) {
    rsac2(kname, yarray, nlen, xarray, max_, nerr, kname_s);
}
void
rsac2__(char *kname, float *yarray, int *nlen, float *xarray, int *max_, int *nerr, int kname_s) {
    rsac2(kname, yarray, nlen, xarray, max_, nerr, kname_s);
}
void
wsac1_(char *kname, float *yarray, int *nlen, float *beg, float *del, int *nerr,  int kname_s) {
    wsac1(kname, yarray, nlen, beg, del, nerr,  kname_s);
}
void
wsac1__(char *kname, float *yarray, int *nlen, float *beg, float *del, int *nerr,  int kname_s) {
    wsac1(kname, yarray, nlen, beg, del, nerr,  kname_s);
}
void
wsac2_(char *kname, float *yarray, int *nlen, float *xarray, int *nerr, int kname_s) {
    wsac2(kname, yarray, nlen, xarray, nerr, kname_s);
}
void
wsac2__(char *kname, float *yarray, int *nlen, float *xarray, int *nerr, int kname_s) {
    wsac2(kname, yarray, nlen, xarray, nerr, kname_s);
}
void
wsac3_(char *kname, float *xarray, float *yarray, int *nerr, int kname_s) {
    wsac3(kname, xarray, yarray, nerr, kname_s);
}
void
wsac3__(char *kname, float *xarray, float *yarray, int *nerr, int kname_s) {
    wsac3(kname, xarray, yarray, nerr, kname_s);
}


void sac_data_read_new() {}
void sac_header_read_new() {}
void sacio_message(int nerr, char *name) {
    UNUSED(nerr);
    UNUSED(name);
}
void sacio_message_control() {}
void sacio_char_to_keyword(char *in, char out[9]) {
    UNUSED(in);
    UNUSED(out);
}
int hdr_len(int index) {
    UNUSED(index);
    return 0;
}

