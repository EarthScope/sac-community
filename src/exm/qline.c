/** 
 * @file   qline.c
 * 
 * @brief  Report line parameters
 * 
 */

#include "eam.h"
#include "gem.h"
#include "gam.h"
#include "exm.h"
#include "msg.h"

GEM_EXTERN

/** 
 * Report current values of the line parameters
 * 
 * @date   910301:  Changed iline to icline. iline was in gem and gam. (wct)
 * @date   870728:  Original version.
 *
 */
void
qline() {

    mprint("   LINE option is %s", ON_OFF(cmgem.lline));
    if (cmgem.lline) {
        mprint("   Current linestyle is " REPORT_INT, cmgem.icline);
        mprint("   Line INCREMENT option is %s", ON_OFF(cmgem.liline));
        if (cmgem.liline)
            repivl("Line increment LIST:", cmgem.iiline, cmgem.niline);
    }

    return;
}
