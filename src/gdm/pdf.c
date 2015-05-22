
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#include "amf.h"
#include "gdm.h"
#include "gem.h"
#include "bool.h"
#include "xyz.h"

#include "sgfcolor.h"

#include "docs/pdf.h"
#include "docs/font.h"
#include "array.h"

#include "debug.h"

#include "debug.h"

char *record_filename(char *in);

#define XSCALE 792
#define YSCALE 612

#define PDF_LINE_SPACING   1.333
#define PDF_FONT_SIZE_MIN 10.0

/* These patterns match those in sgftops.c */
void fill_pdf();

void put_image_pdf(char *data, unsigned int xloc, unsigned int yloc,
                   unsigned int width, unsigned int height, int *nerr);

display_t _pdf;

static pdf_t *xPDF = NULL;


void begindevice_pdf(int *nerr);
void beginframe_pdf(int *nerr);
void draw_pdf(float x, float y);
void drawpoly_pdf(float *x, float *y, int n);
void endframe_pdf(int *nerr);
char *fill_image_pdf(unsigned int height, unsigned int width, float data[],
                     float dmin, float range, int npseudocolors, int nsaccolors,
                     int ndefcolors, int *nerr);
char *fill_clrbar_pdf(int npseudocolors, int width, int npricolors,
                      int ndefcolors, int *nerr);
void getwindowstat_pdf(int number, int *exists);
void getratio_pdf(float *aspect);
void get_geometry_pdf(int number, unsigned int *width, unsigned int *height,
                      int *nerr);
void move_pdf(float x, float y);
void put_image3(char *data, unsigned int xloc, unsigned int yloc,
                unsigned int width, unsigned int height, int *nerr);
void setcolor_pdf(color rgb);
void setlinestyle_pdf(int *iline);
void setwidth_pdf(int index);
void settextangle_pdf(float angle);
void settextsize_pdf(float width, float height);
void text_pdf(display_t * out, char *text, int n);
void init_postscript(void);

void
save_pdf(display_t * out, char *file) {
    record_write_file(out, file);
}

void
begindevice_pdf(int *nerr) {
    *nerr = 0;
    set_skeleton_fudge(0.0003);
}

#ifdef __DEBUG__
static void
marker(float x, float y) {
    pdf_save(xPDF);
    pdf_moveto(xPDF, x, y);
    pdf_lineto(xPDF, x + 1, y);
    pdf_lineto(xPDF, x, y + 1);
    pdf_lineto(xPDF, x - 1, y);
    pdf_lineto(xPDF, x, y - 1);
    pdf_color_fill(xPDF, COLORS[1]);
    pdf_fill(xPDF);
    pdf_restore(xPDF);
}
#else
static void
marker(float x, float y) {
    UNUSED(x);
    UNUSED(y);
}
#endif

void
beginframe_pdf(int *nerr) {
    color cb;
    pdf_color_t c;

    *nerr = 0;
    if (xPDF) {
        pdf_free(xPDF);
        xPDF = NULL;
    }

    xPDF = pdf_new();
    pdf_page_new(xPDF);

    if (color_on()) {
        color_background(&cb);
        c.r = cb.r/255.;
        c.g = cb.g/255.;
        c.b = cb.b/255.;
        if (c.r != 1.0 || c.b != 1.0 || c.g != 1.0) {
            pdf_rectangle(xPDF, XSCALE, YSCALE, c);
        }
    }
}

