
#include "co.h"
#include "gdm.h"

/** 
 * Get the name and type of graphics device #1
 *
 * @param name 
 *   Name of graphics device
 * @param name_s
 *   Length of \p name
 * @param type 
 *   Type of the graphics device
 *
 * @date   981112:  Device 1 was originally the terminal (eg. vt100).  
 *             Today, this obsolete device is removed from SAC.
 *             The plumbing is left in place in case a new device is
 *             added later, it can become device1.  For now, 
 *             pass back "        ", and 3.
 *             (Q: Why 3?  A: That's essentially what it was setting
 *             *idtype to before; the process was more complicated, but
 *             the result was always 3)   maf 
 * @date   862020:  Original version.
 *
 * REMOVE THIS FUNCTION
 *
 */
void 
getdeviceinfo1(char *kdname,
               int kdname_s,
               int *idtype)
{

	fstrncpy( kdname , kdname_s-1 , "        " , 8 );
	*idtype = 3 ;

}

