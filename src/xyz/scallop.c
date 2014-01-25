
#include <stdio.h>
#include <stdlib.h>

#include "xyz.h"

void 
scallop(float *sdata,
	int specwidth,
	int speclength,
	float freqmax,
	float fmin,
	float fmax,
	int lmean,
	float *scdata,
	int *nerr)
{
  int i, j;
  int nfull0, nfull1, nhalf0, nhalf1;
  int *index;
  float factor0, factor1;
  float *smooth0, *smooth1, *trans_sdata, *diff;

  *nerr = 0;

  if((fmax > freqmax) || (fmin > freqmax)){
    printf("Error: smoothing window frequency range > data maximum frequency\n");
    *nerr = 1;
    return;
  }

  nfull0 = fmin/(freqmax/(float)specwidth);
  nfull0 = nfull0 == 0 ? 0 : ( (nfull0 % 2) == 0 ) ? nfull0 - 1: nfull0 ;

  nhalf0 = nfull0/2;

  if ( nhalf0 < 3 ){  /* need at least three points in the smoothing window */
    printf("Error: Fmin smoothing window less than three points, resetting to 0, no smoothing\n");
    nfull0 = 0;
    nhalf0 = 0;
    fmin = 0.0;
  }


  if( fmax <= fmin ) {
    printf("Error: Fmax must be larger than Fmin\n");
    *nerr = 1;
    return;
  }

  nfull1 = fmax/(freqmax/(float)specwidth);
  nfull1 = nfull1 == 0 ? 0 : ( (nfull1 % 2) == 0 ) ? nfull1 - 1: nfull1 ;

  nhalf1 = nfull1/2;

  factor0 = nfull0 == 0 ? 0.0 : 1./(float)nfull0;
  factor1 = nfull1 == 0 ? 0.0 : 1./(float)nfull1;

  if(nfull1 > 0) {
      if((index = (int *)malloc(nfull1 * sizeof(int))) == NULL){
          printf("Error allocating index array--scallop\n");
          *nerr = 301;
          return;
      }
  } else {
      index = NULL;
  }

  if((smooth0 = (float *)malloc(specwidth*sizeof(float))) == NULL){
    printf("Error allocating smooth0--scallop\n");
    *nerr = 301;
    goto L_8888;
  }

  if((smooth1 = (float *)malloc(specwidth*sizeof(float))) == NULL){
    printf("Error allocating smooth1--scallop\n");
    *nerr = 301;
    goto L_8887;
  }

  if((trans_sdata = (float *)malloc(specwidth*sizeof(float))) == NULL){
    printf("Error allocating trans_sdata--scallop\n");
    *nerr = 301;
    goto L_8886;
  }


  if((diff = (float *)malloc(specwidth*sizeof(float))) == NULL){
    printf("Error allocating diff--scallop\n");
    *nerr = 301;
    goto L_8885;
  }

/* loop over the slices of the spectrogram */

  for (j=0; j<speclength; j++){
    for (i=0; i<specwidth; i++){
      trans_sdata[i] = sdata[i*speclength+j];
    }
    if ( nfull0 != 0 ) {
      smooth(trans_sdata,specwidth,lmean,index,nhalf0,nfull0,factor0,smooth0,nerr);
      if(*nerr != 0) break;

    }else {
      for (i=0; i<specwidth; i++) smooth0[i] = trans_sdata[i];
    }

    if ( nfull1 != 0 ) {
      smooth(trans_sdata,specwidth,lmean,index,nhalf1,nfull1,factor1,smooth1,nerr);
      if(*nerr != 0) break;

    }else {
      for (i=0; i<specwidth; i++) smooth1[i] = trans_sdata[i];
    }

    subtract(smooth0,smooth1,diff,specwidth,nerr);
    if(*nerr != 0) break;

    for (i=0; i<specwidth; i++){
      scdata[i*speclength+j] = diff[i];
    }
  }

  free(diff);

L_8885:
  free(trans_sdata);

L_8886:
  free(smooth1);

L_8887:
  free(smooth0);

L_8888:
  free(index);

  return;
  
  }


