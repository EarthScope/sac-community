/** 
 * @file   icm.h
 * 
 * @brief  Instrument Correction Module
 * 
 */


#ifndef _ICM_H_
#define _ICM_H_

#include "mach.h"
#include "complex.h"
#include "wtofd.h"

#define	MAXFP	10
#define	MAXIP	1
#define	MAXKP	2
#define	MINSTR	50


/** 
 * @struct cmicm 
 *    Instrument Correction Variables
 *
 */
struct t_cmicm {
  float fpfrom[MAXFP];
  int   lfpfrom[MAXFP];
  int   ipfrom[MAXIP];
  int   lipfrom[MAXIP];
  int   lkpfrom[MAXKP];
  float fpto[MAXFP];
  int   lfpto[MAXFP];
  int   ipto[MAXIP];
  int   lipto[MAXIP];
  int   lkpto[MAXKP];
  int   lfreql;
  int   lprew;
  int   iprew;
  double freq[4];
  int   ninstr;
  int   lfd ;
} cmicm;


/** 
 * @struct kmicm 
 *    Instrument Correction Characters 
 *
 */
struct t_kmicm {
  char  kpfrom[MAXKP][MCPFN+1];
  char  kpto[MAXKP][MCPFN+1];
  char  kinstr[MINSTR][9];
} kmicm;


#ifdef DOINITS

   float *const Fpfrom  = &cmicm.fpfrom[0] - 1;
   float *const Fpto    = &cmicm.fpto[0] - 1;
   double *const Freq    = &cmicm.freq[0] - 1;
   int   *const Ipfrom  = &cmicm.ipfrom[0] - 1;
   int   *const Ipto    = &cmicm.ipto[0] - 1;
   int   *const Lfpfrom = &cmicm.lfpfrom[0] - 1;
   int   *const Lfpto   = &cmicm.lfpto[0] - 1;
   int   *const Lipfrom = &cmicm.lipfrom[0] - 1;
   int   *const Lipto   = &cmicm.lipto[0] - 1;
   int   *const Lkpfrom = &cmicm.lkpfrom[0] - 1;
   int   *const Lkpto   = &cmicm.lkpto[0] - 1;

#else

   extern float *const Fpfrom;
   extern float *const Fpto;
   extern double *const Freq;
   extern int   *const Ipfrom;
   extern int   *const Ipto;
   extern int   *const Lfpfrom;
   extern int   *const Lfpto;
   extern int   *const Lipfrom;
   extern int   *const Lipto;
   extern int   *const Lkpfrom;
   extern int   *const Lkpto;

#endif

void InterpolateArrays ( double *freqs, 
                         int nfreqs, 
                         double *tmpRe, 
                         double *tmpIm, 
                         int nfreq, 
                         double *xre, 
                         double *xim);
void deblank ( char *strg);
int EvrespGateway ( int nfreq, 
                    double delfrq, 
                    double xre[], 
                    double xim[], 
                    float *nmScale, 
                    char *inFile);
void acc ( int nfreq, 
           double delfrq, 
           double xre[], 
           double xim[]);
void exchange ( double **matrix, 
                int kdx, 
                int mdx, 
                int order);
int p1norm0 ( double **matrix, 
              int kdx, 
              int order);
int balance ( double **matrix, 
              int order);
void bbdisp ( int nfreq, 
              double delfrq, 
              double xre[], 
              double xim[]);
void bbvel ( int nfreq, 
             double delfrq, 
             double xre[], 
             double xim[]);
void benbog ( int nfreq, 
              double delfrq, 
              double xre[], 
              double xim[]);
void ckinst ( float fp[], 
              int lfp[], 
              int ip[], 
              int lip[], 
              char *kp, 
              int kp_s, 
              int lkp[], 
              int *nerr);
void clh ( int nfreq, 
           double delfrq, 
           double xre[], 
           double xim[]);
void clz ( int nfreq, 
           double delfrq, 
           double xre[], 
           double xim[]);
void cmh ( int nfreq, 
           double delfrq, 
           double xre[], 
           double xim[]);
void cmz ( int nfreq, 
           double delfrq, 
           double xre[], 
           double xim[]);
