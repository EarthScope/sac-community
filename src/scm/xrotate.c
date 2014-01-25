
#include <string.h>

#include "scm.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"


#include "msg.h"
#include "clf.h"
#include "ucf.h"
#include "cpf.h"
#include "dff.h"

void /*FUNCTION*/ xrotate(nerr)
int *nerr;
{
	int lhorz, lnpin, lnpout;
	int jdfl, ndx1, ndx2, ndxx, 
	 ndxy, nlen, nlen1, nlen2, notused;
	float cmpaz1, cmpaz2, cmpin1, cmpin2, delaz, delin, rotang, rotaz, 
	 v270m, v270p, v90m, v90p;
  char *tmp1, *tmp2;

	/*=====================================================================
	 * PURPOSE:  To execute the action command ROTATE.
	 *           This command rotates pairs of files through an angle.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers: 2001, 2002, 2003, 2004, 2010.
	 *=====================================================================
	 * MODULE/LEVEL:  scm/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *    hdr:
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    scm:     krottp, usraz, usrang, lnpreq
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    880128:  Added check to make sure pairs of files an have equal 
	 *             number of data points.
	 *    810325:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* PARSING PHASE: */

	/* - Loop on each token in command: */

L_1000:
	if( lcmore( nerr ) ){

		/* -- "TO GCP/v":  rotate to great circle path or a fixed angle. */
		if( lckey( "TO$",4 ) ){
			if( lckey( "G$",3 ) ){
				strcpy( kmscm.krottp, "HDRGCP  " );
				}
			else if( lcreal( &cmscm.usraz ) ){
				strcpy( kmscm.krottp, "USRAZ   " );
				}
			else{
				cfmt( "NEED \"gcp\" OR AN ANGLE:",25 );
				cresp();
				}

			/* -- "TO v":  rotate through a fixed angle. */
			}
		else if( lkreal( "TH$",4, &cmscm.usrang ) ){
			strcpy( kmscm.krottp, "USRANG  " );

			/* -- "NORMAL/REVERSED":  define polarity of rotation. */
			}
		else if( lclog2( "N$",3, "R$",3, &cmscm.lnpreq ) ){

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

	/*TEMPORARY */
	v90m = 89.99;
	v90p = 90.01;
	v270m = 269.99;
	v270p = 270.01;

	/* CHECKING PHASE: */

	/* - Check for null data file list. */

	vflist( nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Make sure there is an even number of data files. */

	if( (cmdfm.ndfl/2)*2 != cmdfm.ndfl ){
		*nerr = 2001;
		setmsg( "ERROR", *nerr );
		goto L_8888;
		}

	/* EXECUTION PHASE: */

	/* - Perform the requested function on each pair of files in DFL. */

	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl += 2 ){
		/* -- Retrieve the file name indices. */
        tmp1 = string_list_get(datafiles, jdfl-1);
        tmp2 = string_list_get(datafiles, jdfl-1+1);

		/* -- Get the second file in each pair, moving header to CMHDR. */

		getfil( jdfl + 1, TRUE, &nlen2, &ndx2, &notused, nerr );
		if( *nerr != 0 )
			goto L_8888;

		/* -- Save some variables from the header in local variables. */

		cmpaz2 = *cmpaz;
		cmpin2 = *cmpinc;

		/* -- Get the first file in each pair, moving header to CMHDR. */

		getfil( jdfl, TRUE, &nlen1, &ndx1, &notused, nerr );
		if( *nerr != 0 )
			goto L_8888;
		cmpaz1 = *cmpaz;
		cmpin1 = *cmpinc;

		/* -- Check to make sure both files have the same number of points. */

		if( nlen1 == nlen2 ){
			nlen = nlen1;
			}
		else{
			*nerr = 2010;
			setmsg( "ERROR", *nerr );
            apcmsg2(tmp1, strlen(tmp1)+1);
            apcmsg2(tmp2, strlen(tmp2)+1);
			goto L_8888;
			}

		/* -- Get azimuth or back azimuth from header variables if requested. */

		if( strcmp(kmscm.krottp,"HDRGCP  ") == 0 ){
			if( ((*stlo != cmhdr.fundef && *stla != cmhdr.fundef) && 
			 *evlo != cmhdr.fundef) && *evla != cmhdr.fundef ){
				distaz( *evla, *evlo, (float*)stla, (float*)stlo, 
				 1, (float*)dist, (float*)az, (float*)baz, (float*)gcarc, 
				 nerr );
				if( *nerr != 0 )
					goto L_8888;
				rotaz = *baz + 180.;
				}
			else{
				*nerr = 2004;
				setmsg( "ERROR", *nerr );
                apcmsg2(tmp1, strlen(tmp1)+1);
				goto L_8888;
				}
			}
		else if( strcmp(kmscm.krottp,"USRAZ   ") == 0 ){
			rotaz = cmscm.usraz;
        } else { /* USRANG */
            rotaz = 0.0; 
        }

		/* -- Determine the angular relationship between the pair of
		 *    files and compute the rotation angle. */

		if( linrng( cmpin1, v90m, v90p ) && linrng( cmpin2, v90m, 
		 v90p ) ){
			lhorz = TRUE;
			delaz = cmpaz2 - cmpaz1;
			if( linrng( delaz, v90m, v90p ) || linrng( -delaz, v270m, 
			 v270p ) ){
				lnpin = TRUE;
				}
			else if( linrng( -delaz, v90m, v90p ) || linrng( delaz, 
			 v270m, v270p ) ){
				lnpin = FALSE;
				}
			else{
				*nerr = 2002;
				setmsg( "ERROR", *nerr );
                apcmsg2(tmp1, strlen(tmp1)+1);
                apcmsg2(tmp2, strlen(tmp2)+1);
				goto L_8888;
				}
			if( strcmp(kmscm.krottp,"USRANG  ") != 0 ){
				rotang = rotaz - cmpaz1;
				lnpout = cmscm.lnpreq;
				}
			else{
				rotang = cmscm.usrang;
				lnpout = lnpin;
				}
			}
		else if( strcmp(kmscm.krottp,"USRANG  ") == 0 ){
			lhorz = FALSE;
			delin = cmpin2 - cmpin1;
			if( linrng( delin, v90m, v90p ) || linrng( -delin, v90m, 
			 v90p ) ){
				lnpin = TRUE;
				}
			else if( linrng( -delin, v90m, v90p ) || linrng( delin, 
			 v270m, v270p ) ){
				lnpin = FALSE;
				}
			else{
				*nerr = 2002;
				setmsg( "ERROR", *nerr );
                apcmsg2(tmp1, strlen(tmp1)+1);
                apcmsg2(tmp2, strlen(tmp2)+1);
				goto L_8888;
				}
			rotang = cmscm.usrang;
			lnpout = lnpin;
			}
		else{
			*nerr = 2003;
			setmsg( "ERROR", *nerr );
            apcmsg2(tmp1, strlen(tmp1)+1);
            apcmsg2(tmp2, strlen(tmp2)+1);
			goto L_8888;
			}

		/* -- Perform rotation of signal pair. */

		rotate( cmmem.sacmem[ndx1], cmmem.sacmem[ndx2], nlen, rotang, lnpin, 
		 lnpout, cmmem.sacmem[ndx1], cmmem.sacmem[ndx2] );

		/* -- Update any header fields that may have changed. */

		/* --- First component of pair. */

		getfil( jdfl, FALSE, &nlen, &ndxy, &ndxx, nerr );
		if( *nerr != 0 )
			goto L_8888;
		if( lhorz ){
			*cmpaz = *cmpaz + rotang;
L_4000:
			if( *cmpaz >= 360. ){
				*cmpaz = *cmpaz - 360.;
				goto L_4000;
				}
			else if( *cmpaz < 0. ){
				*cmpaz = *cmpaz + 360.;
				goto L_4000;
				}
			}
		else{
			*cmpinc = *cmpinc + rotang;
L_4100:
			if( *cmpinc > 180. ){
				*cmpinc = *cmpinc - 360.;
				goto L_4100;
				}
			else if( *cmpinc <= -180. ){
				*cmpinc = *cmpinc + 360.;
				goto L_4100;
				}
			}
		strcpy( kcmpnm, kmhdr.kundef );
		*lpspol = TRUE;
		extrma( cmmem.sacmem[ndx1], 1, nlen, depmin, depmax, depmen );
		putfil( jdfl, nerr );
		if( *nerr != 0 )
			goto L_8888;

		/* --- Second file: */

		getfil( jdfl + 1, FALSE, &nlen, &ndxy, &ndxx, nerr );
		if( *nerr != 0 )
			goto L_8888;
		if( lhorz ){
			if( (lnpin && !lnpout) || (!lnpin && lnpout) ){
				*cmpaz = *cmpaz + rotang + 180.;
				}
			else{
				*cmpaz = *cmpaz + rotang;
				}
			*lpspol = lnpout;
L_4200:
			if( *cmpaz >= 360. ){
				*cmpaz = *cmpaz - 360.;
				goto L_4200;
				}
			else if( *cmpaz < 0. ){
				*cmpaz = *cmpaz + 360.;
				goto L_4200;
				}
			}
		else{
			*cmpinc = *cmpinc + rotang;
L_4300:
			if( *cmpinc > 180. ){
				*cmpinc = *cmpaz - 360.;
				goto L_4300;
				}
			else if( *cmpinc <= -180. ){
				*cmpinc = *cmpaz + 360.;
				goto L_4300;
				}
			*lpspol = TRUE;
			}
		strcpy( kcmpnm, kmhdr.kundef );
		extrma( cmmem.sacmem[ndx2], 1, nlen, depmin, depmax, depmen );
		putfil( jdfl + 1, nerr );
		if( *nerr != 0 )
			goto L_8888;

		}

	/* - Calculate and set new range of dependent variable. */

	setrng();

L_8888:
	return;

} /* end of function */

