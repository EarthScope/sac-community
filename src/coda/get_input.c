#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "coda.h"

#include "string_utils.h"

FILE	*fpcfg;		 /* Pointer to the configuration input file  */

int get_input(inputfile,envelopes,nbands,global_params)
   char *inputfile;
   struct envelope envelopes[MAXBANDS];
   int *nbands;
   struct global_params *global_params;
   
{
  int ifreq,length;
  int nchar=200;
  float tempfloat;
  static char charbuf[200],readbuf[200];
  char *rp, *key;

  ifreq = 0;
  key = &charbuf[0];

  length = strcspn(inputfile," ");
  inputfile[length] = '\0';
  envelopes[ifreq].minimum_SNR = MINIMUM_SNR;
  envelopes[ifreq].max_residual = MAX_RESIDUAL;
  envelopes[ifreq].dist_critical = DIST_CRITICAL;
  envelopes[ifreq].gamma = GAMMA;
  envelopes[ifreq].Mb_weight = 0.0;
  envelopes[ifreq].vel_slope = 0.0;
  envelopes[ifreq].vel0 = 0.0;
  if((fpcfg=fopen(inputfile,"r"))== NULL) {
    perror("Error opening input file \n");
    fprintf(stderr,"\n\t%%Error opening input file %s; Exiting.\n",inputfile);
    return -1;
  }

