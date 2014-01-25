/** 
 * @file initdevice2.c
 *
 */
#include <string.h>

#include "co.h"
#include "gd2.h"
#include "bool.h"
#include "gdm.h"
#include "gam.h"

display_t sgf;

static char *sgf_name = "SGF";
static char *sgf_ext  = "sgf";


void
save_sgf(display_t *out, char *file) {
  record_write_file(out, file);
}

void
adj_geometry2(unsigned int *w, unsigned int *h) {
  *w = *w / 32;
  *h = *h / 32;
}

/** 
 * Initialize the "second" graphics device Sac Graphic Format (SGF)
 *
 * @date   900310:  Changed plot size variables.
 * @date   870929:  Deleted initialization of xw and yw.
 * @date   861010:  Original version.
 *
 */
void 
initdevice2() {

	/* - Initialize common block. */
	/* -- Device name. */
	strcpy( kmgd2.kname2, "SGF     " );
	cmgd2.itype2 = 1;

	/* -- Frame directory, id, and number. */
	fstrncpy( kmgd2.kfdir, MCPFN, " ", 1);
	fstrncpy( kmgd2.kfdirStore, MCPFN, " ", 1);
	cmgd2.nfdir = 0;
	cmgd2.nfdirStore = 0 ;
	fstrncpy( kmgd2.kfnamb, MCPFN, "f", 1);
	cmgd2.nfnamb = 1;
	cmgd2.nfnum = 1;
	cmgd2.lfnum = FALSE;

	/* - Fixed plot size attributes. */

	strcpy( kmgd2.sizetype, "NORMAL  " );
	cmgd2.sizevalue = 10.0;
	cmgd2.encodesize = FALSE;

	cmgd2.lover = FALSE ;  /* TRUE overwrite SGF files */

	/* set print filename to "" */
	kmgd2.kfilename[ 0 ] = '\0' ;

        initdevice_null( &sgf );

        sgf.name               = sgf_name;
        sgf.extension          = sgf_ext;
        sgf.id                 = SGF;

        sgf.active_device      = TRUE;
        sgf.adjust_geometry    = adj_geometry2;
        sgf.begin_device       = begindevice2;
        sgf.begin_frame        = beginframe2;
        sgf.begin_window       = beginwindow2;
        sgf.change_color_table = changectable2;
        sgf.create_window      = createwindow2;
        sgf.cursor_text        = cursortext2;
        sgf.cursor             = cursor2;
        sgf.calc_loc           = calculate_location2;
        sgf.draw               = draw2;
        sgf.drawpoly           = drawpoly2;
        sgf.fillpoly           = fillpoly2;
        sgf.end_device         = enddevice2;
        sgf.end_frame          = endframe2;
        sgf.erase              = erase2;
        sgf.flush_buffer       = flushbuffer2;
        sgf.fill_image         = fill_image2;
        sgf.fill_colorbar      = fill_clrbar2;
        //sgf.get_device_ratio   = getdevicerat2;
        sgf.get_ratio          = getratio2;
        sgf.get_geometry       = get_geometry2;
        sgf.get_alpha_info     = getalphainfo2;
        sgf.get_window_status  = getwindowstat2;
        sgf.move               = move2;
        sgf.put_image          = put_image2;
        sgf.set_color          = setcolor2;
        sgf.set_color_table    = setctable2;
        sgf.set_line_style     = setlinestyle2;
        sgf.set_line_width     = setwidth2;
        sgf.set_text_angle     = settextangle2;
        sgf.set_text_size      = settextsize2;
        sgf.text               = softwaretext;
        sgf.textbox            = text_box_sgf;
        sgf.show_image         = show_image_sgf;
        /* sgf.save               = save_sgf; */
        sgf.save               = NULL;
        gdm_register_device( &sgf );
        
	return;

} /* end of function */

