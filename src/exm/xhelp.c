/** 
 * @file   xhelp.c
 * 
 * @brief  HELP Command
 * 
 */

#include <string.h>

#include "exm.h"
#include "bool.h"

#include "clf.h"
#include "bot.h"
#include "ucf.h"
#include "cpf.h"
#include "co.h"
#include "debug.h"

/** 
 * Execute the HELP command printing the online help package
 * 
 * @param lprint 
 *    - TRUE print the output on a printer
 *    - FALSE do not print
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   820823:  Factored from original larger subroutine.
 *
 */
void
xhelp(int lprint, int *nerr) {

    int lintro;
    static char kintro[9] = "HLPINTRO";

    int i;
    char *file;
    string_list *list;

    *nerr = 0;
    lintro = TRUE;

    /* - Loop on each token in command: */

    if (lcmore(nerr)) {
        if ((list = lcdfl())) {
            lintro = FALSE;

            for (i = 0; i < string_list_length(list); i++) {
                file = strdup(string_list_get(list, i));
                modcase(FALSE, file, strlen(file), file);
                wrhelp(file, strlen(file) + 1, 1, lprint, nerr);
                FREE(file);
                if (*nerr != 0) {
                    if (*nerr < 0)
                        *nerr = 0;
                    goto L_8888;
                }
            }                   /* end for */
        } else {
            cfmt("ILLEGAL OPTION:", 17);
            cresp();
        }
    }

    if (*nerr != 0)
        goto L_8888;

    /* - If there were no tokens in command, print the
     *   introductory help package. */

    if (lintro)
        wrhelp(kintro, 9, 1, lprint, nerr);

  L_8888:
    return;
}