void csh ( int nfreq, 
           double delfrq, 
           double xre[], 
           double xim[]);
void csz ( int nfreq, 
           double delfrq, 
           double xre[], 
           double xim[]);
void dcpft ( double re[], 
             double im[], 
             int nfreq, 
             int incp, 
             int isignp);
void dewit ( float data[], 
             int nsamps, 
             int order, 
             float a[], 
             char *errmsg);
void dseis ( int nfreq, 
             double delfrq, 
             double xre[], 
             double xim[], 
             float fp[], 
             int ip[], 
             char kp[MAXKP][MCPFN+1], 
             int kp_s, 
             float *nmScale, 
             int *nerr);
void dss ( int nfreq, 
           double delfrq, 
           double xre[], 
           double xim[]);
void dwwssn ( int nfreq, 
              double delfrq, 
              double xre[], 
              double xim[]);
void ekalp6 ( int nfreq, 
              double delfrq, 
              double xre[], 
              double xim[]);
void ekasp2 ( int nfreq, 
              double delfrq, 
              double xre[], 
              double xim[]);
void elmag ( int nfreq, 
             double delfrq, 
             double xre[], 
             double xim[], 
             double freepd, 
             double mag, 
             int *nerr);
void eyeomg ( int nfreq, 
              double delfrq, 
              double xre[], 
              double xim[], 
              int nzer);
void gbalp ( int nfreq, 
             double delfrq, 
             double xre[], 
             double xim[]);
void gbasp ( int nfreq, 
             double delfrq, 
             double xre[], 
             double xim[]);
void general ( int nfreq, 
               double delfrq, 
               double xre[], 
               double xim[], 
               int nzer, 
               double t0, 
               double h, 
               double const_);
void getins ( char *kinstr, 
              int kinstr_s, 
              int ninstr, 
              int *ldone, 
              float fp[], 
              int lfp[], 
              int ip[], 
              int lip[], 
              char *kp, 
              int kp_s, 
              int lkp[], 
              int *nerr);
void getran(int nfreq, 
            double delfrq, 
            double const_, 
            int nzero, 
            complexf zero[], 
            int npole, 
            complexf pole[], 
            double xre[], 
            double xim[]);
void getroots ( float in[], 
                int order, 
                struct roots theseRoots[], 
                int *nerr);
void gsref ( int nfreq, 
             double delfrq, 
             double xre[], 
             double xim[]);
void hfslpwb ( int nfreq, 
               double delfrq, 
               double xre[], 
               double xim[]);
void hqr ( double **matrix, 
           int order, 
           struct roots theseRoots[], 
           int *nerr);
void hs3 ( int nfreq, 
           double delfrq, 
           double xre[], 
           double xim[]);
void iniicm (void);
void lll ( int nfreq, 
           double delfrq, 
           double xre[], 
           double xim[], 
           char *subtyp, 
           int subtyp_s, 
           double freepd, 
           double damp, 
           int *nerr);
void llsn ( int nfreq, 
            double delfrq, 
            double xre[], 
            double xim[]);
void lrsmlp ( int nfreq, 
              double delfrq, 
              double xre[], 
              double xim[]);
void lrsmsp ( int nfreq, 
              double delfrq, 
              double xre[], 
              double xim[]);
void noress ( int nfreq, 
              double delfrq, 
              double xre[], 
              double xim[], 
              char *subtyp, 
              int subtyp_s);
void noresshf ( int nfreq, 
                double delfrq, 
                double xre[], 
                double xim[]);
void oldbb ( int nfreq, 
             double delfrq, 
             double xre[], 
             double xim[]);
void oldkir ( int nfreq, 
              double delfrq, 
              double xre[], 
              double xim[]);
void polezero ( int nfreq, 
                double delfrq, 
                double xre[], 
                double xim[], 
                char *subtyp, 
                int subtyp_s, 
                int *nerr);
void portable ( int nfreq, 
                double delfrq, 
                double xre[], 
                double xim[], 
                double freepd, 
                double damp, 
                double crfrq);
void predfl ( float data[], 
              int npts, 
              float a[], 
              int nc, 
              float result[], 
              char *errmsg);
