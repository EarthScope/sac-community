
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ucf.h"
#include "icm.h"
#include "co.h"
#include "bool.h"

#include <complex.h>

#define FFT_FORWARD -1
#define FFT_INVERSE  1


void
dcpft(re, im, nfreq, incp, isignp)
     double re[], im[];
     int nfreq, incp, isignp;
{
    int ij, inc, is, it, ji, k0, k1, n1, n2, ninc, rc, span;
    double c, c0, i0, i1, r0, r1, s, s0, sgn, t;

    int alloc;

    static double *sines;
    static double *Sines;

    static int first = 1;
    static int mpow2 = 0;

    double *const Im = &im[0] - 1;
    double *const Re = &re[0] - 1;

    /* DOUBLE PRECISION VERSION.
     *  FORTRAN TRANSLITERATION OF SINGLETON'S 6600 ASSEMBLY-CODED FFT.
     *  INTENDED TO BE OF ASSISTANCE IN UNDERSTANDING HIS CODE, AND IN
     *  FUTURE WRITING OF AN FFT FOR ANOTHER MACHINE.
     *  IT SHOULD BE TRANSLATED INTO MACHINE CODE RATHER THAN USED FOR
     *  PRODUCTION AS IS BECAUSE- IT IS VERSATILE AND EFFICIENT ENOUGH TO
     *  SEE LOTS OF USE, IT BENEFITS GREATLY FROM CAREFUL HAND CODING, AND IS
     *  SHORT AND SIMPLE ENOUGH TO DO QUICKLY.
     *  A. BRUCE LANGDON, M DIVISION, L.L.L., 1971.
     *
     *  COMMENTS BELOW ARE MOSTLY FROM 6600-7600 VERSION.
     *  R      REAL PART OF DATA VECTOR.
     *  I      IMAG PART OF DATA VECTOR.
     *  N      NUMBER OF ELEMENTS (=1,2,4,8...32768).
     *  INC    SPACING IN MEMORY OF DATA (USUALLY 1, BUT SEE BELOW).
     *  SIGN   ITS SIGN WILL BE SIGN OF ARGUMENT IN TRANSFORM EXPONENTIAL.
     *
     *    ON ENTRY ARRAYS R AND I CONTAIN THE SEQUENCE TO BE TRANSFORMED.
     *  ON EXIT THEY CONTAIN THE TRANSFORM. INPUT AND OUTPUT SEQUENCES ARE
     *  BOTH IN NATURAL ORDER (I.E. NOT BIT-REVERSED SCRAMBLED).
     *
     *    A CALL TO CPFT WITH SIGN=+1, FOLLOWED BY ANOTHER CALL WITH THE
     *  FIRST 4 PARAMETERS THE SAME AND SIGN=-1, WILL LEAVE R AND I WITH
     *  THEIR ORIGINAL CONTENTS TIMES N. THE SAME IS TRUE IF FIRST SIGN=-1,
     *  AND NEXT SIGN=+1.
     *
     *    THE USEFULNESS OF PARAMETER INC MAY BE ILLUSTRATED BY 2 EXAMPLES:
     *    SUPPOSE THE COMPLEX SEQUENCE IS STORED AS A FORTRAN COMPLEX ARRAY
     *  Z, I.E. REAL AND IMAGINARY PARTS IN ALTERNATE MEMORY CELLS. THE
     *  SEPARATION BETWEEN CONSECUTIVE REAL (OR IMAGINARY) ELEMENTS IS 2
     *  WORDS, SO INC=2. THE CALL MIGHT BE
     *          CALL CPFT(REAL(Z), AIMAG(Z), N, 2, SIGN)
     *  FOR MANY COMPILERS ONE WOULD INSTEAD HAVE TO DO SOMETHING LIKE
     *          CALL CPFT(RI, RI(2), N, 2, SIGN)
     *  WHERE RI IS A REAL ARRAY EQUIVALENCED TO Z.
     *    SUPPOSE ONE HAD AN ARRAY C WITH DIMENSIONS N1, N2. ONE WANTS R TO
     *  BE ROW I1 AND I TO BE ROW I2. THE SEPARATION OF CONSECUTIVE ELEMENTS
     *  IS N1 AND STARTING ADDRESSES ARE C(I1,1) AND C(I2,1), SO USE
     *          CALL CPFT(C(I1,1), C(I2,1), N2, N1, SIGN)
     *
     *  TIMING, ASSUMING MINIMAL MEMORY BANK CONFLICTS:
     *    6400 TIME FOR N=1024 IS 220,000=21.5*N*LOG2N MICROSECONDS.
     *    6600 TIME FOR N=1024 IS  44,500=4.35*N*LOG2N MICROSECONDS.
     *    7600 TIME FOR N=1024 IS   8,300=0.81*N*LOG2N MICROSECONDS.
     *
     *    A RADIX 2 FFT PROVOKES MEMORY BANK CONFLICTS AT BEST, BUT TIMING
     *  IS NOTICEABLY WORSENED WHEN LIKE ELEMENTS OF R AND I ARE IN THE SAME
     *  BANK AND/OR INC IS A MULTIPLE OF A POWER OF 2. IN A WORST CASE ON
     *  THE 7600 THE SPEED WAS DECREASED BY A FACTOR OF 3.
     *    THUS IN THE EXAMPLE ABOVE, IF N1=MULTIPLE OF 32 ONE MIGHT
     *  DECIDE TO WASTE A LITTLE MEMORY BY INCREASING N1 TO 33, THUS
     *  DECREASING CONFLICTS FOR TRANSFORMS OVER ROWS OR OVER COLUMNS.
     *
     *  WRITTEN BY R. C. SINGLETON, STANFORD RESEARCH INSTITUTE, NOV. 1968.
     *  COMMENTARY, LRL LINKAGE AND OTHER MINOR CHANGES BY A. BRUCE LANGDON
     *  LAWRENCE RADIATION LABORATORY, LIVERMORE, APRIL 1971.
     *
     *  REFERENCES:
     *    (1) R. C. SINGLETON, 'ON COMPUTING THE FAST FOURIER TRANSFORM',
     *        COMM. ASSOC. COMP. MACH. VOL. 10, PP. 647-654 (1967).
     *    (2) R. C. SINGLETON, ALGORITHM 345 'AN ALGOL CONVOLUTION PROCEDURE
     *        BASED ON THE FAST FOURIER TRANSFORM', COMM. ACM VOL. 12,
     *        PP. 179-184 (1969).
     *    (3) W. M. GENTLEMAN AND G. SANDE, 'FAST FOURIER TRANSFORMS - FOR
     *        FUN AND PROFIT', PROC. AFIPS 1966 FALL JOINT COMPUTER CONF.,
     *        VOL. 29, PP. 563-578.
     * */

    /*  TABLE OF SINES.
     *    THESE SHOULD BE GOOD TO THE VERY LAST BIT. THEY ARE GIVEN IN OCTAL
     *  TO PREVENT AN ASSEMBLER FROM CONVERTING THEM POORLY. THEY MAY BE
     *  OBTAINED BY EVALUATING THE INDICATED SINES IN DOUBLE PRECISION AND
     *  PUNCHING THEM OUT IN OCTAL FORMAT (A SINGLE PRECISION SINE ROUTINE
     *  IS NOT ACCURATE ENOUGH). USE THE MOST SIGNIFICANT WORD, ROUNDED
     *  ACCORDING TO THE LEAST SIGNIFICANT WORD.
     *  IN THIS VERSION I DO IT A LAZY WAY ON THE FIRST CALL. */

    s = 0;
    c = 0;
    s0 = 0;
    c0 = 0;
    k0 = 0;

    if (nfreq == 1)
        return;

    alloc = FALSE;

    while (ipow(2, mpow2) < nfreq) {
        mpow2 += 1;
        alloc = TRUE;
    }

    if (alloc) {
        if (!first) {
            free(sines);
        }
        first = 0;
        if ((sines = (double *) malloc(mpow2 * sizeof(double))) == NULL) {
            printf("error allocating memory-dcpft\n");
            return;
        }
        memset(sines, 0, mpow2 * sizeof(double));
        Sines = sines - 1;
        Sines[1] = 1.0e0;
        t = atan(1.0e0);
        for (is = 2; is <= mpow2; is++) {
            Sines[is] = sin(t);
            t = t / 2.0e0;
        }
    }
    if (!Sines) {
        fprintf(stderr, "Error accessing memory in dcpft\n");
        return;
    }

    /*  SET UP VARIOUS INDICES.
     * */
    inc = incp;
    sgn = isignp;
    ninc = nfreq * inc;
    span = ninc;
    it = nfreq / 2;
    for (is = 1; is <= mpow2; is++) {
        if (it == 1)
            goto L_12;
        it = it / 2;
    }

    /*    THERE ARE 2 INNER LOOPS WHICH RUN OVER THE N/(2*SPAN) REPLICATIONS
     *  OF TRANSFORMS OF LENGTH (2*SPAN). THESE LOOPS FIT INTO THE
     *  INSTRUCTION STACK OF THE 6600 OR 7600. ONE LOOP IS FOR ARBITRARY
     *  ROTATION FACTOR ANGLE. THE OTHER TAKES CARE OF THE SPECIAL CASE IN
     *  WHICH THE ANGLE IS ZERO SO THAT NO COMPLEX MULTIPLICATION IS NEEDED.
     *  THIS IS MORE EFFICIENT THAN TESTING AND BRANCHING INSIDE THE INNER
     *  LOOP, AS IS OFTEN DONE. THE OTHER SPECIAL CASE IN WHICH NO COMPLEX
     *  MULTIPLY IS NEEDED IS ANGLE=PI (I.E. FACTOR=I); THIS IS NOT HANDLED
     *  SPECIALLY. THESE MEASURES ARE MOST HELPFUL FOR SMALL N.
     *
     *    THE ORGANIZATION OF THE RECURSION IS THAT OF SANDE (REF. (3),
     *  PP. 566-568). THAT IS, THE DATA IS IN NORMAL ORDER TO START AND
     *  SCRAMBLED AFTERWARD, AND THE EXPONENTIAL ROTATION ('TWIDDLE') FACTOR
     *  ANGLES ARE USED IN ASCENDING ORDER DURING EACH RECURSION LEVEL.
     *  ALL THE SINES AND COSINES NEEDED ARE GENERATED FROM A SHORT TABLE
     *  USING A STABLE MULTIPLE-ANGLE RECURSION (REF. (1), P651 AND REF. (2),
     *  PP. 179-180). THIS METHOD IS ECONOMICAL IN STORAGE AND TIME, AND
     *  YIELDS ACCURACY COMPARABLE TO GOOD LIBRARY SIN-COS ROUTINES.
     *  ANGLES BETWEEN 0 AND PI ARE NEEDED. THE RECURSION IS USED FOR
     *  ANGLES UP TO PI/2; LARGER ANGLES ARE OBTAINED BY REFLECTION IN THE
     *  IMAGINARY AXIS (ANGLE:=PI-ANGLE). THESE PAIRS OF ANGLES ARE USED
     *  ONE RIGHT AFTER THE OTHER.
     *
     *    FOR SIMPLICITY, COMMENTARY BELOW APPLIES TO INC=1 CASE.
     *
     *  IF TRUNCATED RATHER THAN ROUNDED ARITHMETIC IS USED, SINGLETON'S
     *  MAGNITUDE CORRECTION SHOULD BE APPLIED TO C AND S.
     * */
  L_10:
    t = s + (s0 * c - c0 * s);
    c = c - (c0 * c + s0 * s);
    s = t;

    /*  REPLICATION LOOP. */
  L_11:
    k1 = k0 + span;
    r0 = Re[k0 + 1];
    r1 = Re[k1 + 1];
    i0 = Im[k0 + 1];
    i1 = Im[k1 + 1];
    Re[k0 + 1] = r0 + r1;
    Im[k0 + 1] = i0 + i1;
    r0 = r0 - r1;
    i0 = i0 - i1;
    Re[k1 + 1] = c * r0 - s * i0;
    Im[k1 + 1] = s * r0 + c * i0;
    k0 = k1 + span;
    if (k0 < ninc)
        goto L_11;
    k1 = k0 - ninc;
    c = -c;
    k0 = span - k1;
    if (k1 < k0)
        goto L_11;
    k0 = k0 + inc;
    if (k0 < k1)
        goto L_10;

    /*  RECURSION TO NEXT LEVEL. */
  L_12:
    span = span / 2;
    k0 = 0;

    /*  ANGLE=0 LOOP. */
  L_13:
    k1 = k0 + span;
    r0 = Re[k0 + 1];
    r1 = Re[k1 + 1];
    i0 = Im[k0 + 1];
    i1 = Im[k1 + 1];
    Re[k0 + 1] = r0 + r1;
    Im[k0 + 1] = i0 + i1;
    Re[k1 + 1] = r0 - r1;
    Im[k1 + 1] = i0 - i1;
    k0 = k1 + span;
    if (k0 < ninc)
        goto L_13;

    /*  ARE WE FINISHED... */
    if (span == inc)
        goto L_20;
    /*  NO. PREPARE NON-ZERO ANGLES. */
    c0 = 2.0e0 * powi(Sines[is], 2);
    is = is - 1;
    s = sign(Sines[is], sgn);
    s0 = s;
    c = 1.0e0 - c0;
    k0 = inc;
    goto L_11;

    /*    ARRAYS Re AND I NOW CONTAIN TRANSFORM, BUT STORED IN 'REVERSE-
     *    BINARY' ORDER. THE RE-ORDERING IS DONE BY PAIR EXCHANGES.
     *    REFERENCE FOR SORTING PRINCIPLE IS P. 180 AND P. 182 OF REF. (2).
     *
     *    ONCE AGAIN, COMMENTARY APPLIES TO INC=1 CASE.
     *  INDICES ARE:
     *    IJ:=0,1,2...N/2-1 ( A SIMPLE COUNTER).
     *    JI:=REVERSAL OF IJ.
     *    RC:=REVERSAL OF 0,2,4...N/2 (INCREMENTED N/4 TIMES).
     *  RC IS INCREMENTED THUSLY: STARTING WITH THE NEXT-TO-LEFTMOST BIT,
     *  CHANGE EACH BIT UP TO AND INCLUDING FIRST 0. (THE ACTUAL CODING IS
     *  DONE SO AS TO WORK FOR ANY INC>0 WITH EQUAL EFFICIENCY.)
     *    FOR ALL EXCHANGES IJ FITS ONE OF THESE CASES:
     *      (1) 1ST AND LAST BITS ARE 0 (IJ,JI EVEN AND <N/2), AND IJ<=JI.
     *      (2) ONE'S COMPLEMENT OF CASE (1) (BOTH ODD AND >N/2).
     *      (3) 1ST BIT 0, LAST BIT 1 (IJ ODD AND <N/2, JI>N/2).
     *    THE CODE FROM LABEL EVEN DOWN TO ODD IS ENTERED WITH IJ EVEN AND
     *  <=JI. FIRST TIME THRU THE COMPLEMENTS ARE DONE -CASE (2). SECOND
     *  TIME THRU GETS CASE (1). THUS A PAIR OF ELEMENTS BOTH IN THE FIRST
     *  HALF OF THE SEQUENCE, AND ANOTHER PAIR IN THE 2ND HALF, ARE
     *  EXCHANGED. THE CONDITION IJ<JI PREVENTS A PAIR FROM BEING EXCHANGED
     *  TWICE.
     *    THE CODE FROM LABEL ODD DOWN TO INCREV DOES CASE (3).
     * */
  L_20:
    n1 = ninc - inc;
    n2 = ninc / 2;
    rc = 0;
    ji = rc;
    ij = ji;
    if (n2 == inc)
        return;
    goto L_22;

    /*  EVEN. */
  L_21:
    ij = n1 - ij;
    ji = n1 - ji;
    t = Re[ij + 1];
    Re[ij + 1] = Re[ji + 1];
    Re[ji + 1] = t;
    t = Im[ij + 1];
    Im[ij + 1] = Im[ji + 1];
    Im[ji + 1] = t;
    if (ij > n2)
        goto L_21;

    /*  ODD. */
  L_22:
    ij = ij + inc;
    ji = ji + n2;
    t = Re[ij + 1];
    Re[ij + 1] = Re[ji + 1];
    Re[ji + 1] = t;
    t = Im[ij + 1];
    Im[ij + 1] = Im[ji + 1];
    Im[ji + 1] = t;
    it = n2;

    /*  INCREMENT REVERSED COUNTER. */
  L_23:
    it = it / 2;
    rc = rc - it;
    if (rc >= 0)
        goto L_23;
    rc = rc + 2 * it;
    ji = rc;
    ij = ij + inc;
    if (ij < ji)
        goto L_21;
    if (ij < n2)
        goto L_22;

    return;
}                               /* end of function */



