#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "coda.h"
#include "dff.h"

void
calc_coda_amplitudes(envelopes,nbands,dist,begin,evid,lcalibrate)

     struct envelope envelopes[MAXBANDS];
     int nbands;
     float dist, begin;
     char *evid;
     int lcalibrate;
{
  int i,j,noisecount,namelength,nptswrite;
  float start_time, sigsum, noisesum;
  char filename[200];
  float *databuf1;
  float delta;
  float  dist_slope;
  int nerr;

  for(i=0;i<nbands;i++) {
    envelopes[i].GFnpts = NPTS_GF;
    envelopes[i].GFstart = START_GF;
    envelopes[i].GFdelta = DELTA_GF;

    /* align GF and observed envelopes--find point in observed that corresponds to first point of GF */
    if(envelopes[i].vel_slope != 0.0) { /* old way */
      dist_slope = envelopes[i].dist_slope;
      if(dist > envelopes[i].dist_critical)
	dist_slope = 0.0;
      start_time = dist / ( envelopes[i].vel0 + (dist_slope * envelopes[i].vel_slope)) + START_WINDOW_ADD;
    } else {
      start_time =  envelopes[i].vel0 + (dist * envelopes[i].vel1) + (dist*dist * envelopes[i].vel2)+ START_WINDOW_ADD;
    }
    if( envelopes[i].fit_window_picked == 0) {
      calc_codaGF(&envelopes[i],dist);
      envelopes[i].window_start = (int) ((start_time) / envelopes[i].GFdelta );
      envelopes[i].window_start_seconds = start_time;
    }
    if(( envelopes[i].fit_window_picked == 0) || ( envelopes[i].fit_window_picked == 1)) { 
      if(lcalibrate) {
	fit_coda_params(&envelopes[i],begin,dist);
      } else {
	fit_coda_amp(&envelopes[i],begin);
      }
    }
    if( envelopes[i].fit_window_picked == 0)
      envelopes[i].fit_window_picked = 2;
 
    databuf1 = (float *) malloc(envelopes[i].fit_npoints*sizeof(float));
    sprintf(filename, "%s.GFfit-%2.2f_%2.2f",evid,envelopes[i].freq_low,envelopes[i].freq_high);
    binary_op(envelopes[i].GFenvelope,databuf1,envelopes[i].fit_npoints,"copy");
    unary_op(databuf1,&envelopes[i].fit_npoints,"add",envelopes[i].CodaAmp,&delta);
    setfhv("USER0", (float *)&envelopes[i].fit_residual,&nerr,6);
    setfhv("USER1", (float *)&envelopes[i].fit_npoints,&nerr,6);
    nptswrite=(envelopes[i].fit_npoints);
    if(envelopes[i].fit_npoints > 0) {
      namelength = strcspn(filename," ");
    wsac1(filename,databuf1,&nptswrite,&envelopes[i].window_start_seconds,&envelopes[i].GFdelta,&nerr,namelength+1);
    }
    
    /* Measure SNR */
    noisesum = 0.;
    noisecount = 0;
    for(j=((int) (NOISE_START/envelopes[i].GFdelta));j<((int) ((NOISE_START+NOISE_LENGTH)/envelopes[i].GFdelta));j++) {
     noisesum = noisesum + *(envelopes[i].envelope_data+j);
     noisecount++;
    }

    noisesum = noisesum/noisecount;
    
    sigsum = 0.;
    noisecount = 0;
    envelopes[i].window_stop = envelopes[i].window_start+envelopes[i].fit_npoints;
    for(j=envelopes[i].window_start;j<envelopes[i].window_stop;j++) {
     sigsum = sigsum + *(envelopes[i].envelope_data+j);
     noisecount++;
    }

    sigsum = sigsum/noisecount;
    envelopes[i].fit_SNR = sigsum - noisesum;
  }

}
