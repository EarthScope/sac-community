
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "coda.h"

void amoeba(float p[NPMAX+1][NPMAX], float y[NPMAX], int mp, int np, float ftol);
float ampresid(float xx[NPMAX]);
float fit_amplitude_only(float ftol);
void fit_coda_amp(struct envelope *Passed_envelopes, float abegin);

int current_band;
struct envelope *envelopes;
float CF_begin;

void fit_coda_amp(struct envelope *Passed_envelopes, float abegin)
{


  float xx[NPMAX];

  int mp,np,n,m,i,j,window_start;
  float pzero[NPMAX],lam[NPMAX],p[NPMAX+1][NPMAX],yy[NPMAX+1];
  float ftol,srvar;

  envelopes = Passed_envelopes;
  CF_begin = abegin;

  if( envelopes->fit_window_picked) { /* use simple fit for amplitude */
    envelopes->CodaAmp = fit_amplitude_only(.02);

  } else {  /* use amoeba to fit amp and npoints */

    /*    fprintf(stderr,"Starting fit for frequency %f \n",envelopes->freq_low); */
    /*** note that the precision of the result depend on the choice 
	 c of lam(i).   I think small lambda's are better but slower.  */

    /*parameter2 (1) is number of points in fit
      parameter1 is amplitude scale factor (coda amplitude)
             initial guesses at parameters*/
    
    window_start = envelopes->window_start;
    j = window_start-(CF_begin/envelopes->GFdelta);
    i = FIT_INITIAL_LENGTH; 
    pzero[0] = *(envelopes->envelope_data+j) - *(envelopes->GFenvelope+i);
    
    pzero[1] = FIT_INITIAL_LENGTH; 

    /*     uncertainties in parameters
	   to fix a parameter, set lam = 0 */
    lam[0] = .3;
    lam[1] = 25;


    np = 2;

    /*  initialize parameter arrays*/
    mp = np + 1;
    for(m=0;m<mp;m++){
      for(n=0;n<np;n++){
	if (n != m) 
	  p[m][n] = pzero[n];
	else 
	  p[m][n] = pzero[n] + lam[n];
	xx[n] = p[m][n];
      }
      yy[m] = ampresid (xx);
    }

    /*  use the simplex algorithm to find the best-fitting parameters */
    ftol = 0.0001;
    amoeba (p,yy,mp,np,ftol);
    
    /*   need to be here if fit is done and results are to be summarized */
    
    for(i=0;i<np;i++)
      xx[i] = 0.0;
    
    for(m=0;m<mp;m++)
      for(n=0;n<np;n++)
	xx[n] = xx[n] + p[m][n] / (float) mp;
    
    srvar = ampresid(xx);
    /*    fprintf(stdout, "Residual: %f \n", srvar);
        fprintf(stdout, "Number of points:  %f \n", xx[1]);
        fprintf(stdout, "Coda Amplitude: %f \n", xx[0]); */
    if(srvar > 1.0) {
      envelopes->fit_npoints = (int) (envelopes->min_length / envelopes->GFdelta);
      envelopes->CodaAmp = fit_amplitude_only(.02);
    } else {
      envelopes->CodaAmp =  xx[0];
      envelopes->fit_residual = (srvar*(pow(xx[1],FIT_NPTS_POW)))/xx[1];
      envelopes->fit_npoints = (int) xx[1];
    }
  } /* end of else for type of fit */

}

