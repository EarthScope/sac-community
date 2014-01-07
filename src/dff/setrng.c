/** 
 * @file   setrng.c
 * 
 * @brief  Calculate and set the dependent variable range
 * 
 */

#include <math.h>

#include "amf.h"
#include "dff.h"
#include "gam.h"
#include "bool.h"
#include "dfm.h"
#include "hdr.h"
#include "co.h"


/** 
 * Calculate and set the dependent variable range for all files in 
 *    the data file list.  Find the maximum value from the depmin
 *    and depmax and save them to cmgam.rngmin and cmgam.rngmax.
 * 
 * @date   820818:  Original version.
 *
 */
void 
setrng() {

	int i, nerr;
  sac *s;
	/* - Initialize range variables. */
	cmgam.rngmin = VLARGE;
	cmgam.rngmax = -VLARGE;

	/* - For each file in DFL: */
	for( i = 0; i < saclen(); i++ ){
    if(!(s = sacget(i, FALSE, &nerr))) {
      return;
    }
		/* -- Adjust range variables. */
		cmgam.rngmin = fmin( cmgam.rngmin, s->h->depmin );
		cmgam.rngmax = fmax( cmgam.rngmax, s->h->depmax );
    DEBUG("%d %f %f\n", i, cmgam.rngmin, cmgam.rngmax );
	}

  //L_8888:
	return;

}
