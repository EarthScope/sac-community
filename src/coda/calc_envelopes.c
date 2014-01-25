#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "coda.h"
#include "dff.h"
#include "dbh.h"

#include "debug.h"

void calc_envelopes(float *data1,
                    float *data2,
                    int ndata,
                    int horizontals,
                    struct envelope envelopes[MAXBANDS],
                    int *nbands,
                    float begin,
                    float delta,
                    int npts,
                    float dist,
                    char *evid) {

  int i,orig_npts,new_npts,namelength;
  float *databuf1, *databuf2;
  float orig_delta;
  char filename[200];
  int nerr;
  UNUSED(npts);
  UNUSED(dist);
  databuf1 = (float *) malloc(ndata*sizeof(float));
  if(horizontals){ 
     databuf2 = (float *) malloc(ndata*sizeof(float));
  }

  for(i=0;i<*nbands;i++) {
    if(envelopes[i].freq_high > ((1/delta)/2)) {
      fprintf(stderr, "Frequency %f is greater than nyquist! Skip this band \n",envelopes[i].freq_high);
      *nbands = *nbands -1;
    }
    binary_op(data1,databuf1,ndata,"copy");
    xapiir(databuf1, ndata, "BU", 0., 0., 2, "BP",(envelopes+i)->freq_low, envelopes[i].freq_high, delta, 2);
    unary_op(databuf1,&ndata,"envelope",0.0,&delta);
    unary_op(databuf1,&ndata,"log10",0.0,&delta);
    if(horizontals) {
      binary_op(data2,databuf2,ndata,"copy");
      xapiir(databuf2, ndata, "BU", 0., 0., 2, "BP",envelopes[i].freq_low, envelopes[i].freq_high, delta, 2);
      unary_op(databuf2,&ndata,"envelope",0.0,&delta);
      unary_op(databuf2,&ndata,"log10",0.0,&delta);
      binary_op(databuf1,databuf2,ndata,"addf");
      unary_op(databuf1,&ndata,"div",2.0,&delta);
    }
    orig_npts = ndata;
    orig_delta = delta;
    unary_op(databuf1,&ndata,"interpolate",DELTA_GF,&delta);
    new_npts = ndata-5;
    ndata = orig_npts;
    unary_op(databuf1,&new_npts,"smooth",10.0,&delta);
    envelopes[i].envelope_data = (float *) malloc(new_npts*sizeof(float));
    binary_op(databuf1,envelopes[i].envelope_data,new_npts,"copy");
    sprintf(filename, "%s.env_%2.2f-%2.2f",evid,envelopes[i].freq_low,envelopes[i].freq_high);
    envelopes[i].number_of_points = new_npts;
    envelopes[i].GFdelta = DELTA_GF;
    namelength = strcspn(filename," ");
    wsac1(filename,databuf1,&new_npts,&begin,&delta,&nerr,namelength+1);
    delta = orig_delta;
    envelopes[i].fit_window_picked = 0;

  }
  free(databuf1);
  if(horizontals) {
     free(databuf2);
  }
}
