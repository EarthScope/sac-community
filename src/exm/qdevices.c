/** 
 * @file   qdevices.c
 * 
 * @brief  Report available graphics devices
 * 
 */

#include <stdio.h>
#include <string.h>
#include "string_utils.h"

#include "eam.h"
#include "msg.h"
#include "gdm.h"

/** 
 * Report the currently available graphics devices
 * 
 * @date   870514:  Original version.
 *
 */
void
qdevices() {
    char kline[MCMSG + 1], name[13];
    int j, number;

    memset(kline, 0, sizeof(kline));
    mprint("   %s", "Available graphics devices are:");

    getmaxdevices(&number);
    for (j = 1; j <= number; j++) {
        getdevicename(j, name, 13);
        if (strcmp(name, "            ") != 0 && strcasecmp(name, "RECORD") != 0
            && strcasecmp(name, "TEXT") != 0) {
            mprint("%s", name);
        }
    }

    return;
}
