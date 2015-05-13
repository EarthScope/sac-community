/** 
 * @file   qxlabl.c
 * 
 * @brief  Report XLABEL parameters
 * 
 */

#include "eam.h"
#include "gem.h"
#include "exm.h"
#include "msg.h"
/** 
 * Report current values of the xlabel command parameters
 * 
 * @date   820316:  Original version.
 *
 */
void
qxlabl() {
    mprint("   XLABEL option is %s", ON_OFF(cmgem.xlabel.on));
    repkv("Text of xlabel$", 16, kmgem.kxlab, 145);
    mprint("   SIZE of xlabel is " REPORT_FLOAT, cmgem.xlabel.text_size);
    repav("LOCATION of xlabel$", 20,
          (char *) kmgem.ksides[cmgem.xlabel.pos - 1], 9);
    return;
}
