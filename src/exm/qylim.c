/** 
 * @file   qylim.c
 * 
 * @brief  Report Ylim parameters
 * 
 */

#include <stdio.h>
#include <string.h>

#include "eam.h"
#include "gam.h"
#include "msg.h"
#include "bool.h"

/** 
 * Report current values of YLIM parameters
 * 
 * @date   830121:  Original version.
 *
 */
void
qylim() {
    char kline[MCMSG + 1];
    int lall;
    int j, j_;

    lall = FALSE;
    memset(kline, 0, sizeof(kline));
    mprint("   %s", "YLIM option(s) are:");
    for (j = 1; j <= cmgam.nylim; j++) {
        j_ = j - 1;
        if (strcmp(kmgam.kylims[j_], "ON      ") == 0) {
            mprint("     %.4s%12.5g%12.5g", kmgam.kylims[j_]
                    , cmgam.ylims[j_][0], cmgam.ylims[j_][1]);
        } else if (strcmp(kmgam.kylims[j_], "ALL     ") == 0) {
            mprint("     %.4s", kmgam.kylims[j_]);
            lall = TRUE;
        } else {
            mprint("     %.4s", kmgam.kylims[j_]);
        }
    }

    if (lall) {
        mprint("   %s%12.5g%12.5g", "Range of dependent variable is:",
                cmgam.rngmin, cmgam.rngmax);
    }
    return;
}
