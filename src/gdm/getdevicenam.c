
#include <string.h>

#include "co.h"
#include "gdm.h"
#include "debug.h"

/** 
 * Inquire about the name of a graphics device
 *
 * @param number 
 *    Number of the graphics device
 * @param name
 *    Name of the graphics device on output
 *    Set to all blanks if \p number is a bad device value
 * @param name_s 
 *    Length of \p name, Maximum length is 12 characters
 *
 */
void 
getdevicename(int number,
              char *name,
              int name_s)
{
  
        display_t **dev;
        dev = gdm_get_devices();
        UNUSED(name_s);
        strncpy(name, dev[number-1]->name, strlen(dev[number-1]->name));
        name[strlen(dev[number-1]->name)] = 0;
        /*
	if( number > 0 && number <= MGD ){
		fstrncpy( name, name_s-1, kmgdm.kgdnam[number - 1],
                                   strlen(kmgdm.kgdnam[number - 1]));
		}
	else{
		fstrncpy( name, name_s-1, " ", 1 );
		}
        */

	return;

}

