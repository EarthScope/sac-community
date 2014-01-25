
#include <math.h>

#include "xyz.h"
#include "co.h"

void /*FUNCTION*/ zrvfft(x, m)
float x[];
int m;
{
	int i, i0, id, is, j, k, 
	 n, n2, n4;
	float t1;

	float *const X = &x[0] - 1;



	n = ipow(2,m);
	/*-------Digit reverse counter-----------------------------------C                */
	j = 1;
	for( i = 1; i <= (n - 1); i++ ){
		if( i >= j )
			goto L_10;
		t1 = X[j];
		X[j] = X[i];
		X[i] = t1;
L_10:
		k = n/2;
L_20:
		if( k >= j )
			goto L_30;
		j = j - k;
		k = k/2;
		goto L_20;
L_30:
		j = j + k;
		}
	/*-----Length two butterflies------------------------------------C                */
	is = 1;
	id = 4;
L_50:
	for( i0 = is; i0 <= n; i0 += id ){
		t1 = X[i0];
		X[i0] = t1 + X[i0 + 1];
		X[i0 + 1] = t1 - X[i0 + 1];
		}
	is = 2*id - 1;
	id = 4*id;
	if( is < n )
		goto L_50;
	/*-------L shaped butterflies------------------------------------C                */
	n2 = 2;
	for( k = 2; k <= m; k++ ){
		n2 = n2*2;
		n4 = n2/4;
		zstage( n, n2, n4, &X[1], &X[n4 + 1], &X[2*n4 + 1], &X[3*n4 + 1] );
		}

	return;
} /* end of function */

void /*FUNCTION*/ zirvfft(x, m)
float x[];
int m;
{
	int i, i1, id, is, j, k, 
	 n, n2, n4;
	float t1;

	float *const X = &x[0] - 1;


	/*-------L shaped butterflies-------------------------------------C               */
	n = ipow(2,m);
	n2 = 2*n;
	for( k = 1; k <= (m - 1); k++ ){
		n2 = n2/2;
		n4 = n2/4;
		zistage( n, n2, n4, &X[1], &X[n4 + 1], &X[2*n4 + 1], &X[3*n4 + 1] );
		}
	/*-----Length two butterflies-----------------------------------C                 */
	is = 1;
	id = 4;
L_70:
	for( i1 = is; i1 <= n; i1 += id ){
		t1 = X[i1];
		X[i1] = t1 + X[i1 + 1];
		X[i1 + 1] = t1 - X[i1 + 1];
		}
	is = 2*id - 1;
	id = 4*id;
	if( is < n )
		goto L_70;
	/*-------Digit reverse counter-----------------------------------C                */
	j = 1;
	for( i = 1; i <= (n - 1); i++ ){
		if( i >= j )
			goto L_101;
		t1 = X[j];
		X[j] = X[i];
		X[i] = t1;
L_101:
		k = n/2;
L_102:
		if( k >= j )
			goto L_103;
		j = j - k;
		k = k/2;
		goto L_102;
L_103:
		j = j + k;
		}
	/*-------Divide by N---------------------------------------------C                */
	for( i = 1; i <= n; i++ ){
		X[i] = X[i]/n;
		}
	return;
} /* end of function */

void /*FUNCTION*/ zstage(n, n2, n4, x1, x2, x3, x4)
int n, n2, n4;
float x1[], x2[], x3[], x4[];
{
	int i1, 
	 i2, id, is, j, jn, n8;
	float cc1, cc3, cd1, cd3, e, sd1, sd3, ss1, ss3, t1, t2, t3, t4, 
	 t5, t6;

	float *const X1 = &x1[0] - 1;
	float *const X2 = &x2[0] - 1;
	float *const X3 = &x3[0] - 1;
	float *const X4 = &x4[0] - 1;


	/*===============================================================C               
	 *   Subroutine ZSTAGE - the work-horse of the FFT                C               
	 *===============================================================C                */
	n8 = n2/8;
	is = 0;
	id = n2*2;
L_10:
	for( i1 = is + 1; i1 <= n; i1 += id ){
		t1 = X4[i1] + X3[i1];
		X4[i1] = X4[i1] - X3[i1];
		X3[i1] = X1[i1] - t1;
		X1[i1] = X1[i1] + t1;
		}
	is = 2*id - n2;
	id = 4*id;
	if( is < n )
		goto L_10;

        if((n4-1) <= 0) goto L_100;

	is = 0;
	id = n2*2;
L_40:
	for( i2 = is + 1 + n8; i2 <= n; i2 += id ){
		t1 = (X3[i2] + X4[i2])*.7071067811865475;
		t2 = (X3[i2] - X4[i2])*.7071067811865475;
		X4[i2] = X2[i2] - t1;
		X3[i2] = -X2[i2] - t1;
		X2[i2] = X1[i2] - t2;
		X1[i2] = X1[i2] + t2;
		}
	is = 2*id - n2;
	id = 4*id;
	if( is < n )
		goto L_40;

        if((n8-1) <= 0) goto L_100;

	e = 2.*3.14159265358979323/n2;
	ss1 = sin( e );
	sd1 = ss1;
	sd3 = 3.*sd1 - 4.*powi(sd1,3);
	ss3 = sd3;
	cc1 = cos( e );
	cd1 = cc1;
	cd3 = 4.*powi(cd1,3) - 3.*cd1;
	cc3 = cd3;
	for( j = 2; j <= n8; j++ ){
		is = 0;
		id = 2*n2;
		jn = n4 - 2*j + 2;
L_70:
		for( i1 = is + j; i1 <= n; i1 += id ){
			i2 = i1 + jn;
			t1 = X3[i1]*cc1 + X3[i2]*ss1;
			t2 = X3[i2]*cc1 - X3[i1]*ss1;
			t3 = X4[i1]*cc3 + X4[i2]*ss3;
			t4 = X4[i2]*cc3 - X4[i1]*ss3;
			t5 = t1 + t3;
			t6 = t2 + t4;
			t3 = t1 - t3;
			t4 = t2 - t4;
			t2 = t6 + X2[i2];
			X3[i1] = t6 - X2[i2];
			X4[i2] = t2;
			t2 = X2[i1] - t3;
			X3[i2] = -X2[i1] - t3;
			X4[i1] = t2;
			t1 = X1[i1] + t5;
			X2[i2] = X1[i1] - t5;
			X1[i1] = t1;
			t1 = X1[i2] + t4;
			X1[i2] = X1[i2] - t4;
			X2[i1] = t1;
			}
		is = 2*id - n2;
		id = 4*id;
		if( is < n )
			goto L_70;

		t1 = cc1*cd1 - ss1*sd1;
		ss1 = cc1*sd1 + ss1*cd1;
		cc1 = t1;
		t3 = cc3*cd3 - ss3*sd3;
		ss3 = cc3*sd3 + ss3*cd3;
		cc3 = t3;
		}

L_100:
	return;
} /* end of function */