/**
 * Allocate memory for real and imaginary complex array
 *
 * Arguments:
 *   - `re` - Pointer to pointer of real array
 *   - `im` - Pointer to pointer of imaginary array
 *   - `n` - Length of input data, output will be next power of 2
 *
 * Example:
 *
 *  int n = 1000;
 *  double *re, *im;
 *  re = NULL;
 *  im = NULL;
 *  fft_alloc(&re, &im, *n);
 *  assert(n == 1024);
 *
 */
void
fft_alloc(double **re, double **im, int n) {
    *re = (double *) calloc(n, sizeof(double));
    *im = (double *) calloc(n, sizeof(double));

}
/**
 * Copy float (single-precision) array into real part of complex array
 *
 * Arguments:
 *   - `data` - Single precision (float) array of data
 *   - `n` - Length of data
 *   - `re` - Real part of an complex array (double-precision)
 *
 * Example:
 *
 *  int n = 1024;
 *  float data[1024] = { ... };
 *  double *re, *im;
 *  re = im = NULL;
 *  fft_alloc(&re, &im, n);
 *
 *  fft_data_to_real(data, n, re);
 *
 */
void
fft_data_to_real(float *data, int n, double *re) {
    for(size_t i = 0; i < (size_t) n; i++) {
        re[i] = data[i];
    }
}
/**
 * Copy double (double-precision) array into real part of complex array
 *
 * Arguments:
 *   - `data` - Double precision (double) array of data
 *   - `n` - Length of data
 *   - `re` - Real part of an complex array (double-precision)
 *
 * Example:
 *
 *  int n = 1024;
 *  double data[1024] = { ... };
 *  double *re, *im;
 *  re = im = NULL;
 *  fft_alloc(&re, &im, n);
 *
 *  fft_data_to_real_d(data, n, re);
 *
 */
