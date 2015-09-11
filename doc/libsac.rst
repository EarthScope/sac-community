

libsac Functions
----------------

These functions are directly from the SAC source code and have more options and scaffolding that would normally be required.  

- `Cross Correlation`_
- `Cut`_
- `Envelope`_
- `Interpolate`_
- `Remove Mean`_
- `Remove Trend`_
- `RMS`_
- `Rotate`_
- `Taper`_
- `Filter`_
- `Instrument Response`_


Cross Correlation
+++++++++++++++++

Compute the cross correlation function between two sequences.

.. code-block:: c

  void
  crscor(float *data1,
         float *data2,
         int    nsamps,
         int    nwin,
         int    wlen,
         char  *type,
         float *c,
         int   *nfft,
         char  *err,
         int    err_s)

.. code-block:: fortran

  subroutine
  crscor(real*4    data1(nsamps),
         real*4    data2(nsamps),
         integer*4 nsamps,
         integer*4 nwin,
         integer*4 wlen,
         character type(TYPE_LEN),
         real*4    c(nfft),
         integer*4 nfft,
         char      err(err_s),
         int       err_s)


- :code:`data1` - (IN) Array containing the first data sequence
- :code:`data2` - (IN) Array containing the second data sequence
- :code:`nsamps` - (IN) Maximum number of samples in data sequence
- :code:`nwin` - (IN) Requested Number of windows
- :code:`wlen` - (IN) Requested number of samples in each window.  The subroutine will calculate the window overlap.  Maximum value is 2048
- :code:`type` - (IN) Type of data analysis window to use.  Minimum Length: 3. Valid values are:
      - :code:`HAM` - HAMMING
      - :code:`HAN` - HANNING
      - :code:`C` - COSINE
      - :code:`R` - RECTAN
      - :code:`T` - TRIANG
- :code:`c` -   (OUT) Output Array containing resulting 2 * wlen - 1 length 
     correlation coefficients.  The correlation sequence is 
     circularly rotated in the array so that the zeroth lag 
     is at the beginning.  Array dimensions 0:4095
- :code:`nfft` - (INOUT) Number of samples in the correlation sequence.  May be padded with zeros.
- :code:`err` - (OUT) Error Message
- :code:`err_s` - (OUT) Length of string err

Cut
+++
Cut a data sequence

.. code-block:: c

  void
  cut(float *in,
      int    nstart,
      int    nstop,
      int    nfillb,
      int    nfille,
      float *out)

.. code-block:: fortran

  subroutine
  cut(real*4    in(IN_LEN),
      integer*4 nstart,
      integer*4 nstop,
      integer*4 nfillb,
      integer*4 nfille,
      real*4    out( nstop - nstart + 1 + nfillb + nfille ))

- :code:`in` - (IN) Input array to cut
- :code:`nstart` - (IN) Starting data point in cut
- :code:`nstop` - (IN) Ending data point in cut
- :code:`nfillb` - (IN) Number of data points before first data point
- :code:`nfille` - (IN) Number of data points after last data point
- :code:`out` - (OUT) Cut array 

cut_define
cut_define_check

Envelope
++++++++

Envelope function using the Hilbert Transform

.. code-block:: c

  void
  envelope(int    n,
           float *in,
           float *out)


.. code-block:: fortran

  subroutine
  envelope(integer n,
           real*4 in(n),
           real*4 out(n))

- :code:`n` - (IN) Number of point in the signals in and out
- :code:`in` - (IN) Input signal of length n
- :code:`out` - (OUT)  Output signal of length n


Interpolate
+++++++++++

Interpolate an evenly spaced data sequence

.. code-block:: c

  void
  interp(float *in,
         int    nlen,
         float *out,
         int    newlen,
         float  bval,
         float  eval,
         float  dt,
         float  tstart,
         float  dtnew,
         float  eps)

