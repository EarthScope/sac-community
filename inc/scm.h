/** 
 * @file   scm.h
 * 
 * @brief  Signal Correction, Measurement and Processing Module (scmp ?)
 * 
 */

#ifndef _SCM_H_
#define _SCM_H_

#define	MQGAIN	8
#define	MRGLMT	3
#define	MRGLTP	2

/** 
 * @struct cmscm
 *    Signal Correction Variables 
 *
 */
struct t_cmscm {
    double usraz;
    double usrang;
    int lnpreq;
    double rqqcon;
    double rqrcon;
    double rqccon;
    double dtnew;
    float eps;
    int lbreq;
    double breq;
    int lnreq;
    int nreq;
    int iqgain[MQGAIN + 1];
    double qlevel;
    int nqmant;
    int nstrfc;
    int lstrfi;
    int lmean;
    int nhalf;
    int irgltp;
    int irglmt;
    double thold;
    int lrglwin;
    double orglwin[2];
    int ndecfc;
    int ldecfi;
} ;

/** 
 * @struct kmscm
 *    Signal Correction Characters
 *
 */
struct t_kmscm {
    char krottp[9];
    char krgltp[MRGLTP][9];
    char krglmt[MRGLMT][9];
    char krglwin[2][9];
} ;

#define SCM_EXTERN \
    extern struct t_kmscm kmscm; \
    extern struct t_cmscm cmscm;


void iniscm(void);
void lifite(double x1, double dx, float y[], int n, float *a, float *b,
            float *siga, float *sigb, float *sig, float *cc);
void lifitu(float x[], float y[], int n, float *a, float *b, float *siga,
            float *sigb, float *sig, float *cc);
void linear(float segin[], int number, float segout[]);
void rotate(float si1[], float si2[], int ns, double angle, int lnpi, int lnpo,
            float so1[], float so2[]);
void wigint(float x[], float y[], int npts, double dx, double b, double epsi, double t,
            float *f);
void xabsgl(float data[], int nlen, double thold, int irgltp, int *nerr);
void xdecimate(int *nerr);
void xinterpolate(int *nerr);
void xlinefit(int *nerr);
void xpowgl(float data[], int nlen, double sr, double alpha, double doval,
            int irgltp, int *nerr);
void xquantize(int *nerr);
void xreverse(int *nerr);
float R4Mean(float *array, int Nsamples);
float VarianceR4(float *array, int Nlen);
void RglitchR4(float *data, int NPTS);
void xrglitches(int *nerr);
void xrmean(int *nerr);
void xrotate(int *nerr);
void xrq(int *nerr);
void xrtr(int *nerr);
void xscmc(int index, int *nerr);
void xsmooth(int *nerr);
void xstretch(int *nerr);
void lpdesign(int irate, int n, float c[], int *nc);
void inter(float x[], int nx, int irate, float c[], int nc, float y[], int *ny);
void xtaper(int *nerr);

void taper_width_to_points(float width, int npts, int *ipts);
void taper_points(float *data, int n, int taper_type, int ipts);

void rmean(float *data, int n, float mean);
void rtrend(float *data, int n, float yint, float slope, double b, double delta);
void rtrend2(float *data, int n, float yint, float slope, float *t);

void interp(float *in, int nlen, float *out, int newlen, double bval, double eval,
            double dt, double tstart, double dtnew, float eps);
void interp2(float *in, int nlen, float *out, int newlen, double bval,
             double eval, float *t, double tstart, double dtnew, float eps);
void rotinc(int *nerr);
#endif /* _SCM_H_ */
