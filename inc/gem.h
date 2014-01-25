/** 
 * @file   gem.h
 * 
 * @brief  Graphics Environment, structures used for 
 *              saving/restoring graphics environment 
 * 
 */

#ifndef _GEM_H_
#define _GEM_H_

#define SAC_LABEL_LOCATIONS  4
#define SAC_FONT_SIZES       4
#define SAC_KEY_SIZE_LENGTH  9

#define	MCMGEM	611
#define	MCPTXT	144
#define	MICOL	10
#define	MILINE	10
#define	MIPEN	10
#define	MISYM	20
#define	MKMGEM	323
#define	MPLAB	25
#define	MSISYM	8
#define	MSYM	16
#define	MSYMTB	40
#define	MTXSIZ	4
#define	MUNSYM	1
#define	MWIDTH	10
#define MAXPRNTRNAMELEN  128

#define GROUP_LENGTH 50

/* For Generic Plot Labels */
typedef struct _label label;
struct _label {
  int    plot;      /* Plot this label: TRUE/FALSE */
  int    relative;  /* Position below Label n-1: TRUE/FALSE; (FALSE use x,y) */
  float  x;         /* X Location of Label */
  float  y;         /* Y Location of Label */
  float  size;      /* Text size of Label */
  float  angle;     /* Text angle of Label */
  char  *text;      /* Text of label, was kplab */
};

/* For Title, XLabel and YLabel */
typedef struct _sac_label_t sac_label_t;
struct _sac_label_t {
  int   on, len, pos;
  float text_size;
  char  text[145]; /* Not currently used */
};

enum {
  TOP = 1,
  BOTTOM,
  RIGHT,
  LEFT,
};

enum {
  TEXT_TINY = 1,
  TEXT_SMALL,
  TEXT_MEDIUM,
  TEXT_LARGE
};

enum {
  AXIS_LINEAR = 0,
  AXIS_LOG,
};

enum {
  LINE_STYLE_SOLID = 1,
  LINE_STYLE_DOTTED = 8, /* Corresponds to 8th line in aux/linestyles.txt */
};

#define LINE_WIDTH_THIN  1


#define TEXT_VERTICAL    90.0
#define TEXT_HORIZONTAL   0.0

typedef struct _limits_t limits_t;
struct _limits_t {
  float xmin, xmax, ymin, ymax;
};

typedef struct _location_t location_t;
struct _location_t {
  int top, bottom, left, right;
};

typedef struct _datagen_t datagen_t;
struct _datagen_t {
  int on;
  float first, delta;
};


typedef struct _axis_t axis_t;
struct _axis_t {
  int ticks;
  int annotate;
  float width;
};

typedef struct _group_t group_t;
struct _group_t {
  int on;
  int i;
  int increment;
  int data[GROUP_LENGTH];
  int len;
  int j;
};


struct t_cmgem {
  int    lxlim; /* Limit X Scale Flag for Input Data */
  float  ximn;  /* X Scale Minimum for Input Data */
  float  ximx;  /* Y Scale Maximum for Input Data */

  int    lylim; /* Limit Y Scale Flag for Input Data */
  float  yimn;  /* Y Scale Minimum for Input Data */
  float  yimx;  /* Y Scale MAximum for Input Data */

  limits_t data;  /* Input Data Limits */
  limits_t zdata; /* Modified Input Data Limits */
  limits_t view;  /* Viewpace Limits */
  limits_t plot;  /* Plot Limits */
  limits_t uplot; /* Used Plot Limits */

  float  xmpip1; /* X Axis Plot Coordinate Scale Factor */
  float  xmpip2; /* X Axis Plot Coordinate Offset */
  float  ympip1; /* Y Axis Plot Coordinate Scale Factor */
  float  ympip2; /* Y Axis Plot Coordinate Offset */

  int    lxfudg; /* X Axis Scaling Flag */
  float  xfudg; /* X Axis Scaling Fraction */
  int    lyfudg; /* Y Axis Scaling Flag */
  float  yfudg; /* Y Axis Scaling Fraction */
  int    lxrev; /* X Axis Reverse Flag */
  int    lyrev; /* Y Axis Reverse Flag */
  int    ixint; /* X Axis mode = Linear or Log */
  int    iyint; /* Y Axis mode = Linear or Log */
  int    lxfull; /* X Axis Full Logarithmic decade decoding flag */
  int    lyfull; /* X Axis Full Logarithmic decade decoding flag */
  int    lloglb; /* Secondary Log Axis Flag */