void prewit ( float data[], 
              int nsamps, 
              int *order, 
              float array[], 
              char *kprefix, 
              char *errmsg);
void ptbllp ( int nfreq, 
              double delfrq, 
              double xre[], 
              double xim[]);
void redkir ( int nfreq, 
              double delfrq, 
              double xre[], 
              double xim[]);
void reftek ( int nfreq, 
              double delfrq, 
              double xre[], 
              double xim[], 
              double freepd, 
              double damp, 
              double crfrq, 
              double hpfrq);
void rs7 ( int nfreq, 
           double delfrq, 
           double xre[], 
           double xim[], 
           char *subtyp);
void rsk ( int nfreq, 
           double delfrq, 
           double xre[], 
           double xim[], 
           char *subtyp);
void rsl ( int nfreq, 
           double delfrq, 
           double xre[], 
           double xim[]);
void rsm ( int nfreq, 
           double delfrq, 
           double xre[], 
           double xim[]);
void rstn ( int nfreq, 
            double delfrq, 
            double xre[], 
            double xim[], 
            char *subtyp, 
            int subtyp_s, 
            int *nerr);
void sandia ( int nfreq, 
              double delfrq, 
              double xre[], 
              double xim[], 
              char *subtyp, 
              int subtyp_s, 
              int *nerr);
void snla3 ( int nfreq, 
             double delfrq, 
             double xre[], 
             double xim[]);
void sro ( int nfreq, 
           double delfrq, 
           double xre[], 
           double xim[], 
           char *subtyp, 
           int subtyp_s);
double taper_spectra ( double freq, 
               double fqh, 
               double fql);
void dcpft ( double re[], 
             double im[], 
             int nfreq, 
             int incp, 
             int isignp);
int NearestInt ( double D);
int IsNormalized ( double calper, 
                   int nfreq, 
                   double delfrq, 
                   const double *xre, 
                   const double *xim);
double GetNormalizationFactor ( int nfreq, 
                                double delfrq, 
                                const double *xre, 
                                const double *xim);
void transfer ( float dat[], 
                int npts, 
                double delta, 
                float fpfrom[], 
                int ipfrom[], 
                char kpfrom[MAXKP][MCPFN+1], 
                int kpfrom_s, 
                float fpto[], 
                int ipto[], 
                char kpto[MAXKP][MCPFN+1], 
                int kpto_s, 
                double f[], 
                int *iprew, 
                double sre[], 
                double sim[], 
                int nfft, 
                double xre[], 
                double xim[], 
                int nfreq, 
                int *nerr);
void vel ( int nfreq, 
           double delfrq, 
           double xre[], 
           double xim[]);
void wa ( int nfreq, 
          double delfrq, 
          double xre[], 
          double xim[]);
void wabn ( int nfreq, 
            double delfrq, 
            double xre[], 
            double xim[]);
void wiech ( int nfreq, 
             double delfrq, 
             double xre[], 
             double xim[]);
void wwlpbn ( int nfreq, 
              double delfrq, 
              double xre[], 
              double xim[]);
void wwsp ( int nfreq, 
            double delfrq, 
            double xre[], 
            double xim[]);
void wwspbn ( int nfreq, 
              double delfrq, 
              double xre[], 
              double xim[]);
void xicmc ( int index, 
             int *nerr);
void xprewit ( int *nerr);
void xtransfer ( int *nerr);
void ykalp ( int nfreq, 
             double delfrq, 
             double xre[], 
             double xim[]);
void ykasp ( int nfreq, 
             double delfrq, 
             double xre[], 
             double xim[]);

void dbaseResponse ( int a, 
                     double b, 
                     double *c, 
                     double *d, 
                     float *e, 
                     int *nerr);
void DisconnectFromOracleTransfer (void);
void getSensorInstrumentCalibInfo ( double *ncalper, 
                                    double *ncalib, 
                                    double *calper, 
                                    double *calratio);
void frequency_amplitude_phase(int     nf,
                               double  df,
                               double *xre,
                               double *xim,
                               char   *file,
                               int     file_s,
                               int    *nerr) ;

#endif /* _ICM_H_ */
