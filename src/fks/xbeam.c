/** 
 * @file   xbeam.c
 * 
 * @brief  Compute Beam
 * 
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "fks.h"
#include "dfm.h"
#include "bool.h"
#include "hdr.h"
#include "amf.h"


#include "ucf.h"
#include "msg.h"
#include "cpf.h"
#include "co.h"
#include "ncpf.h"
#include "dff.h"

#define NFILE_LENGTH 128 /* max length of output file name */

void
xbeam(int *nerr) {

	char kfile[NFILE_LENGTH];
	int elevc;
	int iadv, idummy, inptr, jdx, jdfl, jdfl_, joptr, jout, 
	 nckofbeam, nec, nsamps, nsampsout, number, numbersav;
	float advance, angle, anglev, beginout, delt_horiz, deltaout, 
	 el_delay, endout, vmax, vmen, vmin,
	 xr[MXLENP], yr[MXLENP], zr[MXLENP]; 
  double ra[3];
	int   lLocalRef ;	
	int   nLocalRef ;	
	double rLocalRef[ 3 ] ;

	int   lfillz = 0 ;	/* set to 1 if npts or begins differ. maf 970211 */
  int n;
  double tmp;
	double *const Ra = &ra[0] - 1;
  float *Sacmem;

	/*=====================================================================
	 * PURPOSE: To compute beam for an array of stations
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers:
	 *=====================================================================
	 * MODULE/LEVEL:   fks/2
	 *=====================================================================
	 * GLOBAL INPUT:  (to be updated)
	 *=====================================================================
	 * GLOBAL OUTPUT: (to be updated)
	 *=====================================================================
	 * SUBROUTINES CALLED: (to be updated)
	 *    saclib: 
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    970306:  OFFSET option allows user to specify method of 
	 *             determining offsets.  maf
	 *    970211:  Stops and gives error message if deltas aren't the same.
	 *             If npts or begin times aren't the same, it warns the 
	 *             user, and then fills with zeros and does the beam.  maf
	 *    970207:  Added reference option, and replaced calcoffsets with
	 *             calcBeamOffsets.  maf
	 *    970108:  xr, yr, and zr are set by calcoffsets() now as opposed
	 *             to being set to user7, user8, and user9.  In the 
	 *             process, xr, yr, and zr became arrays.  maf
	 *    960709:  nint didn't compile, so I wrote my own.
	 *    960709:  Changed back to nint so it will work on negatives
	 *    910826:  Changed nint(x) to int(x + .5) to improve portability 
	 *             to DEC 5000 workstation per Gyu-sang Jang @ UC Davis.
	 *    91064:   Changed write(*,*) to write(munout,*) wct.
	 *    910301:  Port from a code segment from XAP/STKOPS
	 *
	 * ORIGINAL AUTHOR:  Dave Harris 
	 *           L-205 
	 *           Lawrence Livermore National Laboratory  
	 *           P.O. Box 808     
	 *           Livermore, CA  94550  
	 *           USA               
	 *
	 *           (415) 423-0617  
	 *
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  
	 *=====================================================================
	 *
	 *  Command parser declarations                                                  
	 *
	 *  Main (signal stack) common block                                             
	 * */


	/* These were the parameters XAP looked for
	 *          CALL KEYCHN( 'VELOCITY BEARING EC OUTNAME CENTER' )           
	 * The non-keyworded parameter NUMBER is suppressed for SAC
	 *          CALL FI( ' ', 1, 'OPTIONAL', NCH, NUMBER)                      */
	number = cmdfm.ndfl;

	/*    PARSING PHASE    
	 * */
	elevc = FALSE;
	nckofbeam = nstrlensp( cmfks.kofbeam,MCPFN+1 );

	while ( lcmore( nerr ) ){
	    int offsetTemp = 0 ;

	    if( lkreal( "B#EARING$",10, &tmp ) ) {
        cmfks.bear = (float) tmp;
      }
	    else if( lkreal( "V#ELOCITY$",11, &tmp ) ) {
        cmfks.veloc = (float) tmp;
      }

	    else if( lkra( "E#C$",5, 0, 2, ra, &nec ) ){
		elevc = TRUE;
		/* Angle of incidence -- measured from vertical
		 *  (the farther the source, the smaller the angle) */
		if( nec == 0 )
		{ /* do nothing */ }

		/* keep values initialized or previously set in /common/cmfks */
		else if( nec == 1 ){
		    cmfks.anginc = Ra[1];
		}

		/* - keep the value for survel  */
		else{
		    cmfks.anginc = Ra[1];
		    cmfks.survel = Ra[2];
		}
	    } /* end else if( lkra( "E#C$",5, 0, 2, ra, &nec ) ) */

	    else if( lkra( "C#ENTER$",9, 2, 3, ra, &n ) ){
        cmfks.nctr = n;
        cmfks.ctrx = Ra[1];
        cmfks.ctry = Ra[2];
        if( cmfks.nctr == 3 )
          cmfks.ctrz = Ra[3];
	    }

	    /* OFFSET: allow user to specify a method of determining offsets */
	    else if ( lklist( "O#FFSET$",9, (char*)cmfks.koffset,9,
		      MOFFSETOPTS, &offsetTemp ) )
	    { cmfks.flagOffset = offsetTemp - 1 ; }

	    /* REFERENCE: allow user to specify a reference point. maf 970207 */
	    else if ( lklogra ( "REF#ERENCE$" , 12 , &lLocalRef , 
		      2 , 3 , rLocalRef , &nLocalRef , nerr ) ) {
		if ( *nerr != 0 ) {
		    *nerr = 0 ;
		    goto L_9999 ;
		}

		if ( lLocalRef == 2 ) {		/* Numbers were entered */
		    cmfks.lReference = TRUE ;
		    cmfks.rReference[0] = rLocalRef[0] ;
		    cmfks.rReference[1] = rLocalRef[1] ;
		    cmfks.rReference[2] = nLocalRef == 3 ? rLocalRef[2] : 0.0 ;
		    cmfks.nReference = nLocalRef ;
		}
		else
		    cmfks.lReference = lLocalRef ;
	    } /* end else if ( lklogra ( "REF#ERENCE$" , 12 ... ) */

	    else if(lkchar("W#RITE$",8, 24, cmfks.kofbeam,MCPFN+1, &nckofbeam))
	    { /* do nothing */ }

	    else{
		cfmt( "ILLEGAL OPTION:",17 );
		cresp();
		goto L_9999;
	    }

	} /* end while */

	/* CHECKING PHASE 
	 * */
	if( elevc && cmfks.nctr == 2 ){
	    fprintf( MUNOUT, "ERR: If EC is desired, you must specify z for CENTER.\n" );
	    goto L_9999;
	}


	/* EXECUTION PHASE 

	 * */
	angle = (cmfks.bear/180.0)*3.14159265;
	anglev = (cmfks.anginc/180.)*3.14159265;

	/* This line was in for loop in if(jdfl == 1) block. 
	   moved out here.  maf 970211 */
	fstrncpy( kfile, NFILE_LENGTH - 1, cmfks.kofbeam, min(nckofbeam,MCPFN));

	numbersav = number;

	/* Now getting xr, yr, and zr from calcoffsets instead of from
	   user7, 8, and 9 directly.  maf 970108 */
	/* calcBeamOffsets replaces calcoffsets.  maf 970207 */
	calcBeamOffsets ( number , elevc , xr , yr , zr , nerr ) ;
	if ( *nerr != 0 )
	    goto L_9999 ;
	
    beginout = endout = 0.0;
    deltaout = 0;
    nsampsout  = 0;
        
	/* Preliminary loop through files to check delta and get extremes added.*/
	for( jdfl = 1; jdfl <= number; jdfl++ ){
	    getfil( jdfl, TRUE, &nsamps, &inptr, &idummy, nerr );
	    if( *nerr != 0 )
		goto L_9999;

	    if( jdfl == 1 ){
		nsampsout = nsamps;
		beginout = *begin;
		deltaout = *delta;
		endout = *ennd;
	    }

	    else {
		if ( *delta != deltaout ) {
		    /* error handling */
		    *nerr = 1801;
		    setmsg ("ERROR" , *nerr) ;
		    apcmsg ( "DELTA" , 6 ) ;
		    apcmsg ( " - can be fixed using INTERPOLATE command", 42);
		    outmsg () ;
		    goto L_9999;
		} /* end if ( *delta != deltaout ) */

		/* if waveforms not properly aligned ... */
		if ( nsamps != nsampsout || *begin != beginout ) {
		    lfillz = 1 ; /* set flag to fill with zeros */

		    /* get the extreme values. */
		    beginout  = beginout < *begin ? beginout : *begin ;
		    endout    = endout   > *ennd  ? endout   : *ennd ;
		} 
	    } /* end else associated with if( jdfl == 1 ) */
	} /* end Preliminary loop through files. */

	/* if waveforms weren't properly aligned, warn user. maf 970211 */
	if ( lfillz ) {
	    printf ( "\a\nCAUTION:  Waveforms not properly aligned." ) ;
	    printf ( "  Filling with zeros and proceding.\n");
	}
	
	/* These lines were in for loop in if(jdfl == 1) block.  I moved them
	   out to here, and there's a new way to get nsampsout. The '+ 2' in
	   the calculation of nsampsout is 1 for the math, and 1 for the
	   FORTRAN indexing.  maf 970211 */
	jout = number + 1 ;
	Ncomp[jout] = 1 ;
	Nlndta[jout] = nsampsout = ( ( endout - beginout ) / deltaout ) + 2 ;

	/* Allocate space for the input waveforms. */
	allamb( &cmmem, Nlndta[jout], &cmdfm.ndxdta[jout - 1][0], nerr );
	if( *nerr != 0 )
	    goto L_9999;
	joptr = cmdfm.ndxdta[jout - 1][0];

	/* Loop between files to build the beam, overhauled.  maf 970211 */
	for( jdfl = 1; jdfl <= number; jdfl++ ){
	    float * waveform = NULL ;

	    jdfl_ = jdfl - 1;

	    getfil( jdfl, TRUE, &nsamps, &inptr, &idummy, nerr );
	    if( *nerr != 0 )
		goto L_9999;
	    if( *iftype != *itime ){
		numbersav = jdfl - 1;
		break ;
	    }

	    /* if necessary, do a fillz.  maf 970211 */
	    if ( lfillz ) {
		int n ;

		/* allocate space for the filled waveform. 
		   calloc initiallizes the space to zreo. */
		waveform = (float *) calloc ( nsampsout , sizeof ( float ) ) ;
		if ( waveform == NULL ) {
		    /* error handling */
		    printf("error allocating filled waveform--xbeam\n");
		    *nerr = 301;
		    return ;
		}

		/* copy the waveform. */
		n = ( ( *begin - beginout ) / *delta ) /* + 0.5 */ ;
		memcpy ( waveform + n , cmmem.sacmem[inptr] ,
			 nsamps * sizeof ( float ) ) ;

	    } /* end if ( lfillz ) */
	    else
		waveform = cmmem.sacmem[inptr] ;

	    /* Use dot product to get signed horizontal distance from
               array center to station */

	    /* xr and yr are now arrays.  maf 970108 */
	    delt_horiz = cos ( angle ) * ( yr[ jdfl_ ] - cmfks.ctry ) + 
			 sin ( angle ) * ( xr[ jdfl_ ] - cmfks.ctrx ) ;

	    /* Compute the delay for the input phase velocity.  */

	    advance = delt_horiz/cmfks.veloc;

	    if( elevc ){	/* zr is now an array.  maf 970108 */
		el_delay = ( zr[ jdfl_ ] - cmfks.ctrz ) /
			   ( cmfks.survel * cos ( anglev ) ) ;
		advance = advance - el_delay;
	    }

	    /* Convention:  input bearing is receiver to station backazimuth. 
	       Signals at station on the source side from the center of the
	       array should start earlier in time and should have a positive
	       delay. */  

	    /* rewrote rounding, maf 960709 */
	    iadv = (int) ( advance >= 0 ? advance / *delta + 0.5 :
					   advance / *delta - 0.5 ) ;
	    beamadd( waveform, cmmem.sacmem[joptr], nsampsout, iadv );

	    /* if new space was created release it */
	    if ( lfillz && waveform != NULL ) {
		free ( waveform ) ;
		waveform = NULL ;
	    }

	} /* end for( jdfl = 1; jdfl <= number; jdfl++ ) */

	number = numbersav;
	fprintf( MUNOUT, "NUM TIME-SERIES FILES FROM THE START OF DFL:%d \n", 
	 number );
	if( number < 1 )
	    goto L_9999;

        Sacmem = cmmem.sacmem[joptr];
	for( jdx = 0; jdx <= (nsampsout - 1); jdx++ ){
	    *(Sacmem++) /= (float)number;
	}

	/*  Set up header for output file */
	allamb( &cmmem, SAC_HEADER_WORDS, &Ndxhdr[jout], nerr );
	if( *nerr != 0 ){
	    relamb( cmmem.sacmem, cmdfm.ndxdta[jout - 1][0], nerr );
	    goto L_9999;
	}

	newhdr();
	*iftype = *itime;
	*npts = nsampsout;
	*begin = beginout;
	*delta = deltaout;
	extrma( cmmem.sacmem[cmdfm.ndxdta[jout - 1][0]], 1, nsampsout, &vmin, 
	 &vmax, &vmen );
	Fhdr[2] = vmin;
	Fhdr[3] = vmax;
	Fhdr[57] = vmen;
	*user7 = 0.;
	*user8 = 0.;
	*user9 = 0.;

	putfil( jout, nerr );
	if( *nerr != 0 ){
	    relamb( cmmem.sacmem, Ndxhdr[jout], nerr );
	    relamb( cmmem.sacmem, cmdfm.ndxdta[jout - 1][0], nerr );
	    goto L_9999;
	}

	cmdfm.ndfl = jout;

	/*  Write SAC file from SACMEM out to OS
	 * */
	wrsac( jout, kfile,NFILE_LENGTH, TRUE, nerr );
	relamb( cmmem.sacmem, Ndxhdr[jout], nerr );
	relamb( cmmem.sacmem, cmdfm.ndxdta[jout - 1][0], nerr );
	cmdfm.ndfl = cmdfm.ndfl - 1;

L_9999:
	return;
} /* end of function */


/*  Function to add beam */ 
void 
beamadd(float *s, 
        float *b, 
        int    n, 
        int    iadv) {

	int i;
	float *const B = &b[0];
	float *const S = &s[0];

	for( i = 0; i < n; i++ )
	  if ( (i-iadv) >= 0 && (i-iadv) < n ) 
	    B[i] += S[i-iadv];

	return;
} 
