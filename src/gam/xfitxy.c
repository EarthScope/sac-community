#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "complex.h"
#include "proto.h"
#include "mach.h"
#include "gem.h"
#include "gam.h"
#include "dfm.h"
#include "hdr.h"
#include "amf.h"
#include "bool.h"


#include "msg.h"
#include "scm.h"
#include "clf.h"
#include "cpf.h"
#include "dff.h"
#include "co.h"

void /*FUNCTION*/ xfitxy(nerr)
int *nerr;
{
	char kfile[MCPFN+1];
	int lchange;
	int idflnumber[MDFL], jdfl, 
	 jdflnumber, ncfile, ndflnumber, nlcx, nlcy, notused, 
	 num, numx, numy;
	float cc, sig, siga, sigb;

	int *const Idflnumber = &idflnumber[0] - 1;
    char *tmpx, *tmpy;

	/*=====================================================================
	 * PURPOSE:  To execute the action command FITXY.
	 *           This command fits a line through pairs of data files.
	 *           The user specifies which data file contains the "x" data
	 *           and which data file(s) contain the "y" data.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers:
	 *=====================================================================
	 * MODULE/LEVEL:   gam/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:    MCMSG
	 *    gem:     lframe, xpmn, xpmx, ypmn, ypmx, lpen, ipen, lcol, icol,
	 *             lsym, isym, iskpen, iskcol, tsdef, chwid, chht
	 *    gam:     kgddef, lfidrq, ifidtp, kfidnm, ifidlc, fidbdr, tsfid
	 *    dfm:     ndfl
	 *    hdr:     depmin, depmax
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    gem:     lxlim, ximn, ximx, lylim, yimn, yimx
	 *    gam:     xfidlc, yfidlc
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  lcmore, lcirc, cfmt, cresp, vflist, plrest, getstatus
	 *             begindevice, plsave, getfil, ldttm, setcolor,
	 *             getxlm, getylm, plmap, settextsize,
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    atrwid:  Width of linestyle/symbol attribute part of fileid. [f]
	 *    xlinl1:  X beginning of linestyle attribute display. [f]
	 *    xlinl2:  X ending of linestyle attribute display. [f]
	 *    xsymlc:  X location of symbol attribute display. [f]
	 *    yatrlc:  Y location of linestyle/symbol attribute display. [f]
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    890420:  Original version based on xp2.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  890420
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* PARSING PHASE: */

	/* - Loop on each token in command: */

	jdflnumber = 0;
	lchange = FALSE;
L_1000:
	if( lcmore( nerr ) ){

		/* -- integer:  the index number of data file in data file list. */
		if( lcirc( 1, cmdfm.ndfl, &jdfl ) ){
			jdflnumber = jdflnumber + 1;
			Idflnumber[jdflnumber] = jdfl;
			lchange = TRUE;

			/* -- "filename":  the name of a data file in the data file list. */
			}
		else if( lcchar( MCPFN, kfile,MCPFN+1, &ncfile ) ){
			jdfl = 1 + string_list_find(datafiles, kfile, MCPFN+1);
			if( jdfl > 0 ){
				jdflnumber = jdflnumber + 1;
				Idflnumber[jdflnumber] = jdfl;
				lchange = TRUE;
				}
			else{
				*nerr = 5106;
				setmsg( "ERROR", *nerr );
				apcmsg( kfile,MCPFN+1 );
				goto L_8888;
				}

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

    ndflnumber = 0;
	if( lchange )
		ndflnumber = jdflnumber;

	/* CHECKING PHASE: */

	/* - Make sure there are at least two data files specified. */

	if( ndflnumber < 2 ){
		*nerr = 1505;
		setmsg( "ERROR", *nerr );
		goto L_8888;
		}

	/* - Check for null data file list. */

	vflist( nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* EXECUTION PHASE: */

	/* - Loop on each pair of files to compute straight line fit. */

	jdfl = Idflnumber[1];
	getfil( jdfl, TRUE, &numx, &nlcx, &notused, nerr );
	if( *nerr != 0 )
		goto L_8888;
    tmpx = string_list_get(datafiles, jdfl);
	for( jdflnumber = 2; jdflnumber <= ndflnumber; jdflnumber++ ){
		jdfl = Idflnumber[jdflnumber];
		getfil( jdfl, TRUE, &numy, &nlcy, &notused, nerr );
		if( *nerr != 0 )
			goto L_8888;
		num = min( numx, numy );
		lifitu( cmmem.sacmem[nlcx], cmmem.sacmem[nlcy], num, a, b, &siga, &sigb, 
		 &sig, &cc );
        tmpy = string_list_get(datafiles, jdfl-1);
		setmsg( "INFO", 1 );
		apcmsg( "Slope and intercept for",24 );
        apcmsg2(tmpy, strlen(tmpy)+1);
		apcmsg( "vs.",4 );
        apcmsg2(tmpx, strlen(tmpx)+1);
		apcmsg( ":",2 );
		apfmsg( *a );
		apfmsg( *b );
		outmsg();
		}

L_8888:
	return;

} /* end of function */

