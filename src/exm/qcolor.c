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

/** 
 * Report the current values of the color parameters
 * 
 * @date   820316:  Original version.
 *
 */
void
qcolor() {

    char kcol[9];

    mprint("   COLOR option is %s", ON_OFF(cmgem.lcol));
    convcolornum(cmgem.icol, kcol, 9);
    repav("DATA color$", 12, kcol, 9);
    mprint("   INCREMENT data color is %s", ON_OFF(cmgem.licol));
    convcolornum(cmgem.iskcol, kcol, 9);
    repav("SKELETON color$", 16, kcol, 9);
    convcolornum(cmgem.ibacol, kcol, 9);
    repav("BACKGROUND color$", 18, kcol, 9);

    return;
}
