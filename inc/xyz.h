/** 
 * @file   xyz.h
 * 
 * @brief  XYZ Plotting
 * 
 */


#ifndef _XYZ_H_
#define _XYZ_H_

#include "mach.h"

#define	MZLLIST	40
#define	MZREGIONS	(2*MZLLIST + 1)

/** 
 * @struct cmxyz
 *     XYZ Plotting Variables
 */
struct t_cmxyz {
  int    lcleanup;            /** */
  float  zllist[MZLLIST];     /** */
  int    nzllist;             /** */
  int    lzllist;             /** */
  int    lzlmin;              /** */
  float  zlmin;               /** */
  int    lzlmax;              /** */
  float  zlmax;               /** */
  int    lzlinc;              /** */
  float  zlinc;               /** */
  int    lzlines;             /** */
  int    izlines[MZLLIST];    /** */
  int    nzlines;             /** */
  float  zregions[MZREGIONS]; /** */
  int    nzregions;           /** */
  int    laspect;             /** */
} cmxyz;


#ifdef DOINITS

   int *const Izlines = &cmxyz.izlines[0] - 1;
   float *const Zllist = &cmxyz.zllist[0] - 1;
   float *const Zregions = &cmxyz.zregions[0] - 1;

#else

   extern int *const Izlines;
   extern float *const Zllist;
   extern float *const Zregions;

#endif

void adjust_height ( float *input_image, 
                     unsigned int width, 
                     unsigned int height, 
                     float *output_image, 
                     unsigned int height_out, 
                     float ymin, 
                     float ymax, 
                     int *nerr);
void adjust_width ( float *input_image, 
                    unsigned int width, 
                    unsigned int height, 
                    float *output_image, 
                    unsigned int width_out, 
                    float xmin, 
                    float xmax, 
                    int *nerr);
int calcfftsize ( double delta, 
                  double sliceint, 
                  double window, 
                  int *iorfft, 
                  int *lfft, 
                  int *nptswndw, 
                  int *buffersize, 
                  int *windowovrl);
void calcloc ( unsigned int *xloc, 
               unsigned int *yloc, 
               unsigned int *cbarxoffset, 
               unsigned int *cbaryoffset, 
               unsigned int w_width, 
               unsigned int w_height, 
               unsigned int i_width, 
               unsigned int i_height, 
               float xpmn, 
               float xpmx, 
               float xmin, 
               float first, 
               float last, 
               float ypmn, 
               float  ypdel, 
               float vsratio, 
               int *nerr);
void calcsize ( unsigned int *w_width, 
                unsigned int *w_height, 
                unsigned int *width, 
                unsigned int *height, 
                float xmax, 
                float xmin, 
                float last, 
                float first, 
                float xpmn, 
                float xpmx, 
                float ypmn, 
                float ypmx, 
                float yfactor, 
                int *nerr);
void cbar_window ( unsigned int xloc, 
                   unsigned int yloc, 
                   unsigned int height, 
                   unsigned int w_height, 
                   unsigned int w_width, 
                   float vspaceratio, 
                   float ypmax, 
                   int *nerr);
void flipdata ( float *olddata, 
                int lengthx, 
                int lengthy, 
                float *newdata);
int getdata ( int nfiles, 
              double delta, 
              int windwsize, 
              int windwovrl, 
              char *windwfunc, 
              int buffersize, 
              int filelength[], 
              int bufindex, 
              float signals[]);
void inixyz (void);
void label_cbar ( float xloc, 
                  float yloc, 
                  float width, 
                  float height, 
                  float dmin, 
                  float dmax, 
                  int *nerr);
void linear_interp ( float *array, 
                     float xmin, 
                     float xmax, 
                     int nx, 
                     float *array_out, 
                     int newnx, 
                     int *nerr);
