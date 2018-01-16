/** 
 * @file   xapiir.c
 * 
 * @brief  IIR Filter Design and Implmentation
 * 
 */
#include <string.h>

#include "dbh.h"
#include "bool.h"

/** 
 *  IIR filter design and implementation
 * 
 * @param data 
 *    real array containing sequence to be filtered
 *    original data destroyed, replaced by filtered data
 * @param nsamps 
 *    number of samples in data
 * @param aproto 
 *    character*8 variable, contains type of analog prototype filter
 *      - '(BU)tter  ' -- butterworth filter
 *      - '(BE)ssel  ' -- bessel filter
 *      - 'C1      ' -- chebyshev type i
 *      - 'C2      ' -- chebyshev type ii
 * @param trbndw 
 *    transition bandwidth as fraction of lowpass
 *    prototype filter cutoff frequency.  used
 *    only by chebyshev filters.
 * @param a 
 *    attenuation factor.  equals amplitude
 *    reached at stopband edge.  used only by
 *    chebyshev filters.
 * @param iord 
 *    order (#poles) of analog prototype
 *    not to exceed 10 in this configuration.  4 - 5
 *    should be ample.
 * @param type 
 *    character*8 variable containing filter type
 *     - 'LP' -- low pass
 *     - 'HP' -- high pass
 *     - 'BP' -- band pass
 *     - 'BR' -- band reject
 * @param flo 
 *    low frequency cutoff of filter (hertz)
 *    ignored if type = 'lp'
 * @param fhi 
 *    high frequency cutoff of filter (hertz)
 *    ignored if type = 'hp'
 * @param ts 
 *    sampling interval (seconds)
 * @param passes 
 *    integer variable containing the number of passes
 *     - 1 -- forward filtering only
 *     - 2 -- forward and reverse (i.e. zero phase) filtering
 *
 * @author:  Dave B. Harris
 *
 * @date 120990 Last Modified:  September 12, 1990
 */
void
xapiir(float *data, int nsamps, char *aproto, double trbndw, double a, int iord,
       char *type, double flo, double fhi, double ts, int passes) {

    int zp;
    int nsects;
    float sd[30], sn[30];
    char strtemp1[3], strtemp2[3];

    /*  Filter designed  */
    strncpy(strtemp1, type, 2);
    strtemp1[2] = '\0';
    strncpy(strtemp2, aproto, 2);
    strtemp2[2] = '\0';

    design(iord, strtemp1, strtemp2, a, trbndw, flo, fhi, ts, sn, sd, &nsects);

    /*  Filter data  */
    if (passes == 1) {
        zp = FALSE;
    } else {
        zp = TRUE;
    }
    apply(data, nsamps, zp, sn, sd, nsects);

    return;
}

/** 
 *  IIR filter design and implementation
 *     Fortran Interface 
 * 
 * @see xapiir
 *
 */
void
xapiir_(float *data, int *nsamps, char *aproto, double *trbndw, double *a,
        int *iord, char *type, double *flo, double *fhi, double *ts,
        int *passes) {
    xapiir(data, *nsamps, aproto, *trbndw, *a, *iord, type, *flo, *fhi, *ts,
           *passes);
}

/** 
 *  IIR filter design and implementation
 *     Fortran Interface 
 * 
 * @see xapiir
 *
 */
void
xapiir__(float *data, int *nsamps, char *aproto, double *trbndw, double *a,
         int *iord, char *type, double *flo, double *fhi, double *ts,
         int *passes) {
    xapiir(data, *nsamps, aproto, *trbndw, *a, *iord, type, *flo, *fhi, *ts,
           *passes);
}

enum FilterPrototype {
    SAC_BUTTERWORTH = 0,
    SAC_BESSEL,
    SAC_CHEBYSHEV_I,
    SAC_CHEBYSHEV_II
};
enum FilterType {
    SAC_BANDPASS = 0,
    SAC_HIGHPASS,
    SAC_LOWPASS,
    SAC_BANDREJECT,
};


void
filter(enum FilterPrototype prototype,
       enum FilterType type,
       float *data, int n, float dt,
       float low, float high, int passes, int order,
       float transition,
       float attenuation) {
    char proto[4], ftype[4];
    static char *FilterPrototypeKey[] = { "BU", "BE", "C1", "C2" };
    static char *FilterTypeKey[]      = { "BP", "HP", "LP", "BR" };

    strcpy(ftype, FilterTypeKey[type]);
    strcpy(proto, FilterPrototypeKey[prototype]);

    xapiir(data, n, proto, transition, attenuation,
           order, ftype, low, high, (double) dt, passes);
}

#define PASSES 2
#define ORDER  4

void
bandpass(float *data, int n, float dt, float low, float high) {
    filter(SAC_BUTTERWORTH, SAC_BANDPASS, data, n, dt, low, high, PASSES, ORDER, 0., 0.);
}
void
lowpass(float *data, int n, float dt, float corner) {
    filter(SAC_BUTTERWORTH, SAC_LOWPASS, data, n, dt, corner, corner, PASSES, ORDER, 0., 0.);
}
void
highpass(float *data, int n, float dt, float corner) {
    filter(SAC_BUTTERWORTH, SAC_HIGHPASS, data, n, dt, corner, corner, PASSES, ORDER, 0., 0.);
}

void
filter_(int *proto, int *type, float *data, int *n, float *dt,
        float *low, float *high, int *passes, int *order,
        float *trans, float *att) {
    filter(*proto, *type, data, *n, *dt, *low, *high, *passes, *order, *trans, *att);
}
void
filter__(int *proto, int *type, float *data, int *n, float *dt,
         float *low, float *high, int *passes, int *order,
         float *trans, float *att) {
    filter(*proto, *type, data, *n, *dt, *low, *high, *passes, *order, *trans, *att);
}

void
bandpass_(float *data, int *n, float *dt, float *low, float *high) {
    bandpass(data, *n, *dt, *low, *high);
}
void
bandpass__(float *data, int *n, float *dt, float *low, float *high) {
    bandpass(data, *n, *dt, *low, *high);
}
void lowpass_  (float *data, int *n, float *dt, float *co) { lowpass (data, *n, *dt, *co);  }
void lowpass__ (float *data, int *n, float *dt, float *co) { lowpass (data, *n, *dt, *co);  }
void highpass_ (float *data, int *n, float *dt, float *co) { highpass(data, *n, *dt, *co); }
void highpass__(float *data, int *n, float *dt, float *co) { highpass(data, *n, *dt, *co); }
