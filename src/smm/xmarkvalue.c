
#include <stdio.h>
#include <string.h>

#include "smm.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"


#include "msg.h"
#include "clf.h"
#include "bot.h"
#include "cpf.h"
#include "dff.h"

void /*FUNCTION*/ xmarkvalue(nerr)
int *nerr;
{
	char kdescr[9];
	int ifpick, ikpick, index, ipick, j, jdfl, 
    ndxx, ndxy, nlen, nlnatw, nofatw;
	double tmax, tmin;
  char *tmp;

	/*=====================================================================
	 * PURPOSE: To parse and execute the action command MARKVALUE.
	 *          This command marks the first occurance of a data value.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *=====================================================================
	 * MODULE/LEVEL:
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    DFM:     NDFL, KDFL
	 *    SMM:     LMTW, KMTW, OMTW
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    HDR:     
	 *    SMM:     KVMARK, LGEDATA
	 *    MEM:     SACMEM
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  LCMORE, CFMT, CRESP
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    870128:  Added use of measurement time window (MTW).
	 *    861128:  Moved to SMM as an internal SAC command called MARKVALUE.
	 *    841207:  Reworked into new XSC format.
	 *    830124:  Original version called LDV.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* PARSING PHASE: */

	/* - Loop on each token in command: */

L_1000:
	if( lcmore( nerr ) ){

		/* -- "GE v":  search for first point that is greater than or equal to v. */
		if( lkreal( "GE$",4, &cmsmm.value ) ){
			cmsmm.lgedata = TRUE;

			/* -- "LE v":  search for first point that is less than or equal to v. */
			}
		else if( lkreal( "LE$",4, &cmsmm.value ) ){
			cmsmm.lgedata = FALSE;

			/* -- "TO hdrvar":  the name of the header variable to store time. */
			}
		else if( lklist( "TO$",4, (char*)kmdfm.kpick[7],9, MPICK - 
		 7, &ipick ) ){
			strcpy( kmsmm.kvmark, kmdfm.kpick[ipick + 6] );

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

	/* - Determine offset in header arrays for requested time pick. */

	ipick = nequal( kmsmm.kvmark, (char*)kmdfm.kpick,9, MPICK );
	ifpick = ipick + 10 - 7;
	ikpick = ipick + 6 - 7;

	/* - Format description for storage in header. */

	if( cmsmm.lgedata ){
                sprintf(kdescr,"%s%5.1f", "GE ", cmsmm.value );
		}
	else{
                sprintf(kdescr,"%s%5.1f", "LE ", cmsmm.value );
		}

	/* - Perform the requested function on each file in DFL. */

	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){

		/* -- Get next file from the memory manager.
		 *    (Header is moved into common blocks CMHDR and KMHDR.) */
		getfil( jdfl, TRUE, &nlen, &ndxy, &ndxx, nerr );
		if( *nerr != 0 )
			goto L_8888;

		/* -- Determine measurement window. */
		if( cmsmm.lmtw ){
			getatw( (char*)kmsmm.kmtw,9, cmsmm.omtw, &tmin, &tmax, 
			 &nofatw, &nlnatw, nerr );
			if( *nerr != 0 )
				goto L_8888;
			}
		else{
			nofatw = 0;
			nlnatw = *npts;
			}

		/* -- Perform search of data file. */
		index = -1;
		if( cmsmm.lgedata ){
			for( j = nofatw; j <= (nofatw + nlnatw - 1); j++ ){
               if( *(cmmem.sacmem[ndxy] + j) >= cmsmm.value ){
					index = j;
					goto L_4000;
					}
				}
			}
		else{
			for( j = nofatw; j <= (nofatw + nlnatw - 1); j++ ){
				if( *(cmmem.sacmem[ndxy] + j) <= cmsmm.value ){
					index = j;
					goto L_4000;
					}
				}
			}

		/* -- Store in the requested header field. */
L_4000:
		if( index >= 0 ){
			Fhdr[ifpick] = *b + (float)( index )**delta;
			strcpy( kmhdr.khdr[ikpick - 1], kdescr );
			}
		else{
			Fhdr[ifpick] = cmhdr.fundef;
			strcpy( kmhdr.khdr[ikpick - 1], kmhdr.kundef );
			setmsg( "WARNING", 1 );
			apcmsg( "Could not find value",21 );
			apfmsg( cmsmm.value );
			apcmsg( "in file",8 );
            tmp = string_list_get(datafiles, jdfl-1);
            apcmsg2(tmp, strlen(tmp)+1);
			wrtmsg( stdout );
			}

		/* -- Return file to memory manager. */
		putfil( jdfl, nerr );
		if( *nerr != 0 )
			goto L_8888;

		}

L_8888:
	return;

} /* end of function */

