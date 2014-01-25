
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <windows.h> 
#include <Windowsx.h> //  The RGB macro is declared in "Windowsx.h"

#include "WinSacView.h"

#include "xyz.h"

extern SacColor pixdef[100000];

SacColor 
MakeColor(float r, float g, float b) {
    SacColor c;
    c.red   = r;
    c.green = g;
    c.blue  = b;
    return c;
}

SacPoint
MakePoint(float x, float y) {
    SacPoint p;
    p.x = x;
    p.y = y;
    return p;
}

SacColor 
CopyColor(SacColor c) {
    SacColor a;
    a.red   = c.red;
    a.green = c.green;
    a.blue  = c.blue;
    return a;
}

SacPoint 
CopyPoint(SacPoint a) {
    SacPoint b;
    b.x = a.x;
    b.y = a.y;
    return b;
}


SacImage *
ImageNew() {
    SacImage *im;
    im = (SacImage *) malloc(sizeof(SacImage));
    return im;
}

SacImage *
ImageInit(float *data, int width, int height,
          float xmin, float xmax, float ymin, float ymax,
          float x, float y, float w, float h,
          int nps, int nsa, int ndef,
          int binary) {
    SacImage *im = ImageNew();
    if(im) {
        im->data          = (float *) malloc(sizeof(float) * width * height);
        memcpy(im->data, data, sizeof(float) * width * height);
        im->width         = width;
        im->height        = height;
        im->xmin          = xmin;
        im->xmax          = xmax;
        im->ymin          = ymin;
        im->ymax          = ymax;
        im->x             = x;
        im->y             = y;
        im->w             = w;
        im->h             = h;
        im->npseudocolors = nps;
        im->nsacolors     = nsa;
        im->ndefcolors    = ndef;
        im->binary        = binary;
    }
    return im;
}

void
ImageFree(SacImage *im) {
    if(im) {
        if(im->data) {
            free(im->data);
            im->data = NULL;
        }
        free(im);
        im = NULL;
    }
}

float * 
ImageScale(SacImage *im, int w, int h) {
    int nerr;
    float *new;
    new = (float *) malloc(sizeof(float) * w * h);
    scaleimage(im->data, (unsigned int) im->width, (unsigned int) im->height,
               new, (unsigned int) w, (unsigned int ) h,
               im->xmin, im->xmax, im->ymin, im->ymax, &nerr);
    return new;
}

char * 
ImageColor(SacImage *im, float *new, int w, int h) {
    int i,j,k,kk;
    float zmin, zmax;
    float fmin, fmax, wrange, fsave;
    unsigned char *cdata;
    unsigned char r,g,b;
    frange(new, w*h, &zmin, &zmax);
    fmin = (float) im->nsacolors + 1.0 + (float) im->ndefcolors;
    fmax = (float) (im->nsacolors + im->npseudocolors + im->ndefcolors);
    wrange = (zmax - zmin);
    if(wrange == 0.0) {
        wrange = 1e-6;
    }
    
    cdata = (unsigned char *) malloc(sizeof(unsigned char)  * w * h * 4);
    k = 0;
    for(i = h-1; i >= 0; i--) {
        for(j = 0; j < w; j++) {
            kk = (i*w) + j;
            fsave = (new[kk] - zmin) / wrange;
            fsave = fsave * (float) im->npseudocolors;
            fsave = fsave + (float) im->nsacolors + 1.0 + (float) im->ndefcolors;
            fsave = (fsave < fmin) ? fmin : fsave;
            fsave = (fsave > fmax) ? fmax : fsave;
            r = (unsigned int) pixdef[(unsigned int)fsave].red;
            g = (unsigned int) pixdef[(unsigned int)fsave].green;
            b = (unsigned int) pixdef[(unsigned int)fsave].blue;
            cdata[k++] = b;
            cdata[k++] = g;
            cdata[k++] = r;
            cdata[k++] = 0xFF;
        }        
    }
    return (char *) cdata;
}

char * 
ImageToBounds(SacImage *im, SacRect bounds) {
    float *new;
    int ipw, iph, ipx, ipy;
    unsigned char *cdata;

    ipw = im->w * bounds.width;
    iph = im->h * bounds.height;
    ipx = im->x * bounds.width;
    ipy = im->y * bounds.height;

    new = ImageScale(im, ipw, iph);
    cdata = ImageColor(im, new, ipw, iph);
    return (char *) cdata;
}

SacViewObj *
SacViewObjectNew() {
    SacViewObj *s;
    s = (SacViewObj *) malloc(sizeof(SacViewObj));
    return s;
}

SacViewObj * 
SacViewObjectInitInternal() {
    SacViewObj *s = SacViewObjectNew();
    if(s) {
        s->type   = SV_Undef;
        s->point  = MakePoint(0,0);
        s->points = NULL;
        s->npts   = 0;
        s->color  = MakeColor(0,0,0);
        s->width  = 0;
        s->image  = NULL;
    }
    return s;
}

