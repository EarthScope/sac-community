
#include <stdlib.h>
#include <string.h>

#include "ncpf.h"
#include "bool.h"
#include "hdr.h"
#include "amf.h"


#include "bot.h"
#include "ucf.h"
#include "dfm.h"
#include "msg.h"
#include "cpf.h"
#include "dff.h"
#include "debug.h"
#include "bool.h"

int
gettime(int lmax, int lvalue, double tvalue, double *value) {

	int j, ndx1, ndx2, nlen, npt;
  int nerr;
  float *Sacmem1, *Sacmem2;

	/*=====================================================================
	 * PURPOSE: Returns the time offset in the file for the first occurence
	 *          of a given value, or the offset coresponding the the first
	 *          MAXIMUM or MINUMUM data value in the file.
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    kfunction:  The full gettime command string [c].
	 *    nc:         Length of command line string [i].
	 *    ic:         Pointer to next token in command [i].
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    kvalue:     Returned time offset, as a string.
	 *    nerr:       Nonzero if error occurs.
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    lmax:       True if looking for first value greater or equal [l].
	 *    lmin:       True if looking for first value less than or equal [l].
	 *    lvalue:     True if a target value is supplied [l].
	 *    tvalue:     Target value to search for.
	 *    value:      Located data point offset relative to begin time [f].
	 *    npt:        Point number where target value was located [n].
	 *    nlen:       Length of data file [i].
	 *    ndx1:       Pointer to first component [i].
	 *    ndx2:       Pointer to second component (unevenly spaced file) [i].
	 *=====================================================================
	 * MODULE/LEVEL:  SERVICE/4
	 *=====================================================================
	 * MODIFICATION HISTORY:
         *    970129:  Add parameter (0) to cnvatf.  0 means that if a string
         *             of digits is too long, let it slide by.  maf 
	 *    920325:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  
	 *===================================================================== */
  nerr = 0;
	npt = 0;

	/* CHECKING PHASE: */

	/* - Check for null data file list. */

	vflist( &nerr );
	if( nerr != 0 ) {
    return nerr;
  }

	/* - Get the first file from the memory manager */

	getfil( 1, TRUE, &nlen, &ndx1, &ndx2, &nerr );
	if( nerr != 0 ) {
    return nerr;
  }

  Sacmem1 = cmmem.sacmem[ndx1];
  if (!*leven) {
    Sacmem2 = cmmem.sacmem[ndx2];
  }
  
	if( ! lvalue ){
    if( lmax ) {
      tvalue = *depmax;
    } else {
      tvalue = *depmin;
    }
  }

  for( j = ndx1; j <= (ndx1 + nlen - 1); j++ ){
    npt = npt + 1;
    if( (  lmax && *(Sacmem1++) >= tvalue ) ||
        ( !lmax && *(Sacmem1++) <= tvalue ) ) {
      if( *leven ){
        *value = *b + (*delta * (npt - 1));
			} else {
        *value = *(Sacmem2 + npt - 1);
			}
      return FALSE;
    }
  } 
  if(lmax) {
    error(8201, ": %g > %g (depmax)", tvalue, *depmax);
  } else {
    error(8201, ": %g < %g (depmin)", tvalue, *depmin);
  }
	return 8201;
}

