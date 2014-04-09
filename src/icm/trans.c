
#include <math.h>
#include <float.h>

double taper_spectra ( double freq, 
               double fqh, 
               double fql);


void
ztransfer(float *dat, int npts, double delta, double *sre, double *sim, double *xre, double *xim, int nfreq, int nfft, double delfrq, double *F) {
  int i,j;
  double temp, freq, fac;
    /* Multiply the two transfer functions together to get a composite 
    transfer function (xre, xim)
       Input  (xre,xim): TO transfer fuction
       Input  (sre,sim): FROM transfer function
       Output (xre,xim): Composite transfer function */
	for( i = 1; i < nfreq; i++ ){
	    temp = xre[i]*sre[i] - xim[i]*sim[i];
	    xim[i] = xre[i]*sim[i] + xim[i]*sre[i];
	    xre[i] = temp;
	}

    /* Apply the taper to the composite transfer function to (xre, xim) 
       This is the FREQLIMITS option */
	for( i = 1; i < nfreq; i++ ){
	    freq = (double)( i )*delfrq;
	    fac = delfrq*taper_spectra( freq, F[1], F[0] ) * taper_spectra( freq, F[2], F[3] );
	    xre[i] *= fac;
	    xim[i] *= fac;
	}

  
	/* - Fill a complex vector (sre +i*sim) [Note reuse of these arrays] and then transform the data.
       Input  (dat):      Time Series Data
       Output (sre, sim): Time series data in Real Part, 0.0 in Imaginary Part
     */
	for( i = 0; i < npts; i++ ){
	    sre[i] = (double)( dat[i] )*(double)( delta );
	    sim[i] = 0.0e0;
	}
    /* Pad with zero from npts to nfft, nfft is a power of 2 */
	if (npts < nfft) {
	    for( i = npts; i < nfft; i++ ){
	        sre[i] = 0.0e0;
	        sim[i] = 0.0e0;
	    }
	}

    /* Transform Data using FFT to get the complex frequency response 
       Input  (sre,sim): Time series data in Real Part, 0.0 in Imaginary Part
       Output (sre,sim): Complex Freuqnecy Response */
       
	dcpft( sre, sim, nfft, 1, -1 ); 

	/* - Multiply transformed data (sre, sim) by composite transfer 
	operator (xre, xim) . 
     Input  (sre, sim): FFT of data (single-precision => double precision)
     Input  (xre, xim): Composite Transfer funtion
     Output (sre, sim): Transfered Data in Complex Frequency Space 
     Frequency Response at zero-frequency is set to 0.0e0 */
     
	for( i = 1; i < nfreq; i++ ){
	    double tempR = xre[i]*sre[i] - xim[i]*sim[i];
	    double tempI = xre[i]*sim[i] + xim[i]*sre[i];
	    sre[i] = tempR;
	    sim[i] = tempI;
	    /* Input data are real so F(N-j) = (F(j))*   */
	    if (i < nfreq-1){
          j = nfft - i;
          sre[j] = tempR;
          sim[j] = -tempI;
	    }
	}
    /* Set the Frequency Response at zero-frequency to 0.0 */
	sre[0] = 0.0;
    sim[0] = 0.0;
    
    /* Set the real part of the Frequecy Response at the Nyquist Frequency to 
    the Amplitude of the Nyquist Freuquency */
    
    sre[nfft-1] = sqrt(sre[nfft-1]*sre[nfft-1] + sim[nfft-1]*sim[nfft-1]);
	sim[nfft-1] = 0.0;

	/* - Perform the inverse transform. 
       Input  (sre,sim): Transfered Data in Complex Frequency Space 
       Output (sre,sim): Time series data equivalent of input */
       
	dcpft( sre, sim, nfft, 1, 1 );

}
