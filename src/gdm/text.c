
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "mach.h"
#include "gdm.h"
#include "gem.h"
#include "gtm.h"
#include "co.h"
#include "bool.h"
#include "debug.h"

/** 
 * Display a text string at the current plot location
 *
 * @param ktext
 *    Text string
 * @param ktext_s
 *    Length of \p ktext
 * @param nctext
 *    Number of characters to display
 *
 * @date   870416:  Deleted hardware text for graphics devices 3 and 4.
 * @date   861017:  Major retstructuring.
 * @date   850208:  Forced use of software text for rotated text requests.
 * @date   831026:  Original Version.
 *
 */
void 
text(char *ktext,
     int ktext_s,
     int nctext)
{
	int lrotated, ltemp;
	float swidth, xchar, xdel, ychar, ydel;
  UNUSED(ktext_s);

        int i, n;
        display_t **dev;
        n   = gdm_get_ndevices();
        dev = gdm_get_devices();

        /*
	 * LOCAL VARIABLES:
	 *    swidth:  Width in plot coordinates of text string. [f]
	 *    shgt:    Height of text string. [f]
	 *    [xy]del: Offset due to current text justification attribute. [f]
	 *    del:     Total offset due to text justification. [f]
	 *    theta:   Rotation angle of offset. [f]
	 *    [xy]char: Position of lower left hand corner of first character. [f]
         */

	/* - Determine string extent. */
	getstringsize( ktext, nctext, &swidth );

	/* - Determine relative move to make depending upon text justification.
	 *   (Left and bottom justification produces no relative move.) */


	/* - Plot text, making a relative move to account for text justification. */

	/* -- Software text is used when:
	 *    (1) software text has been requested
	 *    (2) hardware text requested but text is to be rotated.
         *        (unless writing sgf file (lgdon(2) = TRUE)         */
    ltemp = FALSE;
	lrotated = cmgdm.tangle < -VSMALL || cmgdm.tangle > VSMALL;
        if(cmgdm.ltsoft || lrotated ) ltemp = TRUE;

/* allow rotated hardware text if writing to SGF file */

        if( Lgdon[2] && (!cmgdm.ltsoft)) ltemp = FALSE;

	if( (ltemp) && (!cmgdm.lfhard) && FALSE){
        xdel = 0.0;
        ydel = 0.0;
		xchar = cmgdm.xold + xdel;
		ychar = cmgdm.yold + ydel;
		move( xchar, ychar );
                for(i = 0; i < n; i++) {
                  if(dev[i]->on) {
                    softwaretext( dev[i], ktext, nctext );
                  }
                }

		/* -- Unrotated hardware text.  Send to active graphics devices.
		 *    (Use software text for graphics devices 3 and 4.) */
        }
	else{
                if(!Lgdon[2]){ 
		  /* xchar = cmgdm.xold + xdel; */
		  /* xchar = fmax( 0., fmin( 1., xchar ) ); */
		  /* ychar = cmgdm.yold + ydel; */
		  /* ychar = fmax( 0., fmin( 1., ychar ) ); */
		  //move( xchar, ychar ); /* Devices need to incorporate this */
	        }
                for(i = 0; i < n; i++) {
                  if(dev[i]->on && dev[i]->text) {
                    dev[i]->text( dev[i], ktext, nctext );
                  }
                }
        }

	/* - Move back to current position. */

        //	if (!Lgdon[2]) move( xchar - xdel, ychar - ydel );

	return;

}

void
textbox_show(textbox *tbox) {
  int i, n;
  display_t **dev;
  n   = gdm_get_ndevices();
  dev = gdm_get_devices();
  
  for(i = 0; i < n; i++) {
    if(dev[i]->on && dev[i]->textbox) {
      dev[i]->textbox( tbox );
    }
  }
}

