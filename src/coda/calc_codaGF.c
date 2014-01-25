#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "coda.h"

void calc_codaGF(env,dist) 
     struct envelope *env;
     float dist;
{
  float time, b, CodaAmp, *GFenvptr;
  float A0 = 1.0, gamma=0.7;
  int i, npoints;
  float dist_b;

  if(dist > env->dist_critical)
    dist_b = env->dist_critical;
  else
    dist_b = dist;

  time=env->GFstart;
  npoints = env->GFnpts;
  A0=1.0;
  gamma=env->gamma;
  b=env->b0+(dist_b*env->b_slope);
  env->GFenvelope = (float *) malloc(npoints*sizeof(float));
  GFenvptr = env->GFenvelope;
  for(i=(int)time;i<npoints;i++) {
    time= time + env->GFdelta;
    CodaAmp= A0-(gamma*log10(time))+(b*time);
    /*    CodaAmp = CodaAmp + (env->dist_slope * dist); Old linear dist correction */
    CodaAmp = CodaAmp + ((env->dist_order1 * dist) + (env->dist_order2 * dist * dist));
    *GFenvptr++ = CodaAmp;
    
  }
  /*  fprintf(stderr, "A0=%f gam=%f b=%f dist=%f b0=%f bslope=%f time=%f inter=%f dslope=%f freq=%f\n", A0,gamma,b,dist,env->b0,env->b_slope,time,env->intercept,env->dist_slope,env->freq_low); */
}