void plotimage ( float array[], 
                 int nxsize, 
                 float xmin, 
                 float xmax, 
                 int nysize, 
                 float ymin, 
                 float ymax, 
                 int jxstart, 
                 int jxstop, 
                 int jystart, 
                 int jystop, 
                 unsigned int image_width, 
                 unsigned int image_height, 
                 unsigned int w_width, 
                 unsigned int w_height, 
                 unsigned int xstartloc, 
                 unsigned int ystartloc, 
                 unsigned int cbarxoffset, 
                 unsigned int cbaryoffset, 
                 float ypmax, 
                 int lbinary, 
                 int lcbar, 
                 int *nerr);
void scaleimage ( float *input_image, 
                  unsigned int width, 
                  unsigned int height, 
                  float *output_image, 
                  unsigned int width_out, 
                  unsigned int height_out, 
                  float xstart, 
                  float xstop, 
                  float ystart, 
                  float ystop, 
                  int *nerr);
void scallop ( float *sdata, 
               int specwidth, 
               int speclength, 
               float freqmax, 
               float fmin, 
               float fmax, 
               int lmean, 
               float *scdata, 
               int *nerr);
void smooth ( float *input, 
              int npoints, 
              int lmean, 
              int *index, 
              int nhalf, 
              int nfull, 
              float factor, 
              float *output, 
              int *nerr);
void spcgrm ( float signal[], 
              int lfft, 
              int iorfft, 
              char *type, 
              int *order, 
              double delta, 
              int wlen, 
              int sfft, 
              int lncorwin, 
              int ncorwin, 
              float lencorwin, 
              char *wintype, 
              char *scale, 
              float ridge_fac, 
              int *err);
void specplot ( float *specdata, 
                int nx, 
                int ny, 
                float xmin, 
                float xmax, 
                float ymin, 
                float ymax, 
                float ywmin, 
                float ywmax, 
                char *imagetype, 
                int lbinary, 
                int lcbar, 
                int lprint, 
                int *nerr);
int spectrogram ( double window, 
                  double sliceint, 
                  char *type, 
                  int *order, 
                  int nfiles, 
                  int filelength[], 
                  double delta, 
                  int *specindex, 
                  int *specwidth, 
                  int *speclength, 
                  int sfft, 
                  float cwinlength, 
                  int lcnumber, 
                  int cnumber, 
                  char *cwintype, 
                  char *scale);
void subtract ( float *input1, 
                float *input2, 
                float *output, 
                int npoints, 
                int *nerr);
void vfxyz ( int *nerr);
void window_data ( float *array, 
                   int nxsize, 
                   int nysize, 
                   float *wdata, 
                   int jxstart, 
                   int jxstop, 
                   int jystart, 
                   int jystop);
void writezdata ( char *filename, 
                  int filename_s, 
                  float zdata[], 
                  int *nzsize, 
                  int *nerr);
void xcontour ( int *nerr);
void xgrayscale ( int *nerr);
void ximage ( int *nerr);
void xscallop ( int *nerr);
void xspectrogram ( int *nerr);
void xxyzc ( int index, 
             int *nerr);
void xyzcleanup (void);
void xzcolors ( int *nerr);
void xzlabels ( int *nerr);
void xzlevels ( int *nerr);
void xzlines ( int *nerr);
void xzticks ( int *nerr);
void zrvfft ( float x[], 
              int m);
void zirvfft ( float x[], 
               int m);
void zstage ( int n, 
              int n2, 
              int n4, 
              float x1[], 
              float x2[], 
              float x3[], 
              float x4[]);
void zistage ( int n, 
               int n2, 
               int n4, 
               float x1[], 
               float x2[], 
               float x3[], 
               float x4[]);

void frange(float *z, int n, float *zmin, float *zmax);
void fbinary(float *z, int n);
void show_colorbar(float x,
                   float y,
                   float width,
                   float height,
                   float  zmin, 
                   float  zmax, 
                   int    lbinary, 
                   int   *nerr);

#endif /* _XYZ_H_ */
