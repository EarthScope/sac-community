/** 
 * @file   smm.h
 * 
 * @brief  Signal Measurement Module
 * 
 */

#ifndef _SMM_H_
#define _SMM_H_

#define	MVEL	10

/** 
 * @struct kmsmm
 *     Signal Measuerement Module Variables
 *
 */
struct t_cmsmm {
  int    lmtw;
  double omtw[2];
  int    nvel;
  double vel[MVEL];
  int    ldistr;
  double distr;
  int    loriginr;
  double originr;
  int    lgmt;
  int    iodttm[6];
  double value;
  int    lgedata;
  double  winlen;
  int    lnoisemtw;
  double onoisemtw[2];
  int    irmspick;
} cmsmm;

/** 
 * @struct kmsmm
 *     Signal Measuerement Module Characters
 *
 */
struct t_kmsmm {
  char  kmtw[2][9];
  char  ktmark[9];
  char  kvmark[9];
  char  kpmark[9];
  char  knoisemtw[2][9];
} kmsmm;


#ifdef DOINITS

   int   *const Iodttm = &cmsmm.iodttm[0] - 1;
   double *const Omtw = &cmsmm.omtw[0] - 1;
   double *const Onoisemtw = &cmsmm.onoisemtw[0] - 1;
   double *const Vel = &cmsmm.vel[0] - 1;

#else

   extern int   *const Iodttm;
   extern double *const Omtw;
   extern double *const Onoisemtw;
   extern double *const Vel;

#endif

void inismm (void);
void ptp ( float signal[], 
           int npts, 
           int *length, 
           float *ptpval, 
           int *ipmin, 
           int *ipmax);
void xmarkptp ( int *nerr);
void xmarktimes ( int *nerr);
void xmarkvalue ( int *nerr);
void xrms ( int *nerr);
void xsmmc ( int index, 
             int *nerr);

#endif /* _SMM_H_ */
