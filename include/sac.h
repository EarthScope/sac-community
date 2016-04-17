/** 
 * @file sac.h
 * 
 * @brief SAC Library Routines
 * 
 */

#ifndef __SAC_H__
#define __SAC_H__

/** 
 * IIR Filter Prototypes 
 *
 * @see xapiir
 */
#define SAC_BUTTERWORTH       "BU"
#define SAC_BESSEL            "BE"
#define SAC_CHEBYSHEV_TYPE_I  "C1"
#define SAC_CHEBYSHEV_TYPE_II "C2"

/** 
 * IIR Filter Types 
 *
 * @see xapiir
 */
#define SAC_BANDPASS          "BP"
#define SAC_HIGHPASS          "HP"
#define SAC_LOWPASS           "LP"
#define SAC_BANDREJECT        "BR"

/** 
 * FIR Filter Type 
 *
 * @see firtrn
 */
#define SAC_HILBERT           "HILBERT"
#define SAC_DERIVATIVE        "DERIVATIVE"

/** 
 * Window Type
 * 
 * @see crscor, window
 */
#define SAC_HAMMING           "HAMMING"
#define SAC_HANNING           "HANNING"
#define SAC_RECTANGLE         "RECTANGLE"
#define SAC_COSINE            "COSINE"
#define SAC_TRIANGULAR        "TRIANGULAR"

/** 
 * Filter
 *   
 *   IIR (Infinte Impulse Response) filter and is the same 
 *   that is used in lowpass, highpass, bandpass and bandreject. 
 */
void xapiir ( float      data[],  
              int        nsamps, 
              char      *aproto,  
              double     trbndw, 
              double     a, 
              int        iord, 
              char      *type, 
              double     flo, 
              double     fhi, 
              double     ts, 
              int        passes);

/** 
 * Compute the envelope of a function
 *
 * @param n - Length of \p in and \p out
 * @param in - Input data series
 * @param out - Output data series
 *
 */
void envelope(int        n, 
              float     *in, 
              float     *out);

/** 
 * Calculate the Hilbert Transform or derivative of a signal
 *   with a FIR filter.  Currently uses a 201 point filter 
 *   constructed by windowing the ideal impulse response
 *   with a hamming window.
 */
void firtrn(char     *ftype, 
            float     x[], 
            int       n, 
            float     buffer[], 
            float     y[]);


/* Compute the Cross-Correlation Function */
void crscor(float     data1[], 
            float     data2[], 
            int       nsamps, 
            int       nwin, 
            int       wlen, 
            char     *type, 
            float     c[], 
            int      *nfft, 
            char     *err, 
            int       err_s);


/* Find the next largest power of two greater than num */
int next2(int num);

/* Add and/or remove an instrument response  */
void ztransfer(float *dat, int npts, double delta, double *sre, double *sim,
               double *xre, double *xim, int nfreq, int nfft, double delfrq,
               double *F);

/* Compute response from poles and zero */
void getrand(int nfreq, double delfrq, double const_, int nzero, complexd zero[],
             int npole, complexd pole[], double xre[], double xim[]);

/* Determine trend of even and unevenly spaced data */
void lifite(double x1, double dx, float y[], int n, float *a, float *b,
            float *siga, float *sigb, float *sig, float *cc);
void lifitu(float x[], float y[], int n, float *a, float *b, float *siga,
            float *sigb, float *sig, float *cc);

/* Remove trend from even and unevely spaced data */
void rtrend(float *data, int n, float yint, float slope, float b, float delta);
void rtrend2(float *data, int n, float yint, float slope, float *t);

/* Remove mean from data*/
void rmean(float *data, int n, float mean);

/* Interpolate even and unevely spaced data */
void interp(float *in, int nlen, float *out, int newlen, float bval, float eval,
            float dt, float tstart, float dtnew, float eps);
void interp2(float *in, int nlen, float *out, int newlen, float bval,
             float eval, float *t, float tstart, float dtnew, float eps);
float geteps_xy(float y[], int nlen, float x[]);
float geteps(float y[], int nlen, float dx);

/* Integrate and Differentiate */
enum {
    SAC_INT_TRAPEZODIAL = 1,
    SAC_INT_RECTANGULAR = 2,
};
enum {
    SAC_DIFF_TWO_POINT   = 2,
    SAC_DIFF_THREE_POINT = 3,
    SAC_DIFF_FIVE_POINT  = 5,
};

void int_trap(float *y, int npts, double delta);
void int_rect(float *y, int npts, double delta);
void dif2(float *array, int number, double step, float *output);
void dif3(float *array, int number, double step, float *output);
void dif5(float *array, int number, double step, float *output);

/* icm.h */
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


pz_t * polezero_parse(char *filename, station_id_t *stat);
station_id_t * station_id_from_sac(sac *s);
void polezero_free(pz_t *pz);

/* scm.h */
void lifite(double x1, double dx, float y[], int n, float *a, float *b,
            float *siga, float *sigb, float *sig, float *cc);
void lifitu(float x[], float y[], int n, float *a, float *b, float *siga,
            float *sigb, float *sig, float *cc);
void rtrend(float *data, int n, float yint, float slope, float b, float delta);
void rtrend2(float *data, int n, float yint, float slope, float *t);
void rmean(float *data, int n, float mean);
void interp(float *in, int nlen, float *out, int newlen, float bval, float eval,
            float dt, float tstart, float dtnew, float eps);
void interp2(float *in, int nlen, float *out, int newlen, float bval,
             float eval, float *t, float tstart, float dtnew, float eps);

void cut(float *in, int nstart, int nstop, int nfillb, int nfille, float *out);
void cut_define(float b, float delta, double dt, int *n);

/**
 * cuterr
 * Options for options cuterr in function cut_define_check
 *
 */
enum {
    CUT_FILLZ = 3,
    CUT_USEBE = 2,
    CUR_FATAL = 1,
};

/**
 *
 */
void cut_define_check(float start, float stop, int npts, int cuterr, int *nstart,
                      int *nstop, int *nfillb, int *nfille, int *nerr);


void rotate(float si1[], float si2[], int ns, double angle, int lnpi, int lnpo,
            float so1[], float so2[]);


#endif /* __SAC_H__ */
