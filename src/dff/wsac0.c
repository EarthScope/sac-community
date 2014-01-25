/** 
 * @file   wsac0.c
 * 
 * @brief  Write a SAC file
 * 
 */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "dff.h"
#include "hdr.h"
#include "msg.h"
#include "co.h"
#include "ucf.h"

#include "proto.h"
#include "errors.h"
#include "bool.h"

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
sac_data_write(int nun, float *y, float *x, int npts, int swap, int *nerr) {
  int n;
  if(swap) {
    sac_data_swap(y, npts);
  }
  n = write(-nun, y, npts * SAC_DATA_SIZE);
  if(n != npts * SAC_DATA_SIZE) {
    *nerr = ERROR_WRITING_FILE;
    return;
  }

  /* - If the data consists of two components (unevenly spaced or complex), write both. */
  if( *iftype == *itime || *iftype == *ixy || *iftype == *iunkn ) {
    if(*leven) {
      n = 1;
    } else {
      n = 2;
    }
  } else if(*iftype == *ixyz) {
    n = 1;
  } else {
    n = 2;
  }
  if(n == 2) {
    if(swap) {
      sac_data_swap(x, npts);
    }
    n = write(-nun, x, npts * sizeof(float));
    if((size_t) n != npts * sizeof(float)) {
      *nerr = ERROR_WRITING_FILE;
      return;
    }
  }
  return;
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

        int ncerr, nderr, nun;
        int swap;
	/* These are here because zwabs only reads in floats, which is really dumb
	   The reads should be done straight away using fread() */

	char *kname_c;

        nun = 0;

	kname_c = fstrdup(kname, kname_s);
	kname_s = strlen(kname_c) + 1;

	*nerr = 0;

        if(*npts <= 0) {
          *nerr = ERROR_WRITING_FILE;
          goto L_8888;
        }

	/* - Create the requested file and open it. */
	zdest( kname_c,kname_s, &nderr );
	znfile( &nun, kname_c,kname_s, "DATA",5, nerr );

	if( *nerr != 0 )
	    goto L_8888;

        /* Check overwrite-protect flag in header record */
	if(! *lovrok ) {
	  *nerr = ERROR_OVERWRITE_FLAG_IS_OFF;
	  setmsg("ERROR", *nerr);
	  apcmsg2(kname_c, kname_s);
	  outmsg();
	  clrmsg();
	  goto L_8888;
	}
	
	/* Update the Variables describing the dependent variable array*/
	extrma(yarray, 1, *npts, depmin, depmax, depmen);

	/* Recompute the distance, azimuth, etc if proper header fields are present */
	update_distaz();

        swap = sac_byte_order(-1);

	/* - Write the header to disk starting at word 0. */
        sac_header_write(nun, cmhdr.fhdr, (char *) kmhdr.khdr, swap, nerr);
        if(*nerr != SAC_OK) {
          goto L_8888;
        }
	/* - Write the array containing the dependent variable to disk
	 *   starting after the end of the header. */
        sac_data_write(nun, yarray, xarray, *npts, swap, nerr);
        if(*nerr != SAC_OK) {
          goto L_8888;
        }

	/* - Write any error message to terminal, close the disk file, and return. */
L_8888:
	if( *nerr != 0 )
	    outmsg();
	zclose( &nun, &ncerr );
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
