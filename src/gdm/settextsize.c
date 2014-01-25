
#include <stdio.h>

#include "gdm.h"

void
settextsize_internal(float width, float height) {
  cmgdm.twidth = width;
  cmgdm.thgt   = height;
}

/** 
 * Change the graphics text size
 *
 * @param width
 *    The width of a single character in viewport units. [f]
 *    This width includes the inter character gap.
 *    The smaller the value the smaller the characters.  For
 *    example, a value of 0.02 would mean that 50 characters
 *    of text would fit in a single line of text.
 *    Note that when graphics quality text is used, the text
 *    produced is proportionally spaced.  The width in this
 *    case is that of the widest characters.  In practice you
 *    will be able to get more that this number of characters
 *    on a single line.
 * @param height
 *     The height of a single line in viewport units. [f]
 *     This height includes the inter line gap.
 *
 * @date   831026:  Original version.
 *
 */
void 
settextsize(float width,
            float height)
{
        int i, n;
        display_t **dev;
        n   = gdm_get_ndevices();
        dev = gdm_get_devices();

	/* - If size is different than current size: */
	if( width != cmgdm.twidth || height != cmgdm.thgt ){

		/* -- Save new size. */
                settextsize_internal(width, height);

		/* -- Send new text size to active graphics devices. */
                for(i = 0; i < n; i++) {
                  if(dev[i]->on && dev[i]->set_text_size) {
                    dev[i]->set_text_size(width, height);
                  }
                }
        }
}