textbox *
textbox_new(int n) {
  int i;
  textbox *t;
  t = (textbox *) malloc(sizeof(textbox));
  t->n = n;

  /* Memory Allocation */
  t->text   = (char **) malloc(sizeof(char *) * t->n);
  t->color  = (int *)   malloc(sizeof(int)    * t->n);
  t->symbol = (int *)   malloc(sizeof(int)    * t->n);
  t->style  = (int *)   malloc(sizeof(int)    * t->n);
  t->width  = (int *)   malloc(sizeof(int)    * t->n);

  /* Initialization */
  for(i = 0; i < n; i++) {
    t->text[i] = NULL;
  }
  memset(t->color,  0, t->n * sizeof(int));
  memset(t->symbol, 0, t->n * sizeof(int));
  memset(t->style,  0, t->n * sizeof(int));
  memset(t->width,  0, t->n * sizeof(int));
  t->x          = 0.0;
  t->y          = 0.0;
  t->location   = TEXT_BOX_UPPER | TEXT_BOX_RIGHT;
  t->use_symbol = FALSE;
  t->use_style  = FALSE;

  return t;
}

textbox * 
textbox_copy(textbox *old) {
  int i;
  textbox *t;
  t = textbox_new( old->n );
  for(i = 0; i < old->n; i++) {
    if(old->text[i]) {
    t->text[i]   = strdup(old->text[i]);
    }
    t->color[i]  = old->color[i];
    t->symbol[i] = old->symbol[i];
    t->style[i]  = old->style[i];
    t->width[i]  = old->width[i];
  }
  t->x          = old->x;
  t->y          = old->y;
  t->location   = old->location;
  t->use_symbol = old->use_symbol;
  t->use_style  = old->use_style;
  return t;
}

void
textbox_free(textbox *t) {
  int i;
  if(t->text) {
    for(i = 0; i < t->n; i++) {
      if(t->text[i]) {
        free(t->text[i]);
        t->text[i] = NULL;
      }
    }
    free(t->text);
    t->text = NULL;
  }
  if(t->color) {
    free(t->color);
    t->color = NULL;
  }
  if(t->symbol) {
    free(t->symbol);
    t->symbol = NULL;
  }
  if(t->style) {
    free(t->style);
    t->style = NULL;
  }
  if(t->width) {
    free(t->width);
    t->width = NULL;
  }
  free(t);
  t = NULL;
}

void
text_box_line(display_t *out, 
              float      x,
              float      y,
              int        width, 
              int        style, 
              float      text_width, 
              float      text_height) {

  /*    width  (w) = Character Width
   *    height (h) = Character Height 
   *                                --------------
   *                   |<-0.5 * w->|      |
   * |<- 2.5 * width ->|                  |       ______    \   /     \
   * -------------------  -               |      /      \    \ /      |
   *                      |               |      --------     X     ---+---
   *                      | 0.5 * h       |      \           / \      |
   *                      -               |       ------/   /   \     \_/
   */                   
  int nerr;
  unsigned int w, h;
    
  out->get_geometry(-1, &w, &h, &nerr);
  
  if(out->id == SGF) {
      w = SGF_PIXELS(w);
  }

  /* Set Line Style and Width */
  out->set_line_width( width );
  out->set_line_style( &style );

  /* Shift to start of line */
  y += 0.5 * text_height;
  x -= TEXTBOX_LINE_LENGTH_PIXELS/ (float)w + 0.5 * text_width; //5.5 * text_width;
  out->move(x, y);

  /* Draw Line */
  x += TEXTBOX_LINE_LENGTH_PIXELS / (float)w;
  out->draw(x, y);

  if(out->stroke) {
    out->stroke();
  }

  /* Reset line style to SOLID */
  style = LINE_STYLE_SOLID;
  out->set_line_style( &style );
}

void
text_box_symbol(display_t *out,
                float x,
                float y,
                int symbol,
                int with_line,
                float text_width,
                float text_height) {

      int nerr;
      unsigned int w, h;
    
      if(symbol <= 0) {
        return;
      }
      
      out->get_geometry(-1, &w, &h, &nerr);

      if(out->id == SGF) {
          w = SGF_PIXELS(w);
      }

      out->set_line_width( cmgem.isymwidth );
      setsymbolnum( symbol );
      if(with_line) {
        x = x - 0.5 * text_width - (TEXTBOX_LINE_LENGTH_PIXELS/w) / 2.0;
      } else {
        x = x - text_width;
      }
      y = y + 0.5 * text_height;

      symbol_single(out, x, y);

      out->set_line_width( cmgem.iwidth );
  
}
