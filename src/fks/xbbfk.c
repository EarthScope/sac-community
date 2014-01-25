/** 
 * @file   xbbfk.c
 * 
 * @brief  Compute Broadband FK
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "complex.h"
#include "fks.h"
#include "dfir.h"
#include "bool.h"
#include "dfm.h"
#include "gem.h"
#include "hdr.h"
#include "gdm.h"
#include "amf.h"
#include "co.h"


#include "bot.h"
#include "msg.h"
#include "clf.h"
#include "bbs.h"
#include "dbh.h"
#include "cpf.h"
#include "dff.h"
#include "ncpf.h"

#define	MXLENB	40000
#define MXLENS 200

void 
xbbfk(int *nerr) {
	char cnumber[13], kfile[9], label[121];
	int lformer, llocal, lssq_specified , lprint = FALSE , ltry = FALSE ;
	int idx, iasiz[2], idummy, isacm, issq, ix, iy,
	 jdx, jdfl, nch, nckofbbfk, ncktitle, notused ,
	 ns, nsamps[MXLENS], nsiz, nssav, ptr[MXLENS], wvaz;
	float buffer[MXLENB], curntval, offset, trace, 
	 xr[MXLENS], yr[MXLENS], zr[MXLENS];
	complexf scm[MXLENS*MXLENS], sinv[MXLENS*MXLENS];
  double tmp;
  int maxflag;

  char *cattemp;

  float *Sacmem;

	float *const Buffer = &buffer[0] - 1;
	int *const Iasiz = &iasiz[0] - 1;
	int *const Nsamps = &nsamps[0] - 1;
	int *const Ptr = &ptr[0] - 1;
	complexf *const Scm = &scm[0] - 1;
	complexf *const Sinv = &sinv[0] - 1;

	/* ============================================================
	 * PURPOSE: to compute the broadband FK (frequency-wavenumber)
	 *          spectral estimate on all files in memory.
	 *          also displays the spectral estimate as a contour plot.  
	 *          Sets up broadband covariance matrix, calls wavenumber 
	 *          spectrum calculation routine, then calls plotting routines.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    nerr:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers:
	 *=====================================================================
	 * MODULE/LEVEL:   fks/2
	 *=====================================================================
	 * GLOBAL INPUT:  (to be updated)
	 *    dfm:     ndfl
	 *    hdr:     delta, begin
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    dfm:     ndfl
	 *=====================================================================
	 * SUBROUTINES CALLED: (to be updated)
	 *    saclib: 
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *   911001:  Added music option as alternative high-res method to mlm;
	 *            handle paramters 'mlm neigenmlm' or 'mus neigenmus',
	 *            and display eigenvalues
	 *   910920:  Replaced existing routine of 'covmatf' with a new version
	 *            'covmat' supplied by Dave Harris;
	 *            'covmat' calls iirfilter (new vers in delta dated 10/1/91)
	 *            the high resolution method 'MLM' now calls 'eigenanal'
	 *            (replacing 'cominv') 
	 *            add the high resolution method 'MUSIC' which also calls
	 *            'eigenanal'
	 *   910624:  Changed write(*,*) to write(munout,*)
	 *   901201:  Port from the version in XAP, which has
	 *            DLM = 891016 (in dir). JYio
	 *
	 * ORIGINAL AUTHOR:  Dave Harris -- BBFK originally implemented in XAP
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
	 *=====================================================================  */

	/*    Load signal stack common block
	 *
	 *
	 *  Main (signal stack) common block
	 * */


	/* CHECKING PHASE: */
	/* - Check for null data file list. */
	vflist( nerr );
	if( *nerr != 0 ) 
	    goto L_8888 ;

	nch = cmdfm.ndfl;

	/* Exit if to many files */
	if( nch < 3 || nch > MXLENS ){
            *nerr = 5308 ;
	    /*            setmsg( "ERROR", *nerr ) ;
            apcmsg( "Try fewer files.", 17 ) ;
            outmsg() ; */
	    fprintf( MUNOUT, "Number of files must be between 3 and MXLENS=%d \n", MXLENS );
	    goto L_8888 ;
	}


	/* Initialize local variables with default values
	 *
	 * Parse command    
	 * */
	lssq_specified = FALSE;

	ns = nch;

	while ( lcmore( nerr ) ){
	    if( lklog( "F#ILTER$",9, &cmfks.lkfilter ) )
	    { /* do nothing */ }

	    else if( lklog( "NO#RMALIZE$",12, &cmfks.lknorm ) )
	    { /* do nothing */ }

	    else if( lklogr( "EP#S$",6, &cmfks.leps, &tmp ) ) {
        cmfks.eps = (float) tmp;
      }

	    else if( lklogi( "ML#M$",6, &llocal, &cmfks.neigenmlm ) ){
		if( llocal ){
		    strcpy( cmfks.kfkmethod, "MLM " );
		}
		else{
		    strcpy( cmfks.kfkmethod, "PDS " );
		}
	    }

	    else if( lklogi( "MU#SIC$",8, &llocal, &cmfks.neigenmus ) ){
		if( llocal ){
		    strcpy( cmfks.kfkmethod, "MUS " );
		}
		else{
		    strcpy( cmfks.kfkmethod, "PDS " );
		}
	    }

	    else if( lklog( "P#DS$",6, &llocal ) ){
		if( llocal ){
		    strcpy( cmfks.kfkmethod, "PDS " );
		}
		else{
		    strcpy( cmfks.kfkmethod, "MLM " );
		}
	    }

	    else if( lklogi( "EX#P$",6, &cmfks.lexp, &cmfks.iexp ) )
	    { /* do nothing */ }

	    else if( lkreal( "WA#VE$",7, &tmp ) ) {
        cmfks.rkhor = (float) tmp;
      }

	    else if( lkia( "S#IZES$",8, 2, 2, iasiz, &nsiz ) ){
		cmfks.iazs = Iasiz[1];
		cmfks.iwvs = Iasiz[2];
		if( (((cmfks.iazs%2) != 0) || ((cmfks.iwvs%2) != 0)) || 
		 (cmfks.iazs*cmfks.iwvs > MXLENB) ){
		    fprintf( MUNOUT, "ERR: For SIZE, you must give 2 even nums whose product is < %d \n", 
		     MXLENB );
		    goto L_8888 ;
		}
	    }

	    else if( lkint( "LE#VELS$",9, &cmfks.nlcontour ) )
	    { /* do nothing */ }

	    else if( lclog2( "D#B$",5, "LI#NEAR$",9, &lformer ) ){
		if( lformer ){
		    strcpy( cmfks.scalng, "DB      " );
		}
		else{
		    strcpy( cmfks.scalng, "LINEAR  " );
		}
	    }

	    else if( lkquot( "T#ITLE$",8, 80, cmfks.ktitle,81, &ncktitle ) ){
		cmfks.ktitle[ncktitle] = '$';
	    }

	    else if( lklogc( "WR#ITE$",8, &cmfks.lwr, cmfks.kofbbfk,MCPFN+1 ) )
	    { /* do nothing */ }

	    else if( lkint( "SS#Q$",6, &issq ) ){
		lssq_specified = TRUE;
		if( ((cmfks.idimsq%2) != 0) || cmfks.idimsq*cmfks.idimsq > 
		 MXLENB ){
		    fprintf( MUNOUT, "ERR: For WRITE, num of samples squared must be < %d \n", 
		     MXLENB );
		    goto L_8888 ;
		}
	    }

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

	    else{
		cfmt( "ILLEGAL OPTION:",17 );
		cresp();
		goto L_8888 ;
	    }
	} /* end while ( lcmore( nerr ) ) */

	if( lssq_specified ){
	    if( cmfks.lwr ){
		cmfks.idimsq = issq;
	    }
	    else{
		fprintf( MUNOUT, "WARNING: new SSQ ignored when WRITE is OFF.\n" );
	    }
	}


	/*  find data for each channel in sacmem (known here as DATA)
	 *  storing start word index ptr (in PTR); 
	 *  also read NSAMPS, VDELTA and X,Y,Z
	 * */
	nssav = ns;
	for( jdfl = 1; jdfl <= ns; jdfl++ ){
	    getfil( jdfl, TRUE, &Nsamps[jdfl], &Ptr[jdfl], &idummy, nerr );
	    if( *nerr != 0 )
		goto L_8888 ;
	    if( *iftype != *itime ){
		nssav = jdfl - 1;
		break ;
	    }
	}

	ns = nssav;
	fprintf( MUNOUT, "NUM TIME-SERIES FILES FROM THE START OF DFL:%d \n", ns );
	if( ns < 3 || ns > MXLENS ){
	    fprintf( MUNOUT, "ERR: THIS NUM MUST BE FROM 3 TO %d \n", MXLENS );
	    goto L_8888 ;
	}

        /* calculate offsets */
        calcoffsets(ns, xr, yr, zr, nerr);
        if( *nerr != 0 )
	    goto L_8888 ;

  
	/*  Calculate broadband spatial covariance matrix    
	 * */
	covmat( cmmem.sacmem, ptr, ns, Nsamps[1], cmfks.lkfilter, scm, nerr );

	if( *nerr != 0 )
	    goto L_8888 ;

	/*    Normalization, preserves the trace of the matrix, but equalizes
	 *      the major diagonal elements                                  
	 * */
	trace = 0.0;
	for( idx = 1; idx <= ns; idx++ ){
	    trace = trace + cmplxtof( Scm[idx + (idx - 1)*ns] );
	}
	if( cmfks.lknorm ){
	    for( idx = 1; idx <= ns; idx++ ){
		*scale = sqrt( trace/(cmplxtof( Scm[idx + (idx - 1)*ns] )*ns) );
		for( jdx = 1; jdx <= ns; jdx++ ){
		    Scm[idx + (jdx - 1)*ns] = cmplxmul(Scm[idx + (jdx - 1)*ns],flttocmplx(*scale,0.));
		    Scm[jdx + (idx - 1)*ns] = cmplxmul(Scm[jdx + (idx - 1)*ns],flttocmplx(*scale,0.));
		}
	    }
	}

	/*    Regularization  
	 * */
	if( cmfks.leps ){
	    offset = (trace*cmfks.eps)/(float)( ns );
	    for( idx = 1; idx <= ns; idx++ ){
		Scm[idx + (idx - 1)*ns] = cmplxadd(Scm[idx + (idx - 1)*ns],flttocmplx( offset, 0. ));
	    }
	}


	/*  If the type is mlm, invert the spatial covariance matrix 
	 * */
	if( strcmp(cmfks.kfkmethod,"MLM ") == 0 ){
	    if( cmfks.neigenmlm == 0 ){
		eigenanal( ns, ns, scm, cmfks.kfkmethod, ns, sinv, nerr );
	    }
	    else{
		eigenanal( ns, ns, scm, cmfks.kfkmethod, cmfks.neigenmlm, 
		 sinv, nerr );
	    }
	    for( idx = 1; idx <= (ipow(ns,2)); idx++ ){
		Scm[idx] = Sinv[idx];
	    }
	}

	/*   IF THE TYPE IS MUS, CALCULATE THE PROJECTION MATRIX 
	 * */
	else if( strcmp(cmfks.kfkmethod,"MUS ") == 0 ){
	    if( cmfks.neigenmus == 0 ){
		eigenanal( ns, ns, scm, cmfks.kfkmethod, (ns/4)+1, sinv, nerr );
	    }
	    else{
		eigenanal( ns, ns, scm, cmfks.kfkmethod, cmfks.neigenmus, sinv, nerr );
	    }
	    for( idx = 1; idx <= (ipow(ns,2)); idx++ ){
		Scm[idx] = Sinv[idx];
	    }
	}

	/*  Call full-rank wavenumber spectrum calculation routine  
	 * */
	if ( strcmp ( cmfks.kfkmethod , "MLM " ) == 0 ||
	     strcmp ( cmfks.kfkmethod , "MUS " ) == 0 ) {
	    cmfks.fkmax = FMAXFLOAT;
	    cmfks.fkmax_azimuth = FMINFLOAT;
	    cmfks.fkmax_wavenum = FMAXFLOAT;
	    maxflag = FALSE;
	}
	else {
	    cmfks.fkmax = FMINFLOAT;
	    cmfks.fkmax_azimuth = FMINFLOAT;
	    cmfks.fkmax_wavenum = FMAXFLOAT;
	    maxflag = TRUE;
	}
        
	fkevalp( scm, xr, yr, ns, cmfks.iazs, cmfks.rkhor, cmfks.iwvs, 
	 buffer, cmfks.fkmax, cmfks.fkmax_azimuth, cmfks.fkmax_wavenum, maxflag );

	/*  If the type is mlm, invert the wavenumber spectrum  
	 * */
	if ( strcmp ( cmfks.kfkmethod , "MLM " ) == 0 ||
	     strcmp ( cmfks.kfkmethod , "MUS " ) == 0 ) {
	    for( idx = 1; idx <= (cmfks.iazs*cmfks.iwvs); idx++ ){
		Buffer[idx] = 1./Buffer[idx];
	    }
	}

	/*  Raise the wavenumber spectrum to the indicated power           
	 * */
	if( cmfks.lexp && cmfks.iexp != 1 ){
	    for( idx = 1; idx <= (cmfks.iazs*cmfks.iwvs); idx++ ){
		Buffer[idx] = powi(Buffer[idx],cmfks.iexp);
	    }
	}

	/*  Replicate first line
	 * */
	wvaz = cmfks.iwvs*cmfks.iazs;
	for( idx = 1; idx <= cmfks.iwvs; idx++ ){
	    Buffer[wvaz + idx] = Buffer[idx];
	}

	/*  Create contour map 
	 * */
	fstrncpy( label, 120, "Broadband Wavenumber Spectrum$", 30 );
	if ( strcmp ( cmfks.kfkmethod , "MLM " ) == 0 ||
	     strcmp ( cmfks.kfkmethod , "MUS " ) == 0 ) {
	    append( label,121, "@High-Resolution Method: @",27 );
            cattemp = malloc(1+strlen(cmfks.kfkmethod)+2+1);
            strcpy(cattemp,"@");
            strcat(cattemp,cmfks.kfkmethod);
            strcat(cattemp,"$@");
	    append( label,121, cattemp, 1+strlen(cmfks.kfkmethod)+2+1 );
            free(cattemp);
	}
        sprintf(cnumber,"%12.5g", cmfks.rkhor );
	append( label,121, "@Maximum Radial Wavenumber: @",30 );
        cattemp = malloc(1+strlen(cnumber)+2+1);
        strcpy(cattemp, "@");
        strcat(cattemp,cnumber);
        strcat(cattemp,"$@");
	append( label,121, cattemp, 1+strlen(cnumber)+2+1 );
        free(cattemp);
	fprintf( MUNOUT, "title: %s \n", cmfks.ktitle );
	pltplr( buffer, cmfks.iazs + 1, cmfks.iwvs, cmfks.nlcontour, cmfks.scalng ,
	 lprint , label,121, cmfks.ktitle,81 );


	if( cmfks.lwr ){

	    /*  Call full-rank wavenumber spectrum calculation routine        
	     * */
	    fkevalr( scm, xr, yr, ns, cmfks.idimsq, cmfks.rkhor, buffer );


	    /*  If the type is mlm, invert the wavenumber spectrum  
	     * */
	    if ( strcmp ( cmfks.kfkmethod , "MLM " ) == 0 ||
		 strcmp ( cmfks.kfkmethod , "MUS " ) == 0 ) {
		for( idx = 1; idx <= (ipow(cmfks.idimsq,2)); idx++ ){
		    Buffer[idx] = 1./Buffer[idx];
		}
	    }

	    /*  Raise the wavenumber spectrum to the indicated power           
	     * */
	    if( cmfks.iexp != 1 ){
		for( idx = 1; idx <= (ipow(cmfks.idimsq,2)); idx++ ){
		    Buffer[idx] = powi(Buffer[idx],cmfks.iexp);
		}
	    }

	    /*  Put the square-mode contour data in sacmem                            
	     * */
	    jdfl = cmdfm.ndfl + 1;
	    nckofbbfk = nstrlensp( cmfks.kofbbfk,MCPFN+1 );
	    fstrncpy( kfile, 8, cmfks.kofbbfk, min(nckofbbfk,MCPFN));

        string_list_put(datafiles, kfile, 9);
	    if( *nerr != 0 )
		goto L_8888 ;

	    allamb( &cmmem, SAC_HEADER_WORDS, &Ndxhdr[jdfl], nerr );
	    if( *nerr != 0 )
		goto L_8888 ;

	    newhdr();
	    *iftype = *ixyz;
	    *nxsize = cmfks.idimsq;
	    *nysize = cmfks.idimsq;
	    *npts = *nxsize**nysize;
	    *xminimum = -cmfks.rkhor;
	    *xmaximum = cmfks.rkhor;
	    *yminimum = -cmfks.rkhor;
	    *ymaximum = cmfks.rkhor;

	    Ncomp[jdfl] = 1;
	    Nlndta[jdfl] = *npts;
	    allamb( &cmmem, Nlndta[jdfl], &cmdfm.ndxdta[jdfl - 1][0], nerr );
	    if( *nerr != 0 ){
		relamb( cmmem.sacmem, Ndxhdr[jdfl], nerr );
		goto L_8888 ;
	    }

	    /* BUFFER as returned from FKEVALR is installed into SACMEM in
	     * upside-down order.  This is because FKEVALR's result is the upside down
	     * reflection of that produced with FKEVALP  -- JYio 2/4/91
	     * */
	    *depmin = Buffer[1];
	    *depmax = *depmin;
	    isacm = cmdfm.ndxdta[jdfl - 1][0];
            Sacmem = cmmem.sacmem[isacm];
	    for( iy = 1; iy <= cmfks.idimsq; iy++ ){
		*ib = (cmfks.idimsq - iy)*cmfks.idimsq + 1;
		for( ix = 1; ix <= cmfks.idimsq; ix++ ){
		    curntval = Buffer[*ib];
		    *(Sacmem++) = curntval;
		    if( curntval < *depmin )
			*depmin = curntval;
		    if( curntval > *depmax )
			*depmax = curntval;
		    *ib = *ib + 1;
		    isacm = isacm + 1;
		}
	    }

	    putfil( jdfl, nerr );
	    if( *nerr != 0 ){
		relamb( cmmem.sacmem, Ndxhdr[jdfl], nerr );
		relamb( cmmem.sacmem, cmdfm.ndxdta[jdfl - 1][0], nerr );
		return ;
	    }

	    cmdfm.ndfl = jdfl;

	    /*  Write SAC file from SACMEM out to OS
	     * */
	    wrsac( jdfl, kfile,9, TRUE, nerr );
	    if( *nerr != 0 )
		return ;
	    relamb( cmmem.sacmem, Ndxhdr[jdfl], nerr );
	    relamb( cmmem.sacmem, cmdfm.ndxdta[jdfl - 1][0], nerr );
	    cmdfm.ndfl = cmdfm.ndfl - 1;

	}

	/*  Bye                                                                          
	 * */

