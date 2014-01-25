
#include <stdio.h>

#include "gdm.h"

/** 
 * Determine the width of a text string
 *
 * @param ktext
 *   Text string
 * @param ntext
 *   Length of \p ktext
 * @param width
 *   Width of text string in viewport coordinates on output
 *
 * @date   861017:  Original version.
 *
 */
void 
getstringsize(char *ktext,
              int ntext,
              float *width)
{
	int ich, j;
	float wfactr;

	/* - If software quality text is on, compute string width. */
	if( cmgdm.ltsoft ){
		/* -- Determine width factor and initialize string length. */
		wfactr = cmgdm.twidth/(float)( cmgdm.iofset );
		*width = 0.;

		/* -- For each character in string.
		 * --- Convert character to it's index number in font array.
		 * --- Sum up incremental character lengths. */
		for( j = 1; j <= ntext; j++ ){
			ich = ( ktext[j - 1] );
			if( ich > 128 )
				ich = ich - 128;
			*width = *width + (Stxmax[ich] - Stxmin[ich])*wfactr;
                }

        } else {
          /* - Otherwise, compute hardware (monospaced) text width. */
          *width = (float)( ntext )*cmgdm.twidth;
        }

}

