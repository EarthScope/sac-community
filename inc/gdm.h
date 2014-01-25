/** 
 * @file   gdm.h
 * 
 * @brief  Graphics Display Manager
 * 
 */

#ifndef _GDM_H_
#define _GDM_H_

#include "gd4.null.h"

#define	MCTSIZE	  256
#define	MGD	   50
#define	MWINDOWS   10  /* Must Match MAX_WINS in gd3.aux.h */
#define NPSCIMAGE 237

#define TEXTBOX_LINE_LENGTH_PIXELS 55.0
#define SGF_PIXELS(x) ((float)x * 0.0225) 

enum {
  SGF  = 2,
  X11  = 3,
  GUI  = 5,
  PS   = 6,
  PDF  = 7,
  REC  = 8,
  TEXT = 9,
  XPM  = 10,
  PNG  = 11,
};

/** 
 * @struct kmgdm 
 *    Graphics Display Manager Characters
 */
struct t_kmgdm {
  char  kgdnam[MGD][13];
  char  ctname[MCTSIZE - 1-(0)+1][9];
  char  kgtfn[4][41];
} kmgdm;

/** 
 * @struct kmgdm 
 *    Graphics Display Manager Variables
 */
struct t_cmgdm {
  int    lgdon[MGD];
  int    igdtyp[MGD];
  int    lactiv;
  int    lpasiv;
  int    lcur;
  int    igdcur;
  int    igdhc, iflhc[MGD], igdtxt;
  int    lginit;
  int    lbegf;
  int    iwindow;
  float  xwindowmin[MWINDOWS];
  float  xwindowmax[MWINDOWS];
  float  ywindowmin[MWINDOWS];
  float  ywindowmax[MWINDOWS];
  int    lvsful;
  float  vsrat;
  float  xvs[2];
  float  yvs[2];
  int    lvsclip;
  int    ltsoft;
  float  thgt;
  float  twidth;
  float  tangle;
  int    ihjust;
  int    ivjust;
  int    itcol;
  int    lfhard;
  int    icolor;
  int    nctsize;
  int    npscimage;
  float  ctred[MCTSIZE - 1-(0)+1];
  float  ctgreen[MCTSIZE - 1-(0)+1];
  float  ctblue[MCTSIZE - 1-(0)+1];
  int    iline;
  float  xold;
  float  yold;
  int    iold;
  int    nfont;
  int    iofset;
  int    ispace;
  int    iyoff;
  int    iyhigh;
  int    maxstr;
  float  fnthit;
  float  fntwid;
  float  fntrot;
  short int ascstr[128];
  short int stxmin[128];
  short int stxmax[128];
  short int stroke[3252];
  int    lgui;
} cmgdm;


typedef struct _textbox textbox;
struct _textbox {
  int    n;        /* Number of Strings */
  char **text;     /* Text Strings */
  int   *color;    /* Color of strings and lines */
  int   *symbol;   /* Symbol Type */
  int   *style;    /* Line style  */
  int   *width;    /* Line width */
  int    use_symbol;
  int    use_style;
  float  x;        /* Text Box X Location */
  float  y;        /* Text Box Y Location */
  int    location; /* Text Box Location Type */
};


#define TEXT_BOX_UPPER  ( 1 << 0 ) /* 0x01 */
#define TEXT_BOX_LOWER  ( 1 << 1 ) /* 0x02 */
#define TEXT_BOX_RIGHT  ( 1 << 2 ) /* 0x04 */
#define TEXT_BOX_LEFT   ( 1 << 3 ) /* 0x08 */

typedef void (*print_device_begin_t)     ( int *nerr );
typedef void (*print_device_end_t)       ( int *nerr );

typedef struct _display_t display_t;

typedef void (*event_t)                  (int *nerr);
typedef void (*event_i_t)                (int *number, 
                                          int *nerr);
typedef void (*ratio_t)                  (float *ratio);
typedef void (*create_t)                 (int *number, 
                                          float *xmin, 
                                          float *xmax, 
                                          float *ymin, 
                                          float *ymax, 
                                          int *nerr);
typedef void (*move_t)                   (float x, 
                                          float y);
typedef void (*cursortext_t)             (float *x, 
                                          float *y, 
                                          char *ktext, 
                                          int len);
