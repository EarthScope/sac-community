/** 
 * @file   putcl.c
 * 
 * @brief  Put an entry into a character list
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "clf.h"
#include "bot.h"
#include "bool.h"
#include "msg.h"
#include "co.h"
#include "dff.h"

#include "errors.h"
#include "string_utils.h"
#include "debug.h"

/** 
 * Put an entry into a character list
 * 
 * @param kcl 
 *    Character list
 * @param kcl_s 
 *    Length of \p kcl
 * @param kentry 
 *    Entry to put into \p kcl
 *    Trailing blanks are trimmed before putting into list.
 * @param kentry_s 
 *    Length of \p kentry
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_CHARACER_LIST_EXCEED_BOUNDS
 *    - ERROR_DELIMITER_FOUND_IN_ENTRY
 *
 * @date   870722:  Added descriptive error messages.
 * @date   860128:  Original version.
 *
 */
void
putcl(char *kcl, int kcl_s, char *kentry, int kentry_s, int *nerr) {

    char kdel;
    int ibeg, idel, iend_, ncl, nentry;
    char *strtemp;

    *nerr = 0;

    /* - Determine length of character list and  delimiter. */
    ncl = (kcl_s - 1);
    kdel = kcl[0];

    /* - Determine length of entry without trailing blanks. */
    nentry = indexb(kentry, kentry_s);

    strtemp = malloc(nentry + 1);
    strncpy(strtemp, kentry, nentry);
    strtemp[nentry] = '\0';

    /* - Make sure delimiter is not present in entry. */
    idel = indexa(strtemp, nentry + 1, kdel, TRUE, TRUE);

    if (idel > 0) {
        *nerr = ERROR_DELIMITER_FOUND_IN_ENTRY;
        setmsg("ERROR", *nerr);
        apcmsg2(kentry, nentry);
        goto L_8888;
    }

    /* - Determine end of last entry in character list.
     *   (This is the first nonoccurance of delimiter searching backwards.) */
    iend_ = indexa(kcl, kcl_s, kdel, FALSE, FALSE);

    /* - Make sure there is room in character list for entry and delimiter. */
    ibeg = iend_ + 2;
    if ((ibeg + nentry + 1) > ncl) {
        *nerr = ERROR_CHARACER_LIST_EXCEED_BOUNDS;
        setmsg("ERROR", *nerr);
        apcmsg2(kentry, nentry);
        goto L_8888;
    }

    /* - Copy entry to character list. */
    subscpy(kcl, ibeg - 1, ibeg + nentry - 2, kcl_s - 1, strtemp);

  L_8888:
    free(strtemp);
    return;
}

