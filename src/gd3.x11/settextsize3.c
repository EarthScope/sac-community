/*******************************************************************************
** PURPOSE:
*    To set the textsize.
*
** INPUT ARGUMENTS:
*    width:    Width of text in fraction of viewport. (Pointer)
*    height:   Height of text in fraction of viewport. (Pointer)
*
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "config.h"

#include "gdm.h"
#include "gd3.x11.h"
#include "bbs.h"
#include "bool.h"

#include "dff.h"                /* fstrdup */
#include "gem.h"                /* color_on, color_skeleton, color_foreground_default */

#include "debug.h"

#ifdef HAVE_XFT
#include <X11/Xft/Xft.h>
#endif

#include "debug.h"

#define BASE "-*-helvetica-medium-r-normal-*"

#define XFT_FONT_MIN  8

#define PI M_PI
enum {
    XFONT_BASE_HELVETICA,
    XFONT_BASE_TIMES,
    XFONT_BASE_COURIER,
    XFONT_BASE_ZAPFDINGBATS,
};

static int font_system = XFONT_TYPE_SOFTWARE;
static int font_base = XFONT_BASE_HELVETICA;

char *font_bases[] = { "Helvetica",
    "Times",
    "Courier",
    "Wingdings",
    "ZapfDingbats"
};

extern display_t x11;

char *xfont_style(XFont * f);

int
int_compare(const void *pa, const void *pb) {
    int *a, *b;
    a = (int *) pa;
    b = (int *) pb;
    if (*a == *b) {
        return 0;
    }
    if (*a < *b) {
        return -1;
    }
    return 1;
}

int *
unique(int *z, int *n) {
    int *out;
    int i, k, j;

    qsort(z, *n, sizeof(int), int_compare);

    j = 0;
    while (z[j] <= 0) {
        j++;
    }

    z[0] = z[j];
    k = 1;
    for (i = j; i < *n; i++) {
        if (z[i] != z[k - 1]) {
            z[k++] = z[i];
        }
    }
    *n = k;
    out = (int *) malloc(sizeof(int) * k);
    for (i = 0; i < k; i++) {
        out[i] = z[i];
    }
    free(z);
    return out;
}

XFontStruct **
unique_font_struct(char *base, int **out_sizes, int *len) {
    int i, j, k, n;
    int *sizes;
    char *font, *p, *c;
    char **list;
    XFontStruct **fs;
    XScreen *xs;

    xs = xscreen_get();

    sizes = (int *) malloc(sizeof(int) * 1000);
    k = 0;

    list = XListFonts(xs->display, base, 1000, &n);
    for (i = 0; i < n; i++) {
        font = strdup(list[i]);
        p = font;
        for (j = 0; j < 14; j++) {
            c = strsep(&font, "-");
            if (j == 7) {
                sizes[k++] = atoi(c);
            }
        }
        free(p);
    }
    XFreeFontNames(list);

    sizes = unique(sizes, &k);

    fs = (XFontStruct **) malloc(sizeof(XFontStruct *) * k);

    for (i = 0; i < k; i++) {
        asprintf(&p, "%s-%d-*", base, sizes[i]);
        fs[i] = XLoadQueryFont(xs->display, p);
        free(p);
        p = NULL;
    }

    *len = k;
    *out_sizes = sizes;
    return fs;
}

