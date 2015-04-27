/** 
 * @file   zgetgd.c
 * 
 * @brief  Get the Default SAC Graphics Device
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "co.h"

/** 
 * Get the Default SAC Graphics device 
 * 
 * @param name 
 *    Default graphics device
 * @param name_len 
 *    Length of \p name
 *
 * @date 11/22/88    Original version based upon zbasename.
 *
 */
void
zgetgd(char *name, int name_len) {

    char *temp;
    int i;

    if ((temp = getenv("SACGRAPHICSDEVICE")) != NULL)
        strcpy(name, temp);
    else {
#ifdef X11_APP
        strcpy(name, "xwindows");
#endif
#ifdef OSX_APP
        strcpy(name, "MAC");
#endif
#ifdef WIN32
        strcpy(name, "WIN");
#endif
    }

    for (i = strlen(name); i < name_len; i++)
        name[i] = ' ';

    return;
}
