
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "co.h"
#include "xyz.h"
#include "spectrogram.h"
#include "dbh.h"
#include "debug.h"

#define LNCOR 4096
#define LNAUX 10240

void 
spcgrm(	float signal[],
	int lfft,
	int iorfft,
	char *type,
	int *order,
	double delta,
	int wlen,
	int sfft,
	int lncorwin,
	int ncorwin,
	float lencorwin,
	char *wintype,
	char *scale,
	float ridge_fac,
	int *err)
{
	int i;
        int nfft = 0;
        int nlags = 0;

        char errmsg[131];

        float samfrq;
        int cwinln;

        float *ac, *aux;
	float *const Signal = &signal[0] - 1;

        *err = 0;
        strcpy(errmsg,"                    ");
        UNUSED(iorfft);
        UNUSED(lfft);

	/*  Arguments                                                                     */
	/*  Local Variables         */
	/*  Code Implementation */

       /* need to allocate ac and aux */

        if((ac = (float *)malloc(LNCOR * sizeof(float))) == NULL){
          printf("error allocating ac--spcgrm\n");
          *err = 1;
          return;
	}

        if((aux = (float *)malloc(LNAUX * sizeof(float))) == NULL){
          printf("error allocating aux--spcgrm\n");
          *err = 1;
          free(ac);
          return;
	}
       /* compute length of correlation window */

        samfrq = 1. / delta;
        cwinln = min((int)(samfrq*lencorwin+0.5),wlen);

        if(!lncorwin){ /* if number of correlation windows was not specified by user */
          ncorwin = wlen/cwinln;
          if((cwinln*ncorwin) < wlen) ncorwin++;
	}

        autcor(signal, delta, wlen, ncorwin, cwinln, wintype, scale, ac,
               &nfft, &nlags, errmsg, 131, aux, ridge_fac);               
        if(memcmp(errmsg,"        ",8) != 0) {
          printf("error calculating autocorrelation function\n %s\n",errmsg);
          goto L_8888;
	}

	/*  Apply Correlation Function */
	if( (memcmp(type,"MLM",3) == 0) || (memcmp(type,"mlm",3) == 0) ){
                if( *order > (.5*cwinln)){
                  *order = .5 * cwinln;
                  fprintf(stderr,"Warning:  ORDER begin reset to 1/2 length of correlation window.\n\
         ORDER = %d\n",*order);
		}
                mlm(ac, *order, sfft, signal, errmsg, 131, aux);
                if(memcmp(errmsg,"        ",8) != 0) {
                  printf("error calculating mlm %s\n",errmsg);
                  goto L_8888;
		}
                                                     
		}
        else if( (memcmp(type,"MEM",3) == 0) || (memcmp(type,"mem",3) == 0) ){
                if( *order > (.5*cwinln)){
                  *order = .5 * cwinln;
                  fprintf(stderr,"Warning:  ORDER begin reset to 1/2 length of correlation window.\n\
         ORDER = %d\n",*order);
		}
                mem(ac, *order, sfft, signal, errmsg, 131, aux);
                if(memcmp(errmsg,"        ",8) != 0) {
                  printf("error calculating mem %s\n",errmsg);
                  goto L_8888;
		}
                                                     
		}
	else{ /* PDS */
                if( *order > wlen){
                  *order = wlen;
                  fprintf(stderr,"Warning:  ORDER begin reset to length of window.\n\
         ORDER = %d\n",*order);
		}
                for( i = 0; i < LNCOR; i++){
                  signal[i] = ac[i];
		}

		for( i = 1; i <= (*order - 1); i++ ){
			Signal[i] = Signal[i]*(1. - (float)( i - 1 )/(float)( *order - 
			 1 ));
			}

		/*  Zero out array after index value of 'ORDER' */
		for( i = *order; i <= IMGFFT; i++ ){
			Signal[i] = 0.;
			}

		/*  Load in last values from first values */
		for( i = 2; i <= *order; i++ ){
			Signal[IMGFFT + 2 - i] = Signal[i];
			}

		/*  Calculate FFT */
		zrvfft( signal, IIMGFFT );

		}
L_8888:        

        free(aux);
        free(ac);

	return;
} /* end of function */

