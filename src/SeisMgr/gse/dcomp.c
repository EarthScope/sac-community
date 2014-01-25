#include <stdio.h>
#include <math.h>
#include <stdlib.h>

/* Translated from Fortran by I. Henson, Dec. 1995
 */

void remdif1( int *data, int npts)
{
	/* Remove Data-compression (first differences)
	 *
	 * Urs Kradolfer, January 1990
	 */
	int	i;

	for(i = 1; i < npts; i++) data[i] += data[i-1];
}
	
/******************************************************************************
*    The following data compression and decompression routines have
*    been developped by Dr. Shane Ingate and Dr. Ken Muirhead, at the
*    Australian Seismological Centre, Bureau of Mineral Resources,
*    Canberra, Australia.
*    They provided me with these routines in March 1989 during the
*    session of the Group of Scientific Experts (GSE) in Geneva.
*    These compression/decompression algorithms are used by all members
*    of the GSE during the large-scale data exchange experiment GSETT-2,
*    carried out from 1989 to 1991.
*    It is recommended to use second differences and the six-bit compression.
*    The second differences can be computed by two subsequent calls of
*    subroutine DIF1 (see above).
*    These routines are already running on many different machines.
*    Because the AND function is not standard FORTRAN 77, this operation
*    has been moved to a separate subroutine INTAND. All users of these
*    routines will have to modify INTAND so that it performs correctly on
*    their computers.
*                                 Urs Kradolfer, Swiss Seismological Service
*
******************************************************************************/

/***********************************************************************
*                                                                      *
*     dcomp6(buf, data)						       *
*                                                                      *
*     subroutine decompress integer data that has been compressed      *
*     into ascii characters and returns values in int format 	       *
*     see subroutine cmprs6 for compression format                     *
*								       *
*     input - buf null terminated string			       *
*     return value is the length of *data			       *
***********************************************************************/

int
dcomp6( char *buf, int **data)
{
	static int ch[128] = 
	{
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 2, 
		 3, 4, 5, 6, 7, 8, 9,10,11, 0, 0, 0, 0, 0, 0, 0, 
		12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,
		28,29,30,31,32,33,34,35,36,37, 0, 0, 0, 0, 0, 0,
		38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,
		54,55,56,57,58,59,60,61,62,63, 0, 0, 0, 0, 0, 0
	};
	int	i, j, k, ibyte, joflow, jsign, itemp;
	int	isign = 16, ioflow = 32, mask1 = 15, mask2 = 31;
        int     len;
        int     *tmp;
	union
	{
		char	a[4];
		int	i;
	} ai;

	ai.a[0] = '1';
	ai.a[1] = '2';
	ai.a[2] = '3';
	ai.a[3] = '4';

	/* work out which way bytes are stored in computer
	 */
	ibyte = (ai.i == (((52*256+51)*256+50)*256+49)) ? 0 : 3;

        len = 2;
        *data = (int *) malloc(sizeof(int) * len);
        if(*data == NULL) {
          fprintf(stderr, "dcomp6: malloc error\n");
          return -1;
        }
	/* start of decoding
	 */
	for(i = j = 0; buf[i] != '\0'; i++)
	{
		while(buf[i] != '\0' &&
			(buf[i] == '\n' || buf[i] == '\r')) i++;
    if(buf[i] == '\0' || buf[i] == ' ') {
      return(j);
    }
		ai.i = 0;
		ai.a[ibyte] = buf[i];

		/* strip off any higher order bits */
		k = ai.i & 127;

		/* get number representation of input character */
		ai.i = ch[k-1];

		/* get sign bit */
		jsign = ai.i & isign;

		/* get continuation bit (if any) */
		joflow = ai.i & ioflow;

		/* remove bits we don't want */
		itemp = ai.i & mask1;

		while(joflow != 0)
		{
			/* there is another byte in this sample */
			itemp *= 32;
			i++;
			while(buf[i] != '\0' &&
				(buf[i] == '\n' || buf[i] == '\r')) i++;
      if(buf[i] == '\0' || buf[i] == ' ') {
        return(j);
      }

			ai.a[ibyte] = buf[i];
			/* strip off any higher order bits */
			k = ai.i & 127;
			ai.i = ch[k-1];
			
			/* get continuation bit (if any) */
			joflow = ai.i & ioflow;
			k = ai.i & mask2;
			itemp += k;
		}

		if(jsign != 0) itemp = -itemp;

    if(j >= len) {
      len = len * 2; 
      tmp = (int *) realloc(*data, sizeof(int) * len);
      if(!tmp) {
        fprintf(stderr, "dcomp6: malloc error.\n");
        return(-1);
      }
      *data = tmp;
    }
		(*data)[j++] = itemp;
	}
	return(j);
}

/************************************************************************
*                                                                       *
*     dcomp7(buf, data)							*
*                                                                       *
*     decompress integer data that has been compressed			*
*     into 7 bit characters and returns values in int format.		*
*     see subroutine cmprs7 for compression format.                     *
************************************************************************/

