/** 
 * @file
 *
 * @brief Declares functions in usrlib
 *
 */
 
#include <stdio.h>
#include "config.h"
 
#ifdef MISSING_FUNC_TOLOWER
#define tolower(c)     ((c>='A' && c<='Z') ? (c+('a'-'A')) : c)
#endif /* MISSING_FUNC_TOLOWER */

#ifdef MISSING_FUNC_TOUPPER
#define toupper(c)     ((c>='a' && c<='z') ? (c-('a'-'A')) : c)
#endif /* MISSING_FUNC_TOUPPER */



