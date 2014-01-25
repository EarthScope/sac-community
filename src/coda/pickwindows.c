/** 
 * @file   pickwindows.c
 *
 * @brief  Pick Windows
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define	MWIN	5

#include "mach.h"
#include "dfm.h"
#include "hdr.h"
#include "amf.h"
#include "gem.h"
#include "gdm.h"
#include "pl.h"
#include "eam.h"
#include "gam.h"
#include "coda.h"
#include "bool.h"
#include "ucf.h"
#include "dff.h"
#include "co.h"
#include "bot.h"
#include "clf.h"
#include "gtm.h"
#include "msg.h"

void 
pickwindows(struct envelope   envelopes[MAXBANDS],
            int               nbands,
            float             C_begin,
            int              *nerr) {

	char ktemp[MCMSG+7];	/* increased array size for jdfl.  maf 970130 */
	int lany;
	char kchar;
	int lxlimj, lylimj,num, num1, num2, num2m1, jx, jy, nc;
	float atrwid, fjunk, ximnj, ximxj, 
	 xlinl1, xlinl2, xsymlc, yatrlc, yimnj, yimxj;
	int jdx, jdfl, ndfl,
	 jfr, jhdr1, jhdr2, jmark1, jmark2, 
     jwin, ndxx, ndxy, 
     nfr, nlcx, nlcy, nlen, nperfr, npmark ;
	float amplmn, amplmx, 
	 secinc, tmax, tmin, tminew, toff[MDFL], 
	 twin[MWIN][2], xloc,  
	 xtpos, yimnzs[MDFL], yimxzs[MDFL], yloc, ypdel, ypdelv, ypmns, 
	 ypmnv, ypmxs, ypmxv, ytpos;
	static int lnewxw = FALSE;

	float *const Toff = &toff[0] - 1;
	float *const Yimnzs = &yimnzs[0] - 1;
	float *const Yimxzs = &yimxzs[0] - 1;

        extern int bellON;
        int ndx1, ndx2, ndxh, j, junk;
        float *Sacmem1, *Sacmem2;
        float *Sacmem;
        char *tmp;
	/* PROCEDURE: */
	
        memset(toff, 0, sizeof(toff));
        memset(yimnzs, 0, sizeof(yimnzs));
        memset(yimxzs, 0, sizeof(yimxzs));
	atrwid = 0.0;
	*nerr = 0;
        bellON = TRUE;

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

	plsave();

	/* - Temporarily turn on cursor graphics device only. */

	cursoron();
	/* - Set up specific options for this plot. */
	cmgem.axis[LEFT].annotate  = TRUE;
	cmgem.axis[RIGHT].annotate = FALSE;
	cmgem.axis[TOP].annotate   = FALSE;
	cmgem.axis[LEFT].ticks  = TRUE;
	cmgem.axis[RIGHT].ticks = TRUE;
	cmgem.axis[TOP].ticks   = TRUE;
	cmgem.plot.ymax = 0.80;
	cmgem.plot.ymin = 0.10;
	cmgem.xdiv_number_on = TRUE;
	cmgem.xdiv_number = 7;
	cmgem.ydiv_number_on = TRUE;
	cmgem.ydiv_number = 5;
	cmgem.lxfudg = FALSE;
	kchar = 'U';

	/* - Set up y window for each subplot. */

        nfr = nbands;
        nperfr = 1;
	ypdel = (cmgem.plot.ymax - cmgem.plot.ymin)/(float)( nperfr );


	/* these need to be set to correct values eventually */
        for(j=0;j<nfr;j++) {
	  Toff[j] = 0. ;
	}


	/* - Loop on number of frames. */

	ypmns = cmgem.plot.ymin;
	ypmxs = cmgem.plot.ymax;
	jfr = 1;
L_1900:
	if( jfr > nfr )
	    goto L_7777;

        tmin = C_begin;
        tmax = envelopes[0].number_of_points*envelopes[0].GFdelta;
	jwin = 1;
	twin[jwin - 1][0] = tmin;
	twin[jwin - 1][1] = tmax;


	/* -- Begin new frame and set up some parameters. */
