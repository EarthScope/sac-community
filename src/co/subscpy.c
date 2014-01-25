/** 
 * @file   subscpy.c
 * 
 * @brief  Copy a string 
 * 
 */
#include <string.h>

#include "co.h"

/** 
 * Copy a string from \p from to \p to, starting at index \p start and
 *    finishing at index \p end
 * 
 * @param to 
 *    Destination string
 * @param start 
 *    Index to start at, use 0 if < 0
 * @param end 
 *    Index to end at, use \plen - 1 < 0 or if > \p len
 * @param len 
 *    Length of \p from
 * @param from 
 *    Source string
 * @return 
 *    Destination string
 *
 */
char *
subscpy(char *to,
	int   start,
	int   end,
	int   len,
	char *from) {

	/* null terminates the target string, the end of
	   the target string is determined from "to+len"! ! !
	   The substring bounds assume C range (i.e., start at 0).
	 */
        int n, ncopy, fromlen, tolen;


	if( to == NULL || from == NULL ) return( NULL );

        if (start < 0) start = 0;
        
        if ((end < 0) || (end > len-1) ) end = len-1;

        n = end - start + 1;
        if ( n < 0 ) n = 0;

        fromlen = strlen(from);

        tolen = strlen(to);

        if (tolen < start) memset(to+tolen, (int)' ', start-tolen);

        if ( fromlen < n ) {
           ncopy = fromlen;
           memset(to+start+ncopy, (int)' ', n-ncopy);
        }
        else  ncopy = n;

        memcpy(to+start,from,ncopy);
        to[len] = '\0';

	return( to );
}
