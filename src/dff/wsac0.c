/** 
 * @file   wsac0.c
 * 
 * @brief  Write a SAC file
 * 
 */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "amf.h"
#include "dff.h"
#include "hdr.h"
#include "msg.h"
#include "co.h"
#include "ucf.h"
#include "SacHeader.h"
#include "proto.h"
#include "errors.h"
#include "bool.h"
extern sac *CURRENT;
/** 
 * Determine the byte order of the machine
 * 
 * @return 
 *    - ENDIAN_BIG 
 *    - ENDIAN_LITTLE
 */
int
CheckByteOrder() {
  static int byte_order = ENDIAN_UNKNOWN;
  short int word = 0x0001;
  char *byte = (char *) &word;
  if(byte_order == ENDIAN_UNKNOWN) {
    byte_order = (! byte[0]) ? ENDIAN_BIG : ENDIAN_LITTLE;
  }
  return byte_order;
}

int
sac_check_npts(int npts) {
  if(npts <= 0) {
    return ERROR_WRITING_FILE;
  }
  return SAC_OK;
}

int
sac_check_lovrok(int lovrok) {
  if(!lovrok) {
    return ERROR_OVERWRITE_FLAG_IS_OFF;
  }
  return SAC_OK;
}

int
sac_byte_order(int getset) {
  int i, n;
  int byte_order;
  char *env_string;
  char *env_big[]    = {"big", "solaris", "powerpc", "ppc" };
  char *env_little[] = {"little", "x86", "intel" };
  static int swap = -1; /* Default to System Byte Order */

  if(getset < 0 && swap == -1) { /* Initial Call, Get */
    env_string = getenv("SAC_WRITE_BYTE_ORDER");
    getset = CheckByteOrder();
    if(env_string != NULL) {
      n = strlen(env_string);
      for(i = 0; i < (int)(sizeof(env_big)/sizeof(char *)); i++) {
        if(strncasecmp(env_string, env_big[i], min(n, strlen(env_big[i]))) == 0) {
          getset = ENDIAN_BIG;
        }
      }
      for(i = 0; i < (int)(sizeof(env_little)/sizeof(char *)); i++) {
        if(strncasecmp(env_string, env_little[i], min(n, strlen(env_little[i]))) == 0) {
          getset = ENDIAN_LITTLE;
        }
      }
    }
  }
  if(getset >= 0) { /* Set, Initial or later call */
    byte_order = CheckByteOrder();    
    swap = ! (byte_order == getset);
  }
  return swap;
}


/** 
 * Write a Sac Header 
 *
 * @param nun
 *    Logical file unite to write SAC Header To
 * @param nerr
 *    Error return Flag
 *    - SAC_OK
 *    - Non-Zero on Error
 *
 * @date November 7, 2010 
 */
void
sac_header_write(int nun, float *hdr, char *khdr, int swap, int *nerr) {
  int n;
  /* These are here because zwabs only reads in floats, which is really dumb
     The writes should be done straight away using fread() */
  float temp2[SAC_HEADER_STRINGS_SIZE_FILE];
  
  if(swap) {
    sac_header_swap(hdr);
  }
  /* Write the numerical values */
  n = write(-nun, hdr, SAC_HEADER_NUMBERS_SIZE_BYTES_FILE);

  if(n != SAC_HEADER_NUMBERS_SIZE_BYTES_FILE) {
    *nerr = ERROR_WRITING_FILE;
    return;
  }
  if(swap) {
    sac_header_swap(hdr);
  }
  
  map_chdr_out((float *)khdr,temp2);
  n = write(-nun, temp2, SAC_HEADER_STRINGS_SIZE_BYTES_FILE);
  if(n != SAC_HEADER_STRINGS_SIZE_BYTES_FILE) {
    *nerr = ERROR_WRITING_FILE;
    return;
  }
  return;
}

void
sac_data_write1(int nun, float *data, int npts, int swap, int *nerr) {
  int n;
  if(swap) {
    sac_data_swap(data, npts);
  }
  n = write(-nun, data, npts * SAC_DATA_SIZE);
  if(n != npts * SAC_DATA_SIZE){
    *nerr = ERROR_WRITING_FILE;
    return;
  }
}

void
sac_data_write2(int nun, float *y, float *x, int npts, int swap, int *nerr) {
  sac_data_write1(nun, y, npts, swap, nerr);
  if(*nerr) {
    return;
  }
  sac_data_write1(nun, x, npts, swap, nerr);
}

void
sac_data_write(int nun, float *y, float *x, int comps, int npts, int swap, int *nerr) {
  if(comps == 1) {
    sac_data_write1(nun, y, npts, swap, nerr);
  } else if(comps == 2) {
    sac_data_write2(nun, y, x, npts, swap, nerr);
  } else {
    *nerr = ERROR_WRITING_FILE;
  }
  return;
}

