/** 
 * @file   crsac.c
 * 
 * @brief  Create a new file in SAC memory
 * 
 */

#include "dfm.h"
#include "amf.h"
#include "hdr.h"


#include "ucf.h"
#include "dff.h"
#include "debug.h"
/** 
 * Create a new file in SAC memory
 * 
 * @param idfl 
 *    Data file list index number
 * @param ncmp 
 *    Number of data components
 * @param nlen 
 *    Length of each data component
 * @param ndxh 
 *    Index in sacmem array of header
 * @param ndx1 
 *    Index in sacmem array of first data component
 * @param ndx2 
 *    Index in sacmem array of second data component
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    
 * @date   850731:  Changes due to new memory manager.
 *                  CHANGED NUMBER AND ORDER OF ARGUMENTS.
 * @date   840120:  Cleaned up and documented.
 * @date   821222:  Added zeroing of data arrays.
 * @date   810120:  Changed to output message retrieval from disk.
 *
 */
void 
crsac(int  idfl, 
      int  ncmp, 
      int  nlen, 
      int *ndxh, 
      int *ndx1, 
      int *ndx2, 
      int *nerr) {

	int jcomp;
  float *d;
  sac *s;
	*nerr = 0;
  UNUSED(ndxh);
  UNUSED(ndx1);
  UNUSED(ndx2);
  s = sacget(idfl, TRUE, nerr);

  s->h->npts = nlen;

	/* - For each data component: */
	//Nlndta[idfl] = nlen;
	//Ncomp[idfl] = ncmp;
	for( jcomp = 1; jcomp <= sac_comps(s); jcomp++ ){

		/* -- Allocate memory block. */
    d = (float *) malloc(sizeof(float) * s->h->npts);
    if(!d) {
      goto ERROR;
    }
    if(jcomp == 1) {
      s->y = d;
    } else {
      s->x = d;
    }

	}

	/* - Initialize header and data components to default values. */
	newhdr();
	fill( s->y, s->h->npts, 0. );
	s->h->leven = ncmp == 1;
	if( ! s->h->leven ) {
		fill( s->x, s->h->npts, 0. );
  }

	return;
 ERROR:
  return;
}

