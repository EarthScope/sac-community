
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scm.h"
#include "dfm.h"
#include "hdr.h"
#include "amf.h"
#include "bool.h"

#include "co.h"
#include "msg.h"
#include "clf.h"
#include "ucf.h"
#include "cpf.h"
#include "dff.h"

void /*FUNCTION*/ xquantize(nerr)
int *nerr;
{
	int irange, ivalue, j, jdfl, jqgain, 
	 nclip, ndxx, ndxy, nlen, nqgain;
	float factor, half, temp;

    float *Sacmem;
    char *tmp;


	/*=====================================================================
	 * PURPOSE: To parse and execute the action command QUANTIZE.
	 *          This command quantizes the data in DFL.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *=====================================================================
	 * MODULE/LEVEL:
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    DFM:     NDFL
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    HDR:     DEPMIN, DEPMAX, DEPMEN
	 *    MEM:     SACMEM
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    870217:  Modified error and warning messages.
	 *    861129:  Converted to internal SAC command.
	 *    860417:  Modified to fit into new XSC format.
	 *    830124:  Modified to include new set of parsing/checking functions.
	 *    810624:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  870217
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;
    nqgain = 0;
	/* PARSING PHASE */

	/* - Loop on each token in command. */

L_1000:
	if( lcmore( nerr ) ){

		/* -- "GAINS ilist":  list of allowed gains. */
		if( lkia( "GAINS$",7, 1, MQGAIN, cmscm.iqgain, &nqgain ) ){
			Iqgain[nqgain + 1] = 1;

			/* -- "LEVEL v":  quantization level for lowest gain. */
			}
		else if( lkreal( "LEVEL$",7, &cmscm.qlevel ) ){

			/* -- "MANTISSA n":  number of bits in mantissa. */
			}
		else if( lkint( "MANTISS$",9, &cmscm.nqmant ) ){

			/* -- Bad syntax. */
			}
		else{
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();

			}
		goto L_1000;

		}

	/* - The above loop is over when one of two conditions has beenmet:
	 *   (1) An error in parsing has occurred.  In this case NERR is > 0
	 *   (2) All the tokens in the command have been successfully parsed */

	if( *nerr != 0 )
		goto L_8888;

	/* CHECKING PHASE: */

	/* - Test for a non-null data file list. */

	vflist( nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Make sure each file is an evenly spaced time series file. */

	vfeven( nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Make sure gains are monotonically decreasing. */

	for( j = 2; j <= nqgain; j++ ){
		if( Iqgain[j] >= Iqgain[j - 1] ){
			*nerr = 2006;
			setmsg( "ERROR", *nerr );
			goto L_8888;
			}
		}

	/* EXECUTION PHASE: */

	/* - Convert number of bits in mantissa to maximum integer range. */

	irange = (ipow(2,cmscm.nqmant) - 1)/2;

	/* - Calculate scale factors. */

	factor = cmscm.qlevel*(float)( Iqgain[1] );

	/* - For each file in DFL: */

	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){

		/* -- Get next file from the memory manager.
		 *   (Header is moved into common blocks CMHDR and KMHDR.) */

		getfil( jdfl, TRUE, &nlen, &ndxy, &ndxx, nerr );
		if( *nerr != 0 )
			goto L_8888;

		/* -- For each data point in file:
		 *    (1) Determine proper gain.
		 *    (2) Store quantized value (using truncation model.) */

		nclip = 0;
                Sacmem = cmmem.sacmem[ndxy];
		for( j = ndxy; j <= (ndxy + nlen - 1); j++ ){
			jqgain = 1;
			temp = *Sacmem;
			half = sign( 0.5, temp );
L_3000:
			ivalue = (int)( half + temp*(float)( Iqgain[jqgain] )/
			 factor );
			if( labs( ivalue ) > irange ){
				jqgain = jqgain + 1;
				if( jqgain <= nqgain )
					goto L_3000;
				nclip = nclip + 1;
				ivalue = irange + 1;
				}
			*(Sacmem++) = (float)( ivalue )*factor/(float)( Iqgain[jqgain] );
			}

		/* -- Write warning message if any data points clipped. */

		if( nclip > 0 ){
			setmsg( "WARNING", 1 );
			apimsg( nclip );
			apcmsg( "data point(s) clipped in file:",31 );
            tmp = string_list_get(datafiles, jdfl-1);
            apcmsg2(tmp, strlen(tmp)+1);
			wrtmsg( stdout );
			}

		/* -- Update any header fields that may have changed. */

		extrma( cmmem.sacmem[ndxy], 1, nlen, depmin, depmax, depmen );

		/* -- Return file to memory manager. */

		putfil( jdfl, nerr );
		if( *nerr != 0 )
			goto L_8888;

		}

	/* - Calculate and set new range of dependent variable. */

	setrng();

L_8888:
	return;

} /* end of function */

