
#include <math.h>

#include "mach.h"
#include "co.h"
#include "gdm.h"

#ifdef USE_X11
#include "gd3.x11.h"
#endif

/** 
 * Display a text stirng using software font
 *
 * @param out
 *    Display Structure containing drawing commands [move,draw,stroke]
 * @param ktext
 *    Text string to display
 * @param ntext
 *    Length of \p ktext
 *
 * @date   861103:  Added check to make sure rotation is needed.
 * @date   861017:  Major restructuring.
 * @date   831026:  Original Version.
 *
 */

void 
softwaretext(display_t *out,
             char *ktext,
             int ntext)
{
	int italic, rotate;
	int ich, index, j, jopcode, jstroke, jstxmn, jstxmx, 
	 jxdel, jydel;
	float hfactr, rotrad, sqxy, theta, wfactr, xdel, xsav, 
	 ydel, ysav;

        float hsave;

        float xmin,xmax,ymin,ymax;

        /*
	 * LOCAL VARIABLES:
	 *    rotate:  .TRUE. if text is to be rotated from horizontal. [l]
	 *    italic:  .TRUE. if text is to be italicized. [l]
	 *    [xy]sav: Starting point for each character. [f]
	 *    jstroke: Current stroke from stroke table. [i]
	 *             Each stroke has a delta-location (x and y) and an opcode
	 *             encoded into it.
	 *    jopcode: Op code for current stroke. [i]
	 *             = 0 Move to location.
	 *             = 1 Draw to location.
	 *             = 2 Final move for this character.
	 *    j[xy]del: Delta location in normalized units. [i]
	 *    [xy]del:  Delta location scaled to current character size. [f]
         */

	/* - Convert current values of width, height, and angle to the
	 *   units needed by this subroutine. */

        hsave = cmgdm.thgt;

	wfactr = cmgdm.twidth/cmgdm.iofset;
	hfactr = cmgdm.thgt/cmgdm.iyhigh;

	rotate = cmgdm.tangle != 0.0;
	if( rotate )
		rotrad = cmgdm.tangle*TORAD;
	italic = (cmgdm.nfont%2) == 0;

        /* Text Justification */
        {
          float swidth, shgt, del;
          /* - Determine string extent. */
          getstringsize( ktext, ntext, &swidth );
          shgt = cmgdm.thgt;
          
          xdel = 0.;
          if( cmgdm.ihjust == 2 ){
            xdel = -0.5*swidth;
          }
          else if( cmgdm.ihjust == 3 ){
            xdel = -swidth;
          }
          
          ydel = 0.;
          if( cmgdm.ivjust == 2 ){
            ydel = -0.5*shgt;
          }
          else if( cmgdm.ivjust == 3 ){
            ydel = -shgt;
          }
          /* - Apply rotation of offsets. */
          
          if( cmgdm.tangle != 0. && (xdel != 0. || ydel != 0) ){
            del = sqrt( powi(xdel,2) + powi(ydel,2) );
            theta = atan2( ydel, xdel );
            xdel = del*cos( theta + TORAD*cmgdm.tangle );
            ydel = del*sin( theta + TORAD*cmgdm.tangle );
          }

          /* Justify Text */
          out->move(cmgdm.xold + xdel, cmgdm.yold + ydel);
          set_position(cmgdm.xold + xdel, cmgdm.yold + ydel);
        }

	/* - For each character in text string. */

	for( j = 1; j <= ntext; j++ ){
          xmin = 0;
          xmax = 0;
          ymin = 0;
          ymax = 0;
		/* -- Convert ASCII value into its integer equivalent. */
		ich = ( ktext[j - 1] );
		if( ich > 128 )
			ich = ich - 128;

		/* -- Get index in stroke table for character and its width limits. */
		index = Ascstr[ich];
		jstxmn = Stxmin[ich];
		jstxmx = Stxmax[ich];

		/* -- Keep character starting point because all values in stroke
		 *    table use this reference point. */
		xsav = cmgdm.xold;
		ysav = cmgdm.yold;

		/* -- For each stroke in this character.
		 * --- Get opcode and x and y location from stroke table. */
L_40:
		jstroke = Stroke[index];
		jopcode = jstroke/4096;
		jxdel = jstroke/64 - jopcode*64;
		jydel = jstroke - 64*(jstroke/64);
		jxdel = jxdel - jstxmn;
		jydel = jydel - cmgdm.iyoff;
		/* --- Multiply by width and heigth factors. */
                
		xdel = jxdel*wfactr;
		ydel = jydel*hfactr;

                if(xdel > xmax) { xmax = xdel; }
                if(xdel < xmin) { xmin = xdel; }
                if(ydel > ymax) { ymax = ydel; }
                if(ydel < ymin) { ymin = ydel; }

		/* --- If "italic" font, slant the character. */
		if( italic )
			xdel = xdel + 0.414*ydel;
		/* --- Do rotation. */
		if( rotate ){
			sqxy = pow(powi(xdel,2) + powi(ydel,2),.5);
			theta = atan( ydel/xdel );
			xdel = sqxy*cos( theta + rotrad );
			ydel = sqxy*sin( theta + rotrad );
			}
		/* --- Depending on opcode do move or draw to delta location. */
		if( jopcode == 0 || jopcode == 2 ) {
                  out->move( xsav + xdel, ysav + ydel );
                  set_position(xsav + xdel, ysav + ydel);
                }
		else{
                  out->draw( xsav + xdel, ysav + ydel );
                  set_position(xsav + xdel, ysav + ydel );
                }

		/* -- Do next stroke. */
		index = index + 1;
		if( jopcode <= 1 )
			goto L_40;

                if(out->stroke) {
                  out->stroke();
                }

		/* -- Final stroke for this character has been reached.
		 *    Move to next character location. */
		jxdel = jstxmx - jstxmn;
		ydel = 0.;
		xdel = jxdel*wfactr;

		if( rotate ){
			ydel = xdel*sin( rotrad );
			xdel = xdel*cos( rotrad );
			}
		out->move( xsav + xdel, ysav + ydel );
                set_position(xsav + xdel, ysav + ydel );

		/* - Do next character. */
		}
        cmgdm.thgt = hsave;
	return;
}