L_8888:

	return ;
}

/*------------------------------------------------------------------------------
 *                                                       covmat          
 * Coded by:  D. Harris                                                  
 *            6/30/91                                                    
 * Last modified:  June 30, 1991                                         
 *            Lawrence Livermore National Laboratory                     
 *            L-205                                                      
 *            P.O. Box 808                                               
 *            Livermore, CA  94550                                       
 *            USA                                                        
 *
 *            (415) 423-0617                                             
 *
 * Copyright 1991 Regents of the University of California                
 *
 *
 * */


void 
covmat(float   **data, 
       int      *ptrs, 
       int       nch, 
       int       nsamples, 
       int       ltofilter, 
       complexf *scm, 
       int      *nerr) {

	int decrate, idx, index1, index2, iptr, jdx, jptr, jptr0, 
	 kdx, mcount, ncurrent, ns2;
	float a[MXNSECTS], buffer[100], g, output1[MXLENS][100], output2[MXLENS][100], 
	 qstates[MXLENS][MXLENS - 1], sdd[2*MXNSECTS], snn[2*MXNSECTS], 
	 states[MXLENS][2*MXNSECTS], xiyi, xiyr, xryi, xryr;

	float *const A = &a[0] - 1;
	int *const Ptrs = &ptrs[0] - 1;
	complexf *const Scm = &scm[0] - 1;
	float *const Sdd = &sdd[0] - 1;
	float *const Snn = &snn[0] - 1;


	/*      include '../../inc/mach'
	 *      include '../../inc/fks' */


	/*     data variables                                                    
	 * */

	/*     bandpass filter variables                                         
	 * */

	/*     complex analytic network variables                                
	 * */

	/*     covariance matrix variables                                       
	 * */

	/*	miscellaneous
	 * */


	/*  convert filter coeffs from commons in include file 'dfir' */
	if( ltofilter ){

		if( cmfir3.nsects == 0 || (strcmp(cmfir3.filttype,"bp") != 
		 0 && strcmp(cmfir3.filttype,"BP") != 0) ){
			fprintf( MUNOUT, "You have not run FILTERDESIGN or filter type is not BP.\n" );
			*nerr = 9999;
			goto L_9999;
			}

		jptr = 1;
		jptr0 = 1;
		for( jdx = 1; jdx <= cmfir3.nsects; jdx++ ){
			g = Sn[jptr0];
			A[jdx] = g;
			Snn[jptr] = Sn[jptr0 + 1]/g;
			Snn[jptr + 1] = Sn[jptr0 + 2]/g;
			Sdd[jptr] = Sd[jptr0 + 1];
			Sdd[jptr + 1] = Sd[jptr0 + 2];
			jptr = jptr + 2;
			jptr0 = jptr0 + 3;
			}

		}

	/*  initialize the covariance matrix                                     
	 * */
	ns2 = ipow(nch,2);
	for( idx = 1; idx <= ns2; idx++ ){
		Scm[idx] = flttocmplx( 0., 0. );
		}


	/*        if (ltofilter) */
	zero( (float*)states, 4*MXNSECTS*MXLENS );

	/*    complex analytic network                                           
	 * */
	zero( (float*)qstates, (MXLENS - 1)*MXLENS );


	/*  Main loop                                                            
	 * */
	decrate = 2;
	iptr = 1;
	mcount = 1;
L_4:
	;
	if( iptr > nsamples )
		goto L_5;
	ncurrent = min( nsamples - iptr + 1, 100 );

	/*    Filter a block                                                     
	 * */
	for( jdx = 0; jdx < nch; jdx++ ){

		if( ltofilter ){

			/*    bandpas    */
			iirfilter( data[Ptrs[jdx+1]]+iptr-1, ncurrent, a, snn, 
			 sdd, cmfir3.nsects, &states[jdx][0], buffer );

			/*    complex analytic signal representation                             
			 * */
			phaseshift( buffer, ncurrent, &output1[jdx][0], &output2[jdx][0], 
			 &qstates[jdx][0] );

			}
		else{


			/*    complex analytic signal representation                             
			 * */
			phaseshift( data[Ptrs[jdx+1]]+iptr-1, ncurrent, &output1[jdx][0], 
			 &output2[jdx][0], &qstates[jdx][0] );

			}

		}

	/*    covariance update                                                  
	 * */
	for( idx = 0; idx < ncurrent; idx++ ){

		if( mcount == decrate ){

			/*      evaluate dot products with factor of decrate decimation          
			 * */
			for( jdx = 0; jdx < nch; jdx++ ){

				index2 = jdx*nch;
				index1 = jdx + 1 ;

				for( kdx = 0; kdx < jdx + 1 ; kdx++ ){
					index2 = index2 + 1;

					xryr = output1[jdx][idx]*output1[kdx][idx];
					xryi = output1[jdx][idx]*output2[kdx][idx];
					xiyr = output2[jdx][idx]*output1[kdx][idx];
					xiyi = output2[jdx][idx]*output2[kdx][idx];

					Scm[index1] = cmplxadd(Scm[index1],flttocmplx( xryr + 
					 xiyi, xiyr - xryi ));
					Scm[index2] = cmplxcj( Scm[index1] );
					index1 = index1 + nch;

					}

				}

			mcount = 1;

			}
		else{

			mcount = mcount + 1;

			}

		iptr = iptr + 1;

		}

	goto L_4;
L_5:
	;

	/*  write out covariance matrix - PROMATLAB or MATLAB format             
	 *  (this function was not brought over from the source
	 *   Dave Harris (dbh@s18.es.llnl.gov) s18:/us/dsp/xap/src/bbfk.f)
	 * */
L_9999:
	return;
} /* end of function */

