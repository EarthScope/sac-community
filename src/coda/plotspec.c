#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "mach.h"
#include "gem.h"
#include "co.h"
#include "pl.h"
#include "gdm.h"
#include "gam.h"
#include "dfm.h"
#include "hdr.h"
#include "amf.h"
#include "dff.h"
#include "ucf.h"
#include "gtm.h"
#include "coda.h"
#include "bool.h"
#include "ucf.h"
#include "debug.h"

void /*FUNCTION*/ plotspec(envelopes,nbands,C_begin,nerr)
     struct envelope envelopes[MAXBANDS];
     float C_begin;
     int nbands;
     int *nerr;
{
	int lany, lframs, lxgens, lprint = FALSE ;
	int jdfl, nlcx, nlcy, nlen;
        int ndx1, ndxh, j, junk;
        float funk;
        float *Sacmem1, *Sacmem2;
        UNUSED(C_begin);
	/* PROCEDURE: */
	*nerr = 0;

	/* - Save current values of several GEM parameters. */

	lxgens = cmgem.xgen.on;
	lframs = cmgem.lframe;

	/* - Check for null data file list. */

	vflist( nerr );
	if( *nerr != 0 )
	    goto L_8888;

	/* - If no graphics device is open, try to open the default device. */

	getstatus( "ANY", &lany );
	if( !lany ){
	    zgetgd( kmgam.kgddef,9 );
	    begindevices( kmgam.kgddef,9, 1, nerr );
	    if( *nerr != 0 )
		goto L_8888;
	}

	/* EXECUTION PHASE: */
	/* - Save plot environment. */
        jdfl = 1;
	plsave();

	cleardfl( nerr ); 

	nlen = nbands;
       	cmdfm.ndfl = 1;
	crsac( jdfl, 2, nlen, &ndxh, &ndx1, &junk, nerr );
        putfil( jdfl, nerr);
	getfil( jdfl, TRUE, &nlen, &ndx1, &ndxh, nerr );
	Sacmem1 = cmmem.sacmem[ndx1];
	Sacmem2 = cmmem.sacmem[ndxh];
	
        nlen = 0;
	for( j = 0; j < nbands; j++ ){
          if(envelopes[j].Moment > 0.0) {
	    *(Sacmem1++) = envelopes[j].Moment;
	    *(Sacmem2++) = (envelopes[j].freq_high + envelopes[j].freq_low)/2;
	    nlen++;
	  }
	}
	*iftype = *ixy;
	*leven = FALSE;
	*npts = nlen;
	extrma( cmmem.sacmem[ndxh], 1, *npts, begin, ennd, &funk );
	extrma( cmmem.sacmem[ndx1], 1, *npts, depmin, depmax, depmen ); 
	putfil( jdfl, nerr );

	/* -- Get file from memory manager. */
	getfil( jdfl, TRUE, &nlen, &nlcy, &nlcx, nerr );
	if( *nerr != 0 )
	  goto L_8888;
	
	cmgem.xgen.on = FALSE;
        cmgem.lxlim = TRUE;
        cmgem.lylim = TRUE;

	/* -- Determine x axis plot limits. */
	/*	getxlm( &cmgem.lxlim, &cmgem.ximn, &cmgem.ximx ); */
	cmgem.ximn = envelopes[0].freq_low;;
	cmgem.ximx = envelopes[nbands-1].freq_high;
	
	/* -- Determine y axis plot limits. */
	getylm( &cmgem.lylim, &cmgem.yimn, &cmgem.yimx );
        cmgem.lxlim = TRUE;
        cmgem.lylim = TRUE;
	nlen = *npts;
        cmgem.yimn = cmgem.yimn - 1.0;
	cmgem.yimx = cmgem.yimx + 1.0;
        cmgem.ximn = cmgem.ximn - 0.02;
	cmgem.ximx = cmgem.ximx + 1.0;
        cmgem.ixint = AXIS_LOG; 
        cmgem.symsz = 0.015;
        cmgem.isymwidth = 1.0;
	cmgem.lwidth = TRUE;
        cmgem.isym = 14;
        setsymbolnum( cmgem.isym );
        cmgem.lsym = TRUE;
        cmgem.title.on = TRUE;
	sprintf(kmgem.ktitl, "Moment Spectrum from Coda Measurements");
        cmgem.title.len = (strcspn(kmgem.ktitl," ") + 1);
        cmgem.ylabel.on = TRUE;
	sprintf(kmgem.kylab, "Log Moment");
        cmgem.ylabel.len = (strcspn(kmgem.kylab," ") + 1);
        cmgem.xlabel.on = TRUE;
	sprintf(kmgem.kxlab, "Frequency (Hz)");
        cmgem.xlabel.len = (strcspn(kmgem.kxlab," ") + 1);
        cmgem.igtfnt=7;
	setvspacetype( FALSE, 1.0 );
        setsymbolsize( cmgem.symsz);

	beginframe( lprint , nerr );
	if( *nerr != 0 )
	  goto L_8888;
	getvspace( &cmgem.view.xmin, &cmgem.view.xmax, 
                   &cmgem.view.ymin, &cmgem.view.ymax );

	/* -- Plot the data.  Do not allow PL2D to perform framing. */
	cmgem.lframe = FALSE;
       	pl2d( cmmem.sacmem[nlcx], cmmem.sacmem[nlcy], nlen, 1, 1, nerr );
	if( *nerr != 0 )
	  goto L_8888;

	flushbuffer( nerr );
	
	/* - Restore GEM parameters before returning. */

L_8888:
	cmgem.xgen.on = lxgens;
	cmgem.lframe = lframs;
	plrest();
	return;

} /* end of function */