/* Set Font Size */
void
xwindow_set_font_size_core(XWindow * xw, float width, float height) {
    int i, k;
    int w;
    float s, ds;
    char *p;

    static int n = 0;
    static XFontStruct **fs = NULL;
    static int *sizes = NULL;

    UNUSED(width);

    if (!xw) {
        return;
    }

    /* Window dimensions */
    w = xw->width;

    /* Load Fond Data, if not already available */
    if (!fs) {
        fs = unique_font_struct(BASE, &sizes, &n);
    }

    /* Find font size based on font height in window 
     *  This is a bit weird as the scaling is done by
     *  the window width, but this fits with the 
     *  output and scaling of the softwaretext() routine
     */
    k = -1;
    ds = 1.0;
    for (i = 0; i < n; i++) {
        s = fabs(height -
                 ((float) (fs[i]->ascent + fs[i]->descent) / (float) w));
        if (s < ds) {
            k = i;
            ds = s;
        }
    }

    /* Load the closest font */
    if (k >= 0) {
        asprintf(&p, "%s-%d-*", BASE, sizes[k]);
        if (xw->font->font_core) {
            XFreeFont(DISPLAY(xw), xw->font->font_core);
        }
        xw->font->font_core = XLoadQueryFont(DISPLAY(xw), p);
        if (xw->font->font_core == None) {
            fprintf(stderr, "error loading font: %s\n", p);
        } else {
            XSetFont(DISPLAY(xw), xw->gc, xw->font->font_core->fid);
        }

        free(p);
        p = NULL;
    }
}

#ifdef HAVE_XFT

void
xwindow_set_font_size_xft(XWindow * xw, float width, float height) {
    XScreen *xs;
    float pt;
    UNUSED(height);
    UNUSED(width);
    xs = xscreen_get();

    /* 
     *                   Current_Window_Height
     * New_Font_Size = -------------------------- * Original_Font_Size 
     *                   Original_Window_Height
     *
     */
    pt = (xw->height / (xs->height * 0.55)) * 12.0;

    /* Round to nearest integer value */
    pt = (int) (pt + 0.5);

    if (pt < XFT_FONT_MIN) {
        pt = XFT_FONT_MIN;
    }

    xwindow_font_set_size(xw, pt);
}

#else

void
xwindow_set_font_size_xft(XWindow * xw, float width, float height) {
    UNUSED(xw);
    UNUSED(width);
    UNUSED(height);
    fprintf(stderr, "SAC not compiled with XFT support\n");
}

#endif

void
settextsize3(float width, float height) {

    XWindow *xw;

    xw = plot_window(CURRENT);
    DEBUG("[%f %f]\n", width, height);
    switch (xw->font->type) {
        case XFONT_TYPE_SOFTWARE:
            break;
        case XFONT_TYPE_CORE:
            //xwindow_set_font_size_core(xw, width, height);
            xwindow_set_font_size_xft(xw, width, height);
            xwindow_font_load(xw);
            xw->font->font_core = NULL;
            break;
#ifdef HAVE_XFT
        case XFONT_TYPE_XFT:
            xwindow_set_font_size_xft(xw, width, height);
            xwindow_font_load(xw);
            xw->font->font_xft = NULL;
            break;
#endif /* HAVE_XFT */
        default:
            fprintf(stderr, "SAC: Unknown X11 Font Subsystem\n");
            break;
    }
}

/* Draw String */
#ifdef HAVE_XFT