/*----------------------------------------------------------------------------   
 *                                                fkevalp                        
 *
 *
 *  Evaluates the fk spectrum on a polar grid, given the spectral                
 *    covariance matrix - uses partial decimation.                               
 *
 *  Developed by:  Dave Harris                                                   
 *
 *  Input Arguments:                                                             
 *  ----- ----------                                                             
 *
 *    SCM                  COMPLEX vector containing the spectral covariance matr
 *
 *    X                    Real array containing the x location of the station
 *                           in kilometers.                                      
 *
 *    Y                    Real array containing the y location of the station
 *                           in kilometers.                                      
 *
 *    #CHANNELS            The number of stations used in estimating the covarian
 *
 *    #AZIMUTHS            The number of samples in azimuth (must be an even numb
 *
 *    MAX_WAVENUMBER       The maximum wavenumber (radius) to be evaluated (cycle
 *
 *    #WAVENUMBERS         The number of wavenumber samples                      
 *
 *  Output Arguments:                                                            
 *  ------ ----------                                                            
 *
 *    FKS                  Real array containing the wavenumber spectrum         
 *
 *
 *  Linkage:  None                                                               
 * */

#define LENRXY  ( MXLENS * MXLENS )

void 
fkevalp(complexf *scm, 
        float    *x, 
        float    *y, 
        int       nch, 
        int       naz, 
        double    wvnum, 
        int       iwvs, 
        float    *fks,
        float     fkmax, 
        float     fkmax_az, 
        float     fkmax_wav, 
        int       maxflag) {

	int half, idx, ic, id, ifks, jdx, mdx, ndx, offset;
	float azimuth, caz, dec[LENRXY], deltaz, deltwv, des[LENRXY], doti, 
	 doti2, dotr, dotr2, ec[LENRXY], es[LENRXY], pi, ri[LENRXY], ri2[LENRXY], 
	 rr[LENRXY], rr2[LENRXY], s0, saz, t, theta, twopi, zx[LENRXY], zy[LENRXY];

	float *const Dec = &dec[0] - 1;
	float *const Des = &des[0] - 1;
	float *const Ec = &ec[0] - 1;
	float *const Es = &es[0] - 1;
	float *const Fks = &fks[0] - 1;
	float *const Ri = &ri[0] - 1;
	float *const Ri2 = &ri2[0] - 1;
	float *const Rr = &rr[0] - 1;
	float *const Rr2 = &rr2[0] - 1;
	complexf *const Scm = &scm[0] - 1;
	float *const X = &x[0] - 1;
	float *const Y = &y[0] - 1;
	float *const Zx = &zx[0] - 1;
	float *const Zy = &zy[0] - 1;

        char bbfk_amp[20], bbfk_wvnbr[20], bbfk_bazim[20];

        int nerr;
        


	/*  Zeroth order term, precomputed since it is invariant                         
	 * */
	s0 = 0.;
	for( idx = 1; idx <= nch; idx++ ){
		s0 = s0 + cmplxtof( Scm[idx + (idx - 1)*nch] );
		}

	/*  Set up coarray samples and reorder covariance matrix into a vector           
	 * */
	ic = 0;
	for( mdx = 1; mdx <= (nch - 1); mdx++ ){
		for( ndx = mdx + 1; ndx <= nch; ndx++ ){
			ic = ic + 1;
			Zx[ic] = X[mdx] - X[ndx];
			Zy[ic] = Y[mdx] - Y[ndx];
			Rr[ic] = cmplxtof( Scm[mdx + (ndx - 1)*nch] );
			Ri[ic] = aimag( Scm[mdx + (ndx - 1)*nch] );
			}
		}

	/*  Definition of constants                                                      
	 * */
	pi = 3.14159265;
	twopi = 2.*pi;
	deltaz = twopi/((float)( naz ) - 1.);
	deltwv = twopi*wvnum/((float)( iwvs ) - 1.);
	azimuth = 0.;
	half = naz/2;
	offset = half*iwvs;

	ifks = 1;
	for( idx = 1; idx <= half; idx++ ){

		/*    Set up scanning vector and incremental scanning vector                     
		 *      Real and imaginary parts treated separately                              
		 * */
		caz = cos( azimuth );
		saz = sin( azimuth );
		for( jdx = 1; jdx <= ic; jdx++ ){
			theta = deltwv*(caz*Zy[jdx] + saz*Zx[jdx]);
			Dec[jdx] = cos( theta );
			Des[jdx] = sin( theta );
			Ec[jdx] = 1.0;
			Es[jdx] = 0.0;
			Rr2[jdx] = Rr[jdx]*Dec[jdx] - Ri[jdx]*Des[jdx];
			Ri2[jdx] = Rr[jdx]*Des[jdx] + Ri[jdx]*Dec[jdx];
			t = Dec[jdx]*Dec[jdx] - Des[jdx]*Des[jdx];
			Des[jdx] = Dec[jdx]*Des[jdx] + Des[jdx]*Dec[jdx];
			Dec[jdx] = t;
			}

		jdx = 0;
		/*                       Loop to compute samples along radial line                */
L_33:
		;

		/*  Dot products real and imaginary                                              
		 * */
		dotr = 0.;
		doti = 0.;
		dotr2 = 0.;
		doti2 = 0.;
		for( id = 1; id <= ic; id++ ){

			dotr = dotr + Rr[id]*Ec[id];
			doti = doti + Ri[id]*Es[id];
			dotr2 = dotr2 + Rr2[id]*Ec[id];
			doti2 = doti2 + Ri2[id]*Es[id];

			/*    Increment scanning vector elements                                         
			 * */
			t = Ec[id]*Dec[id] - Es[id]*Des[id];
			Es[id] = Ec[id]*Des[id] + Es[id]*Dec[id];
			Ec[id] = t;

			}

		/*    Store FK value at current azimuth and at that azimuth + 180                
		 * */
		Fks[ifks] = s0 + 2.*(dotr - doti);

                if(maxflag){
                  if (Fks[ifks] > fkmax){
                    fkmax = Fks[ifks];
                    fkmax_az = azimuth;
                    fkmax_wav = (float)jdx * deltwv;
                  }
		}else if (Fks[ifks] < fkmax) {
                    fkmax = Fks[ifks];
                    fkmax_az = azimuth;
                    fkmax_wav = (float)jdx * deltwv;
		  }

		Fks[ifks + offset] = s0 + 2.*(dotr + doti);
                if(maxflag){
                  if (Fks[ifks + offset] > fkmax) {
                    fkmax = Fks[ifks + offset];
                    fkmax_az = azimuth + pi;
                    fkmax_wav = (float)jdx * deltwv;
		  }
		}else if (Fks[ifks + offset] < fkmax) {
                    fkmax = Fks[ifks + offset];
                    fkmax_az = azimuth + pi;
                    fkmax_wav = (float)jdx * deltwv;
		  }

		ifks = ifks + 1;

		Fks[ifks] = s0 + 2.*(dotr2 - doti2);
                if(maxflag){
                  if (Fks[ifks] > fkmax){
                    fkmax = Fks[ifks];
                    fkmax_az = azimuth;
                    fkmax_wav = (float)(jdx+1) * deltwv;
                  }
	        }else if (Fks[ifks] < fkmax){
                    fkmax = Fks[ifks];
                    fkmax_az = azimuth;
                    fkmax_wav = (float)(jdx+1) * deltwv;
		}

		if( !(jdx == 0) ){
			Fks[ifks + offset - 2] = s0 + 2.*(dotr2 + doti2);
                        if(maxflag){
                          if (Fks[ifks + offset - 2] > fkmax) {
                            fkmax = Fks[ifks + offset - 2];
                            fkmax_az = azimuth + pi;
                            fkmax_wav = (float)(jdx+1) * deltwv;
			  }
		        }else if(Fks[ifks + offset - 2] < fkmax){
                            fkmax = Fks[ifks + offset - 2];
                            fkmax_az = azimuth + pi;
                            fkmax_wav = (float)(jdx+1) * deltwv;
			}
		}
		ifks = ifks + 1;
		jdx = jdx + 2;

		if( jdx >= iwvs ){
			dotr2 = 0.;
			doti2 = 0.;
			for( id = 1; id <= ic; id++ ){
				dotr2 = dotr2 + Rr2[id]*Ec[id];
				doti2 = doti2 + Ri2[id]*Es[id];
				}
			Fks[ifks + offset - 1] = s0 + 2.*(dotr2 + doti2);
                        if(maxflag){
                          if (Fks[ifks + offset - 1] > fkmax){
                            fkmax = Fks[ifks + offset - 1];
                            fkmax_az = azimuth + pi;
                            fkmax_wav = (float)jdx * deltwv;
			  }
		        }else if(Fks[ifks + offset - 1] < fkmax){
                            fkmax = Fks[ifks + offset - 1];
                            fkmax_az = azimuth + pi;
                            fkmax_wav = (float)jdx * deltwv;
			}
			goto L_36;
			}

		goto L_33;
L_36:
		;

		azimuth = azimuth + deltaz;

		}

/* convert from radians to degrees */
/* really the back azimuth */

        fkmax_az = fkmax_az * 180. / pi;
        fkmax_wav = fkmax_wav/(2*pi);

        printf("FK peak =    %13.4g \nBack Azimuth =%7.4g(degrees)\nWave Number =   %10.6g(cycles/kilometer)\n", 
                fkmax, fkmax_az, fkmax_wav);

        sprintf(bbfk_amp,"%13.4g",fkmax);
        sprintf(bbfk_wvnbr,"%10.6g",fkmax_wav);
        sprintf(bbfk_bazim,"%7.4g",fkmax_az);

        setbbv("bbfk_wvnbr ",bbfk_wvnbr,&nerr,10,19);
        setbbv("bbfk_bazim ",bbfk_bazim,&nerr,10,19);
        setbbv("bbfk_amp ",bbfk_amp,&nerr,8,19);

	return;
} /* end of function */


