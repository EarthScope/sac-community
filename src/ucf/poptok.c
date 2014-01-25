/** 
 * @file   poptok.c
 * 
 * @brief  Pop a token off a text string
 * 
 */

#include <string.h>

#include "ucf.h"
#include "tok.h"
#include "bool.h"

#include "co.h"

/** 
 * Pop the next token off a text string
 * 
 * @param ktext 
 *    Text string to tokenize and pop
 * @param nctext 
 *    Length of \p ktext
 * @param icpntr 
 *    Pointer to the next character in string to examine. Use 1 to
 *    initialize the process.
 *    On output, pointer to the next character in \p ktext.  Do not change
 *    the value in between calls.
 * @param icloc1 
 *    Location of the first character in the new token
 * @param icloc2 
 *    Location of the last character in the new token
 * @param itype 
 *    Type of token
 *    - 0 if no tokens are left.
 *    - 1 for a simple token
 *    - 2 for a token delimiter (other than blank.)
 *    - 3 for a message.  First character is message delimiter.
 *
 * @note Local Variables
 *   - KCHAR:   Character currently being examined.
 *   - LMSG:    Set to .TRUE. while searching for the end of a message.
 *   - KCURDL:  Set to the message delimiter being searched for.
 *
 * @note Assumptions 
 *   - A blank is always considered a token delimiter.
 *   - Multiple blanks are not significant, except inside a message.
 *   - The end of line is considered a token or message delimiter.
 *   - Subroutine TOKDEL defines all other token and message delimiters.
 *
 * @date   900425:  Fixed bug when passing in a blank line. (VAX/VMS bug fix)
 * @date   820930:  Added logic to return token type.
 * @date   820419:  Original version.
 * @date   900425:  Documented/Reviewed
 *
 */
void 
poptok(char  *ktext, 
       int    nctext, 
       int   *icpntr, 
       int   *icloc1, 
       int   *icloc2, 
       int   *itype)
{
	char kcurdl[9];
	int  lmsg;
	char kchar;
	int   _l0;

	/* - Upon entry, ICPNTR points to the next character to be analyzed. */
	/* - If this is first pass through string (ICPNTR<=1), squeeze out
	 *   any leading blanks.  Trailing blanks are squeezed out below
	 *   after each token has been located. */
	if( *icpntr <= 1 ){
	    *icpntr = 1;
	    while( *icpntr <= nctext ){
		kchar = ktext[*icpntr - 1];
		if( kchar == ' ' ) *icpntr = *icpntr + 1;
		else break ;
	    }
	}

	/* - Initialize various counters and flags. */

	lmsg = FALSE;
	*icloc1 = *icpntr;
	*itype = 0;

	/* - Loop on each character: */

	while( *icpntr <= nctext ){
	    kchar = ktext[*icpntr - 1];

	    /* -- If we are in the middle of a message:
	     *    (1) Check for second occurance of message delimiter.
	     *    (2) Check for end of text string. */
	    if( lmsg ){
		if( memcmp(&kchar,kcurdl,1) == 0 ){
		    *icloc2 = *icpntr - 1;
		    *itype = 3;
		    break ;
		}
		else if( *icpntr == nctext ){
		    *icloc2 = *icpntr;
		    *itype = 3;
		    break ;
		}

	    }

	    /* -- A blank is always a token delimiter (except within messages)*/
	    else if( kchar == ' ' ){
		*icloc2 = *icpntr - 1;
		*itype = 1;
		break ;
	    }

	    /* -- Check for other token delimiters.
	     *    If this is the first character found this time, it is 
	     *    returned as the token.  Otherwise it signifies the end
	     *    of the token before it. */
	    else{
		_l0 = nccomp( &kchar, kmtok.ktokdl, 1, cmtok.ntokdl, 1 ) > 0;
		if( _l0 ){
		    if( *icloc1 == *icpntr ){
			*icloc2 = *icpntr;
			*itype = 2;
		    }
		    else{
			*icloc2 = *icpntr - 1;
			*icpntr = *icpntr - 1;
			*itype = 1;
		    }
		    break ;
		}

		/* -- Check for a message delimiter.
		 *    If this is the first character found this time, begin the
		 *    search for a matching delimiter to signify the end of the
		 *    message.  Otherwise, it serves as a token delimiter.   */
		else{
		    _l0 = nccomp( &kchar, kmtok.kmsgdl,1, cmtok.nmsgdl, 1 ) > 0;
		    if( _l0 ){
			if( *icloc1 == *icpntr ){
			    lmsg = TRUE;
			    fstrncpy( kcurdl, 8, (char *)&kchar, 1 );
			}
			else{
			    *icloc2 = *icpntr - 1;
			    *icpntr = *icpntr - 1;
			    *itype = 1;
			    break ;
			}
		    }

		    /* - Last character in text string is handled differently.
		     *   End token even if no delimiter has been found. */

		    else if( *icpntr == nctext ){
			*itype = 1;
			*icloc2 = *icpntr;
			break ;
		    }
		}
	    }

	    /* -- Increment character index and loop. */
	    *icpntr = *icpntr + 1;
	}

	/* - After token has been located, squeeze out any extra blanks. */
	do{
	    *icpntr = *icpntr + 1;
	}while( *icpntr <= nctext && ktext[*icpntr - 1] == ' ' ) ;

	return;

}
