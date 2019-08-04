/** 
 * @file   rsach.c
 * 
 * @brief  Read Sac Header
 * 
 */

#include <string.h>

#include "amf.h"
#include "proto.h"
#include "dff.h"
#include "hdr.h"
#include "bool.h"
#include "co.h"
#include "msg.h"
#include "ucf.h"
#include "errors.h"

#include <fstr.h>

/** 
 * Initialize the Common block for sacio
 * 
 */
void
sacio_initialize_common() {
    static int init = FALSE;
    if (init == FALSE) {
        init = TRUE;
        inihdr();
        inilhf();
        inimsg();
    }
}

/** 
 * Check the Sac Header Version
 * 
 * @param hdr 
 *   Sac Header to Check
 * @param nerr 
 *   Error return Flag
 *   - SAC_OK
 *   - SAC_ERROR_NOT_A_SAC_FILE
 * 
 * @return 
 *    - TRUE if file needs to be swapped
 *    - FALSE if file does not need to be swapped
 *    - -1 if there was an error checking the header
 *
 * @date July 01, 2007 Initial Version -- B. Savage
 */
/*
int
sac_check_header_version(float *hdr, int *nerr) {
    int lswap;
    int *ver;

    lswap = FALSE;
    *nerr = SAC_OK;
    / * determine if the data needs to be swapped. * /
    ver = (int *) (hdr + SAC_VERSION_LOCATION);
    if (*ver < 1 || *ver > SAC_HEADER_MAJOR_VERSION) {
        byteswap((void *) ver, SAC_HEADER_SIZEOF_NUMBER);

        if (*ver < 1 || *ver > SAC_HEADER_MAJOR_VERSION) {
            error(*nerr = ERROR_NOT_A_SAC_FILE,
                  "not in sac format, nor byteswapped sac format.");
            outmsg();
            clrmsg();
            return -1;
        } else {
            / * swap back, so it can be * /
            byteswap((void *) ver, SAC_HEADER_SIZEOF_NUMBER);
            lswap = TRUE;
        }
    }
    return lswap;
}*/

/** 
 * Swap the Sac Header
 * 
 * @param hdr 
 *    Sac Header to Swap, packed
 *
 * @bug Unexpected and compiler packing of the structure
 *    will cause problems swapping the header, each value
 *    should be swapped individually, it is the safest way
 *
 * @date July 01, 2007 Initial Version -- B. Savage
 */
/*
void
sac_header_swap(float *hdr) {
    int i;
    float *ptr;
    for (i = 0, ptr = hdr; i < SAC_HEADER_NUMBERS; i++, ptr++) {
        byteswap((void *) ptr, SAC_HEADER_SIZEOF_NUMBER);
    }
    }*/

/** 
 * Read a Sac Header
 * 
 * @param nun 
 *    Logical file unit to read Sac Header From
 * @param nerr 
 *    Error return Flag
 *    - SAC_OK
 *    - Non-Zero on Error
 * 
 * @bug Sac Header Character decoding
 *      This is dodgy at best
 *      24 = Character Strings
 *      9  = Character String Length in Memory (includes terminator)
 *      8  = Character String Length in File (no terminator)
 *      4  = Size of Float
 *      6*4 = 24 Terminators, UGH!
 *      Sac Header in the File
 *             8 * 24 = 192 bytes  (Actual String)
 *         4 * 2 * 24 = 192 bytes  (Size in Floats)
 *      Sac Header in Memory
 *             9 * 24 = 216 bytes  (Actual String)
 *   4 * (2 * 24 + 6) = 216 bytes  (Size in Floats)
 *
 * @return 
 *    - TRUE if file needs to be swapped
 *    - FALSE if file does not need to be swapped
 *
 * @bug This routine should be reading in the character strings as 
 *      characters and null terminating them correctly and not 
 *      shifting the data through a float using co/zrabs().
 *
 * @date July 01, 2007 Initial Version -- B. Savage
 */
