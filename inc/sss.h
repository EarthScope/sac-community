/** 
 * @file   sss.h
 * 
 * @brief  Signal Stacking Subprocess
 * 
 */

#ifndef _SSS_H_
#define _SSS_H_

#include <stdio.h>

#include "mach.h"

#define	MDWUN	  5
#define	MSFL	  MDFL
#define	MVM	  2
#define	MVMTP	  5
#define	RKMPERDG  111.11

#define MAX_PHASES 300

/** 
 * @struct kmsss
 *    Signal Stacking Variables
 *
 */
struct t_cmsss {
  int	lincl[MSFL];
  float	wt[MSFL];	/** ? weight */
  int	lpol[MSFL];	/** logical for instrument polarity */
  float	dst[MSFL]; 	/** distance values of data points. accessed by Dst */
  float beginTime[MSFL];/** these 2 store begin and ennd from hdr.h in array so */
  float endTime[MSFL];	/** the data from all files can be present at once, maf 960701 */
  float dlyt[MSFL];	/** ? delay time, accessed by Dlyt */
  float dlyn[MSFL]; 
  float dlyvm[MSFL]; 
  float dlyti[MSFL]; 
  float dlyni[MSFL]; 
  float dlytg; 
  float dlyng; 
  float wtg;;
  int	lpolg;
  float	dstg;
  float dlytig;
  float dlynig;
  int	lsrc;
  float	srcfac;
  int ndxsum;
  int nlnsum;
  int ndwun;
  int idwun;
  int idwop;	  /** distance window option: default is 1
                   *  1 = use min and max from data file
                   *  2 = use min from file, fix width
                   *  3 = fix data window from v1 to v2 
                   */
  float	dwwid;    /** Distance window width */
  float dwlim[2]; /** distance window limit */
  int   idaop;    /** Distance axis option: default is 1
                   *  1 = fixed axis
                   *  2 = scaled axis	
                   */
  float	dalen;    /** Distance axis length */
  float dasca;    /** Distance axis scale */
  float del;      /** Distance axis delta t */
  float twlim[2]; /** Distance axis limits */
  int	ltwlim;
  int	itaop;
  float	talen;
  float tasca;
  int	lnorm;
  int   lnarli;
  int   lrslab;
  int   lrswt;
  int   lrspol;
  int   lrslin;
  int   lOriginDefault; /** sets location of origin in prs. formerly llefor, maf 961004.*/
  int   lrscur;	        /** Curson Mode in PRS  */
  int	lPlottingTT ;   /** Plotting Travel Time Mode */
  int	lorient;
  float	xpfrac; 
  float xpsize; 
  float axdel;
  int	laspect; 
  int   lpswt; 
  int   lpspl; 
  int   lpssum; 
  int   lpsper;
  int	npsper;
  int	lroset;
  int	iroset;
  int   nvmtp;
  int	lvm[MVM];
  int	ivm[MVM];
  double vapp[MVM];
  double t0vm[MVM];
  double vappi[MVM];
  double t0vmi[MVM];
  int	ntvm[MVM];
  double  tvm[MVM][2];
  int	ndvm[MVM];
  double  dvm[MVM][2];
  int	lcvapp[MVM];
  int   lct0vm[MVM];
  int	inmo;
  int   irefr;
  int	lxlabreq;
  int   lxlabdef;
  int   lylabreq;
  int   lylabdef;
} cmsss;


/** 
 * @struct kmsss
 *    Signal Stacking Characters
 *
 */
struct t_kmsss {
  char  knmsum[MCPFN+1];
  char  kdwun[MDWUN][9];
  char  knmlab[9];
  char  kvmtp[MVMTP][9]; 
  char  kxlabreq[MCMSG+1];
  char  kylabreq[MCMSG+1];
} kmsss;


#ifdef DOINITS

   float *const Dlyn   = &cmsss.dlyn[0] - 1;
   float *const Dlyni  = &cmsss.dlyni[0] - 1;
   float *const Dlyt   = &cmsss.dlyt[0] - 1;
   float *const Dlyti  = &cmsss.dlyti[0] - 1;
   float *const Dlyvm  = &cmsss.dlyvm[0] - 1;
   float *const Dst    = &cmsss.dst[0] - 1;
   float *const Dwlim  = &cmsss.dwlim[0] - 1;
   int   *const Ivm    = &cmsss.ivm[0] - 1;
   int   *const Lct0vm = &cmsss.lct0vm[0] - 1;
   int   *const Lcvapp = &cmsss.lcvapp[0] - 1;
   int   *const Lincl  = &cmsss.lincl[0] - 1;
   int   *const Lpol   = &cmsss.lpol[0] - 1;
   int   *const Lvm    = &cmsss.lvm[0] - 1;
   int   *const Ndvm   = &cmsss.ndvm[0] - 1;
   int   *const Ntvm   = &cmsss.ntvm[0] - 1;
   double *const T0vm   = &cmsss.t0vm[0] - 1;
   double *const T0vmi  = &cmsss.t0vmi[0] - 1;
   float *const Tbegin = &cmsss.beginTime[0] - 1;
   float *const Tend   = &cmsss.endTime[0] - 1;
   float *const Twlim  = &cmsss.twlim[0] - 1;
   double *const Vapp   = &cmsss.vapp[0] - 1;
   double *const Vappi  = &cmsss.vappi[0] - 1;
   float *const Wt     = &cmsss.wt[0] - 1;

