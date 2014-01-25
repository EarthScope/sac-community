/** 
 * @file   gam.h
 * 
 * @brief  Graphics Attributes
 * 
 */


#ifndef _GAM_H_
#define _GAM_H_

#include <stdio.h>

#include "mach.h"

#define	MFIDFM	5
#define	MFIDLC	5
#define	MFIDNM	10
#define	MFIDTP	5
#define	MOP1	20
#define	MOP2	20
#define	MOPE	100
#define	MOPN	10
#define	MOPNLI	50
#define	MOPT	10
#define	MP2LOC	5
#define	MPKNAM	13
#define	MPKTYP	3

#define	MYLIM	1000
#define MDEFAULT 0
#define MGREY   1
#define MCOLOR  2


struct t_cmgam {
  int   lppkpp;
  int   nppkpp;
  int   lppkut;
  int   lppkrl;
  float vppkrl;
  int   lmkall;
  int   lsavelocs;
  int   lp2abs;
  int   ip2loc;
  float tsp2;
  int   nope;
  int   nop1;
  int   nop2;
  int   nopn;
  int   nopt;
  int   lrplrq;
  int   lpcfil;
  float scamac;
  float rotmac;
  float xopnli[MOPNLI];
  float yopnli[MOPNLI];
  float pcdegi;
  int   npcpsi;
  int   lglori;
  float xori;
  float yori;
  float xcdp;
  float ycdp;
  float xcen;
  float ycen;
  float xcdpe;
  float ycdpe;
  float pcalen;
  float pcamul;
  float pcaang;
  int   lpcavi;
  int   lpcafi;
  int   icsize;
  int   lbdrrq;
  int   nopei;
  int   nhtick;
  int   nvtick;
  int   lbdron;
  int   ldsppk;
  int   ipktyp[MPKNAM];
  float pkwdth;
  float pkhgth;
  float tspk;
  float tsfid;
  int   lfidrq;	/* 1 or 0 if fileid is on or off respectively */
  int   lfinorq;	/* 1 or 0 if filenumber is on or off respectively. maf 970204 */
  int   nfidtp;
  int   ifidtp;
  int   nfidlc;
  int   ifidlc;
  float xfidlc;
  float yfidlc;
  int   nfidnm;
  int   nfidst;
  int   nfidtx;
  int   ifidfm;
  int   nfidfm;
  int   iur;
  int   iul;
  int   ilr;
  int   ill;
  int   lwaitr;
  int   lwaite;
  int   lrtwxl;
  double ortwxl[2];
  int   nylim;
  float ylims[MYLIM][2];
  float rngmin;
  float rngmax;
  float fidbdr;
  float xrect[4];
  float yrect[4];
  float extgam[11];
  int   cmap;
} cmgam;

struct t_kmgam {
  char  kp2loc[MP2LOC][9];
  char  kope[MOPE][3];
  char  kop1[MOP1];
  char  kop2[MOP2];
  char  kopn[MOPN];
  char  kopt[MOPT];
  char  kopetx[81];
  char  kopqui;
  char  kopdel;
  char  kopmac;
  char  kopbe;
  char  kopee;
  char  kopcmt;
  char  kpcfil[MCPFN+1];
  char  kpcmac[MCPFN+1];
  char  kpcfsu[5];
  char  kpcmsu[5];
  char  kopnli[MOPNLI][9];
  char  kpknam[MPKNAM][9];
  char  kpktyp[MPKTYP][9];
  char  kfidtp[MFIDTP][9];
  char  kfidlc[MFIDLC][9];
  char  kfidnm[MFIDNM][9];
  char  kfidst[MFIDNM][9];
  char  kfidtx[MFIDNM][41];
  char  kfidfm[MFIDFM][9];
  char  krtwxl[2][9];
  char  kylims[MYLIM][9];
  char  kgddef[9];
  char  kxtgam[9][9];
} kmgam;


#ifdef DOINITS

   float *const Extgam = &cmgam.extgam[0] - 1;
   int   *const Ipktyp = &cmgam.ipktyp[0] - 1;
   char  *const Kop1   = &kmgam.kop1[0] - 1;
   char  *const Kop2   = &kmgam.kop2[0] - 1;
   char  *const Kopn   = &kmgam.kopn[0] - 1;
   char  *const Kopt   = &kmgam.kopt[0] - 1;
   double *const Ortwxl = &cmgam.ortwxl[0] - 1;
   float *const Xopnli = &cmgam.xopnli[0] - 1;
   float *const Xrect  = &cmgam.xrect[0] - 1;
   float *const Yopnli = &cmgam.yopnli[0] - 1;
   float *const Yrect  = &cmgam.yrect[0] - 1;

#else

   extern float *const Extgam;
   extern int   *const Ipktyp;
   extern char  *const Kop1;
   extern char  *const Kop2;
   extern char  *const Kopn;
   extern char  *const Kopt;
   extern double *const Ortwxl;
   extern float *const Xopnli;
   extern float *const Xrect;
   extern float *const Yopnli;
   extern float *const Yrect;

#endif

char *tmpfile_create(char *template, int xs);
int sgf_to_ps(char *sgf, char *ps);
int ps_print(char *ps, char *printer);
int sgf_print(char *sgf, char *printer);

void pkdet(float array[], int ndxmx, float si, int ndxst, int *ndxpk);
void pkfunc(float fdold, float fdnew, float *chfsta, float *chflta, float *chf);
void pkfilt(float diff, float fdold, float *fdnew, float *rmnabs);
void pkchar ( float *array, int ndxmx, float si, int ndxpk, char *ktype, char *kdir, char *kqual);

void dispid(int ldfl, int idfl, int nlast, char **last);
void disppk(double tdelay);
void disppkLandscape(double tdelay);
void getxlm(int *lxlm, float *xmin, float *xmax);
void getylm(int *lylmon, float *ystart, float *ystop);
void inigam(void);
void markvert(int jmark1, int jmark2, float *xloc, double ytop, double ydel, char *klabel, int klabel_s, int nmarks);
void markwf(int jmark1, int jmark2, float *xloc1, float *xloc2, double ytop, double ydel, char *klabel, int klabel_s);
void pcmcur(FILE *nunmac);
void pcmrpl(FILE *nunmac, double scale, double angle);
void pcrrpl(FILE *nunrpl, char *kchar, char *kchar2, int *lend, int *lquit);
void pcxcur(FILE *nunrpl);
void pcxop1(int iop1);
void pcxop2(int iop1, int iop2);
void pcxope(int iope, int iopei);
void pcxops(int iop1, int iop2, float xloc[], float yloc[], double degi);
void pcxrpl(FILE *nunrpl, int *lquit);
void pkeval(float array[], int ndxmx, double si, int ndxpk, int *nlncda);
void wavfrm(float array[], int nst, int nlen, double value, int mwf, int iwf[], int *lwfok);
void xfid(int *nerr);
void xfilenumber(int *nerr);
void xfitxy(int *nerr);
void xgamc(int index, int *nerr);
void xp(int *nerr);
void xp1(int *nerr);
void xp2(int *nerr);
void xpc(int *nerr);
void xpicks(int *nerr);
void xplotalpha(int *nerr);
void xplotdy(int *nerr);
void xplotpm(int *nerr);
void xplotxy(int *nerr);
void xppk(int *nerr);
void xprint(int *nerr);
void xsetdevice(int *nerr);
void xylim(int *nerr);


#endif /* _GAM_H_ */
