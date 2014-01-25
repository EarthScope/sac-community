/** 
 * @file   envelope.c
 * 
 * @brief  Envelope Function
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "sam.h"
#include "dbh.h"


/** 
 * Envelope Function using the Hilbert transform
 * 
 * @param n 
 *    Number of points in the signals \p in and \p out
 * @param in 
 *    Input signal of length \p n
 * @param out 
 *    Output signal of length \p n
 *
 * @date June 30, 2008 Original Version, B. Savage
 *
 */
void
envelope(int n, float *in, float *out) {
  int i;
  int n2;
  float *scratch;

  n2 = 5*1024;
  scratch = (float *)calloc(n2, sizeof(float));
  if(scratch == NULL) {
    fprintf(stderr, 
	    "Error allocating space for hilbert transform: npts %d\n", 
           n2);
    return;
  }
  for(i = 0; i < n; i++) {
    out[i] = 0.0;
  }
  firtrn("HILBERT",  in, n, scratch, out);

  for(i = 0; i < n; i++) {
    out[i] = sqrt(in[i] * in[i] + out[i] * out[i]);
  }

  free(scratch);
  scratch = NULL;
  return;
}

/** 
 * Envelope Function
 *   Fortran Interface
 * 
 * @see envelope
 */
void
envelope_(int *n, float *in, float *out) {
  envelope(*n, in, out);
}

/** 
 * Envelope Function
 *   Fortran Interface
 * 
 * @see envelope
 */
void
envelope__(int *n, float *in, float *out) {
  envelope(*n, in, out);
}
