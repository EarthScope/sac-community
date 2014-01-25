/** 
 * @file   decont.c
 * 
 * @brief  Decode a Content field
 * 
 */

#include "dfm.h"
#include "bool.h"


#include "msg.h"
#include "bot.h"

/** 
 * Decode the Content field from an alphanumeric data file
 * 
 * @param kcont 
 *    Content field
 *      The CONTENT field from an alphanumeric data file describes what 
 *      to do with each token on each data card in the file.  Each 
 *      character in the CONTENT field signifies a different type of
 *      output channel.  The details of the CONTENT token are documented
 *      in the description of the READALPHA command.
 * @param kcont_s 
 *    Length of \p kcont
 * @param maxch 
 *    Maximum number of allowed channels
 * @param numch 
 *    Total number of output channels
 * @param numxch 
 *    Number of x output channels, either 0 or 1
 * @param numych 
 *    Number of y output channels
 * @param iopch 
 *    Array of indices giving type of output channel
 *    - -1 ignore the jth channel
 *    -  0 jth channel is the x channel
 *    -  n jth channel is the nth y channel
 * @param ltoend 
 *    - TRUE if the last token in \p kcont was a "."
 *           denoting continue previous field to end of the line
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   920713:  Changed to include one label field.  Added a common block 
 *                  to contain this label field index.
 * @date   910807:  Changed local variable nstart to nchstart to avoid
 *                  conflict with global nstart in debugger.
 * @date   870722:  Added descriptive error messages.
 * @date   860910:  Original version.
 *
 */