/*----------------------------------------------------------------------         
 *
 *                                                FKEVALR                        
 * Possible Bug:  This routine yields a result which appears to be
 * the up-side-down reverse of that produced by FKEVALP.
 * This discrepancy is compensated for in BBFK, by reverse way in which
 * data is written out to SAC memory.   JYio 2/4/91
 *
 *
 *  Evaluates the fk spectrum on a rectangular grid, given the spectral          
 *    covariance matrix.                                                         
 *
 *  Developed by:  Terri Quinn and Dave Harris                                   
 *
 *  Input Arguments:                                                             
 *  ----- ----------                                                             
 *
 *    SCM                  spectral covariance matrix                            
 *
 *    X                    Real array containing the array x locations 
 *			  (kilometers)
 *
 *    Y                    Real array containing the array y location 
 *			  (kilometers)
 *
 *    NCH		  Number of stations
 *
 *    NSAMPS 	          The number of samples in wave space for evaluation    
 *
 *    MAX_WAVENUMBER       The maximum wavenumber (radius) to be evaluated 
 *			  (cycles)
 *
 *
 *  Output Arguments:                                                            
 *  ------ ----------                                                            
 *
 *    FKS                  Real array containing the wavenumber spectrum         
 *
 *
 *  Linkage:  none                                                               
 *
 * */

