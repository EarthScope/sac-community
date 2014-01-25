
#include <string.h>

#include "icm.h"
#include "complex.h"

#include "msg.h"

void /*FUNCTION*/ noress(nfreq, delfrq, xre, xim, subtyp, subtyp_s)
int nfreq;
double delfrq, xre[], xim[];
char *subtyp;   int subtyp_s;
{
	int idx, nerr;
	float delomg, gkip, gklp, go, omega;
	complexf ch, ch1, ch2, ch3, ch4, ch5, cha, chb, chbp1, chbp2, 
	 chc, chd, chdf, che, chf, chg, chh, chhp1, chi, chif, chipf, 
	 chka, chkv, chlpf, cho, chpa, chs3, chsp, cht, ckb, cs;
	static double twopi = 6.283185307179586;

	double *const Xim = &xim[0] - 1;
	double *const Xre = &xre[0] - 1;




	/*   .....NORESS - for NORESS instrument....
	 *          (poles and zeros due to H. Durham)
	 * */


	delomg = twopi*delfrq;

	/*   .....computation based on LP, IP, or SP sub-type.....
	 * */
	if( memcmp(subtyp,"LP      ",8) == 0 ){
	    for( idx = 1; idx <= nfreq; idx++ ){
		omega = (float)( idx - 1 )*delomg;
		cs = flttocmplx( 0.0, omega );
		cht = cmplxdiv(flttocmplx(12.66,0.),(cmplxadd(cmplxadd(flttocmplx(1.0,0.),
		 cmplxmul(flttocmplx(0.02256,0.),cs)),cmplxmul(flttocmplx(0.6333,0.),cmplxpow(cs,(double)2)))));
		cha = cmplxneg(cmplxdiv(flttocmplx(3360.,0.),(cmplxadd(flttocmplx(1.0,0.),
		 cmplxmul(flttocmplx(0.003789,0.),cs)))));
		chb = cmplxdiv(cmplxmul(flttocmplx(1.4215e-4,0.),(cmplxadd(flttocmplx(1.0,0.),
		 cmplxmul(flttocmplx(0.01393,0.),cs)))),(cmplxadd(flttocmplx(1.0,0.),cmplxmul(flttocmplx(0.0009936,0.),
		 cs))));
		ckb = flttocmplx(1370.,0.);
		chka = cmplxdiv(cmplxmul(cht,cha),(cmplxsub(flttocmplx(1.,0.),cmplxmul(cmplxmul(cmplxmul(cht,
		 cha),chb),ckb))));
		chkv = cmplxmul(cmplxmul(flttocmplx(100.0,0.),chka),cs);
		chhp1 = cmplxneg(cmplxdiv(cmplxmul(flttocmplx(390.6,0.),cs),(cmplxadd(cmplxadd(flttocmplx(1.0,0.),
		 cmplxmul(flttocmplx(7.813,0.),cs)),cmplxmul(flttocmplx(0.4505,0.),cmplxpow(cs,(double)2))))));
		chbp1 = cmplxdiv(cmplxmul(flttocmplx(324.0,0.),cs),(cmplxadd(cmplxadd(flttocmplx(1.0,0.),
		 cmplxmul(flttocmplx(16.90,0.),cs)),cmplxmul(flttocmplx(113.3,0.),cmplxpow(cs,(double)2)))));
		chdf = cmplxneg(cmplxdiv(flttocmplx(1.0,0.),(cmplxadd(flttocmplx(1.0,0.),
		 cmplxmul(flttocmplx(0.001592,0.),cs)))));
		cha = cmplxdiv(flttocmplx(1.0,0.),(cmplxadd(flttocmplx(1.0,0.),cmplxmul(flttocmplx(0.00159,0.),
		 cs))));
		chb = cmplxdiv(flttocmplx(1.0,0.),(cmplxadd(cmplxadd(flttocmplx(1.0,0.),cmplxmul(flttocmplx(4.576,0.),
		 cs)),cmplxmul(flttocmplx(11.66,0.),cmplxpow(cs,(double)2)))));
		chc = cmplxdiv(cs,(cmplxmul((cmplxadd(flttocmplx(1.0,0.),cmplxmul(flttocmplx((double)1504,0.),
		 cs))),(cmplxadd(flttocmplx(1.0,0.),cmplxmul(flttocmplx(32.02,0.),cs))))));
		che = cmplxdiv(flttocmplx(1.0,0.),(cmplxadd(cmplxadd(flttocmplx(1.0,0.),cmplxmul(flttocmplx(0.9477,0.),
		 cs)),cmplxmul(flttocmplx(0.2428,0.),cmplxpow(cs,(double)2)))));
		chf = cmplxdiv(flttocmplx(1.0,0.),(cmplxadd(cmplxadd(flttocmplx(1.0,0.),cmplxmul(flttocmplx(0.6927,0.),
		 cs)),cmplxmul(flttocmplx(0.2399,0.),cmplxpow(cs,(double)2)))));
		chg = cmplxdiv(flttocmplx(1.0,0.),(cmplxadd(cmplxadd(flttocmplx(1.0,0.),cmplxmul(flttocmplx(0.1913,0.),
		 cs)),cmplxmul(flttocmplx(0.2403,0.),cmplxpow(cs,(double)2)))));
		chh = cmplxdiv(cs,(cmplxmul((cmplxadd(flttocmplx(1.0,0.),cmplxmul(flttocmplx(0.1913,0.),
		 cs))),(cmplxadd(flttocmplx(1.0,0.),cmplxmul(flttocmplx(32.02,0.),cs))))));
		gklp = 6589.6;
		chlpf = cmplxneg(cmplxmul(cmplxmul(cmplxmul(cmplxmul(cmplxmul(cmplxmul(cmplxmul(cmplxmul(flttocmplx(gklp,0.),
		 cha),chb),chc),chb),che),chf),chg),chh));
		ch = cmplxmul(cmplxmul(cmplxmul(cmplxmul(cmplxmul(cmplxmul(chkv,chhp1),chbp1),
		 chdf),chlpf),cs),flttocmplx(1.0e-9,0.));
		Xre[idx] = cmplxtof( ch );
		Xim[idx] = aimag( ch );
	    }
	    return;

	}
	else if( memcmp(subtyp,"IP      ",8) == 0 ){
	    for( idx = 1; idx <= nfreq; idx++ ){
		omega = (float)( idx - 1 )*delomg;
		cs = flttocmplx( 0.0, omega );
		cht = cmplxdiv(flttocmplx(12.66,0.),(cmplxadd(cmplxadd(flttocmplx(1.0,0.),
		 cmplxmul(flttocmplx(0.02256,0.),cs)),cmplxmul(flttocmplx(0.6333,0.),cmplxpow(cs,(double)2)))));
		cha = cmplxneg(cmplxdiv(flttocmplx(3360.,0.),(cmplxadd(flttocmplx(1.0,0.),
		 cmplxmul(flttocmplx(0.003789,0.),cs)))));
		chb = cmplxdiv(cmplxmul(flttocmplx(1.4215e-4,0.),(cmplxadd(flttocmplx(1.0,0.),
		 cmplxmul(flttocmplx(0.01393,0.),cs)))),(cmplxadd(flttocmplx(1.0,0.),cmplxmul(flttocmplx(0.0009936,0.),
		 cs))));
		ckb = flttocmplx(1370.,0.);
		chka = cmplxdiv(cmplxmul(cht,cha),(cmplxsub(flttocmplx(1.,0.),cmplxmul(cmplxmul(cmplxmul(cht,
		 cha),chb),ckb))));
		chkv = cmplxmul(cmplxmul(flttocmplx(100.0,0.),chka),cs);
		chbp2 = cmplxneg(cmplxdiv(cmplxmul(cmplxmul(flttocmplx(10.17,0.),cs),(cmplxadd(flttocmplx(127.8,0.),
		 cs))),(cmplxadd(cmplxadd(flttocmplx(1.0,0.),cmplxmul(flttocmplx(16.90,0.),
		 cs)),cmplxmul(flttocmplx(113.1,0.),cmplxpow(cs,(double)2))))));
		chdf = cmplxneg(cmplxdiv(flttocmplx(1.0,0.),(cmplxadd(flttocmplx(1.0,0.),
		 cmplxmul(flttocmplx(0.001592,0.),cs)))));
		cha = cmplxdiv(flttocmplx(1.0,0.),(cmplxadd(flttocmplx(1.0,0.),cmplxmul(flttocmplx(0.00159,0.),
		 cs))));
		chb = cmplxdiv(flttocmplx(1.0,0.),(cmplxadd(cmplxadd(flttocmplx(1.0,0.),cmplxmul(flttocmplx(0.195,0.),
		 cs)),cmplxmul(flttocmplx(0.00990,0.),cmplxpow(cs,(double)2)))));
		chc = cmplxdiv(cs,(cmplxmul((cmplxadd(flttocmplx(1.0,0.),cmplxmul(flttocmplx(0.00153,0.),
		 cs))),(cmplxadd(flttocmplx(1.0,0.),cmplxmul(flttocmplx(5.11,0.),cs))))));
		chd = cmplxdiv(flttocmplx(1.0,0.),(cmplxadd(cmplxadd(flttocmplx(1.0,0.),cmplxmul(flttocmplx(0.154,0.),
		 cs)),cmplxmul(flttocmplx(0.00888,0.),cmplxpow(cs,(double)2)))));
		che = cmplxdiv(flttocmplx(1.0,0.),(cmplxadd(cmplxadd(flttocmplx(1.0,0.),cmplxmul(flttocmplx(0.0817,0.),
		 cs)),cmplxmul(flttocmplx(0.0069,0.),cmplxpow(cs,(double)2)))));
		chf = cmplxdiv(cs,(cmplxmul((cmplxadd(flttocmplx(1.0,0.),cmplxmul(flttocmplx(0.00419,0.),
		 cs))),(cmplxadd(flttocmplx(1.0,0.),cmplxmul(flttocmplx(5.11,0.),cs))))));
		gkip = 129.5;
		chipf = cmplxneg(cmplxmul(cmplxmul(cmplxmul(cmplxmul(cmplxmul(cmplxmul(flttocmplx(gkip,0.),
		 cha),chb),chc),chd),che),chf));
		ch = cmplxmul(cmplxmul(cmplxmul(cmplxmul(cmplxmul(chkv,chbp2),chdf),chipf),
		 cs),flttocmplx(1.0e-9,0.));
		Xre[idx] = cmplxtof( ch );
		Xim[idx] = aimag( ch );
	    }
	    return;

	}
	else if( memcmp(subtyp,"SP      ",8) == 0 ){
	    for( idx = 1; idx <= nfreq; idx++ ){
		omega = (float)( idx - 1 )*delomg;
		cs = flttocmplx( 0.0, omega );
		chs3 = cmplxdiv(cmplxmul(flttocmplx(55.73,0.),cmplxpow(cs,(double)2)),
		 (cmplxadd(cmplxadd(flttocmplx(1.0,0.),cmplxmul(flttocmplx(0.2387,0.),cs)),
		 cmplxmul(flttocmplx(0.02533,0.),cmplxpow(cs,(double)2)))));
		chif = cmplxdiv(flttocmplx(0.9174,0.),(cmplxadd(cmplxadd(flttocmplx(1.0,0.),
		 cmplxmul(flttocmplx(8.165e-4,0.),cs)),cmplxmul(flttocmplx(7.339e-7,0.),
		 cmplxpow(cs,(double)2)))));
		chpa = cmplxdiv(cmplxmul(flttocmplx(5.544,0.),cs),(cmplxmul((cmplxadd(flttocmplx(1.0,0.),
		 cmplxmul(flttocmplx(1.559e-3,0.),cs))),(cmplxadd(flttocmplx(1.0,0.),cmplxmul(flttocmplx(0.1598,0.),
		 cs))))));
		go = 50.0;
		chi = cmplxdiv(flttocmplx(1.0,0.),(cmplxadd(flttocmplx(1.0,0.),cmplxmul(flttocmplx(7.950e-4,0.),
		 cs))));
		ch1 = cmplxdiv(flttocmplx(1.0,0.),(cmplxadd(cmplxadd(flttocmplx(1.0,0.),cmplxmul(flttocmplx(0.0221,0.),
		 cs)),cmplxmul(flttocmplx(0.000125,0.),cmplxpow(cs,(double)2)))));
		ch2 = cmplxdiv(flttocmplx(1.0,0.),(cmplxadd(cmplxadd(flttocmplx(1.0,0.),cmplxmul(flttocmplx(0.0195,0.),
		 cs)),cmplxmul(flttocmplx(0.000119,0.),cmplxpow(cs,(double)2)))));
		ch3 = cmplxdiv(flttocmplx(1.0,0.),(cmplxadd(cmplxadd(flttocmplx(1.0,0.),cmplxmul(flttocmplx(0.0143,0.),
		 cs)),cmplxmul(flttocmplx(0.000104,0.),cmplxpow(cs,(double)2)))));
		ch4 = cmplxdiv(flttocmplx(1.0,0.),(cmplxadd(cmplxadd(flttocmplx(1.0,0.),cmplxmul(flttocmplx(0.00739,0.),
		 cs)),cmplxmul(flttocmplx(0.0000820,0.),cmplxpow(cs,(double)2)))));
		ch5 = cmplxdiv(flttocmplx(1.0,0.),(cmplxadd(cmplxadd(flttocmplx(1.0,0.),cmplxmul(flttocmplx(0.0150,0.),
		 cs)),cmplxmul(flttocmplx(0.000113,0.),cmplxpow(cs,(double)2)))));
		cho = cmplxdiv(cmplxmul(flttocmplx(0.03186,0.),cs),(cmplxmul((cmplxadd(flttocmplx(1.0,0.),
		 cmplxmul(flttocmplx(0.1593,0.),cs))),(cmplxadd(flttocmplx(1.0,0.),cmplxmul(flttocmplx(1.416e-3,0.),
		 cs))))));
		chsp = cmplxneg(cmplxmul(cmplxmul(cmplxmul(cmplxmul(cmplxmul(cmplxmul(cmplxmul(flttocmplx(go,0.),
		 chi),ch1),ch2),ch3),ch4),ch5),cho));
		chdf = cmplxneg(cmplxdiv(flttocmplx(1.0,0.),(cmplxadd(flttocmplx(1.0,0.),
		 cmplxmul(flttocmplx(0.001592,0.),cs)))));
		ch = cmplxmul(cmplxmul(cmplxmul(cmplxmul(cmplxmul(cmplxmul(chs3,chif),chpa),
		 chsp),chdf),cs),flttocmplx(1.0e-9,0.));
		Xre[idx] = cmplxtof( ch );
		Xim[idx] = aimag( ch );
	    }
	    return;
	}
	else{
	    nerr = 2105;
	    setmsg( "ERROR", nerr );
	    apcmsg( "NORESS:",8 );
	    apcmsg( subtyp,subtyp_s );
	    return;
	}

} /* end of function */

