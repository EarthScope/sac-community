
#include <stdio.h>
#include <string.h>

#include "co.h"
#include "lhf.h"
#include "amf.h"
#include "gdm.h"
#include "gem.h"
#include "msg.h"
#include "bot.h"
#include "exm.h"
#include "top.h"
#include "dfm.h"
#include "ncpf.h"

#include "color.h"

#define __DEBUG__
#include "string_utils.h"


#include "osx_color.h"
OSXColor pixdef6[100000];

#define SAC_COLOR_MAXIMUM 65535

void main_command(char *ksmg, int n);
void execute_command_line(char *kmsg, int len);
float osx_sac_osx_to_view_x(float x);
float osx_sac_osx_to_view_y(float y);
float osx_sac_view_to_osx_x(float x);
float osx_sac_view_to_osx_y(float y);
void osx_sac_create_window(int n);
int osx_sac_find_window(int n);
void osx_sac_focus_window(int n);
void osx_sac_wait_for_keypress(float *x, float *y, char c[]);

typedef struct _osx_color_t osx_color_t;
struct _osx_color_t {
    float r,g,b;
};
static display_t osx;

static osx_color_t COLORS[] = { 
  {1.0, 1.0, 1.0},
  {1.0, 0.0, 0.0},
  {0.0, 1.0, 0.0},
  {0.0, 0.0, 1.0},
  {1.0, 1.0, 0.0},
  {0.0, 1.0, 1.0},
  {1.0, 0.0, 1.0},
  {0.0, 0.0, 0.0},
};

void png_write(display_t *out, char *file) { }
void initdevice3() { }
void xpm_write(display_t *xpm, char *file) { }
void xwindow_set_font_system(int type) { }
void set_plot_ratio_x11(float ratio) { }
void set_constrain_plot_ratio_x11(int set) { }
void xwindow_set_font_base(int type) { }

void osx_sac_flush();
void osx_sac_size(int *width, int *height);
void osx_sac_poly(int n, float *x, float *y);
void osx_sac_width(int width);
void osx_sac_mds(int type, float x, float y);
void osx_sac_color(float r, float g, float b);
void osx_sac_show_image(float *data,     
                        unsigned int iw, 
                        unsigned int ih,
                        float xmin,
                        float xmax,
                        float ymin,
                        float ymax,
                        float x,    
                        float y,
                        float w,
                        float h,
                        int npseudocolors,
                        int nsacolors,
                        int ndefcolors,
                        int lbinary);

void
osx_begindevice() {
    int nerr;
    begindevices("MAC      ", 9, 1, &nerr);
}

void
osx_execute_macro(char *file) {
  fprintf(stderr, "file: %s\n",file);
  execute_command_line(file, strlen(file));
}

void
osx_main_gui_command(char *kmsg, int n, int plot, int echo) {
    if(echo) {
        fprintf(stdout, "%s\n", kmsg);
    }
    DEBUG("%s\n", kmsg);
    main_command(kmsg, n);
    DEBUG("%d\n", plot);
    if(plot == 1) {
        fprintf(stdout, "SAC> plot1\n");
        main_command("plot1 ", 6);
    } else if (plot == 2) {
        fprintf(stdout, "SAC> plot2\n");
        main_command("plot2 ", 6);        
    }
    DEBUG("Prompt\n");
    fprintf(stdout, "SAC> %c", 0x04);
    DEBUG("Flush\n");
    fflush(stdout);
    DEBUG("Return\n");
}

void
osx_init() {
    DEBUG("\n");
    initsac();
    osx_begindevice();
}

void
osx_main() {
    char kmsg[MCMSG];
    osx_init();
    while( 1 ) {
        zgpmsg( kmexm.kprmt, 13, kmsg, MCMSG+1);
        main_command(kmsg, MCMSG);
    }
}

void
osx_begin_device(int *nerr) {
    *nerr = 0;
    //DEBUG("\n");
}

void
osx_begin_window(int *nerr) {
    *nerr = 0;
    //DEBUG("\n");
}

void
osx_end_device(int *nerr) {
    *nerr = 0;
    //DEBUG("\n");
}

void
osx_end_frame(int *nerr) {
    *nerr = 0;
    //DEBUG("\n");
    osx_sac_flush();
}

void
osx_flush_buffer(int *nerr) {
    //DEBUG("\n");
    *nerr = 0;
    osx_sac_flush();
} 

void
osx_device_ratio(float *ratio) {
    *ratio = 1.0;
}
void
osx_ratio(float *ratio) {
    int w, h;
    osx_sac_size(&w, &h);
    *ratio = (float)h/(float)w;
}

void
osx_window_size(float *xmin, float *xmax, float *ymin, float *ymax) {
    *xmin = 0.0;
    *xmax = 1.0;
    *ymin = 0.0;
    *ymax = 1.0;
}

