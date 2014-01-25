/** 
 * @file   zgetgd.c
 * 
 * @brief  Get the Default SAC Graphics Device
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
zgetgd(char *name,
       int   name_len) {

    char *temp;
    int i;

    if ((temp = getenv("SACGRAPHICSDEVICE")) != NULL)
      strcpy(name,temp);
    else {
      strcpy(name,"xwindows");
    }

    for(i=strlen(name);i<name_len;i++)
      name[i]=' ';      

    return;
}

            
