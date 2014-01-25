/** 
 * @file   gd2.h
 * 
 * @brief  Graphics Device (2) SGF
 * 
 */

#ifndef _GD2_H_
#define _GD2_H_

#include "gdm.h"
#include "mach.h"

#define	JFBMAX    5000
#define	JFBSIZ    (JFBMAX + 100)
#define	MOPCOL    (-4)
#define	MOPDON    (-2)
#define	MOPFIL    (-10)
#define	MOPFRT    (-11)
#define	MOPHWS    (-6)
#define	MOPHWT    (-5)
#define	MOPLIN	  (-7)
#define	MOPMOV	  (-3)
#define	MOPNUL    (-1)
#define	MOPSIZ	  (-8)
#define	MOPTAN	  (-12)
#define	MOPWIDTH  (-9)
#define MOPCIMAGE (-13)
#define MOPFILLRGB (-14)
#define	XW	  32000.
#define	YW	  24000.


/** 
 * @struct kmgd2 
 *    SGF Graphic Stucture Characters
 *
 */
struct t_kmgd2 {
  char  kname2[9];               /**  */
  char  kfnamb[MCPFN+1];         /**  */
  char  kfdir[MCPFN+1];          /**  */
  char  sizetype[9];             /**  */
  char  kfdirStore[MCPFN+1];     /** store value of kfdir for use later */
  char  kfilename[ MCPFN+1 ] ;   /** name of last SGF file written. */
} kmgd2;


/** 
 * @struct cmgd2 
 *    SGF Graphic Stucture Characters
 *
 */
struct t_cmgd2 {
  int   itype2;            /** */
  int   nfbuf;             /** */
  short int mfbuf[JFBSIZ]; /** */
  int   jfbpnt;            /** */
  int   jfdpnt;            /** */
  int   jfun;              /** */
  int   nfnamb;            /** */
  int   nfdir;             /** */
  int   nfdirStore;        /** */
  int   lfnum;             /** */
  int   nfnum;             /** */
  float sizevalue;         /** */
  int   encodesize;        /** */
  int   lover;             /** TRUE overwrites sgf files. */
} cmgd2;




#ifdef DOINITS

   short *const Mfbuf = &cmgd2.mfbuf[0] - 1;

#else

   extern short *const Mfbuf;

#endif

void beginSGFtemp ( int *nerr);
void begindevice2 ( int *nerr);
void beginframe2 ( int *nerr);
void beginwindow2 ( int *number, 
                    int *nerr);
void calc_loc2 ( unsigned int *xloc, 
                 unsigned int *yloc, 
                 unsigned int *cbarxoffset, 
                 unsigned int *cbaryoffset, 
                 unsigned int w_width, 
                 unsigned int w_height, 
                 unsigned int i_width, 
                 unsigned int i_height, 
                 double xpmn, 
                 double xpmx, 
                 double xmin, 
                 double first, 
                 double last, 
                 double ypmn, 
                 double ypdel, 
                 double vsratio, 
                 int *nerr);
void cbar_window2 ( unsigned int xloc, 
                    unsigned int yloc, 
                    unsigned int height, 
                    unsigned int w_height, 
                    unsigned int w_width, 
                    double vspaceratio, 
                    double ypmax, 
                    int *nerr);
void changectable2 ( int nentry, 
                    int icolortable);
void createwindow2 ( int *number, 
                     float *xwinmn, 
                     float *xwinmx, 
                     float *ywinmn, 
                     float *ywinmx, 
                     int *nerr);
void cursor2 ( float *xloc, 
               float *yloc, 
               char * kchar,
               int len);
void cursortext2 ( float *xloc, 
                   float *yloc, 
                   char *ktext,
                   int len);
void draw2 ( float xloc, 
             float yloc);
void drawpoly2(float *x, float *y, int n);
void fillpoly2(float *x, float *y, int n);
void endSGFtemp ( int *nerr);
void enddevice2 ( int *nerr);
void endframe2 ( int *nerr);
void erase2 (void);
char *fill_clrbar2 ( int npseudocolors, 
                     int width, 
                     int npricolors, 
                     int ndefcolors, 
                     int *nerr);
char *fill_image2 ( unsigned int height, 
                    unsigned int width, 
                    float data[], 
                    float dmin, 
                    float range, 
                    int npseudocolors, 
                    int nsaccolors, 
                    int ndefcolors, 
                    int *nerr);
void flushbuffer2 ( int *nerr);
void get_geometry2 ( int number,
                     unsigned int *width_return, 
                     unsigned int *height_return, 
                     int *nerr);
void getalphainfo2 ( int *nlines, 
                     char *erase,
                     int len);
void getdeviceinfo2 ( char *kdname, 
                      int kdname_s, 
                      int *idtype);
void getdevicerat2 ( float *ratio);
void getratio2 ( float *aspect);
void getwindowstat2 ( int number, 
                      int *exists);
void hardwaretext2 ( char *ktext, 
                     int ntext);
void initdevice2 (void);
void move2 ( float xloc, 
             float yloc);
void put_image2 ( char *data, 
                  unsigned int xloc, 
                  unsigned int yloc, 
                  unsigned int width, 
                  unsigned int height, 
                  int *nerr);
void setcolor2 ( int index);
void setctable2 ( int iwindow, 
                  unsigned int nentry, 
                  float red[], 
                  float green[], 
                  float blue[]);
void setlinestyle2 ( int *iline);
void setsgfdir ( char *dir, 
                 int dir_s);
void setsgfnumber ( int number);
void setsgfprefix ( char *prefix);
void setsgfsize ( char *type, 
                  double value);
void settextangle2 (float angle);
void settextsize2 ( float width, 
                    float height);
void setwidth2 ( int index);
void calculate_location2(float *x,
                         float *y,
                         float *cx,
                         float *cy,
                         unsigned int w,
                         unsigned int h,
                         unsigned int iw,
                         unsigned int ih) ;
void show_image_sgf(float *data,     
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

void text_box_sgf(textbox *t);

void init_print_device_SGF(print_device_begin_t *begin,
                           print_device_end_t   *end) ;
void adj_geometry2(unsigned int *w, unsigned int *h);

#endif /* _GD2_H_ */