void
show_image_pdf(float *data, unsigned int iw,    /* Size of the image in data points */
               unsigned int ih, float xmin,     /* Limits of the data */
               float xmax, float ymin, float ymax, float x,     /* In View space Coordinates [0,1] */
               float y, float w, float h, int npseudocolors, int nsacolors,
               int ndefcolors, int lbinary, int *nerr) {

    unsigned int wpw, wph;
    unsigned int ipw, iph;
    unsigned int ipx, ipy;
    float zmin, zmax;
    char *cdata;
    float *data_image;

    DEBUG("size: [%d %d] %d\n", iw, ih, iw * ih);

    /* Get the size of the window in pixels */
    get_geometry_pdf(0, &wpw, &wph, nerr);

    /* Determine the Image size in Pixels */
    ipw = (float) (wpw) * w;
    iph = (float) (wph) * h;

    /* Determine the Image location in Pixels */
    ipx = (float) (wpw) * x;
    ipy = (float) (wph) * (y - h);

    DEBUG("size: [%d %d] %d\n", ipw, iph, ipw * iph);
    DEBUG("loc:  [%d %d]\n", ipx, ipy);

    /* Scale Image from data [iw,ih] to output [ipw, iph] */
    data_image = (float *) malloc(sizeof(float) * ipw * iph);

    scaleimage(data, iw, ih, data_image, ipw, iph, xmin, xmax, ymin, ymax,
               nerr);

    /* Flatten to +- binary if wanted */
    if (lbinary) {
        fbinary(data_image, ipw * iph);
    }

    /* Determine the Z-Range of the image */
    frange(data_image, ipw * iph, &zmin, &zmax);

    /* Convert float data into color data */
    cdata =
        fill_image_pdf(iph, ipw, data_image, zmin, zmax - zmin, npseudocolors,
                       nsacolors, ndefcolors, nerr);

    /* Show data */
    put_image_pdf(cdata, ipx, ipy, ipw, iph, nerr);

    free(data_image);
    free(cdata);
}

void
calculate_location_pdf(float *x, float *y, float *cx, float *cy, unsigned int w,
                       unsigned int h, unsigned int iw, unsigned int ih) {
    UNUSED(iw);
    UNUSED(cx);

    /* Image X Position scaled by "Window" x-size */
    *x = *x * (float) w;
    /* Image Y Position scale by "Window" y-size 
     *   ==> Get top of window by subtracting 1.0
     *       remove dimensionless height to set the bottom of the window
     *       Put dimensions back in, multiply by "window" height
     */
    *y = ((1.0 - *y) - (float) ih / (float) h) * (float) h;
    /* Colorbar Y Position: Centered on Image height
     *  ==> (Image Height - Colorbar Height) / 2.0
     */
    *cy = (ih - cmgdm.npscimage) / 2.0;
}

void
draw_pdf(float x, float y) {
    pdf_lineto(xPDF, x * XSCALE, y * YSCALE);
}

void
fillpoly_pdf(float *x, float *y, int n) {
    int i;

    pdf_line_join_style(xPDF, PDF_LINE_JOIN_STYLE_ROUND);
    move_pdf(x[0], y[0]);
    for (i = 1; i < n; i++) {
        pdf_lineto(xPDF, x[i] * XSCALE, y[i] * YSCALE);
    }
    fill_pdf();
}

void
drawpoly_pdf(float *x, float *y, int n) {
    int i;

    pdf_line_join_style(xPDF, PDF_LINE_JOIN_STYLE_ROUND);
    move_pdf(x[0], y[0]);
    for (i = 1; i < n; i++) {
        pdf_lineto(xPDF, x[i] * XSCALE, y[i] * YSCALE);
    }
}

void
endframe_pdf(int *nerr) {
    char *file;

    file = record_filename(NULL);

    xPDF->fp = fopen(file, "wb");

    if (!xPDF->fp) {
        *nerr = 102;
        return;
    }
    pdf_output(xPDF);
    pdf_free(xPDF);
    xPDF = NULL;
}

char *
fill_image_pdf(unsigned int height, unsigned int width, float data[],
               float dmin, float range, int npseudocolors, int nsaccolors,
               int ndefcolors, int *nerr) {
    int i, j, k;
    char *array;
    float v;
    UNUSED(nerr);
    UNUSED(ndefcolors);
    UNUSED(nsaccolors);

    array = (char *) malloc(sizeof(char) * 3 * width * height);
    k = 0;
    for (i = height - 1; i >= 0; i--) {
        for (j = 0; j < (int) width; j++) {
            v = ((data[(i * width) + j] -
                  dmin) / range) * (float) npseudocolors;
            v = (v < 0.0) ? 0.0 : v;
            v = (v >
                 (float) npseudocolors - 1) ? (float) (npseudocolors - 1) : v;
            array[k++] = sred[(unsigned int) v];
            array[k++] = sgreen[(unsigned int) v];
            array[k++] = sblue[(unsigned int) v];
        }
    }
    return array;
}

