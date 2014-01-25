
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "mach.h"
#include "xyz.h"
#include "gdm.h"
#include "gam.h"
#include "gem.h"
#include "co.h"
#include "hdr.h"
#include "amf.h"
#include "bool.h"
#include "msg.h"
#include "gtm.h"
#include "pl.h"
#include "ucf.h"
#include "dff.h"

void
show_colorbar(float  x,
              float  y,
              float  width,
              float  height,
              float  zmin, 
              float  zmax, 
              int    lbinary, 
              int   *nerr) {
  unsigned int w, h;
  float *cbar;
  int i, j;

  /* Set size of the input image */
  w = 20;
  h = cmgdm.npscimage;
  
  /* Generate the "data" for the colorbar */
  cbar = (float *) malloc(sizeof(float) * w * h);
  for(i = 0; i < (int)h; i++) {
      for(j = 0; j < (int)w; j++) {
          cbar[j + i * w] = zmin + (i * (zmax - zmin));
      }
  }
  
  show_image(cbar, w, h,
             0, (float) w, 0, (float) h,
             x,
             y,
             width,
             height,
             cmgdm.npscimage, cmgdm.nctsize, 7,
             lbinary,
             nerr);

  free(cbar);

  label_cbar(x, y, width, height, zmin, zmax, nerr);

}