void
fft_data_to_real_d(double *data, int n, double *re) {
    memcpy(re, data, n * sizeof(double));
}

/**
 * Calls the FFT routine 
 *
 * Arguments:
 *   - `re` - Input time series and Transfromed Real part on output
 *   - `im` - 0.0 on input, Transformed Imaginary part on output
 *   - `n2` - Length of re and im
 *
 * Call dcpft()
 *
 */
void
fft_base(double *re, double *im, int n2) {
    dcpft(re, im, n2, 1, FFT_FORWARD);
}

/**
 * Scale the FFT spectrum by dt
 *
 * Arguments:
 *   - `re` - Real part of the Spectrum
 *   - `im` - Imaginary part of the Spectrum
 *   - `nf` - Length of re and im
 *   - `dt` - Scaling factor, normally the time sampling
 *
 * m = nf/2
 * re[:]        =  re[:] * dt        ! All Frequencies
 * im[0]        =  im[0]             ! Zero Frequency
 * im[1:m-1]    =  im[1:m-1] * dt    ! Positive Frequencies
 * im[m]        =  im[m]             ! Nyquist
 * im[m+1:nf-1] = -im[m+1:nf-1] * dt ! Negative Frequencies
 */
void
fft_scale(double *re, double *im, int nf, double dt) {
    re[0]    *= dt;
    re[nf/2] *= dt;
    for(int i = 1; i < (nf/2)- 1; i++) {
        int j = nf - i;
        re[i] *= dt;
        im[i] *= dt;
        re[j] =  re[i];
        im[j] = -im[i];
    }
}

