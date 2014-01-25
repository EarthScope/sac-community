#		<< IRIS SEED Reader, Release 4.5.1 >>
#		
#		======== CHANNEL RESPONSE DATA ========
B050F03     Station:     N11A
B050F16     Network:     TA
B052F03     Location:    ??
B052F04     Channel:     LHZ
B052F22     Start date:  2006,123,00:00:00
B052F23     End date:    2007,122,22:15:00
#		=======================================
#		+               +--------------------------------------------+                +
#		+               |   Response (Poles & Zeros),  N11A ch LHZ   |                +
#		+               +--------------------------------------------+                +
#		
B053F03     Transfer function type:                A [Laplace Transform (Rad/sec)]
B053F04     Stage sequence number:                 1
B053F05     Response in units lookup:              M/S - velocity in meters per second
B053F06     Response out units lookup:             V - emf in volts
B053F07     A0 normalization factor:               5.73092E+08
B053F08     Normalization frequency:               0.03
B053F09     Number of zeroes:                      2
B053F14     Number of poles:                       5
#		Complex zeroes:
#		  i  real          imag          real_error    imag_error
B053F10-13    0  0.000000E+00  0.000000E+00  0.000000E+00  0.000000E+00
B053F10-13    1  0.000000E+00  0.000000E+00  0.000000E+00  0.000000E+00
#		Complex poles:
#		  i  real          imag          real_error    imag_error
B053F15-18    0 -3.701000E-02  3.701000E-02  0.000000E+00  0.000000E+00
B053F15-18    1 -3.701000E-02 -3.701000E-02  0.000000E+00  0.000000E+00
B053F15-18    2 -1.131000E+03  0.000000E+00  0.000000E+00  0.000000E+00
B053F15-18    3 -1.005000E+03  0.000000E+00  0.000000E+00  0.000000E+00
B053F15-18    4 -5.027000E+02  0.000000E+00  0.000000E+00  0.000000E+00
#		
#		+                  +---------------------------------------+                  +
#		+                  |       Channel Gain,  N11A ch LHZ      |                  +
#		+                  +---------------------------------------+                  +
#		
B058F03     Stage sequence number:                 1
B058F04     Gain:                                  1.499760E+03
B058F05     Frequency of gain:                     3.000000E-02 HZ
B058F06     Number of calibrations:                0
#		
#		+               +-------------------------------------------+                 +
#		+               |   Response (Coefficients),  N11A ch LHZ   |                 +
#		+               +-------------------------------------------+                 +
#		
B054F03     Transfer function type:                D
B054F04     Stage sequence number:                 2
B054F05     Response in units lookup:              V - emf in volts
B054F06     Response out units lookup:             COUNTS - digital counts
B054F07     Number of numerators:                  0
B054F10     Number of denominators:                0
#		
#		+                      +------------------------------+                       +
#		+                      |   Decimation,  N11A ch LHZ   |                       +
#		+                      +------------------------------+                       +
#		
B057F03     Stage sequence number:                 2
B057F04     Input sample rate:                     1.000000E+00
B057F05     Decimation factor:                     1
B057F06     Decimation offset:                     0
B057F07     Estimated delay (seconds):             0.000000E+00
B057F08     Correction applied (seconds):          0.000000E+00
#		
#		+                  +---------------------------------------+                  +
#		+                  |       Channel Gain,  N11A ch LHZ      |                  +
#		+                  +---------------------------------------+                  +
#		
B058F03     Stage sequence number:                 2
B058F04     Gain:                                  4.194300E+05
B058F05     Frequency of gain:                     3.000000E-02 HZ
B058F06     Number of calibrations:                0
#		
#		+               +-------------------------------------------+                 +
#		+               |   Response (Coefficients),  N11A ch LHZ   |                 +
#		+               +-------------------------------------------+                 +
#		
B054F03     Transfer function type:                D
B054F04     Stage sequence number:                 3
B054F05     Response in units lookup:              COUNTS - digital counts
B054F06     Response out units lookup:             COUNTS - digital counts
B054F07     Number of numerators:                  31
B054F10     Number of denominators:                0
#		Numerator coefficients:
#		  i, coefficient,  error
B054F08-09    0 -1.220040E-16  0.000000E+00
B054F08-09    1  3.372780E-11  0.000000E+00
B054F08-09    2 -3.190940E-07  0.000000E+00
B054F08-09    3  1.263130E-06  0.000000E+00
B054F08-09    4 -1.484910E-05  0.000000E+00
B054F08-09    5  3.321990E-03  0.000000E+00
B054F08-09    6  1.083230E-03  0.000000E+00
B054F08-09    7 -6.240460E-03  0.000000E+00
B054F08-09    8  1.265660E-02  0.000000E+00
B054F08-09    9 -1.576630E-02  0.000000E+00
B054F08-09   10  9.567620E-03  0.000000E+00
B054F08-09   11  1.071100E-02  0.000000E+00
B054F08-09   12 -4.564250E-02  0.000000E+00
B054F08-09   13  8.975230E-02  0.000000E+00
B054F08-09   14 -1.324370E-01  0.000000E+00
B054F08-09   15  1.552890E-01  0.000000E+00
B054F08-09   16  8.123710E-01  0.000000E+00
B054F08-09   17  1.854150E-01  0.000000E+00
B054F08-09   18 -1.406090E-01  0.000000E+00
B054F08-09   19  8.970830E-02  0.000000E+00
B054F08-09   20 -4.243970E-02  0.000000E+00
B054F08-09   21  7.140010E-03  0.000000E+00
B054F08-09   22  1.210510E-02  0.000000E+00
B054F08-09   23 -1.693070E-02  0.000000E+00
B054F08-09   24  1.277350E-02  0.000000E+00
B054F08-09   25 -5.864240E-03  0.000000E+00
B054F08-09   26  6.451770E-04  0.000000E+00
B054F08-09   27  3.487130E-03  0.000000E+00
B054F08-09   28 -8.530580E-05  0.000000E+00
B054F08-09   29  2.297130E-06  0.000000E+00
B054F08-09   30 -2.258300E-07  0.000000E+00
#		
#		+                      +------------------------------+                       +
#		+                      |   Decimation,  N11A ch LHZ   |                       +
#		+                      +------------------------------+                       +
#		
B057F03     Stage sequence number:                 3
B057F04     Input sample rate:                     1.000000E+00
B057F05     Decimation factor:                     1
B057F06     Decimation offset:                     0
B057F07     Estimated delay (seconds):             1.600000E+01
B057F08     Correction applied (seconds):          1.600000E+01
#		
#		+                  +---------------------------------------+                  +
#		+                  |       Channel Gain,  N11A ch LHZ      |                  +
#		+                  +---------------------------------------+                  +
#		
B058F03     Stage sequence number:                 3
B058F04     Gain:                                  1.000000E+00
B058F05     Frequency of gain:                     3.000000E-02 HZ
B058F06     Number of calibrations:                0
#		
#		+                  +---------------------------------------+                  +
#		+                  |   Channel Sensitivity,  N11A ch LHZ   |                  +
#		+                  +---------------------------------------+                  +
#		
B058F03     Stage sequence number:                 0
B058F04     Sensitivity:                           6.290440E+08
B058F05     Frequency of sensitivity:              3.000000E-02 HZ
B058F06     Number of calibrations:                0
#		
