/** 
 * @file   indexb.c
 * 
 * @brief  Find the length of a string
 * 
 */
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "bool.h"
#include "bot.h"
#include "co.h"

/** 
 * Find the length of a character string.  Length is define by the location
 *   of the last non-blank character.
 * 
 * @param string 
 *    Character string
 * @param string_s 
 *    Length of \p string
 * 
 * @return Index of the last non-blank character in the string
 *
 * @note Call general character search routine, searching for:
 *   - (1) the first non-occurance of a SPACE working backwards OR
 *   - (2) the first occurance of a NULL working forward.
 *
 * @date   870629:  Changed logic when checking for a NULL.
 * @date   850806:  Added check for NULL as well as SPACE.
 * @date   830527:  Original version.
 *
 */
int
indexb(char *string, 
        int   string_s) {
        char null_;
	int indexb_v, inull;

	indexb_v = indexa( string,string_s, ' ', FALSE, FALSE );
	null_ = '\000';
	inull = indexa( string,string_s, null_, TRUE, TRUE );
	if( inull > 0 )
		indexb_v = min( indexb_v, inull - 1 );

	return( indexb_v );
}


char *
strcut(char *in, unsigned int start, unsigned int end) {
    unsigned int n;
    char *out;
    n = end - start + 1;
    if(end < start) {
        fprintf(stdout, "strcut: end < start: '%s' [start: %d, end: %d]\n", in,start,end);
        return NULL;
    }
    out = malloc(n+1);
    strncpy(out, in + start - 1, n);
    out[n] = 0;
    return out;
}

char *
lstrip(char *s) {
        if(*s == 0) {
                return s;
        }
        while(isspace(*s)) {
                s++;
        }
        return s;
}

char *
rstrip(char *s) {
        char *back;
        if(*s == 0) {
                return s;
        }
        back = s + strlen(s) - 1;
        while(back >= s && isspace(*back)) {
          --back;
        }
        *(back+1) = 0;
        return s;
}