static void
d2f(double *d, float *f, int n) {
    for(int i = 0; i < n; i++) {
        f[i] = (float) d[i];
    }
}

int
fft_npts_check(int n, int nf) {
    int m = next2(n);
    if(m > nf) {
        printf("Length of the FFT needs to be at least %d long\n", m);
        return -1;
    }
    return m;
}

/**
 * FFT for double precision input
 *
 * See fft()
 */
void
dfft(double *data, int n, double *re, double *im, int *nf) {
    if((*nf = fft_npts_check(n, *nf)) < 0) {
        return;
    }
    if(data != re) {
        memset(re, 0, sizeof(double) * *nf);
        memset(im, 0, sizeof(double) * *nf);
        fft_data_to_real_d(data, n, re);
    }
    fft_base(re, im, *nf);
    //fft_scale(*re, *im, *nf, dt);
}

/**
 * Fast Fourier Transform
 *
 * Arguments:
 *   - `data` - Input data
 *   - `n` - Length of input data
 *   - `dt` - Sampling rate of input data
 *   - `re` - Real component of Fourier Transform of data (Output)
 *   - `im` - Imaginary component of Fourier Transform of data (Ouput)
 *   - `nf` - Length of re and im (Ouput)
 *
 * Processing:
 *   Real and Imaginary components are allocated and padded to the next power of 2,
 *   data is converted to double-precision then the actual FFT is taken.
 *   Following the transform, the spectrum is scaled by the sampling rate of the data.
 *
 * Spectrum:
 *   Data is organized with the zero-frequency component at the first value and
 *   postive frequencies following up to the Nyquist and negative frequencies in
 *   reverse order. 
 *
 */
