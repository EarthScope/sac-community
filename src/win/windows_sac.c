
#include <stdio.h>
#include <string.h>

#include "gdm.h"

#include "color.h"

#include "debug.h"

#include "WinSacView.h"

void      SacWindowShow (SacView *view);
SacView * SacWindow     (int id);
void      SacWindowAdd  (int id);
void      SacViewUpdate (SacView *view);
void      SacViewAdd    (SacView *view, SacViewType type, ...);
void      initsac       ();
int       color_on      ();
int       color_skeleton();
int       color_foreground_default (void);


extern SacViewWindows *wins;

static SacView *current_view = NULL;

static display_t win;

SacColor pixdef[100000];

static SacColor COLORS[] = { 
  {1.0, 1.0, 1.0},
  {1.0, 0.0, 0.0},
  {0.0, 1.0, 0.0},
  {0.0, 0.0, 1.0},
  {1.0, 1.0, 0.0},
  {0.0, 1.0, 1.0},
  {1.0, 0.0, 1.0},
  {0.0, 0.0, 0.0},
};

void
win_set_current(SacView *view) {
    current_view = view;
}


void
win_begindevice() {
    int nerr;
    DEBUG("\n");
    begindevices("WIN     ", 9, 1, &nerr);
    DEBUG("\n");
}

void
win_init() {
    DEBUG("\n");
    initsac();
    DEBUG("\n");
}

void win_begin_device(int *nerr) { *nerr = 0; }
void 
win_begin_window(int *number, int *nerr) { 
    SacView *view;
    DEBUG("\n");
    *nerr = 0; 
    view = SacViewWindowsGetByID(wins, *number);
    if(view) {
        win_set_current(view);
        SacWindowShow(view);
    } else {
        fprintf(stderr, "Error finding window with ID: %d\n", *number);
    }
}

void 
win_end_device(int *nerr) { 
    DEBUG("\n");
    *nerr = 0; 
}

void 
win_end_frame(int *nerr) { 
    DEBUG("\n");
    *nerr = 0; 
    SacViewUpdate(current_view);
}
void
win_flush_buffer(int *nerr) {
    DEBUG("\n");
    *nerr = 0;
    SacViewUpdate(current_view);
}

void
win_device_ratio(float *ratio) {
    DEBUG("\n");
    *ratio = 1.0;
}

void
win_ratio(float *ratio) {
    DEBUG("\n");
    *ratio = current_view->bounds.width / current_view->bounds.height;
}

void
win_window_size(float *xmin, float *xmax, float *ymin, float *ymax) {
    DEBUG("\n");
    *xmin = 0.0;
    *xmax = 1.0;
    *ymin = 0.0;
    *ymax = 1.0;
}

//    return (SacViewWindowsFindByID(wins, num) > 0);


void
win_create_window(int   *win_num,
                  float *xmin,
                  float *xmax,
                  float *ymin,
                  float *ymax,
                  int   *nerr) {
    DEBUG("\n");
    *nerr = 0;
    if(!SacViewWindowsGetByID(wins, *win_num)) {
        DEBUG("No window with ID\n");
        SacWindowAdd( *win_num );
    } 
}

void win_erase() { }

void
win_begin_frame(int *nerr) {
    DEBUG("\n");
    *nerr = 0;
    SacViewAdd(current_view, SV_Begin);
}

void
win_set_color_table(int          win_num,
                    unsigned int nentry,
                    float        red[],
                    float        green[],
                    float        blue[]) {
    int i;
    DEBUG("\n");
    for(i = 0; i < (int) nentry; i++) {
        pixdef[i].red   = red[i]   * WIN_COLOR_MAXIMUM;
        pixdef[i].green = green[i] * WIN_COLOR_MAXIMUM;
        pixdef[i].blue  = blue[i]  * WIN_COLOR_MAXIMUM;
    }
}

static void
set_color(SacColor *c, float r, float g, float b, int scale) {
    c->red   = r * scale;
    c->green = g * scale;
    c->blue  = b * scale;
}

void
win_set_pseudo_color_table(int          *win_num,
                           unsigned int  un,
                           float        *red,
                           float        *green,
                           float        *blue) {
    int k;
    int i;
    int n = (int) un;
    int nalloc = n + cmgdm.npscimage + 7;
    DEBUG("\n");

    k = n - 1 + 7;
    set_color(&pixdef[n-1+7], red[n-1], green[n-1], blue[n-1], WIN_COLOR_MAXIMUM);
    set_color(&pixdef[7],     red[0],   green[0],   blue[0], WIN_COLOR_MAXIMUM);
    for(i = 8; i < n-1+7; i++) {
        set_color(&pixdef[i], red[i-7], green[i-7], blue[i-7], WIN_COLOR_MAXIMUM);
    }
    for(i = n+7; i < nalloc; i++) {
        set_color(&pixdef[i], psred[i-(n+7)], psgreen[i-(n+7)], psblue[i-(n+7)], 1);
    }
}

void
win_stroke() {
    SacViewAdd(current_view, SV_Stroke);
}

void
win_draw(float x, float y) {
    SacViewAdd(current_view, SV_Line, MakePoint(x,y));
}
void
win_draw_poly(float *x, float *y, int n) {
    int i;
    SacPoint *p;
    p = (SacPoint *) malloc(sizeof(SacPoint) * n);
    for(i = 0; i < n; i++) {
        p[i] = MakePoint(x[i], y[i]);
    }
    SacViewAdd(current_view, SV_Poly, n, p);
}

void
win_move(float x, float y) {
    SacViewAdd(current_view, SV_Move, MakePoint(x,y));
}

void
win_text(display_t *out, char *text, int n) {
    softwaretext(out, text, n);
}

