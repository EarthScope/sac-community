/** 
 * @file   zshft.c
 * 
 * @brief  Shift a signal in place
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "dbh.h"

/** 
 *  Subroutine to shift a signal in-place with zero filling.
 *
 * @param signal 
 *    Real array containing signal to be shifted.          
 * @param n
 *    Length of the \p signal.
 * @param ishft
 *    Integer number of samples to shift the signal.       
 *    - ISHFT > 0 implies a shift to the right.            
 *    - ISHFT < 0 implies a shift to the left.             
 *
 * @author  Dave Harris
 *
 * @date November 17, 1981  Created
 *
 */
void 
zshft(float *signal, 
      int    n, 
      int    ishft) {

       int k, nhalt;

	float *const Signal = &signal[0] - 1;




	/* Shift specified too large.   */
	if( labs( ishft ) > n ){

	  fprintf( stdout, "*** ZSHFT - SPECIFIED SHIFT TOO LARGE ***\n" );

	}
	else if( ishft < 0 ){

	  /*   Left shift.  */
	  /*  Shift array entries to the left.  */
	  nhalt = n + ishft + 1;
	  k = 1;
	L_6:

	  if( k == nhalt )
	    goto L_7;
	  Signal[k] = Signal[k - ishft];
	  k = k + 1;
	  goto L_6;
	L_7:

	  
	  /*  Zero high end of array */
	  zero( &Signal[n + ishft + 1], -ishft );
	  
	}
	else if( ishft > 0 ){
	  
	  /*  Shift array entries to the right.  */
	  k = n;
	L_8:

	  if( k == ishft )
	    goto L_9;
	  Signal[k] = Signal[k - ishft];
	  k = k - 1;
	  goto L_8;
	L_9:

	  
	  /* Zero low end of array. */
	  zero( signal, ishft );
	  
	}
	return;
}

