#include<stdio.h>
#include <string.h>

#include "smMemory/smMemory.h"

#define INTSTOREAD  16384
#define BYTESTOREAD (INTSTOREAD*sizeof (int))

int samples_per_block[6] = { 7,  3, 4,  5, 4, 1 };
int words_per_block[6]   = { 2,  1, 1,  2, 2, 1 };
int bits_per_sample[6]   = { 9, 10, 7, 12, 15, 28 };
int initial_shift[6]     = { 1,  2, 4,  4,  4,  4 };

static int e_decompress(unsigned int*, int*);

void enlarge(FILE *fp, int outMax, int* data, /* int * byteOff,*/ int* nerr)
/********************************************************************************
 *
 *    convert e1 compressed data to s4
 *
 ********************************************************************************/
{
   unsigned int *in = NULL;
   int inMax = 0;
   int idx = 0, rv = 0;
   int space = 10*INTSTOREAD;
   int spaceLeft = 10*BYTESTOREAD;
   int bytesRead;

   *nerr = 0;

   in = (unsigned int *) smCalloc (space, sizeof(int));
   if (in == NULL)
   {
       *nerr = 301;
       return;
   }
  
/*
   if ( fseek ( fp , *byteOff , SEEK_SET ) )
   {
	*nerr = 114 ;
	smFree ( in ) ;
	return ;
   }
*/

   do
   {
       if (spaceLeft < (int)BYTESTOREAD) 
       {
         unsigned int* temp;
   
         space += (5*INTSTOREAD);
         temp = (unsigned int *) smRealloc (in, space*sizeof(int));
         if (temp == NULL)
         {
             smFree (in);
             *nerr = 301;
             return;
         }
         in = temp;
         spaceLeft += 5*BYTESTOREAD;
       }
        bytesRead = fread( (char*)in + inMax, 1, BYTESTOREAD, fp);
       inMax += bytesRead;
       spaceLeft -= bytesRead;
   } while (bytesRead == BYTESTOREAD);

   while ( rv < outMax && (idx + 2)*sizeof(int) < (size_t)inMax) 
        {
       rv += e_decompress(in+idx, data+rv);
       for (idx += 2; !in[idx] && idx < inMax/4; idx++) { /* do nothing */ }
       if (rv > outMax)
       {
	  /* smFree ( in ) ;
	   *nerr = 402 ;
	   return ; */
	   break ;
       }
   }
   smFree (in);
   return;
}

/* static void b_rshift(unsigned int* input, int nwords, int size) */
/* { */
/*    register int  i; */
/*    unsigned int ltmp, rtmp; */
/*    unsigned int right  = size; */
/*    unsigned int left   = sizeof(int)*8 - size; */
   
/*    rtmp = input[nwords - 1] >> right; */

/*    for(i = nwords; i > 1; i--) */
/*    { */
/*    	ltmp = ((unsigned) input[i-2]) << left; */
/*    	input[i-1] = ltmp | rtmp; */
/*    	rtmp = (input[i-2]) >> size; */
/*    } */
   
/*    input[0] = rtmp; */
   
/*    return; */
/* } */

static void b_lshift(unsigned int* input, int nwords, int size)
{
   register int  i;
   unsigned int ltmp, rtmp;
   int left    = size;
   int right   = sizeof(int)*8 - size;
   
   ltmp = input[0] << left;

   for(i = 0; i < nwords - 1; i++)
   {
   	rtmp = ((unsigned) input[i+1]) >> right;
   	input[i] = ltmp | rtmp;
   	ltmp = (input[i+1]) << left;
   }
   
   input[nwords - 1] = ltmp;
   
   return;
}

/* static int b_rval(unsigned int input, int size) */
/* { */
/*    unsigned int val; */
/*    unsigned int mask = (1 << size) - 1; */
/*    unsigned int emask  = 0xffffffff << (size -1); */

/*    val = input & mask; */
/*    if(val & emask) val |= emask; */
   
/*    return(val); */
/* } */

static int b_lval(unsigned int input, int size)
{
   unsigned int val = input >> (sizeof(int)*8 - size);
   unsigned int emask  = 0xffffffff << (size -1);

   if(val & emask) val |= emask;
   
   return(val);
}

static int e_decompress(unsigned int* in, int* out)
{
   register int i, j, k;
   int  nd, ns;
   int  ms;
   int check;

   unsigned int code, idx;
   int *signed_out = (int *) out;

   /* lr   = (int) ((short *) (&in[0]))[0] / 4; */
   ns   = (int) ((short *) (&in[0]))[1];
   nd   = (int) ((char  *) (&in[1]))[0];
   
   /*
   * nd is now overloaded:
    *
   * first quartet has the number of diffs.
    *
   * second quartet has a code: 0 for "regular" e compression
    *
   *                            1 for uncompressed 32 bit ints
         *
         */

   switch(nd>>4)
   {
   case 0:
   	nd   &= 0xf;

   	b_lshift(&in[1], 1, 8);
   	check = b_lval(in[1], 24);

   	ms   = 0;
   	i    = 2;

   	while(ms < ns)
   	{
            code = in[i] >> 28;

            if(code < 8) idx = 0;
            else if (code < 0xc) idx = 1;
            else if(code >= 0xc) idx = 2 + (code & 0x3);
         
            b_lshift(&in[i], words_per_block[idx], initial_shift[idx]);

            for(j = 0; j < samples_per_block[idx]; j++)
            {
            	out[ms++] = b_lval(in[i], bits_per_sample[idx]);
            	b_lshift(&in[i], words_per_block[idx], bits_per_sample[idx]);
            }
            i += words_per_block[idx];
   	}
   
   	for(k = 0; k < nd; k++)
   	{
   	   for(j = 1; j < ms; signed_out[j] += signed_out[j-1], j++);
   	}

   	if(check != signed_out[ms-1]) return(-1);
   	else return(ms);

   	break;

   case 1:
   	memcpy(out, &in[2], ns*4);
   	return(ns);

   default:
   	return(-1);
   }
}
