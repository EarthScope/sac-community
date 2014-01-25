
#include <stdio.h>
#include <string.h>

#include "smm.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"


#include "bot.h"
#include "ucf.h"
#include "msg.h"
#include "clf.h"
#include "cpf.h"
#include "dff.h"

void 
xmarktimes(int *nerr) {

	int ifpick, ikpick, ipick, j, j_, jdfl, 
	 ndxx, ndxy, nlen, nodttm, nvelu;
	float distu, originu;
  char *tmp;

	/*=====================================================================
	 * PURPOSE: To parse and execute the action command MARKTIMES.
	 *          This command marks files with travel times from a velocity set.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *=====================================================================
	 * MODULE/LEVEL:  SMM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    DFM:     NDFL
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    HDR:     T0, T1, ..., KT0, KT1, ...,
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  LCMORE, CFMT, CRESP, LKLOGR, LCKEY, LCLOGR, LKIA, LKRA
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *     861128:  Modified command structure.
	 *              Converted to an internal SAC command.
	 *     841211:  Modified to conform to new XSC structure.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  861128
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* PARSING PHASE: */

	/* - Loop on each token in command: */

L_1000:
	if( lcmore( nerr ) ){

		/* -- "DISTANCE HEADER|v":  set epicentral distance to use. */
		if( lckey( "DISTANCE$",10 ) ){
			if( lckey( "HEADER$",8 ) ){
				cmsmm.ldistr = FALSE;
				}
			else if( lcreal( &cmsmm.distr ) ){
				cmsmm.ldistr = TRUE;
				}
			else{
				cfmt( "ILLEGAL DISTANCE OPTION:",26 );
				cresp();
				}

			/* -- "ORIGIN HEADER|v|GMT i1 i2 i3 i4 i5 i6":  set origin time to use. */
			}
		else if( lckey( "ORIGIN$",8 ) ){
			if( lckey( "HEADER$",8 ) ){
				cmsmm.loriginr = FALSE;
				cmsmm.lgmt = FALSE;
				}
			else if( lcreal( &cmsmm.originr ) ){
				cmsmm.loriginr = TRUE;
				cmsmm.lgmt = FALSE;
				}
			else if( lkia( "GMT$",5, 6, 6, cmsmm.iodttm, &nodttm ) ){
				cmsmm.loriginr = TRUE;
				cmsmm.lgmt = TRUE;
				}
			else{
				cfmt( "ILLEGAL ORIGIN OPTION:",24 );
				cresp();
				}

			/* -- "VELOCTIES v1 v2 ...":  set velocities to use. */
			}
		else if( lkra( "VELOCIT$",9, 1, MVEL, cmsmm.vel, &cmsmm.nvel ) ){

			/* -- "TO hdr":  set starting header marker field. */
			}
		else if( lklist( "TO$",4, (char*)kmdfm.kpick[7],9, MPICK - 
		 7, &ipick ) ){
			strcpy( kmsmm.ktmark, kmdfm.kpick[ipick + 6] );

			/* -- Bad syntax. */
			}
		else{
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();

			}
		goto L_1000;

		}

	/* - The above loop is over when one of two conditions has been met:
	 *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
	 *   (2) All the tokens in the command have been successfully parsed. */

	if( *nerr != 0 )
		goto L_8888;

	/* CHECKING PHASE: */

	/* - Test for a non-null data file list. */

	vflist( nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Make sure each file is a time series file. */

	vftime( nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* EXECUTION PHASE: */

	/* - Convert the header marker field to starting locations in the
	 *   floating and character arrays. */

	ipick = nequal( kmsmm.ktmark, (char*)kmdfm.kpick,9, MPICK );
	ifpick = ipick + 10 - 7;
	ikpick = ipick + 6 - 7;

	/* - Make sure there are enough markers for the requested number
	 *   of travel times. */

	if( cmsmm.nvel <= (MPICK - ipick) ){
		nvelu = cmsmm.nvel;
		}
	else{
		nvelu = MPICK - ipick;
		setmsg( "WARNING", 1 );
		apcmsg( "Not enough markers for requested travel times.",47 );
		apcmsg( "Number of travel times reduced to",34 );
		apimsg( nvelu );
		wrtmsg( stdout );
		}

	/* - For each file in DFL: */

	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){

		/* -- Get next file from the memory manager.
		 *    (Header is moved into common blocks CMHDR and KMHDR.) */
		getfil( jdfl, TRUE, &nlen, &ndxy, &ndxx, nerr );
		if( *nerr != 0 )
			goto L_8888;

		/* -- Use requested distance or use the one in the header. */
		if( cmsmm.ldistr ){
			distu = cmsmm.distr;
			}
		else if( *dist != cmhdr.fundef ){
			distu = *dist;
			}
		else{
			*nerr = 1;
			setmsg( "ERROR", *nerr );
			apcmsg( "DIST",5 );
			apcmsg( "is not defined in header for file",34 );
            tmp = string_list_get(datafiles, jdfl-1);
            apcmsg2(tmp, strlen(tmp)+1);
			goto L_8888;
			}

		/* -- Compute origin time offset or use the one in the header. */
		if( cmsmm.lgmt ){
			ddttm( cmsmm.iodttm, nzdttm, &originu );
			}
		else if( cmsmm.loriginr ){
			originu = cmsmm.originr;
			}
		else if( *o != cmhdr.fundef ){
			originu = *o;
			}
		else{
			*nerr = 2;
			setmsg( "ERROR", *nerr );
			apcmsg( "ORIGIN",7 );
			apcmsg( "is not defined in header for file",34 );
            tmp = string_list_get(datafiles, jdfl-1);
            apcmsg2(tmp, strlen(tmp)+1);
			goto L_8888;
			}

		/* -- Compute theoretical arrival times and fill time pick header fields.
		 *    Put alphanumeric values of even velocities in time pick ids. */

		for( j = 1; j <= nvelu; j++ ){
			j_ = j - 1;
			Fhdr[ifpick + j - 1] = originu + distu/Vel[j];
      sprintf(kmhdr.khdr[ikpick + j_ - 1],"%3.1lf", Vel[j] );
			}

		/* -- Return file to memory manager. */
		putfil( jdfl, nerr );
		if( *nerr != 0 )
			goto L_8888;

		}

L_8888:
	return;

} /* end of function */

