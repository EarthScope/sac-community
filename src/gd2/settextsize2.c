
#include <stdio.h>
#include <string.h>

#include "gd2.h"
#include "gem.h"
#include "gam.h"

extern display_t sgf;

void 
settextsize2(float width, float height)
{
	int nerr;



	/*=====================================================================
	 * PURPOSE:  To change size of graphics text for device 2 (SGF).
	 *=====================================================================
	 * INPUT ARGUMENTS:
	 *    WIDTH:   The width of a single character in viewport units. [f]
	 *    HEIGHT:  The height of a single line in viewport units. [f]
	 *=====================================================================
	 * MODULE/LEVEL:  GD2/4
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    GD2:     MOPHWS, JFBMAX, XW
	 *=====================================================================
	 * GLOBAL OUTPUT;
	 *    GD2:     MFBUF, JFBPNT
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *   SACLIB:   FLUSHBUFFER2
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    831026:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  861017
	 *===================================================================== */
	/* PROCEDURE: */
	/* - Write hardware text size opcode into SGF buffer. */
	Mfbuf[cmgd2.jfbpnt] = MOPHWS;
	Mfbuf[cmgd2.jfbpnt + 1] = 2;
	Mfbuf[cmgd2.jfbpnt + 2] = width*XW;
	Mfbuf[cmgd2.jfbpnt + 3] = height*XW;
	cmgd2.jfbpnt = cmgd2.jfbpnt + 4;

	/* - Flush buffer if necessary. */

	if( cmgd2.jfbpnt > JFBMAX )
		flushbuffer2( &nerr );

       
	return;

} /* end of function */


void
text_box_sgf(textbox *t) {
  int i, s;
  float len, xlen;
  float x, y;
  int style;

  if(!t || t->n == 0) {
    return;
  }
  len = 0;

  x = t->x;
  y = t->y;

  if(t->location & TEXT_BOX_RIGHT) {
    for(i = 0; i < t->n; i++) {
      xlen = 0.0;
      if(t->text[i]) {
      getstringsize( t->text[i], strlen(t->text[i]), &xlen);
      }
      if(xlen > len) {
        len = xlen;
      }
    }
    x = x - len;
  }
  if(t->location & TEXT_BOX_LOWER ) {
    y = y + cmgem.chht * (t->n - 1);
  } else {
    y = y - cmgem.chht;
  }

  if(t->use_style && t->location & TEXT_BOX_LEFT) {
      x += (0.5 * cmgem.chwid) + (TEXTBOX_LINE_LENGTH_PIXELS) / (SGF_PIXELS(XW));
  }
  
  getlinestyle( &style );

  s = 1;
  sgf.set_line_style( &s );
  sgf.set_line_width( LINE_WIDTH_THIN );

  /* Set text size */
  gettextsize( &cmgem.chwid, &cmgem.chht );
  settextjust("LEFT", "BOTTOM");

  for(i = 0; i < t->n; i++) {
    sgf.set_color( t->color[i] );
    sgf.move( x, y );
    set_position(x, y);
    if(t->text[i]) {
        sgf.text( &sgf, t->text[i], strlen(t->text[i]) );
    }
    if(t->use_style) {
        text_box_line(&sgf, x, y, t->width[i], t->style[i],
                      cmgem.chwid, cmgem.chht);
    }
    if(t->use_symbol) {
        text_box_symbol(&sgf, x, y, t->symbol[i], t->use_style,
                        cmgem.chwid, cmgem.chht);
    }
    y = y - cmgem.chht;
  }

  sgf.set_line_style( &style );
  sgf.set_line_width( cmgem.iwidth );

  /* Color for Skeleton */
  setcolor2( color_on() ? color_skeleton() : color_foreground_default() );
}