void
sac_write_internal(sac *s, char *filename, int write_data, int lswap, int *nerr, int report) {
  int nun, ncerr;
  nun = 0;
  if(write_data) {
    /* Make sure the number of points is bigger than zero */
    if((*nerr = sac_check_npts(s->h->npts)) != SAC_OK) {
      goto L_8888;
    }
  }

  /* Check overwrite-protect flag in header record */
  if((*nerr = sac_check_lovrok(s->h->lovrok)) != SAC_OK) {
    goto L_8888;
  }

	/* Update the Variables describing the dependent variable array*/
	extrma(s->y, 1, s->h->npts, &s->h->depmin, &s->h->depmax, &s->h->depmen);

	/* Recompute the distance, azimuth, etc if proper header fields are present */
	update_distaz(s);

  if(write_data) {
    znfile(&nun, filename, strlen(filename)+1, "DATA", 5, nerr);
    if(*nerr) {
      goto L_9999;
    }
  } else {
    zopen_sac( &nun, filename,strlen(filename)+1, "DATA",5, nerr );
    if( *nerr != SAC_OK ) {
      goto L_9999;
    }
    lswap = s->m->swap;
  }
	/* - Write the header */
  sac_header_write(nun, (float *)s->h, (char *) &(s->h->kstnm), lswap, nerr);
  if(*nerr != SAC_OK) {
    goto L_8888;
  }

	/* - Write the data */
  if(write_data) {
    sac_data_write(nun, s->y, s->x, sac_comps(s), s->h->npts, lswap, nerr);
    if(*nerr != SAC_OK) {
      goto L_8888;
    }
  }

 L_8888:
  if(*nerr != SAC_OK) {
    if(report) {
      error(*nerr, "%s", filename);
    }
  }
 L_9999:
	zclose( &nun, &ncerr );
}

void
sac_write(sac *s, char *filename, int write_data, int lswap, int *nerr) {
  sac_write_internal(s, filename, write_data, lswap, nerr, FALSE);
}
void
sac_write_r(sac *s, char *filename, int write_data, int lswap, int *nerr) {
  sac_write_internal(s, filename, write_data, lswap, nerr, TRUE);
}



/** 
 * Write a SAC file to disk using the current header values
 * 
 * @param kname 
 *    Name of the file to write
 * @param xarray 
 *    Array containing the independent variable, e.g. Time
 *    Not used if the data is evenly spaced.
 *    Array containing the indepdent variable, e.g. Time
 * @param yarray 
 *    Array containing the dependent variable, e.g. Amplitude
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Succces
 *    - ERROR_OVERWRITE_FLAG_IS_OFF
 *
 * @param kname_s 
 *    Length of \p kname
 *
 * @date   2007:    Created from wsac0() by adding a routine to update the distance and azimuth
 * @date   961031:  ninf and nhst were changed to norid and nevid for
 *                  compatability with the CSS format.  maf 961031
 * @date   870513:  Changed call to wrtxtd to wrtmsg.
 * @date   840118:  Deleted call to ZTRUNC.
 * @date   830125:  Changes due to modified header common block.
 * @date   820118:  Added logic to truncate file before closing.
 * @date   810120:  Changed to output message retrieval from disk.
 * @date   800821:  Original version [Prime].
 *
 */
void 
wsac0(char  *kname, 
      float *xarray, 
      float *yarray, 
      int   *nerr, 
      int    kname_s) {

  int nderr;
  int swap;
  sac *s;
	char *kname_c;
  float *x,*y;

  s = CURRENT;

  sacio_initialize_common();

	kname_c = fstrdup(kname, kname_s);
	kname_s = strlen(kname_c) + 1;

	*nerr = 0;

  /* Determine if swapping is necessary */
  swap = sac_byte_order(-1);

	/* If file exists, remove */
	zdest( kname_c,kname_s, &nderr );
  clrmsg();

  x = s->x;
  y = s->y;
  s->x = xarray;
  s->y = yarray;
  /* Write the file */
  sac_write(s, kname_c, TRUE, swap, nerr);
  s->x = x;
  s->y = y;

	if( *nerr != 0 ) {
    outmsg();
    clrmsg();
  }
	free(kname_c);
	kname_c = NULL;

	return;
}




/* Wrapper to make the function more convenient for FORTRAN programmers. */

void wsac0_ (char      *kname, 
	     float     *xarray,
	     float     *yarray, 
	     int       *nerr, 
	     int        kname_s) {
  wsac0 ( kname , xarray , yarray , nerr , kname_s ) ;
}
void wsac0__ (char      *kname, 
	      float     *xarray,
	      float     *yarray, 
	      int       *nerr, 
	      int        kname_s) {
  wsac0 ( kname , xarray , yarray , nerr , kname_s ) ;
}
