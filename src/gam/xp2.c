#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "gem.h"
#include "gdm.h"
#include "gam.h"
#include "dfm.h"
#include "hdr.h"
#include "amf.h"
#include "bool.h"

#include "SacHeader.h"

#include "gtm.h"
#include "pl.h"
#include "bot.h"
#include "ucf.h"
#include "msg.h"
#include "clf.h"
#include "cpf.h"
#include "co.h"
#include "dff.h"

void xp2(int *nerr)
{
	char ktemp[MCMSG+7];	/* increased array size for jdfl.  maf 970130 */
	int lany, lfirst, lxlimj, lxlims, lylimj,
	     lprint = FALSE , ltry = FALSE ;
	int jdx, jdfl,
	 notused, nrdttm[6], num1, 
	 num2, num2m1;
	float atrwid, fjunk, ximnj, ximxj, xjunk, 
    yimnj, yimxj;
  char *tmp;

  float *toff = NULL;
  
  sac *s;
  textbox *tbox;

	/*=====================================================================
	 * PURPOSE:  To execute the action command PLOT2.
	 *           This command makes a multi-trace, single window plot.
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
	 *             lsym, isym, iskpen, iskcol, tsdef, chwid, chht, icline,
	 *             lwidth, iwidth, iskwidth
	 *    gam:     kgddef, lp2abs, lfidrq, ifidtp, kfidnm, ifidlc, fidbdr,
	 *             tsfid
	 *    dfm:     ndfl
	 *    hdr:     nzdttm, leven, delta, begin
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    gem:     lxlim, ximn, ximx, lylim, yimn, yimx
	 *    gam:     lp2abs, xfidlc, yfidlc
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    saclib:  lcmore, lclog2, cfmt, cresp, vflist, ddttm, plrest,
	 *             getstatus, begindevices, plsave, getfil, ldttm, setcolor,
	 *             getxlm, getylm, beginframe, plmap, settextsize, copyi
	 *             pltext, symbol, pldta, plgrid, plhome, endframe,
	 *             setlinewidth
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    nrdttm:  six element array containing "reference" date/time
	 *             used to calculate time offsets in absolute mode. [i]
	 *    toff(j): Contains the time offset for file "j". [f]
	 *             Used in both absolute and relative modes.* MODULE/LEVEL: 
	 *    atrwid:  Width of linestyle/symbol attribute part of fileid. [f]
	 *    xlinl1:  X beginning of linestyle attribute display. [f]
	 *    xlinl2:  X ending of linestyle attribute display. [f]
	 *    xsymlc:  X location of symbol attribute display. [f]
	 *    yatrlc:  Y location of linestyle/symbol attribute display. [f]
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    100409:  If spectral file and Xlim not fixed, make it delta (jas/vt)
	 *    100407:  Fix bug in spectral file Y axis limits.  (GRH/UB)
	 *    980128:  Fixed bug plotting fileid. maf
	 *    970908:  Changed response to ddttm().  maf
	 *    970205:  Moved settextjust() to for loop to fix a bug.  maf
	 *    970130:  Allows printing of file numbers.
	 *    920528:  Added setlinewidth call, lwidth & iwidth variables.
	 *    910608:  Added call to zgetgd when no graphics device specified.
	 *             Changed begindevice to begindevices. (wct)
	 *    910301:  Changed iline to icline.
	 *    890227:  Fileid can now be the first header item from the LIST
	 *             option of the FILEID command.
	 *    861112:  Deleted calls to ZPEN.
	 *    860213:  Deleted LNICE logic.
	 *    860114:  Fixed bug in fileid involving linestyle in fileid.
	 *    850814:  Added linestyle attribute display to fileid.
	 *    850516:  Fixed bug in fileid placement when symbol plotting is on.
	 *    850506:  Text and display attributes now being set correctly.
	 *    831006:  Changed to FILEID options instead of local ones.
	 *             Added ABSOLUTE/RELATIVE option.
	 *    820817:  Changed to newest set of parsing and checking functions.
	 *    820317:  Fixed bug with setting pen numbers for table.
	 *    811228:  Added symbol display to legend.
	 *    811030:  Fixed bug when framing option was off.
	 *    810708:  Added SOLID option which plots first trace as a solid
	 *             line and then reverts to current line style.
	 *    810120:  Changed to output message retrieval from disk.
	 *    791206:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  861112
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;
    tbox = NULL;
	/* PARSING PHASE: */

	/* - Loop on each token in command: */

    toff = xarray_new_with_length('f', saclen()+1);
    memset(toff, 0.0, sizeof(float) * saclen());
	while ( lcmore( nerr ) ){

	    /* -- "RELATIVE/ABSOLUTE":  change method of displaying time
					on x axis. */
	    if( lclog2( "A$",3, "R$",3, &cmgam.lp2abs ) )
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

	/* CHECKING PHASE: */

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

	/* - Calculate x and y axis limits. */

	getxlm( &lxlims, &xjunk, &xjunk );
	cmgem.lxlim = TRUE;
	cmgem.lylim = TRUE;
	cmgem.ximn = VLARGE;
	cmgem.ximx = -VLARGE;
	cmgem.yimn = VLARGE;
	cmgem.yimx = -VLARGE;

	/* -- Absolute mode. */
	if( cmgam.lp2abs ){
	    lfirst = TRUE;
	    for( jdfl = 1; jdfl <= saclen(); jdfl++ ){
        if(!(s = sacget(jdfl-1, TRUE, nerr))) {
          goto L_8888;
        }
        //getfil( jdfl, TRUE, &nlen, &ndxy, &ndxx, nerr );

		/* --- LDTTM function returns .TRUE. if 
			     date-time stamp is defined. */
		if( ldttm( &s->h->nzyear ) ){
		    if( lfirst ){
			lfirst = FALSE;
			copyi( &s->h->nzyear, nrdttm, 6 );
			toff[jdfl-1] = 0.;
		    }
		    else if ( s->h->iftype == IRLIM || s->h->iftype == IAMPH ) {
			/* if it is frequency information, plot relative */
          toff[jdfl-1] = 0. ;
		    }
		    else{
			/* --- DDTTM function computes difference in two
				     date-time stamps. */
			ddttm( &s->h->nzyear, nrdttm, &toff[jdfl-1] );
			/* if difference is more than twodays, plot relative */
			if ( fabs ( toff[jdfl-1] ) > TWODAYS )
			    toff[jdfl-1] = 0. ;
		    }
		} /* end if( ldttm( nzdttm ) ) */
		else{
		    toff[jdfl-1] = 0.;
		}
		/* --- X limits first. */
		getxlm( &lxlimj, &ximnj, &ximxj );
		if(!lxlims && ( s->h->iftype == IRLIM || s->h->iftype == IAMPH )) 
		  ximnj = s->h->delta;
		cmgem.ximn = fmin( cmgem.ximn, ximnj + toff[jdfl-1] ); 
		cmgem.ximx = fmax( cmgem.ximx, ximxj + toff[jdfl-1] );
		/* --- Y limits are more complicated if XLIM is already on. */
		/* If spectral file, use first component range (AM or RL). */
		getylm( &lylimj, &yimnj, &yimxj );
		if(!lylimj && !lxlims && ( s->h->iftype == IRLIM || s->h->iftype == IAMPH )) 
		  extrma(&s->y[1],1,s->h->npts-11,&yimnj,&yimxj,&fjunk); 
		if( lxlims && !lylimj ){
		    if( s->h->leven ){
			num1 = (int)( (ximnj - s->h->b)/ s->h->delta ) + 1;
			if( num1 < 1 )
			    num1 = 1;
			num2 = (int)( (ximxj - s->h->b)/ s->h->delta ) + 1;
			if( num2 > s->h->npts )
			    num2 = s->h->npts;
			if( num1 <= s->h->npts && num2 >= 1 ){
			    num2m1 = num2 - num1 + 1;
			    extrma( &s->y[num1-1], 1, num2m1,
				    &yimnj, &yimxj, &fjunk );
			}
			else{
			    yimnj = -1.;
			    yimxj = 1.;
			}
		    }
		    else{
			yimnj = VLARGE;
			yimxj = -VLARGE;
			for( jdx = 1; jdx <= s->h->npts; jdx++ ){
			    if( s->x[jdx-1] >= ximnj && s->x[jdx-1] <= ximxj ){
				if( s->y[jdx-1] < yimnj )
				    yimnj = s->y[jdx-1];
				if( s->y[jdx-1] > yimxj )
          yimxj = s->y[jdx-1];
			    } /* end if( *Sacmem1 >= ximnj && ... */
			} /* end for( jdx = 1; jdx <= nlen; jdx++ ) */
		    } /* end else associated with if( s->h->leven ) */
		} /* end if( lxlims && !lylimj ) */
		cmgem.yimn = fmin( cmgem.yimn, yimnj );
		cmgem.yimx = fmax( cmgem.yimx, yimxj );
	    } /* end for( jdfl = 1; jdfl <= saclen(); jdfl++ ) */
	} /* end if( cmgam.lp2abs ) */

	/* -- Relative mode. */
	else{
	    cmgem.ximn = 0.;
	    cmgem.ximx = -VLARGE;
	    for( jdfl = 1; jdfl <= saclen(); jdfl++ ){
        if(!(s = sacget(jdfl-1, FALSE, nerr))) {
          goto L_8888;
        }
        //getfil( jdfl, FALSE, &nlen, &ndx1, &ndx2, nerr );

		getxlm( &lxlimj, &ximnj, &ximxj );
		cmgem.ximx = fmax( cmgem.ximx, ximxj - ximnj );
		toff[jdfl-1] = -ximnj;
		getylm( &lylimj, &yimnj, &yimxj );
		if(!lylimj && ( s->h->iftype == IRLIM || s->h->iftype == IAMPH ))
		  extrma(&s->y[1],1,s->h->npts-1,&yimnj,&yimxj,&fjunk);
		cmgem.yimn = fmin( cmgem.yimn, yimnj );
		cmgem.yimx = fmax( cmgem.yimx, yimxj );
	    } /* end for ( jdfl = 1; jdfl <= saclen(); jdfl++ ) */
	} /* end else */

	/* - Set background and skeleton attributes. */

	settexttype( kmgem.kgtqua );
	settextfont( cmgem.igtfnt );
	setlinestyle( LINE_STYLE_SOLID );
	setcolor( cmgem.iskcol );
	setlinewidth( LINE_WIDTH_THIN );

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

	plmap( NULL, NULL, 1, 1, 1, nerr );
	if( *nerr != 0 )
	    goto L_8888;

	/* - Determine location for id. */

	if( cmgam.lfidrq ){
        tbox        = textbox_new( saclen() );
        if(!tbox) {
            goto L_8888;
        }
	    cmgem.chht = cmgam.tsfid;
	    cmgem.chwid = cmgem.txrat*cmgem.chht;
	    settextsize( cmgem.chwid, cmgem.chht );
	    cmgam.fidbdr = cmgem.chht;
	    for( jdfl = 1; jdfl <= saclen(); jdfl++ ){
      if(!(s = sacget(jdfl-1, FALSE, nerr))) {
        goto L_8888;
      }
      //getfil( jdfl, FALSE, &notused, &notused, &notused, nerr );
		if( cmgam.ifidtp == 4 ){
		    formhv( (char*)kmgam.kfidnm[0],9, cmgam.ifidfm, ktemp
			 ,MCMSG+7, nerr );
		    if( *nerr != 0 )
			goto L_8888;
                    tbox->text[jdfl-1] = fstrdup(ktemp, MCMSG+7);
		} /* end if( cmgam.ifidtp == 4 ) */
		else{
         tmp = s->m->filename;
         if(tmp) {
                if ( cmgam.lfinorq ){
                    sprintf ( ktemp , "%s - %d" , tmp , jdfl ) ;
                } else {
                    sprintf( ktemp, "%s", tmp);
                }
                tbox->text[jdfl-1] = fstrdup(ktemp, MCMSG + 7);
		     }
		} /* end else associated with if( cmgam.lp2abs ) */
	    } /* end for ( jdfl = 1; jdfl <= saclen(); jdfl++ ) */

	    if( cmgem.liline )
		atrwid = 2.5*cmgem.chwid;
	    else if( cmgem.lsym )
		atrwid = cmgem.chwid;
	    else
		atrwid = 0.;

        atrwid = 0;

	    if( cmgam.ifidlc == cmgam.iur ){
                tbox->x = cmgem.uplot.xmax - cmgam.fidbdr;
                tbox->y = cmgem.uplot.ymax - cmgam.fidbdr;
                tbox->location = TEXT_BOX_UPPER | TEXT_BOX_RIGHT;
	    }
	    else if( cmgam.ifidlc == cmgam.iul ){
                tbox->x = cmgem.uplot.xmin + cmgam.fidbdr + atrwid;
                tbox->y = cmgem.uplot.ymax - cmgam.fidbdr;
                tbox->location = TEXT_BOX_UPPER | TEXT_BOX_LEFT;
	    }
	    else if( cmgam.ifidlc == cmgam.ilr ){
                tbox->x = cmgem.uplot.xmax - cmgam.fidbdr;
                tbox->y = cmgem.uplot.ymin + cmgam.fidbdr;
                tbox->location = TEXT_BOX_LOWER | TEXT_BOX_RIGHT;
	    }
	    else if( cmgam.ifidlc == cmgam.ill ){
                tbox->x = cmgem.uplot.xmin + cmgam.fidbdr + atrwid;
                tbox->y = cmgem.uplot.ymin + cmgam.fidbdr;
                tbox->location = TEXT_BOX_LOWER | TEXT_BOX_LEFT;
	    }
	    else{
                tbox->x = cmgem.uplot.xmin + cmgam.fidbdr + atrwid;
                tbox->y = cmgem.uplot.ymax - cmgam.fidbdr;
                tbox->location = TEXT_BOX_UPPER | TEXT_BOX_LEFT;
	    }
	}

    if(tbox) {
        if( cmgem.lline && cmgem.liline ){
	    /* xlinl2 = cmgam.xfidlc - 0.5*cmgem.chwid; */
	    /* xlinl1 = xlinl2 - atrwid; */
            tbox->use_style = TRUE;
        }
        if( cmgem.lsym) {
            /* xsymlc = cmgam.xfidlc - 0.5*cmgem.chwid - 0.5*atrwid; */
            tbox->use_symbol = TRUE;
        }
    }

	/* - Loop to plot each trace in DFL on same plot window. */

	for( jdfl = 1; jdfl <= saclen(); jdfl++ ){
    if(!(s = sacget(jdfl-1, TRUE, nerr))) {
      goto L_8888;
    }
    //getfil( jdfl, TRUE, &num, &nlcy, &nlcx, nerr );

    if( s->h->leven ){
		cmgem.xgen.on = TRUE;
		cmgem.xgen.delta = s->h->delta;
		cmgem.xgen.first = s->h->b + toff[jdfl-1];
	    }
	    else{
		cmgem.xgen.on = FALSE;
	    }
	    if( cmgam.lfidrq ){
            settextjust( "LEFT", "BOTTOM" );
            if(tbox) {
                if( cmgem.lcol ) {
                    tbox->color[jdfl-1] = cmgem.icol;
                } else {
                    tbox->color[jdfl-1] = color_foreground_default();
                }
                if( cmgem.lline && cmgem.liline && cmgem.icline > 0 ){
                    tbox->style[jdfl-1] = cmgem.icline;
                    tbox->width[jdfl-1] = cmgem.iwidth;
                }
                
                if( cmgem.lsym && cmgem.isym > 0 ){
                    tbox->symbol[jdfl-1] = cmgem.isym;
                }
            }
	    }
	    pldta( s->x, s->y, s->h->npts, 1, 1, nerr );
	    if( *nerr != 0 )
		goto L_8888;
	} /* end for( jdfl = 1; jdfl <= saclen(); jdfl++ ) */
        
	if( cmgam.lfidrq && tbox ){
            textbox_show( tbox );
            textbox_free( tbox );
            tbox = NULL;

	    cmgem.chht = cmgem.tsdef;
	    cmgem.chwid = cmgem.txrat*cmgem.chht;
	    settextsize( cmgem.chwid, cmgem.chht );
	}

	/* - Draw grid lines, axes and such. */

	setlinewidth( LINE_WIDTH_THIN );
	if( !cmgam.lp2abs )
	    pltext( "RELATIVE MODE",14, cmgam.xfidlc, cmgam.yfidlc );
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
  xarray_free(toff);
	return;

} /* end of function */