void
xwindow_draw_string_xft(XWindow * xw, char *text, int x, int y) {
    int n;
    XftDraw *draw;
    XftColor color;
    XRenderColor rc;
    XftFont *font;
    XGlyphInfo extents;
    float shift[] = { 0.0, 0.0, -0.5, -1.0 };

    n = strlen(text);

    font = xw->font->font_xft;
    if (!font) {
        return;
    }
    draw =
        XftDrawCreate(DISPLAY(xw), xw->buffer, VISUAL(xw),
                      DefaultColormap(DISPLAY(xw), SCREEN(xw)));

    /* Set Text from current Color */
    rc.red = xw->color.red;
    rc.green = xw->color.green;
    rc.blue = xw->color.blue;
    rc.alpha = 0xFFFF;          /* 65535 */
    XftColorAllocValue(DISPLAY(xw), VISUAL(xw),
                       DefaultColormap(DISPLAY(xw), SCREEN(xw)), &rc, &color);

    XftTextExtents8(DISPLAY(xw), font, (FcChar8 *) text, n, &extents);

    if (cmgdm.tangle != 0.0) {
        XftMatrix m;
        m.xx = 0.0;
        m.xy = -1.0;
        m.yx = 1.0;
        m.yy = 0.0;

        y = y - shift[cmgdm.ihjust] * extents.xOff;
        x = x - shift[cmgdm.ivjust] * (font->ascent + font->descent);

        font =
            XftFontOpen(DISPLAY(xw), SCREEN(xw), XFT_FAMILY, XftTypeString,
                        xw->font->family, XFT_SIZE, XftTypeDouble,
                        xw->font->size, XFT_STYLE, XftTypeString,
                        xfont_style(xw->font), XFT_ANTIALIAS, XftTypeBool,
                        xw->font->antialias, XFT_MATRIX, XftTypeMatrix, &m,
                        NULL);

    } else {
        x = x + shift[cmgdm.ihjust] * extents.xOff;
        y = y - shift[cmgdm.ivjust] * (font->ascent + font->descent);
    }
    XftDrawString8(draw, &color, font, x, y, (FcChar8 *) text, n);
}

#else

void
xwindow_draw_string_xft(XWindow * xw, char *text, int x, int y) {
    UNUSED(xw);
    UNUSED(text);
    UNUSED(x);
    UNUSED(y);
    fprintf(stderr, "SAC not compiled with XFT support\n");
}

#endif /* HAVE_XFT */

void
XTextSize(XFontStruct * font_struct, char *string, int length,
          XCharStruct * size) {
    int dir, ascent, descent;
    XTextExtents(font_struct, string, length, &dir, &ascent, &descent, size);
}

void
XDrawString_90(XWindow * xw, Drawable d, int x, int y, char *string, int length) {
    int i, j;
    XCharStruct size;
    unsigned int width, height;
    unsigned int x0, y0;
    XImage *in, *out;
    Pixmap pix;
    XGCValues val;

    XGetGCValues(DISPLAY(xw), xw->gc, GCForeground | GCBackground, &val);

    XTextSize(xw->font->font_core, string, length, &size);

    width = -size.lbearing + size.rbearing;
    height = size.ascent + size.descent;
    x0 = -size.lbearing;
    y0 = size.ascent;

    pix = XCreatePixmap(DISPLAY(xw), ROOT(xw), width, height, DEPTH(xw));

    /* Background Fill */
    XSetForeground(DISPLAY(xw), xw->gc, val.background);
    XFillRectangle(DISPLAY(xw), pix, xw->gc, 0, 0, width, height);
    XSetForeground(DISPLAY(xw), xw->gc, val.foreground);

    XDrawString(DISPLAY(xw), pix, xw->gc, x0, y0, string, length);

    /* Convert from pixmap to image */
    in = XGetImage(DISPLAY(xw), pix, 0, 0, width, height, AllPlanes, ZPixmap);
    XFreePixmap(DISPLAY(xw), pix);

    /* Create Image with Width and Height exchanged */
    out =
        XCreateImage(DISPLAY(xw), VISUAL(xw), DEPTH(xw), ZPixmap, 0, NULL,
                     height, width, 32, 0);
    out->data = (char *) malloc(sizeof(char) * width * out->bytes_per_line);

    /* "Rotate" Image */
    for (j = 0; j < (int) height; j++) {
        for (i = 0; i < (int) width; i++) {
            /* width - i - 1 : Flip the Image Vertically  */
            XPutPixel(out, j, width - i - 1, XGetPixel(in, i, j));
        }
    }
    pix = XCreatePixmap(DISPLAY(xw), ROOT(xw), height, width, DEPTH(xw));
    XPutImage(DISPLAY(xw), pix, xw->gc, out, 0, 0, 0, 0, height, width);
    XCopyArea(DISPLAY(xw), pix, d, xw->gc, 0, 0, height, width, x, y);

    XFreePixmap(DISPLAY(xw), pix);
    XDestroyImage(out);
    XDestroyImage(in);
}

