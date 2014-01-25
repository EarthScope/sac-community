
#include <string.h>

#include "gdm.h"
#include "bool.h"

/** 
 * Get graphics device status
 *
 * @param kstat
 *   Name of desired status variable
 *    - 'ANY' to see if any devices are currently on.
 *    - 'ACTIVE' to see if any active devices are currently on.
 *    - 'PASSIVE' to see if any passive devices are on.
 *    - 'CURSOR' to see if any active devices with cursor are on.
 * @param ltrue
 *    Status result
 *    - TRUE if attribute is on 
 *    - FALSE if attribute is off or unknown
 *
 * @date   861020:  Original version.
 *
 */
void 
getstatus(char *kstat,
          int *ltrue)
{
	if( memcmp(kstat,"AN",2) == 0 || memcmp(kstat,"an",2) == 0 ){
		*ltrue = cmgdm.lactiv || cmgdm.lpasiv;
        }
	else if( memcmp(kstat,"AC",2) == 0 || memcmp(kstat,"ac",2) == 0 ){
		*ltrue = cmgdm.lactiv;
        }
	else if( kstat[0] == 'P' || kstat[0] == 'p' ){
		*ltrue = cmgdm.lpasiv;
        }
	else if( kstat[0] == 'C' || kstat[0] == 'c' ){
		*ltrue = cmgdm.lcur;
        }
	else{
		*ltrue = FALSE;
        }

}

