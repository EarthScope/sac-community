
#include <stdio.h>
#include <stdlib.h>

#include "xyz.h"
#include "gdm.h"
#include "bool.h"

void
frange(float *z, int n, float *zmin, float *zmax) {
  int i;
  float xmin, xmax;
  
  xmin =  VLARGE;
  xmax = -VLARGE;

  for(i = 0; i < n; i++) {
    if(z[i] > xmax) { xmax = z[i]; }
    if(z[i] < xmin) { xmin = z[i]; }
  }
  *zmin = xmin;
  *zmax = xmax;
}

void
fbinary(float *z, int n) {
  int i;
  for(i = 0; i < n; i++) {
    z[i] = (z[i] > 0.0) ? 1.0 : 0.0;
  }
}

