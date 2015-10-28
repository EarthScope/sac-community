/** 
 * @file   icm.h
 * 
 * @brief  Instrument Correction Module
 * 
 */

#ifndef _ICM_H_
#define _ICM_H_

#include <stdio.h>
#include "mach.h"
#include "sac_complex.h"
#include "wtofd.h"
#include "sac_datetime.h"

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
    int lfpfrom[MAXFP];
    int ipfrom[MAXIP];
    int lipfrom[MAXIP];
    int lkpfrom[MAXKP];
    float fpto[MAXFP];
    int lfpto[MAXFP];
    int ipto[MAXIP];
    int lipto[MAXIP];
    int lkpto[MAXKP];
    int lfreql;
    int lprew;
    int iprew;
    double freq[4];
    int ninstr;
    int lfd;
} ;

/** 
 * @struct kmicm 
 *    Instrument Correction Characters 
 *
 */
struct t_kmicm {
    char kpfrom[MAXKP][MCPFN + 1];
    char kpto[MAXKP][MCPFN + 1];
    char kinstr[MINSTR][9];
} ;

#define ICM_EXTERN \
    extern struct t_kmicm kmicm;                \
    extern struct t_cmicm cmicm;


typedef struct _pzmeta_t pzmeta_t;
typedef struct _pzcomment_t pzcomment_t;
typedef struct _pz_t pz_t;
typedef struct _station_id_t station_id_t;

struct _pz_t {
    int nzero;
    int npole;
    complexd *poles;
    complexd *zeros;
    double constant;
    int nerr;
    char *line;
};

struct _station_id_t {
    char *net;
    char *stat;
    char *loc;
    char *chan;
    datetime *ref;
};

struct _pzmeta_t {
    char *net;
    char *stat;
    char *chan;
    char *loc;
    datetime *created;
    datetime *start;
    datetime *end;
    char *descrip;
    float lat;
    float lon;
    float elev;
    float depth;
    float dip;
    float az;
    float sample_rate;
    char *input_unit;
    char *output_unit;
    char *instrument_type;
    float gain;
    char *comment;
    float sensitivity;
    float a0;
};

struct _pzcomment_t {
    char *key;
    void (*parse) (char *p, pzmeta_t * meta, pzcomment_t * c);
    size_t off;
};


void InterpolateArrays(double *freqs, int nfreqs, double *tmpRe, double *tmpIm,
                       int nfreq, double *xre, double *xim);
void deblank(char *strg);
int EvrespGateway(int nfreq, double delfrq, double xre[], double xim[],
                  float *nmScale, char *inFile);
void acc(int nfreq, double delfrq, double xre[], double xim[]);
void exchange(double **matrix, int kdx, int mdx, int order);
int p1norm0(double **matrix, int kdx, int order);
int balance(double **matrix, int order);
void bbdisp(int nfreq, double delfrq, double xre[], double xim[]);
void bbvel(int nfreq, double delfrq, double xre[], double xim[]);
void benbog(int nfreq, double delfrq, double xre[], double xim[]);
void ckinst(float fp[], int lfp[], int ip[], int lip[], char *kp, int kp_s,
            int lkp[], int *nerr);
void clh(int nfreq, double delfrq, double xre[], double xim[]);
void clz(int nfreq, double delfrq, double xre[], double xim[]);
void cmh(int nfreq, double delfrq, double xre[], double xim[]);
void cmz(int nfreq, double delfrq, double xre[], double xim[]);
void csh(int nfreq, double delfrq, double xre[], double xim[]);
void csz(int nfreq, double delfrq, double xre[], double xim[]);
void dcpft(double re[], double im[], int nfreq, int incp, int isignp);
void dewit(float data[], int nsamps, int order, float a[], char *errmsg);
void dseis(int nfreq, double delfrq, double xre[], double xim[], float fp[],
           int ip[], char kp[MAXKP][MCPFN + 1], int kp_s, float *nmScale,
           int *nerr);
void dss(int nfreq, double delfrq, double xre[], double xim[]);
void dwwssn(int nfreq, double delfrq, double xre[], double xim[]);
void ekalp6(int nfreq, double delfrq, double xre[], double xim[]);
void ekasp2(int nfreq, double delfrq, double xre[], double xim[]);
void elmag(int nfreq, double delfrq, double xre[], double xim[], double freepd,
           double mag, int *nerr);
void eyeomg(int nfreq, double delfrq, double xre[], double xim[], int nzer);
void gbalp(int nfreq, double delfrq, double xre[], double xim[]);
void gbasp(int nfreq, double delfrq, double xre[], double xim[]);
void general(int nfreq, double delfrq, double xre[], double xim[], int nzer,
             double t0, double h, double const_);
void getins(char *kinstr, int kinstr_s, int ninstr, int *ldone, float fp[],
            int lfp[], int ip[], int lip[], char *kp, int kp_s, int lkp[],
            int *nerr);
void getran(int nfreq, double delfrq, double const_, int nzero, complexf zero[],
            int npole, complexf pole[], double xre[], double xim[]);
void getrand(int nfreq, double delfrq, double const_, int nzero, complexd zero[],
            int npole, complexd pole[], double xre[], double xim[]);
