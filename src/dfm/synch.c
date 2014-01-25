/** 
 * @file   synch.c
 * 
 * @brief  Synchronize a set of times
 * 
 */

#include "dfm.h"


#include "ucf.h"

/** 
 * Synchronize a set of times to the latest time
 * 
 * @param ndttmi 
 *    Current reference times, [\p num][6]
 * @param offsti 
 *    Current beginning times, [\pnum]
 * @param num 
 *    Number of times to synch
 * @param ndttmo 
 *    New reference times [\p num][6]
 * @param offsto 
 *    New beginning times
 * @param lbegin 
 *    Set beginning time of each to 0.0
 *
 * @date   810624:  Original version.
 *
 */
void 
synch(int     ndttmi[][6], 
      float  *offsti, 
      int     num, 
      int     ndttmo[][6], 
      float  *offsto, 
      int     lbegin) {

	int j, j_, jmx;
	float offstm;

	float *const Offsti = &offsti[0] - 1;

	float *const Offsto = &offsto[0] - 1;

	/* - Adjust input date time arrays (DTA) for input offsets. */
	for( j = 1; j <= num; j++ ){
	    j_ = j - 1;
	    idttm( &ndttmi[j_][0], Offsti[j], &ndttmo[j_][0] );
	}

	/* - Calculate the differences in DTA between the first and 
	 *   subsequent entries. Store differences in output offset array. 
	 *   If lbegin is set, set all offsets to 0.0
	 */
	Offsto[1] = 0.;
	if ( lbegin ) {
	    for( j = 2; j <= num; j++ ){
		Offsto[j] = 0. ;
	    }
	}
	else {
	    for( j = 2; j <= num; j++ ){
		j_ = j - 1;
		ddttm( &ndttmo[j_][0], &ndttmo[0][0], &Offsto[j] );
	    }
	}

	/* - Determine the largest difference. */
	jmx = 1;
	for( j = 2; j <= num; j++ ){
	    if( Offsto[j] > Offsto[jmx] )
		jmx = j;
	}
	offstm = Offsto[jmx];

	/* - All output DTA's have the DTA of the entry with 
	 *   the largest difference. */
	for( j = 1; j <= num; j++ ){
	    j_ = j - 1;
	    if( j != jmx )
		copyi( &ndttmo[jmx - 1][0], &ndttmo[j_][0], 6 );
	}

	/* - All output offsets are relative to this new output DTA. */
	for( j = 1; j <= num; j++ ){
	    Offsto[j] = Offsto[j] - offstm;
	}

	return;
}