.. code-block:: fortran

  subroutine
  interp(real*4    in(nlen),
         integer*4 nlen,
         real*4    out(newlen),
         integer*4 newlen,
         real*4    bval,
         real*4    eval,
         real*4    dt,
         real*4    tstart,
         real*4    dtnew,
         real*4    eps)

- :code:`in` - (IN) Input data sequence to interpolate
- :code:`nlen` - (IN) Length of input data sequence
- :code:`out` - (OUT) Output interpolated data sequence
- :code:`newlen` - (IN) Length of output data sequence
- :code:`bval` - (IN) Time of first data value
- :code:`eval` - (IN) Time of last data value
- :code:`dt` - (IN) Time sampling of input data 
- :code:`tstart` - (IN) Time of first data value of output, usually the same as bval
- :code:`dtnew` - (IN) New time sampling of output data sequence
- :code:`eps` - (IN) Water level for interpolation; Internally sac uses the average of the 0.0001 of the absolute value of the slope

Interpolate an unevenly spaced data sequence

.. code-block:: c

  void
  interp2(float *in,
          int    nlen,
          float *out,
          int    newlen,
          float  bval,
          float  eval,
          float *t,
          float  tstart,
          float  dtnew,
          float  eps)

.. code-block:: fortran

  subroutine
  interp2(real*4    in(nlen),
          integer*4 nlen,
          real*4    out(newlen),
          integer*4 newlen,
          real*4    bval,
          real*4    eval,
          real*4    t(nlen),
          real*4    tstart,
          real*4    dtnew,
          real*4    eps)

- :code:`in` - (IN) Input data sequence to interpolate
- :code:`nlen` - (IN) Length of input data sequence
- :code:`out` - (OUT) Output interpolated data sequence
- :code:`newlen` - (IN) Length of output data sequence
- :code:`bval` - (IN) Time of first data value
- :code:`eval` - (IN) Time of last data value
- :code:`t` - (IN) Times of the input data sequence
- :code:`tstart` - (IN) Time of first data value of output, usually the same as bval
- :code:`dtnew` - (IN) New time sampling of output data sequence
- :code:`eps` - (IN) Water level for interpolation; Internally sac uses the average of the 0.0001 of the absolute value of the slope


Remove Mean
+++++++++++

Remove the mean of a data sequence

.. code-block:: c

  void
  rmean(float *data,
        int    n,
        float  mean)

.. code-block:: fortran

  subroutine
  rmean(real*4   data(n),
        integer  n,
        real*4   mean)

- :code:`data` - (INOUT) Data sequence to have mean removed
- :code:`n` - (IN) Length of data sequence
- :code:`mean` - (IN) Mean value to remove from all sequences

Remove Trend
++++++++++++
Remove the trend from an evenly spaced data sequence

.. code-block:: c

  void
  rtrend(float *data,
         int    n,
         float  yint,
         float  slope,
         float  b,
         float  delta)

.. code-block:: fortran

  subroutine
  rtrend(real*4    data(n),
         integer*4 n,
         real*4    yint,
         real*4    slope,
         real*4    b,
         real*4    delta)

- :code:`data` - (INOUT) Data sequence to have trend removed
- :code:`n` - (IN) Length of data sequence
- :code:`yint` - (IN) Y value at zero time
- :code:`slope` - (IN) Slope of trend
- :code:`b` - (IN) Beginning time value of data sequence
- :code:`delta` - (IN) Time sampling of data sequence


Remove the trend from an unevely spaced data sequence

.. code-block:: c

  void
  rtrend(float *data,
         int    n,
         float  yint,
         float  slope,
         float  t(n))


