
#ifndef __CODA_H__
#define __CODA_H__

#include "complex.h"

/* needs to be in an include file */
#define NPMAX 6
#define MAXDATA 1000000
#define MAXBANDS 50
#define SCRATCH_SIZE 100000
#define NPTS_GF 5000
#define START_GF 20
#define DELTA_GF 1.0
#define START_WINDOW_ADD 20
#define MAX_RESIDUAL .10
#define DIST_CRITICAL 350.0
#define GAMMA 1.0
#define MAX_MOMENT_INDEX 5
#define MW_MULTIPLIER 0.66666
#define MW_SUBTRACT 10.7
#define FIT_FUNC_MULT 0.000001
#define FIT_FUNC_SUB  100
#define FIT_INITIAL_LENGTH 200.
#define FIT_NPTS_POW 1.4
#define NOISE_START 0.0
#define NOISE_LENGTH 100.0
#define MINIMUM_SNR 0.0
#define YVPMAX 30.0
#define YVPMIN 0.0

struct envelope{
     float       freq_low;
     float       freq_high;
     float       *envelope_data;
     int         number_of_points;
     int         window_start;
     int         window_stop;
     float       window_start_seconds;
     float       coda_amplitude;
     float       *GFenvelope;
     float       GFstart;
     int         GFnpts;
     float       GFdelta;
     float       b0;
     float       vel0;
     float       vel1;
     float       vel2;
     float       vel_int;
     float       vel_slope;
     float       dist_critical;
     float       gamma;
     float       min_length;
     float       minimum_SNR;
     float       max_residual;
     float       noise_thresh;
     float       b_slope;
     float       dist_slope;
     float       dist_order1;
     float       dist_order2;
     float       Moment_correction;
     int         fit_npoints;
     int         fit_window_picked;
     float       CodaAmp;
     float       fit_residual;
     float       fit_SNR;
     float       fit_b0;
     float       fit_gamma;
     float       Moment;
     float       Mb_weight;
     char        ResidStat[2];
     char        SNRStat[2];
   };

struct global_params{
     float Mb_scale;
     float Mb_constant;
     float Moment;
     float Mw;
     float Mb;
     float Energy_Obs;
     float Energy_High;
     float Energy_Low;     
     float Energy_Total;
     float Stress_Total;
     float Max_Residual;
     float Minimum_SNR;
   };

/* C_apply.c */
void C_apply(float data[], int nsamps, int zp, float sn[], float sd[], int nsects);
/* C_bilin2.c */
void C_bilin2(float sn[], float sd[], int nsects);
/* C_buroots.c */
void C_buroots(complexf p[], char *rtype, int rtype_s, float *dcvalue, int *nsects, int iord);

/* C_cutoffs.c */
void C_cutoffs(float sn[], float sd[], int nsects, double f);
/* C_design.c */
void C_design(int iord, char *type, char *aproto, double a, double trbndw, double fl, double fh, double ts, float sn[], float sd[], int *nsects);
/* C_fft.c */
void C_fft(float xreal[], float ximag[], int n, int idir);
/* C_firtrn.c */
void C_firtrn(char *ftype, float x[], int n, float buffer[], float y[]);
/* C_fstrncpy.c */
char *C_fstrncpy(char *to, int tolen, char *from, int fromlen);
/* C_lp.c */
void C_lp(complexf p[], complexf z[], char *rtype, int rtype_s, double dcvalue, int nsects, float sn[], float sd[]);
/* C_lptbp.c */
void C_lptbp(complexf p[], complexf z[], char *rtype, int rtype_s, double dcvalue, int *nsects, double fl, double fh, float sn[], float sd[]);
/* C_lptbr.c */
void C_lptbr(complexf p[], complexf z[], char *rtype, int rtype_s, double dcvalue, int *nsects, double fl, double fh, float sn[], float sd[]);
/* C_lpthp.c */
void C_lpthp(complexf p[], complexf z[], char *rtype, int rtype_s, double dcvalue, int nsects, float sn[], float sd[]);
/* C_overlp.c */
void C_overlap(float input[], int npts, float output[], float c[], int nc, int nfft, float buffer[], float cbuff[]);
/* C_warp.c */
float C_warp(double f, double ts);
/* C_wigint.c */
void C_wigint(float x[], float y[], int npts, double dx, double eps, double t, float *f);
/* C_xapiir.c */
void C_xapiir(float data[], int nsamps, char *aproto, double trbndw, double a, int iord, char *type, double flo, double fhi, double ts, int passes);
/* C_zshft.c */
void C_zshft(float signal[], int n, int ishft);
/* binary_op.c */
int binary_op(float *data1, float *data2, int n, char *op);
/* calc_codaGF.c */
void calc_codaGF(struct envelope *env, double dist);
/* calc_coda_amplitudes.c */
void calc_coda_amplitudes(struct envelope envelopes[50], int nbands, double dist, double begin, char *evid, int lcalibrate);
/* calc_energy.c */
void calc_energy(struct envelope envelopes[50], int nbands, char *evid, int lcalibrate, struct global_params *global_params);
/* calc_envelopes.c */
void calc_envelopes(float *data1, float *data2, int ndata, int horizontals, struct envelope envelopes[MAXBANDS], int *nbands, float begin, float delta, int npts, float dist, char *evid);
/* calc_moment_magnitude.c */
void calc_moment_magnitude(struct envelope envelopes[50], int nbands, struct global_params *global_params);
/* fit_coda_amp.c */
void fit_coda_amp(struct envelope *Passed_envelopes, float abegin);
void amoeba(float p[NPMAX+1][NPMAX], float y[NPMAX], int mp, int np, float ftol);
float ampresid(float xx[6]);
float fit_amplitude_only(float ftol);
/* fit_coda_params.c */
void fit_coda_params(struct envelope *Passed_envelopes, float abegin, float dist);
void amoebap(float p[NPMAX+1][NPMAX], float y[NPMAX], int mp, int np, float ftol);
float ampresidp(float xx[6]);
/* get_input.c */
int get_input(char *inputfile, struct envelope envelopes[50], int *nbands, struct global_params *global_params);
/* pickwindows.c */
void pickwindows(struct envelope envelopes[50], int nbands, float C_begin, int *nerr);
/* plotspec.c */
void plotspec(struct envelope envelopes[50], int nbands, double C_begin, int *nerr);
/* send_output.c */
int send_output(struct envelope envelopes[50], int nbands, char *evid, int lcalibrate, struct global_params *global_params);
/* unary_op.c */
int unary_op(float *data, int *npts, char *op, double arg, float *delta);
/* xcoda.c */
void xcoda(int index, int *nerr);


#endif /* __CODA_H__ */