void
fft(float *data, int n, float *re, float *im, int *nf) {
    double *xre, *xim;
    xre = xim = NULL;
    if((*nf = fft_npts_check(n, *nf)) < 0) {
        return;
    }

    fft_alloc(&xre, &xim, *nf);

    fft_data_to_real(data, n, xre);
    dfft(xre, n, xre, xim, nf);

    /* Copy double to float */
    d2f(xre, re, *nf);
    d2f(xim, im, *nf);
    /* Free allocated array */
    free(xre);
    free(xim);
}

void
fftz(float *data, int n, float complex *z, int *nf) {
    double *xre, *xim;
    xre = xim = NULL;
    if((*nf = fft_npts_check(n, *nf)) < 0) {
        return;
    }
    fft_alloc(&xre, &xim, *nf);

    fft_data_to_real(data, n, xre);
    dfft(xre, n, xre, xim, nf);

    /* Copy double to complex */
    for(int i = 0; i < *nf; i++) {
        z[i] = xre[i] + xim[i] * I;
    }
    /* Free allocated array */
    free(xre);
    free(xim);
}

void fftz_(float *data, int *n, float complex *z, int *nf) {
    fftz(data, *n, z, nf);
}
void fftz__(float *data, int *n, float complex *z, int *nf) {
    fftz(data, *n, z, nf);
}