.. code-block:: fortran

  subroutine
  rtrend(real*4    data(n),
         integer*4 n,
         real*4    yint,
         real*4    slope,
         real*4    t(n)

- :code:`data` - (INOUT) Data sequence to have trend removed
- :code:`n` - (IN) Length of data sequence
- :code:`yint` - (IN) Y value at zero time
- :code:`slope` - (IN) Slope of trend
- :code:`t` - (IN) Time values of the data sequence

lifite

RMS
+++
Compute the Root Mean Squared value of a data sequence

.. code-block:: c

  double
  rms(float *data,
      int    nsamps)

- :code:`data` - (IN) Input data sequence
- :code:`nsamps` - (IN) Length of data
- Returns :code:`double` RMS 


Rotate
++++++
Perform a clockwise rotation on a pair of signals


.. code-block:: c

  void
  rotate(float  *si1,
         float  *si2,
         int     ns,
         double  angle,
         int     lnpi,
         int     lnpo,
         float  *so1,
         float  *so2)

.. code-block:: fortran

  void
  rotate(real*4    si1(ns),
         real*4    si2(ns),
         integer*4 ns,
         real*8    angle,
         integer*4 lnpi,
         integer*4 lnpo,
         real*4    so1(ns),
         real*4    so2(ns))

- :code:`si1` - (IN) First input data signal
- :code:`si2` - (IN) Second input data signal
- :code:`ns` - (IN) Length of si1, si2, so1, so2
- :code:`angle` - (IN) Angle of rotation, clockwise from S1
- :code:`lnpi` - (IN) 1 if input signals have normal polarity, 0 otherwise
- :code:`lnpo` - (IN) 1 if output signals have normal polarity, 0 otherwise
- :code:`so1` - (OUT) First output signal
- :code:`so2` - (OUT) Second output signal

Taper
+++++

Taper the end of a signal

.. code-block:: c

  void
  taper(float *data,
        int    n,
        int    taper_type,
        int    ipts)

.. code-block:: fortran

  void
  taper(real*4    data(n),
        integer*4 n,
        integer*4 taper_type,
        integer*4 ipts)

- :code:`data` - (INOUT) Input data signal to be tapered
- :code:`n` - (IN) Length of data
- :code:`taper_type` - (IN) Taper type
         - TAPER_TYPE_COSINE
         - TAPER_TYPE_HANNING
         - TAPER_TYPE_HAMMING
- :code:`ipts` - (IN) Number of points to use in the taper

Filter
++++++
Filter a data signal with an Infinite Impulse Response Filter

.. code-block:: c

  void
  xapiir(float  *data,
         int     nsamps,
         char   *aproto,
         double  trbndw,
         double  a,
         int     iord,
         char   *type,
         double  flo,
         double  fhi,
         double  ts,
         int     passes)

.. code-block:: fortran

  void
  xapiir(real*4     data(nsamps),
         integer    nsamps,
         character  aproto(CLEN),
         real*8     trbndw,
         real*8     a,
         integer    iord,
         character  type(CLEN),
         real*8     flo,
         real*8     fhi,
         real*8     ts,
         integer    passes)

- :code:`data` - (INOUT) Data signal to be filtered
- :code:`nsamps` - (IN) Length of data
- :code:`aproto` - (IN) Analog Filter Prototype
             - 'BU' - Butterworth
             - 'BE' - Bessel
             - 'C1' - Chebyshev Type I
             - 'C2' - Chebyshev Type II
- :code:`trbndw` - (IN) Transition bandwidth as a factor of low prototype, used in Chebyshev filters
- :code:`a` - (IN) Attenuation Factor used in Chebyshev filters
- :code:`iord` - (IN) Order of the analog Prototype, Max 10, 4-5 is "ample"
- :code:`type` - (IN) Filter Type
             - 'LP' - Low Pass
             - 'HP' - High Pass
             - 'BP' - Band Pass
             - 'BR' - Band Reject
- :code:`flo` - (IN) Low frequency cutoff, ignored in LP
- :code:`fhi` - (IN) High frequency cutoff, ignored in HP
- :code:`ts` - (IN) time sampling
- :code:`passes` - (IN) Number of filter passes
            - 1 Forward time filtering
            - 2 Forward and reverse filtering (Zero Phase)

Instrument Response
+++++++++++++++++++

Add or remove an instrument response (Transfer Function)

.. code-block:: c

  void
  ztransfer(float   *dat,
            int      npts,
            double   delta,
            double  *sre,
            double  *sim,
            double  *xre,
            double  *xim,
            int      nfreq,
            int      nfft,
            double   delfrq,
            double  *F)

- :code:`dat` - (IN) Input data to add or remove an instrument response
- :code:`npts` - (IN) Length of dat
- :code:`delta` - (IN) Time sampling of input data
- :code:`sre` - 
              - (IN) Real Part of Instrument Response to Remove
              - (OUT) Output time data with Intrument added and removed
- :code:`sim` - (IN) Imaginary Part of Instrument Response to Remove, overwritten
- :code:`xre` - (IN) Real Part of Instrument Response to Add
- :code:`xim` - (IN) Real Part of Instrument Response to Add
- :code:`nfreq` - (IN) nfft / 2 + 1
- :code:`nfft` - (IN) Length of Instrument responses sre, sim, xre, xim
- :code:`delfrq` - (IN) 1 / ( nfft * delta )
- :code:`F` -  (IN) Frequency Limits, Length: 4,  f1 < f2 < f3 < f4
         - f < f1 - 0.0
         - f1 < f < f2 - High Pass Filter at Low Frequencies
         - f2 < f < f3 - 1.0
         - f3 < f < f4 - Low Pass Filter at High Frequencies
         - f4 < f - 0.0


Utility Functions
-----------------

The routines above use the following functions. These routines should be used with caution. 

- apply - Apply an IIR Filter to a data sequence 
- beroots - Compute Bessel Poles for a Normalized Low Pass Filter
- bilin2 - Bilinear Transform, Convert an Analog Fitler to a Digital Filter
- buroots - Compute Butterworth Poles for a Normalized Low Pass Filter
- c1roots - Compute Chebyshev Type I Poles for a Normalized Low Pass Filter
- c2roots - Compute Chebyshev Type II Poles for a Normalized Low Pass Filter
- chebparm - Calculate Chebyshev Type I and II Design Parameters
- copy_float - Copy an array of float / real*4
- copydouble - Copy an array of double / real*8
- cross_corelation_normalized - Normalize a Cross Correlation Function using the Absolute Value of the  Sum of the signals
- cutoffs - Alter the cutoff of a filter
- dcpft - Double Precision FFT
- design - Design IIR Digital Filter form Analog Prototypes
- firtrn - Calculate the transform of a signal with a FIR Filter
- fft - Single Precision FFT
- getran - Compute a Transfer Function from Poles and Zerso
- getranx - Compute a Transfer Function from Poles and Zeros 
- lp - Generate a Second Order Section for a Low Pass Filter
- lptbp - Convert a Low Pass Filter to a Band Pass Filter
- lptbr - Convert a Low Pass Filter to a Band Reject Filter
- lpthp - Convert a Low Pass Filter to a High Pass Filter
- next2 -  Find the next power of 2
- overlp - Simplified overlap-save routine
- taper_spectra - Taper spectra with a cosine
- taper_width_to_points - Convert perctange of signal length to points
- warp - Applies tangent frequency warping to compensate for bilinear analog -> digital transformation
- wigint - Interpolate evenly or unevely spaced data
- window - Window a data sequence
- zero - Fill an array with zeros
- zshft - Shift a signal in place with zero filling


Complex Number Functions
++++++++++++++++++++++++

 These are functions to handle complex numbers.  Do not use these, there are better more robust complex number handling routines in all languages. 
 
- aimag
- cmplxabs
- cmplxadd
- cmplxang
- cmplxcj
- cmplxdiv
- cmplxexp
- cmplxlog
- cmplxmul
- cmplxneg
- cmplxpow
- cmplxsqrt
- cmplxsub
- cmplxtof
- flttocmplx

