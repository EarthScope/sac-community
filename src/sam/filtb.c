
#include <stdio.h>
#include <math.h>

#include "sam.h"
#include "co.h"

double /*FUNCTION*/ filtb(int iopt, float xt)
{
	int i, i_, j;
        static int icnt[4];
	float filtb_v;
	static double a, aa, atk, b, btk, c, con1, con2, ga1, ga2, ga3, gb1, 
	 gb2, gb3, hp1, hp2, lp1, lp2, lp3, ng, rad, rt, s1, s2, s3, s4, 
	 t1, t2, t3, th1[3][4], th1a, th2[2][4], th2a, th3[4], th3a, th4[2][4], 
	 th4a, th5[2][4], th5a, wb1, wf1, wg1, x1[3][4], x2[2][4], x3[4], 
	 x4[2][4], x5[2][4];
	static double ws = 6.283185308e0;
	static double ks = 355.0e0;
	static double m = 107.5e0;
	static double hs = 0.0096e0;
	static double l = 4.0e0;
	static double hg = 0.004e0;
	static double tg = 79.42e0;
	static double wg = 31.41592654e0;
	static double wa = 0.6283185308e0;
	static double wb = 62.83185308e0;
	static double wf = 31.41592654e0;
	static double nf = 0.9e0;
	static double att = 40.0e0;
	static double rd = 20.0e0;
	static double k0 = 30.61e0;
	static double ert = 1.0e0;

	int *const Icnt = &icnt[0] - 1;
	double *const Th3 = &th3[0] - 1;
	double *const X3 = &x3[0] - 1;



	/* . . .    Benioff recursive filter
	 *
	 * . . .    Linkage - X = FILTB (IOPT, XT)
	 *
	 * . . .    Arguments -
	 *            IOPT - 0 => initialize filter
	 *                   1 => filter radial data
	 *                   2 => filter tangential data
	 *                   3 => filter vertical data
	 *                   4 => extra
	 *
	 *            XT - input data to be filtered if IOPT # 0
	 *               - time between points if IOPT = 0
	 *
	 *
	 *
	 *
	 *          WS = 2.0 * PI * natural frequency of seismometer (in Hz)
	 *          KS = generator constant in newtons per amp
	 *          M = mass of seismometer in Kg
	 *          HS = mechanical damping factor
	 *          L = inductance of data coils in ohm * sec
	 *          HG = mechanical damping factor
	 *          TG = galvanometer sensitivity in nt * m / amp
	 *          WG = 2.0 * pi * natural frequency of galvanometer
	 *          ERT = inertia of galvanometer in Kg*m**2
	 *
	 *
	 *     Modified 12-28-79 to stop ringing....Bill Taylor
	 *
	 * */






	if( iopt != 0 )
		goto L_2000;


	/* . . .    CONSTANTS FOR SEIS
	 * */
	atk = pow(10.0e0,att/20.0e0);
	btk = pow(atk,2.0);
	con1 = 1471.5e0*btk - 3.5e0;
	con2 = 9.0e0*btk - 1.0e0;
	rt = 163.3e0 + rd - 500.0e0*(btk - 1.0e0 + (1280.0e0*btk/con1))/
	 con2;

	a = rt + 2.0e0*hs*ws*l;
	b = l*ws*ws + rt*2.0e0*hs*ws + ks*ks/m;
	c = rt*ws*ws;

	t1 = 2.0e0/ xt;
	t2 = t1*t1;
	t3 = t2*t1;

	aa = l*t3 + a*t2 + b*t1 + c;
	s1 = t2/aa;
	s2 = (3.0e0*l*t3 + a*t2 - b*t1 - 3.0e0*c)/aa;
	s3 = (-3.0e0*l*t3 + a*t2 + b*t1 - 3.0e0*c)/aa;
	s4 = (l*t3 - a*t2 + b*t1 - c)/aa;

	/* . . .    CONSTANTS FOR GALVO # 1
	 * */
	ng = hg + tg*tg/ert*(9.0e0*atk*atk - 1.0e0)/2.0e0/wg/(1471.5e0*
	 atk*atk - 3.5e0);
	rad = wg/t1;
	wg1 = t1*sin( rad )/cos( rad );

	a = t2 + 2.0e0*ng*wg1*t1 + wg1*wg1;
	b = -2.0e0*t2 + 2.0e0*wg1*wg1;
	c = t2 - 2.0*ng*wg1*t1 + wg1*wg1;

	ga1 = wg1*wg1/a;
	ga2 = -b/a;
	ga3 = -c/a;

	/* . . .    HIGH PASS FILTER CONSTANTS
	 * */
	hp1 = (2.0e0 - wa* xt)/(2.0e0 + wa* xt);
	hp2 = 2.0e0/(2.0e0 + wa* xt);

	/* . . .    LOW PASS FILTER CONSTANTS
	 * */
	rad = wb/t1;
	wb1 = t1*sin( rad )/cos( rad );
	a = t2 + 1.414213562e0*wb1*t1 + wb1*wb1;
	b = -2.0e0*t2 + 2.0e0*wb1*wb1;
	c = t2 - 1.414213562e0*wb1*t1 + wb1*wb1;

	lp1 = wb1*wb1/a;
	lp2 = -b/a;
	lp3 = -c/a;

	/* . . .    GALVO # 2 CONSTANTS
	 * */
	rad = wf/t1;
	wf1 = t1*sin( rad )/cos( rad );
	a = t2 + 2.0e0*nf*wf1*t1 + wf1*wf1;
	b = -2.0e0*t2 + 2.0e0*wf1*wf1;
	c = t2 - 2.0e0*nf*wf1*t1 + wf1*wf1;

	gb1 = wf1*wf1/a;
	gb2 = -b/a;
	gb3 = -c/a;


	/* . . .    INITIALIZE ARRAYS
	 * */
	for( i = 0; i < 4; i++ ){
          icnt[i] = 0;
          for( j = 0; j < 3; j++ ){
             x1[j][i] = 0.0e0;
            th1[j][i] = 0.0e0;
          }
          
          for( j = 0; j < 2; j++ ){
             x2[j][i] = 0.0e0;
            th2[j][i] = 0.0e0;
             x4[j][i] = 0.0e0;
            th4[j][i] = 0.0e0;
             x5[j][i] = 0.0e0;
            th5[j][i] = 0.0e0;
          }
           x3[i] = 0.0e0;
          th3[i] = 0.0e0;
        }
	goto L_9000;



	/* . . .    SEIS FILTER
	 * */
L_2000:
	;
	if( Icnt[iopt] > 84 )
		goto L_2100;
	xt = xt * (1 - exp( -powi((double)( Icnt[iopt] ),2)/441. ));
	Icnt[iopt] = Icnt[iopt] + 1;
L_2100:
	th1a = s1*(xt - x1[0][iopt - 1] - x1[1][iopt - 1] + x1[2][iopt - 1]) + 
	 s2*th1[0][iopt - 1] + s3*th1[1][iopt - 1] + s4*th1[2][iopt - 1];

	for( i = 3; i >= 2; i-- ){
		i_ = i - 1;
		th1[i_][iopt - 1] = th1[i_ - 1][iopt - 1];
		x1[i_][iopt - 1] = x1[i_ - 1][iopt - 1];
		}
	th1[0][iopt - 1] = th1a;
	x1[0][iopt - 1] = xt;

	/* . . .    GALVO #1 FILTER
	 * */
	th2a = ga1*(th1a + 2.0e0*x2[0][iopt - 1] + x2[1][iopt - 1]) + 
	 ga2*th2[0][iopt - 1] + ga3*th2[1][iopt - 1];

	th2[1][iopt - 1] = th2[0][iopt - 1];
	th2[0][iopt - 1] = th2a;
	x2[1][iopt - 1] = x2[0][iopt - 1];
	x2[0][iopt - 1] = th1a;

	/* . . .    HIGH PASS FILTER
	 * */
	th3a = hp1*Th3[iopt] + hp2*(th2a - X3[iopt]);
	Th3[iopt] = th3a;
	X3[iopt] = th2a;

	/* . . .    LOW PASS FILTER
	 * */
	th4a = lp1*(th3a + 2.0e0*x4[0][iopt - 1] + x4[1][iopt - 1]) + 
	 lp2*th4[0][iopt - 1] + lp3*th4[1][iopt - 1];
	x4[1][iopt - 1] = x4[0][iopt - 1];
	x4[0][iopt - 1] = th3a;
	th4[1][iopt - 1] = th4[0][iopt - 1];
	th4[0][iopt - 1] = th4a;

	/* . . .    GALVO #2 FILTER
	 * */
	th5a = gb1*(th4a + 2.0e0*x5[0][iopt - 1] + x5[1][iopt - 1]) + 
	 gb2*th5[0][iopt - 1] + gb3*th5[1][iopt - 1];

	th5[1][iopt - 1] = th5[0][iopt - 1];
	th5[0][iopt - 1] = th5a;
	x5[1][iopt - 1] = x5[0][iopt - 1];
	x5[0][iopt - 1] = th4a;

	filtb_v = th5a*k0;

L_9000:
	;
	return( filtb_v );
} /* end of function */