  int    xdiv_spacing_on;  /* X Axis Division Spacing Flag */
  float  xdiv_spacing;   /* X Axis Division Spacing  */
  int    xdiv_number_on; /* X Axis Number of Division Flag */
  int    xdiv_number;  /* X AXis Number of Division */

  int    ydiv_spacing_on;  /* Y Axis Division Spacing Flag */
  float  ydiv_spacing;   /* Y Axis Division Spacing  */
  int    ydiv_number_on; /* Y Axis Number of Division Flag */
  int    ydiv_number;  /* Y AXis Number of Division */

  int    lxpowr; /* X Divisions displayed as powers */
  int    lypowr; /* Y Divisions displayed as powers */

  axis_t axis[5];

  datagen_t xgen;
  datagen_t ygen;

  float  tsaxis; /* Text size for Axis Annotations */
  int    lfloor; /* Minimum value on data flag */
  float  floor;  /* Minimum value on data with logarithm on and floor is on */
  int    lflusd; /* If floor was unsed in last data set flag */
  int    lrqclp; /* Viewport clipping flag */

  int    ltqdp;  /* Terminal Quick and Dirty Plotting Flag */
  int    ntqdp;  /* Number of Points for QDP */

  int    lnull; /* Skip null values on a plot flag */
  float  vnull; /* Default null value */

  int    lbdr;  /* Border drawn aroun viewport flag */

  int    lxgrd; /* Grid line parallel to X Axis flag */
  int    ixgrd; /* X Axis Grid line style */
  int    lygrd; /* Grid line parallel to Y Axis flag */
  int    iygrd; /* Y Axis Grid line style */

  sac_label_t title;
  sac_label_t xlabel;
  sac_label_t ylabel;

  int    nplab; /* Number of last plot modified   */
  int    lplab[MPLAB]; /* Label n is to be plotted flag */
  int    lplabl[MPLAB]; /* Label n is to be plotted below label n-1 flag */
  float  xplabl[MPLAB-(0)+1]; /* Label n X Position */
  float  yplabl[MPLAB-(0)+1]; /* Label n Y Position */
  float  tsplab[MPLAB-(0)+1]; /* Label n Text Size */
  float  taplab[MPLAB-(0)+1]; /* Label n Tex Position */

  // group_t line;
  int    lline; /* Line drawing flag */
  int    icline; /* Line style */
  int    liline; /* Line style incrementing flag */
  int    iiline[MILINE]; /* Line style array */
  int    niline; /* Length of line style array */
  int    jiline; /* Current position in line style array */

  int    isklin; /* Skeleton linestyle */
  
  // group_t symbol;
  int    lsym; /* Symbol drawing flag */
  int    isym; /* Current symbol number */ 
  int    lisym; /* Symbole incrementing flag */
  int    iisym[MISYM]; /* Symbol array */
  int    nisym; /* Length of symbol array */
  int    jisym; /* Current posiiton in symbol array */

  float  symsz; /* Symbol size */
  float  symsp; /* Symbol spacing */

  // group_t color;
  int    lcol; /* Color drawing flag */
  int    icol; /* Current color */
  int    licol; /* Color incrementing flag */
  int    iicol[MICOL]; /* Color array */
  int    nicol; /* Length of color array */
  int    jicol; /* Current position in color array */

  int    iskcol; /* Skeleton color */
  int    ibacol; /* Background color */

  // group_t width;
  int    lwidth; /* Line Width drawing flag */
  int    iwidth; /* Current line width */
  int    iswidth; /* Saved line width */
  int    isymwidth; /* Symbol Width */
  int    liwidth; /* Line Width incrementing flag */
  int    iiwidth[MWIDTH]; /* Line Width Array */
  int    iskwidth; /* Skeleton Width */
  int    isskwidth; /* Saved Skeleton line width */
  int    niwidth; /* Length of line width array */
  int    jiwidth; /* Current position in line width array */