char *
fill_clrbar_pdf(int npseudocolors,      /* number of pseudocolors in the colortable */
                int width,      /* number of elements in one scan line of the color bar */
                int npricolors, /* number of SAC primary colors */
                int ndefcolors, /* number of default colors in the colortable */
                int *nerr) {

    int i, j, k, n;
    char *array;
    UNUSED(nerr);
    UNUSED(ndefcolors);
    UNUSED(npricolors);

    array = (char *) malloc(3 * width * npseudocolors * sizeof(char));
    k = npseudocolors - 1;
    n = 0;
    for (i = 0; i < npseudocolors; i++) {
        for (j = 0; j < width; j++) {
            array[n++] = sred[k];
            array[n++] = sgreen[k];
            array[n++] = sblue[k];
        }
        k--;
    }
    return array;
}

void
getwindowstat_pdf(int number, int *exists) {
    UNUSED(number);
    *exists = FALSE;
}

void
getratio_pdf(float *aspect) {
    *aspect = (float) YSCALE / (float) XSCALE;
    *aspect = 1.0;
}

void
get_geometry_pdf(int number, unsigned int *width, unsigned int *height,
                 int *nerr) {
    UNUSED(nerr);
    UNUSED(number);
    *width = XSCALE;
    *height = YSCALE;
}

void
move_pdf(float x, float y) {
    pdf_moveto(xPDF, x * XSCALE, y * YSCALE);
}

void
put_image_pdf(char *data, unsigned int xloc, unsigned int yloc,
              unsigned int width, unsigned int height, int *nerr) {

    pdf_object_t *s;
    char *name;
    static int k = 1;
    UNUSED(nerr);

    asprintf(&name, "Img%d", k);

    /* Generate Image Showing Stream */
    pdf_save(xPDF);
    pdf_translate_scale(xPDF, (float) xloc, (float) yloc, (float) width,
                        (float) height);
    pdf_image_show(xPDF, name);
    pdf_restore(xPDF);

    /* Image Data Stream */
    s = pdf_image_new();
    pdf_image_set(s, data, width, height, PDF_IMAGE_RGB);
#ifdef HAVE_ZLIB
    pdf_image_set_encoding(s, PDF_ENCODE_FLATE);
#else
    pdf_image_set_encoding(s, PDF_ENCODE_RLE);
#endif
    pdf_object_add(xPDF, s);

    /* Add Color Images Routines to Procedure Set */
    pdf_image_color_add_resources(xPDF);

    /* Add Image Data to Page Resources */
    pdf_image_add_xobject(xPDF, s, name);

    k = k + 1;
    free(name);

}

void
setcolor_pdf(color rgb) {
    pdf_color_t c;
    c.r = rgb.r/255.;
    c.g = rgb.g/255.;
    c.b = rgb.b/255.;
    if (xPDF && xPDF->stream) {
        pdf_color_stroke(xPDF, c);
    }
}

void
line_style_pdf(char *line, void *data) {
    char *c;
    char ***v = (char ***) data;
    asprintf(&c, "[ %s ] 0 d", line);
    *v = xarray_append(*v, strdup(c));
    FREE(c);
}

void
setlinestyle_pdf(int *iline) {
    int k;
    static char **dash = NULL;
    k = *iline;
    if (k <= 0) {
        k = 1;
    }
    if (!dash) {
        dash = xarray_new('p');
        dash = xarray_append(dash, strdup("[] 0 d"));
        sac_line_style_read(line_style_pdf, &dash);
    }
    pdf_stream_add(xPDF->stream, "%s\n", dash[k - 1]);
}

void
setwidth_pdf(int index) {
    pdf_linewidth(xPDF, index);
}

void
stroke_pdf() {
    pdf_stroke(xPDF);
}

void
fill_pdf() {
    pdf_fill(xPDF);
}

