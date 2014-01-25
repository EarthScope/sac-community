/** 
 * @file   gd5.gui.h
 * 
 * @brief  Graphics Device 5 (GUI, really X11)
 * 
 */
/*******************************************************************************
** COMMON for XWindow graphics device.
*
** DESCRIPTION:
*    current_pt_p5:       Pixel location of current point in XWindow
*                         coordinates.
*    basew3:              Base window structure attributes.
*    titlew3:             Title window structure attributes.
*    plotw3:              Plot window structure attributes.
*    iconw3:              Icon window structure attributes.
*    win_attr:            Window structure attributes.
*      win:               Handle of window.
*      width_p:           Width of window in pixels.
*      height_p:          Height of window in pixels.
*      status:            Status of window.
*    title_font3:         Font id of title label.
*    pixdef3:             Pixel color definitions.
*    borderwidth3:        Width of window borders in pixels.
*    current_mouse_x_p5:  Current x pixel location of mouse in XWindow
*                         coordinates.
*    current_mouse_y_p5:  Current y pixel location of mouse in XWindow
*                         coordinates.
*    num_wins3:           Number of windows which have been created.
*    c_win3:              Current window.  Numbered 1 to n
*    device_type:         Device type.
*    cursor_on:           Set to 1 if waiting for cursor input, otherwise 0.
*    cursortext_on4:      Set to 1 if waiting for cursor text input, otherwise 0.
*    color:               Index into color table to current color entry.
*    xcursor_p3:          x position of cursor in pixels.
*    ycursor_p3:          y position of cursor in pixels.
*    scr_width_p3:         Width of screen in pixels.
*    scr_height_p3:        Height of screen in pixels.
*    char_cursor:         Character struck at cursor location.
*    text_cursor4:        Line of text struck at cursor location.
*    device_name:         Device name.
************************************************************************
** MODIFICATION HISTORY:
*    920609:  Changed OW_OFFSET from hardcoded -8 to 0. Problem with
*             XdrawLine callback was fixed in OpenWindows version 3.
*    920318:  Protability to IBM required changing "struct{int x,y} point"
*             to "struct{int x,y;}
*    910506:  Added defines OW_OFFSET and ZERO for kludge to openwindows
*             cursor problem. XdrawLine (callback to server) retruns
*             eight pixels more than the values sent to it. Only happens
*             immediately after the XQueryPointer call. The offset is
*             applied in cursor3.c cursortext3.c and dispatcheve3.c (wct).
*    890606:  Added GC and display3 functionality.
*    870323:  Added variables for cursortext3_ routine.
*    870318:  Changed win_status3 structure.
*    870317:  Added title font.
*    870310:  Added icon.
*    870309:  Added title window.
*    870223:  Original Version.
*******************************************************************************/

#ifndef _GD5_GUI_H_
#define _GD5_GUI_H_

#include <X11/Xlib.h>
#include <X11/Intrinsic.h>

#include "config.h"

#include "gd3.aux.h"

struct win_status5 {
  Window win;
  int width_p;
  int height_p;
  int status;
  GC  gc;
};

point current_pt_p5;
/*  struct win_status3 basew3[MAX_WINS+1]; */
/*  struct win_status3 titlew3[MAX_WINS+1]; */
struct win_status5 plotw5[MAX_WINS+1];
GC cursorgc5[MAX_WINS+1];
/* unsigned int borderwidth5; */
int current_mouse_x_p5, current_mouse_y_p5;
int num_wins5;
int c_win5;
int device_type5;
int cursor_on5;
int cursortext_on5;
int linestyle5;
int xcursor_p5, ycursor_p5;
int scr_width_p5;
int scr_height_p5;
char char_cursor5[1];
char text_cursor5[132];
char device_name5[13];
Font title_font5;
Display *display5;
XColor pixdef5[256];
unsigned long color5;


void begindevice5 ( int *nerr);
void beginframe5 ( int *nerr);
void beginwindow5 ( int *win_num, 
	 int *nerr);
void calc_loc5 ( unsigned int *xloc, 
	 unsigned int *yloc, 
	 unsigned int *cbarxoffset, 
	 unsigned int *cbaryoffset, 
	 unsigned int w_width, 
	 unsigned int w_height, 
	 double xpmn, 
	 double xpmx, 
	 double xmin, 
	 double first, 
	 double last, 
	 double ypmn, 
	 double ypdel, 
	 int *nerr);
void cbar_window5 ( unsigned int xloc, 
	 unsigned int yloc, 
	 unsigned int height, 
	 unsigned int w_height, 
	 unsigned int w_width, 
	 double vspaceratio, 
	 double ypmax, 
	 int *nerr);
void changectable5 ( int nentry, 
	 int icolortable);
void createwindow5 ( int *win_num, 
	 float *xmin_vp, 
	 float *xmax_vp, 
	 float *ymin_vp, 
	 float *ymax_vp, 
	 int *nerr);
void cursor5 ( float *xloc_vp, 
	 float *yloc_vp, 
	 char cchar[], 
	 int cchar_length);
void cursortext5 ( float *xloc_vp, 
	 float *yloc_vp, 
	 char ktext[], 
	 int ktext_length);
int dispatchevent5 ( int *nerr);
void draw5 ( float xloc_vp, 
             float yloc_vp);
void drawpoly5 ( float *xloc_vp, 
                float *yloc_vp, 
                int npts);
void enddevice5 ( int *nerr);
void endframe5 ( int *nerr);
void erase5 (void);
char *fill_clrbar5 ( int npseudocolors, 
	 int width, 
	 int npricolors, 
	 int ndefcolors, 
	 int *nerr);
char *fill_image5 ( unsigned int height, 
	 unsigned int width, 
	 float data[], 
	 double dmin, 
	 double range, 
	 int npseudocolors, 
	 int nsaccolors, 
	 int ndefcolors, 
	 int *nerr);
void flushbuffer5 ( int *nerr);
void get_geometry5 ( int window, 
	 unsigned int *width_return, 
	 unsigned int *height_return, 
	 int *nerr);
void getalphainfo5 ( int *num_lines, 
	 char erase[], 
	 int erase_length);
int getdeviceinfo5 ( char dev_name[], 
	 int dev_name_length, 
	 int *dev_type);
void getdevicerat5 ( float *ratio);
void getratio5 ( float *ratio);
void getwindowstat5 ( int win_num, 
	 int *exists);
void initdevice5 ( Display *display, 
                   Widget draw_area);
void move5 ( float xloc_vp, 
	 float yloc_vp);
void put_image5 ( char *data, 
	 unsigned int xloc, 
	 unsigned int yloc, 
	 unsigned int width, 
	 unsigned int height, 
	 int *nerr);
void setcolor5 ( int index);
void setctable5 ( int win_num, 
	 unsigned int nentry, 
	 float red[], 
	 float green[], 
	 float blue[]);
void setlinestyle5 ( int *linestyle);
void setpsctable5 ( int *win_num, 
	 unsigned int nentry, 
	 float red[], 
	 float green[], 
	 float blue[]);
void settextsize5 ( float width, 
                    float height);
void setwidth5 ( int index);


#endif /* _GD5_GUI_H_ */