#else

   extern float *const Dlyn;
   extern float *const Dlyni;
   extern float *const Dlyt;
   extern float *const Dlyti;
   extern float *const Dlyvm;
   extern float *const Dst;
   extern float *const Dwlim;
   extern int   *const Ivm;
   extern int   *const Lct0vm;
   extern int   *const Lcvapp;
   extern int   *const Lincl;
   extern int   *const Lpol;
   extern int   *const Lvm;
   extern int   *const Ndvm;
   extern int   *const Ntvm;
   extern float *const T0vm;
   extern float *const T0vmi;
   extern float *const Tbegin;
   extern float *const Tend;
   extern float *const Twlim;
   extern float *const Vapp;
   extern float *const Vappi;
   extern float *const Wt;

#endif

void definelimits ( double beginwindow, 
	 double endwindow, 
	 double begindata, 
	 double enddata, 
	 double delta, 
	 int *indexwindow, 
	 int *indexdata, 
	 int *numintersect);
void iaspmodel ( double zs, 
	 float *dstart, 
	 double dinc, 
	 int nloops, 
	 int *nerr);
void inisss (void);
void bkin ( int lu, 
	 int nrec, 
	 int len, 
	 double buf[]);
void brnset ( int nn, 
	 char *pcntl, 
	 int pcntl_s, 
	 int prflg[]);
void depcor ( int nph);
void depset ( double dep, 
	 float usrc[]);
void findtt ( int jb, 
	 double x0[], 
	 int max_, 
	 int *n, 
	 float tt[], 
	 float dtdd[], 
	 float dtdh[], 
	 float dddp[], 
	 char *phnm, 
	 int phnm_s);
void fitspl ( int i1, 
	 int i2, 
	 double tau[][4], 
	 double x1, 
	 double xn, 
	 double coef[][5]);
int iupcor ( char *phnm, 
	 double dtdd, 
	 float *xcor, 
	 float *tcor);
void pdecu ( int i1, 
	 int i2, 
	 double x0, 
	 double x1, 
	 double xmin, 
	 int int_, 
	 int *len);
void r4sort ( int n, 
	 float rkey[], 
	 int iptr[]);
void spfit ( int jb, 
	 int int_);
void tabin ( char *modnam, 
	 int modnam_s);
void tauint ( double ptk, 
	 double ptj, 
	 double pti, 
	 double zj, 
	 double zi, 
	 double *tau, 
	 double *x);
void tauspl ( int i1, 
	 int i2, 
	 double pt[], 
	 double coef[][5]);
void trtm ( double delta, 
	 int max_, 
	 int *n, 
	 float tt[], 
	 float dtdd[], 
	 float dtdh[], 
	 float dddp[], 
	 char *phnm, 
	 int phnm_s);
double umod ( double zs, 
	 int isrc[], 
	 int nph);
double zmod ( double uend, 
	 int js, 
	 int nph);
int _umod_ ( int _entry_);
void iaspcl ( int *nerr);
void phaseadj ( int leven, 
	 int npts, 
	 float xarray[], 
	 float yarray[], 
	 double xfirst, 
	 double xdel, 
	 double bdist, 
	 double dist, 
	 float *atime, 
	 int *nerr);
void rscursor ( float **limits, 
	 int *action, 
	 int *nerr);
void timeadj ( double rdist, 
	 float *atime, 
	 int *nerr);
float timecrossing ( float *distArray, 
	 float distPoint, 
	 float *time, 
	 int nPoints);
void ttint ( float darray[], 
	 float tarray[], 
	 int npts, 
	 float *dvint, 
	 float *tvint, 
	 int *nerr);
void velocityadj ( double velocity, 
	 double bdist, 
	 double dist, 
	 float *atime, 
	 int *nerr);
void vmcalc ( int jvm, 
	 int *nerr);
void vmdly ( int *nerr);
void vmline ( double dstmn, 
	 double dstmx, 
	 int *nerr);
void xaddstack ( int *nerr);
void xchangestack ( int *nerr);
void xdeletestack ( int *nerr);
void xdeltacheck ( int *nerr);
void xdistanceaxis ( int *nerr);
void xdistancewind ( int *nerr);
void xglobalstack ( int *nerr);
void xincrementsta ( int *nerr);
void xliststack ( int *nerr);
void xphase ( int *nerr);
void xplotrecords ( int *nerr);
void xplotstack ( int *nerr);
void xquitsss ( int *nerr);
void xsss ( int *nerr);
void xsssc ( int index, 
	 int *nerr);
void xsumstack ( int *nerr);
void xtimeaxis ( int *nerr);
void xtimewindow ( int *nerr);
void xtraveltime ( int *nerr);
void readtaup ( FILE *taupfile , int *ncurves , int *nerr);
void xvelocitymode ( int *nerr);
void xvelocityrose ( int *nerr);
void xwritestack ( int *nerr);
void xzerostack ( int *nerr);

#endif /* _SSS_H_ */