void
osx_create_window(int *win_num,
                  float *xmin_vp,
                  float *xmax_vp,
                  float *ymin_vp,
                  float *ymax_yp,
                  int *nerr) {
    *nerr = 0;
    if(! osx_sac_find_window(*win_num) ) {
        osx_sac_create_window(*win_num);
    } else {
        osx_sac_focus_window(*win_num);
    }
}

void
osx_erase() {
    //DEBUG("\n");
}

void
osx_begin_frame(int *nerr) {
    *nerr = 0;
    //DEBUG("\n");
    osx_sac_mds(0, 0.0, 0.0);
}

void
osx_set_color_table(int          win_num, 
                    unsigned int nentry, 
                    float        red[], 
                    float        green[], 
                    float        blue[]) { 
    int i;
    for(i = 0; i < (int)nentry; i++) {
        pixdef6[i].red   = red[i]   * SAC_COLOR_MAXIMUM;
        pixdef6[i].green = green[i] * SAC_COLOR_MAXIMUM;
        pixdef6[i].blue  = blue[i]  * SAC_COLOR_MAXIMUM;
    }
    
    return;
}

void
set_color(OSXColor *c, float r, float g, float b, int scale) {
    c->red   = r * scale;
    c->green = g * scale;
    c->blue  = b * scale;
}

void
osx_set_pseudo_color_table(int *win_num,
                           unsigned int un,
                           float *red,
                           float *green,
                           float *blue) {
    int k;
    int i;
    int n = (int) un;
    int nalloc = n + cmgdm.npscimage + 7;
    k = n - 1 + 7;
    set_color(&pixdef6[n-1+7], red[n-1], green[n-1], blue[n-1], SAC_COLOR_MAXIMUM);
    set_color(&pixdef6[7],     red[0],   green[0],   blue[0], SAC_COLOR_MAXIMUM);
    for(i = 8; i < n-1+7; i++) {
        set_color(&pixdef6[i], red[i-7], green[i-7], blue[i-7], SAC_COLOR_MAXIMUM);
    }
    for(i = n+7; i < nalloc; i++) {
        set_color(&pixdef6[i], psred[i-(n+7)], psgreen[i-(n+7)], psblue[i-(n+7)], 1);
    }
}

typedef struct {int x, y;} point;
point osx_pt;



void
osx_stroke() {
    osx_sac_mds(3, 0.0, 0.0);
}

void
osx_draw(float x, float y) {
    osx_sac_mds(2, x, y);
}

void
osx_draw_poly(float *x, float *y, int n) {
    osx_sac_poly(n,x,y);
}

void
osx_move(float x, float y) {
    osx_sac_mds(1,x,y);
}

void
osx_text(display_t *out, char *text, int n) {
    softwaretext(out, text, n);
}

float
view_to_osx_x(float x) {
    return osx_sac_view_to_osx_x(x);
}
float
view_to_osx_y(float y) {
    return osx_sac_view_to_osx_y(y);
}

float
osx_to_view_x(float x) {
    return osx_sac_osx_to_view_x(x);
}
float
osx_to_view_y(float y) {
    return osx_sac_osx_to_view_y(y);
}

float
osx_text_width(char *text) {
    float size;
    getstringsize(text, strlen(text), &size);
    return view_to_osx_x( size );
}

void
osx_set_color(int index) {
    osx_sac_color(COLORS[index].r, COLORS[index].g, COLORS[index].b);
}

void
osx_text_box(textbox *t) {
    int i;
    int cwidth, cheight, ascent;
    int len, xlen;
    float x,y,sx,sy;
    int width, height;

    if(!t || t->n == 0) {
        return;
    }
    len = 0;

    osx_sac_size(&width, &height);

    cwidth  = view_to_osx_x( cmgdm.twidth );
    cheight = view_to_osx_x( cmgdm.thgt );
    ascent  = view_to_osx_x( cmgdm.thgt * 1.10 );

    x = view_to_osx_x(t->x);
    y = view_to_osx_y(t->y);

    if(t->location & TEXT_BOX_RIGHT) {
        for(i = 0; i < t->n; i++) {
            xlen = osx_text_width(t->text[i]);
            if(xlen > len) {
                len = xlen;
            }
        }
        x = x - len;
    }
    if(t->location & TEXT_BOX_LOWER) {
        y += cheight * (t->n - 1);
    } else {
        y -= cheight;
    }

    for(i = 0; i < t->n; i++) {
        osx_set_color(t->color[i]);
        sx = osx_to_view_x(x);
        sy = osx_to_view_y(y);

        set_position(sx, sy);
        softwaretext(&osx, t->text[i], strlen(t->text[i]));
        
        if(t->use_style) {
            text_box_line(&osx,  sx, sy, t->width[i], t->style[i],
                          cwidth / (float) width,
                          ascent / (float) width );
        }
        if(t->use_symbol) {
            text_box_symbol(&osx, sx, sy, t->symbol[i], t->use_style,
                            cwidth / (float) width,
                            ascent / (float) width );                                
        }
        y -= cheight;
    }
    
    osx_set_color( color_on() ? color_skeleton() : color_foreground_default() );
}

