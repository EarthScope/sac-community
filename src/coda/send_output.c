#include <stdio.h>
#include <math.h>

#include "coda.h"

int
send_output(envelopes,nbands,evid,lcalibrate,global_params)

     struct envelope envelopes[MAXBANDS];
     int nbands;
     char *evid;
     int lcalibrate;
     struct global_params *global_params;
{
  int i, npoints;
  float freq;
  char filename[200], specname[200], min[10], max[10],pickstatus[2];
  FILE	*fpcfg, *fpspc;		 /* Pointer to the configuration input file  */

  sprintf(filename, "%s.out",evid);
  if((fpcfg=fopen(filename,"w"))== NULL) {
    perror("Error opening output file \n");
    fprintf(stderr,"\n\t%%Error opening output file %s; Exiting.\n",filename);
    return -1;
  }
  sprintf(specname, "%s.spec",evid);
  if((fpspc=fopen(specname,"w"))== NULL) {
    perror("Error opening output file \n");
    fprintf(stderr,"\n\t%%Error opening output file %s; Exiting.\n",specname);
    return -1;
  }
  if(lcalibrate) {
    fprintf(stdout, "Stat   Freq     CodaAmp     Mo      Resid        win    SNR   b0  gamma\n");
    fprintf(fpcfg, " Stat   Freq     CodaAmp     Mo      Resid        win    SNR   b0  gamma\n");
  } else {
    fprintf(stdout, "Stat   Freq     CodaAmp     Mo      Resid        win    SNR\n");
    fprintf(fpcfg,  "Stat   Freq     CodaAmp     Mo      Resid        win    SNR\n");
  }

  for(i=0;i<nbands;i++) {

    freq = (envelopes[i].freq_low + envelopes[i].freq_high) / 2;
    if(envelopes[i].fit_window_picked == -1) {
       sprintf(pickstatus, "D");
    }
    else if(envelopes[i].fit_window_picked == 1) {
      sprintf(pickstatus, "P");
    }
    else if(envelopes[i].fit_window_picked == 2) {
      sprintf(pickstatus, "A");
    } 
    npoints=(int)(envelopes[i].fit_npoints*envelopes[i].GFdelta);
    if(lcalibrate) {
      fprintf(stdout, " %s     %2.2f     %2.2f     %02.2f    %1.5f%s    %6d  %5.2f%s  %5.4f  %5.2f\n",pickstatus,freq,envelopes[i].CodaAmp,envelopes[i].Moment,envelopes[i].fit_residual, envelopes[i].ResidStat,npoints,envelopes[i].fit_SNR,envelopes[i].SNRStat,envelopes[i].fit_b0,envelopes[i].fit_gamma );
      fprintf(fpcfg, " %s     %2.2f     %2.2f     %02.2f    %1.5f%s    %6d  %5.2f%s  %5.4f  %5.2f\n",pickstatus,freq,envelopes[i].CodaAmp,envelopes[i].Moment,envelopes[i].fit_residual,envelopes[i].ResidStat, npoints,envelopes[i].fit_SNR,envelopes[i].SNRStat,envelopes[i].fit_b0,envelopes[i].fit_gamma );
    } else { 
      fprintf(stdout, " %s     %2.2f     %2.2f     %02.2f    %1.5f%s    %6d  %5.2f%s\n",pickstatus,freq,envelopes[i].CodaAmp,envelopes[i].Moment,envelopes[i].fit_residual,envelopes[i].ResidStat, npoints,envelopes[i].fit_SNR,envelopes[i].SNRStat );
      fprintf(fpcfg, " %s     %2.2f     %2.2f     %02.2f    %1.5f%s    %6d  %5.2f%s\n",pickstatus,freq,envelopes[i].CodaAmp,envelopes[i].Moment,envelopes[i].fit_residual, envelopes[i].ResidStat,npoints,envelopes[i].fit_SNR,envelopes[i].SNRStat );
    }
    if(envelopes[i].Moment > 0.0) {
      fprintf(fpspc, " %2.2f     %2.2f \n",freq,envelopes[i].Moment );
    }
  }
  if(global_params->Moment > 0.0) {
    fprintf(stdout, "Moment= %f, Mw= %f \n",global_params->Moment, global_params->Mw);
    fprintf(fpcfg, "Log Moment= %f, Mw= %f \n",global_params->Moment, global_params->Mw);
  } else {
    fprintf(stdout, "Unable to calculate Moment and Mw \n");
    fprintf(fpcfg, "Unable to calculate Moment and Mw \n");
  }
  if(global_params->Mb > 0.0) {
    fprintf(stdout, "Mb= %f \n",global_params->Mb);
    fprintf(fpcfg, "Mb= %f \n",global_params->Mb);
  } else {
    fprintf(stdout, "Unable to calculate Mb \n");
    fprintf(fpcfg, "Unable to calculate Mb \n");
  }
  if(global_params->Energy_Total > 0.0) {
    fprintf(stdout, "Total Energy= %e dyne-cm\n",global_params->Energy_Total);
    fprintf(fpcfg, "Total Energy= %e dyne-cm\n",global_params->Energy_Total);
    fprintf(stdout, "Energies: Observed= %e High= %e Low=%e \n",global_params->Energy_Obs,global_params->Energy_High,global_params->Energy_Low);
    fprintf(fpcfg, "Energies: Observed= %e High= %e Low=%e \n",global_params->Energy_Obs,global_params->Energy_High,global_params->Energy_Low);
    fprintf(stdout,"Stress Drop= %e bars \n",global_params->Stress_Total);
    fprintf(fpcfg,"Stress Drop= %e bars \n",global_params->Stress_Total);

  } else {
    fprintf(stdout, "Unable to calculate Energy\n");
    fprintf(fpcfg, "Unable to calculate Energy \n");
  }

    sprintf(max,"%1.3f", MAX_RESIDUAL);
    sprintf(min,"%1.3f", MINIMUM_SNR);
    fprintf(stdout, "*-Denotes did not pass acceptance threshold for SNR or residual \n");
    fprintf(fpcfg, "*-Denotes did not pass acceptance threshold for SNR or residual \n");

    fclose(fpcfg);
    fclose(fpspc);
    return 1;
}

