/** 
 * @file   tt.h
 * 
 * @brief  Travel Time
 * 
 */

#ifndef _TT_H_
#define _TT_H_

#define	MTTLEN          9
#define	MTTRD           2
#define	MXTT            60
#define TTKILOMETER     1       /* indicates distance in kilometers */
#define	TTDEGREE        2       /* indicates distance in degrees */


/** 
 * @struct kmtt
 *    Travel Time Characters
 */
struct t_kmtt {
  char  kttnm[MXTT][9];
  char  kttmodl[MXTT][9];
  char  kttrd[MTTRD][9];
  char  krdph[9];
  char  kphases[MXTT][9];
  char  kmodel[9];
  char  **kphaseNames;
} kmtt;

/** 
 * @struct cmtt
 *    Travel Time Variables
 */
struct t_cmtt {
  int   lttm;                 /** 1 means plot travel time curves in prs, 0 means don't */
  int   ltteven[MXTT];        /** 1 means data is evenly spaced */
  int   lttplt[MXTT];         /**  */
  int   lpreviousModel;       /** 1 means a model was used last time tt command used. maf 960829 */
  int   npreviousFileNames;   /** number of files used last time. maf 960829 */
  char *previousFileNames;    /** space delimited list of file names used last time. maf 960829 */
  float	xttfirst[MXTT];       /**  */
  float xttdel[MXTT];         /**  */
  float ttdist;               /**  */
  float ttdep;                /**  */
  int   ittunit;              /** distance units (TTDEGREE or TTKILOMETER) */
  int   lrdtt;                /**  */
  int   nttrd;                /**  */
  int   nhlines ;             /** number of header lines to skip.  maf 970808 */
  float rdvel;                /**  */
  int   nrdph;                /**  */
  int   nphases;              /** number of phases, eg P, S, Pn etc. */
  int   nttm;                 /**  */
  int   ndxtty[MXTT];         /** indicates the beginning of a tt data curve in cmmem.sacmem */
  int   ndxttx[MXTT];         /** same for x axis */
  int   nttpt[MXTT];          /**  */
} cmtt;


#ifdef DOINITS

   int   *const Ltteven  = &cmtt.ltteven[0] - 1;
   int   *const Lttplt   = &cmtt.lttplt[0] - 1;
   int   *const Ndxttx   = &cmtt.ndxttx[0] - 1;
   int   *const Ndxtty   = &cmtt.ndxtty[0] - 1;
   int   *const Nttpt    = &cmtt.nttpt[0] - 1;
   float *const Xttdel   = &cmtt.xttdel[0] - 1;
   float *const Xttfirst = &cmtt.xttfirst[0] - 1;

#else

   extern int   *const Ltteven;
   extern int   *const Lttplt;
   extern int   *const Ndxttx;
   extern int   *const Ndxtty;
   extern int   *const Nttpt;
   extern float *const Xttdel;
   extern float *const Xttfirst;

#endif

#endif /* _TT_H_ */
