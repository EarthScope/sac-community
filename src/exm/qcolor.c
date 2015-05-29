/** 
 * @file   qcolor.c
 * 
 * @brief  Report Color parameters
 * 
 */

#include <string.h>

#include "bot.h"
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
    char tmp[32];
    
    mprint("   COLOR option is %s", ON_OFF(cmgem.lcol));
    upcase(cmgem.icol.name, strlen(cmgem.icol.name), tmp, 32);
    mprint("   DATA color is %s", tmp);
    mprint("   INCREMENT data color is %s", ON_OFF(cmgem.licol));
    upcase(cmgem.iskcol.name, strlen(cmgem.iskcol.name), tmp, 32);
    mprint("   SKELETON color is %s", tmp);
    upcase(cmgem.ibacol.name, strlen(cmgem.ibacol.name), tmp, 32);
    mprint("   BACKGROUND color is %s", tmp);

    return;
}
