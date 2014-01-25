#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "gem.h"
#include "gdm.h"
#include "gam.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "xyz.h"
#include "bool.h"


#include "gtm.h"
#include "pl.h"
#include "bot.h"
#include "msg.h"
#include "clf.h"
#include "cpf.h"
#include "co.h"
#include "dff.h"

void xplotdy(int *nerr)
{
	char kfile[ MCPFN + 1 ] ; 
	int lany, lchange, lydlimj, lprint = FALSE , ltry = FALSE ;
	int idx, idflnumber[MDFL], issym, jdfl, jdflnumber, 
	 ncfile, ndflnumber, ndx2, nlcdy, nlcdy2, nlcx, nlcy, num, numdy,
	 notused ;
	float vportratio, xarray[3], yarray[3], ydimnj, ydimxj, 
	 ydvalue, ydyimx, yrange, yvalue;

        float *Sacmem1, *Sacmem2, *Sacmem3, *Sacmem4;

	int *const Idflnumber = &idflnumber[0] - 1;
	float *const Xarray = &xarray[0] - 1;
	float *const Yarray = &yarray[0] - 1;

    Sacmem1 = NULL;
    Sacmem2 = NULL;
    Sacmem3 = NULL;
    Sacmem4 = NULL;
    memset(xarray, 0, sizeof(xarray));
    memset(yarray, 0, sizeof(xarray));

	/*=====================================================================
	 * PURPOSE:  To execute the action command PLOTDY.
	 *           The user specifies which data file contains the "y" data &
	 *           which data file contains the "dy" data.
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
	 *             lsym, isym, iskpen, iskcol, tsdef, chwid, chht, icline
	 *    gam:     kgddef, lfidrq, ifidtp, kfidnm, ifidlc, fidbdr, tsfid
	 *    dfm:     ndfl
	 *    hdr:     depmin, depmax
	 *    xyz:     laspect
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    gem:     lxlim, ximn, ximx, lylim, yimn, yimx
	 *    gam:     xfidlc, yfidlc
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  lcmore, lcirc, cfmt, cresp, vflist, plrest, getstatus
	 *             begindevices, plsave, getfil, ldttm, setcolor,
	 *             getxlm, getylm, beginframe, plmap, settextsize,
	 *             pltext, symbol, pldta, plgrid, plhome, endframe
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    atrwid:  Width of linestyle/symbol attribute part of fileid. [f]
	 *    xlinl1:  X beginning of linestyle attribute display. [f]
	 *    xlinl2:  X ending of linestyle attribute display. [f]
	 *    xsymlc:  X location of symbol attribute display. [f]
	 *    yatrlc:  Y location of linestyle/symbol attribute display. [f]
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    910608:  Added call to zgetgd when no graphics device specified.
	 *             Changed call to begindevice to begindevices. (wct)
	 *    910301:  Changed iline to icline.
	 *    901215:  Fixed xlim option, added 'lxlim = .true. if limits are set.
	 *    901121:  Added aspect option, like is used in contour command. wct
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

	while ( lcmore( nerr ) ){

	    /* -- ASPECT ON|OFF:  maintain aspect ratio of data or not. */
	    if( lklog( "ASPECT$",8, &cmxyz.laspect ) )
	    { /* do nothing */ }

            /* if PRINT option is tried, get printer name */
            else if ( ltry ) {
                lcchar ( MAXPRNTRNAMELEN   , kmgem.kptrName ,
                         MAXPRNTRNAMELEN+1 , &notused ) ;
                terminate ( kmgem.kptrName ) ;
                if ( !lprint )
                    kmgem.kptrName[0] = '\0' ;

                ltry = FALSE ;
            }

            /* -- "PRINT":  print the final product */
            else if( lckey( "PRINT#$", 8 ) ) {
                ltry = TRUE ;
                if ( cmgdm.lbegf ) {
                    setmsg ( "WARNING" , 2403 ) ;
                    outmsg () ;
                    clrmsg () ;
                }
                else {
                    lprint = TRUE ;
                }
            }

	    /* -- integer: the index number of file in data file list. */
	    else if( lcirc( 1, cmdfm.ndfl, &jdfl ) ){
		jdflnumber = jdflnumber + 1;
		Idflnumber[jdflnumber] = jdfl;
		lchange = TRUE;
	    }

	    /* -- "filename":  the name of a file in the data file list. */
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

    ndflnumber = 0;
	if( lchange )
	    ndflnumber = jdflnumber;

	/* CHECKING PHASE: */

	/* - Make sure there are two data files specified. */

	if( ndflnumber != 2 && ndflnumber != 3 ){
	    *nerr = 1505;
	    setmsg( "ERROR", *nerr );
	    goto L_8888;
	}

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

	/* - Save current plot environment. */

	plsave();

	/* - Set dy limits */

	getfil( Idflnumber[2], TRUE, &numdy, &nlcdy, &ndx2, nerr );
	if( *nerr != 0 )
	    goto L_8888;
	getylm( &lydlimj, &ydimnj, &ydimxj );
	ydyimx = fmax( fabs( ydimnj ), fabs( ydimxj ) );

	/* - Set dy2 limits */

	if( ndflnumber != 2 ){
	    getfil( Idflnumber[3], TRUE, &numdy, &nlcdy2, &ndx2, nerr );
	    if( *nerr != 0 )
		goto L_8888;
	    getylm( &lydlimj, &ydimnj, &ydimxj );
	    ydyimx = fmax( fabs( ydimnj ), ydyimx);
	    ydyimx = fmax( fabs( ydimxj ), ydyimx);
	}

	/* - Set x axis limits on data file unless limits are already set. */

	getfil( Idflnumber[1], TRUE, &num, &nlcy, &nlcx, nerr );
	if( *nerr != 0 )
	    goto L_8888;
	getxlm( &cmgem.lxlim, &cmgem.ximn, &cmgem.ximx );
	num = min( num, numdy );

	/* - Set y axis limits based on data file plus dy 
	     unless limits are already set. */

	getylm( &cmgem.lylim, &cmgem.yimn, &cmgem.yimx );
	if( !cmgem.lylim ){
	    yrange = *depmax - *depmin;
	    cmgem.yimn = *depmin - cmgem.yfudg*yrange - ydyimx;
	    cmgem.yimx = *depmax + cmgem.yfudg*yrange + ydyimx;
	}
	cmgem.lylim = TRUE;
	cmgem.lxlim = TRUE;

	/* - Set background and skeleton attributes. */

	settexttype( kmgem.kgtqua );
	settextfont( cmgem.igtfnt );
	setlinestyle( LINE_STYLE_SOLID );
	setcolor( cmgem.iskcol );

	/* -- Set viewport using different aspect ratio if ASPECT ON. */
	if( cmxyz.laspect ){
	    vportratio = fabs( (cmgem.yimx - cmgem.yimn) /
			       (cmgem.ximx - cmgem.ximn) );
	    setvspacetype( FALSE, vportratio );
	    getvport( &cmgem.uplot.xmin, &cmgem.uplot.xmax, &cmgem.uplot.ymin, &cmgem.uplot.ymax );
	}
	else{
	    setvspacetype( TRUE, 1.0 );
	    getvport( &cmgem.uplot.xmin, &cmgem.uplot.xmax, &cmgem.uplot.ymin, &cmgem.uplot.ymax );
	}

	/* - Begin new frame if requested. */

	if( cmgem.lframe ){
	    beginframe( lprint , nerr );
	    if( *nerr != 0 )
		goto L_8888;
	    getvspace( &cmgem.view.xmin, &cmgem.view.xmax,
		       &cmgem.view.ymin, &cmgem.view.ymax );
	}

	/* - Calculate mapping transformation for these fixed limits.
	 *   (In this case, all passed variables but NERR are unused.) */

	plmap( cmmem.sacmem[1], cmmem.sacmem[1], 1, 1, 1, nerr );
	if( *nerr != 0 )
	    goto L_8888;


	/* - Loop to plot an error bar for each point in main file */

	cmgem.lline = TRUE;
	cmgem.lsym = TRUE;
	issym = cmgem.isym;
	cmgem.isym = 1;
	setsymbolnum( cmgem.isym );
	cmgem.xgen.on = FALSE;
	cmgem.ygen.on = FALSE;

        if ( !*leven) Sacmem1 = cmmem.sacmem[nlcx];
        Sacmem2 = cmmem.sacmem[nlcy];
        Sacmem3 = cmmem.sacmem[nlcdy];
        if ( ndflnumber != 2) Sacmem4 = cmmem.sacmem[nlcdy2];

	for( idx = 1; idx <= num; idx++ ){

	    if( *leven ){
		Xarray[1] = (float)( idx - 1 )**delta + *begin;
	    }
	    else{
            if(Sacmem1) {
                Xarray[1] = *(Sacmem1++);
            }
	    }
	    Xarray[2] = Xarray[1];
	    Xarray[3] = Xarray[1];
	    yvalue = *(Sacmem2++);
	    if( ndflnumber == 2 ){
		ydvalue = *(Sacmem3++);
		Yarray[1] = yvalue - ydvalue;
		Yarray[2] = yvalue;
		Yarray[3] = yvalue + ydvalue;
	    }
	    else{
		Yarray[1] = yvalue + *(Sacmem3++);
		Yarray[2] = yvalue;
		Yarray[3] = yvalue + *(Sacmem4++);
	    }
	    pldta( xarray, yarray, 3, 1, 1, nerr );
	    if( *nerr != 0 )
		goto L_8888;
	}
	setsymbolnum( issym );


	/* - Draw grid lines, axes and such. */

	plgrid( nerr );

	/* - Home cursor and end frame if requested. */

	plhome();
	if( cmgem.lframe )
	    endframe( FALSE , nerr );
        else 
          flushbuffer( nerr );
	/* - Restore plot environment and return. */

L_8888:
	plrest();
	settextjust( "LEFT", "BOTTOM" );

	return;

} /* end of function */