typedef void (*change_t)                 (int nentry, int ctable);
typedef void (*linestyle_t)              (int *linestyle);
typedef char* (*fill_t)                  (unsigned int height, 
                                          unsigned int width, 
                                          float data[], 
                                          float dmin,
                                          float range, 
                                          int nspeudocolors,
                                          int nsaccolors,
                                          int ndefcolors,
                                          int *nerr);
typedef void (*put_t)                    (char *data, 
                                          unsigned int xloc, 
                                          unsigned int yloc,
                                          unsigned int width,
                                          unsigned int height,
                                          int *nerr);
typedef void (*text_t)                   (display_t *out,
                                          char *text, 
                                          int len);
typedef void (*cursor_t)                 (float *x, 
                                          float *y, 
                                          char *k, 
                                          int len);
typedef void (*draw_t)                   (float x, 
                                          float y);
typedef void (*drawpoly_t)               (float *x, 
                                          float *y, 
                                          int n);
typedef void (*erase_t)                  ();
typedef void (*get_geometry_t)           (int number, 
                                          unsigned int *width, 
                                          unsigned int *height,
                                          int *nerr);
typedef void (*linewidth_t)              (int width);
typedef char * (*fill_colorbar_t)        (int npseudocolors, 
                                          int width, 
                                          int npricolors, 
                                          int ndefcolors, 
                                          int *nerr);
typedef void (*get_alpha_info_t)         (int *num_lines, char erase[], int len);
typedef void (*set_color_t)              (int index);
typedef void (*set_color_table_t)        (int win_num,
                                          unsigned int nentry,
                                          float red[],
                                          float green[],
                                          float blue[]);
typedef void (*set_pseudo_color_table_t) (int *num, 
                                          unsigned int nentry,
                                          float red[],
                                          float green[],
                                          float blue[]);
typedef void (*set_text_size_t)          (float width, 
                                          float height);
typedef void (*get_window_status_t)      (int number, 
                                          int *exists);
typedef void (*set_text_angle_t)         (float angle);
typedef void (*adjust_geometry_t)        (unsigned int *width, 
                                          unsigned int *height);
typedef void (*stroke_t)                 ();
typedef void (*calc_loc_t)               (float *x,
                                          float *y,
                                          float *cx,
                                          float *cy,
                                          unsigned int w,
                                          unsigned int h,
                                          unsigned int iw,
                                          unsigned int ih);
typedef void (*show_image_t)             (float *data,     
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
                                          int *nerr);
typedef void (*textbox_t)                (textbox *tbox);
typedef void (*set_window_size_t)       ( int size );
typedef void (*save_t)                  ( display_t *out, 
                                          char *file );
typedef int  (*get_file_descriptor_t)   ( void );
typedef void (*handle_event_t)          ( int *nerr );
typedef void (*get_window_size_t)       ( float *xmin, 
                                          float *xmax,
                                          float *ymin,
                                          float *ymax );
typedef void (*fillpoly_t)              ( float *x,
                                          float *y,
                                          int    n );
                                          
struct _display_t {
  char                    *name;
  char                    *extension;
  int                      on;
  int                      id;
  int                      cursor_flag; /* WAS_ON, NOTHING, TURNED_ON */

  int                      active_device; /* TRUE - active, FALSE - passive */
  int                      cursor_enabled; /* TRUE can react to mouse events*/
  int                      mirror_device;

  adjust_geometry_t        adjust_geometry;

  event_t                  begin_device;
  event_t                  begin_frame;
  event_i_t                begin_window;

  change_t                 change_color_table;
  create_t                 create_window;
  cursor_t                 cursor;
  cursortext_t             cursor_text;
  calc_loc_t               calc_loc;

  draw_t                   draw;
  drawpoly_t               drawpoly;
  fillpoly_t               fillpoly;

  event_t                  end_device;
  event_t                  end_frame;
  erase_t                  erase;

  event_t                  flush_buffer;
  fill_t                   fill_image;
  fill_colorbar_t          fill_colorbar;

  ratio_t                  get_device_ratio;
  ratio_t                  get_ratio;
  get_geometry_t           get_geometry;
  get_alpha_info_t         get_alpha_info;
  get_window_status_t      get_window_status;