  float  skdevfudge; /* Skeleton Axis Fudge Factor line width */
  float  chwid; /* Current character width */
  float  chht; /* Current character height */
  float  tscur; /* Current character size */
  float  txsiz[MTXSIZ]; /* Text Size Array */
  float  otxsiz[MTXSIZ]; /* Old Text Size Array */
  int    igtfnt; /* Current graphics text font number */
  float  tsdef; /* Default Text Size */
  float  txrat; /* Default Text Ratio */ 
  float  otxrat; /* Old Text Ratio */
  int    lframe; /* Automatic framing between data sets */
  float  dtxsiz[MTXSIZ]; /* New or default Text Sizes */
  float  dtxrat; /* Default Text Ratio */
  int    lfqdp; /* SGF quick and dirty plotting flag */
  int    nfqdp; /* Number of points for QDP/SGF */
  float  fac[9];
  int    lprint ;   /* TRUE if user wants to print a plot */
  int    lSGFtemp ; /* TRUE if SGF turned on exclusively for PRINT option */

  int    lfill; /* Turn on color filling of traces */
  int    ifill[2]; /* Current fill color */
  int    lifill; /* Color fill increment flag */
  int    iifillp[MILINE];  /* Filling Color Array */
  int    iifilln[MILINE];  /* Filling Color Array */
  int    nifill; /* Length of color fill array */
  int    jifill[2]; /* Current position in color fill array */
} cmgem;
struct t_kmgem {
  char   ksides[4][9];
  char   ktitl[145];
  char   kxlab[145];
  char   kylab[145];
  char   kplab[MPLAB][145];
  char   ktxsiz[MTXSIZ][9];
  char   kgtqua[9];
  char   khjust[9];
  char   kvjust[9];
  char   ktxpos[5][9];
  char   ktxori[2][9];
  char   kxtgem[20][9];
  char   kptrName[ MAXPRNTRNAMELEN+1 ] ;
  char   kfac[9];
} kmgem;


int            lgems;
struct t_cmgem cmgemsav;
struct t_kmgem kmgemsav;


void inisym ( int iisym[], 
              int *nisym);
void xaxes ( int *nerr);
void xbeginframe ( int *nerr);
void xbeginwindow ( int *nerr);
int color_on (void);
int color_foreground (void);
int color_background (void);
int color_skeleton   (void);
int color_foreground_default (void);
int color_background_default (void);
void color_switch ( int value);
void color_increment_set ( int value);
int color_background_set ( int color);
int color_background_set_by_name ( char *color);
int color_data_set ( int color);
int color_data_set_by_name ( char *color);
int color_skeleton_set ( int color);
int color_skeleton_set_by_name ( char *color);
int color_foreground_set ( int color);
int color_foreground_set_by_name ( char *color);
void xcolor ( int *nerr);
void xendframe ( int *nerr);
void xgemc ( int index, 
             int *nerr);
void xgrid ( int *nerr);
void xgt ( int *nerr);
void xlct ( int *nerr);
void xline ( int *nerr);
void xplab ( int *nerr);
void qdp_switch ( int flag);
void qdp_points ( int n);
void xqdp ( int *nerr);
void xsym ( int *nerr);
void xticks ( int *nerr);
void xtitle ( int *nerr);
void xtsize ( int *nerr);
void xwait ( int *nerr);
void xwidth ( int *nerr);
void xwindow ( int *nerr);
void xdiv_nice ( int flag);
void xdiv_increment ( float z);
void xdiv_number ( int n);
void xdiv_power ( int flag);
void xxdiv ( int *nerr);
void xxgrid ( int *nerr);
void xlabel_switch ( int flag);
void xlabel_label ( char *c);
void xlabel_location ( char *c);
void xlabel_size ( char *c);
void xxlab ( int *nerr);
void ydiv_nice ( int flag);
void ydiv_increment ( float z);
void ydiv_number ( int n);
void ydiv_power ( int flag);
void xydiv ( int *nerr);
void xygrid ( int *nerr);
void ylabel_switch ( int flag);
void ylabel_label ( char *c);
void ylabel_location ( char *c);
void ylabel_size ( char *c);
void xylab ( int *nerr);

void set_skeleton_fudge(float fudge);

label *label_store_get( int n );
int    label_store_length();

#endif /* _GEM_H_ */



