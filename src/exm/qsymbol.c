/** 
 * @file   qsymbol.c
 * 
 * @brief  Report SYMBOL parameters
 * 
 */

#include "eam.h"
#include "gem.h"

#include "exm.h"

/** 
 * Report the current values of the SYMBOL parameters
 * 
 * @date   870728:  Original version.
 *
 */
void
qsymbol() {

    mprint("   SYMBOL option is %s", ON_OFF(cmgem.lsym));
    if (cmgem.lsym) {
        mprint("   Current SYMBOL is " REPORT_INT, cmgem.isym);
        mprint("   Symbol SIZE is " REPORT_FLOAT, cmgem.symsz);
        mprint("   Symbol SPACEING is " REPORT_FLOAT, cmgem.symsp);
        mprint("   Symbol INCREMENT option is %s", ON_OFF(cmgem.lisym));
        if (cmgem.lisym)
            repivl("Symbol increment LIST:", cmgem.iisym, cmgem.nisym);
    }

    return;
}
