
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xyz.h"

void 
linear_interp(float *array,
              float xmin,
              float xmax,
              int nx,
              float *array_out,
              int newnx,
              int *nerr)

{
  float dx, newdx, xnew;
  float *xarray;
  int i, k;

  *nerr = 0;

  dx = (xmax - xmin)/(float)(nx-1);
  newdx = (xmax - xmin)/(float)(newnx-1);

  if((xarray = (float *)malloc(nx*sizeof(float))) == NULL){
    printf("memory allocation error in linear_interp\n");
    *nerr = 0301;
    return;
  }
  memset(xarray, 0, nx * sizeof(float));

  for(i=0; i<nx; i++){
    xarray[i] = xmin + ((float)i * dx);
  }

  xnew = xmin;
  array_out[0] = array[0];

  k = 0;

  for(i=1; i<(newnx-1); i++){
    xnew += newdx;
    while(xnew > xarray[k]) k++;
    if(k > nx){
      printf("internal error linear_interp\n");
      *nerr = 1;
      goto L_8888;
    }

    if(xnew == xarray[k]) array_out[i] = array[k];
    else {
      array_out[i] = (((dx - (xnew - xarray[k-1]))/dx) * array[k-1]) +
                     (((dx - (xarray[k] - xnew))/dx) * array[k]);
    } 
  }

  array_out[newnx-1] = array[nx-1];

L_8888:
  free(xarray);

  return;


}
