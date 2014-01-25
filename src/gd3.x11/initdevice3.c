/*******************************************************************************
** PURPOSE:
*    To intialize the common blockfor the XWindow device.
*
** GLOBAL OUTPUT:
*    gd3.x11.h:  device_name3, basew3->status, titlew3->status, plotw->status,
*                iconw3->status, borderwidth3, device_type3, cursor_on3,
*                cursortext_on3, char_cursor3, text_cursor3, num_wins3
*
** LOCAL VARIABLES:
*    i:  Loop variable.
*******************************************************************************/

#include <string.h>

#include "bool.h"
#include "gdm.h"
#include "gd3.x11.h"

display_t x11;

static char *x11_name = "XWINDOWS";
static char *x11_ext  = "x11";

void
initdevice3()
{

  int i;

/* Set the device name */

  device_name3[0] = 'X';
  device_name3[1] = 'W';
  device_name3[2] = 'I';
  device_name3[3] = 'N';
  device_name3[4] = 'D';
  device_name3[5] = 'O';
  device_name3[6] = 'W';
  device_name3[7] = 'S';
  device_name3[8] = ' ';
  device_name3[9] = ' ';
  device_name3[10] = ' ';
  device_name3[11] = ' ';
  device_name3[12] = '\0';

/* Set the initial state of the windows */

  for (i=0; i<=MAX_WINS; i++) {
    basew3[i].status  = UNAVAILABLE;
    titlew3[i].status = UNAVAILABLE;
    plotw3[i].status  = UNAVAILABLE;
    plotw3[i].win     = 0;
    titlew3[i].win    = 0;
    basew3[i].win     = 0;
  }

/* Initialize some variables */
  device_type3 = 3;
  cursor_on3 = 0;
  cursortext_on3 = 0;
  char_cursor3[0] = '0';
  strcpy(text_cursor3, " ");
  num_wins3 = 0;

  initdevice_null( &x11 );

  x11.name               = x11_name;
  x11.extension          = x11_ext;
  x11.id                 = X11;

  x11.active_device      = TRUE;
  x11.cursor_enabled     = TRUE;
  x11.begin_device       = begindevice3;
  x11.begin_frame        = beginframe3;
  x11.begin_window       = beginwindow3;
  x11.cursor_text        = cursortext3;
  x11.cursor             = cursor3;
  x11.create_window      = createwindow3;
  x11.change_color_table = changectable3;
  x11.calc_loc           = calculate_location3;
  x11.draw               = draw3;
  x11.drawpoly           = drawpoly3;
  x11.fillpoly           = fillpoly3;
  x11.erase              = erase3;
  x11.end_device         = enddevice3;
  x11.end_frame          = endframe3;
  x11.fill_image         = fill_image3;
  x11.fill_colorbar      = fill_clrbar3;
  x11.flush_buffer       = flushbuffer3;
  x11.get_window_status  = getwindowstat3;
  x11.get_ratio          = getratio3;
  x11.get_device_ratio   = getdevicerat3;
  x11.get_alpha_info     = getalphainfo3;
  x11.get_geometry       = get_geometry3;
  x11.move               = move3; 
  x11.put_image          = put_image3;
  x11.set_color          = setcolor3;
  x11.set_color_table    = setctable3;
  x11.set_line_style     = setlinestyle3;
  x11.set_line_width     = setwidth3;
  x11.set_pseudo_color_table = setpsctable3;
  x11.set_text_size      = settextsize3;
  x11.text               = text_x11;
  x11.textbox            = text_box_x11;
  x11.show_image         = show_image_x11;
  x11.set_window_width   = set_window_width_x11;
  x11.set_window_height  = set_window_height_x11;
  x11.get_window_size    = get_window_size_x11;
  x11.get_file_descriptor = get_file_descriptor_x11;
  x11.handle_event        = dispatchevent3;

  gdm_register_device( &x11 );

}

/*******************************************************************************
** MODIFICATION HISTORY:
*    890608:  Modified to run under X11 rather than X10.  (kjm)
*    870323:  Added initializations for cursortext3_ routine.
*    870318:  Changes due to gd3.x10.h structure change.
*    870227:  Original Version
*******************************************************************************/