/*
int
sac_header_read(int nun, sac * s, int *nerr) {
    int lswap;
    float temp2[2 * SAC_HEADER_STRINGS];
    float temp[(2 * SAC_HEADER_STRINGS) + 6];
    int word;

    lswap = FALSE;
    word = 0;
    zrabs(&nun, (char *) s->h, SAC_HEADER_NUMBERS, &word, nerr);
    if (*nerr != SAC_OK)
        return lswap;

    lswap = sac_check_header_version((float *) s->h, nerr);

    if (lswap) {                / * byteswap all the non-character header elements. * /
        sac_header_swap((float *) s->h);
    }

    word = word + SAC_HEADER_NUMBERS;
    zrabs(&nun, (char *) temp2, 2 * SAC_HEADER_STRINGS, &word, nerr);
    if (*nerr != SAC_OK)
        return lswap;

    map_chdr_in(temp, temp2);

    zgetc((int *) temp, (char *) s->h->kstnm,
          SAC_HEADER_STRING_LENGTH * SAC_HEADER_STRINGS);

    sac_check_time_precision(s->h);
    return lswap;

}
*/ 
/** 
 * Read a Sac Header
 * 
 * @param kname 
 *    File to read Sac Header from
 * @param nerr 
 *    Error Return Flag
 *    - SAC_OK
 *    - Non-Zero on Error
 * @param kname_s 
 *    Length of filename \p kname
 *
 * @date July 01, 2007 Initial Version -- B. Savage
 */

/* void */
/* rsach(char *kname, int *nerr, int kname_s) { */
/*     int ncerr; */
/*     int nun; */
/*     sac *s; */
/*     nun = 0; */

/*     *nerr = SAC_OK; */

/*     sacio_initialize_common(); */

/*     /\* - Open the file. *\/ */
/*     zopen_sac((int *) &nun, kname, kname_s, "RODATA", 7, (int *) nerr); */
/*     if (*nerr != SAC_OK) */
/*         goto ERROR; */

/*     s = sac_new(); */
/*     s->m->filename = fstrdup(kname, kname_s); */
/*     sacput(s); */

/*     sac_header_read(nun, s, nerr); */
/*     if (*nerr != SAC_OK) */
/*         goto ERROR; */

/*   ERROR: */
/*     zclose((int *) &nun, (int *) &ncerr); */
/*     return; */
/* } */

/** 
 * Determine the byte order of the machine
 * 
 * @return 
 *    - ENDIAN_BIG 
 *    - ENDIAN_LITTLE
 */
int
CheckByteOrder() {
    static int byte_order = ENDIAN_UNKNOWN;
    short int word = 0x0001;
    char *byte = (char *) &word;
    if (byte_order == ENDIAN_UNKNOWN) {
        byte_order = (!byte[0]) ? ENDIAN_BIG : ENDIAN_LITTLE;
    }
    return byte_order;
}

void sac_write_internal(sac *s, char *filename, int write_data, int swap, int *nerr);

void
sac_write_r(sac * s, char *filename, int write_data, int lswap, int *nerr) {
    sac_write_internal(s, filename, write_data, lswap, nerr);
}
int
is_kundef(char *kvalue) {
    size_t n = strlen(kvalue);
    return n == strlen(SAC_CHAR_UNDEFINED) &&
        (memcmp(kvalue, SAC_CHAR_UNDEFINED, strlen(SAC_CHAR_UNDEFINED)) == 0);
}
int
is_kundef2(char *kvalue) {
    size_t n = strlen(kvalue);
    return n == strlen(SAC_CHAR_UNDEFINED_2 ) &&
        (memcmp(kvalue, SAC_CHAR_UNDEFINED_2, strlen(SAC_CHAR_UNDEFINED_2)) == 0);
}

int
is_kundefn(char *kvalue, int item) {
    if(item == 2) {
        return is_kundef2(kvalue);
    }
    return is_kundef(kvalue);
}
int
sac_byte_order(int getset) {
    int i, n;
    int byte_order;
    char *env_string;
    char *env_big[] = { "big", "solaris", "powerpc", "ppc" };
    char *env_little[] = { "little", "x86", "intel" };
    static int swap = -1;       /* Default to System Byte Order */

    if (getset < 0 && swap == -1) {     /* Initial Call, Get */
        env_string = getenv("SAC_WRITE_BYTE_ORDER");
        getset = CheckByteOrder();
        if (env_string != NULL) {
            n = strlen(env_string);
            for (i = 0; i < (int) (sizeof(env_big) / sizeof(char *)); i++) {
                if (strncasecmp
                    (env_string, env_big[i],
                     min(n, (int) strlen(env_big[i]))) == 0) {
                    getset = ENDIAN_BIG;
                }
            }
            for (i = 0; i < (int) (sizeof(env_little) / sizeof(char *)); i++) {
                if (strncasecmp
                    (env_string, env_little[i],
                     min(n, (int) strlen(env_little[i]))) == 0) {
                    getset = ENDIAN_LITTLE;
                }
            }
        }
    }
    if (getset >= 0) {          /* Set, Initial or later call */
        byte_order = CheckByteOrder();
        swap = !(byte_order == getset);
    }
    return swap;
}
