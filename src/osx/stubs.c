
#include <stdio.h>
#include <string.h>
#include <unistd.h>

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

GDM_EXTERN

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
    float r, g, b;
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

void osx_sac_flush();
void osx_sac_size(int *width, int *height);
void osx_sac_poly(int n, float *x, float *y);
void osx_sac_width(int width);
void osx_sac_mds(int type, float x, float y);
void osx_sac_color(float r, float g, float b);
void osx_sac_show_image(float *data, unsigned int iw, unsigned int ih,
                        float xmin, float xmax, float ymin, float ymax, float x,
                        float y, float w, float h, int npseudocolors,
                        int nsacolors, int ndefcolors, int lbinary);

void
osx_begindevice() {
    int nerr;
    begindevices("MAC      ", 9, 1, &nerr);
}

void
osx_init() {
    initsac();
}

void
osx_begin_device(int *nerr) {
    *nerr = 0;
}

void
osx_begin_window(int *number, int *nerr) {
    UNUSED(number);
    *nerr = 0;
}

void
osx_end_device(int *nerr) {
    *nerr = 0;
}

void
osx_end_frame(int *nerr) {
    *nerr = 0;
    osx_sac_flush();
}

void
osx_flush_buffer(int *nerr) {
    *nerr = 0;
    osx_sac_flush();
}

void
osx_device_ratio(float *ratio) {
    *ratio = 1.0;
}

void
osx_begin_frame(int *nerr) {
    *nerr = 0;
    osx_sac_mds(0, 0.0, 0.0);
}

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
    osx_sac_poly(n, x, y);
}

void
osx_move(float x, float y) {
    osx_sac_mds(1, x, y);
}

