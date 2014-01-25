/** 
 * @file   fks.h
 * 
 * @brief  FKS Module
 * 
 */


#ifndef _FKS_H_
#define _FKS_H_

#include <stdio.h>

#include "mach.h"
#include "complex.h"

#define	MXLENP	MDFL

#define MOFFSETOPTS	5
#define OCASCADE	0
#define OREFERENCE	1
#define OUSER		2
#define OSTATION	3
#define OEVENT		4

#define IUSR		1
#define	ISTATION	2
#define IEVENT		4

/** 
 * @struct cmfks
 *    FKS Module Variables
 *
 */
struct t_cmfks {
  char  kmaptype[9];
  int   lkfilter;
  int   lknorm;
  int   leps;
  float eps;
  char  kfkmethod[5];
  int   neigenmlm;
  int   neigenmus;
  int   lexp;
  int   iexp;
  float rkhor;
  int   iazs;
  int   iwvs;
  int   nlcontour;
  float fkmax;
  float fkmax_azimuth;
  float fkmax_wavenum;
  char  scalng[9];
  char  ktitle[81];
  char  kofbbfk[MCPFN+1];
  int   idimsq;
  int   lwr;
  float bear;
  float veloc;
  float anginc;
  float survel;
  float ctrx;
  float ctry;
  float ctrz;
  int   nctr;
  char  kofbeam[MCPFN+1];
  int   flagOffset ;	 /** added for OFFSET option. maf 970306 */
  char  koffset [ MOFFSETOPTS ] [ 9 ] ;	
  
  int   lReference ;	/* 1 if reference option is set. maf 970207 */
  int   nReference ;	/* number of numbers set in reference option. */
  float rReference[ 3 ] ;	/* array of numbers for reference option. maf */
} cmfks;


void calcBeamOffsets ( int ns, 
                       int elevc, 
                       float *xr, 
                       float *yr, 
                       float *zr, 
                       int *nerr);
void cascade ( int ns, 
               int elevc, 
               float *xr, 
               float *yr, 
               float *zr, 
               int *nerr);
int isInfoThere ( int nFiles, 
                  int elevc, 
                  int *nerr);
void refOffsets ( int nFiles, 
                  float *referencePosition, 
                  float *xr, 
                  float *yr, 
                  float *zr, 
                  int *nerr);
void userOffsets ( int nFiles, 
                   float *xr, 
                   float *yr, 
                   float *zr, 
                   int *nerr);
void eventOffsets ( int nFiles, 
                    float *xr, 
                    float *yr, 
                    float *zr, 
                    int *nerr);
void calcoffsets ( int ns, 
                   float *xr, 
                   float *yr, 
                   float *zr, 
                   int *nerr);
void inifks (void);
void xbbfk ( int *nerr);
void covmat ( float **data, 
              int *ptrs, 
              int nch, 
              int nsamples, 
              int ltofilter, 
              complexf *scm, 
              int *nerr);
void fkevalp ( complexf *scm, 
               float *x, 
               float *y, 
               int nch, 
               int naz, 
               double wvnum, 
               int iwvs, 
               float *fks, 
               float fkmax, 
               float fkmax_az, 
               float fkmax_wav, 
               int maxflag);
void fkevalr ( complexf *scm, 
               float *x, 
               float *y, 
               int nch, 
               int nsamps, 
               double wvnum, 
               float *fks);
void eigenanal ( int nm, 
                 int n, 
                 complexf *a, 
                 char *hires, 
                 int m, 
                 complexf *s, 
                 int *nerr);
void xbeam ( int *nerr);
void beamadd ( float *s, 
               float *b, 
               int n, 
               int iadv);
void xfksc ( int index, 
             int *nerr);
int eventfile_count_lines ( char *file, 
                            int *nerr);
float lon180 ( float lon);
float lon360 ( float lon);
void clamp_lat_lon ( int meridian, 
                     float *minlon, 
                     float *maxlon, 
                     float *minlat, 
                     float *maxlat);
void plot_legend_box ( FILE *fp, 
                       float x, 
                       float y, 
                       char *output);
void plot_legend_symbols ( FILE *fp, 
                           char *symbol, 
                           float x, 
                           float y, 
                           float min, 
                           float mean, 
                           float max, 
                           char *output);
void plot_legend_text ( FILE *fp, 
                        float x, 
                        float y, 
                        float min, 
                        float mean, 
                        float max, 
                        char *output);
void xgmtmap ( int *nerr);
void xmap ( int *nerr);

#endif /* _FKS_H_ */
