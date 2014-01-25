#include <stdio.h>
#include <string.h>
#include <math.h>

#include "coda.h"
#include "debug.h"

void
calc_energy(envelopes,nbands,evid,lcalibrate,global_params)

     struct envelope envelopes[MAXBANDS];
     int nbands;
     char *evid;
     int lcalibrate;
     struct global_params *global_params;
{
  int jj, ibeg, iend;
  float d, df, mu, stress_total, energy;
  float f[MAXBANDS], A[MAXBANDS], E[MAXBANDS];
  float mo, Etotal_obs, Etotal_HI, Etotal_low, sum, temp, pi;
  UNUSED(evid);
  UNUSED(lcalibrate);
  /*Now compute the total energy by summing the squared product of the 
    discrete path-corrected moment rate spectra (ie. (10**A(1))/d ) 
    and omega, and then
    divide by 4*pi*rho*(vel**5)/(Radiation)        Radiation=(2/5)
    rho=2.7 gm/cm**3    vel=3.5E5 cm/s
    Then finally multiply by dw (delta omega) at the front.
  */
  d=sqrt(4.45E29); /*This must be put inside the equation due to precision */
  pi = 3.141592654;
  sum=0.0;
  Etotal_HI=0.0;
  Etotal_low=0.0;
  mo=global_params->Moment;

  memset(A, 0, sizeof(A));
  /* Calculate observed energy */
  /* first see if there are enough measurements? */
  for(jj=0;jj<nbands-1;jj++) {
    f[jj] = (envelopes[jj].freq_low + envelopes[jj].freq_high) / 2;
    f[jj+1] = (envelopes[jj+1].freq_low + envelopes[jj+1].freq_high) / 2;
    A[jj] = envelopes[jj].Moment;
    A[jj+1] = envelopes[jj+1].Moment;
    E[jj]=pow(((pow(10.,A[jj])/d)*(2.0*pi*f[jj])),2); /* Energy at f(jj) */

    E[jj+1]=pow(((pow(10.,A[jj+1])/d)*(2.0*pi*f[jj+1])),2);   /* Energy at f(jj+1) */
    df=2.0*pi*(f[jj+1]-f[jj]);
    if((E[jj+1] > E[jj]) && (E[jj] != 0.000)) {
      sum=sum+(E[jj+1]*df)-(((E[jj+1]-E[jj])*df)/2.0);
    }	else {
      if((E[jj+1] <= E[jj]) && (E[jj] != 0.000)) {
	sum=sum+(E[jj]*df)-(((E[jj]-E[jj+1])*df)/2.0);
      }
    }
  }
  Etotal_obs=sum;

  ibeg = -1;
  iend = -1;
  jj = 0;
  while((ibeg < 0) && (jj < nbands-1)) {
    if((A[jj] != 0.0) && (A[jj+1] != 0.0) && (A[jj+2] != 0.0)) {
      ibeg=jj;
    } else {
      jj++;
    }
  }

  jj = nbands-1;
  while((iend < 0) && (jj > 1)) {
    if((A[jj] != 0.0) && (A[jj-1] != 0.0)) {
      iend=jj;
    } else {
      jj--;
    }
  }
  if((ibeg > 0) && (iend > 0)) {
    temp=(2.0*pi*f[ibeg]);
    Etotal_low=temp*pow(((pow(10.,mo)/d)*temp),2);
    Etotal_low=Etotal_low/3.0;
    temp=(2.0*pi*f[iend]);
    Etotal_HI=pow(((pow(10.,A[iend])/d)*temp),2);
    Etotal_HI=Etotal_HI*temp;
  } else {
    fprintf(stderr, "Warning! Unable to calculate low and high frequency energy \n");
  }    

  /*    TOTAL ENERGY */
  energy=Etotal_low+Etotal_obs+Etotal_HI;  /*this is the total energy of low,observed and high */
  energy=1.07*energy;    /* this 7% extra energy is for the P-wave contribution */

  /* Compute stress drops */
  mu=3.3E5;
  stress_total=(2.0*mu*(energy)/pow(10.,mo));

  global_params->Energy_Obs = Etotal_obs;
  global_params->Energy_High = Etotal_HI;
  global_params->Energy_Low = Etotal_low;
  global_params->Energy_Total = energy;
  global_params->Stress_Total = stress_total;
}