void 
fkevalr(complexf *scm, 
        float    *x, 
        float    *y, 
        int       nch, 
        int       nsamps, 
        double    wvnum, 
        float    *fks) {

	int halfns, idx, ic, id, ifks, ifksr, jdx, mdx, maxfks, ndx;
	float dec[LENRXY], deltwv, des[LENRXY], doti, dotr, ec[LENRXY], 
	 es[LENRXY], fks0, ri[LENRXY], rr[LENRXY], t, theta, twopi, wv, 
	 zx[LENRXY], zy[LENRXY];

	float *const Dec = &dec[0] - 1;
	float *const Des = &des[0] - 1;
	float *const Ec = &ec[0] - 1;
	float *const Es = &es[0] - 1;
	float *const Fks = &fks[0] - 1;
	float *const Ri = &ri[0] - 1;
	float *const Rr = &rr[0] - 1;
	complexf *const Scm = &scm[0] - 1;
	float *const X = &x[0] - 1;
	float *const Y = &y[0] - 1;
	float *const Zx = &zx[0] - 1;
	float *const Zy = &zy[0] - 1;






	/*  Zeroth order term, precomputed since it is invariant                         
	 * */
	fks0 = 0.;
	for( idx = 1; idx <= nch; idx++ ){
		fks0 = fks0 + cmplxtof( Scm[idx + (idx - 1)*nch] );
		}

	/*  Set up coarray samples and reorder covariance matrix into a vector           
	 * */
	ic = 0;
	for( mdx = 1; mdx <= (nch - 1); mdx++ ){
		for( ndx = mdx + 1; ndx <= nch; ndx++ ){
			ic = ic + 1;
			Zx[ic] = X[mdx] - X[ndx];
			Zy[ic] = Y[mdx] - Y[ndx];
			Rr[ic] = cmplxtof( Scm[mdx + (ndx - 1)*nch] );
			Ri[ic] = aimag( Scm[mdx + (ndx - 1)*nch] );
			}
		}

	/*  Definition of constants                                                      
	 * */
	twopi = 2.*3.14159265;
	maxfks = ipow(nsamps,2);
	deltwv = twopi*(2.*wvnum)/((float)( nsamps ) - 1.);
	wv = twopi*wvnum;

	/*    Set up incremental scanning vector                                         
	 *      Real and imaginary parts treated separately                              
	 * */
	for( jdx = 1; jdx <= ic; jdx++ ){
		theta = deltwv*Zx[jdx];
		Dec[jdx] = cos( theta );
		Des[jdx] = sin( theta );
		}

	ifks = 1;
	ifksr = maxfks;
	/* The iteration range of I is halfed, with no change in result -- J.Yio 2/4/91
	 *        DO    2 I = 1, NSAMPS  */
	halfns = nsamps/2;
	for( idx = 1; idx <= halfns; idx++ ){

		/*    Set up scanning vector                                                     
		 *      Real and imaginary parts treated separately                              
		 * */
		for( jdx = 1; jdx <= ic; jdx++ ){
			theta = Zx[jdx]*(-wv) + Zy[jdx]*(wv - (idx - 1)*deltwv);
			Ec[jdx] = cos( theta );
			Es[jdx] = sin( theta );
			}


		/*   Loop to compute samples along horizontal direction                          
		 * */
		for( jdx = 1; jdx <= nsamps; jdx++ ){

			/*  Dot products real and imaginary                                              
			 * */
			dotr = 0.;
			doti = 0.;

			for( id = 1; id <= ic; id++ ){
				dotr = dotr + Rr[id]*Ec[id];
				doti = doti + Ri[id]*Es[id];

				/*    Increment scanning vector elements
				 * */
				t = Ec[id]*Dec[id] - Es[id]*Des[id];
				Es[id] = Ec[id]*Des[id] + Es[id]*Dec[id];
				Ec[id] = t;

				}

			/*    Store FK value at current azimuth and at that azimuth + 180                
			 * */
			Fks[ifks] = fks0 + 2.*(dotr - doti);
			Fks[ifksr] = fks0 + 2.*(dotr + doti);
			ifks = ifks + 1;
			ifksr = ifksr - 1;
			if( ifksr < ifks ){
				goto L_9999;
				}

			}

		}

L_9999:
	;

	return;
} /* end of function */

