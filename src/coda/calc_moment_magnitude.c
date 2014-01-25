#include <stdio.h>
#include <math.h>

#include "coda.h"
#include "co.h"

void
calc_moment_magnitude(envelopes,nbands,global_params)

     struct envelope envelopes[MAXBANDS];
     int nbands;
     struct global_params *global_params;
{
  int i, Moment_count=0, Mb_count=0;
  float Moment, Mb;

  Moment = 0.0;
  Moment_count = 0;
  Mb = 0.;
  Mb_count = 0;

  for(i=0;i<nbands;i++) {
    if((envelopes[i].fit_residual <= envelopes[i].max_residual ) && (envelopes[i].fit_SNR >= envelopes[i].minimum_SNR) && (envelopes[i].fit_window_picked > -1)) {
      envelopes[i].Moment = envelopes[i].CodaAmp + envelopes[i].Moment_correction;
      if((i+1) < MAX_MOMENT_INDEX) {
         Moment = Moment + envelopes[i].Moment;
         Moment_count++;
      }
      else if (Moment_count == 0) {
         Moment = Moment + envelopes[i].Moment;
         Moment_count++;
      }
      /* calculate Mb */
      if(envelopes[i].Mb_weight > 0.0) {
	Mb = Mb + envelopes[i].Mb_weight*envelopes[i].Moment;
	Mb_count++;
      }
      sprintf(envelopes[i].ResidStat," ");
      sprintf(envelopes[i].SNRStat," ");
    }
    else {
      envelopes[i].Moment = 0.0;
      if(envelopes[i].fit_residual > envelopes[i].max_residual) 
	sprintf(envelopes[i].ResidStat,"*");
      if(envelopes[i].fit_SNR < envelopes[i].minimum_SNR)
	sprintf(envelopes[i].SNRStat,"*");
    }

  }
  /* Now combine some of the moments to get magnitude */
  if(Moment_count > 0) {
    global_params->Moment = Moment / Moment_count;
    global_params->Mw = global_params->Moment * MW_MULTIPLIER - MW_SUBTRACT;
  } else {
    global_params->Moment = 0.0;
    global_params->Mw = 0.0;
  }
  if(Mb_count > 0) {
    Mb = Mb / Mb_count;
    global_params->Mb = Mb*global_params->Mb_scale + global_params->Mb_constant;
    
  } else {
    global_params->Mb = 0.0;
  }
}