void amoeba(float p[NPMAX+1][NPMAX], float y[NPMAX], int mp, int np, float ftol)
{
#define  ITMAX 1000
  float alpha=1.0,beta=0.5,gamma=2.0;
  float pr[NPMAX],prr[NPMAX],pbar[NPMAX];
  int iter,ilo,ihi,inhi,i,j;
  float ypr,yprr,rtol;
 
  for (iter=0;iter<ITMAX;iter++) {
    ilo=0;
      if(y[0] > y[1]) {
        ihi=0;
        inhi=1;
      }
      else {
        ihi=1;
        inhi=0;
      }
      for( i=0;i<mp;i++) {
        if(y[i] < y[ilo]) 
	  ilo=i;
        if(y[i] > y[ihi]) {
          inhi=ihi;
          ihi=i;
	}
        else 
	  if(y[i] > y[inhi]) 
	    if(i != ihi) 
	      inhi=i;
      }
      rtol=2.*fabs(y[ihi]-y[ilo])/(fabs(y[ihi])+fabs(y[ilo]));
      if(rtol < ftol)
	return;
      for (j=0;j<np;j++)
        pbar[j]=0.;
      
      for (i=0;i<mp;i++) {
        if(i != ihi)
          for(j=0;j<np;j++)
            pbar[j]=pbar[j]+p[i][j];
      }
      for (j=0;j<np;j++) {
        pbar[j]=pbar[j]/np;
        pr[j]=(1.+alpha)*pbar[j]-alpha*p[ihi][j];
      }
      ypr=ampresid(pr);
      if(ypr <= y[ilo]) {
	for( j=0;j<np;j++)
	  prr[j]=gamma*pr[j]+(1.-gamma)*pbar[j];
	yprr=ampresid(prr);
	if(yprr < y[ilo]) {
	  for( j=0;j<np;j++)
	    p[ihi][j]=prr[j];
	  y[ihi]=yprr;
	}
	
	else {
	  for( j=0;j<np;j++)
	    p[ihi][j]=pr[j];
	  y[ihi]=ypr;
	}
      }
      else if (ypr >= y[inhi]) {
	if(ypr < y[ihi]) {
	  for(j=0;j<np;j++)
	    p[ihi][j]=pr[j];
	  y[ihi]=ypr;
	}
	for(j=0;j<np;j++)
	  prr[j]=beta*p[ihi][j]+(1.-beta)*pbar[j];
	yprr=ampresid(prr);
	if(yprr < y[ihi]) {
	  for(j=0;j<np;j++)
	    p[ihi][j]=prr[j];
	  y[ihi]=yprr; 
	}
	else {
	  for(i=0;i<mp;i++) {
	    if(i!=ilo) {
	      for(j=0;j<np;j++) {
		pr[j]=0.5*(p[i][j]+p[ilo][j]);
		  p[i][j]=pr[j];
	      }
	      y[i]=ampresid(pr);
	    }
	  }
	}
      }
      else {
	for(j=0;j<np;j++)
	  p[ihi][j]=pr[j];
	y[ihi]=ypr;
      }
      
    }
}

float ampresid(float xx[NPMAX])
{
  
  int i,j,window_start,npoints,min_length,nptsAfterStart;
  float resid=0.0, amp, diff;
  /* xx[1] is npoints, xx[0] is amplitude */
  min_length = (int) (envelopes->min_length / envelopes->GFdelta);
  window_start = envelopes->window_start;
  nptsAfterStart = envelopes->number_of_points -window_start + (int) (CF_begin/envelopes->GFdelta);

  npoints = (int) xx[1];
  amp = xx[0];
  resid = 0.0;

  if(npoints < min_length) {
    npoints = min_length+10.;
    xx[1] = npoints;
    resid = 1000.;
  }
  if( npoints > nptsAfterStart) {
    npoints = nptsAfterStart-100;
    xx[1] = npoints;
    resid = 1000.0;
  }
  if( npoints >= NPTS_GF) {
    npoints=NPTS_GF-100;
    xx[1] = npoints;
    resid = 1000.0;
  }

  j = window_start-(CF_begin/envelopes->GFdelta);
  for(i=0;i<npoints;i++) {
    diff = *(envelopes->GFenvelope+i)+amp - *(envelopes->envelope_data+j);
    resid = resid + diff*diff;
    j++;
  }
    resid = sqrt(resid);
    /*    fprintf(stderr,"%f %d %f \n",resid/npoints,npoints,amp); */
    return resid/(pow(npoints,FIT_NPTS_POW));
}

float fit_amplitude_only(float ftol)
{
  
  int i,j,window_start,npoints,ntries;
  float resid=0.0, amp, diff, minresid, bestamp, ampstop;

  window_start = envelopes->window_start_seconds / envelopes->GFdelta;
  npoints = envelopes->fit_npoints;

  j = (window_start-CF_begin)+(npoints/2);
  i = npoints/2;
  diff = *(envelopes->envelope_data+j) - *(envelopes->GFenvelope+i);
  amp = diff;
  ntries=500;
  ampstop = amp + ftol*ntries;
  amp = amp - ftol*ntries;
  minresid = 1e20;
  bestamp = amp;
  while(amp < ampstop) {
    j = window_start-(CF_begin/envelopes->GFdelta);
    resid = 0.0;
    for(i=0;i<npoints;i++) {
      diff = *(envelopes->GFenvelope+i)+amp - *(envelopes->envelope_data+j);
      resid = resid + diff*diff;
      j++;
    }

    if(sqrt(resid) < minresid) {
      minresid = sqrt(resid);
      bestamp = amp;
      envelopes->fit_residual = sqrt(resid)/npoints;
    }
    amp = amp + ftol;
  }
    return bestamp;
}

