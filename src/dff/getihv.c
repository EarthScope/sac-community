/** 
 * @file   getihv.c
 * 
 * @brief  Get an enumerated header value
 * 
 */

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "amf.h"
#include "dff.h"
#include "bot.h"
#include "msg.h"
#include "hdr.h"
#include "co.h"
#include "lhf.h"
#include "bool.h"
#include "SacHeader.h"
#include "errors.h"
#include <fstr.h>

LHF_EXTERN

#define UNDEFINED "UNDEFINED"
#define ILLEGAL   "ILLEGAL"

/** 
 * Get an enumerated header value from the current SAC file
 * 
 * @param kname 
 *    Name of the header field to get
 * @param kvalue 
 *    Value of heade field from the current SAC data file
 *    Each value represents a specific condition
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_UNDEFINED_HEADER_FIELD_VALUE
 *    - 1337
 * @param kname_s 
 *    Length of \p kname
 * @param kvalue_s 
 *    Length of \p kvalye
 *
 * @date   870902:  Original version.
 *
 */
void
getihv_internal(char *kname, char *kvalue, int *nerr, int kname_s, int kvalue_s,
                int null_terminate) {

    char ktest[9];
    int index, ivalue;
    char *kname_c;
    sac *s;
    char *p;
    int n;

    if(kvalue_s <= 0) {
        *nerr = ERROR_ILLEGAL_HEADER_FIELD_NAME;
        return;
    }
    kname_c = fstrdup(kname, kname_s);
    *nerr = 0;
    s = sacget_current();
    /* - Convert input name to uppercase and 
     *   check versus list of legal names. */
    sacio_char_to_keyword(kname_c, ktest);
    index = nequal(ktest, (char *) kmlhf.kihdr, 9, SAC_HEADER_ENUMS);

    /* - If legal name, return current value.
     *   Otherwise, set error condition. */
    memset(kvalue, ' ', kvalue_s);
    if (index > 0) {
        ivalue = IHDR(s)[index - 1];
        if (ivalue == SAC_ENUM_UNDEFINED) {
            n = strlen(UNDEFINED);
            memcpy(kvalue, UNDEFINED, min(kvalue_s, n));
            *nerr = ERROR_UNDEFINED_HEADER_FIELD_VALUE;
        } else {
            p = kmlhf.kiv[ivalue-1];
            n = strlen(p);
            memcpy(kvalue, p, min(kvalue_s, n));
        }
    } else {
        n = strlen(ILLEGAL);
        memcpy(kvalue, ILLEGAL, min(kvalue_s, n));
        //fstrncpy(kvalue, kvalue_s - 1, "ILLEGAL", 7);
        *nerr = ERROR_ILLEGAL_HEADER_FIELD_NAME;
    }
    if(null_terminate) {
        n = min(kvalue_s-1, n);
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
getihv(char *kname, char *kvalue, int *nerr, int kname_s, int kvalue_s) {
    getihv_internal(kname, kvalue, nerr, kname_s, kvalue_s, 1);
}

/* Wrapper to make the function more convenient for FORTRAN programmers. */

void
getihv_(char *kname, char *kvalue, int *nerr, int kname_s, int kvalue_s) {
    getihv_internal(kname, kvalue, nerr, kname_s, kvalue_s, 0);
}

void
getihv__(char *kname, char *kvalue, int *nerr, int kname_s, int kvalue_s) {
    getihv_internal(kname, kvalue, nerr, kname_s, kvalue_s, 0);
}