void /*FUNCTION*/ zistage(n, n2, n4, x1, x2, x3, x4)
int n, n2, n4;
float x1[], x2[], x3[], x4[];
{
	int i1, 
	 i2, id, is, j, jn, n8;
	float cc1, cc3, cd1, cd3, e, sd1, sd3, ss1, ss3, t1, t2, t3, t4, 
	 t5;

	float *const X1 = &x1[0] - 1;
	float *const X2 = &x2[0] - 1;
	float *const X3 = &x3[0] - 1;
	float *const X4 = &x4[0] - 1;


	/*===============================================================C               
	 *   Subroutine ZISTAGE - the work-horse of the IFFT              C               
	 *===============================================================C                */
	n8 = n4/2;
	is = 0;
	id = 2*n2;
L_10:
	for( i1 = is + 1; i1 <= n; i1 += id ){
		t1 = X1[i1] - X3[i1];
		X1[i1] = X1[i1] + X3[i1];
		X2[i1] = 2*X2[i1];
		t2 = 2*X4[i1];
		X4[i1] = t1 + t2;
		X3[i1] = t1 - t2;
		}
	is = 2*id - n2;
	id = 4*id;
	if( is < n )
		goto L_10;

        if((n4-1) <= 0) goto L_100;

	is = 0;
	id = 2*n2;
L_40:
	for( i1 = is + 1 + n8; i1 <= n; i1 += id ){
		t1 = (X2[i1] - X1[i1])*1.4142135623730950488;
		t2 = (X4[i1] + X3[i1])*1.4142135623730950488;
		X1[i1] = X1[i1] + X2[i1];
		X2[i1] = X4[i1] - X3[i1];
		X3[i1] = -t2 - t1;
		X4[i1] = -t2 + t1;
		}
	is = 2*id - n2;
	id = 4*id;
	if( is < n - 1 )
		goto L_40;

        if((n8-1) <= 0)goto L_100;

	e = 6.283185307179586/n2;
	ss1 = sin( e );
	sd1 = ss1;
	sd3 = 3.*sd1 - 4.*powi(sd1,3);
	ss3 = sd3;
	cc1 = cos( e );
	cd1 = cc1;
	cd3 = 4.*powi(cd1,3) - 3.*cd1;
	cc3 = cd3;
	for( j = 2; j <= n8; j++ ){
		is = 0;
		id = 2*n2;
		jn = n4 - 2*j + 2;
L_70:
		for( i1 = is + j; i1 <= n; i1 += id ){
			i2 = i1 + jn;
			t1 = X1[i1] - X2[i2];
			X1[i1] = X1[i1] + X2[i2];
			t2 = X1[i2] - X2[i1];
			X1[i2] = X2[i1] + X1[i2];
			t3 = X4[i2] + X3[i1];
			X2[i2] = X4[i2] - X3[i1];
			t4 = X4[i1] + X3[i2];
			X2[i1] = X4[i1] - X3[i2];
			t5 = t1 - t4;
			t1 = t1 + t4;
			t4 = t2 - t3;
			t2 = t2 + t3;
			X3[i1] = t5*cc1 + t4*ss1;
			X3[i2] = -t4*cc1 + t5*ss1;
			X4[i1] = t1*cc3 - t2*ss3;
			X4[i2] = t2*cc3 + t1*ss3;
			}
		is = 2*id - n2;
		id = 4*id;
		if( is < n )
			goto L_70;

		t1 = cc1*cd1 - ss1*sd1;
		ss1 = cc1*sd1 + ss1*cd1;
		cc1 = t1;
		t3 = cc3*cd3 - ss3*sd3;
		ss3 = cc3*sd3 + ss3*cd3;
		cc3 = t3;
		}
L_100:
	return;
} /* end of function */

