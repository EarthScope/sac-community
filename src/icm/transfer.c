
#include <stdio.h>
#include <math.h>
#include <float.h>

#include "icm.h"
#include "hdr.h"
#include "co.h"
#include "EVRESPnames.h"

#include "cssListOps/dblPublicDefs.h"
#include "smDataIO.h"
#include "cssListOps/cssStrucs.h"

#define EPS                 1e-6

#define EQUAL(x,y) ( fabs(x - y) < EPS )

int  GetCalibCalperFromWfdisc(DBlist tree, int wfid, double *calib, double* calper );

int NearestInt( double D )
{
   double Floor = floor(D);
   double Ceil  = ceil(D);
   return (Ceil - D > D - Floor) ? (int) Floor : (int) Ceil;
}

int IsNormalized( double calper, int nfreq, double delfrq, 
const double* xre, const double* xim )
{
   double value;
   double NormFreq = 1.0 / calper;
   int j = NearestInt( NormFreq / delfrq );
   if( j < 0 || j >= nfreq ){
      printf( "\tCannot determine normalization status because NormFreq is outside bounds of transfer function.\n" );
      return 0;
   }
   value = sqrt( xre[j]*xre[j] + xim[j]*xim[j] );
   return fabs( value - 1.0 ) < 0.01;
}

double GetNormalizationFactor( int nfreq, double delfrq, 
const double* xre, const double* xim )
{
   /* This function attempts to determine a scaling factor to apply 
   to the seismogram data to account for normalized vs non-normalized 
   transfer functions, and scaled vs unscaled data.
   When the transfer function is non-normalized and the data were scaled 
   on input, the scale factor is 1/calib.
   When the transfer function is normalized and the data have not been
   scaled on input, the scale factor is calib.
   In all other cases, the scale factor is 1.0.
      
   The transfer function is determined to be normalized when its value at 
   the calper is within 0.01 of 1.0.
   The data are determined not to have been scaled if the difference 
   between calib and scale is < 0.01.
   This is because when data are scaled, SAC sets scale to 1.0, but leaves 
   the calib value olone.
   */
      
   enum NormalizationStatus{ Normalized, UnNormalized, Unknown } NormStatus;
   enum ScalingStatus{ Scaled, UnScaled, ScaleUnknown } ScaleStatus;


   double DataMultiplier = 1.0;
   DBlist tree;          /* used in search for calib, calper */
   double calib, calper; /* Used to determine whether data are scaled and
	                    whether response is normalized. */
   double EffectiveCalib;

   /* If a wfdisc file is loaded, then get the Instrument Information from
      there.  If not, set to a default value
   */
   tree = smGetDefaultTree();	

   if( !GetCalibCalperFromWfdisc(tree, *nwfid, &calib, &calper ) ){
       calib  = CALIB_UNDEF;
       calper = CALPER_UNDEF;
   }

   /*
     Attribute calper
 	Units ( "Seconds" ) 
	Null  ( "-1.0"  ) 
	Description ( "nominal calibration period" ) 
	Detail {
	    This gives the period for which calib, ncalib and calratio
	    are valid.  
	}

     Attribute calib
	Units ( "Nanometers/digital count" ) 
	Null  ( "0.0"  ) 
	Description ( "nominal calibration" ) 
	Detail {
	    This is the conversion factor that maps digital data to
	    displacement, velocity, or acceleration, depending on the
	    value of segtype or rsptype.  The factor holds true at the
	    oscillation period specified by the attribute calper.  A
	    positive value means ground motion (velocity,
	    acceleration) increasing in the component direction (up,
	    north, east) is indicated by increasing counts.  A
	    negative value means the opposite.  Calib generally
	    reflects the best calibration information available at the
	    time of recording, but refinement may be given in sensor
	    reflecting a subsequent recalibration of the instrument. 
	    See calratio.  
	}; 

	Inserted/Modified from passcal/database/data/schemas/css3.0
	Your location may vary.

    */
   /* Now determine whether response has been normalized or not...
      Calper should only be set if a CSS file was read in
    */
   if( calper <= 0 ){
      NormStatus = Unknown;
   }
   else{
      NormStatus = IsNormalized( calper, nfreq, delfrq, xre, xim ) ? 
      Normalized : UnNormalized;
   }


   /* Now determine whether scale has been applied... 
      Calib should only be set if a CSS file was read in
    */
   if( EQUAL(calib, CALIB_UNDEF) ) {
       ScaleStatus = ScaleUnknown;
   } else {
       if( EQUAL(calib, *scale ) ){
           ScaleStatus = UnScaled;
       }
       else{
           ScaleStatus = Scaled;
       }
   }
   
   EffectiveCalib = calib;
   if( NormStatus == UnNormalized && ScaleStatus == Scaled ){
       DataMultiplier = 1.0 / EffectiveCalib;
   }
   else if( NormStatus == Normalized && ScaleStatus == UnScaled ){
       DataMultiplier = EffectiveCalib;
   }
   if( ! EQUAL(DataMultiplier, 1.0) ) {
       printf( "\tWaveform multiplied by %f after deconvolution.\n", DataMultiplier );
   }
   

   return DataMultiplier;
}
/* -------------------------------------------------------------------- */


