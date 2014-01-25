#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../inc/extfunc.h"

/* #define NULL 0 */
int setup_data(call_data, fyinput, fxinput, numfiles, nptsmax)
sac_files **call_data;
float **fyinput;
float **fxinput;
int *numfiles;
int *nptsmax;
{
 sac_files *input;
 sac_header *hdr;
 float *xdata, *ydata;
 float *fydata, *fxdata;
 int npts, i, error, leven, j;

 input = *call_data;

/* Get the number of input files. */
 *numfiles = input->nfiles;

/* Create a two dimensional array for each         */
/* of the Y and X data components.  This           */
/* array will be dimensioned (nptsmax,nfiles).     */

/* First find the max number of input points.      */
 *nptsmax = -1;

 for( i=0; i< *numfiles; i++){
   hdr = input->ext_hdrs[i];
   npts = getnhdr(hdr, "npts", &error);
   if( error != 0 ){
     printf("Error accessing input headers\n");
     return 1;
   }
   *nptsmax = (npts > *nptsmax) ? npts: *nptsmax;
 }

/* Allocate the storage for y and x data to pass to FORTRAN routine. */
 *fyinput = (float *) malloc(*numfiles * *nptsmax * sizeof(float));
 *fxinput = (float *) malloc(*numfiles * *nptsmax * sizeof(float));
 if( (*fyinput == NULL) || (*fxinput == NULL) ){
   printf("Error allocating storage for x and y data\n");
   return 1; 
 }


 fydata = *fyinput;
 fxdata = *fxinput;

/* Fill the data arrays to pass in to the FORTRAN routine. */
 for( i=0; i<*numfiles; i++ ){
/* Set a pointer to the input y data. */
  ydata = input->ext_yvalues[i];
  xdata = input->ext_xvalues[i];

/* Set a pointer to the header.       */
  hdr   = input->ext_hdrs[i];

/* Get the number of points from the header. */
  npts = getnhdr(hdr, "npts", &error);
  if( error != 0 ){
    printf("Error getting NPTS field from input header\n");
    return 1;
  }

/* Is this an evenly spaced file? */
  leven = getlhdr(hdr, "leven", &error);
  if( error != 0 ){
    printf("Error getting LEVEN field from input header\n");
    return 1;
  }

/* Load data values. */
  for( j=0; j<npts; j++ ){
    *fydata++ = *ydata++;
    if(leven) *fxdata++ = 0.0;
    else      *fxdata++ = *xdata++;
  }

/* Fill with zeroes. */
  for( j=npts; j<*nptsmax; j++ ){
    *fydata++ = 0.0;
    *fxdata++ = 0.0;
  }

  free(input->ext_yvalues[i]);
  free(input->ext_xvalues[i]);

 }

 return 0;
}

int setup_args(argc, argv, fargs, lenarg)
int argc;
char **argv;
char **fargs;
int *lenarg;
{
 /* concatenate the input args as a single command line */
 /* suitable for passing to a FORTRAN routine.          */ 
 char *temp;
 int i;

 *lenarg = 0;

 /* determine space requirements. */
 for( i=0; i<argc; i++) *lenarg += strlen(argv[i]) + 1;

 if((*fargs = malloc(*lenarg + 1)) == NULL) return 1;

 temp = *fargs;
 *temp = '\0';

 for( i=0; i<argc; i++) {
  strcat(temp, argv[i]);
  strcat(temp, " ");
 }

/* eliminate the trailing NULL to make a FORTRAN string. */
 temp[*lenarg] = ' ';

 return 0;
}

int retrieve_data(call_data, fyinput, fxinput, numfiles, nptsmax)
sac_files **call_data;
float *fyinput;
float *fxinput;
int numfiles;
int nptsmax;
{
  sac_files *output;
  sac_header *hdr;
  float *fydata, *fxdata;
  float *ydata, *xdata;

  int i, npts, error, leven, j;

  output = *call_data;

/* Load the output data into the struct. */
  for( i=0; i<numfiles; i++ ){

    fydata = fyinput + (i * nptsmax);
    fxdata = fxinput + (i * nptsmax);

    hdr   = output->ext_hdrs[i];

    npts = getnhdr(hdr, "npts", &error);
    if( error != 0 ){
      printf("Error getting NPTS field from output header\n");
      return 1;
    }

    leven = getlhdr(hdr, "leven", &error);
    if( error != 0 ){
      printf("Error getting LEVEN field from output header\n");
      return 1;
    }

/* Allocate memory for output data. */
    if((ydata = (float *) malloc(npts * sizeof(float))) == NULL){
      printf("Error allocating output ydata\n");
      return 1;
    }
    
    if( !leven ) {
      if((xdata = (float *) malloc(npts * sizeof(float))) == NULL){
        free(ydata);
        printf("Error allocating output xdata\n");
        return 1;
      }
    }
    else
      xdata = NULL;

    output->ext_yvalues[i] = ydata;
    output->ext_xvalues[i] = xdata;


/* Load output data values. */
   for( j=0; j<npts; j++ ){
     *ydata++ = *fydata++;
     if( !leven ) *xdata++ = *fxdata++;      
   }   
  }

  free(fyinput);
  free(fxinput);

  return 0;

}
