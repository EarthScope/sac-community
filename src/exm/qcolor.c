/** 
 * @file   qcolor.c
 * 
 * @brief  Report Color parameters
 * 
 */

#include "eam.h"
#include "gem.h"
#include "gdm.h"
#include "exm.h"
#include "msg.h"

/** 
 * Report the current values of the color parameters
 * 
 * @date   820316:  Original version.
 *
 */
void
qcolor() {

    mprint("   COLOR option is %s", ON_OFF(cmgem.lcol));
    mprint("   DATA color is ", cmgem.icol.name);
    mprint("   INCREMENT data color is %s", ON_OFF(cmgem.licol));
    mprint("   SKELETON color is %s", cmgem.iskcol.name);
    mprint("   BACKGROUND color is %s", cmgem.ibacol.name);

    return;
}