void /*FUNCTION*/ transfer(dat, npts, delta, fpfrom, ipfrom, kpfrom, 
	 kpfrom_s, fpto, ipto, kpto, kpto_s, f, iprew, sre, sim, nfft, 
	 xre, xim, nfreq, nerr)
float dat[];
int npts;
double delta;
float fpfrom[];
int ipfrom[];
char kpfrom[MAXKP][MCPFN+1];   int kpfrom_s;
float fpto[];
int ipto[];
char kpto[MAXKP][MCPFN+1];   int kpto_s;
double f[];
int *iprew;
double sre[], sim[];
int nfft;
double xre[], xim[];
int nfreq, *nerr;
{
	char errmsg[131];
	int i, j;
	float a[21];
        float nmScale = 1 ;  /* for EVALRESP: scales meters to nanometers. */
	double delfrq, denr, fac, freq;


	double *const F = &f[0] - 1;
	
	double DataMultiplier = 1.0;
	
	/*=================================================================
	 * PURPOSE:  To apply an instrument transfer function to a data set.
	 *=================================================================
	 * INPUT ARGUMENTS:
	 *=================================================================
	 * OUTPUT ARGUMENTS:
	 *=================================================================
	 * MODULE/LEVEL:  ICM/4
	 *=================================================================
	 * SUBROUTINES CALLED:
	 *=================================================================
	 * MODIFICATION HISTORY:
	 *    100325:  Cleaned up ranges in do loops
	 *    871023:  Major restructuring of instrument parameter storage.
	 *    870317:  Now passing in work arrays rather than local ones.
	 *    870219:  Added prewhiten/dewhiten for flatter spectrum
	 *             and changed order of spectral operations
	 *    861105:  Original version from K. Nakanishi's TRANSFER program.
	 *==================================================================
	 * DOCUMENTED/REVIEWED:  870317
	 *================================================================ */

	/* PROCEDURE: */
	delfrq = 1.0e0/((double)( nfft )*(double)( delta ));


	/* - Prewhiten the data if requested. */
	if( *iprew > 0 ){
	    errmsg[ 0 ] = '\0' ;
	    prewit( dat, npts, iprew, a, NULL , errmsg );
	    if( errmsg[ 0 ] )
		fprintf( stdout, "%s \n", errmsg );
	}

	/* - Deconvolve seismometer 'FROM' diretion (xre, xim) */
    
    setTransferDirection(FROM);
	dseis( nfreq, delfrq, sre, sim, fpfrom, ipfrom, kpfrom,kpfrom_s, 
	&nmScale, nerr );
	if( *nerr != 0 )
	    goto L_8888;

	printf( " Station (%s), Channel (%s)\n", kstnm, kcmpnm );
    
    /* Attempt to determine whether data have been scaled or not and if
    transfer function is normalized or not. Based on that information, 
    the data may need to be scaled. This function must be called immediately 
    after getting the deconvolution transfer function before the real and 
    imaginary arrays have been modified. */
    
    DataMultiplier = GetNormalizationFactor( nfreq, delfrq, sre, sim );
	

    /* compute transfer function 
       Input:  (xre,xim) 'FROM' Transfer direction 
       Output: (sre, sim) 1.0/ 'FROM' Transfer direction
       Set to zero if less than FLT_MIN */
       
	for( i = 1; i < nfreq; i++ ){
      denr=(powi(sre[i],2) + powi(sim[i],2));
	    if(denr <= FLT_MIN){
	      sre[i]=0.0e0;
	      sim[i]=0.0e0;
	    }
	    else{
	      denr = 1.0e0/denr;
	      sre[i] = sre[i]*denr;
	      sim[i] = -sim[i]*denr;
	    }
	}

	/* - Determine seismometer transfer function in the 'TO' direction 
	(xre, xim). */
	
    setTransferDirection(TO);
	dseis( nfreq, delfrq, xre, xim, fpto, ipto, kpto,kpto_s, 
	&nmScale, nerr );

	if( *nerr != 0 )
	    goto L_8888;


  /* 
     Apply TO and FROM Instrument responses
                   F(DATA) * TO
     DATA = F^-1(----------------)
                      FROM
     DATA is overwritten
   */

  ztransfer(dat, npts, delta, sre, sim, xre, xim, nfreq, nfft, delfrq, f);
  
	/* - Copy the transformed data back into the original data array. 
	double precieions => single precision */
	
	/* nmScale is 1 by default, but if EVALRESP is used, nmScale converts 
	to nm.  DataMultiplier is used to apply or unapply calib */
	
	for( i = 0; i < npts; i++ )
	    dat[i] = sre[i] * nmScale * DataMultiplier;
	    
	/* - Undo the effects of prewhitening if necessary. */

	if( *iprew > 0 ){
	    errmsg[ 0 ] = '\0' ;
	    dewit( dat, npts, *iprew, a, errmsg );
	    if( errmsg[ 0 ] )
		fprintf( stdout, "%s \n", errmsg );
	}


L_8888:
	return;

} /* end of function */

