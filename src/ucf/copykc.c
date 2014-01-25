/** 
 * @file   copykc.c
 * 
 * @brief  Copy a character string
 * 
 */

#include "mach.h"
#include "ucf.h"

/** 
 * Copy a character array 
 * 
 * @param kin 
 *    Where to copy from
 * @param kin_s 
 *    Length of \p kin
 * @param ncopy 
 *    Number of characters to copy
 * @param kout 
 *    Where to copy to
 *
 * @bug  The copying of character arrays is confusing at best.  
 *
 * @date   900517:  Corrected declaration for kin to fix VAX VMS problem.
 * @date   830810:  Cleaned up and converted to indepenent subroutine.
 * @date   810000:  Original version.
 *
 */
void 
copykc(char *kin, 
       int   kin_s, 
       int   ncopy, 
       char *kout) { 

#define KIN(I_,J_)	(kin+(I_)*(kin_s)+(J_))

	int icin, icout, iwin;

	/* - KIN is a character array, each element being MCPW characters long.
	 *   KOUT is a character string of arbitrary length. */
	iwin = 1;
	icin = 1;

	/* - For each character being copied. */

	for( icout = 1; icout <= ncopy; icout++ ){

		/* -- Copy character to output string. */
		kout[icout - 1] = KIN(iwin - 1,0)[icin - 1];

		/* -- Increment input character and array counters. */
		icin = icin + 1;
		if( icin > MCPW ){
                  iwin = iwin + 1;
                  icin = 1;
                }
        }

	return;

#undef	KIN

} 