void
SacViewObjectFree(SacViewObj *s) {
    if(s) {
        if(s->points) {
            free(s->points);
            s->points = NULL;
        }
        if(s->image) {
            ImageFree(s->image);
            s->image = NULL;
        }
    }
    s = NULL;
}

SacViewObj *
SacViewObjectInitAP(SacViewType t, va_list ap) {
    SacViewObj *s = SacViewObjectInitInternal();
    if(s) {
        s->type = t;
        switch(t) {
        case SV_Move:
        case SV_Line:
            s->point = CopyPoint( va_arg(ap, SacPoint) );
            break;
        case SV_Stroke:
            break;
        case SV_Poly:
            s->npts   = va_arg(ap, int);
            s->points = va_arg(ap, SacPoint *);
            break;
        case SV_Color:
            s->color = CopyColor(va_arg(ap, SacColor));
            break;
        case SV_Width:
            s->width = va_arg(ap, int);
            break;
        case SV_Image:
            s->image = va_arg(ap, SacImage *);
            break;
        case SV_Begin:
        case SV_Flush:
            break;
        }
    }
    return s;
}

SacViewObj *
SacViewObjectInit(SacViewType t, ...) {
    SacViewObj *s;
    va_list ap;
    va_start(ap, t);
    s = SacViewObjectInitAP(t, ap);
    va_end(ap);
    return s;
}

SacView *
SacViewNew() {
    SacView *s;
    s = (SacView *) malloc(sizeof(SacView));
    return s;
}

SacView * 
SacViewInit() {
    SacView *s;
    s = SacViewNew();
    if(s) {
        s->n              = 0;
        s->alloc          = 0;
        s->objs           = NULL;
        s->original_ratio = 1.0;
        s->bounds.width   = 1;
        s->bounds.height  = 1;
        s->window_handle  = NULL;
        s->color          = MakeColor(0,0,0);
        s->width          = 1;
        s->id             = -1;
    }
    return s;
}

void
SacViewAddObject(SacView *view, SacViewObj *obj) {
    SacViewObj **tmp;
    if(!view) {
        fprintf(stderr, "view undefined\n");
        return;
    }
    if(!view->objs) {
        view->n     = 0;
        view->alloc = 4;
        view->objs  = (SacViewObj **) malloc(sizeof(SacViewObj *) * view->alloc);
    }
    if(view->n + 1 >= view->alloc) {
        view->alloc *= 2;
        tmp = (SacViewObj **) realloc(view->objs, sizeof(SacViewObj *) * view->alloc);
        if(!tmp) {
            fprintf(stderr, "Error allocating space for Drawing Object\n");
            return;
        }
        view->objs = tmp;
    }
    view->objs[view->n] = obj;
    view->n += 1;
}

void
SacViewClear(SacView *view) {
    int i;
    if(view->objs) {
        for(i = 0; i < view->n; i++) {
            SacViewObjectFree(view->objs[i]);
            view->objs[i] = NULL;
        }
        free(view->objs);
    }
    view->objs  = NULL;
    view->n     = 0;
    view->alloc = 0;
}

void
SacViewUpdate(SacView *view) {
    InvalidateRect(view->window_handle, NULL, TRUE);
}

void
SacViewAdd(SacView *view, SacViewType type, ...) {
    va_list ap;
    SacViewObj *obj;
    if(type == SV_Stroke) {
        return;
    }
    if(type == SV_Flush) {
        SacViewUpdate(view);
        return;
    }
    if(type == SV_Begin) {
        SacViewClear(view);
    }
    va_start(ap, type);
    obj = SacViewObjectInitAP(type, ap);
    va_end(ap);
    SacViewAddObject(view, obj);
}

SacPoint 
SacViewToBounds(SacView *view, SacPoint in, float ratio) {
    return MakePoint(in.x * view->bounds.width, 
                     (1.0 - in.y) * view->bounds.width / ratio);
}

float
SacViewMap(SacView *view, SacViewMapping map, SacPointComponent p, float z) {
    switch(map) {
    case SacViewFromScreen:
        switch(p) {
        case SacPointX: return z / (float)view->bounds.width; break;
        case SacPointY: return 1.0 - (z / (float)view->bounds.width); break;
        }
    case SacViewToScreen:
        switch(p) {
        case SacPointX: return z * (float)view->bounds.width; break;
        case SacPointY: return (1.0 - z) * (float)view->bounds.width; break;
        }
        break;
    }
    return 0.0;
}

void sac_draw_line(HDC hdc, 
                   float x1, float y1, float x2, float y2,
                   float red, float green, float blue, 
                   int width);
void sac_draw_image(HDC hdc, int x, int y, int w, int h, void *data);

