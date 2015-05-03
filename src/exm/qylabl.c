/** 
 * @file   qylabl.c
 * 
 * @brief  Report YLABEL parameters
 * 
 */

#include <string.h>
#include "eam.h"
#include "gem.h"
#include "exm.h"
#include "msg.h"
#include "bot.h"

/** 
 * Report current values of the ylable command parameters
 * 
 * @date   820316:  Original version.
 *
 */
void
qylabl() {
    char text[145];
    memcpy(text, kmgem.kylab, sizeof(text));
    rstrip(text);
    mprint("   YLABEL option is %s\n"
           "    Text of ylabel is '%s'\n"
           "    SIZE of ylabel is %12.5g\n"
           "    LOCATION of ylabel is %s", 
           (cmgem.ylabel.on) ? "ON" : "OFF",
           text,
           cmgem.ylabel.text_size,
           (char *) kmgem.ksides[cmgem.ylabel.pos - 1]);
    return;
}