char *iso8859_1[] = {
    "dotlessi",                 //220
    "grave",                    //221
    "undef",                    //222
    "circumflex",               //223
    "tilde",                    //224
    "undef",                    //225
    "breve",                    //226
    "dotaccent",                //227

    "undef",                    //230
    "undef",                    //231
    "ring",                     //232
    "undef",                    //233
    "undef",                    //234
    "hungarumlaut",             //235
    "ogonek",                   //236
    "caron",                    //237

    "NBSP",                     //240 160
    "exclamdown",               //241 161
    "cent",                     //242 162
    "sterling",                 //243 163
    "currency",                 //244 168
    "yen",                      //245
    "brokenbar",                //246
    "section",                  //247

    "dieresis",                 //250
    "copyright",                //251
    "ordfeminine",              //252
    "guillemotleft",            //253
    "logicalnot",               //254
    "hyphen",                   //255
    "registered",               //256
    "macron",                   //257

    "degree",                   //260
    "plusminus",                //261
    "twosuperior",              //262
    "threesuperior",            //263
    "acute",                    //264
    "mu",                       //265
    "paragraph",                //266
    "periodcentered",           //267

    "cedilla",                  //270
    "onesuperior",              //271
    "ordmasculine",             //272
    "guillemotright",           //273
    "onequarter",               //274
    "onehalf",                  //275
    "threequarters",            //276
    "questiondown",             //277

    "Agrave",                   //300
    "Aacute",                   //301
    "Acircumflex",              //302
    "Atilde",                   //303
    "Adieresis",                //304
    "Aring",                    //305
    "AE",                       //306
    "Ccedilla",                 //307

    "Egrave",                   //310
    "Eacute",                   //311
    "Ecircumflex",              //312
    "Edieresis",                //313
    "Igrave",                   //314
    "Iacute",                   //315
    "Icirculflex",              //316
    "Idieresis",                //317

    "Eth",                      //320
    "Ntilde",                   //321
    "Ograve",                   //322
    "Oacute",                   //323
    "Ocircumflex",              //324
    "Otilde",                   //325
    "Odieresis",                //326
    "multiply",                 //327

    "Oslash",                   //330
    "Ugrave",                   //331
    "Uacute",                   //332
    "Ucircumflex",              //333
    "Udieresis",                //334
    "Yacute",                   //335
    "Thorn",                    //336
    "germandbls",               //337

    "agrave",                   //340
    "aacute",                   //341
    "acircumflex",              //342
    "atilde",                   //343
    "adieresis",                //344
    "aring",                    //345
    "ae",                       //346
    "ccedilla",                 //347

    "egrave",                   //350
    "eacute",                   //351
    "ecircumflex",              //352
    "edieresis",                //353
    "igrave",                   //354
    "iacute",                   //355
    "icircumflex",              //356
    "idieresis",                //357

    "eth",                      //360
    "ntilde",                   //361
    "ograve",                   //362
    "oacute",                   //363
    "ocircumflex",              //364
    "otilde",                   //365
    "odieresis",                //366
    "divide",                   //367

    "oslash",                   //370
    "ugrave",                   //371
    "uacute",                   //372
    "ucircumflex",              //373
    "udieresis",                //374
    "yacute",                   //375
    "thorn",                    //376
    "ydieresis",                //377
};

char
name_to_octal(char *name) {
    int i;

    //fprintf(stderr, "name: %s\n", name);
    if (!name) {
        return '\0';
    }
    for (i = 0; i < (int) (sizeof(iso8859_1) / sizeof(char *)); i++) {
        if (strcmp(name, iso8859_1[i]) == 0) {
            /*fprintf(stderr, "%d %c '%s' '%s'\n", 144+i, (char)144+i, name, iso8859_1[i]); */
            return (char) (144 + i);
        }
    }
    return '\0';
}

