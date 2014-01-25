
#include <string.h>

#include "sss.h"
#include "tt.h"
#include "amf.h"
#include "dfm.h"
#include "bool.h"


#include "msg.h"

#define	MAX_	MAX_PHASES

void iaspmodel(zs, dstart, dinc, nloops, nerr)
double zs;
float *dstart;
double dinc;
int nloops, *nerr;
{
	char kphcd[MAX_][9] ;
	static char kphlst[2][9];
	static int lprnt[3];
	int iloops, jdx, jdx_, jen, jen_, jph, 
        jph_, ndx, nblksz;
	float dddp[MAX_], dtdd[MAX_], dtdh[MAX_], tt[MAX_], ttscale, usrc[2];
	static int _aini = 1;

	int *const Lprnt = &lprnt[0] - 1;
	float *const Tt = &tt[0] - 1;

	if( _aini ){ /* Do 1 TIME INITIALIZATIONS! */
	    strcpy( kphlst[0], "all     " );
	    Lprnt[3] = TRUE;
	    _aini = 0;
	}

	/*=====================================================================
	 * PURPOSE:  To generate travel time curves from the iasp91 model files.
	 *           This is from some other source.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers: 
	 *=====================================================================
	 * MODULE/LEVEL: SSS/3
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    960829:  Now sets up array in X direction in case the data is
	 *             plotted in portrait mode.  Also established default
	 *             units: degrees for the models; the user can change
	 *             this at the command line. 
	 *    920805:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* INITIALIZATION PHASE: */

	Lprnt[1] = FALSE;
	Lprnt[2] = FALSE;
	tabin( kmtt.kmodel,9 );
	brnset( 1, (char*)kphlst,9, lprnt );
	depset( zs, usrc );

    for(jdx = 0; jdx < MAX_; jdx++) {
        tt[jdx] = 0.0;
    }

	/* - Main loop on each  */

	if( cmtt.ittunit == TTDEGREE ){
	    ttscale = 1.0;
	}
	else{
	    ttscale = RKMPERDG;
	}


	/* -- Allocate memory blocks for y channel(s). */
	nblksz = nloops + 1;
	if( cmtt.nphases >= 1 ){
	    for( jdx = 1; jdx <= cmtt.nphases; jdx++ ){
		jdx_ = jdx - 1;
		Ltteven[jdx + cmtt.nttm] = TRUE;
		Xttfirst[jdx + cmtt.nttm] = *dstart;
		Xttdel[jdx + cmtt.nttm] = dinc*ttscale;
		Nttpt[cmtt.nttm + jdx] = nloops;
		allamb( &cmmem, nblksz, &Ndxtty[jdx + cmtt.nttm], nerr );
		if ( *nerr != 0 )
		    goto L_8888;
		/* next five lines added to set X values. maf 960829 */
		allamb( &cmmem, nblksz, &Ndxttx[jdx + cmtt.nttm], nerr );
		if( *nerr != 0 ) {
		    relamb( cmmem.sacmem, Ndxtty[jdx+cmtt.nttm], nerr );
		    goto L_8888;
		}
		for( ndx = 0; ndx <= nloops; ndx++ ){
		    *( cmmem.sacmem[Ndxtty[jdx + cmtt.nttm]] + ndx ) = -1.0;
		    /* next line added to set X values. maf 960829 */
		    /* This line will have to be tested when dinc gets a value other than 1 */
		    *( cmmem.sacmem[Ndxttx[jdx + cmtt.nttm]] + ndx ) = ndx * Xttdel[jdx + cmtt.nttm] ;
		}
		strcpy( kmtt.kttnm[jdx_ + cmtt.nttm], kmtt.kphases[jdx_] );
	    } /* end for( jdx = 1; jdx <= cmtt.nphases; jdx++ ) */
	} /* end if( cmtt.nphases >= 1 ) */


	/* --- Fill in the channels, according to the travel times */

	for( iloops = 1; iloops <= nloops; iloops++ ){
        ndx = 0;
	    trtm( *dstart, MAX_, &ndx, tt, dtdd, dtdh, dddp, (char*)kphcd ,9 );
	    if( ndx > 0 ){
            for( jph = 1; jph <= cmtt.nphases; jph++ ){
                jph_ = jph - 1;
                for( jen = 1; jen <= ndx; jen++ ){
                    jen_ = jen - 1;
                    if(memcmp(kphcd[jen_],kmtt.kphases[jph_],MTTLEN) == 0 ) {
                        *(cmmem.sacmem[Ndxtty[cmtt.nttm + jph]] + iloops - 1) = Tt[jen];
                    }
                }
            }
	    }
	    *dstart = *dstart + dinc;
	} 


	/* -- Come to here on end-of-file. */

	/* -- Update current number of travel time curves. */
	cmtt.nttm = cmtt.nttm + cmtt.nphases;

	/* - Check again for a non-null DFL. */

	if( cmtt.nttm <= 0 ){
	    *nerr = 1301;
	    setmsg( "ERROR", *nerr );
	}


	/* - Return. (Try to close data file just to be sure.) */

L_8888:
	iaspcl( nerr );
	return;

} /* end of function */
/*                                    choose source depth */