void
textbox_pdf(textbox * t) {
    int i;
    int len, xlen;
    float x, y, w, h, sx, sy;
    int height, size;
    pdf_color_t c;

    if (!t || t->n == 0) {
        return;
    }

    gettextsize(&w, &h);

    size = h * 10 * 72;

    len = 0;
    x = t->x * XSCALE;
    y = t->y * YSCALE;

    height = string_height("Helvetica", size) * PDF_LINE_SPACING;
    DEBUG("height: %d position: (%f %f)\n", height, x, y);
    if (t->location & TEXT_BOX_RIGHT) {
        for (i = 0; i < t->n; i++) {
            xlen = string_width("Helvetica", t->text[i], size);
            if (xlen > len) {
                len = xlen;
            }
        }
        x = x - len;
    }
    if (t->location & TEXT_BOX_LOWER) {
        y += height * (t->n - 1);
    } else {
        y -= height;
    }

    if (t->use_style && t->location & TEXT_BOX_LEFT) {
        x += 0.5 * 1.25 * string_average_width("Helvetica",
                                               size) +
            TEXTBOX_LINE_LENGTH_PIXELS;
    }

    for (i = 0; i < t->n; i++) {
        pdf_save(xPDF);
        c.r = t->color[i].r/255.;
        c.g = t->color[i].g/255.;
        c.b = t->color[i].b/255.;
        pdf_color_fill(xPDF, c);
        pdf_text_font(xPDF, "/F1", size);
        pdf_text_begin(xPDF);
        pdf_text_position(xPDF, x, y);
        pdf_text_show(xPDF, t->text[i]);
        pdf_text_end(xPDF);
        pdf_stroke(xPDF);
        pdf_restore(xPDF);

        sx = x / (float) XSCALE;
        sy = y / (float) YSCALE;
        if (t->use_style) {
            text_box_line(&_pdf, sx, sy, t->width[i], t->style[i],
                          1.25 * string_average_width("Helvetica",
                                                      size) / (float) XSCALE,
                          height / (float) YSCALE / 2.0);
        }
        if (t->use_symbol) {
            text_box_symbol(&_pdf, sx, sy, t->symbol[i], t->use_style,
                            1.25 * string_average_width("Helvetica",
                                                        size) / (float) XSCALE,
                            height / (float) YSCALE / 2.0);
        }
        y -= height;
    }

}

void
text_pdf(display_t * out, char *text, int n) {
    char c;
    float dx, dy, w, h, x, y;
    float angle;
    float size;
    float off[4] = { 0.0, 0.0, -0.5, -1.0 };
    UNUSED(out);
    gettextsize(&w, &h);

    get_position(&x, &y);
    angle = gettextangle();
    x = x * XSCALE;
    y = y * YSCALE;

    size = h * 10 * 72;
    c = text[n];
    text[n] = 0;

    size = (size < PDF_FONT_SIZE_MIN) ? PDF_FONT_SIZE_MIN : size;

    dx = string_width("Helvetica", text, size) * off[cmgdm.ihjust];
    dy = string_height("Helvetica", size) * off[cmgdm.ivjust];

    DEBUG("text  %s (%f %f) (%f %f) %d %d\n", text, x, y, dx, dy, cmgdm.ihjust,
          cmgdm.ivjust);

    pdf_stream_backup(xPDF);
    pdf_save(xPDF);
    pdf_text_font(xPDF, "/F1", size);
    pdf_text_begin(xPDF);

    if (angle == 0.0) {
        pdf_text_position(xPDF, x + dx, y + dy);
    } else {
        pdf_text_matrix(xPDF, x - dy, y + dx, angle);
    }
    pdf_text_show(xPDF, text);
    pdf_text_end(xPDF);
    pdf_restore(xPDF);
    pdf_stroke(xPDF);
    text[n] = c;

    marker(x, y);

}

static char *pdf_name = "PDF";
static char *pdf_ext = "pdf";

void
initdevice_pdf() {
    initdevice_null(&_pdf);
    _pdf.name = pdf_name;
    _pdf.extension = pdf_ext;
    _pdf.id = PDF;
    _pdf.begin_device = begindevice_pdf;
    _pdf.begin_frame = beginframe_pdf;
    _pdf.calc_loc = calculate_location_pdf;
    _pdf.draw = draw_pdf;
    _pdf.drawpoly = drawpoly_pdf;
    _pdf.fillpoly = fillpoly_pdf;
    _pdf.end_frame = endframe_pdf;
    _pdf.get_window_status = getwindowstat_pdf;
    _pdf.get_ratio = getratio_pdf;
    _pdf.get_device_ratio = getratio_pdf;
    _pdf.get_geometry = get_geometry_pdf;
    _pdf.move = move_pdf;
    _pdf.put_image = put_image_pdf;
    _pdf.set_color = setcolor_pdf;
    _pdf.set_line_style = setlinestyle_pdf;
    _pdf.set_line_width = setwidth_pdf;
    _pdf.stroke = stroke_pdf;
    _pdf.text = text_pdf;
    _pdf.textbox = textbox_pdf;
    _pdf.show_image = show_image_pdf;
    _pdf.save = save_pdf;
    gdm_register_device(&_pdf);
}
