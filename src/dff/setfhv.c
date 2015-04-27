/** 
 * @file   setfhv.c
 * 
 * @brief  Set a floating point header value
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "amf.h"
#include "dff.h"
#include "hdr.h"
#include "msg.h"
#include "co.h"
#include "bot.h"
#include "lhf.h"
#include "bool.h"
#include "SacHeader.h"
#include "errors.h"

/** 
 * Set a floating point header value in the current SAC file
 * 
 * @param kname 
 *    Name of the header field
 * @param fvalue 
 *    New Value 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_ILLEGAL_HEADER_FIELD_NAME
 * @param kname_s 
 *    Length of \p kname
 *
 * @date   870902:  Original version.
 *
 */
void
setfhv(char *kname, float *fvalue, int *nerr, int kname_s) {

    char ktest[9];
    int index;
    float *fp;
    char *kname_c;
    sac *s;

    kname_c = fstrdup(kname, kname_s);
    kname_s = strlen(kname_c) + 1;

    *nerr = 0;
    s = sacget_current();
    /* - Convert input name to uppercase and 
     *   check versus list of legal names. */
    sacio_char_to_keyword(kname_c, ktest);
    index = nequal(ktest, (char *) kmlhf.kfhdr, 9, SAC_HEADER_FLOATS);

    /* - Store value in appropriate header field. */
    if (index > 0) {
        fp = fhdr(s, index);
        VALUE(fp) = VALUE(fvalue);
    } else {
        *nerr = ERROR_ILLEGAL_HEADER_FIELD_NAME;
    }

    /* - Create error message and write to terminal. */

    if (*nerr != 0) {
        sacio_message(*nerr, kname_c);
    }

    free(kname_c);
    kname_c = NULL;

    return;
}

/* Wrapper to make the function more convenient for FORTRAN programmers. */

void
setfhv_(char *kname, float *fvalue, int *nerr, int kname_s) {
    setfhv(kname, fvalue, nerr, kname_s);
}

void
setfhv__(char *kname, float *fvalue, int *nerr, int kname_s) {
    setfhv(kname, fvalue, nerr, kname_s);
}
