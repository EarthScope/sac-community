/** 
 * @file   xgrayscale.c
 * 
 * @brief  Gray Scale
 * 
 */

#include <stdio.h>

#include "xyz.h"
#include "amf.h"
#include "bool.h"

#include "dfm.h"
#include "msg.h"
#include "co.h"
#include "cpf.h"
#include "dff.h"

void 
xgrayscale(int *nerr) {

	char arg1[151], runfile[13];
	int  lvideo;
	int  graylength, graywidth, imagelength, imagewidth, jfile, 
	 ndxz, nfiles, nlen, notused, nxsize, nysize, nzsize, 
          xmax, xmin, ymax, ymin;
        FILE *un;
  double tmp;
	static int maximagesize = 2050;
	static char videotype = 'n';
	static float scale = 1.0;
	static int zoom = 0;
	static int xcrop = FALSE;
	static int ycrop = FALSE;
	static int xcropmin = 0;
	static int xcropmax = 0;
	static int ycropmin = 0;
	static int ycropmax = 0;


	/*=====================================================================
	 * PURPOSE:  To execute the action command GRAYSCALE.
	 *           This command displays the xyz data stored in memory as a
	 *           grayscale image using Utah's Toolkit utilities. A shell script
	 *           called 'Utahgrayscale' is run that converts the floating 
	 *           point image to a gray scale image and displays the image.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *   nerr:  0 - no error, .ne. 0 - error
	 *=====================================================================
	 * MODULE/LEVEL:  xyz/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mem:  sacmem
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    xyz:  lcleanup
	 *=====================================================================
	 * GLOBAL COUPLING: none
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB: cfmt, cresp, cnvfta, cnvita, zrunname, zrun
	 *=====================================================================
	 * LOCAL VARIABLES: see below
	 *=====================================================================
	 * ASSUMPTIONS: none
	 *=====================================================================
	 * LIMITATIONS: none
	 *=====================================================================
	 * KNOWN ERRORS: none
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900305:  Modified due to changes in xyz data storage method.
	 *    900130:  Original version by Terri Quinn.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:
	 *===================================================================== */
	/* LOCAL VARIABLES */
	/* DATA SET: */
	/* PROCEDURE: */
	*nerr = 0;

	/* - Loop on each token in command: */
L_1000:
	if( lcmore( nerr ) ){

		/* -- VIDEOTYPE c: set image video display type to be 'n' for normal
		 *                 or 'r' for reverse. */
		if( lklog2( "VIDEOTYPE$",11, "NORMAL$",8, "REVERSE$",9, &lvideo ) ){
			if( lvideo ){
				videotype = 'n';
				}
			else{
				videotype = 'r';
				}

			/* -- SCALE v: set image scaling power. */
			}
		else if( lkreal( "SCALE$",7, &tmp ) ){
      scale = (float) tmp;
			/* -- ZOOM n: set integer zoom factor for image - zoom is
			 *            accomplished by pixel replication. */
			}
		else if( lkint( "ZOOM$",6, &zoom ) ){
			if( zoom <= 1 )
				zoom = 0;

			/* -- XCROP ON|OFF|n1 n2: set limits for cropping image in 'x' direction. */
			}
		else if( lckey( "XCROP$",7 ) ){
			if( lclog( &xcrop ) ){
				}
			else if( lcircp( 0, maximagesize, &xcropmin, &xcropmax ) ){
				xcrop = TRUE;
				}
			else{
				xcrop = TRUE;
				}

			/* -- YCROP ON|OFF|n1 n2: set limits for cropping image in 'y' direction. */
			}
		else if( lckey( "YCROP$",7 ) ){
			if( lclog( &ycrop ) ){
				}
			else if( lcircp( 0, maximagesize, &ycropmin, &ycropmax ) ){
				ycrop = TRUE;
				}
			else{
				ycrop = TRUE;
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

	/* CHECKING PHASE: */

	/* - Check for null data file list. */

	vflist( nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Check to make sure all files are xyz data files. */

	vfxyz( nerr );
	if( *nerr != 0 ){
		aplmsg( "Must be XYZ data files to use this command.",44 );
		goto L_8888;
		}

	/* - For each file in DFL: */

	getnfiles( &nfiles );
	for( jfile = 1; jfile <= nfiles; jfile++ ){

		/* -- Get file from memory manager. */
		getfil( jfile, TRUE, &nlen, &ndxz, &notused, nerr );
		if( *nerr != 0 )
			goto L_8888;

		/* -- Create image dimensions to agree with the grayscale image produced by
		 *    'float2image' called by script 'Utahgrayscale'. The image dimensions 
		 *    must both be even for display with University of Utah's toolkit. */
		getnhv( "NXSIZE", &nxsize, nerr , 6 );
		if( *nerr != 0 )
			goto L_8888;
		getnhv( "NYSIZE", &nysize, nerr , 6 );
		if( *nerr != 0 )
			goto L_8888;
		imagewidth = nxsize;
		imagelength = nysize;
		nzsize = nxsize*nysize;
		graywidth = imagewidth;
		graylength = imagelength;
		if( (imagewidth%2) != 0 )
			graywidth = imagewidth + 1;
		if( (imagelength%2) != 0 )
			graylength = imagelength + 1;

		/* -- Set cropping limits if any. */
		if( xcrop ){
			xmin = max( xcropmin, 1 );
			xmax = min( xcropmax, imagewidth );
			}
		else{
			xmin = 1;
			xmax = imagewidth;
			}
		if( ycrop ){
			ymin = max( ycropmin, 1 );
			ymax = min( ycropmax, imagelength );
			}
		else{
			ymin = 1;
			ymax = imagelength;
			}

		/* -- Make cropping limits even for Utah's Toolkit */
		if( ((xmax - xmin + 1)%2) != 0 )
			xmax = xmax + 1;
		if( ((ymax - ymin + 1)%2) != 0 )
			ymax = ymax + 1;

		/* -- Make sure zoom doesn't create an image too large for screen */
		if( zoom > 0 ){
			if( (zoom*graywidth > maximagesize) || (zoom*graylength > 
			 maximagesize) ){
				*nerr = 2902;
				setmsg( "ERROR", *nerr );
				apimsg( maximagesize );
				goto L_8888;
				}
			}

		/* -- Format argument list for "display." */
                sprintf(arg1,"%10d%10d%10.2f %c%10d%10d%10d%10d%10d%10d%10d%10d%s\n",
		 imagewidth, imagelength, scale, videotype, zoom, xcrop || 
		 ycrop, graywidth, graylength, xmin, ymin, xmax, ymax, " >>output.Utahtmp"
		  );

		/* -- Write image in memory to disk */
		writezdata( "zdata.Utahtmp",14, cmmem.sacmem[ndxz], &nzsize, nerr );
		if( *nerr != 0 )
			goto L_8888;

		/* -- Write c-shell script to display image using Univ. Utah's toolkit */
		zrunname( "Utahgrayscale",14, arg1,151, &un, runfile,13, nerr );
		if( *nerr != 0 )
			goto L_8888;

		/* -- Run script */
		zrun( &un, runfile,13, nerr );
		if( *nerr != 0 )
			goto L_8888;

		}

	/* - Set cleanup flag. */

	cmxyz.lcleanup = TRUE;

L_8888:
	return;

} /* end of function */

