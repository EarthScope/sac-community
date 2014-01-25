/** 
 * @file   spe.h
 * 
 * @brief  Spectral Estimation
 * 
 * 
 */

#ifndef _SPE_H_
#define _SPE_H_

#include "mach.h"


#define	MLNPE	100
#define	MPREWH	12
#define	MPSPTP	5
#define	MWINTP	5
#define MINPOW  2048	/* minimum power of 2 for spectral windows. maf 980602 */


/** 
 * @struct cmspe
 *    Spectral Estimation Lengths
 */
struct t_cmspe {
  int   lfile;
  int   ndxdat;
  int   nlndat;
  float samfrq;
  int   ndxcor;
  int   ndxpe;
  int   ndxspe;
  int   ndxaux;
  int   lcor;
  int   iwncor;
  int   numwin;
  float winlen;
  int   nwinln;
  int   lsnumw;
  int   lprewh;
  int   nprerq;
  int   nprewh;
  int   junk;
  float cprewh[MPREWH + 1];
  int   nlncor;
  int   nlnfft;
  int   nlnspe;
  int   lspe;
/* 100422 jas/vt fixed three typos pointed out by grh/ub */
/*  int   lpcspec;  typo */
  int   lpcsec;
/*  float pcspec;  typo */
  float pcsec;
  int   iwnpds;
/*  float specpds;  typo */
  float secpds;
  int   nlgpds;
  int   nlgmem;
  int   nlgmlm;
  int   lresl;
  float resl;
  int   lcl;
  float clu;
  float cll;
  int   lrqcl;
  int   lspeid;
  int   npsptp;
  int   ipsptp;
  float extspe[20];
  int   firstPowerOf2 ;   /** first power of 2 >= number of datapoints. maf 980527 */
} cmspe;


/** 
 * @struct kmspe
 *    Spectral Estimation Characters
 */
struct t_kmspe {
  char kscale[11];
  char kwintp[MWINTP][9];
  char kpsptp[MPSPTP][9];
  char kpspl1[17];
  char kpspl2[17];
  char kpspl3[17];
  char knmcor[MCPFN+1];
  char knmspe[MCPFN+1];
  char kermsg[131];
  char kxtspe[10][9];
} kmspe;


#ifdef DOINITS

   float *const Cprewh = &cmspe.cprewh[0] - 1;
   float *const Extspe = &cmspe.extspe[0] - 1;

#else

   extern float *const Cprewh;
   extern float *const Extspe;

#endif

void inispe (void);
void xcor ( int *nerr);
void xmem ( int *nerr);
void xmlm ( int *nerr);
void xpcor ( int *nerr);
void xpds ( int *nerr);
void xppe ( int *nerr);
void xpspe ( int *nerr);
void xquitspe ( int *nerr);
void xrcor ( int *nerr);
void xspe ( int linit, 
            int *nerr);
void xspec ( int index, 
             int *nerr);
void xwcor ( int *nerr);
void xwhiten ( int *nerr);
void xwspe ( int *nerr);

#endif /* _SPE_H_ */
