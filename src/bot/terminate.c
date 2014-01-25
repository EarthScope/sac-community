/** 
 * @file   terminate.c
 * 
 * @brief  Terminate a space padded string
 * 
 */

#include <ctype.h>
#include <string.h>


/** 
 * Takes a space-padded string, and terminates it at the first space.
 * 
 * @param paddedString 
 *   String to be terminated
 * 
 * @return 
 *    - 0 if terminated successfully
 *    - 1 if no padding found. 
 *
 * @note Requires that string be terminated at the end of the padding.
 *
 */
void 
terminate (char * paddedString) {

    int nLen ;

    for ( nLen = strlen ( paddedString ) - 1 ; 
	  isspace ( paddedString[ nLen ] ) ;
	  nLen-- )
      paddedString[ nLen ] = '\0' ;
}