char *
encode(char *str) {
    int n;
    char oct;
    char name[100];
    char *c, *start, *end;

    /* Find  '\' */
    c = strchr(str, '\\');
    while (c) {
        memset(name, 0, 100);
        start = c + 1;
        if (*start == '{') {    /* Brace enclosed name */
            end = strchr(start, '}');
            if (!end) {
                fprintf(stderr, "sac string encode: missing }\n");
                return str;
            }
            /* Grab character name */
            strncpy(&name[0], start + 1, end - (start + 1));
            name[end - (start)] = 0;
            end++;
        } else {                /* Not Brace enclosed */
            end = start;
            /* Terminate at End-of-String, whitespace of \ */
            while (*end && !isspace(*end) && *end != '\\') {
                end++;
            }
            /* Grab character name */
            if (end - start > 0 && end - start < (int) sizeof(name)) {
                strncpy(&name[0], start, end - start);
                name[end - start] = 0;
            }
        }
        /* Convert name to octal code */
        if ((oct = name_to_octal(&name[0]))) {
            *c = oct;
            n = strlen(end);
            memmove(c + 1, end, strlen(end));
            c[n + 1] = 0;
        }
        /* Advance is no conversion done */
        if (*c == '\\') {
            c++;
        }
        //fprintf(stderr, "%s\n", str);
        /* Find next \ */
        c = strchr(c, '\\');
    }
    return str;
}

void
xwindow_draw_string_core(XWindow * xw, char *text, int x, int y) {
    int text_width;
    float shift[] = { 0.0, 0.0, -0.5, -1.0 };
    XFontStruct *font;

    font = xw->font->font_core;

    /* Set Foreground Color */
    XSetForeground(DISPLAY(xw), xw->gc, xw->color.pixel);

    /* Determine the Width of Font and Text */
    text_width = XTextWidth(font, text, strlen(text));

    if (fabs(cmgdm.tangle - 0.0) < 1e-3) {      /* Horizontal Text */
        x = x + shift[cmgdm.ihjust] * text_width;
        y = y - shift[cmgdm.ivjust] * (font->ascent + font->descent);

        XDrawString(DISPLAY(xw), xw->buffer, xw->gc, (int) x, (int) y, text,
                    strlen(text));
    } else {                    /* Vertical Text */
        x = x + shift[cmgdm.ivjust] * (font->ascent + font->descent);
        y = y + shift[cmgdm.ihjust] * text_width;

        XDrawString_90(xw, xw->buffer, (int) x, (int) y, text, strlen(text));
    }

}

void
text_x11(display_t * out, char *text, int n) {

    float x, y;
    char *tmp;
    XWindow *win;

    win = plot_window(CURRENT);
    xwindow_font_load(win);

    tmp = fstrdup(text, n);

    tmp = encode(tmp);

    get_position(&x, &y);

    x = view_to_x11_x(x, win);
    y = view_to_x11_y(y, win);

    switch (win->font->type) {
        case XFONT_TYPE_SOFTWARE:
            softwaretext(out, text, n);
            break;
        case XFONT_TYPE_CORE:
            xwindow_draw_string_core(win, tmp, x, y);
            break;
        case XFONT_TYPE_XFT:
            xwindow_draw_string_xft(win, tmp, x, y);
            break;
        default:
            fprintf(stderr, "SAC: Unknown X11 Font Subsystem\n");
            break;
    }
    free(tmp);
    tmp = NULL;
}

#ifdef HAVE_XFT

int
xwindow_text_width_xft(XWindow * xw, char *text) {
    XGlyphInfo extents;
    XftTextExtents8(DISPLAY(xw), xw->font->font_xft, (FcChar8 *) text,
                    strlen(text), &extents);
    return extents.xOff;
}

#else