/*--------------------------------------------------------------------            */
void 
eigenanal(int       nm, 
          int       n, 
          complexf *a, 
          char     *hires, 
          int       m, 
          complexf *s, 
          int      *nerr) {

#define A(I_,J_)	(*(a+(I_)*(nm)+(J_)))
#define S(I_,J_)	(*(s+(I_)*(nm)+(J_)))

	int idx, ierr, jdx, kdx;
	float w[25];
	complexf b[25][25], z[25][25];

	float *const W = &w[0] - 1;


	for( idx = 0; idx < n; idx++ ){
		for( jdx = 0; jdx < n; jdx++ ){
			b[jdx][idx] = A(jdx,idx);
			}
		}

	ceigvv( 25, n, (complexf*)b, w, (complexf*)z, &ierr );

	fputc( '\n', MUNOUT );
	fprintf( MUNOUT, "***** Listing of Eigen Values *****\n" );
	fprintf( MUNOUT, "   For high resolustion parameters:\n" );

	if( memcmp(hires,"MLM",3) == 0 || memcmp(hires,"mlm",3) == 0 ){
		fprintf( MUNOUT, "      mlm %d \n", m );
		fprintf( MUNOUT, "   BBFK computed with eigen terms [vectors & values]\n" );
		fprintf( MUNOUT, "   that have the HIGHEST%d  eigenvalues.\n", m );
		fputc( '\n', MUNOUT );
		fprintf( MUNOUT, "(*)used  ascend(MUS)  descend(MLM)          eigenvalue\n" );

		for( idx = 0; idx < n; idx++ ){
			for( jdx = 0; jdx < n; jdx++ ){
				S(jdx,idx) = flttocmplx( 0.0, 0.0 );
				for( kdx = n - m ; kdx < n; kdx++ ){
					S(jdx,idx) = cmplxadd(S(jdx,idx),cmplxmul(cmplxmul(flttocmplx(1./
					 W[kdx+1],0.),z[kdx][idx]),cmplxcj( z[kdx][jdx] )));
					}
				}
			}
		for( kdx = 1; kdx <= n; kdx++ ){
			if( kdx >= n - m + 1 ){
                                fprintf(MUNOUT," %c        %2d           %2d         %21.10f\n", '*', 
				 kdx, n - kdx + 1, W[kdx] );
				}
			else{
                                fprintf(MUNOUT," %c        %2d           %2d         %21.10f\n", ' ', 
				 kdx, n - kdx + 1, W[kdx] );
				}
			}
		*nerr = 0;

		}
	else if( memcmp(hires,"MUS",3) == 0 || memcmp(hires,"mus",3) == 0 ){
		fprintf( MUNOUT, "      mus %d \n", m );
		fprintf( MUNOUT, "   BBFK computed with eigenvectors associated\n" );
		fprintf( MUNOUT, "   with the LOWEST%d  eigenvalues.\n", m );
		fputc( '\n', MUNOUT );
		fprintf( MUNOUT, "(*)used  ascend(MUS)  descend(MLM)          eigenvalue\n" );

		for( idx = 0; idx < n; idx++ ){
			for( jdx = 0; jdx < n; jdx++ ){
				S(jdx,idx) = flttocmplx( 0.0, 0.0 );
				for( kdx = 0; kdx < m; kdx++ ){
					S(jdx,idx) = cmplxadd(S(jdx,idx),cmplxmul(z[kdx][idx],cmplxcj( z[kdx][jdx] )));
					}
				}
			}
		for( kdx = 1; kdx <= n; kdx++ ){
			if( kdx <= m ){
                                fprintf(MUNOUT," %c        %2d           %2d         %21.10f\n", '*', 
				 kdx, n - kdx + 1, W[kdx] );
				}
			else{
                                fprintf(MUNOUT," %c        %2d           %2d         %21.10f\n", ' ', 
				 kdx, n - kdx + 1, W[kdx] );
				}
			}
		*nerr = 0;

		}
	else{
		*nerr = 1;

		}

	return;
#undef	S
#undef	A
} /* end of function */