  move_t                   move;

  put_t                    put_image;

  set_color_t              set_color;
  set_color_table_t        set_color_table;
  linestyle_t              set_line_style;
  linewidth_t              set_line_width;
  set_pseudo_color_table_t set_pseudo_color_table;
  set_text_angle_t         set_text_angle;
  set_text_size_t          set_text_size;
  stroke_t                 stroke;
  show_image_t             show_image;

  text_t                   text;
  textbox_t                textbox;

  set_window_size_t       set_window_width;
  set_window_size_t       set_window_height;

  get_window_size_t       get_window_size;

  save_t                  save;
  
  get_file_descriptor_t   get_file_descriptor;
  handle_event_t          handle_event;
};

#ifdef DOINITS

   short *const Ascstr = &cmgdm.ascstr[0] - 1;
   int   *const Iflhc = &cmgdm.iflhc[0] - 1;
   int   *const Igdtyp = &cmgdm.igdtyp[0] - 1;
   int   *const Lgdon = &cmgdm.lgdon[0] - 1;
   short *const Stroke = &cmgdm.stroke[0] - 1;
   short *const Stxmax = &cmgdm.stxmax[0] - 1;
   short *const Stxmin = &cmgdm.stxmin[0] - 1;
   float *const Xvs = &cmgdm.xvs[0] - 1;
   float *const Xwindowmax = &cmgdm.xwindowmax[0] - 1;
   float *const Xwindowmin = &cmgdm.xwindowmin[0] - 1;
   float *const Yvs = &cmgdm.yvs[0] - 1;
   float *const Ywindowmax = &cmgdm.ywindowmax[0] - 1;
   float *const Ywindowmin = &cmgdm.ywindowmin[0] - 1;

#else

   extern short *const Ascstr;
   extern int   *const Iflhc;
   extern int   *const Igdtyp;
   extern int   *const Lgdon;
   extern short *const Stroke;
   extern short *const Stxmax;
   extern short *const Stxmin;
   extern float *const Xvs;
   extern float *const Xwindowmax;
   extern float *const Xwindowmin;
   extern float *const Yvs;
   extern float *const Ywindowmax;
   extern float *const Ywindowmin;

#endif

void initdevice_null( display_t *d );

void adj_geometry ( unsigned int *width, 
                    unsigned int *height, 
                    int *nerr);
void begindevice ( char *device, 
                   int device_s, 
                   int *nerr);
void begindevices ( char *devices, 
                    int devices_s, 
                    int ndevices, 
                    int *nerr);
void beginframe ( int lprint, 
                  int *nerr);
void beginwindow ( int number, 
                   int *nerr);
void calstatus (void);
void calvspace (void);
void changectable ( int nentry, 
                    int ctable);
int  convcolorname ( char *name, 
                     int *number);
void convcolornum ( int number, 
                    char *name, 
                    int name_s);
void createwindow ( int *number, 
                    double xwinmn, 
                    double xwinmx, 
                    double ywinmn, 
                    double ywinmx, 
                    int *nerr);
void cursor0 ( float *xloc, 
               float *yloc, 
               char *kchar);
void cursoroff (void);
void cursoron (void);
void cursortext ( float *xloc, 
                  float *yloc, 
                  char *ktext, 
                  int ktext_s);
void draw (float xloc, 
           float yloc);
void enddevice ( char *device, 
                 int device_s, 
                 int *nerr);
void endframe ( int ldelay, 
                int *nerr);
void endgraphics ( int *nerr);
void erase ( int *nerr);
char **fill_colorbar ( int npseudocolors, 
                       int width, 
                       int npricolors, 
                       int ndefcolors, 
                       int *nerr);
char **fill_image ( unsigned int height, 
                    unsigned int width, 
                    float data[], 
                    float dmin, 
                    float range, 
                    int npseudocolors, 
                    int nsaccolors, 
                    int ndefcolors, 
                    int *nerr);
void flushbuffer ( int *nerr);
void get_geometry ( unsigned int *width_return, 
                    unsigned int *height_return, 
                    int *nerr);
void getalphainfo ( int *nlines, 
                    char *erase, 
                    int erase_s);
