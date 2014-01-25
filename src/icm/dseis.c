
#include <stdio.h>
#include <string.h>

#include "icm.h"
#include "bool.h"


#include "bot.h"

void dbaseResponse(int nfreq, double delfrq, double* xre, double* xim, 
                   float* nmScale, int* nerr);

#ifdef NDC
int ndcTransfer( char *dir, char *dfile, double dt, int nfr,
                 double *xre, double *xim, int *nerr );
#endif


void /*FUNCTION*/ dseis(nfreq, delfrq, xre, xim, fp, ip, kp, kp_s, nmScale, nerr)
int nfreq;
double delfrq, xre[], xim[];
float fp[];
int ip[];
char kp[MAXKP][MCPFN+1];   int kp_s;
float *nmScale ;
int *nerr;
{
	int idx;
	float *const Fp = &fp[0] - 1;
	int *const Ip = &ip[0] - 1;


	/*   .....Branching routine to apply the individual instrument responses.
	 *
	 * MODIFICATIONS:
	 *    920420:  Changed adjustable array specifier from "1" to "*".
	 *    900409:  Added LNN instrument type.
	 * */
	*nerr = 0;

	for( idx = 0; idx < nfreq; idx++ ){
	    xre[idx] = 1.0e0;
	    xim[idx] = 0.0e0;
	}

	if( strncmp(kp[ 0 ],"ACC",3) == 0 ){
	    acc( nfreq, delfrq, xre, xim );
	}

	else if( strncmp(kp[ 0 ],"POLEZERO",8) == 0 ){
	    polezero( nfreq, delfrq, xre, xim, kp[ 1 ],kp_s, nerr );
	}
    else if ( strncmp(kp[0], "FAP", 3) == 0 ) {
      frequency_amplitude_phase( nfreq, delfrq, xre, xim, kp[1], kp_s, nerr);
    }
	else if( strncmp(kp[ 0 ],"BBDISP",6) == 0 ){
	    bbdisp( nfreq, delfrq, xre, xim );
	}

	else if( strncmp(kp[ 0 ],"BBVEL",5) == 0 ){
	    bbvel( nfreq, delfrq, xre, xim );
	}

	else if( strncmp(kp[ 0 ],"BENBOG",6) == 0 ){
	    benbog( nfreq, delfrq, xre, xim );
	}

	else if( strncmp(kp[ 0 ],"DSS",3) == 0 ){
	    dss( nfreq, delfrq, xre, xim );
	}

	else if( strncmp(kp[ 0 ],"DWWSSN",6) == 0 ){
	    dwwssn( nfreq, delfrq, xre, xim );
	}

	else if( strncmp(kp[ 0 ],"EKALP6",6) == 0 ){
	    ekalp6( nfreq, delfrq, xre, xim );
	}

	else if( strncmp(kp[ 0 ],"EKASP2",6) == 0 ){
	    ekasp2( nfreq, delfrq, xre, xim );
	}

	else if( strncmp(kp[ 0 ],"ELMAG",5) == 0 ){
	    elmag( nfreq, delfrq, xre, xim, Fp[1], Fp[2], nerr );
	}

	else if( strncmp(kp[ 0 ],"IW",2) == 0 ){
	    eyeomg( nfreq, delfrq, xre, xim, Ip[1] );
	}

	else if( strncmp(kp[ 0 ],"GBALP",5) == 0 ){
	    gbalp( nfreq, delfrq, xre, xim );
	}

	else if( strncmp(kp[ 0 ],"GBASP",5) == 0 ){
	    gbasp( nfreq, delfrq, xre, xim );
	}

	else if( strncmp(kp[ 0 ],"GENERAL",7) == 0 ){
	    general( nfreq, delfrq, xre, xim, Ip[1], Fp[1], Fp[3], Fp[2] );
	}

	else if( strncmp(kp[ 0 ],"GSREF",5) == 0 ){
	    gsref( nfreq, delfrq, xre, xim );
	}

	else if( strncmp(kp[ 0 ],"HFSLPWB",7) == 0 ){
	    hfslpwb( nfreq, delfrq, xre, xim );
	}

	else if( strncmp(kp[ 0 ],"S750",4) == 0 ){
	    hs3( nfreq, delfrq, xre, xim );
	}

	else if( strncmp(kp[ 0 ],"LLL",3) == 0 ){
	    modcase( TRUE, kp[ 1 ], MCPFN, kp[ 1 ] );
	    lll( nfreq, delfrq, xre, xim, kp[ 1 ],kp_s, Fp[1], Fp[3], nerr );
	}

	else if( strncmp(kp[ 0 ],"LLSN",4) == 0 ){
	    llsn( nfreq, delfrq, xre, xim );
	}

	else if( strncmp(kp[ 0 ],"LRSMLP",6) == 0 ){
	    lrsmlp( nfreq, delfrq, xre, xim );
	}

	else if( strncmp(kp[ 0 ],"LRSMSP",6) == 0 ){
	    lrsmsp( nfreq, delfrq, xre, xim );
	}

	else if( strncmp(kp[ 0 ],"NORESS",6) == 0 ){
	    modcase( TRUE, kp[ 1 ], MCPFN, kp[ 1 ] );
	    noress( nfreq, delfrq, xre, xim, kp[ 1 ],kp_s );
	}

	else if( strncmp(kp[ 0 ],"NORESSHF",8) == 0 ){
	    noresshf( nfreq, delfrq, xre, xim );
	}

	else if( strncmp(kp[ 0 ],"OLDBB",5) == 0 ){
	    oldbb( nfreq, delfrq, xre, xim );
	}

	else if( strncmp(kp[ 0 ],"OLDKIR",6) == 0 ){
	    oldkir( nfreq, delfrq, xre, xim );
	}

	else if( strncmp(kp[ 0 ],"PORTABLE",8) == 0 ){
	    portable( nfreq, delfrq, xre, xim, Fp[1], Fp[3], Fp[4] );
	}

	else if( strncmp(kp[ 0 ],"PTBLLP",6) == 0 ){
	    ptbllp( nfreq, delfrq, xre, xim );
	}

	else if( strncmp(kp[ 0 ],"REDKIR",6) == 0 ){
	    redkir( nfreq, delfrq, xre, xim );
	}

	else if( strncmp(kp[ 0 ],"REFTEK",6) == 0 ){
	    reftek( nfreq, delfrq, xre, xim, Fp[1], Fp[3], Fp[4], Fp[6] );
	}

	else if( strncmp(kp[ 0 ],"LNN",3) == 0 ){
	    if( strncmp(kp[ 1 ],"BB",2) == 0 ){
		reftek( nfreq, delfrq, xre, xim, 15.0, 0.7, 6.0, 0.0 );
	    }

	    else{
		reftek( nfreq, delfrq, xre, xim, 1.0, 0.7, 32.0, 0.0 );
	    }

	}

	else if( strncmp(kp[ 0 ],"RSTN",4) == 0 ){
	    modcase( TRUE, kp[ 1 ], MCPFN, kp[ 1 ] );
	    rstn( nfreq, delfrq, xre, xim, kp[ 1 ],kp_s, nerr );
	}

	else if( strncmp(kp[ 0 ],"SANDIA3",7) == 0 ){
	    snla3( nfreq, delfrq, xre, xim );
	}

	else if( strncmp(kp[ 0 ],"SANDIA",6) == 0 ){
	    modcase( TRUE, kp[ 1 ], MCPFN, kp[ 1 ] );
	    sandia( nfreq, delfrq, xre, xim, kp[ 1 ],kp_s, nerr );
	}

	else if( strncmp(kp[ 0 ],"SRO",3) == 0 ){
	    modcase( TRUE, kp[ 1 ], MCPFN, kp[ 1 ] );
	    sro( nfreq, delfrq, xre, xim, kp[ 1 ],kp_s );
	}

	else if( strncmp(kp[ 0 ],"VEL",3) == 0 ){
	    vel( nfreq, delfrq, xre, xim );
	}

	else if( strncmp(kp[ 0 ],"WIECH",5) == 0 ){
	    wiech( nfreq, delfrq, xre, xim );
	}

	else if( strncmp(kp[ 0 ],"WABN",4) == 0 ){
	    wabn( nfreq, delfrq, xre, xim );
	}

	else if( strncmp(kp[ 0 ],"WA",2) == 0 ){
	    wa( nfreq, delfrq, xre, xim );
	}

	else if( strncmp(kp[ 0 ],"WWLPBN",6) == 0 ){
	    wwlpbn( nfreq, delfrq, xre, xim );
	}

	else if( strncmp(kp[ 0 ],"WWSPBN",6) == 0 ){
	    wwspbn( nfreq, delfrq, xre, xim );
	}

	else if( strncmp(kp[ 0 ],"WWSP",4) == 0 ){
	    wwsp( nfreq, delfrq, xre, xim );
	}

	else if( strncmp(kp[ 0 ],"YKALP",5) == 0 ){
	    ykalp( nfreq, delfrq, xre, xim );
	}

	else if( strncmp(kp[ 0 ],"YKASP",5) == 0 ){
	    ykasp( nfreq, delfrq, xre, xim );
	}

	else if( strncmp(kp[ 0 ],"EVALRESP",8) == 0 ){
	    *nerr=EvrespGateway( nfreq, delfrq, xre, xim , nmScale , NULL );
	    if(*nerr != 0){
		printf(" No transfer function applied! \n");
	    }
	}
#ifdef NDC
	else if( !strncmp(kp[ 0 ],"FAPFILE",7) || !strncmp(kp[ 0 ],"NDC",3) ){
            char dfile[MCPFN+1], dir[MCPFN+1] = "." ;
            char *lastSlash ;

            /* check if a path is present in filename. */
            lastSlash = strrchr( kp[ 1 ] , KDIRDL ) ;
            if( lastSlash && lastSlash - kp[ 1 ] + 1 < kp_s ){
                strncpy( dir, kp[ 1 ], lastSlash - kp[ 1 ] ) ;
                strncpy( dfile, lastSlash + 1,
                         kp_s - ( lastSlash - kp[ 1 ] + 1 ) ) ;
            }
            else
               strncpy( dfile, kp[ 1 ], kp_s ) ;

            ndcTransfer( dir, dfile, delfrq, nfreq, xre, xim, nerr ) ;
	}
#endif
	else if( strncmp(kp[ 0 ],"DBASE",5) == 0 ){
           dbaseResponse( nfreq, delfrq, xre, xim, nmScale, nerr );
	}
	
} /* end of function */