int
xwindow_text_width_xft(XWindow * xw, char *text) {
    UNUSED(xw);
    UNUSED(text);
    fprintf(stderr, "SAC not compiled with XFT Support\n");
    return 0;
}

#endif /* HAVE_XFT */

int
xwindow_text_width_core(XWindow * xw, char *text) {
    return XTextWidth(xw->font->font_core, text, strlen(text));
}

int
xwindow_text_width(XWindow * xw, char *text) {
    float size = 0;
    xwindow_font_load(xw);
    switch (xw->font->type) {
        case XFONT_TYPE_SOFTWARE:
            getstringsize(text, strlen(text), &size);
            return view_to_x11_x(size, xw);
        case XFONT_TYPE_CORE:
            return xwindow_text_width_core(xw, text);
            break;
        case XFONT_TYPE_XFT:
            return xwindow_text_width_xft(xw, text);
            break;
        default:
            fprintf(stderr, "SAC: Unknown X11 Font Subsystem\n");
            break;
    }
    return 0;
}

void
xwindow_draw_string(XWindow * xw, char *text, int x, int y) {
    xwindow_font_load(xw);
    switch (xw->font->type) {
        case XFONT_TYPE_SOFTWARE:
            set_position(x11_to_view_x(x, xw), x11_to_view_y(y, xw));
            softwaretext(&x11, text, strlen(text));
            break;
        case XFONT_TYPE_CORE:
            xwindow_draw_string_core(xw, text, x, y);
            break;
        case XFONT_TYPE_XFT:
            xwindow_draw_string_xft(xw, text, x, y);
            break;
        default:
            fprintf(stderr, "SAC: Unknown X11 Font Subsystem\n");
            break;
    }
}

void
text_box_x11(textbox * t) {
    int i;
    float x, y, sx, sy;
    int len, xlen, cwidth, cheight, ascent;
    XWindow *xw;
    XFontStruct *font;

    if (!t || t->n == 0) {
        return;
    }
    len = 0;

    cmgdm.ihjust = 1;
    cmgdm.ivjust = 1;

    xw = plot_window(CURRENT);

    xwindow_font_load(xw);

    if (xw->font->type == XFONT_TYPE_SOFTWARE) {
        font = NULL;
        /* All relative lengths scaled by window width */
        cwidth = view_to_x11_x(cmgdm.twidth, xw);
        cheight = view_to_x11_x(cmgdm.thgt, xw);
        ascent = view_to_x11_x(cmgdm.thgt * 1.10, xw);
    }
    if (xw->font->type == XFONT_TYPE_CORE) {
        font = xw->font->font_core;
        cwidth = font->max_bounds.width;
        cheight = font->ascent + font->descent;
        ascent = font->ascent;
    }
#ifdef HAVE_XFT
    else if (xw->font->type == XFONT_TYPE_XFT) {
        cwidth = xw->font->font_xft->max_advance_width;
        cheight = xw->font->font_xft->ascent + xw->font->font_xft->descent;
        ascent = xw->font->font_xft->ascent;
    }
#endif /* HAVE_XFT */
    x = view_to_x11_x(t->x, xw);
    y = view_to_x11_y(t->y, xw);

    /* Adjust for Text Length */
    if (t->location & TEXT_BOX_RIGHT) {
        for (i = 0; i < t->n; i++) {
            xlen = xwindow_text_width(xw, t->text[i]);
            if (xlen > len) {
                len = xlen;
            }
        }
        x = x - len;
    }

    /* Adjust for Text Height */
    if (t->location & TEXT_BOX_LOWER) {
        y -= cheight * (t->n - 1);
    } else {
        y += cheight;
    }

    if (t->use_style && t->location & TEXT_BOX_LEFT) {
        x += 0.5 * cwidth + TEXTBOX_LINE_LENGTH_PIXELS;
    }

    /* Show Text in correct color */
    for (i = 0; i < t->n; i++) {
        setcolor3(t->color[i]);
        XSetForeground(DISPLAY(xw), xw->gc, xw->color.pixel);
        if (t->text[i]) {
            xwindow_draw_string(xw, t->text[i], (int) x, (int) y);
        }
        sx = x11_to_view_x(x, xw);
        sy = x11_to_view_y(y, xw);

        if (t->use_style) {
            text_box_line(&x11, sx, sy, t->width[i], t->style[i],
                          cwidth / (float) xw->width,
                          ascent / (float) xw->width);
        }

        if (t->use_symbol) {
            text_box_symbol(&x11, sx, sy, t->symbol[i], t->use_style,
                            cwidth / (float) xw->width,
                            ascent / (float) xw->width);
        }
        y += cheight;
    }

    /* Return Color for Skeleton */
    if(color_on()) {
        setcolor_skel();
    } else {
        setcolor_fg_def();
    }


}

