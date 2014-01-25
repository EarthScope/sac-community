#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#include "gdm.h"
#include "bot.h"
#include "cpf.h"

#define	MDEV	4

#include "mach.h"
#include "bool.h"
#include "co.h"

#include "gcm.h"

void /*FUNCTION*/ xbegindevices(nerr)
int *nerr;
{
	char kchar[9], kdev[MDEV][9];
	int nchar, ndev, i;

    memset(kchar,0,sizeof(kchar));
	/*=====================================================================
	 * PURPOSE:  To execute the action command BEGINDEVICES.
	 *           This command begins plotting to one or more graphics devices.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers: 1001.
	 *=====================================================================
	 * MODULE/LEVEL: OFM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  LCMORE, LCCHAR, CFMT, CRESP, BEGINDEVICES
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    MDEV:    Maximum number of graphic devices. [i]
	 *    KDEV:    Scratch array to store device names. [k]
	 *    NDEV:    Current length of KDEV. [i]
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    850404:  Move name checking logic to BEGINDEVICES.
	 *    850307:  Fixed bug if no graphics device was typed.
	 *    820825:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  850307
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* PARSING PHASE: */

	ndev = 0;

        for( i=0; i< MDEV; i++){
	    strcpy(&kdev[i][0],"        ");
	}

	/* - Loop on each token in command: */

	while ( lcmore( nerr ) ){

	    /* -- "device":  save name of new device in scratch array after
	     *               converting name to upper case. */
	    if( lcchar( MCPW, kchar,9, &nchar ) ){
		ndev = min( MDEV, ndev + 1 );
		modcase( TRUE, kchar, MCPW, (char*)kdev[ndev - 1] );
	    }

	    /* -- Bad syntax. */
	    else{
		cfmt( "ILLEGAL OPTION:",17 );
		cresp();
	    }
	}

	/* - The above loop is over when one of two conditions has been met:
	 *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
	 *   (2) All the tokens in the command have been successfully parsed. */

	if( *nerr != 0 )
	    goto L_8888;

	/* EXECUTION PHASE: */

	/* - Call device handler with list of graphic devices.
	 *   Raise syntax error condition if ANY error occurs
	 *   or if no graphics device name was typed in command. */

	if( ndev > 0 ){
	    begindevices( (char*)kdev,9, ndev, nerr );
	    calvspace();
	}
	else{
	    *nerr = 1001;
	    cfmt( "NO DEVICE NAME:",17 );
	}

L_8888:
	return;

} /* end of function */