void
osx_set_width(int index) {
    osx_sac_width(index);
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
view_to_osx_x(float x) {
    return osx_sac_view_to_osx_x(x);
}

float
view_to_osx_y(float y) {
    return osx_sac_view_to_osx_y(y);
}

void
osx_text(display_t * out, char *text, int n) {
    softwaretext(out, text, n);
}

void
osx_ratio(float *ratio) {
    int w, h;
    osx_sac_size(&w, &h);
    *ratio = (float) h / (float) w;
}

void
osx_window_size(float *xmin, float *xmax, float *ymin, float *ymax) {
    *xmin = 0.0;
    *xmax = 1.0;
    *ymin = 0.0;
    *ymax = 1.0;
}

void
osx_create_window(int *win_num, float *xmin_vp, float *xmax_vp, float *ymin_vp,
                  float *ymax_vp, int *nerr) {
    UNUSED(xmin_vp);
    UNUSED(xmax_vp);
    UNUSED(ymin_vp);
    UNUSED(ymax_vp);
    *nerr = 0;
    if (!osx_sac_find_window(*win_num)) {
        osx_sac_create_window(*win_num);
    } else {
        osx_sac_focus_window(*win_num);
    }
}

void
osx_set_color_table(int win_num, unsigned int nentry, float red[],
                    float green[], float blue[]) {
    UNUSED(win_num);
    int i;
    for (i = 0; i < (int) nentry; i++) {
        pixdef6[i].red = red[i] * SAC_COLOR_MAXIMUM;
        pixdef6[i].green = green[i] * SAC_COLOR_MAXIMUM;
        pixdef6[i].blue = blue[i] * SAC_COLOR_MAXIMUM;
    }
    return;
}

void
set_color(OSXColor * c, float r, float g, float b, int scale) {
    c->red = r * scale;
    c->green = g * scale;
    c->blue = b * scale;
}

void
osx_set_pseudo_color_table(int *win_num, unsigned int un, float *red,
                           float *green, float *blue) {
    int k;
    int i;
    int n = (int) un;
    int nalloc = n + cmgdm.npscimage + 7;
    UNUSED(win_num);
    k = n - 1 + 7;
    set_color(&pixdef6[n - 1 + 7], red[n - 1], green[n - 1], blue[n - 1],
              SAC_COLOR_MAXIMUM);
    set_color(&pixdef6[7], red[0], green[0], blue[0], SAC_COLOR_MAXIMUM);
    for (i = 8; i < n - 1 + 7; i++) {
        set_color(&pixdef6[i], red[i - 7], green[i - 7], blue[i - 7],
                  SAC_COLOR_MAXIMUM);
    }
    for (i = n + 7; i < nalloc; i++) {
        set_color(&pixdef6[i], psred[i - (n + 7)], psgreen[i - (n + 7)],
                  psblue[i - (n + 7)], 1);
    }
}

float
osx_text_width(char *text) {
    float size;
    getstringsize(text, strlen(text), &size);
    return view_to_osx_x(size);
}

void
osx_set_color(color c) {
    osx_sac_color(c.r/255., c.g/255., c.b/255.);
}

void
osx_get_bg_color(float *r, float *g, float *b) {
    color bg;
    if(color_on()) {
        color_background(&bg);
    } else {
        color_background_default(&bg);
    }
    *r = bg.r;
    *g = bg.g;
    *b = bg.b;
}

void
osx_text_box(textbox * t) {
    int i;
    int cwidth, cheight, ascent;
    int len, xlen;
    float x, y, sx, sy;
    int width, height;
    color c;

    if (!t || t->n == 0) {
        return;
    }
    len = 0;

    osx_sac_size(&width, &height);

    cwidth = view_to_osx_x(cmgdm.twidth);
    cheight = view_to_osx_x(cmgdm.thgt);
    ascent = view_to_osx_x(cmgdm.thgt * 1.10);

    x = view_to_osx_x(t->x);
    y = view_to_osx_y(t->y);

    if (t->location & TEXT_BOX_RIGHT) {
        for (i = 0; i < t->n; i++) {
            xlen = osx_text_width(t->text[i]);
            if (xlen > len) {
                len = xlen;
            }
        }
        x = x - len;
    }
    if (t->location & TEXT_BOX_LOWER) {
        y += cheight * (t->n - 1);
    } else {
        y -= cheight;
    }

    for (i = 0; i < t->n; i++) {
        osx_set_color(t->color[i]);
        sx = osx_to_view_x(x);
        sy = osx_to_view_y(y);

        set_position(sx, sy);
        softwaretext(&osx, t->text[i], strlen(t->text[i]));

        if (t->use_style) {
            text_box_line(&osx, sx, sy, t->width[i], t->style[i],
                          cwidth / (float) width, ascent / (float) width);
        }
        if (t->use_symbol) {
            text_box_symbol(&osx, sx, sy, t->symbol[i], t->use_style,
                            cwidth / (float) width, ascent / (float) width);
        }
        y -= cheight;
    }
    if(color_on()) {
        color_skeleton(&c);
    } else {
        color_foreground_default(&c);
    }
    osx_set_color(c);
}

void
osx_show_image(float *data, unsigned int iw, unsigned int ih, float xmin,
               float xmax, float ymin, float ymax, float x, float y, float w,
               float h, int npseudocolors, int nsacolors, int ndefcolors,
               int lbinary, int *nerr) {
    *nerr = 0;
    osx_sac_show_image(data, iw, ih, xmin, xmax, ymin, ymax, x, y, w, h,
                       npseudocolors, nsacolors, ndefcolors, lbinary);
}

void
osx_alpha_info(int *num_lines, char erase[], int erase_length) {
    UNUSED(erase_length);
    *num_lines = 40000;
    erase[0] = ' ';
}

void
osx_cursor(float *x, float *y, char c[], int length) {
    UNUSED(length);
    osx_sac_wait_for_keypress(x, y, c);
    *x = osx_to_view_x(*x);
    *y = osx_to_view_y(*y);
}

static int osx_fd[2] = { 0, 0 };

#define OSX_FD_READ  0
#define OSX_FD_WRITE 1
int
osx_get_file_descriptor(void) {
    static int init = FALSE;
    if (!init) {
        init = TRUE;
        pipe(osx_fd);
    }
    return osx_fd[OSX_FD_READ];
}

void
osx_gui_command(char *cmd) {
    size_t n;
    n = strlen(cmd);
    write(osx_fd[OSX_FD_WRITE], &n, sizeof(size_t));
    write(osx_fd[OSX_FD_WRITE], cmd, n * sizeof(char));
}

char *
osx_handle_event(int *nerr) {
    size_t n;
    char pmsg[1024];
    UNUSED(nerr);
    memset(pmsg, 0, sizeof(pmsg));
    read(osx_fd[OSX_FD_READ], &n, sizeof(size_t));      /* Length */
    read(osx_fd[OSX_FD_READ], &pmsg[0], n);     /* Message */
    return strdup(pmsg);
}

void
initdevice_osx() {

    initdevice_null(&osx);

    osx.name = strdup("MacOSX");
    osx.extension = strdup("osx");
    osx.id = 12;

    osx.active_device = TRUE;
    osx.cursor_enabled = TRUE;
    osx.begin_device = osx_begin_device;
    osx.begin_frame = osx_begin_frame;
    osx.begin_window = osx_begin_window;
    osx.cursor = osx_cursor;
    osx.create_window = osx_create_window;
    osx.draw = osx_draw;
    osx.stroke = osx_stroke;
    osx.drawpoly = osx_draw_poly;
    osx.end_device = osx_end_device;
    osx.end_frame = osx_end_frame;
    osx.flush_buffer = osx_flush_buffer;
    osx.get_ratio = osx_ratio;
    osx.get_device_ratio = osx_device_ratio;
    osx.get_alpha_info = osx_alpha_info;
    osx.move = osx_move;
    osx.set_color = osx_set_color;
    osx.set_color_table = osx_set_color_table;
    osx.set_line_width = osx_set_width;
    osx.set_pseudo_color_table = osx_set_pseudo_color_table;
    osx.text = osx_text;
    osx.textbox = osx_text_box;
    osx.show_image = osx_show_image;
    osx.get_window_size = osx_window_size;
    osx.get_file_descriptor = osx_get_file_descriptor;
    osx.handle_event = osx_handle_event;
    gdm_register_device(&osx);

}