void
xwindow_font_set_family(XWindow * xw, char *family) {
    XFont *f = xw->font;
    if (f->family) {
        free(f->family);
        f->family = NULL;
    }
    f->family = strdup(family);
}

void
xwindow_font_set_size(XWindow * xw, float size) {
    xw->font->size = size;
}

void
xwindow_font_set_weight(XWindow * xw, int weight) {
    xw->font->weight = weight;
}

void
xwindow_font_set_slant(XWindow * xw, int slant) {
    xw->font->slant = slant;
}

void
xwindow_font_set_antialias(XWindow * xw, int aa) {
    xw->font->antialias = aa;
}

void
xwindow_font_set_type(XWindow * xw, int type) {
    xw->font->type = type;
}

XFont *
xfont_new() {
    XFont *f;
    f = (XFont *) malloc(sizeof(XFont));
    f->size = 12.0;
    //f->family    = strdup("Helvetica");
    f->weight = XFONT_WEIGHT_MEDIUM;
    //  f->weight    = FC_WEIGHT_REGULAR;
    f->slant = XFONT_SLANT_ROMAN;
    f->antialias = TRUE;

    //f->type      = XFONT_TYPE_CORE;
    f->font_core = NULL;

#ifdef HAVE_XFT
    //f->type      = XFONT_TYPE_XFT;
    f->font_xft = NULL;
#endif /* HAVE_XFT */
    //f->type      = XFONT_TYPE_SOFTWARE;
    f->type = font_system;
    f->family = strdup(font_bases[font_base]);
    return f;
}

char *
xfont_core_weight(XFontWeight weight) {
    switch (weight) {
        case XFONT_WEIGHT_LIGHT:
            return "regular";
            break;
        case XFONT_WEIGHT_MEDIUM:
            return "medium";
            break;
        case XFONT_WEIGHT_DEMIBOLD:
            return "demibold";
            break;
        case XFONT_WEIGHT_BOLD:
        case XFONT_WEIGHT_BLACK:
            return "bold";
            break;
        default:
            break;
    }
    return "*";
}

char *
xfont_core_slant(XFontSlant slant) {
    switch (slant) {
        case XFONT_SLANT_ROMAN:
            return "r";
            break;
        case XFONT_SLANT_ITALIC:
            return "i";
            break;
        case XFONT_SLANT_OBLIQUE:
            return "o";
            break;
        default:
            break;
    }
    return "*";
}

