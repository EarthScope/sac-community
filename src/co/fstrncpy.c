/** 
 * @file   fstrncpy.c
 * 
 * @brief  Copy a string
 * 
 */

#include <string.h>

#include "co.h"

/** 
 * Copy a string from \p from to \p to.  Spaces are padded at the end
 *    of the to string
 * 
 * @param to 
 *    Where to place the string
 * @param tolen 
 *    Length of \p to
 * @param from 
 *    Where to copy the string from
 * @param fromlen 
 *    Length of \p from
 * 
 * @return 
 *    The copied string
 *
 * @bug Should be replaced with strncpy() or equivalent
 *
 */
char *
fstrncpy( char *to, 
	  int   tolen, 
	  char *from, 
	  int   fromlen ) {
   int cpylen;

   if( to == NULL || from == NULL || tolen <= 0 || fromlen <= 0 )
      return( NULL ) ;

   cpylen = fromlen ;
   if( fromlen > tolen ) cpylen = tolen ;

   memcpy( to, from, cpylen ) ;
   if( cpylen < tolen )
      memset( to+cpylen, (int)' ', tolen - cpylen ) ;
   to[ tolen ] = '\0' ;

   return( to ) ;
}