float
view_to_win_x(float x) {
    return SacViewMap(current_view, SacViewToScreen, SacPointX, x);
}
float
view_to_win_y(float y) {
    return SacViewMap(current_view, SacViewToScreen, SacPointY, y);
}
float
win_to_view_x(float x) {
    return SacViewMap(current_view, SacViewFromScreen, SacPointX, x);
}
float
win_to_view_y(float y) {
    return SacViewMap(current_view, SacViewFromScreen, SacPointY, y);
}

float
win_text_width(char *text) {
    float size;
    getstringsize(text, strlen(text), &size);
    return SacViewMap(current_view, SacViewToScreen, SacPointX, size);
}

void
win_set_color(int index) {
    SacColor c = MakeColor(COLORS[index].red   * WIN_COLOR_MAXIMUM,
                           COLORS[index].green * WIN_COLOR_MAXIMUM,
                           COLORS[index].blue  * WIN_COLOR_MAXIMUM);
    SacViewAdd(current_view, SV_Color, c);
}

void
win_size(int *w, int *h) {
    DEBUG("\n");
    *w = current_view->bounds.width;
    *h = current_view->bounds.height;
}

void
win_text_box(textbox *t) {
    int i;
    int cwidth, cheight, ascent;
    int len, xlen;
    float x,y,sx,sy;
    int width, height;

    if(!t || t->n == 0) {
        return;
    }
    len = 0;

    win_size(&width, &height);

    cwidth  = view_to_win_x( cmgdm.twidth );
    cheight = view_to_win_x( cmgdm.thgt );
    ascent  = view_to_win_x( cmgdm.thgt * 1.10 );

    x = view_to_win_x(t->x);
    y = view_to_win_y(t->y);

    if(t->location & TEXT_BOX_RIGHT) {
        for(i = 0; i < t->n; i++) {
            xlen = win_text_width(t->text[i]);
            if(xlen > len) {
                len = xlen;
            }
        }
        x = x - len;
    }
    if(t->location & TEXT_BOX_LOWER) {
        y -= cheight * (t->n - 1);
    } else {
        y += cheight;
    }

    for(i = 0; i < t->n; i++) {
        win_set_color(t->color[i]);
        sx = win_to_view_x(x);
        sy = win_to_view_y(y);

        set_position(sx, sy);
        softwaretext(&win, t->text[i], strlen(t->text[i]));
        
        if(t->use_style) {
            text_box_line(&win,  sx, sy, t->width[i], t->style[i],
                          cwidth / (float) width,
                          ascent / (float) width );
        }
        if(t->use_symbol) {
            text_box_symbol(&win, sx, sy, t->symbol[i], t->use_style,
                            cwidth / (float) width,
                            ascent / (float) width );                                
        }
        y += cheight;
    }
    
    win_set_color( color_on() ? color_skeleton() : color_foreground_default() );    
}

void
win_set_width(int index) {
    SacViewAdd(current_view, SV_Width, index);
}

void
win_show_image(float *data,
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
    SacImage *im = ImageInit(data,
                             iw, ih, 
                             xmin, xmax, ymin, ymax,
                             x, y, w, h, 
                             npseudocolors, nsacolors, ndefcolors,
                             lbinary);
    SacViewAdd(current_view, SV_Image, im);
}

void
win_alpha_info(int *num_lines, char erase[], int erase_length) {
    *num_lines = 40000;
    erase[0] = ' ';
}

void
win_cursor(float *x, float *y, char c[], int length) {
    //win_wait_for_keypress(x,y,c);
    *x = win_to_view_x(*x);
    *y = win_to_view_y(*y);
}


void 
initdevice_win() {

  initdevice_null( &win );

  win.name                   = strdup("WINDOWS");
  win.extension              = strdup("win");
  win.id                     = 13;

  win.active_device          = TRUE;
  win.cursor_enabled         = TRUE;
  win.begin_device           = win_begin_device;
  win.begin_frame            = win_begin_frame;
  win.begin_window           = win_begin_window;
  //win.cursor_text            = cursortext3; *
  win.cursor                 = win_cursor; 
  win.create_window          = win_create_window;
  //win.change_color_table     = changectable3;
  //win.calc_loc               = calculate_location3; *
  win.draw                   = win_draw;
  win.stroke                 = win_stroke;
  win.drawpoly               = win_draw_poly;
  win.erase                  = win_erase;
  win.end_device             = win_end_device;
  win.end_frame              = win_end_frame;
  //win.fill_image             = win_fill_image;
  //win.fill_colorbar          = fill_clrbar3; *
  win.flush_buffer           = win_flush_buffer;
  //win.get_window_status      = getwindowstat3; 
  win.get_ratio              = win_ratio;
  win.get_device_ratio       = win_device_ratio;
  win.get_alpha_info         = win_alpha_info; 
  //win.get_geometry           = get_geometry3;
  win.move                   = win_move; 
  //win.put_image              = win_put_image;
  win.set_color              = win_set_color;
  win.set_color_table        = win_set_color_table;
  //win.set_line_style         = setlinestyle3; *
  win.set_line_width         = win_set_width;
  win.set_pseudo_color_table = win_set_pseudo_color_table;
  //win.set_text_size          = settextsize3; *
  win.text                   = win_text;
  win.textbox                = win_text_box; 
  win.show_image             = win_show_image;
  //win.set_window_width       = set_window_width_win; * 
  //win.set_window_height      = set_window_height_win; *
  win.get_window_size        = win_window_size;
  //win.get_file_descriptor    = get_file_descriptor_win;
  //win.handle_event           = dispatchevent3;

  gdm_register_device( &win );
    
}
