/** 
 * @file   strscpy.c
 * 
 * @brief  Copy a string
 * 
 */

#include <string.h>

#include "co.h"

/** 
 * Copy a string from \p from to \p to
 * 
 * @param to 
 *   Destination string
 * @param from 
 *   Source string
 * @param l 
 *   Dharaceters to copy
 * 
 * @return 
 *   Destination string
 *
 * @note Wrapper around strncpy()
 * @bug Same as strlcpy()
 * 
 */
char *
strscpy(char *to, 
	char *from, 
	int   l) {
  int len;

  if ((to==NULL) || (from==NULL) || (l <= 0)) return NULL;

  len = strlen(from);
  if ( l < len ) len = l;
 
  strncpy(to,from,len);
 
  to[len] = '\0';

  return to ;
}

