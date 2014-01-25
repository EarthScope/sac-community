/** 
 * @file   xfg.c
 * 
 * @brief  Function Generate
 * 
 */

#include <string.h>
#include <math.h>

#include "exm.h"
#include "msg.h"
#include "cpf.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "co.h"
#include "bool.h"
#include "dfm.h"

#include "bot.h"
#include "ucf.h"
#include "ssi.h"
#include "clf.h"
#include "dbh.h"
#include "dff.h"

#include "debug.h"

#define PI M_PI

/** 
 * Generate a Function and store it in memory
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   961031:  ninf and nhst were changed to norid and nevid for
 *             compatability with the CSS format.  Also, the variables
 *             *b and *e were replaced with *begin and *ennd (See hdr.h)
 *             maf
 * @date   911119:  Added data-set storage stuff.
 * @date   910516:  Fixed funcgen problem reported by George Helffrich of
 *             Carnegie Institute of Washington. Funcgen was writing
 *             one too many points in the data block (wct).
 * @date   890907:  Added unit impulse function.
 * @date   870923:  Deleted ".saf" suffix from seismogram name.
 * @date   870721:  Fixed bug involving an incorrect error message in RANDOM.
 * @date   860327:  Fixed several small bugs in SEISMOGRAM option.
 * @date   860218:  Fixed bug in TRIANGLE option.
 * @date   850801:  Changes in argument list for RDSAC.
 * @date   840120:  Added random noise generator function.
 * @date   821123:  Added a new function containing a sample data file.
 * @date   820805:  Major restructuring and addition of new functions.
 * @date   820316:  Modified sine wave generator to include frequency.
 * @date   810528:  Added LINE.
 * @date   810414:  Minor changes relating to common block reorganization.
 * @date   810120:  Changed to error message retrieval from disk.
 * @date   801208:  Changed file generation to use CRSAC.
 * @date   800728:  Added IMPULSE.
 *
 */
void
xfg(int *nerr) {
	char kfile[MCPFN+1];
	int iseed, j, j1, jcmp, jdfl, junk, ndx1, 
	 ndx2, ndxh, nlen, ntused;
	static int nra ;
	double arg, con, del;
    double arg0;
        float *Sacmem;

	*nerr = 0;
  memset(kfile, 0, sizeof(kfile));
  ndx1 = ndx2 = 0;

	while ( lcmore( nerr ) ){

		/* -- select one of the possible functions. */
		if( lclist( (char*)kmexm.kfgtp,9, cmexm.nfgtp, &cmexm.ifgtp ) ){
			if( cmexm.ifgtp == 5 ){
				if( lcra( 0, 2, cmexm.fgsico, &nra ) )
				{ /* do nothing */ }
			}
			else if( cmexm.ifgtp == 6 ){
				if( lcra( 0, 2, cmexm.fglico, &nra ) )
          { /* do nothing */ }
			}
			else if( cmexm.ifgtp == 7 ){
				if( lcra( 0, 3, cmexm.fgquco, &nra ) )
				{ /* do nothing */ }
			}
			else if( cmexm.ifgtp == 8 ){
				if( lcra( 0, 4, cmexm.fgcuco, &nra ) )
				{ /* do nothing */ }
			}
			else if( cmexm.ifgtp == 9 ){
				if( lcra( 0, 2, cmexm.fgraco, &nra ) )
				{ /* do nothing */ }
			}
			else if( cmexm.ifgtp == 12 ){
				if( lcra( 0, 1000, cmexm.fgistr, &nra ) )
				{ /* do nothing */ }
			}

		}

		/* -- "DELTA v":  change sampling interval. */
		else if( lkreal( "D$",3, &cmexm.fgdel ) )
		{ /* do nothing */ }

		/* -- "BEGIN v":  change beginning value. */
		else if( lkreal( "BE$",4, &cmexm.fgbeg ) )
		{ /* do nothing */ }

		/* -- "NPTS n":  change number of data points in function. */
		else if( lkint( "N$",3, &cmexm.nfgpts ) )
		{ /* do nothing */ }

		/* -- Bad syntax. */
		else{
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();
		}
	}

	/* - The above loop is over when one of two conditions has been met:
	 *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
	 *   (2) All the tokens in the command have been successfully parsed. */
    DEBUG("\n");
	if( *nerr != 0 )
		return ;

	/* EXECUTION PHASE: */

	/* - Initialize memory manager and data file list. */
        cleardfl( nerr );
	if( *nerr != 0 ) goto L_8888;
	/* - Create room in memory for generated function.
	 *   All data currently in memory is destroyed. */

	/* -- Random number generator function may generate more than one file.
	 *    (Force number of data points to be even in this case.) */
	if( cmexm.ifgtp == 9 ){
		cmexm.nfgpts = 2*((cmexm.nfgpts - 1)/2 + 1);
		cmdfm.ndfl = min( MDFL, (int)( Fgraco[1] + 0.1 ) );
		jcmp = 1;
		for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){

			/* --- Get next data-set storage index and save some related information */
			Ndsndx[jdfl] = 1 ;

			if( jdfl <= 9 ){
                                sprintf(kfile,"%s%1d","RANDOM0", jdfl );
			}
			else{
                                sprintf(kfile,"%s%2d","RANDOM", jdfl );
			}

            string_list_put(datafiles, kfile, MCPFN+1);
			if( *nerr != 0 )
				goto L_8888;
			crsac( jdfl, jcmp, cmexm.nfgpts, &ndxh, &ndx1, &ntused, nerr );
			if( *nerr != 0 )
				goto L_8888;
		} /* end for */

		/* -- All other functions generate one file only.
		 * -- Sample seismogram is unique in length. */
	} /* end if ( cmexm.ifgtp == 9 ) */
	else{
        DEBUG("\n");
		cmdfm.ndfl = 1;
		jdfl = 1;
		jcmp = 1;
        string_list_put(datafiles, kmexm.kfgtp[cmexm.ifgtp-1], 9);
		if( *nerr != 0 )
			goto L_8888;
		/* -- Do the rest later if generating a seismogram. */
		if( cmexm.ifgtp == 10 )
			goto L_1600;
		crsac( jdfl, jcmp, cmexm.nfgpts, &ndxh, &ndx1, &junk, nerr );
		if( *nerr != 0 )
			goto L_8888;

		/* -- Get next data-set storage index and save all related information */
		Ndsndx[jdfl] = 1 ;
	}

	/* - Set up new header. */