void 
specplot(float *specdata,
         int nx,
         int ny,
         float xmin,
         float xmax,
         float ymin,
         float ymax,
         float ywmin,
         float ywmax,
         char *imagetype,
         int lbinary,
         int lcbar,
         int lprint,
         int *nerr)
{
	char kptext[MCMSG+1];
	int lany, lbotaxsave, lbottcsave, lframesave, ltitlsave, 
	 ltoptcsave, lwait, lxgrdsave, lxlabsave, lxlims, lylabsave;
	int n1dttm[6], nlcx, nlcy, num;
    int nystart, nypoints;
	float tmax, tmin, toff[MDFL], ypdel, ypmxsave, ypmnsave, xpmxsave, xpmnsave;
        float vspaceratio;

        float width, height;
        float *wdata;
        int wdata_alloc;

	static int lrel = FALSE;

	float *const Toff = &toff[0] - 1;


	/*=====================================================================
	 * PURPOSE:  To produce a spectrogram plot.  This routine plots the
	 *         input trace in a small window above a spectrogram, which
         *         is either a color image plot, a greyscale plot or a contour
         *         plot.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error return flag.  Set to 0 if no error occurred.
	 *             Potential error numbers:  1001, 1504.
	 *=====================================================================
	 * MODULE/LEVEL:  gam/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *    dfm:     ndfl, sacmem
	 *    hdr:     begin, ennd, delta
	 *    gem:     lbotax, lbottc, ltopax, ltoptc, lxlab, lylab, ltitl,
	 *             lxgrd, ypmn, ypmx, chht, tsdef
	 *    gam:     kgddef
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *=====================================================================
	 * MODIFICATION HISTORY:
         *    970130:  Added arguments to dispid() not to plot file number. maf
         * 940825:  Original version, adapted from xp1.
         *
	 *=====================================================================
	 * DOCUMENTED/REVIEWED: 
	 *===================================================================== */
	/* PROCEDURE: */
	/* Errors before plsave have to avoid going to execute plrest. */
	*nerr = 0;

	/* - If no graphics device is open, try to open the default device. */

	getstatus( "ANY", &lany );
	if( !lany ){
		zgetgd( kmgam.kgddef,9 );
		begindevices( kmgam.kgddef,9, 1, nerr );
		if( *nerr != 0 )
			goto L_8888;
		}

        if( cmgam.cmap == MDEFAULT){
          setpsctable(nerr);
          if( *nerr != 0 ) goto L_8888;
	}

        if( (cmgam.cmap == MCOLOR) && (strcmp(imagetype,"grey") == 0) ){
          changectable(cmgdm.nctsize+1,MGREY);
          cmgam.cmap = MGREY;
        }
        else if((cmgam.cmap == MGREY) && (strcmp(imagetype,"color") == 0)){
          changectable(cmgdm.nctsize+1,MCOLOR);
          cmgam.cmap = MCOLOR;
        }

	/* EXECUTION PHASE: */

	/* - Save current plot and x limit attributes.
	 * - Error after plsave have to go to execute plrest. */

	plsave();


	/* - Set up specific options that apply only to this plot. */

	lbotaxsave = cmgem.axis[BOTTOM].annotate;
	lbottcsave = cmgem.axis[BOTTOM].ticks;
	ltoptcsave = cmgem.axis[TOP].ticks;
	cmgem.axis[BOTTOM].ticks    = FALSE;
	cmgem.axis[BOTTOM].annotate = FALSE;

	lxlabsave = cmgem.xlabel.on;
	lylabsave = cmgem.ylabel.on;
	ltitlsave = cmgem.title.on;
	lxgrdsave = cmgem.lxgrd;
	cmgem.xlabel.on = FALSE;
	cmgem.ylabel.on = FALSE;
	cmgem.title.on = FALSE;
	cmgem.lxgrd = FALSE;

	/* - Set up y window for seismogram. */
        /* - Use the upper 20% of the viewport. */
	ypdel = (cmgem.plot.ymax - cmgem.plot.ymin)/ 5.0;

	/* - Check WAIT option.  This is on when:
	 * -- A wait request has been made.
	 * -- An active device (normally the user's terminal) is on. */

	if( cmgam.lwaitr ){
		getstatus( "ACTIVE", &lwait );
		}
	else{
		lwait = FALSE;
		}

        /* plot the first input file */

	ypmxsave = cmgem.plot.ymax;
        ypmnsave = cmgem.plot.ymin;
        xpmxsave = cmgem.plot.xmax;
        xpmnsave = cmgem.plot.xmin;

	lframesave = cmgem.lframe;
	cmgem.lframe = FALSE;

		/* -- Determine time limits for x axis of this frame.
		 *    (Correct for any differences in GMT reference time.) */

	getfil( 1, TRUE, &num, &nlcy, &nlcx, nerr );
      	if( *nerr != 0 )goto L_7777;

	getxlm( &lxlims, &tmin, &tmax );
       	if( lrel ){
         	tmax = tmax - tmin;
	       	Toff[1] = -tmin;
	       	tmin = 0.;
      	}
	else{
	       	copyi( nzdttm, n1dttm, 6 );
	       	/* l1dttm = ldttm( n1dttm ); */
	       	Toff[1] = 0.;
       	}

	/* - Check range of time limits to avoid errors that could occur
	 *   later during plotting. */

	if( fabs( tmax - tmin ) > (float)( MLARGE ) ){
      		*nerr = 1504;
       		setmsg( "ERROR", *nerr );
       		goto L_7777;
	}

		/* - Set x axis plot limits. */

       	cmgem.lxlim = TRUE;
       	cmgem.ximn = tmin;
       	cmgem.ximx = tmax;

       	if( lframesave ){
       		beginframe( lprint , nerr );
       		if( *nerr != 0 )
       			goto L_7777;
       		getvspace( &cmgem.view.xmin, &cmgem.view.xmax, 
                           &cmgem.view.ymin, &cmgem.view.ymax );
	}


       	cmgem.tsdef = fmin( cmgem.tsdef, (cmgem.view.ymax - cmgem.view.ymin)/(8.0*5.0 ));
       	cmgam.tsfid = cmgem.tsdef;
       	cmgam.tspk = cmgem.tsdef;
       	cmgem.tsaxis = cmgem.tsdef;

     	cmgem.plot.ymin = cmgem.plot.ymax - ypdel;

        /* --- Get pointers to this file's location in memory. */

     	getfil( 1, TRUE, &num, &nlcy, &nlcx, nerr );
	if( *nerr != 0 )goto L_7777;

        /* --- Set up x axis data values. */

	if( *leven ){
		cmgem.xgen.on = TRUE;
	      	cmgem.xgen.delta = *delta;
	       	cmgem.xgen.first = *begin + Toff[1];
	}else{
	      	cmgem.xgen.on = FALSE;
       	}

       	/* --- Set up y axis plot limits. */

       	getylm( &cmgem.lylim, &cmgem.yimn, &cmgem.yimx );

       	/* --- Plot this file. */

       	pl2d( cmmem.sacmem[nlcx], cmmem.sacmem[nlcy], num, 1, 1, nerr );
       	if( *nerr != 0 )goto L_7777;

       	/* --- Plot picks and fileid. */

       	disppk( Toff[1] );
       	dispid( 0 , 0, 0, NULL );

       	/* --- Add a label with offset time if this is a REL plot. */

       	if( lrel && cmgam.lfidrq ){
          /* I do not think this is ever called as lrel is never TRUE */
               sprintf(kptext,"OFFSET: %10.3e", -Toff[1] );
	       cmgem.chht = cmgem.tsdef;
	       cmgem.chwid = cmgem.txrat*cmgem.chht;
	       settextsize( cmgem.chwid, cmgem.chht );
	       settextangle( TEXT_HORIZONTAL );
	       pltext( kptext,MCMSG+1, cmgam.xfidlc, cmgam.yfidlc );
	       cmgam.yfidlc = cmgam.yfidlc - cmgem.chht;
	}

		/* -- Draw bottom x axis. */
       	cmgem.axis[BOTTOM].annotate = lbotaxsave;
       	cmgem.axis[BOTTOM].ticks    = lbottcsave;
       	cmgem.axis[TOP].ticks       = ltoptcsave;
       	cmgem.lxgrd = lxgrdsave;
       	cmgem.uplot.ymax = ypmxsave * cmgem.view.ymax;
       	cmgem.chht = cmgem.tsaxis;
       	cmgem.chwid = cmgem.txrat*cmgem.chht;
       	settextsize( cmgem.chwid, cmgem.chht );

	xlinax();

       	/* -- Draw axes labels and title. */
       	if( lxlabsave )
       		centxt( kmgem.kxlab,145, cmgem.xlabel.len, cmgem.xlabel.pos, cmgem.xlabel.text_size );
       	if( lylabsave )
       		centxt( kmgem.kylab,145, cmgem.ylabel.len, cmgem.ylabel.pos, cmgem.ylabel.text_size );
       	if( ltitlsave )
       		centxt( kmgem.ktitl,145, cmgem.title.len, cmgem.title.pos, cmgem.title.text_size );

        getratio(&vspaceratio);

        nypoints = (ywmax/ymax)*(float)ny;
        nystart = ((ywmin/ymax)*(float)ny) + 1;

        /* Window Data */
	if( (nystart != 1) || (nypoints != ny) ) {
          if((wdata = (float *)malloc(sizeof(float) * 
                                      nx * (nypoints - nystart + 1)
                                      )) == NULL) {
            printf("error getting memory for windowed data--plotimage\n");
            *nerr = 0301;
            goto L_8888;
          }
          wdata_alloc = TRUE;
          window_data( specdata, nx, ny, wdata, 1, nx, nystart, nypoints );
	}
	else{
          wdata = specdata;
          wdata_alloc = FALSE;
	}

        /* Determine the Size of the Image in Viewspace coordinates */
        width  = (cmgem.plot.xmax - cmgem.plot.xmin) *  /* Total Width [ px ] */
          (xmax - xmin) /  /* Spectrogram Width [ seconds ] */
          (*ennd - cmgem.xgen.first); /* Total Time [ seconds ] */
        height = (cmgem.plot.ymax - ypmnsave) * 0.70; /* Total Height */

        /* Determine the location in viewspace coordinates */
        cmgem.plot.xmin = (cmgem.plot.xmin + 
                           (((xmin-cmgem.xgen.first)/
                             (*ennd-cmgem.xgen.first))*
                            (cmgem.plot.xmax-cmgem.plot.xmin)));
        cmgem.plot.xmax = cmgem.plot.xmin + width;

        /* yloc = 1.0 - (ypmnsave + ypdel); */

        cmgem.plot.ymax = 1.0 - (ypmnsave + ypdel);
        cmgem.plot.ymin = cmgem.plot.ymax - height;

        /* Show the iamge */
        show_image(wdata, nx, nypoints-nystart+1, 
                   xmin, xmax, ymin, ymax,
                   cmgem.plot.xmin, cmgem.plot.ymax, 
                   width, height,
                   cmgdm.npscimage,
                   cmgdm.nctsize,
                   7, lbinary, nerr);

        /* Show Colorbar */
        if(lcbar) {
          float h;
          float zmin, zmax;

          h = cmgem.plot.ymax - cmgem.plot.ymin;

          /* Determine the z-range of the data */
          frange(wdata, nx * (nypoints-nystart+1), &zmin, &zmax);

          show_colorbar(xpmnsave,
                        cmgem.plot.ymax - (h - h * 0.75) / 2,
                        0.025,
                        h * 0.75,
                        zmin, zmax, lbinary, nerr);
        }

        if(wdata_alloc) {
          free(wdata);
        }

        cmgem.ximn = xmin;
        cmgem.ximx = xmax;
        cmgem.yimn = ywmin;
        cmgem.yimx = ywmax;
        cmgem.zdata.xmin = xmin;
        cmgem.zdata.xmax = xmax;
        cmgem.zdata.ymin = ywmin;
        cmgem.zdata.ymax = ywmax;

        cmgem.uplot.xmin = cmgem.plot.xmin;
        cmgem.uplot.xmax = cmgem.plot.xmax;
        cmgem.uplot.ymin = cmgem.plot.ymin*vspaceratio;
        cmgem.uplot.ymax = cmgem.plot.ymax*vspaceratio;

        setvport(cmgem.uplot.xmin, cmgem.uplot.xmax,
                 cmgem.uplot.ymin, cmgem.uplot.ymax);

        plcalwvtrans();
        
        plgrid(nerr);

       	/* -- Home cursor, advance frame. */

       	plhome();
        flushbuffer(nerr);

       	if( lframesave )
          endframe( FALSE , nerr );
        else
          flushbuffer( nerr );

L_7777:
	plrest();
	cmgam.tsfid = cmgem.tsdef;
	cmgam.tspk = cmgem.tsdef;
	cmgem.tsaxis = cmgem.tsdef;

	cmgem.plot.ymax = ypmxsave;
        cmgem.plot.ymin = ypmnsave;
        cmgem.plot.xmax = xpmxsave;
        cmgem.plot.xmin = xpmnsave;

	cmgem.axis[BOTTOM].annotate = lbotaxsave;
	cmgem.axis[BOTTOM].ticks    = lbottcsave;
	cmgem.lframe = lframesave;

L_8888:
	return;
} /* end of function */