char *
xfont_style(XFont * f) {
    if (f->slant == XFONT_SLANT_ROMAN) {
        switch (f->weight) {
            case XFONT_WEIGHT_LIGHT:
            case XFONT_WEIGHT_MEDIUM:
                return "Regular";
                break;
            case XFONT_WEIGHT_DEMIBOLD:
            case XFONT_WEIGHT_BOLD:
            case XFONT_WEIGHT_BLACK:
                return "Bold";
                break;
        }
    }
    if (f->slant == XFONT_SLANT_ITALIC) {
        switch (f->weight) {
            case XFONT_WEIGHT_LIGHT:
            case XFONT_WEIGHT_MEDIUM:
                return "Italic";
                break;
            case XFONT_WEIGHT_DEMIBOLD:
            case XFONT_WEIGHT_BOLD:
            case XFONT_WEIGHT_BLACK:
                return "Bold Italic";
                break;
        }
    }
    if (f->slant == XFONT_SLANT_OBLIQUE) {
        switch (f->weight) {
            case XFONT_WEIGHT_LIGHT:
            case XFONT_WEIGHT_MEDIUM:
                return "Oblique";
                break;
            case XFONT_WEIGHT_DEMIBOLD:
            case XFONT_WEIGHT_BOLD:
            case XFONT_WEIGHT_BLACK:
                return "Bold Oblique";
                break;
        }
    }
    return "Regular";
}

void
xwindow_set_font_base(int type) {
    XWindow *xw;
    font_base = type;
    xw = plot_window(CURRENT);
    if (xw) {
        xw->font->family = strdup(font_bases[type]);
        xw->font->font_core = NULL;
#ifdef HAVE_XFT
        xw->font->font_xft = NULL;
#endif /* HAVE_XFT */
        xwindow_font_load(xw);
        xwindow_redraw(xw);
    }
}

void
xwindow_set_font_system(int type) {
    XWindow *xw;
    font_system = type;
    xw = plot_window(CURRENT);
    if (xw) {
        xw->font->type = font_system;
        xw->font->font_core = NULL;
#ifdef HAVE_XFT
        xw->font->font_xft = NULL;
#endif /* HAVE_XFT */
        xwindow_font_load(xw);
        xwindow_redraw(xw);
    }
}

int
xwindow_font_type(int current) {
    int nerr;
    char value[9];
    getbbv("FONT_TYPE", &value[0], &nerr, -1, 9);
    if (nerr == 0) {
        if (strncmp(value, "CORE", 4) == 0) {
            return XFONT_TYPE_CORE;
        }
        if (strncmp(value, "XFT", 4) == 0) {
            return XFONT_TYPE_XFT;
        }
        if (strncmp(value, "SOFTWARE", 4) == 0) {
            return XFONT_TYPE_SOFTWARE;
        }
    }
    return current;
}

void
xwindow_font_load(XWindow * xw) {
    XFont *f = xw->font;
    f->type = xwindow_font_type(f->type);
    switch (f->type) {
        case XFONT_TYPE_SOFTWARE:
            break;
        case XFONT_TYPE_CORE:
            {
                if (f->font_core) {
                    return;
                }
                char *s;
                asprintf(&s, "-*-%s-%s-%s-*-*-%d-*-*-*-*-*-*-*", f->family,
                         xfont_core_weight(f->weight),
                         xfont_core_slant(f->slant), (int) f->size);
                f->font_core = XLoadQueryFont(DISPLAY(xw), s);
                XSetFont(DISPLAY(xw), xw->gc, f->font_core->fid);
            }
            break;
        case XFONT_TYPE_XFT:
#ifdef HAVE_XFT
            if (f->font_xft) {
                return;
            }
            f->font_xft =
                XftFontOpen(DISPLAY(xw), SCREEN(xw), XFT_FAMILY, XftTypeString,
                            f->family, XFT_SIZE, XftTypeDouble, f->size,
                            XFT_STYLE, XftTypeString, xfont_style(f),
                            XFT_ANTIALIAS, XftTypeBool, f->antialias, NULL);
            if (!f->font_xft) {
                fprintf(stderr,
                        "Warning: Downgrading from XFT to X11/Core Font\n");
                f->type = XFONT_TYPE_CORE;
                xwindow_font_load(xw);
            }
#else
            fprintf(stderr, "SAC not compiled with XFT Support\n");
#endif /* HAVE_XFT */
            break;
        default:
            break;
    }
}