void 
SacViewDraw(SacView *view) {
    int i, j;
    float ratio;
    SacPoint p;
    SacViewObj *obj;
    SacRect bounds;
    PAINTSTRUCT ps;
    LOGBRUSH brush;
    HDC hdc;
    HPEN pen;
    RECT rect;
    int x, y, w, h;

    float x1, y1;
    
	GetClientRect(view->window_handle,&rect);
    view->bounds.width  = rect.right;
    view->bounds.height = rect.bottom;
    ratio = (float)view->bounds.width / (float)view->bounds.height;
    ratio = ratio / view->original_ratio;
    
    hdc = BeginPaint(view->window_handle, &ps);

    for(i = 0; i < view->n; i++) {
        obj = view->objs[i];
        switch(obj->type) {
        case SV_Move:
            p = SacViewToBounds(view, obj->point, ratio);
            x1 = p.x;
            y1 = p.y;
            break;
        case SV_Line:
            p = SacViewToBounds(view, obj->point, ratio);
            sac_draw_line(hdc, x1,y1,p.x,p.y, 
                          view->color.red,
                          view->color.green,
                          view->color.blue,
                          view->width);
            x1 = p.x;
            y1 = p.y;
            break;
        case SV_Stroke:
            break;
        case SV_Poly: /* Polyline(hdc, POINT *points, int npts); */
            p = SacViewToBounds(view, obj->points[0], ratio);
            x1 = p.x;
            y1 = p.y;
            for(j = 1; j < obj->npts; j++) {
                p = SacViewToBounds(view, obj->points[j], ratio);
                sac_draw_line(hdc, x1,y1,p.x,p.y, 
                              view->color.red,
                              view->color.green,
                              view->color.blue,
                              view->width);
                x1 = p.x;
                y1 = p.y;            
            }
            break;
        case SV_Color: 
            view->color = CopyColor(obj->color);
            break;
        case SV_Width:
            view->width = obj->width;
            break;
        case SV_Image:
            x = obj->image->x * view->bounds.width;
            y = (1.0 - obj->image->y) * view->bounds.height;
            w = obj->image->w * view->bounds.width;
            h = obj->image->h * view->bounds.height;
            {
                unsigned char *data = ImageToBounds(obj->image, view->bounds);
                sac_draw_image(hdc, x, y, w, h, data);
                free(data);
            }
            break;
        case SV_Begin:
        case SV_Flush:
            break;
        }
    }
    EndPaint(view->window_handle, &ps);
}


SacViewWindows *
SacViewWindowsNew() {
    SacViewWindows *wins = (SacViewWindows *) malloc(sizeof(SacViewWindows));
    return wins;
}

SacViewWindows *
SacViewWindowsInit() {
    SacViewWindows *wins;
    wins = SacViewWindowsNew();
    if(wins) {
        wins->n           = 0;
        wins->alloc       = 0;
        wins->plotWindows = NULL;
    }
    return wins;
}

void
SacViewWindowsGrow(SacViewWindows *wins, int len) {
    SacView **tmp;
    int i, n;

    if(len < wins->alloc) {
        return;
    }

    n = 1;
    while(n <= len) {
        n *= 2;
    }
    wins->alloc = n;
    tmp = (SacView **) realloc(wins->plotWindows, sizeof(SacView *) * wins->alloc);
    if(!tmp) {
        fprintf(stderr, "Error allocating space for new plot window\n");
        return;
    }
    wins->plotWindows = tmp;
    for(i = wins->n; i < wins->alloc; i++) {
        wins->plotWindows[i] = NULL;
    }
}

int
SacViewWindowsFindByID(SacViewWindows *wins, int id) {
    int i;
    for(i = 0; i < wins->n; i++) {
        if(wins->plotWindows[i]->id == id) {
            return i;
        }
    }
    return -1;
}

int
SacViewWindowsFindByHandle(SacViewWindows *wins, HWND handle) {
    int i;
    for(i = 0; i < wins->n; i++) {
        if(wins->plotWindows[i]->window_handle == handle) {
            return i;
        }
    }
    return -1;
}

SacView * 
SacViewWindowsGetByHandle(SacViewWindows *wins, HWND handle) {
    int i;
    if((i = SacViewWindowsFindByHandle(wins, handle)) < 0) {
        return NULL;
    }
    fprintf(stderr, "Handle: %X ID: %d\n", handle, wins->plotWindows[i]->id);
    return wins->plotWindows[i];
}

SacView * 
SacViewWindowsGetByID(SacViewWindows *wins, int id) {
    int i;
    if((i = SacViewWindowsFindByID(wins, id)) < 0) {
        return NULL;
    }
    return wins->plotWindows[i];
}

void
SacViewWindowsRemove(SacViewWindows *wins, int i) {
    memmove(wins->plotWindows[i], wins->plotWindows[i+1], sizeof(SacView *) * (wins->n-i-1));
    wins->plotWindows[wins->n] = NULL;
    wins->n--;    
}

void
SacViewWindowsRemoveByID(SacViewWindows *wins, int id) {
    int i;
    if((i = SacViewWindowsFindByID(wins, id)) < 0) {
        return;
    }
    SacViewWindowsRemove(wins, i);
}

void
SacViewWindowsAdd(SacViewWindows *wins, SacView *view) {
    if(wins->n + 1 > wins->alloc) {
        SacViewWindowsGrow(wins, wins->n+1);
    }
    wins->plotWindows[wins->n] = view;
    wins->n++;
}

