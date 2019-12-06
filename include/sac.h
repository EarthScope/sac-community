/**
 * @file sac.h
 *
 * @brief SAC Library Routines
 *
 */

#ifndef __SAC_H__
#define __SAC_H__

#include <complex.h>
#include <sacio.h>

/**
 * IIR Filter Prototypes
 *
 * @see xapiir
 */
enum FilterPrototype {
    SAC_BUTTERWORTH = 0,
    SAC_BESSEL,
    SAC_CHEBYSHEV_I,
    SAC_CHEBYSHEV_II,
};

/**
 * IIR Filter Types
 *
 * @see xapiir
 */
enum FilterType {
    SAC_BANDPASS = 0,
    SAC_HIGHPASS,
    SAC_LOWPASS,
    SAC_BANDREJECT,
};

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
 * Taper Types
 *
 */
enum {
    SAC_TAPER_COSINE  = 1,
    SAC_TAPER_HANNING = 2,
    SAC_TAPER_HAMMING = 3,
};


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

void filter(enum FilterPrototype prototype, enum FilterType type,
            float *data, int n, float dt,
            float low, float high, int passes, int order,
            float transition,float attenuation);
void bandpass(float *data, int n, float dt, float low, float high);
void lowpass (float *data, int n, float dt, float corner);
void highpass(float *data, int n, float dt, float corner);


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
int correlate_max(float *c, int nc);
float correlate_time(float dt, float b, int i);
float * correlate_time_array(float dt, float b, int n);
float correlate_time_begin(float dt, float n1, float _n2, float b1, float b2);
void correlate(float *f, int nf, float *g, int ng, float *c, int nc);

void convolve(float *a, int na, float *b, int nb, float *c, int nc);

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
void rtrend(float *data, int n, float yint, float slope, double b, double delta);
void rtrend2(float *data, int n, float yint, float slope, float *t);

void remove_trend(float *data, int n, float delta, float b);

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
    timespec64 ref;
};


//pz_t * polezero_parse(char *filename, station_id_t *stat);
//station_id_t * station_id_from_sac(sac *s);
//void polezero_free(pz_t *pz);

void sac_reference_time(char *when);
void sac_station_id_to_polezero(char *id, char *pzfile);
void sac_station_id(char *id);
void sac_station_id_split(char *id, char *net, char *sta, char *loc, char *cha);
double * fftfreq(int n, double f0, double df);
double dfreq(int n, double dt);

int remove_polezero(float *data, int n, float dt, double limits[4],
                    char *id, char *when, char *pzfile);
int remove_polezero_simple(float *data, int n, float dt, double limits[4]);


/* Fourier Transform */

/* Forward Real/Imaginary, Single precision */
void fft(float *data, int n, float *re, float *im, int *nf);
/* Forward Complex, Single precision */
void fftz(float *data, int n, float complex *z, int *nf);

/* Inverse Real/Imaginary, Single precision */
void ifft(float *data, int n, float *re, float *im, int nf);
/* Inverse Complex, Single precision */
void ifftz(float *data, int n, float complex *z, int nf);

/* Forward Real/Imginary, Double precision */
void dfft(double *data, int n, double *re, double *im, int *nf);
/* Forward Complex, Double precision */
void dfftz(double *data, int n, double complex *z, int *nf);

/* Inverse Real/Imaginary, Double precision */
void idfft(double *data, int n, double *re, double *im, int nf);
/* Inverse Complex, Double precision */
void idfftz(double *data, int n, double complex *z, int nf);


/* scm.h */
void lifite(double x1, double dx, float y[], int n, float *a, float *b,
            float *siga, float *sigb, float *sig, float *cc);
void lifitu(float x[], float y[], int n, float *a, float *b, float *siga,
            float *sigb, float *sig, float *cc);
void rmean(float *data, int n, float mean);
void interp(float *in, int nlen, float *out, int newlen, float bval, float eval,
            float dt, float tstart, float dtnew, float eps);
void interp2(float *in, int nlen, float *out, int newlen, float bval,
             float eval, float *t, float tstart, float dtnew, float eps);

void cut_data(float *in, int nstart, int nstop, int nfillb, int nfille, float *out);
void cut_define(float b, float delta, double dt, int *n);
void cut(float *in, int npts, float b, float dt,
         float begin_cut, float end_cut, int cuterr,
         float *out, int *nout);
void cutd(float *in, int npts, double b, double dt,
          double begin_cut, double end_cut, int cuterr,
          float *out, int *nout);


void  remove_mean(float *data, int n);
float compute_mean(float *data, int n);

/**
 * cuterr
 * Options for options cuterr in function cut_define_check
 *
 */
enum {
    SAC_CUT_FILLZ = 3,
    SAC_CUT_USEBE = 2,
    SAC_CUT_FATAL = 1,
};

/**
 *
 */
void cut_define_check(float start, float stop, int npts, int cuterr, int *nstart,
                      int *nstop, int *nfillb, int *nfille, int *nerr);


void rotate(float si1[], float si2[], int ns, double angle, int lnpi, int lnpo,
            float so1[], float so2[]);


void taper_width_to_points(float width, int npts, int *ipts);
void taper_points(float *data, int n, int taper_type, int ipts);

void taper_seconds(float *data, int n, int taper_type, float sec, float delta);
void taper_width(float *data, int n, int taper_type, float width);


int isclosed      (double a, double b);
int isclosed_par  (double a, double b, double atol, double rtol);
int isclose       (float a, float b);
int isclose_par   (float a, float b, double atol, double rtol);
int allclosef     (float *a, float *b, int n);
int allclosef_par (float *a, float *b, int n, double atol, double rtol);


#endif /* __SAC_H__ */