void getroots(float in[], int order, struct roots theseRoots[], int *nerr);
void gsref(int nfreq, double delfrq, double xre[], double xim[]);
void hfslpwb(int nfreq, double delfrq, double xre[], double xim[]);
void hqr(double **matrix, int order, struct roots theseRoots[], int *nerr);
void hs3(int nfreq, double delfrq, double xre[], double xim[]);
void iniicm(void);
void lll(int nfreq, double delfrq, double xre[], double xim[], char *subtyp,
         int subtyp_s, double freepd, double damp, int *nerr);
void llsn(int nfreq, double delfrq, double xre[], double xim[]);
void lrsmlp(int nfreq, double delfrq, double xre[], double xim[]);
void lrsmsp(int nfreq, double delfrq, double xre[], double xim[]);
void noress(int nfreq, double delfrq, double xre[], double xim[], char *subtyp,
            int subtyp_s);
void noresshf(int nfreq, double delfrq, double xre[], double xim[]);
void oldbb(int nfreq, double delfrq, double xre[], double xim[]);
void oldkir(int nfreq, double delfrq, double xre[], double xim[]);
void polezero(int nfreq, double delfrq, double xre[], double xim[],
              char *subtyp, int subtyp_s, int *nerr);
void portable(int nfreq, double delfrq, double xre[], double xim[],
              double freepd, double damp, double crfrq);
void predfl(float data[], int npts, float a[], int nc, float result[],
            char *errmsg);
void prewit(float data[], int nsamps, float delta, int *order, float array[],
            char *kprefix, char *errmsg);
void ptbllp(int nfreq, double delfrq, double xre[], double xim[]);
void redkir(int nfreq, double delfrq, double xre[], double xim[]);
void reftek(int nfreq, double delfrq, double xre[], double xim[], double freepd,
            double damp, double crfrq, double hpfrq);
void rs7(int nfreq, double delfrq, double xre[], double xim[], char *subtyp);
void rsk(int nfreq, double delfrq, double xre[], double xim[], char *subtyp);
void rsl(int nfreq, double delfrq, double xre[], double xim[]);
void rsm(int nfreq, double delfrq, double xre[], double xim[]);
void rstn(int nfreq, double delfrq, double xre[], double xim[], char *subtyp,
          int subtyp_s, int *nerr);
void sandia(int nfreq, double delfrq, double xre[], double xim[], char *subtyp,
            int subtyp_s, int *nerr);
void snla3(int nfreq, double delfrq, double xre[], double xim[]);
void sro(int nfreq, double delfrq, double xre[], double xim[], char *subtyp,
         int subtyp_s);
double taper_spectra(double freq, double fqh, double fql);
int NearestInt(double D);
int IsNormalized(double calper, int nfreq, double delfrq, const double *xre,
                 const double *xim);
double GetNormalizationFactor(int nfreq, double delfrq, const double *xre,
                              const double *xim);
void transfer(float dat[], int npts, double delta, float fpfrom[], int ipfrom[],
              char kpfrom[MAXKP][MCPFN + 1], int kpfrom_s, float fpto[],
              int ipto[], char kpto[MAXKP][MCPFN + 1], int kpto_s, double f[],
              int *iprew, double sre[], double sim[], int nfft, double xre[],
              double xim[], int nfreq, int *nerr);
void vel(int nfreq, double delfrq, double xre[], double xim[]);
void wa(int nfreq, double delfrq, double xre[], double xim[]);
void wabn(int nfreq, double delfrq, double xre[], double xim[]);
void wiech(int nfreq, double delfrq, double xre[], double xim[]);
void wwlpbn(int nfreq, double delfrq, double xre[], double xim[]);
void wwsp(int nfreq, double delfrq, double xre[], double xim[]);
void wwspbn(int nfreq, double delfrq, double xre[], double xim[]);
void xicmc(int index, int *nerr);
void xprewit(int *nerr);
void xtransfer(int *nerr);
void ykalp(int nfreq, double delfrq, double xre[], double xim[]);
void ykasp(int nfreq, double delfrq, double xre[], double xim[]);

void dbaseResponse(int a, double b, double *c, double *d, float *e, int *nerr);
void DisconnectFromOracleTransfer(void);
void getSensorInstrumentCalibInfo(double *ncalper, double *ncalib,
                                  double *calper, double *calratio);
void frequency_amplitude_phase(int nf, double df, double *xre, double *xim,
                               char *file, int file_s, int *nerr);
void ztransfer(float *dat, int npts, double delta, double *sre, double *sim,
               double *xre, double *xim, int nfreq, int nfft, double delfrq,
               double *F);

#define FFT_FORWARD  -1
#define FFT_BACKWARD  1

pzmeta_t * polezero_meta_new();
void polezero_comment_parse(char *line, pzmeta_t * meta);
int polezero_is_correct_block(pzmeta_t * meta, datetime * filetime, char *stat,
                              char *net, char *loc, char *chan);
pzmeta_t * polezero_meta_copy(pzmeta_t * m);
void polezero_meta_free(pzmeta_t * meta);

#endif /* _ICM_H_ */