void
osx_set_width(int index) {
    osx_sac_width( index );
}

/*
void
osx_put_image(char *data,
              unsigned int xloc,
              unsigned int yloc,
              unsigned int width,
              unsigned int height,
              int *nerr) {
    DEBUG("\n");
    osx_sac_put_image(data, xloc, yloc, width, height);
}
*/
/*
char *
osx_fill_image(unsigned int height,
               unsigned int width,
               float data[],
               float dmin,
               float range,
               int npseudocolors,
               int nsacolors,
               int ndefcolors,
               int *nerr) {
    DEBUG("\n");
    osx_sac_fill_image(height, width, data, dmin, range, 
                       npseudocolors, nsacolors, ndefcolors);
    return NULL;
}
*/

void
osx_show_image(float *data,
               unsigned int iw,
               unsigned int ih,
               float xmin,
               float xmax,
               float ymin,
               float ymax,
               float x,
               float y,
               float w,
               float h,
               int npseudocolors,
               int nsacolors,
               int ndefcolors,
               int lbinary,
               int *nerr) {
    *nerr = 0;
    osx_sac_show_image(data, 
                       iw, ih, 
                       xmin, xmax, ymin, ymax,
                       x, y, w, h, 
                       npseudocolors, nsacolors, ndefcolors,
                       lbinary);
}

void
osx_alpha_info(int *num_lines, char erase[], int erase_length ) {
    *num_lines = 40000;
    erase[0] = ' ';
}

void
osx_cursor(float *x, float *y, char c[], int length) {
    osx_sac_wait_for_keypress(x,y,c);
    *x = osx_to_view_x(*x);
    *y = osx_to_view_y(*y);
}

void initdevice_osx() {

  initdevice_null( &osx );

  osx.name                   = strdup("MacOSX");
  osx.extension              = strdup("osx");
  osx.id                     = 12;

  osx.active_device          = TRUE;
  osx.cursor_enabled         = TRUE;
  osx.begin_device           = osx_begin_device;
  osx.begin_frame            = osx_begin_frame;
  osx.begin_window           = osx_begin_window;
  //osx.cursor_text            = cursortext3; *
  osx.cursor                 = osx_cursor; 
  osx.create_window          = osx_create_window;
  //osx.change_color_table     = changectable3;
  //osx.calc_loc               = calculate_location3; *
  osx.draw                   = osx_draw;
  osx.stroke                 = osx_stroke;
  osx.drawpoly               = osx_draw_poly;
  osx.erase                  = osx_erase;
  osx.end_device             = osx_end_device;
  osx.end_frame              = osx_end_frame;
  //osx.fill_image             = osx_fill_image;
  //osx.fill_colorbar          = fill_clrbar3; *
  osx.flush_buffer           = osx_flush_buffer;
  //osx.get_window_status      = getwindowstat3; 
  osx.get_ratio              = osx_ratio;
  osx.get_device_ratio       = osx_device_ratio;
  osx.get_alpha_info         = osx_alpha_info; 
  //osx.get_geometry           = get_geometry3;
  osx.move                   = osx_move; 
  //osx.put_image              = osx_put_image;
  osx.set_color              = osx_set_color;
  osx.set_color_table        = osx_set_color_table;
  //osx.set_line_style         = setlinestyle3; *
  osx.set_line_width         = osx_set_width;
  osx.set_pseudo_color_table = osx_set_pseudo_color_table;
  //osx.set_text_size          = settextsize3; *
  osx.text                   = osx_text;
  osx.textbox                = osx_text_box; 
  osx.show_image             = osx_show_image;
  //osx.set_window_width       = set_window_width_osx; * 
  //osx.set_window_height      = set_window_height_osx; *
  osx.get_window_size        = osx_window_size;
  //osx.get_file_descriptor    = get_file_descriptor_osx;
  //osx.handle_event           = dispatchevent3;

  gdm_register_device( &osx );
    
}

int 
sac_files_length() {
    return cmdfm.ndfl;
}

float 
sac_files_float(int fid, int hid) {
    float *f;
    f = cmmem.sacmem[Ndxhdr[fid]];
    return f[hid];
}

char *
sac_files_header_name(int hid) {
    return strcut(kmlhf.kfhdr[hid], 1, 8);
}