/**
 * Basic Fortran Interface 
 *
 * Arguments:
 *   - `data` - Input data to be transformed
 *   - `n` - Length of data
 *   - `re` - Output real FFT spectrum
 *   - `im` - Output imaginary FFT spectrum
 *   - `nf` - Length of re and im
 */
void fft_(float *data, int *n, float *re, float *im, int *nf) {
    fft(data, *n, re, im, nf);
}
void fft__(float *data, int *n, float *re, float *im, int *nf) {
    fft(data, *n, re, im, nf);
}
void dfft_(double *data, int *n, double *re, double *im, int *nf) {
    dfft(data, *n, re, im, nf);
}
void dfft__(double *data, int *n, double *re, double *im, int *nf) {
    dfft(data, *n, re, im, nf);
}

void
dfftz(double *data, int n, double complex *z, int *nf) {
    double *ri;
    if((*nf = fft_npts_check(n, *nf)) < 0) {
        return;
    }
    memset(z, 0, sizeof(double complex) * *nf);
    for(int i = 0; i < n; i++) {
        z[i] = data[i];
    }

    ri = (double *) z;
    dcpft(ri, &ri[1], *nf, 2, FFT_FORWARD);
}

void
dfftz_(double *data, int *n, double complex *z, int *nf) {
    dfftz(data, *n, z, nf);
}
void
dfftz__(double *data, int *n, double complex *z, int *nf) {
    dfftz(data, *n, z, nf);
}


