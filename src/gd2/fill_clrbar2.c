
#include <stdio.h>
#include <stdlib.h>

#include "gd2.h"
#include "debug.h"

#include "sgfcolor.h"

char *fill_clrbar2(npseudocolors, width, npricolors, ndefcolors, nerr)

int npseudocolors;   /* number of pseudocolors in the colortable */
int width;       /* number of elements in one scan line of the color bar */
int npricolors;  /* number of SAC primary colors */
int ndefcolors;  /* number of default colors in the colortable */
int *nerr;
{


unsigned int k;
int ibyte, i, j;
char *array;

 UNUSED(ndefcolors);
 UNUSED(npricolors);

/* allocate memory for colorbar byte array */
  if((array = (char *)malloc(3*width*npseudocolors*sizeof(char)))
                    == NULL){
    printf("error allocating colorbar byte array--fill_clrbar2\n");
    *nerr = 301;
    goto L_8888;
  };


/*  Color bar  fill an entire scan line with one color */

  *nerr = 0;

  k = npseudocolors-1;
  ibyte = 0;
  for (i=0; i< npseudocolors; i++){
     for (j=0; j< width; j++){
       array[ibyte++ ] = sred[k];  
       array[ibyte++ ] = sgreen[k];  
       array[ibyte++ ] = sblue[k];  
     }
     k--;
  }


L_8888:
	return array;

} /* end of function */






