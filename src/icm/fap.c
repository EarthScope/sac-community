
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "string_utils.h"


#include "co.h"

#define BUFLEN 2048

#define FAP_COMMENT '*'
#define FAP_NEWLINE '\n'
#define FAP_TERMINATOR '\0'

#define PI M_PI

double * fap_grow(double *p, int n);
void amp_phase_to_complex(double am, double ph, double *re, double *im);
void frequency_amplitude_phase(int     nf, 
                               double  df, 
                               double *xre,
                               double *xim,
                               char   *file,
                               int     file_s,
                               int    *nerr);


double *
fap_grow(double *p, int n) {
  double *tmp;
  tmp = (double *) realloc(p, sizeof(double) * n);
  if(!tmp) {
    fprintf(stderr, "Error allocating memory for Frequency, Amplitude, and Phase file\n");
    free(p);
    p = NULL;
    return NULL;
  }
  return tmp;
}

void
amp_phase_to_complex(double am, double ph, double *re, double *im) {
  *re = am * cos( ph * ( PI / 180.0 ) );
  *im = am * sin( ph * ( PI / 180.0 ) );
}

void
frequency_amplitude_phase(int     nf,
                          double  df,
                          double *xre,
                          double *xim,
                          char   *file,
                          int     file_s,
                          int    *nerr) {
  FILE *fp;
  char line[BUFLEN];
  char *p;
  int i, n, nalloc, j, istart;
  double *freq, *amp, *phase;
  double f, del, fixlog, ampfix, phzfix;

  n = 0;
  nalloc = 0;

  freq = amp = phase = NULL;

  zopens( &fp, file, file_s, "ROTEXT", 7, nerr);
  if(!nerr) {
    return;
  }

  while((p = fgetsp(line, BUFLEN, fp)) != NULL) {
    while( isspace(p[0]) ) { p++; }
    if( p[0] == FAP_COMMENT || p[0] == FAP_NEWLINE || p[0] == FAP_TERMINATOR ) {
      continue;
    }
    if(n >= nalloc) {
      if(n == 0) {
        nalloc = 16;
      } else {
        nalloc *= 2;
      }
      if((freq  = fap_grow(freq,  nalloc)) == NULL) { *nerr = 2; goto ERROR; }
      if((amp   = fap_grow(amp ,  nalloc)) == NULL) { *nerr = 2; goto ERROR; }
      if((phase = fap_grow(phase, nalloc)) == NULL) { *nerr = 2; goto ERROR; }
    }
    if(sscanf(p, "%lf %lf %lf", &freq[n], &amp[n], &phase[n]) != 3) {
      fprintf(stderr, "Error reading frequency amplitude phase line\n");
      *nerr = 2;
    }

    n++;
  }
  
  zcloses(&fp, nerr);
  if(!nerr) {
    goto ERROR;
  }
  n--;
  if(!freq || !amp || !phase) {
    goto ERROR;
  }

  /* Do not use freq[0]  */

  istart = 0;
  if (freq[0] == 0.0) istart = 1;
  for(i = 0; i < nf; i++) {
    f = i * df;
    if(f <= freq[istart]) {
      amp_phase_to_complex(amp[0], phase[0], &xre[i], &xim[i]);
      if (i == 1) printf(" Extrapolating below lowest FAPFILE frequency\n");
    } else if(f >= freq[n]) {
        amp_phase_to_complex(amp[n], phase[n], &xre[i], &xim[i]);
        if (i == nf-1) printf(" Extrapolating above highest FAPFILE frequency\n");
    } else {
      for(j = istart; j < n; j++) {
        if(f >= freq[j] && f < freq[j+1]) {
          /* Interpolate on log-log basis for amplitude response */
          del      = log10(f / freq[j]) / log10(freq[j+1] / freq[j]);
          fixlog = log10(amp[j]) + del * log10( amp[j+1]/amp[j] );
          ampfix = pow(10, fixlog);

          /* Linear interpolation for phase */
          del = (f - freq[j]) / (freq[j+1] - freq[j]);
          phzfix = phase[j] + del * (phase[j+1] - phase[j]);

          /* Convert to Complex Number */
          amp_phase_to_complex(ampfix, phzfix, &xre[i], &xim[i]);
          break;
        }
      }
    }
  }


 ERROR:
  free(freq);
  free(amp);
  free(phase);
  return;

}