L_1600:
	/* changed fg iftype to time series */
	/*	*iftype = *ixy; */
	*iftype = *itime; 
	*leven = TRUE;
/*      *ninf = 0;              ninf and nhst are becoming norid and
        *nhst = 0;              nevid respectively.  maf 961031 */
	*begin = cmexm.fgbeg;
	*delta = cmexm.fgdel;
	*npts = cmexm.nfgpts;
	*ennd = *begin + (float)( *npts - 1 )**delta;

        fstrncpy( kevnm, 17, "FUNCGEN: ", 9);
        fstrncpy( kevnm+9, 17-10, kmexm.kfgtp[cmexm.ifgtp - 1], 
                          strlen(kmexm.kfgtp[cmexm.ifgtp - 1]));

	/*     ndx2=ndx1+nfgpts
	 * - Write one less point to data block (modificatoin 5/16/91) */
/*	ndx2 = ndx1 + cmexm.nfgpts - 1; */
	ndx2 = cmexm.nfgpts - 1;

	/* - Generate function. */
	switch( cmexm.ifgtp ){
		case 1: goto L_2010;
		case 2: goto L_2020;
		case 3: goto L_2030;
		case 4: goto L_2040;
		case 5: goto L_2050;
		case 6: goto L_2060;
		case 7: goto L_2070;
		case 8: goto L_2080;
		case 9: goto L_2090;
		case 10: goto L_2100;
		case 11: goto L_2110;
		case 12: goto L_2120;
	}

	/* -- Impulse. */

L_2010:
        Sacmem = cmmem.sacmem[ndx1];

	for( j = 0; j <= ndx2; j++ ){
                *(Sacmem++) = 0.;
	}
        *(cmmem.sacmem[ndx1]+(ndx2/2)) = 1.;
        DEBUG("\n");
	goto L_8888;

	/* -- Step function. */

L_2020:
	j1 = *npts/2;
        Sacmem = cmmem.sacmem[ndx1];

	for( j = 0; j <= j1; j++ ){
                *(Sacmem++) = 0.;
	}

	for( j = j1 + 1; j <= ndx2; j++ ){
                *(Sacmem++) = 1.;
	}
	goto L_8888;

	/* -- Boxcar. */

L_2030:
	j1 = *npts/3;
        Sacmem = cmmem.sacmem[ndx1];
	for( j = 0; j <= j1; j++ ){
                *(Sacmem++) = 0.;
	}
	for( j = j1 + 1; j <= (2*j1); j++ ){
                *(Sacmem++) = 1.;
	}
	for( j = 2*j1 + 1; j <= ndx2; j++ ){
                *(Sacmem++) = 0.;
	}
	goto L_8888;

	/* -- Triangle function. */

L_2040:
	j1 = *npts/4;
        Sacmem = cmmem.sacmem[ndx1];
	for( j = 0; j <= j1; j++ ){
                *(Sacmem++) = 0.;
	}
	con = 1./(float)( j1 );
	for( j = j1 + 1; j <= (2*j1); j++ ){
                *Sacmem = *(Sacmem - 1) + con;
                Sacmem++;
	}
	for( j = 2*j1 + 1; j <= (3*j1); j++ ){
                *Sacmem = *(Sacmem - 1) - con;
                Sacmem++;
	}
	for( j = 3*j1 + 1; j <= ndx2; j++ ){
                *(Sacmem++) = 0.;
	}
	goto L_8888;

	/* -- Sine wave. */

