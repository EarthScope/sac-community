/** 
 * @file   ldolist.c
 * 
 * @brief  Get the next item in a "DO" list
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "mach.h"
#include "cnd.h"
#include "bool.h"
#include "vars.h"
#include "co.h"
#include "clf.h"
#include "cpf.h"
#include "bot.h"

extern Token *do_token[100];
/** 
 * Pick off the next do list variable off the string
 * 
 * @param nerr 
 *   Error Return Flag
 *   - 0 on Success
 * 
 * @return 
 *   - TRUE if a next token was available
 *   - FALSE if no tokens were available
 *
 * @note Global Output
 *   - ndolevel: Incremented by one.
 *   - lifresp:  Set to value of if test.
 *
 * @date   871110:  Increased size of character list.
 * @date   870817:  Original version.
 */
int
ldolist(int *nerr) {

    int ldolist_v;
    int nchars;
    char *strtemp;

    char *filelist;
    int start, end;

    char *key;
    *nerr = 0;
    Token *t;

    if (!(t = do_token[cnd.ndolevel - 1])) {
        return FALSE;
    }

    key = upcase_dup((char *) kcnd.kdoname[cnd.ndolevel - 1]);

    if (token_is_string(t) || token_is_quoted_string(t)) {
        sac_vars_put_var((char *) kcnd.kdovar[cnd.ndolevel - 1], key,
                         VAR_STRING, t->str);
    } else if (token_is_int_precision(t, TOKEN_INT_PRECISION_NON_ARGUMENT)) {
        sac_vars_put_var((char *) kcnd.kdovar[cnd.ndolevel - 1], key,
                         VAR_INTEGER, (int) t->value);

    } else if (token_is_number(t)) {
        sac_vars_put_var((char *) kcnd.kdovar[cnd.ndolevel - 1], key, VAR_VALUE,
                         t->value);
    } else {
        return FALSE;
    }
    do_token[cnd.ndolevel - 1] = do_token[cnd.ndolevel - 1]->next;

    return TRUE;

    filelist =
        getvvstringZ((char *) kcnd.kdovar[cnd.ndolevel - 1], MCPFN + 1,
                     (char *) kcnd.kdolist[cnd.ndolevel - 1], MCPFN + 1,
                     &nchars, nerr);

    if (lnxtcl
        (filelist, strlen(filelist), &Idoin1[cnd.ndolevel],
         &Idoin2[cnd.ndolevel])) {
        start = Idoin1[cnd.ndolevel];
        end = Idoin2[cnd.ndolevel];
        end = min(end, nchars);
        nchars = end - start + 1;
        strtemp = (char *) malloc(sizeof(char) * (nchars + 1));
        strncpy(strtemp, filelist + start - 1, nchars);
        strtemp[nchars] = '\0';

        /* Copy the token to the variable name */
        putvvstring((char *) kcnd.kdovar[cnd.ndolevel - 1], MCPFN + 1,
                    (char *) kcnd.kdoname[cnd.ndolevel - 1], MCPFN + 1, nchars,
                    strtemp, end - start + 2, nerr);
        free(strtemp);
        ldolist_v = TRUE;
    } else {
        ldolist_v = FALSE;
    }
    free(filelist);
    return (ldolist_v);
}
