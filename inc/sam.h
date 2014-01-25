/** 
 * @file   sam.h
 * 
 * @brief  Spectral Analysis
 * 
 */


#ifndef _SAM_H_
#define _SAM_H_

#include "mach.h"

#define	MFFT	16777216
#define	MPROTYP	6
#define	MRIR	17
#define	MTAPTP	5
#define	MTPIIR	4
#define	MWINTP	5
#define	NDATPTS	501
#define	NIMPPTS	1000
#define	NPTLEN	3


/** 
 * @struct kmsam 
 *    Spectral Analysis Lengths
 */
struct t_cmsam {
  int   ifwd;
  int   ibwd;
  int   lrlim;
  int   lwmean;
  int   lspfwd;
  int   lspbwd;
  char  knmfir[MCPFN+1];
  int   lrqrec;
  int   itplp;
  int   npollp;
  double cflp;
  int   npaslp;
  double tbwlp;
  double atnlp;
  int   itphp;
  int   npolhp;
  double cfhp;
  int   npashp;
  double tbwhp;
  double atnhp;
  int   itpbp;
  int   npolbp;
  double cfbp1;
  double cfbp2;
  int   npasbp;
  double tbwbp;
  double atnbp;
  int   itpbr;
  int   npolbr;
  double cfbr1;
  double cfbr2;
  int   npasbr;
  double tbwbr;
  double atnbr;
  int   ncwien;
  int   lmu;
  int   lepsilon;	 /* 1 if wienmu is set, 0 else.  maf 960723 */
  float	wienwb;  /* 1 if epsilon is set, 0 else. maf 960723 */
  float wienwe;
  double wienmu;
  double epsilon;  /* mu (adaption parameter) used in wiener() */
  double ortwwi[2];
  int   nsptpl;
  int   lramph;
  int   lrspe;
  int   lwspov;
  int   nwspfl;
  int   lwamph;
  int   lwrlim;
  int   lwspc1;
  int   lwspc2;
  int   npsppp;
  int   lpamph;
  int   lprlim;
  int   lpspc1;
  int   lpspc2;
  int   ixspin;
  int   iyspin;
  double widtap;
  int   lrtwwi;
  int   lunwfz;
  int   nunwfz;
  double vunwit;
  double vunwct;
  float extsam[17];
  int   ntaptp;
  int   itaptp;
  double cutkhr;
  int   nwin;
  int   lwinln;
  double winln;
  int   iwintp;
  int   imast;
  float fddelta;
} cmsam;


/** 
 * @struct kmsam 
 *    Spectral Analysis Characters 
 */
struct t_kmsam {
  char ktpiir[MTPIIR][9];
  char kprotyp[MPROTYP][4];
  char krtwwi[2][9];
  char ksptpl[8][9];
  char krsps1[9];
  char krsps2[9];
  char kwsptp[9];
  char kwspfl[MAXCHARS];
  char kwsps1[9];
  char kwsps2[9];
  char kpsptp[9];
  char kpspl1[17];
  char kpspl2[17];
  char ktaptp[MTAPTP][9];
  char kxtsam[6][9];
  char kwintp[MWINTP][9];
} kmsam;


#ifdef DOINITS

   float *const Extsam = &cmsam.extsam[0] - 1;

#else

   extern float *const Extsam;
   extern float *const Ortwwi;

#endif


void chkpha ( float *ph, 
              double pv, 
              int *iscons);
void cpft ( float r[], 
            float i[], 
            int n, 
            int incp, 
            int isignp);
void envelope ( int n, 
                float *in, 
                float *out);
double estpha ( float x[], 
                int nx, 
                int i, 
                float *pphase, 
                float *ppdvt, 
                double ppv, 
                double pdvt, 
                int *iscons);
void fdWhitenWrite ( int *memptr, 
                     char *kprefix, 
                     float *userData, 
                     int newnpts, 
                     int nFreq, 
                     int *nerr);
void aphdrw ( int newnpts, 
              int nFreq);
void gdhdrw ( int newnpts, 
              int nFreq);
void irhdrw ( int newnpts, 
              int nFreq);
void fdWriteFiles ( int *memptr, 
                    char *kprefix, 
                    float *userData, 
                    int newnpts, 
                    int *nerr);
void aphdr ( int newnpts);
void gdhdr ( int newnpts);
void irhdr ( int newnpts);
void fillNZ (void);
void fdbp ( int memptr[], 
            int mxmptr, 
            float *userData, 
            int *nerr);
void fdbr ( int memptr[], 
            int mxmptr, 
            float *userData, 
            int *nerr);
void fdhp ( int memptr[], 
            int mxmptr, 
            float *userData, 
            int *nerr);
void fdlp ( int memptr[], 
            int mxmptr, 
            float *userData, 
            int *nerr);
int fdplot ( int memptr[], 
             int lprint, 
             int *xbeg, 
             int *nerr);
void getlims ( float datary[], 
               int npts, 
               float *min_, 
               float *max_);
void loadxtmp ( float xtmp[], 
                double xwmin, 
                double xwmax, 
                int npts);
void sduration ( float s[], 
                 int *npts, 
                 int *min_, 
                 int *max_);
double filtb ( int iopt, 
               float xt);
void filterdesign ( int *nerr);
double filtk ( int iopt, 
               double freq, 
               double yt);
void inisam (void);
void rfir ( char *knmfir, 
            int knmfir_s, 
            int mfir, 
            float cfir[], 
            int *nfir, 
            float *desdt, 
            char *kidfir, 
            int kidfir_s, 
            int *nerr);
void spcval ( int nx, 
              float x[], 
              double freq, 
              float *xr, 
              float *xi, 
              float *yr, 
              float *yi);
void toamph ( float rl[], 
              float im[], 
              int num, 
              float am[], 
              float ph[]);
void torlim ( float am[], 
              float ph[], 
              int num, 
              float rl[], 
              float im[]);
void unwrap ( float x[], 
              int nx, 
              int nt, 
              double thrcon, 
              double thrinc, 
              float aux1[], 
              float aux2[], 
              float aux3[], 
              float am[], 
              float ph[], 
              int *nok, 
              int *lok);
void xbenioff ( int *nerr);
void xbp ( int *nerr);
void xbr ( int *nerr);
void xconvolve ( int *nerr);
void xcorrelate ( int *nerr);
void xdft ( int *nerr);
void xdivomega ( int *nerr);
void xenvelope ( int *nerr);
void xfir ( int *nerr);
void xhan ( int *nerr);
void xhilbert ( int *nerr);
void xhp ( int *nerr);
void xidft ( int *nerr);
void xkeepam ( int *nerr);
void xkhronhite ( int *nerr);
void xlp ( int *nerr);
void xmulomega ( int *nerr);
void xpsp ( int *nerr);
void xrsp ( int *nerr);
void xsamc ( int index, 
             int *nerr);
void xunwr ( int *nerr);
void xwnr ( int *nerr);
void xwsp ( int *nerr);

#endif /* _SAM_H_ */
