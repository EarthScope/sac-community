/** 
 * @file   initializevar.c
 * 
 * @brief  Initialize the VARS Access library
 * 
 */

#include <stdio.h>

#include "amf.h"
#include "bool.h"
#include "vars.h"
#include "msg.h"

/** 
 * Initialize the VARS access library
 * 
 * @date   920409:  Moved lvarsinit to initcommon block data for initialization.
 * @date   890227:  Original version.
 * @date   890227:  Documented/Reviewed
 *
 */
void 
initializevars()
{

	/* - Return immediately if vars has already been initialized. */
	if( cmvars.lvarsinit )
		goto L_8888;

	/* - Initialize vars common block. */
	inivars();

	/* - Initialize message handling subsystem. */
	inimsg();

	/* - Initialize sacmem array for vars list storage. */
	iniam(&cmmem);

	/* - Set initialization flag. */
	cmvars.lvarsinit = TRUE;

L_8888:
	return;
}