  /* need to fix bug if extra CRLF at end of file */
  ifreq = -1;
  while(fgetsp(readbuf, nchar, fpcfg)!=NULL) {
    rp = &readbuf[0];
    sscanf(rp,"%s",key);
    strtok(rp," ");
    if (!(strcmp(key,"freq")) || !(strcmp(key, "Freq")) || !(strcmp(key, "FREQ"))) {
      ifreq++;
      rp = strtok(NULL," ");
      sscanf(rp,"%f",&tempfloat);
      envelopes[ifreq].freq_low = tempfloat;
      rp = strtok(NULL," ");
      sscanf(rp,"%f",&tempfloat);
      envelopes[ifreq].freq_high = tempfloat;
    }
    if (!(strcmp(key,"b0")) || !(strcmp(key, "B0"))) {
      rp = strtok(NULL," ");
      sscanf(rp,"%f",&tempfloat);
      envelopes[ifreq].b0 = tempfloat;
    }
    if (!(strcmp(key,"mb_weight")) || !(strcmp(key, "MB_WEIGHT")) || !(strcmp(key, "Mb_weight")) || !(strcmp(key, "Mb_Weight"))) {
      rp = strtok(NULL," ");
      sscanf(rp,"%f",&tempfloat);
      envelopes[ifreq].Mb_weight = tempfloat;
    }
    if (!(strcmp(key,"vel_order0")) || !(strcmp(key, "Vel_Order0")) || !(strcmp(key, "VEL_ORDER0"))) {
      rp = strtok(NULL," ");
      sscanf(rp,"%f",&tempfloat);
      envelopes[ifreq].vel0 = tempfloat;
    }
    if (!(strcmp(key,"vel_order1")) || !(strcmp(key, "Vel_Order1")) || !(strcmp(key, "VEL_ORDER1"))) {
      rp = strtok(NULL," ");
      sscanf(rp,"%f",&tempfloat);
      envelopes[ifreq].vel1 = tempfloat;
    }
    if (!(strcmp(key,"vel_order2")) || !(strcmp(key, "Vel_Order2")) || !(strcmp(key, "VEL_ORDER2"))) {
      rp = strtok(NULL," ");
      sscanf(rp,"%f",&tempfloat);
      envelopes[ifreq].vel2 = tempfloat;
    }
    if (!(strcmp(key,"vel0")) || !(strcmp(key, "Vel0")) || !(strcmp(key, "VEL0"))) {
      rp = strtok(NULL," ");
      sscanf(rp,"%f",&tempfloat);
      envelopes[ifreq].vel0 = tempfloat;
    }
    if (!(strcmp(key,"vel_slope")) || !(strcmp(key, "Vel_Slope")) || !(strcmp(key, "VEL_SLOPE"))) {
      rp = strtok(NULL," ");
      sscanf(rp,"%f",&tempfloat);
      envelopes[ifreq].vel_slope = tempfloat;
    }
    if (!(strcmp(key,"dist_critical")) || !(strcmp(key, "Dist_Critical")) || !(strcmp(key, "DIST_CRITICAL"))) {
      rp = strtok(NULL," ");
      sscanf(rp,"%f",&tempfloat);
      envelopes[ifreq].dist_critical = tempfloat;
    }
    if (!(strcmp(key,"minimum_snr")) || !(strcmp(key, "Minimum_SNR")) || !(strcmp(key, "MINIMUM_SNR"))) {
      rp = strtok(NULL," ");
      sscanf(rp,"%f",&tempfloat);
      envelopes[ifreq].minimum_SNR = tempfloat;
    }
    if (!(strcmp(key,"max_residual")) || !(strcmp(key, "Max_Residual")) || !(strcmp(key, "MAX_RESIDUAL"))) {
      rp = strtok(NULL," ");
      sscanf(rp,"%f",&tempfloat);
      envelopes[ifreq].max_residual = tempfloat;
    }
    if (!(strcmp(key,"gamma")) || !(strcmp(key, "Gamma")) || !(strcmp(key, "GAMMA"))) {
      rp = strtok(NULL," ");
      sscanf(rp,"%f",&tempfloat);
      envelopes[ifreq].gamma = tempfloat;
    }
    if (!(strcmp(key,"min_length")) || !(strcmp(key, "Min_Length")) || !(strcmp(key, "MIN_LENGTH"))) {
      rp = strtok(NULL," ");
      sscanf(rp,"%f",&tempfloat);
      envelopes[ifreq].min_length = tempfloat;
    }
    if (!(strcmp(key,"noise_thresh")) || !(strcmp(key, "Noise_Thresh")) || !(strcmp(key, "NOISE_THRESH"))) {
      rp = strtok(NULL," ");
      sscanf(rp,"%f",&tempfloat);
      envelopes[ifreq].noise_thresh = tempfloat;
    }
    if (!(strcmp(key,"b_slope")) || !(strcmp(key, "B_Slope")) || !(strcmp(key, "B_SLOPE"))) {
      rp = strtok(NULL," ");
      sscanf(rp,"%f",&tempfloat);
      envelopes[ifreq].b_slope = tempfloat;
    }
    if (!(strcmp(key,"dist_slope")) || !(strcmp(key, "Dist_Slope")) || !(strcmp(key, "DIST_SLOPE"))) {
      rp = strtok(NULL," ");
      sscanf(rp,"%f",&tempfloat);
      envelopes[ifreq].dist_slope = tempfloat;
    }
    if (!(strcmp(key,"dist_order1")) || !(strcmp(key, "Dist_Order1")) || !(strcmp(key, "DIST_ORDER1"))) {
      rp = strtok(NULL," ");
      sscanf(rp,"%f",&tempfloat);
      envelopes[ifreq].dist_order1 = tempfloat;
    }
    if (!(strcmp(key,"dist_order2")) || !(strcmp(key, "Dist_Order2")) || !(strcmp(key, "DIST_ORDER2"))) {
      rp = strtok(NULL," ");
      sscanf(rp,"%f",&tempfloat);
      envelopes[ifreq].dist_order2 = tempfloat;
    }
    if (!(strcmp(key,"moment_correction")) || !(strcmp(key,"Moment_correction")) || !(strcmp(key, "Moment_Correction")) || !(strcmp(key, "MOMENT_CORRECTION"))) {
      rp = strtok(NULL," ");
      sscanf(rp,"%f",&tempfloat);
      envelopes[ifreq].Moment_correction = tempfloat;
    }
    /* global parameters */
    if (!(strcmp(key,"Mb_scale")) || !(strcmp(key,"MB_SCALE")) || !(strcmp(key, "Mb_Scale")) || !(strcmp(key, "mb_scale"))) {
      rp = strtok(NULL," ");
      sscanf(rp,"%f",&tempfloat);
      global_params->Mb_scale = tempfloat;
    }
    if (!(strcmp(key,"Mb_constant")) || !(strcmp(key,"MB_CONSTANT")) || !(strcmp(key, "Mb_Constant")) || !(strcmp(key, "mb_constant"))) {
      rp = strtok(NULL," ");
      sscanf(rp,"%f",&tempfloat);
      global_params->Mb_constant = tempfloat;
    }
  }
  *nbands = ifreq+1;
  fclose(fpcfg);
  return 0;
}
