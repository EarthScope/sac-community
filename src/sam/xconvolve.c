
#include <string.h>

#include "sam.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"

#include "co.h"
#include "ucf.h"
#include "msg.h"
#include "clf.h"
#include "dbh.h"
#include "cpf.h"
#include "dff.h"

#define	MCORLN	4096
#define	MWINLN	2048

void /*FUNCTION*/ xconvolve(nerr)
int *nerr;
{
	char kermsg[131], ktemp1[MCPFN+1];

	int iwinln, iwinmx, j, jdfl, jdfl_, 
	 ndxcor, ndxmas, ndxsig, ndxx, ndxy, 
	 nfft, notusd, nrerr, ntused, nzeros,
	 nlen , 	/* npts of non-master signal */
	 nlenmx , 	/* max npts of all signals */
	 nlenMaster , 	/* npts of master */
	 nlenCombined ;	/* nlen + nlenMaster - 1 */

        float *destination , *source ; /* reverse and copy master. maf 961204 */

	/*=====================================================================
	 * PURPOSE: To parse and execute the action command CONVOLVE.
	 *          This command computes convolutions.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *=====================================================================
	 * MODULE/LEVEL:  SAM/2
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
	 * LOCAL VARIABLES:
	 *    MWINLN:  Maximum length of each data window. [ip]
	 *    MCORLN:  Maximum length of correlation function. [ip]
	 *    NDXMAS:  Index in SACMEM array for master signal. [i]
	 *    NDXSIG:  Index in SACMEM array for current signal. [i]
	 *    NDXCOR:  Index in SACMEM array for unshifted correlation. [i]
	 *    NDSFILES: Number of files in a given data-set. [i]
	 *    NDSFLNUM: File number (senquental) in the current data set. [i]
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    961204:  Modified to return full range of values instead of just
	 *             the central half.  Timing changed to leave begin times
	 *             of signals unchanged from the input signals.  
	 *    920110:  Added DATA-SET update logic.
	 *    870925:  Fixed bug when signals were of different length.
	 *             Now output signals are all equal in length to the
	 *               maximum length of the input signals.
	 *    870312:  Added ability to choose master file by name.
	 *    870209:  Converted to an internal command.
	 *    830000:  Original XSC version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  870925
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - PARSING PHASE: */

	/* - Loop on each token in command: */

	while ( lcmore( nerr ) ){

		/* -- "MASTER name|n":  determine which file to copy from. */
		if( lckey( "MASTER$",8 ) ){
			if( lcirc( 1, cmdfm.ndfl, &cmsam.imast ) )
			{ /* do nothing */ }
			else if( lcchar( MCPFN, ktemp1,MCPFN+1, &notusd ) ){
        cmsam.imast = string_list_find(datafiles, ktemp1, MCPFN+1);
        if(cmsam.imast < 0) {
          arg_prev();
          cfmt( "BAD FILE NAME:",16 );
          cresp();
        }
        cmsam.imast += 1;
			} 
			else{
				cfmt( "NEED A FILE NAME OR A NUMBER:",31 );
				cresp();
			} /* end else */

			;

		} /* end if ( lckey ( "MASTER$" , 8 ) ) */

		/* -- "NUMBER n":  set number of windows. */
		else if( lkint( "NUMBER$",8, &cmsam.nwin ) )
		{ /* do nothing */ }

		/* -- "LENGTH ON|OFF|v":  set window length in seconds. */
		else if( lklogr( "LENGTH$",8, &cmsam.lwinln, &cmsam.winln ) ){
			if( cmsam.winln <= 0. )
				cmsam.lwinln = FALSE;
		} /* end else if( lklogr( "LENGTH$" ... */

		/* -- "TYPE char":  set window (taper) type. */
		else if( lklist( "TYPE$",6, (char*)kmsam.kwintp,9, MWINTP, 
		 &cmsam.iwintp ) )
		{ /* do nothing */ }

		/* -- Bad syntax. */
		else{
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();

		} /* end else */
	} /* end while */

	/* - The above loop is over when one of two conditions has been met:
	 *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
	 *   (2) All the tokens in the command have been successfully parsed. */

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

	/* - Find longest signal. */

	nlenmx = 0;
        iwinmx = 0;
	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){
		getfil( jdfl, FALSE, &ntused, &ntused, &ntused, nerr );
		if( *nerr != 0 )
			goto L_8888;
		if( cmsam.lwinln ){
			iwinln = (int)( cmsam.winln/ *delta + 0.1 );
			}
		else{
			iwinln = *npts/cmsam.nwin;
			}

		nlenmx = max( nlenmx, *npts );
                iwinmx = max( iwinmx, iwinln);
	} /* end for */

	/* - EXECUTION PHASE: */

	/* - Allocate temporary blocks for the master signal and correlation function. */

	allamb( &cmmem, nlenmx, &ndxmas, nerr );
	if( *nerr != 0 )
		goto L_8888;

        nfft = 8;

        while ( nfft < (2*iwinmx-1))
          nfft *= 2;

	allamb( &cmmem, nfft, &ndxcor, nerr );
	if( *nerr != 0 ){
		relamb( cmmem.sacmem, ndxmas, &nrerr );
		*nerr = 919;
		setmsg( "ERROR", *nerr );
		goto L_8888;
		}

	/* - Get the master signal, reverse and copy to first temporary block.
	 *   Pad with zeros if necessary. */

				 /* nlen became nlenMaster.  maf 961204 */
	getfil( cmsam.imast, TRUE, &nlenMaster, &ndxy, &ndxx, nerr );
	if( *nerr != 0 )
		goto L_7777;	/* L_8888 became L_7777.  maf 961204 */


        /* clone and reverse the master signal. overhauled, maf 961204 */
	destination = cmmem.sacmem[ndxmas];
        source = cmmem.sacmem[ndxy] + nlenMaster - 1 ;	

	while ( source >= cmmem.sacmem[ndxy] ) {
	    *destination = *source ;
	    destination++ ;
	    source-- ;
	}

	/* pad with zeros if necessary. */
	nzeros = nlenmx - nlenMaster ;	/* nlen became nlenMaster.  maf 961204 */
	if( nzeros > 0 )
		fill( cmmem.sacmem[ndxmas] + nlenMaster , nzeros , 0. );

	/* - Perform the requested function on each file in DFL. */

	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){
		jdfl_ = jdfl - 1;

		/* -- Get next file from the memory manager.
		 *    (Header is moved into common blocks CMHDR and KMHDR.) */
		getfil( jdfl, TRUE, &nlen, &ndxy, &ndxx, nerr );
		if( *nerr != 0 )
			goto L_7777;	/* L_8888 became L_7777.  maf 961204 */
		nlenCombined = nlen + nlenMaster - 1 ;	/* added. maf 961204 */

		/* -- Allocate a new block, copy signal to it, and pad with zeros if necessary. */
		allamb( &cmmem, 2 * nlenmx, &ndxsig, nerr ); /* nlenmx became 2*nlenmx. maf 961204 */
		if( *nerr != 0 )
			goto L_7777;	/* L_8888 became L_7777.  maf 961204 */
		/* copy( (int*)cmmem.sacmem[ndxy], (int*)cmmem.sacmem[ndxsig], nlenmx ); */
        copy_float( cmmem.sacmem[ndxy], cmmem.sacmem[ndxsig], nlen); 
		nzeros = 2 * nlenmx - nlen;
		if( nzeros > 0 )
			fill( cmmem.sacmem[ndxsig]+nlen, nzeros, 0. );

		/* -- Update dfl indices to point to this new block and release old one. */
		Nlndta[jdfl] = nlenCombined ; /* nlenmx became nlenCombined. maf 961204 */
		cmdfm.ndxdta[jdfl_][0] = ndxsig;
		relamb( cmmem.sacmem, ndxy, nerr );


		/* -- Compute length of each window. */
		if( cmsam.lwinln ){
			iwinln = (int)( cmsam.winln/ *delta + 0.1 );
			}
		else{
			iwinln = nlenmx/cmsam.nwin;
			}

		/* -- Compute the (unshifted) correlation. */
		crscor( cmmem.sacmem[ndxmas], cmmem.sacmem[ndxsig], nlenmx, cmsam.nwin, 
		 iwinln, (char*)kmsam.kwintp[cmsam.iwintp - 1], cmmem.sacmem[ndxcor], 
		 &nfft, kermsg,131 );
		if( memcmp(kermsg,"        ",8) != 0 ){
			*nerr = 1;
			setmsg( "ERROR", *nerr );
			apcmsg( kermsg,131 );
			goto L_7777;	/* L_8888 became L_7777.  maf 961204 */
			}

		/* -- Perform a circular shift to align the correlation in the output block. */
		/*	overhauled to get full range of convolution.  maf 961204 */
		for( j = 0; j <= nlenMaster - 2 ; j++ )
                        *(cmmem.sacmem[ndxsig]+j) = *(cmmem.sacmem[ndxcor]+nfft-nlenMaster+j+1);
		for ( j = 0 ; j <= nlen - 1 ; j++ )
                        *(cmmem.sacmem[ndxsig]+nlenMaster+j-1) = *(cmmem.sacmem[ndxcor]+j);

		/* Pad with zeros if necessary.  maf 961204 */
                nzeros = 2 * nlenmx - 1 - nlenCombined ;
                if( nzeros > 0 )
                    fill( cmmem.sacmem[ndxsig]+nlenCombined, nzeros, 0. );


		/* -- Update any header fields that may have changed. */
		/*	overhauled to preserve differences in begin times. maf 961204 */
		*npts = nlenCombined;
/*		*begin = -(float)( nlenmx )**delta + *begin - masterBegin ; */
		*ennd = *begin + *delta*(float)( nlenCombined - 1 );
		extrma( cmmem.sacmem[ndxsig], 1, nlenCombined, depmin, depmax, depmen );
/*		*nzyear = cmhdr.fundef ; */
/*		*nzhour = cmhdr.fundef ; */

		/* -- Return file to memory manager. */
		putfil( jdfl, nerr );
		if( *nerr != 0 )
			goto L_7777;	/* L_8888 became L_7777.  maf 961204 */

	} /* end for(jdfl) */

	/* - Release temporary blocks. */
L_7777:	/* added. maf 961204 */
	relamb( cmmem.sacmem, ndxmas, nerr );
	if( *nerr != 0 )
		goto L_8888;
	relamb( cmmem.sacmem, ndxcor, nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Calculate and set new range of dependent variable. */

	setrng();

L_8888:
	return;

} /* end of function */

