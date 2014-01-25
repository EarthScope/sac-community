/** 
 * @file   eam.h
 * 
 * @brief  Event Analysis Module
 * 
 */

#ifndef _EAM_H_
#define _EAM_H_

#include <stdio.h>

#include "mach.h"

/** 
 * @struct cmeam
 *    Event Analysis Module Variables
 *
 */
struct t_cmeam {
  double  c1;
  double  c2;
  double  c3;
  double  c4;
  double  c5;
  double  c6;
  double  c7;
  double  c8;
  int    i3;
  int    i4;
  int    i5;
  int    i6;
  int    i8;
  int    i9;
  double  d5;
  double  d8;
  double  d9;
  int    lhpfop;
  FILE  *nhpfun;
  int    lhpfic;
  int    nhpfic[2];
  int    npyear;
  int    npjday;
  int    npmon;
  int    npday;
  int    nphour;
  int    npmin;
  int    lpphas;
  float  psecs;
  int    lsphas;
  float  ssecs;
  int    lampx;
  float  ampx;
  float  prx;
  int    lfini;
  float  fmp;
  int    lapfop;
  FILE  *napfun;
  float  pkcmpa;
  float  pkcmpi;
  float  pkseci;
  int    npkyr;
  int    npkjdy;
  int    npkhr;
  int    npkmn;
  float  pksecs;
  float  pkampl;
  float  dtwf[5];
  float  awf[5];
  int    lpfgmt;
  int    lpfstd;
  int    lichpf;
  int    ichpf[2];
  int    lvalpk;
  float  exteam[19];
} cmeam;


/** 
 * @struct cmeam
 *    Event Analysis Module Characters
 *
 */
struct t_kmeam {
  char  khpfnm[MCPFN+1];
  char  kstid[9];
  char  kpwave[9];
  char  kswave[9];
  char  kapfnm[MCPFN+1];
  char  kpkid[9];
  char  kpkev[17];
  char  kpkst[9];
  char  kpksrc[9];
  char  kpkrid[9];
  char  kpkfmt[9];
  char  kxteam[10][9];
} kmeam;


#ifdef DOINITS

   float *const Awf = &cmeam.awf[0] - 1;
   float *const Dtwf = &cmeam.dtwf[0] - 1;
   float *const Exteam = &cmeam.exteam[0] - 1;
   int   *const Ichpf = &cmeam.ichpf[0] - 1;
   int   *const Nhpfic = &cmeam.nhpfic[0] - 1;

#else

   extern float *const Awf;
   extern float *const Dtwf;
   extern float *const Exteam;
   extern int   *const Ichpf;
   extern int   *const Nhpfic;

#endif


void inieam ( void);
void whpf1  ( char *koutm, 
              int   koutm_s);
void xapk   ( int  *nerr);
void xeamc  ( int   index, 
              int  *nerr);
void xoapf  ( int  *nerr);
void xohpf  ( int  *nerr);
void xwhpf  ( int  *nerr);


#endif /* _EAM_H_ */