L_2000:

	/* put envelope and GF data into SAC memory */
	cleardfl( nerr ); 

        jdfl = 1; /* Envelope of data */
	nlen = envelopes[jfr-1].number_of_points - 5;
	cmdfm.ndfl = 3;
	crsac( jdfl, 1, nlen, &ndxh, &ndx1, &junk, nerr );
        putfil( jdfl, nerr);
	getfil( jdfl, TRUE, &nlen, &ndx1, &ndxh, nerr );
	Sacmem = cmmem.sacmem[ndx1];
	for( j = 0; j < nlen-1; j++ ){
	  *(Sacmem++) = *(envelopes[jfr-1].envelope_data+j);
	}
        nlen = nlen - 2;
	*iftype = *ixy;
	*leven = TRUE;
	*delta = envelopes[jfr-1].GFdelta;
	*begin = C_begin;
	*npts = nlen;
	*ennd = *begin + (float)( *npts - 1 )**delta;

	extrma( cmmem.sacmem[ndx1], 1, *npts, depmin, depmax, depmen );
	putfil( jdfl, nerr );

        jdfl = 2;  /* Entire GF envelope */
	nlen = envelopes[jfr-1].GFnpts - 50;
	crsac( jdfl, 1, nlen, &ndxh, &ndx2, &junk, nerr );
        putfil( jdfl, nerr);
	getfil( jdfl, TRUE, &nlen, &ndx2, &ndxh, nerr );
	Sacmem = cmmem.sacmem[ndx2];
	for( j = 0; j < nlen-1; j++ ){
	  *(Sacmem++) = (*(envelopes[jfr-1].GFenvelope+j))+envelopes[jfr-1].CodaAmp;
	}
        nlen = nlen - 2;
        *begin = envelopes[jfr-1].window_start_seconds;
	*iftype = *ixy;
	*leven = TRUE;
	*delta = envelopes[jfr-1].GFdelta;
	*npts = nlen;
	*ennd = *begin + (float)( *npts - 1 )**delta;

	extrma( cmmem.sacmem[ndx2], 1, *npts, depmin, depmax, depmen );
	putfil( jdfl, nerr );

        jdfl = 3; /* part of GF envelope used in fit */
	nlen = envelopes[jfr-1].fit_npoints;
	crsac( jdfl, 1, nlen, &ndxh, &ndx2, &junk, nerr );
        putfil( jdfl, nerr);
	getfil( jdfl, TRUE, &nlen, &ndx2, &ndxh, nerr );
	Sacmem = cmmem.sacmem[ndx2];
	for( j = 0; j < nlen-1; j++ ){
	  *(Sacmem++) = (*(envelopes[jfr-1].GFenvelope+j))+envelopes[jfr-1].CodaAmp;
	}
        nlen = nlen - 1;
        *begin = envelopes[jfr-1].window_start_seconds;
	*iftype = *ixy;
	*leven = TRUE;
	*delta = envelopes[jfr-1].GFdelta;
	*npts = nlen;
	*ennd = *begin + (float)( *npts - 1 )**delta;

	extrma( cmmem.sacmem[ndx2], 1, *npts, depmin, depmax, depmen );
	putfil( jdfl, nerr );

	cmgem.lxlim = TRUE;
	cmgem.lylim = TRUE;
	cmgem.ximn = VLARGE;
	cmgem.ximx = -VLARGE;
	cmgem.yimn = VLARGE;
	cmgem.yimx = -VLARGE;
        cmgem.iiwidth[1] = 1;
        cmgem.iiwidth[2] = 5;
        cmgem.niwidth = 3;
        cmgem.lwidth = 1;
        cmgem.liwidth = 1;
        cmgem.iicol[0] = 7;
        cmgem.iicol[1] = 3;
        cmgem.iicol[2] = 1;
        cmgem.nicol = 3;
        cmgem.lcol = 1;
        cmgem.licol = 1;
        cmgem.title.on = TRUE;
	sprintf(kmgem.ktitl, "Frequency Band= %2.2f-%2.2f Hz",envelopes[jfr-1].freq_low,envelopes[jfr-1].freq_high);
        cmgem.title.len = (strcspn(kmgem.ktitl," ") + 1);
        cmgem.ylabel.on = TRUE;
	sprintf(kmgem.kylab, "Log Amplitude");
        cmgem.ylabel.len = (strcspn(kmgem.kylab," ") + 1);
        cmgem.xlabel.on = TRUE;
	sprintf(kmgem.kxlab, "Lapse Time (seconds)");
        cmgem.xlabel.len = (strcspn(kmgem.kxlab," ") + 1);
        cmgem.igtfnt=7;

        ndfl = 3;
	for( jdfl = 1; jdfl <= ndfl; jdfl++ ){
		getfil( jdfl, TRUE, &nlen, &ndxy, &ndxx, nerr );
		nlen = nlen - 1;
		/* --- X limits first. */
		getxlm( &lxlimj, &ximnj, &ximxj );
		cmgem.ximn = fmin( cmgem.ximn, ximnj + Toff[jdfl] );
		cmgem.ximx = fmax( cmgem.ximx, ximxj + Toff[jdfl] );
		/* --- Y limits are more complicated if XLIM is already on. */
		getylm( &lylimj, &yimnj, &yimxj );
		if( lxlimj && !lylimj ){
		    if( *leven ){
			num1 = (int)( (ximnj - *begin)/ *delta ) + 1;
			if( num1 < 1 )
			    num1 = 1;
			num2 = (int)( (ximxj - *begin)/ *delta ) + 1;
			if( num2 > nlen )
			    num2 = nlen;
			if( num1 <= nlen && num2 >= 1 ){
			    num2m1 = num2 - num1 + 1;
			    ndx1 = ndxy + num1 - 1;
			    extrma( cmmem.sacmem[ndxy]+num1-1, 1, num2m1,
				    &yimnj, &yimxj, &fjunk );
			}
			else{
			    yimnj = -1.;
			    yimxj = 1.;
			}
		    }
		    else{
			jx = ndxx;
			jy = ndxy;
			yimnj = VLARGE;
			yimxj = -VLARGE;
                        Sacmem1 = cmmem.sacmem[ndxx];
                        Sacmem2 = cmmem.sacmem[ndxy];
			for( jdx = 1; jdx <= nlen; jdx++ ){
			    if( *Sacmem1 >= ximnj && *Sacmem1 <= ximxj ){
				if( *Sacmem2 < yimnj )
				    yimnj = *Sacmem2;
				if( *Sacmem2 > yimxj )
				    yimxj = *Sacmem2;
			    } /* end if( *Sacmem1 >= ximnj && ... */
			    jx = jx + 1;
			    jy = jy + 1;
                            Sacmem1++; 
			    Sacmem2++;
			} /* end for( jdx = 1; jdx <= nlen; jdx++ ) */
		    } /* end else associated with if( *leven ) */
		} /* end if( lxlimj && !lylimj ) */	
		cmgem.yimn = fmin( cmgem.yimn, yimnj );
		cmgem.yimx = fmax( cmgem.yimx, yimxj );
	} /* end for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ) */

        /* set for fixed plot window */
	if(cmgem.yimn < YVPMIN) 
	  cmgem.yimn = YVPMIN;
	if(cmgem.yimx > YVPMAX)
	  cmgem.yimx = YVPMAX;

	settexttype( kmgem.kgtqua );
	settextfont( cmgem.igtfnt );
	setlinestyle( LINE_STYLE_SOLID );
	setcolor( cmgem.iskcol );
	setlinewidth( LINE_WIDTH_THIN );

	/* - Begin new frame if requested. */

        cmgem.lframe = TRUE;
	if( cmgem.lframe ){
	    beginframe( FALSE , nerr );
	    if( *nerr != 0 )
		goto L_8888;
	    getvspace( &cmgem.view.xmin, &cmgem.view.xmax,
		       &cmgem.view.ymin, &cmgem.view.ymax );
	}
	ypmxv = ypmxs*cmgem.view.ymax;
	ypmnv = ypmns*cmgem.view.ymax;
	ypdelv = ypdel*cmgem.view.ymax;
	cmgem.chht = cmgem.tsdef;
	cmgem.chwid = cmgem.txrat*cmgem.chht;
	settextsize( cmgem.chwid, cmgem.chht );
	xtpos = cmgem.plot.xmin;
	ytpos = cmgem.view.ymax - 1.1*cmgem.chht;
	cmgem.axis[TOP].ticks = TRUE;
	cmgem.lxlim = TRUE;
        cmgem.lframe = FALSE;
	cmgem.ximn = tmin;
	cmgem.ximx = tmax;

	if( *nerr != 0 )
	    goto L_8888;
	if( cmgem.liline ){
	    xlinl2 = cmgam.xfidlc - 0.5*cmgem.chwid;
	    xlinl1 = xlinl2 - atrwid;
	}
	if( cmgem.lsym )
	    xsymlc = cmgam.xfidlc - 0.5*cmgem.chwid - 0.5*atrwid;


	for( jdfl = 1; jdfl <= ndfl; jdfl++ ){
	    getfil( jdfl, TRUE, &num, &nlcy, &nlcx, nerr );
            num = num - 1;
	    if( *nerr != 0 )
		goto L_8888;
	    if( *leven ){
		cmgem.xgen.on = TRUE;
		cmgem.xgen.delta = *delta;
		cmgem.xgen.first = *begin + Toff[jdfl];
	    }
	    else{
		cmgem.xgen.on = FALSE;
	    }
	    if( cmgam.lfidrq ){
		settextjust( "LEFT", "BOTTOM" );
		if( cmgem.lcol ) {
                  setcolor( cmgem.icol );
                } else {
                  setcolor( color_foreground_default() );
                }
		setlinewidth( LINE_WIDTH_THIN );
		move( cmgam.xfidlc, cmgam.yfidlc );
		if( cmgam.ifidtp == 4 ){
		    formhv( (char*)kmgam.kfidnm[0],9, cmgam.ifidfm, ktemp
		     ,MCMSG+7, nerr );
		    if( *nerr != 0 )
			goto L_8888;
		    nc = indexb ( ktemp , MCMSG+7 ) ;
		    text( ktemp, nc + 1 , nc );
		}
		else{
      if((tmp = string_list_get(datafiles, jdfl-1))) {
        if ( cmgam.lfinorq ) {
          sprintf ( ktemp , "%s - %d", tmp , jdfl ) ;
        } else {
          sprintf( ktemp, "%s", tmp);
        }
        nc = strlen ( ktemp ) + 1;
        text( ktemp , nc  , nc );
      }
		}
		yatrlc = cmgam.yfidlc + 0.5*cmgem.chht;
		if( cmgem.liline && cmgem.icline > 0 ){
		    setlinewidth( cmgem.iwidth );
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
		setlinewidth( cmgem.iwidth );
		cmgam.yfidlc = cmgam.yfidlc - cmgem.chht;
	    }
	    pl2d( cmmem.sacmem[nlcx], cmmem.sacmem[nlcy], num, 1, 1, nerr );
	    if( *nerr != 0 )
		goto L_8888;
	} /* end for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ) */

	if( cmgam.lfidrq ){
	    cmgem.chht = cmgem.tsdef;
	    cmgem.chwid = cmgem.txrat*cmgem.chht;
	    settextsize( cmgem.chwid, cmgem.chht );
	}
	npmark = 0;
	/* -- If MARKALL option is on, set header and plot marker limits. */

	jhdr1 = 1;
	jhdr2 = 1;
	jmark1 = 1;
	jmark2 = 1;

	setlinewidth( LINE_WIDTH_THIN );
	plgrid( nerr );
	settextjust( "LEFT", "BOTTOM" );

	/* -- Perform graphics input function. */

	xloc = cmgem.plot.xmin + 0.05*(cmgem.plot.xmax - cmgem.plot.xmin);
	yloc = ypmxv - 0.5*ypdelv;
	cmgem.chht = cmgem.tsdef;
	cmgem.chwid = cmgem.txrat*cmgem.chht;
	settextsize( cmgem.chwid, cmgem.chht );
	settextangle( TEXT_HORIZONTAL );

L_4000:
	cursor0( &xloc, &yloc, &kchar );

	upcase( &kchar, 1, &kchar, 1 );

	/* -- Go back to last x window. */
	if( kchar == 'O' ){
	    tmin = twin[jwin - 1][0];
	    tmax = twin[jwin - 1][1];
	    jwin = max( 1, jwin - 1 );
	    plhome();
	    endframe( FALSE , nerr );
	    /* npmark = 0; */
	    goto L_2000;
	}

	/* -- Kill PPK; return immediately to command level. */
	else if( kchar == 'Q' || kchar == 'K' ){
	    plhome();
	    endframe( FALSE , nerr );
	    goto L_7777;
	}

	/* -- Go to next subplot. */
	else if( kchar == 'N' ){
	    plhome();
	    endframe( FALSE , nerr );
	    /* npmark = 0; */
	    jfr = jfr + 1;
	    goto L_1900;
	}

	/* -- Go back to last subplot. */
	else if( kchar == 'B' ){
	    plhome();
	    endframe( FALSE , nerr );
	    /* npmark = 0; */
	    jfr = jfr - 1;
	    goto L_1900;
	}

	/* - Rest of cursor responses need a valid cursor position. */

	if( ((xloc < cmgem.plot.xmin || xloc > cmgem.plot.xmax) || yloc < ypmnv) || 
	 yloc > ypmxv ){
	    setmsg( "OUTPUT", 1502 );
	    apfmsg( xloc );
	    apfmsg( yloc );
	    pltmsg( &xtpos, &ytpos );
	    ytpos = ytpos - cmgem.chht;
	    goto L_4000;
	}

	/* - Determine time at cursor location.
	 *   (Correct for any differences between the zero times.) */
	if( cmgem.ixint == AXIS_LINEAR ){
	    secinc = (xloc - cmgem.xmpip2)/cmgem.xmpip1 - Toff[jfr];
	}
	else{
	    secinc = pow(10.,(xloc - cmgem.xmpip2)/cmgem.xmpip1);
	}

	/* - Determine amplitude corresponding to cursor position. */
	amplmn = Yimnzs[jfr];
	amplmx = Yimxzs[jfr];
	cmeam.pkampl = amplmx - (ypmxv - ypdelv - yloc)*(amplmx - amplmn)/ypdelv;

	/* - Perform action corresponding to returned non-integer character. */

	/* -- Define end of new x window. */
	if( lnewxw ){
	    jwin = min( MWIN, jwin + 1 );
	    twin[jwin - 1][0] = tmin;
	    twin[jwin - 1][1] = tmax;
	    tmin = tminew;
	    tmax = secinc;
	    lnewxw = FALSE;
	    plhome();
	    endframe( FALSE , nerr );
	    goto L_2000;
	}
	/* -- Define start of new x window. */
	else if( kchar == 'X' ){
	    lnewxw = TRUE;
	    tminew = secinc;
	    markvert( jmark1, jmark2, &xloc, ypmxv, ypdelv, "X",2, 0 );
	}

	/* -- Define first arrival time. */
	else if( kchar == 'A' ){
	    markhdr( jdfl, jhdr1, jhdr2, "A", secinc, kmeam.kpkid );
	    markvert( jmark1, jmark2, &xloc, ypmxv, ypdelv, "A" ,2, npmark );
	    npmark = npmark + 1;
            fprintf(stderr, "Window Begin = %f \n",secinc);
	    envelopes[jfr-1].window_start_seconds = secinc;
	}

	else if( kchar == 'D' ){
            fprintf(stderr, "This band will not be used \n");
	    envelopes[jfr-1].fit_window_picked = -1;
	}

	/* -- Define coda length (fini). */
	else if( kchar == 'F' ){
	    strcpy( kmeam.kpkid, "FINI    " );
	    markhdr( jdfl, jhdr1, jhdr2, kmeam.kpkid, secinc, kmhdr.kundef );
	    markvert( jmark1, jmark2, &xloc, ypmxv, ypdelv, kmeam.kpkid ,9, 0 );
            fprintf(stderr, "Window End = %f \n",secinc);
	    envelopes[jfr-1].fit_npoints = (int)((secinc-envelopes[jfr-1].window_start_seconds)/envelopes[jfr-1].GFdelta);
	    envelopes[jfr-1].fit_window_picked = 1;
	    if( cmeam.lhpfop ){
		cmeam.lfini = TRUE;
	    }
	}

	/* -- Bad cursor response handled here. */
	else{
	    setmsg( "OUTPUT", 1503 );
	    apcmsg( &kchar,1 );
	    pltmsg( &xtpos, &ytpos );
	    ytpos = ytpos - cmgem.chht;
	}

	/* -- Loop back for another cursor response. */

	goto L_4000;

	/* - Restore plot environment. */

L_7777:
	plrest();

	/* - Return to normal graphics device mode. */

	cursoroff();

	/* - Delete any extra locations from blackboard if necessary. */


L_8888:

	return;

} /* end of function */


