
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <config.h>


#include "msg.h"
#include "mat.h"

#ifndef HAVE_MATLAB

void
matlab_unavailable() {
  fprintf(stderr, 
	  "Matlab routines in the default version of SAC are not available\n"
	  "To access the Matlab routines from within SAC, please recompile\n"
	  "SAC with the Matlab access turned on or ask your system administrator\n"
	  "The Readme.buildsac file explains this process in detail\n"
	  );
}

#endif



void /*FUNCTION*/ xmatc(index, nerr)
int index, *nerr;
{



        /*=====================================================================
         * PURPOSE: To execute a MODULE command given its index number.
         *=====================================================================
         * INPUT ARGUMENTS:
         *    INDEX:   The index number of the command.
         *=====================================================================
         * OUTPUT ARGUMENTS:
         *    NERR:    Error flag. Set to 0 if no error occurred.
         *             Potential error numbers: 0901.
         *=====================================================================
         * MODULE/LEVEL: mat/1
         *=====================================================================
         * GLOBAL INPUT:
         *    MACH:
         *=====================================================================
         * SUBROUTINES CALLED:
         *    SACLIB:  xMAT-3C, xMAT
         *=====================================================================
         * MODIFICATION HISTORY:
         *    970901:  Original version.
	 *    000515:  Modified to include DepMec and additional headers
         *===================================================================== */
        /* PROCEDURE: */
        *nerr = 0;

        /* - Jump to correct command based upon its index number. */

        switch( index ){
                case 1: goto L_100;	/* Run 3-component Matlab GUI */
                
                case 2: goto L_200;	/* execute matlab command on workspace contents.*/
                
		case 3: goto L_300;	/* set directory for .m files. */

	        case 4: goto L_400;     /* close matlab engine */

	        case 5: goto L_500;     /* Do record section plot */

	        case 6: goto L_600;     /* Run DepMec GUI */
                }

        /* - Error return if bad index value. */

        *nerr = 901;
        setmsg( "ERROR", *nerr );
        apcmsg( "in XMATC",9 );
        goto L_8888;

/* - Command 01: MAT3C and 3C command */
L_100:
        xMAT3C( nerr );	
        goto L_8888;

/* - Command 02: MAT command */
L_200:
        xMAT( nerr );
        goto L_8888;

/* - Command 03: SETMAT command */
L_300:
	xsetmat( nerr ) ;
	goto L_8888;

/* - Command 05: CLOSEMAT command */
L_400:
	xCLOSEMAT( nerr ) ;
	goto L_8888;
/* - Command 05: MATPRS command */
L_500:
        *nerr = 500;
	xMAT( nerr ) ;
	goto L_8888;

/* - Command 06: MATDEPMEC or DEPMEC command */
L_600:
        *nerr = 600;
	xMAT( nerr ) ;
	goto L_8888;

L_8888:
        return;

} /* end of function */