void getdeviceinfo1 ( char *kdname, 
                      int kdname_s, 
                      int *idtype);
void getdevicename ( int number, 
                     char *name, 
                     int name_s);
void getdeviceratio ( float *ratio);
void getlinestyle ( int *istyle);
void getmaxdevices ( int *number);
void get_position(float *x, float *y);
void getratio ( float *ratio);
void getstatus ( char *kstat, 
                 int *ltrue);
void getstringsize ( char *ktext, 
                     int ntext, 
                     float *width);
float gettextangle();
void gettextjust ( char *khorz, 
                   int khorz_s, 
                   char *kvert,
                   int kvert_s);
void gettextsize ( float *width, 
                   float *height);
void getvspace ( float *xvsmin, 
                 float *xvsmax, 
                 float *yvsmin, 
                 float *yvsmax);
void getvspacetype ( int *lfull, 
                     float *ratio);
void getwindowstatus ( int *nwindow, 
                       int *exists);
void inigdm ( int *nerr);
void initctable ( char *name, 
                  int name_s, 
                  int *nentry, 
                  int *nerr);
void loadctable ( char *name, 
                  char *directory, 
                  int *nentry, 
                  int *nerr);
void move ( float xloc, 
            float yloc);
void put_image ( char **data, 
                 unsigned int xloc, 
                 unsigned int yloc, 
                 unsigned int width, 
                 unsigned int height, 
                 int *nerr);
void readctable ( char *name, 
                  int name_s, 
                  int max_, 
                  float red[], 
                  float green[], 
                  float blue[], 
                  char *cnames, 
                  int cnames_s, 
                  int *nentry, 
                  int *nerr);
void setcolor ( int number);
void setcolorname ( char *color, 
                    int color_s);
void setlinestyle ( int istyle);
void setlinewidth ( int nwidth);
void setpsctable ( int *nerr);
void settextangle ( float angle);
void settextfont ( int ifont);
void settextjust ( char *khorz, 
                   char *kvert);
void settextsize ( float width, 
                   float height);
void settexttype ( char *kqual);
void setvspaceclip ( int lclip);
void setvspacetype ( int lfull, 
                     double ratio);
void softwaretext ( display_t *out,
                    char *ktext, 
                    int ntext);
void set_position(float x, float y);
void stroke ( );
void text ( char *ktext, 
            int ktext_s, 
            int nctext);

void gdm_init(void) ;
void gdm_register_device(display_t *dev);
int  gdm_get_ndevices(void);
display_t ** gdm_get_devices(void);
display_t * gdm_get_device_by_name(char *name);
display_t * gdm_get_device_by_id(int id);
int gdm_get_device_number_by_name(char *name);

void settextsize_internal(float width, float height);
void settextangle_internal(float angle);

void show_image(float *data,     /* Image Data */
                unsigned int iw, /* Image Size */
                unsigned int ih,
                float xmin,
                float xmax,
                float ymin,
                float ymax,
                float x,         /* In View space Coordinates [0,1] */
                float y,
                float w,
                float h,
                int nspeudocolors,
                int nsacolors,
                int ndefcolors,
                int lbinary,
                int *nerr);

void      textbox_show(textbox *tbox);
textbox * textbox_new(int n);
textbox * textbox_copy(textbox *old);
void      textbox_free(textbox *t);

void text_box_line(display_t *out, 
                   float      x,
                   float      y,
                   int        width, 
                   int        style, 
                   float      text_width, 
                   float      text_height);
void
text_box_symbol(display_t *out,
                float x,
                float y,
                int symbol,
                int with_line,
                float text_width,
                float text_height);

void set_window_width    ( int width );
void set_window_height   ( int height );
int  get_file_descriptor ( void );
void handle_event        ( int *nerr );

void xsave();
void initdevice_postscript();
void initdevice_pdf();
void initdevice_record();
void initdevice_text();
void initdevice_xpm();
void initdevice_png();

void record_write_file(display_t *out, char *file);
void record_play_current(display_t *out);

char *record_filename( char *in );

typedef void (*line_style_function) (char *line, void *data);
void sac_line_style_read(line_style_function func, void *data);

#endif /* _GDM_H_ */

