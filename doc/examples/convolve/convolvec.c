/*  convolvec.c
        Reads in a short time series that is convolved with the
          second (longer) time series.  Easiily expanded to read
          in multiple long time series.  Output has same length
          and time parameters as longer series.  (Assumes longer
          goes to zero at start and finish.)
 gcc -o convolvec convolvec.c -I/usr/local/sac/include  -L/usr/local/sac/lib  -lsacio -lsac
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sac.h>
#include <sacio.h>

#define MAX        4000
#define ERROR_MAX  256

static void td_conv(
                    float     *yarray,
                    int        nlen,
                    float     *yarrays,
                    int        nlens,
                    float     *yconv,
                    float      delta,
                    float      begs);
int 
main(int argc, char *argv[]) {
    
    /* Local variables */
    int i, j;
    int nlen, nlens, nerr, max;
    
    float beg, begs, delta;
    char *kname;
    
    float yarray[MAX], yarrays[MAX], yconv[MAX], dummy[MAX];
    
    char error[ERROR_MAX];
    
    max = MAX;
    
    for(i = 0; i < MAX; i++) {
      yarray[i] = 0.0;
      yarrays[i] = 0.0;
      yconv[i] = 0.0;
      dummy[i] = 0.0;
    }
    /* Read in the short time series  */
    kname = strdup("brune_pulse.sac");
    rsac1(kname, yarrays, &nlens, &begs, &delta, &max, &nerr, SAC_STRING_LENGTH);
    
    if (nerr != 0) {
        fprintf(stderr, "Error reading in file(%d): %s\n", nerr, kname);
        exit(-1);
    }
    
    
    /* Read in the long time series against which short series is convolved  */
    kname = strdup("synthetic.sac");
    rsac1(kname, yarray, &nlen, &beg, &delta, &max, &nerr, SAC_STRING_LENGTH);
    
    if (nerr != 0) {
        fprintf(stderr, "Error reading in file: %s\n", kname);
        exit(-1);
    }
    
  /*  Do the convolution (in the time domain) */
  
  td_conv(yarray,nlen,yarrays,nlens,yconv,delta,begs);
  
  setkhv ( "kevnm",  "Convolution", &nerr, SAC_STRING_LENGTH, SAC_STRING_LENGTH);
  
  /* Write output SAC file */
  
    kname = strdup("convolvec_out.sac");
    wsac0(kname, dummy, yconv, &nerr, SAC_STRING_LENGTH);
    if (nerr != 0) {
        fprintf(stderr, "Error writing out file: %s\n", kname);
        exit(-1);
    }
    
    return 0;
} /* end of program convolvec*/

/**
 * @file   td_conv.c
 *
 * @brief  Compute convolution of a long series (yarray) with yarrays
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/**
 * Compute the Cross-Correlation Function
 *
 * @param yarray
 *    Array containing input time series
 * @param nlen
 *    Number of samples in input time series yarray
 * @param yarrays
 *    Array containing the sshort time series to be convolved with array
 * @param nlens
 *    Number of samples in arrays
 * @param yconv
 *    Array containing the output time series
 * @param delta
 *    Time interval for yarray, yarrays, yconv
 * @param begs
 *    Begin time of arrays
 *
 * @return Nothing
 *
 * \author   Arthur Snoke
 *           VT
 *
 * \date 150908  Created
 *
 */
static void td_conv(
             float     *yarray,
             int        nlen,
             float     *yarrays,
             int        nlens,
             float     *yconv,
             float      delta,
             float      begs)
{
  int kshift, k, kstart, kk;
  float sum2, temp;
  
  if (nlens >= nlen) {
    fprintf(stderr, "Error: Long and short lengths %d %d\n", nlen, nlens);
    exit(-1);
  }
  
  sum2 = 0;
  kshift = lrint(begs/delta);
  if (kshift < 0) {
    kstart = -kshift;
  }
  else{
    kstart = 0;
  }
  
  for(k=0; k < nlens; k++)  sum2 = sum2 + yarrays[k]*yarrays[k];
  
  for(k=kstart; k < nlen; k++){
    temp = 0.0;
    for(kk=kstart; kk < nlen; kk++)
    if (k >= (kk-kstart) || nlens >= (k-kk+kstart))
      temp = temp + yarray[kk]*yarrays[k-kk+kstart];
    yconv[k] = delta*temp/sqrt(sum2);
  }
  return;
} /* end of function td_conv*/
