/** 
 * @file   qgtext.c
 * 
 * @brief  Report gtext parameters
 * 
 */

#include <stdio.h>
#include <string.h>

#include "eam.h"
#include "gem.h"
#include "msg.h"

#include "exm.h"

GEM_EXTERN

/** 
 * Report current values of the gtext value parameters
 * 
 * @date   870728:  Original version.
 * 
 */
void
qgtext() {
    char kline[MCPFN + 1];
    memset(kline, 0, sizeof(kline));
    if (kmgem.kgtqua[0] == 'H') {
        mprint("   %s", "HARDWARE text being used.");
    } else {
        mprint("   %s", "SOFTWARE text being used.");
    }
    mprint("   Text FONT is " REPORT_INT, cmgem.igtfnt);
    mprint("   Text SIZE is " REPORT_FLOAT, cmgem.tsdef);

    return;
}