int
dcomp7( char *buf, int **data)
{
	int	isign = 32, ioflow = 64, mask1 = 31, mask2 = 63;
	int	icorr = 32, mask3 = 127, ncntrl = 64;
	int	i, j, k, iend, itemp, joflow, jsign, correction, ibyte;
	union
	{
		char	a[4];
		int	i;
	} ai;

	ai.a[0] = '1';
	ai.a[1] = '2';
	ai.a[2] = '3';
	ai.a[3] = '4';

	/* work out which way bytes are stored in computer
	 */
	ibyte = (ai.i == (((52*256+51)*256+50)*256+49)) ? 0 : 3;

	/* start of decoding
	 */
	for(i = -1, j = 0 ; buf[i+1] != '\0'; )
	{
		iend = 0;
		i++;
		ai.a[ibyte] = buf[i];

		/* remove most significant bit - not used */
		ai.i = ai.i & mask3;

		/* test if character is equal to ncntrl */
		if(ai.i == ncntrl) iend = 1;

		/* get sign bit */
		jsign = ai.i & isign;

		/* get continuation bit */
		joflow = ai.i & ioflow;

		/* remove sign and continuation bits */
		itemp = ai.i & mask1;

		correction = 0;
		while(joflow != 0.)
		{
			/* there is another byte in this sample */
			i++;
			ai.a[ibyte] = buf[i];

			/* remove most significant bit - not used */
			ai.i = ai.i & mask3;

			/* test if character is equal to ncntrl
			 * bit pattern 1000000 followed by
			 * 1000000 means end of data.
			 */
			if(iend == 1 && ai.i == ncntrl) return(j);

			if(iend == 1 && ai.i < 16)
			{
				/* See if previous sample has to be modified.
				 */
				itemp = icorr;
				/* subtract correction from previous sample */
				if((*data)[j-1] < 0) itemp = -itemp;
				(*data)[j-1] -= itemp;
				correction = 1;
				break;
			}
			else
			{
				iend = 0;

				/* get continuation bit */
				joflow = ai.i & ioflow;
				k = ai.i & mask2;

				/* shift what we have 6 bits left and
				 * add in new bits
				 */
				itemp = itemp*64 + k;
			}
		}
		if(correction) continue;

		if(jsign != 0) itemp = -itemp;

		if((j == 0 && (*data = (int *)malloc(sizeof(int))) == NULL) ||
		   (*data = (int *)realloc(*data, (j+1)*sizeof(int))) == NULL)
		{
			fprintf(stderr, "dcomp6: malloc error.\n");
			return(-1);
		}
		(*data)[j++] = itemp;
	}
	return(j);
}
		
/************************************************************************
*     dcomp8(buf, data)							*
*									*
*     decompress integer data that has been compressed			*
*     into 8 bit characters and returns values in int format.		*
*     see subroutine cmprs8 for compression format.                     *
************************************************************************/

int
dcomp8( char *buf, int **data)
{
	int	isign = 64, ioflow = 128, mask1 = 63, mask2 = 127;
	int	ncntrl = 128, icorr = 32;
	int	i, j, k, iend, itemp, jsign, joflow, ibyte, correction;
	union
	{
		char	a[4];
		int	i;
	} ai;

	ai.a[0] = '1';
	ai.a[1] = '2';
	ai.a[2] = '3';
	ai.a[3] = '4';

	/* work out which way bytes are stored in computer
	 */
	ibyte = (ai.i == (((52*256+51)*256+50)*256+49)) ? 0 : 3;


	for(i = -1, j = 0; buf[i+1] != '\0'; )
	{
		iend = 0;
		i++;
		ai.a[ibyte] = buf[i];

		/* test if character is equal to ncntrl (100000000) */
		if(ai.i == ncntrl) iend = 1;

		/* get sign bit */
		jsign = ai.i & isign;

		/* get continuation bit */
		joflow = ai.i & ioflow;

		/* remove sign and continuation bits */
		itemp = ai.i & mask1;

		correction = 0;
		while(joflow != 0)
		{
			/* there is another byte in this sample */
			i++;
			ai.a[ibyte] = buf[i];

			/* test if character is equal to ncntrl
			 * if two control characters in a row then end of data.
			 */
			if(iend == 1 && ai.i == ncntrl) return(j);

			/* see if previous sample has to be modified
			 */
			if(iend == 1 && ai.i < 16)
			{
				itemp = icorr;
				/* now subtract correction from previous sample
				 */
				if((*data)[j-1] < 0) itemp = -itemp;
				(*data)[j-1] -= itemp;
				correction = 1;
				break;
			}
			else
			{
				iend = 0;

				/* get continuation bit */
				joflow = ai.i & ioflow;

				/* mask off continuation bit */
				k = ai.i & mask2;

				/* shift what we have so far 7 bits left and
				 * add in next bits
				 */
				itemp = itemp*128 + k;
			}
		}
		if(correction) continue;

		if(jsign != 0) itemp = -itemp;

		if((j == 0 && (*data = (int *)malloc(sizeof(int))) == NULL) ||
		   (*data = (int *)realloc(*data, (j+1)*sizeof(int))) == NULL)
		{
			fprintf(stderr, "dcomp6: malloc error.\n");
			return(-1);
		}
		(*data)[j++] = itemp;
	}
	return(j);
}
