/** 
 * @file   xsystemcomma.c
 * 
 * @brief  SYSTEMCOMMAND command
 * 
 */

#include <string.h>

#include "exm.h"
#include "co.h"
#include "cpf.h"
#include "co.h"
#include "bbs.h"
#include "debug.h"

void string_to_bb(char *name, char *value);

/** 
 * Execute the systemcommand which allows the user to execute a system command
 * 
 * @param nerr 
 *    Error Return Flag
 *
 * @date   870211:  Changed due to modification of LCREST.
 * @date   820817:  Changed to newest set of parsing and checking functions.
 * @date   820115:  Cleaned up and documented.
 *
 */
void
xsystemcommand(int *nerr) {

    char kmsg[MCMSG + 1];
    int ncmsg;

    char bbvar[128];
    int bbvar_s = 0;
    char *bbval;

    *nerr = 0;

    /* - Store rest of command line in output message buffer. */
    if(lkchar("&TO$", 5, sizeof(bbvar), bbvar, 0, &bbvar_s )) {
    }

    if (!lcrest(MCMSG, kmsg, MCMSG + 1, &ncmsg)) {
        cerr(1001);
        goto L_8888;
    }

    if(bbvar_s > 0) {
        bbval = zsysop_gets(kmsg, MCMSG + 1, &ncmsg, nerr);
        if(bbval && *nerr == 0) {
            string_to_bb(bbvar, bbval);
        }
        FREE(bbval);
    } else {
        /* - Execute the system command. */
        zsysop(kmsg, MCMSG + 1, &ncmsg, nerr);
    }

  L_8888:
    return;

}


static void
chomp(char *s) {
    size_t n = strlen(s);
    if(s[n-1] == '\n') {
        s[n-1] = 0;
    }
}

void
string_to_bb(char *name, char *value) {
    float f;
    int i;
    int n;
    chomp(value);

    if (sscanf(value, "%d%n", &i, &n) == 1 && n == (int) strlen(value)) {
        setbb(name, VAR_INTEGER, i);
    } else if(sscanf(value, "%f%n", &f, &n) == 1 && n == (int) strlen(value)) {
        setbb(name, VAR_VALUE, f);
    } else {
        setbb(name, VAR_STRING, value);
    }
}
