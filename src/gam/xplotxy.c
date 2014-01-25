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
#include "co.h"


#include "gtm.h"
#include "pl.h"
#include "bot.h"
#include "msg.h"
#include "clf.h"
#include "cpf.h"
#include "dff.h"

void xplotxy(int *nerr)
{
	char kfile[MCPFN+1], ktemp[MCMSG+1];
	int lany, lchange, lxlims, lylimj;
	int idflnumber[MDFL], jdfl, jdflnumber,
	 nc, ncfile, ndflnumber, nlcx, nlcy, notused, num, numx, numy;
	float atrwid, slen, slenm, slenvs, vportratio, xlinl1, 
	 xlinl2, xrange, xsymlc, yatrlc, yimnj, yimxj, yrange;

	int *const Idflnumber = &idflnumber[0] - 1;
    char *tmp;

	/*=====================================================================
	 * PURPOSE:  To execute the action command PLOTXY.
	 *           This command makes a multi-trace, single window plot.
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
         *    100420:  Fixed error in xlog and ylog limits (jas/vt and bs(uri)
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
    ndflnumber = 0;
    atrwid = 0;
	/* PARSING PHASE: */

	/* - Loop on each token in command: */

	jdflnumber = 0;
	lchange = FALSE;
L_1000:
	if( lcmore( nerr ) ){

		/* -- ASPECT ON|OFF:  maintain aspect ratio of data or not. */
		if( lklog( "ASPECT$",8, &cmxyz.laspect ) ){

			/* -- integer:  the index number of data file in data file list. */
			}
		else if( lcirc( 1, cmdfm.ndfl, &jdfl ) ){
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

	/* - Set x axis limits based on data file unless limits are already set. */

	getxlm( &lxlims, &cmgem.ximn, &cmgem.ximx );
	if( !lxlims ){
		getfil( Idflnumber[1], FALSE, &notused, &notused, &notused, 
		 nerr );
		if( *nerr != 0 )
			goto L_8888;
		xrange = *depmax - *depmin;
		cmgem.lxlim = TRUE;
            if(cmgem.ixint == AXIS_LINEAR ) {
		cmgem.ximn = *depmin - cmgem.xfudg*xrange;
		cmgem.ximx = *depmax + cmgem.xfudg*xrange;
            } else if(cmgem.ixint == AXIS_LOG ) {
                xrange     = fmax(xrange, VSMALL);
                cmgem.ximn = fmax(*depmin, VSMALL);
                cmgem.ximx = fmax(*depmax, VSMALL);
                xrange = log10(xrange);
                cmgem.ximn = pow(10, log10(cmgem.ximn) - cmgem.xfudg*xrange);
                cmgem.ximx = pow(10, log10(cmgem.ximx) + cmgem.xfudg*xrange);
            }            
		}
	else{
		cmgem.lxlim = TRUE;
		}

	/* - Set y axis limits. */

	cmgem.lylim = TRUE;
	cmgem.yimn = VLARGE;
	cmgem.yimx = -VLARGE;
	for( jdflnumber = 2; jdflnumber <= ndflnumber; jdflnumber++ ){
		jdfl = Idflnumber[jdflnumber];
		getfil( jdfl, FALSE, &notused, &notused, &notused, nerr );
		if( *nerr != 0 )
			goto L_8888;
		getylm( &lylimj, &yimnj, &yimxj );
		cmgem.yimn = fmin( cmgem.yimn, yimnj );
		cmgem.yimx = fmax( cmgem.yimx, yimxj );
		}
	yrange = cmgem.yimx - cmgem.yimn;
	cmgem.lylim = TRUE;
    if(cmgem.iyint == AXIS_LINEAR ) {
	cmgem.yimn = cmgem.yimn - cmgem.yfudg*yrange;
	cmgem.yimx = cmgem.yimx + cmgem.yfudg*yrange;

    } else if(cmgem.iyint == AXIS_LOG ) {
      yrange     = fmax(yrange, VSMALL);
      cmgem.yimn = fmax(cmgem.yimn, VSMALL);
      cmgem.yimx = fmax(cmgem.yimx, VSMALL);
      yrange = log10(yrange);
      cmgem.yimn = pow(10, log10(cmgem.yimn) - cmgem.yfudg*yrange);
      cmgem.yimx = pow(10, log10(cmgem.yimx) + cmgem.yfudg*yrange);
    }
	/* - Set background and skeleton attributes. */

	settexttype( kmgem.kgtqua );
	settextfont( cmgem.igtfnt );
	setlinestyle( LINE_STYLE_SOLID );
	setcolor( cmgem.iskcol );

	/* -- Set viewport using different aspect ratio if ASPECT ON. */
	if( cmxyz.laspect ){
		vportratio = fabs( (cmgem.yimx - cmgem.yimn)/(cmgem.ximx - 
		 cmgem.ximn) );
		setvspacetype( FALSE, vportratio );
		getvport( &cmgem.uplot.xmin, &cmgem.uplot.xmax, &cmgem.uplot.ymin, &cmgem.uplot.ymax );
		}
	else{
		setvspacetype( TRUE, 1.0 );
		getvport( &cmgem.uplot.xmin, &cmgem.uplot.xmax, &cmgem.uplot.ymin, &cmgem.uplot.ymax );
		}

	/* - Begin new frame if requested. */

	if( cmgem.lframe ){
		beginframe( FALSE , nerr );
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

	/* - Determine location for id. */

	if( cmgam.lfidrq ){
		cmgem.chht = cmgam.tsfid;
		cmgem.chwid = cmgem.txrat*cmgem.chht;
		settextsize( cmgem.chwid, cmgem.chht );
		getstringsize( "vs. ", 4, &slenvs );
		cmgam.fidbdr = cmgem.chht;
		slenm = 0.;
		for( jdflnumber = 1; jdflnumber <= ndflnumber; jdflnumber++ ){
			jdfl = Idflnumber[jdflnumber];
			if( cmgam.ifidtp == 4 ){
				getfil( jdfl, FALSE, &notused, &notused, &notused, 
				 nerr );
				if( *nerr != 0 )
					goto L_8888;
				formhv( (char*)kmgam.kfidnm[0],9, cmgam.ifidfm, ktemp
				 ,MCMSG+1, nerr );
				if( *nerr != 0 )
					goto L_8888;
				nc = indexb( ktemp,MCMSG+1 );
				getstringsize( ktemp, nc, &slen );
				}
			else{
        tmp = string_list_get(datafiles, jdfl-1);
        getstringsize( tmp, strlen(tmp), &slen );
				}
			slenm = fmax( slenm, slen );
			}
		/*        if(liline)then
		 *          atrwid=2.5*chwid
		 *        elseif(lsym)then
		 *          atrwid=chwid
		 *        else
		 *          atrwid=0.
		 *        endif */
		atrwid = slenvs;
		if( cmgam.ifidlc == cmgam.iur ){
			cmgam.xfidlc = cmgem.uplot.xmax - cmgam.fidbdr - slenm;
			cmgam.yfidlc = cmgem.uplot.ymax - cmgam.fidbdr - cmgem.chht;
			}
		else if( cmgam.ifidlc == cmgam.iul ){
			cmgam.xfidlc = cmgem.uplot.xmin + cmgam.fidbdr + atrwid;
			cmgam.yfidlc = cmgem.uplot.ymax - cmgam.fidbdr - cmgem.chht;
			}
		else if( cmgam.ifidlc == cmgam.ilr ){
			cmgam.xfidlc = cmgem.uplot.xmax - cmgam.fidbdr - slenm;
			cmgam.yfidlc = cmgem.uplot.ymin + cmgam.fidbdr + (float)( ndflnumber - 
			 1 )*cmgem.chht;
			}
		else if( cmgam.ifidlc == cmgam.ill ){
			cmgam.xfidlc = cmgem.uplot.xmin + cmgam.fidbdr + atrwid;
			cmgam.yfidlc = cmgem.uplot.ymin + cmgam.fidbdr + (float)( ndflnumber - 
			 1 )*cmgem.chht;
			}
		else{
			cmgam.xfidlc = cmgem.uplot.xmin + cmgam.fidbdr + atrwid;
			cmgam.yfidlc = cmgem.uplot.ymax - cmgam.fidbdr - cmgem.chht;
			}
		settextjust( "LEFT", "BOTTOM" );
		}
	if( cmgem.liline ){
		xlinl2 = cmgam.xfidlc - 0.5*cmgem.chwid;
		xlinl1 = xlinl2 - atrwid;
		}
	if( cmgem.lsym )
		xsymlc = cmgam.xfidlc - 0.5*cmgem.chwid - 0.5*atrwid;

	/* - Loop to plot each requested file vs designated x file. */

	getfil( Idflnumber[1], TRUE, &numx, &nlcx, &notused, nerr );
	if( *nerr != 0 )
		goto L_8888;
	cmgem.xgen.on = FALSE;
	for( jdflnumber = 2; jdflnumber <= ndflnumber; jdflnumber++ ){
		jdfl = Idflnumber[jdflnumber];
		getfil( jdfl, TRUE, &numy, &nlcy, &notused, nerr );
		if( *nerr != 0 )
			goto L_8888;
		num = min( numx, numy );
		if( cmgam.lfidrq ){
                  if( cmgem.lcol ) {
                    setcolor( cmgem.icol );
                  } else {
                    setcolor( color_foreground_default() );
                  }
			move( cmgam.xfidlc, cmgam.yfidlc );
			if( cmgam.ifidtp == 4 ){
				formhv( (char*)kmgam.kfidnm[0],9, cmgam.ifidfm, ktemp
				 ,MCMSG+1, nerr );
				if( *nerr != 0 )
					goto L_8888;
				nc = indexb( ktemp,MCMSG+1 );
				text( ktemp,MCMSG+1, nc );
				}
			else{
        tmp = string_list_get(datafiles, jdfl-1);
        text( tmp, strlen(tmp), nc );
      }
			yatrlc = cmgam.yfidlc + 0.5*cmgem.chht;
			if( cmgem.liline && cmgem.icline > 0 ){
				setlinestyle( cmgem.icline );
				move( xlinl1, yatrlc );
				draw( xlinl2, yatrlc );
				setlinestyle( LINE_STYLE_SOLID );
				}
			if( cmgem.lsym && cmgem.isym > 0 ){
                                setlinewidth( cmgem.isymwidth );
				symbol( (float*)&xsymlc, (float*)&yatrlc, 1, TRUE );
                                setlinewidth( cmgem.iwidth );
			      }
			if( cmgem.lcol ) {
                          setcolor( cmgem.iskcol );
                        } else {
                          setcolor( color_foreground_default() );
                        }
			cmgam.yfidlc = cmgam.yfidlc - cmgem.chht;
			}

		pldta( cmmem.sacmem[nlcx], cmmem.sacmem[nlcy], num, 1, 1, nerr );
		if( *nerr != 0 )
			goto L_8888;
		}

	/* - Add last line to fileid and reset text size to default value.
	 *   Last line specifies which data file was used for the x axis. */

	if( cmgam.lfidrq ){
		move( cmgam.xfidlc, cmgam.yfidlc );
		jdfl = Idflnumber[1];
		if( cmgam.ifidtp == 4 ){
			getfil( jdfl, FALSE, &notused, &notused, &notused, nerr );
			if( *nerr != 0 )
				goto L_8888;
			formhv( (char*)kmgam.kfidnm[0],9, cmgam.ifidfm, ktemp,MCMSG+1, 
			 nerr );
			if( *nerr != 0 )
				goto L_8888;
			nc = indexb( ktemp,MCMSG+1 );
			text( ktemp,MCMSG+1, nc );
			}
		else{
      tmp = string_list_get(datafiles, jdfl-1);
      text( tmp, strlen(tmp) , nc );
    }
		getstringsize( "vs. ", 4, &slenvs );
		move( cmgam.xfidlc - slenvs, cmgam.yfidlc );
		text( "vs. ",5, 4 );
		cmgam.yfidlc = cmgam.yfidlc - cmgem.chht;
		cmgem.chht = cmgem.tsdef;
		cmgem.chwid = cmgem.txrat*cmgem.chht;
		settextsize( cmgem.chwid, cmgem.chht );
		}

	/* - Draw grid lines, axes and such. */

	plgrid( nerr );

	/* - Home cursor and end frame if requested. */

	plhome();
	if( cmgem.lframe )
		endframe( FALSE , nerr );

	/* - Restore plot environment and return. */

L_8888:
	plrest();
	settextjust( "LEFT", "BOTTOM" );

	return;

} /* end of function */

