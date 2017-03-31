/** 
 * @file   setkhv.c
 * 
 * @brief  Set a alphanumeric header value
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "amf.h"
#include "dff.h"
#include "bot.h"
#include "msg.h"
#include "co.h"
#include "hdr.h"
#include "lhf.h"
#include "bool.h"
#include "SacHeader.h"

#include "errors.h"


int hdr_len(int index);

LHF_EXTERN

/** 
 * Set an alphanumeric header value in the current SAC file
 * 
 * @param kname 
 *    Header field to set
 * @param kvalue 
 *    Alphanumeric value to set
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_ILLEGAL_HEADER_FIELD_NAME
 * @param kname_s 
 *    Length of \p kname
 * @param kvalue_s 
 *    Length of \p kvalue
 *
 *    870902:  Original version.
 *
 */
void
setkhv(char *kname, char *kvalue, int *nerr, int kname_s, int kvalue_s) {

    char ktest[9];
    int index;
    char *p;
    char *kname_c;
    char *kvalue_c;
    sac *s;

    s = sacget_current();
    kname_c = fstrdup(kname, kname_s);
    kvalue_c = fstrdup(kvalue, kvalue_s);

    kname_s = strlen(kname_c) + 1;
    kvalue_s = strlen(kvalue_c) + 1;

    *nerr = 0;

    /* - Convert input name to uppercase and check versus list of legal names. */
    sacio_char_to_keyword(kname_c, ktest);
    index = nequal(ktest, (char *) kmlhf.kkhdr, 9, SAC_HEADER_STRINGS);

    /* - Store value in appropriate header field. */

    if (index > 0) {
        p = khdr(s, index);
        fstrncpy(p, hdr_len(index), kvalue_c, strlen(kvalue_c));
    } else {
        *nerr = ERROR_ILLEGAL_HEADER_FIELD_NAME;
    }

    /* - Create error message and write to terminal. */
    if (*nerr != 0) {
        sacio_message(*nerr, kname_c);
    }

    free(kname_c);
    free(kvalue_c);

    return;
}

/* Wrapper to make the function for convenient for FORTRAN programmers. */

void
setkhv_(char *kname, char *kvalue, int *nerr, int kname_s, int kvalue_s) {
    setkhv(kname, kvalue, nerr, kname_s, kvalue_s);
}

void
setkhv__(char *kname, char *kvalue, int *nerr, int kname_s, int kvalue_s) {
    setkhv(kname, kvalue, nerr, kname_s, kvalue_s);
}