void
idfftz(double *data, int n, double complex *z, int nf) {
    double *ri;
    ri = (double *) z;
    dcpft(ri, &ri[1], nf, 2, FFT_INVERSE);
    for(int i = 0; i < n; i++) {
        data[i] = creal(z[i]) / nf;
    }
}
void idfftz_(double *data, int *n, double complex *z, int *nf) {
    idfftz(data, *n, z, *nf);
}
void idfftz__(double *data, int *n, double complex *z, int *nf) {
    idfftz(data, *n, z, *nf);
}


void
idfft(double *data, int n, double *re, double *im, int nf) {
    dcpft(re, im, nf, 1, FFT_INVERSE);
    for(int i = 0; i < n; i++) {
        data[i] = re[i] / nf;
    }
}
void idfft_(double *data, int *n, double *re, double *im, int *nf) {
    idfft(data, *n, re, im, *nf);
}
void idfft__(double *data, int *n, double *re, double *im, int *nf) {
    idfft(data, *n, re, im, *nf);
}

void
ifft(float *data, int n, float *re, float *im, int nf) {
    double *xre, *xim;
    xre = (double *) malloc(sizeof(double) * nf);
    xim = (double *) malloc(sizeof(double) * nf);
    for(int i = 0; i < nf; i++) {
        xre[i] = re[i];
        xim[i] = im[i];
    }
    dcpft(xre, xim, nf, 1, FFT_INVERSE);
    for(int i = 0; i < n; i++) {
        data[i] = xre[i] / nf;
    }
}
void
ifftz(float *data, int n, float complex *z, int nf) {
    double *xre, *xim;
    xre = (double *) malloc(sizeof(double) * nf);
    xim = (double *) malloc(sizeof(double) * nf);
    for(int i = 0; i < nf; i++) {
        xre[i] = creal(z[i]);
        xim[i] = cimag(z[i]);
    }
    dcpft(xre, xim, nf, 1, FFT_INVERSE);
    for(int i = 0; i < n; i++) {
        data[i] = xre[i] / nf;
    }
}

void ifft_(float *data, int *n, float *re, float *im, int *nf) {
    ifft(data, *n, re, im, *nf);
}
void ifft__(float *data, int *n, float *re, float *im, int *nf) {
    ifft(data, *n, re, im, *nf);
}
void ifftz_(float *data, int *n, float complex *z, int *nf) {
    ifftz(data, *n, z, *nf);
}
void ifftz__(float *data, int *n, float complex *z, int *nf) {
    ifftz(data, *n, z, *nf);
}
