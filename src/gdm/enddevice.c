
#include <string.h>

#include "gdm.h"
#include "bool.h"
#include "msg.h"
#include "bot.h"

#include "debug.h"

/** 
 * End plotting to a graphics device 
 *
 * @param device
 *   Name of device 
 * @param device_s
 *   Length of \p device
 * @param nerr
 *   Error return flag
 *   - 0 on Success
 *   - Non-Zero on Error 
 *
 * @date   880520:  Fixed bug in call to endgraphics.
 * @date   861010:  Major restructuring.
 * @date   831027:  Original version.
 *
 */
void 
enddevice(char *device,
          int device_s,
          int *nerr)
{
        display_t *dev;

	*nerr = 0;

        DEBUG("device: '%s'\n", device);

	/* - End current frame if necessary. */
	if( cmgdm.lbegf ){
		endframe( FALSE , nerr );
		if( *nerr != 0 )
			goto L_8888;
	}

	/* - Handle request to end all graphics devices here. Terminate library. */

	if( memcmp(device,"ALL",3) == 0 ){
		endgraphics( nerr );
		if( *nerr != 0 )
			goto L_8888;
	}

	/* - Check name versus list of graphics devices. */
        //	else if( lequal( device,device_s, (char*)kmgdm.kgdnam,13, MGD, &igd ) ){
        else if((dev = gdm_get_device_by_name(device))) {
          if(dev->on) {
            if(dev->end_device) {
              dev->end_device( nerr );
            }
            dev->on = FALSE;
          } else{
            *nerr = 901;
            setmsg( "ERROR", *nerr );
            apcmsg( "in ENDDEVICE",13 );
            goto L_8888;
          }
	}

	/*   Raise illegal device error if still no match. */
	else{
		*nerr = 201;
		setmsg( "ERROR", *nerr );
		apcmsg( device,device_s );
		goto L_8888;
	}

	/* - Calculate new values for graphics device status variables. */

	calstatus();

L_8888:
	return;

} /* end of function */

