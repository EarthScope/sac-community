
#ifndef _WIN_SAC_VIEW_H_
#define _WIN_SAC_VIEW_H_

#include <windows.h>

#define WIN_COLOR_MAXIMUM 255

struct _SacColor {
    float red, green, blue;
};
typedef struct _SacColor SacColor;

struct _SacPoint {
    float x, y;
};
typedef struct _SacPoint SacPoint;

enum _SacPointComponent {
    SacPointX = 1,
    SacPointY = 2
};
typedef enum _SacPointComponent SacPointComponent;

struct _SacRect {
    int x;
    int y;
    int width;
    int height;
};
typedef struct _SacRect SacRect;

struct _SacImage {
    float *data; /* z-value */
    int    width, height; /* size in pixels*/
    float  xmin, xmax, ymin, ymax; /* Limits x and y */
    float  x, y, w, h; /* Position and size of image */
    int    npseudocolors, nsacolors, ndefcolors; 
    int    binary; /* Color or Black & White */
};
typedef struct _SacImage SacImage;

enum _SacViewMapping {
    SacViewToScreen   = 1,
    SacViewFromScreen = 2,
};
typedef enum   _SacViewMapping SacViewMapping;

enum _SacViewType {
    SV_Undef  = 0,
    SV_Move   = 1,
    SV_Line   = 2,
    SV_Stroke = 3,
    SV_Poly   = 4,
    SV_Color  = 5,
    SV_Width  = 6,
    SV_Image  = 7,
    SV_Begin  = 8,
    SV_Flush  = 9,
};
typedef enum   _SacViewType SacViewType;

struct _SacViewObj {
    SacViewType type;
    SacPoint    point;
    SacPoint   *points;
    int         npts;
    SacColor    color;
    int         width;
    SacImage   *image;
};
typedef struct _SacViewObj SacViewObj;

struct _SacView {
    int          n;
    int          alloc;
    SacViewObj **objs;
    float        original_ratio;
    SacRect      bounds;
    SacColor     color;
    int          width;
    int          id;
    HWND         window_handle;
};
typedef struct _SacView SacView;

struct _SacViewWindows {
    int       n;
    int       alloc;
    SacView **plotWindows;
};
typedef struct _SacViewWindows SacViewWindows;

SacPoint MakePoint(float x, float y);
SacPoint CopyPoint(SacPoint p);
SacColor MakeColor(float r, float g, float b);
float    SacViewMap(SacView *view, SacViewMapping map, SacPointComponent p, float z);

SacImage *
ImageInit(float *data, int width, int height,
          float xmin, float xmax, float ymin, float ymax,
          float x, float y, float w, float h,
          int nps, int nsa, int ndef,
          int binary);

int       SacViewWindowsFindByID(SacViewWindows *wins, int id);
SacView * SacViewWindowsGetByID(SacViewWindows *wins, int id);

#endif /* _WIN_SAC_VIEW_H_ */
