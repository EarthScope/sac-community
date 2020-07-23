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
#define	MXTT            120
#define TTKILOMETER     1       /* indicates distance in kilometers */
#define	TTDEGREE        2       /* indicates distance in degrees */

#define PHASE_NAME_LENGTH 128

/** 
 * @struct kmtt
 *    Travel Time Characters
 */
struct t_kmtt {
    char kttnm[MXTT][9];
    char kttmodl[MXTT][9];
    char kttrd[MTTRD][9];
    char krdph[9];
    char kphases[MXTT][PHASE_NAME_LENGTH];
    char kmodel[9];
    char **kphaseNames;
} ;

/** 
 * @struct cmtt
 *    Travel Time Variables
 */
struct t_cmtt {
    int lttm;                 /** 1 means plot travel time curves in prs, 0 means don't */
    int ltteven[MXTT];        /** 1 means data is evenly spaced */
    int lttplt[MXTT];         /**  */
    int lpreviousModel;       /** 1 means a model was used last time tt command used. maf 960829 */
    int npreviousFileNames;   /** number of files used last time. maf 960829 */
    char *previousFileNames;  /** space delimited list of file names used last time. maf 960829 */
    float xttfirst[MXTT];     /**  */
    float xttdel[MXTT];       /**  */
    float ttdist;             /**  */
    float ttdep;              /**  */
    int ittunit;              /** distance units (TTDEGREE or TTKILOMETER) */
    int lrdtt;                /**  */
    int nttrd;                /**  */
    int nhlines;              /** number of header lines to skip.  maf 970808 */
    float rdvel;              /**  */
    int nrdph;                /**  */
    int nphases;              /** number of phases, eg P, S, Pn etc. */
    int nttm;                 /**  */
    int ndxtty[MXTT];         /** indicates the beginning of a tt data curve */
    int ndxttx[MXTT];         /** same for x axis */
    int nttpt[MXTT];          /**  */
} ;

#define TT_EXTERN                \
    extern struct t_kmtt kmtt;   \
    extern struct t_cmtt cmtt;   \
    extern int *const Ltteven;   \
    extern int *const Lttplt;    \
    extern int *const Ndxttx;    \
    extern int *const Ndxtty;    \
    extern int *const Nttpt;     \
    extern float *const Xttdel;  \
    extern float *const Xttfirst;

#endif /* _TT_H_ */