L_2050:
	del = 2.*PI*Fgsico[1]**delta;
	arg0 = 2.*PI*(Fgsico[1]**begin + Fgsico[2]/360.);
        Sacmem = cmmem.sacmem[ndx1];
	for( j = 0; j <= ndx2; j++ ){
        arg = arg0 + j * del;
		*(Sacmem++) = sin( arg );
	}
	goto L_8888;

	/* -- Linear equation (Coefficients are linear multiplier and constant [ax+b]). */

L_2060:
    Sacmem = cmmem.sacmem[ndx1];
	for( j = 0; j <= ndx2; j++ ){
        arg = *begin + j * *delta;
		*(Sacmem++) = Fglico[1]*arg + Fglico[2];
	}
	goto L_8888;

	/* -- Quadratic equation. */

L_2070:
    Sacmem = cmmem.sacmem[ndx1];
	for( j = 0; j <= ndx2; j++ ){
        arg = *begin + j * *delta;
		*(Sacmem++) = Fgquco[1]*powi(arg,2) + Fgquco[2]*arg + Fgquco[3];
	}
	goto L_8888;

	/* -- Cubic equation. */

L_2080:
    Sacmem = cmmem.sacmem[ndx1];
	for( j = 0; j <= ndx2; j++ ){
        arg = *begin + j * *delta;
		*(Sacmem++) = Fgcuco[1]*powi(arg,3) + Fgcuco[2]*powi(arg,2) + 
		 Fgcuco[3]*arg + Fgcuco[4];
	}
	goto L_8888;

	/* -- Random number generator.
	 *    (Call to putfil before getfil is to set up each header.) */

L_2090:
	iseed = (int)( Fgraco[2] + 0.1 );
	for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){
		putfil( jdfl, nerr );
		getfil( jdfl, TRUE, &nlen, &ndx1, &ndx2, nerr );
		*user0 = (float)( iseed );
                Sacmem = cmmem.sacmem[ndx1];
		for( j = 0; j <= (nlen - 1); j += 2 ){
			gauss( &iseed, Sacmem, Sacmem + 1 );
                        Sacmem += 2;
		}
		extrma( cmmem.sacmem[ndx1], 1, *npts, depmin, depmax, depmen );
		putfil( jdfl, nerr );
	}
	goto L_8888;

	/* -- Sample data file. */

L_2100:
	zbasename( kfile,MCPFN+1 );
	crname( kfile,MCPFN+1, KDIRDL, "seismogram",11, nerr );
	if( *nerr != 0 )
		goto L_8888;

	rdsac( jdfl, kfile,MCPFN+1, TRUE, TRUE, &nlen, &ndxh, &ndx1, &ndx2, nerr );
	if( *nerr != 0 )
		goto L_8888;
  string_list_pop(datafiles);
	Ndsndx[jdfl] = 1 ;

	goto L_8888;

	/* -- Unit impulse (contains a 1 as the first data point. */

L_2110:
        Sacmem = cmmem.sacmem[ndx1];
        *(Sacmem++) = 1.;
	for( j = 1; j <= ndx2; j++ ){
		*(Sacmem++) = 0.;
	}
	goto L_8888;

	/* -- impulse string  */

L_2120:
        Sacmem = cmmem.sacmem[ndx1];

	for( j = 0; j <= ndx2; j++ ){
                *(Sacmem++) = 0.;
	}

	/* subtracted 1 from fgistr to convert 
	   FORTRAN style input arguments to C compatible 
	   PG 1/4/01. */
        Sacmem = cmmem.sacmem[ndx1];
        for( j = 0; j <= nra-1; j++){
          if(((int)(cmexm.fgistr[j]+RNDOFF) < *npts) &&
             ((int)(cmexm.fgistr[j]+RNDOFF) > 0))
           *(Sacmem + (int)(cmexm.fgistr[j]-1+RNDOFF)) = 1.0;
	}

	goto L_8888;

L_8888:
	if ( cmexm.ifgtp != 9 && cmexm.ifgtp != 10 && *nerr == 0) {
	    extrma( cmmem.sacmem[ndx1], 1, *npts, depmin, depmax, depmen );
	    putfil ( jdfl , nerr ) ;
	}

	if ( *nerr == 0 ) {	/* if no error occured */
	    cmdfm.nfilesFirst = 0 ;
	    cmdfm.nreadflag = LOW ;
	    cmdfm.lread = TRUE ;
	    sacToSeisMgr ( TRUE , FALSE , TRUE , nerr ) ;
	    cmdfm.lread = FALSE ;
	}
    DEBUG("\n");
	return;

} /* end of function */