void 
decont(char *kcont, 
       int   kcont_s, 
       int   maxch, 
       int  *numch, 
       int  *numxch, 
       int  *numych, 
       int  *iopch, 
       int  *ltoend, 
       int  *nerr) {

	char kop, krep;
	int jdx, jc, nc, nchstart, nrep;

	int *const Iopch = &iopch[0] - 1;

	*nerr = 0;

	/* - Determine number of characters in content information. */
	nc = indexb( kcont,kcont_s );

	/* - Initialize counters and flags. */
	*numch = 0;
	*numxch = 0;
	*numych = 0;
	*ltoend = FALSE;
	jc = 1;

	/* - Initialize IOPCH array to IGNORE opcode. */
	for( jdx = 1; jdx <= maxch; jdx++ ){
	    Iopch[jdx] = -1;
	}

	/* - Main loop on characters in content information. */
	while ( jc <= nc && !*ltoend ){

	    /* -- Extract next two characters from string. */
	    kop = kcont[jc - 1];
	    krep = kcont[jc];

	    /* -- Decode second character first. This may be the repeat count.
	     *    (A period denotes repeat to end of line.) */
	    if( krep == '.' ){
		*ltoend = TRUE;
	    }

	    else if ( krep >= '1' && krep <= '9' ) {
		nrep = krep - '0' ;
		jc += 2 ;
	    }

	    else{
		nrep = 1;
		jc++ ;
	    }

	    /* -- Decode first character.  This is the operation code.
	     *--- "X" signifies the x channel. */
		/* D (radial distance) added for traveltime */
	    if( kop == 'X' || kop == 'D' ){
		nchstart = *numch + 1;
		if( *ltoend ){
		    *numch = maxch;
		}
		else if( (nchstart + nrep - 1) <= maxch ){
		    *numch = nchstart + nrep - 1;
		}
		else{
		    *nerr = 1359;
		    setmsg( "ERROR", *nerr );
		    apimsg( maxch );
		    goto L_8000;
		}
		for( jdx = nchstart; jdx <= *numch; jdx++ ){
		    *numxch = *numxch + 1;
		    Iopch[jdx] = 0;
		}
	    }

	    /* --- "Y" signifies the y channel.  T (time) added for traveltime. */
	    else if( kop == 'Y' || kop == 'T' ){
		nchstart = *numch + 1;
		if( *ltoend ){
		    *numch = maxch;
		}
		else if( (nchstart + nrep - 1) <= maxch ){
		    *numch = nchstart + nrep - 1;
		}
		else{
		    *nerr = 1359;
		    setmsg( "ERROR", *nerr );
		    apimsg( maxch );
		    goto L_8000;
		}
		for( jdx = nchstart; jdx <= *numch; jdx++ ){
		    *numych = *numych + 1;
		    Iopch[jdx] = *numych;
		}
	    }

	    /* --- "L" signifies that this channel has the label associated with it */
	    else if( kop == 'L' ){
		if( *ltoend || (nrep > 1) ){
		    *nerr = 1359;
		    setmsg( "ERROR", *nerr );
		    apimsg( maxch );
		    goto L_8000;
		}
		nchstart = *numch + 1;
		*numch = nchstart;
		Iopch[nchstart] = -2;
	    }

	    /* --- "I" signifies that this channel is to be ignored (i.e. skipped.) */
	    else if( kop == 'I' ){
		nchstart = *numch + 1;
		if( *ltoend ){
		    *numch = maxch;
		}
		else if( (nchstart + nrep - 1) <= maxch ){
		    *numch = nchstart + nrep - 1;
		}
		else{
		    *nerr = 1359;
		    setmsg( "ERROR", *nerr );
		    apimsg( maxch );
		    goto L_8000;
		}
		for( jdx = nchstart; jdx <= *numch; jdx++ ){
		    Iopch[jdx] = -1;
		}
	    }

	    /* --- "N" signifies that this is the next y channel. */
	    else if( kop == 'N' ){
		nchstart = *numch + 1;
		if( *ltoend ){
		    *nerr = 1360;
		    setmsg( "ERROR", *nerr );
		    apcmsg( &kop,1 );
		    goto L_8000;
		}
		else if( (nchstart + nrep - 1) <= maxch ){
		    *numch = nchstart + nrep - 1;
		}
		else{
		    *nerr = 1359;
		    setmsg( "ERROR", *nerr );
		    apimsg( maxch );
		    goto L_8000;
		}
		for( jdx = nchstart; jdx <= *numch; jdx++ ){
		    *numych = *numych + 1;
		    Iopch[jdx] = *numych;
		}
	    }

	    /* --- "P" signifies that the next two are a x-y pair. */
	    else if( kop == 'P' ){
		nchstart = *numch + 1;
		if( *ltoend ){
		    *numch = maxch - 1;
		}
		else if( (nchstart + 2*nrep - 1) <= maxch ){
		    *numch = nchstart + 2*nrep - 2;
		}
		else{
		    *nerr = 1359;
		    setmsg( "ERROR", *nerr );
		    apimsg( maxch );
		    goto L_8000;
		}
		for( jdx = nchstart; jdx <= *numch; jdx += 2 ){
		    *numxch = *numxch + 1;
		    *numych = *numych + 1;
		    Iopch[jdx] = 0;
		    Iopch[jdx + 1] = *numych;
		}
		*numch = *numch + 1;
	    }

	    /* --- "R" signifies that the next two are a y-x (reversed) pair. */
	    else if( kop == 'R' ){
		nchstart = *numch + 1;
		if( *ltoend ){
		    *numch = maxch - 1;
		}
		else if( (nchstart + 2*nrep - 1) <= maxch ){
		    *numch = nchstart + 2*nrep - 2;
		}
		else{
		    *nerr = 1359;
		    setmsg( "ERROR", *nerr );
		    apimsg( maxch );
		    goto L_8000;
		}
		for( jdx = nchstart; jdx <= *numch; jdx += 2 ){
		    *numxch = *numxch + 1;
		    *numych = *numych + 1;
		    Iopch[jdx] = *numych;
		    Iopch[jdx + 1] = 0;
		}
		*numch = *numch + 1;
	    }

	    /* --- Illegal character detected. */
	    else{
		*nerr = 1360;
		setmsg( "ERROR", *nerr );
		apcmsg( &kop,1 );
		goto L_8000;
	    }

	} /* end while ( jc <= nc && !*ltoend ) */


	/* - Process any error that might have occurred. */

L_8000:
	if( *nerr > 0 ){
	    setmsg( "ERROR", *nerr );
	    if( *nerr == 1 ){
		apcmsg( "Illegal character = ",21 );
		apcmsg( &kop,1 );
		apcmsg( "found on CONTENT card",22 );
	    }
	    else if( *nerr == 2 ){
		apcmsg( "Exceeded maximum number of output channels =" ,45 );
		apimsg( maxch );
	    }
	}

}

