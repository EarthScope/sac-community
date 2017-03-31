/** 
 * @file   getkhv.c
 * 
 * @brief  Get a characer header value
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "amf.h"
#include "dff.h"
#include "msg.h"
#include "bot.h"
#include "hdr.h"
#include "co.h"
#include "lhf.h"
#include "bool.h"
#include "SacHeader.h"
#include "errors.h"

LHF_EXTERN

#define SPACE_PAD ' '
#define NULL_PAD  '\0'

int
hdr_len(int index) {
    if(index == 2) {
        return 2 * SAC_HEADER_STRING_LENGTH_FILE;
    }
    return SAC_HEADER_STRING_LENGTH_FILE;
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

/** 
 * Get a character header value from the current SAC file
 * 
 * @param kname 
 *    Header variable to get
 * @param kvalue 
 *    Characeter header variable
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_UNDEFINED_HEADER_FIELD_VALUE
 *    - ERROR_ILLEGAL_HEADER_FIELD_NAME 
 * @param kname_s 
 *    Length of \p kname
 * @param kvalue_s 
 *    Length of \p kvalue
 *
 * @date   870902:  Original version.
 *
 */
void
getkhv_internal(char *kname, char *kvalue, int *nerr, int kname_s, int kvalue_s,
                int null_terminate) {

    char* kname_c;
    char ktest[9];
    int index;
    char *p;
    sac *s;

    if(kvalue_s <= 0) {
        *nerr = ERROR_ILLEGAL_HEADER_FIELD_NAME;
        return;
    }

    kname_c = fstrdup((char *)kname, kname_s);
    s = sacget_current();

    *nerr = 0;
    /* - Convert input name to uppercase and 
     *   check versus list of legal names. */
    sacio_char_to_keyword(kname_c, ktest);
    index = nequal(ktest, (char *) kmlhf.kkhdr, 9, SAC_HEADER_STRINGS);

    /* - If legal name, return current value.
     *   Otherwise, set error condition. */
    memset(kvalue, ' ', kvalue_s);
    if (index > 0) {
        p = khdr(s, index);
        memcpy(kvalue, p, min(kvalue_s, hdr_len(index)));
        if (is_kundefn(p, index)) {
            *nerr = ERROR_UNDEFINED_HEADER_FIELD_VALUE;
        }
    } else {
        *nerr = ERROR_ILLEGAL_HEADER_FIELD_NAME;
        memcpy(kvalue, SAC_CHAR_UNDEFINED, min(kvalue_s, (int)strlen(SAC_CHAR_UNDEFINED)));
        index = 1;
    }
    if(null_terminate) {
        int n = min(kvalue_s-1, hdr_len(index));
        kvalue[n] = 0;
    }

    /* - Create error message and write to terminal. */

    if (*nerr != 0) {
        sacio_message(*nerr, kname_c);
    }
    free(kname_c);
    return;
}

void
getkhv(char *kname, char *kvalue, int *nerr, int kname_s, int kvalue_s) {
    getkhv_internal(kname, kvalue, nerr, kname_s, kvalue_s, 1);
}


/* Wrapper to make fuction more convenient for FORTRAN programmers. */

void
getkhv_(char *kname, char *kvalue, int *nerr, int kname_s, int kvalue_s) {
    getkhv_internal(kname, kvalue, nerr, kname_s, kvalue_s, 0);
}

void
getkhv__(char *kname, char *kvalue, int *nerr, int kname_s, int kvalue_s) {
    getkhv_internal(kname, kvalue, nerr, kname_s, kvalue_s, 0);
}
