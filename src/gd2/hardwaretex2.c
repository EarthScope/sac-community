

#include <string.h>

#include "gd2.h"
#include "gdm.h"
#include "co.h"
#include "bool.h"


#include "gtm.h"

void /*FUNCTION*/ hardwaretext2(ktext, ntext)
char *ktext;
int ntext;
{
	short int icl, icr;
	int j, nerr, nw;
	static short is8 = 8;
        float xloc, yloc, unused, xvpmin, xvpmax,
              xvsmin, xvsmax, xwcmin, xwcmax;
        float xfactor, xpsize;
        int ixloc, iyloc;

	/*=====================================================================
	 * PURPOSE:  To write a hardware text string to device 2 (SGF).
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    KTEXT:   Text string. [c]
	 *    NTEXT:   Number of characters in text string. [i]
	 *=====================================================================
	 * MODULE/LEVEL:  GD2/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    GD2:     MOPHWT, JFBMAX
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    GD2:     MFBUF, JFBPNT
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  IZSHFT, FLUSHBUFFER2
	 *    F77:     ICHAR
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    861017:  Original version.
         *    940304:  Ammon, SLU changed to output justification info
	 *=====================================================================
	 * DOCUMENTED:  861017
	 *===================================================================== */
	/* - For the file device the MOPHWT opcode (= -5) indicates hardware text
	 *   will follow.  The next two 16 bit words are the number of words in this
	 *   command and the number of characters in the text.
	 *   Then the text is buffered out two characters per 16 bit word. */
        
        xloc = cmgdm.xold;
        yloc = cmgdm.yold;

        if(cmgd2.encodesize){
          getvport(&xvpmin, &xvpmax, &unused, &unused);
          getvspace(&xvsmin, &xvsmax, &unused, &unused);
          xfactor = (xvsmax-xvsmin)/(xvpmax-xvpmin);

          if(strncmp(kmgd2.sizetype,"FIXED",5) == 0) {
              xpsize = cmgd2.sizevalue * xfactor;
	  } else if(strncmp(kmgd2.sizetype,"SCALED",6) == 0){
              getworld(&xwcmin, &xwcmax, &unused, &unused);
              xpsize = cmgd2.sizevalue * (xwcmax-xwcmin) * xfactor;
	  } else {
              xpsize = 10.0;
	  }
          
          Mfbuf[cmgd2.jfbpnt] = MOPSIZ;
          Mfbuf[cmgd2.jfbpnt+1] = 1;
          Mfbuf[cmgd2.jfbpnt+2] = (int)fmin((100.0*xpsize),XW);
          cmgd2.jfbpnt += 3;
          cmgd2.encodesize = FALSE;
        }
 
/* Scale floating point values to device coordinates */

        ixloc = (int)(xloc*XW);
        iyloc = (int)(yloc*XW);


	nw = ntext/2;
	if( nw*2 != ntext )
		nw = nw + 1;

	Mfbuf[cmgd2.jfbpnt] = MOPHWT;
	Mfbuf[cmgd2.jfbpnt + 1] = nw + 6;
	Mfbuf[cmgd2.jfbpnt + 2] = cmgdm.ihjust;
	Mfbuf[cmgd2.jfbpnt + 3] = cmgdm.ivjust;
	Mfbuf[cmgd2.jfbpnt + 4] = ixloc;
	Mfbuf[cmgd2.jfbpnt + 5] = iyloc;
	Mfbuf[cmgd2.jfbpnt + 6] = (int)(cmgdm.tangle*10.0);
	Mfbuf[cmgd2.jfbpnt + 7] = ntext;
	cmgd2.jfbpnt = cmgd2.jfbpnt + 8;

	for( j = 1; j <= ntext; j += 2 ){
		icl = ( ktext[j - 1] );
		icl = izshft( &icl, &is8 );
		icr = ( ktext[j] );
		Mfbuf[cmgd2.jfbpnt] = ( icl | icr );
		cmgd2.jfbpnt = cmgd2.jfbpnt + 1;
		}

	/* - Flush buffer if necessary. */

	if( cmgd2.jfbpnt > JFBMAX )
		flushbuffer2( &nerr );

       
	return;

} /* end of function */

