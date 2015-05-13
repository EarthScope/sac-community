/** 
 * @file   qwidth.c
 * 
 * @brief  Report WIDTH parameters
 * 
 */

#include "eam.h"
#include "gem.h"
#include "exm.h"
#include "msg.h"

/** 
 * Report current values of the WIDTH parameters
 * 
 * @date   920528:  Original version - copied from qcolor and modified.
 *
 */
void
qwidth() {

    mprint("   WIDTH option is %s", ON_OFF(cmgem.lwidth));
    if (cmgem.lwidth) {
        mprint("   WIDTH INCREMENT option is %s", ON_OFF(cmgem.liwidth));
        mprint("   Current WIDTH value is " REPORT_INT,  cmgem.iwidth);
        mprint("   Current SKELETON WIDTH value is " REPORT_INT, cmgem.iskwidth);
        if (cmgem.liwidth)
            repivl("WIDTH increment LIST:", cmgem.iiwidth, cmgem.niwidth);
    }
    return;
}
