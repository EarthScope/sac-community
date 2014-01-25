#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "coda.h"
#include "dbh.h"
#include "scm.h"

int unary_op (data, npts, op, arg, delta)
     float *data;
     int *npts;
     char* op;
     float arg;
     float *delta;
{
  int i,j,nhalf,n_interp;
  float *scratch,*hilbert,*dataptr,*hilbertptr;
  float sum, newdelt, newtime, begin;
  int n;

  n = *npts;
  if (!(strcmp(op,"log10"))) {
    for(i=0;i<n;i++) {
       *(data+i) = log10(*(data+i));
    }
  }
  else if (!(strcmp(op,"div"))) {
    for(i=0;i<n;i++) {
       *(data+i) = *(data+i) / arg;;
    }
  }
  else if (!(strcmp(op,"mul"))) {
    for(i=0;i<n;i++) {
       *(data+i) = *(data+i) * arg;;
    }
  }
  else if (!(strcmp(op,"add"))) {
    for(i=0;i<n;i++) {
       *(data+i) = *(data+i) + arg;;
    }
  }
  else if (!(strcmp(op,"smooth"))) {
    nhalf = (int) arg;
    for(i=nhalf;i<n-nhalf;i++) {
      sum = 0.0;
      for(j=i-nhalf; j<i+nhalf+1; j++) {
	sum = sum + *(data+j);
      }
       *(data+i) = sum/(2*nhalf + 1);
    }
  }
  else if (!(strcmp(op,"zero"))) {
    for(i=0;i<n;i++) {
       *(data+i) = 0.0;
    }
  }
  else if (!(strcmp(op,"envelope"))) {
    scratch=(float *)malloc(SCRATCH_SIZE*sizeof(float));
    hilbert=(float *) malloc(n*sizeof(float));
    dataptr = data;
    hilbertptr = hilbert;

   /* -- Compute the Hilbert transform. */
    firtrn( "HILBERT", data, n, scratch,  hilbert );

    /* -- Compute the envelope by taking the square root of the sum
     *    of the squares of the signal and its Hilbert transform. */

    for( j = 0; j <= (n - 1); j++ ){
       *dataptr = sqrt( pow(*dataptr,2) + pow(*hilbertptr,2) );
       dataptr++;
       hilbertptr++;
     }
     free(hilbert);
     free(scratch);
  }
  else if (!(strcmp(op,"interpolate"))) {
    hilbert=(float *) malloc(n*sizeof(float));
    n_interp = (int) (n*(*delta/arg)) - 1;
    newdelt = arg;
    newtime = 0.;
    begin = 0.;
    for( j = 0; j <= (n_interp - 1); j++ ){ 
      wigint(&begin, data, n, *delta, .001, newtime, (hilbert+j));
      newtime = newtime + newdelt;
    }
    for(i=0;i<n_interp;i++) {
       *(data+i) = *(hilbert+i);
    }
    *delta = arg;
    *npts = n_interp;
    free(hilbert);
  }
  else {
    fprintf(stderr, "ERROR! Unknown Operation: %s \n",op);
    return(-1);
  }
  return(0);
}
