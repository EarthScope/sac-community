
#include <stdio.h>
#include <math.h>

#include "complex.h"
#include "dbh.h"
#include "co.h"

void ceigvv ( int nd, 
              int n, 
              complexf *a, 
              float *e, 
              complexf *z, 
              int *ierr);
void htridi ( int nm, 
              int n, 
              double *ar, 
              double *ai, 
              double *d, 
              double *e, 
              double *e2, 
              double tau[][2]);
void htribk ( int nm, 
              int n, 
              double *ar, 
              double *ai, 
              double tau[][2], 
              int m, 
              double *zr, 
              double *zi);
double pythag ( double a, 
                double b);
void tql2 ( int nm, 
            int n, 
            double *d, 
            double *e, 
         double *z, 
            int *ierr);

/*     CEIGVV.F 
 *
 *     SUBROUTINE TO CALCULATE THE EIGENVALES AND EIGENVECTORS
 *     OF A COMPLEX HERMITIAN MATRIX
 *
 *
 *
 *     INPUT PARAMETERS:-
 *
 *     ND  -  DIMENSION OF THE ARRAYS IN THE CALLING PROGRAM
 *     N   -  SIZE OF THE CURRENT ARRAYS
 *     A   -  THE 2-D MATRIX COMPLEX MATRIX
 *
 *     OUTPUT PARAMETERS:-
 *
 *     E  - EIGENVALUES IN ASCENDING ORDER
 *     Z  - COMPLEX EIGENVECTORS 
 *       EXAMPLE: MAXIMUM EIGENVALUE = E(1)
 *                MAXIMUM EIGENVECTOR = (Z(J,1) FOR J=1,N)
 *     IERR - IF IERR IS EQUAL TO 0 THEN SUCCESSFUL COMPUTATION
 * */
void 
ceigvv(int        nd, 
       int        n, 
       complexf  *a, 
       float     *e, 
       complexf  *z, 
       int       *ierr)
{
#define A(I_,J_)	(*(a+(I_)*(nd)+(J_)))
#define Z(I_,J_)	(*(z+(I_)*(nd)+(J_)))
	int i, i_, j, j_, nm;
	double ai[25][25], ar[25][25], fm1[25][2], fv1[25], w[25], zi[25][25], 
	 zr[25][25];

	float *const E = &e[0] - 1;
	double *const W = &w[0] - 1;

	nm = 25;
	*ierr = 0;
	if( n > 25 ){
		*ierr = -1;
		fprintf( stdout, "CEIGVV: DIMENSION TOO LARGE\n" );
		goto L_77777;
		}

	/*     CONVERT TO DOUBLE PRECISION
	 * */
	for( i = 1; i <= n; i++ ){
		i_ = i - 1;
		for( j = 1; j <= n; j++ ){
			j_ = j - 1;
			ar[j_][i_] = (double)( cmplxtof( A(j_,i_) ) );
			ai[j_][i_] = (double)( aimag( A(j_,i_) ) );
			}
		}

	htridi( nm, n, (double*)ar, (double*)ai, w, fv1, fv1, fm1 );

	for( i = 1; i <= n; i++ ){
		i_ = i - 1;
		for( j = 1; j <= n; j++ ){
			j_ = j - 1;
			zr[j_][i_] = 0.0;
			}
		zr[i_][i_] = 1.0;
		}

	tql2( nm, n, w, fv1, (double*)zr, ierr );

	if( *ierr != 0 )
		goto L_77777;

	htribk( nm, n, (double*)ar, (double*)ai, fm1, n, (double*)zr, 
	 (double*)zi );

	for( i = 1; i <= n; i++ ){
		i_ = i - 1;
		E[i] = W[i];
		for( j = 1; j <= n; j++ ){
			j_ = j - 1;
			Z(j_,i_) = flttocmplx( zr[j_][i_], zi[j_][i_] );
			}
		}

	goto L_88888;
L_77777:
	fprintf( stdout, "CEIGVV: IERR WAS NOT EQUAL TO 0\n" );
L_88888:
	;
	return;
#undef	Z
#undef	A
} /* end of function */

/*     THESE SUBROUTINES ARE FROM EISPACK AND/OR MATLAB
 * */
void 
htridi(int      nm, 
       int      n,
       double  *ar, 
       double  *ai, 
       double  *d, 
       double  *e, 
       double  *e2, 
       double   tau[][2])
{
#define AR(I_,J_)	(*(ar+(I_)*(nm)+(J_)))
#define AI(I_,J_)	(*(ai+(I_)*(nm)+(J_)))
	int i, i_, ii, j, j_, jp1, k, k_, l;
	double f, fi, g, gi, h, hh, scale, si;

	double *const D = &d[0] - 1;
	double *const E = &e[0] - 1;
	double *const E2 = &e2[0] - 1;


	/*     THIS SUBROUTINE IS A TRANSLATION OF A COMPLEX ANALOGUE OF 
	 *     THE ALGOL PROCEDURE TRED1, NUM. MATH. 11, 181-195(1968) 
	 *     BY MARTIN, REINSCH, AND WILKINSON.
	 *     HANDBOOK FOR AUTO. COMP., VOL.II-LINEAR ALGEBRA, 212-226(1971). 
	 *
	 *     THIS SUBROUTINE REDUCES A COMPLEX HERMITIAN MATRIX
	 *     TO A REAL SYMMETRIC TRIDIAGONAL MATRIX USING
	 *     UNITARY SIMILARITY TRANSFORMATIONS. 
	 *
	 *     ON INPUT. 
	 *
	 *        NM MUST BE SET TO THE ROW DIMENSION OF TWO-DIMENSIONAL 
	 *          ARRAY PARAMETERS AS DECLARED IN THE CALLING PROGRAM
	 *          DIMENSION STATEMENT. 
	 *
	 *        N IS THE ORDER OF THE MATRIX.
	 *
	 *        AR AND AI CONTAIN THE REAL AND IMAGINARY PARTS,
	 *          RESPECTIVELY, OF THE COMPLEX HERMITIAN INPUT MATRIX. 
	 *          ONLY THE LOWER TRIANGLE OF THE MATRIX NEED BE SUPPLIED.
	 *
	 *     ON OUTPUT.
	 *
	 *        AR AND AI CONTAIN INFORMATION ABOUT THE UNITARY TRANS- 
	 *          FORMATIONS USED IN THE REDUCTION IN THEIR FULL LOWER 
	 *          TRIANGLES.  THEIR STRICT UPPER TRIANGLES AND THE 
	 *          DIAGONAL OF AR ARE UNALTERED.
	 *
	 *        D CONTAINS THE DIAGONAL ELEMENTS OF THE THE TRIDIAGONAL MATRIX.
	 *
	 *        E CONTAINS THE SUBDIAGONAL ELEMENTS OF THE TRIDIAGONAL 
	 *          MATRIX IN ITS LAST N-1 POSITIONS.  E(1) IS SET TO ZERO.
	 *
	 *        E2 CONTAINS THE SQUARES OF THE CORRESPONDING ELEMENTS OF E.
	 *          E2 MAY COINCIDE WITH E IF THE SQUARES ARE NOT NEEDED.
	 *
	 *        TAU CONTAINS FURTHER INFORMATION ABOUT THE TRANSFORMATIONS.
	 *
	 *     MODIFIED TO GET RID OF ALL COMPLEX ARITHMETIC, C. MOLER, 6/27/79. 
	 *
	 *     QUESTIONS AND COMMENTS SHOULD BE DIRECTED TO B. S. GARBOW,
	 *     APPLIED MATHEMATICS DIVISION, ARGONNE NATIONAL LABORATORY 
	 *
	 *     ------------------------------------------------------------------
	 * */
	tau[n - 1][0] = 1.0e0;
	tau[n - 1][1] = 0.0e0;

	for( i = 1; i <= n; i++ ){
		i_ = i - 1;
		D[i] = AR(i_,i_);
		}
	/*     .......... FOR I=N STEP -1 UNTIL 1 DO -- ..........  */
	for( ii = 1; ii <= n; ii++ ){
		i = n + 1 - ii;
		l = i - 1;
		h = 0.0e0;
		scale = 0.0e0;
		if( l < 1 )
			goto L_130;
		/*     .......... SCALE ROW (ALGOL TOL THEN NOT NEEDED) ..........  */
		for( k = 1; k <= l; k++ ){
			k_ = k - 1;
			scale = scale + fabs( AR(k_,i - 1) ) + fabs( AI(k_,i - 1) );
			}

		if( scale != 0.0e0 )
			goto L_140;
		tau[l - 1][0] = 1.0e0;
		tau[l - 1][1] = 0.0e0;
L_130:
		E[i] = 0.0e0;
		E2[i] = 0.0e0;
		goto L_290;

L_140:
		for( k = 1; k <= l; k++ ){
			k_ = k - 1;
			AR(k_,i - 1) = AR(k_,i - 1)/scale;
			AI(k_,i - 1) = AI(k_,i - 1)/scale;
			h = h + AR(k_,i - 1)*AR(k_,i - 1) + AI(k_,i - 1)*AI(k_,i - 1);
			}

		E2[i] = scale*scale*h;
		g = sqrt( h );
		E[i] = scale*g;
		f = pythag( AR(l - 1,i - 1), AI(l - 1,i - 1) );
		/*     .......... FORM NEXT DIAGONAL ELEMENT OF MATRIX T .......... */
		if( f == 0.0e0 )
			goto L_160;
		tau[l - 1][0] = (AI(l - 1,i - 1)*tau[i - 1][1] - AR(l - 1,i - 1)*
		 tau[i - 1][0])/f;
		si = (AR(l - 1,i - 1)*tau[i - 1][1] + AI(l - 1,i - 1)*tau[i - 1][0])/
		 f;
		h = h + f*g;
		g = 1.0e0 + g/f;
		AR(l - 1,i - 1) = g*AR(l - 1,i - 1);
		AI(l - 1,i - 1) = g*AI(l - 1,i - 1);
		if( l == 1 )
			goto L_270;
		goto L_170;
L_160:
		tau[l - 1][0] = -tau[i - 1][0];
		si = tau[i - 1][1];
		AR(l - 1,i - 1) = g;
L_170:
		f = 0.0e0;

		for( j = 1; j <= l; j++ ){
			j_ = j - 1;
			g = 0.0e0;
			gi = 0.0e0;
			/*     .......... FORM ELEMENT OF A*U ..........  */
			for( k = 1; k <= j; k++ ){
				k_ = k - 1;
				g = g + AR(k_,j_)*AR(k_,i - 1) + AI(k_,j_)*AI(k_,i - 1);
				gi = gi - AR(k_,j_)*AI(k_,i - 1) + AI(k_,j_)*AR(k_,i - 1);
				}

			jp1 = j + 1;
			if( l < jp1 )
				goto L_220;

			for( k = jp1; k <= l; k++ ){
				k_ = k - 1;
				g = g + AR(j_,k_)*AR(k_,i - 1) - AI(j_,k_)*AI(k_,i - 1);
				gi = gi - AR(j_,k_)*AI(k_,i - 1) - AI(j_,k_)*AR(k_,i - 1);
				}
			/*     .......... FORM ELEMENT OF P ..........  */
L_220:
			E[j] = g/h;
			tau[j_][1] = gi/h;
			f = f + E[j]*AR(j_,i - 1) - tau[j_][1]*AI(j_,i - 1);
			}

		hh = f/(h + h);
		/*     .......... FORM REDUCED A .......... */
		for( j = 1; j <= l; j++ ){
			j_ = j - 1;
			f = AR(j_,i - 1);
			g = E[j] - hh*f;
			E[j] = g;
			fi = -AI(j_,i - 1);
			gi = tau[j_][1] - hh*fi;
			tau[j_][1] = -gi;

			for( k = 1; k <= j; k++ ){
				k_ = k - 1;
				AR(k_,j_) = AR(k_,j_) - f*E[k] - g*AR(k_,i - 1) + 
				 fi*tau[k_][1] + gi*AI(k_,i - 1);
				AI(k_,j_) = AI(k_,j_) - f*tau[k_][1] - g*AI(k_,i - 1) - 
				 fi*E[k] - gi*AR(k_,i - 1);
				}
			}

L_270:
		for( k = 1; k <= l; k++ ){
			k_ = k - 1;
			AR(k_,i - 1) = scale*AR(k_,i - 1);
			AI(k_,i - 1) = scale*AI(k_,i - 1);
			}

		tau[l - 1][1] = -si;
L_290:
		hh = D[i];
		D[i] = AR(i - 1,i - 1);
		AR(i - 1,i - 1) = hh;
		AI(i - 1,i - 1) = scale*sqrt( h );
		}

	return;
#undef	AI
#undef	AR
} /* end of function */



void
htribk(int      nm, 
       int      n, 
       double  *ar, 
       double  *ai, 
       double   tau[][2], 
       int      m, 
       double  *zr, 
       double  *zi)
{
#define AR(I_,J_)	(*(ar+(I_)*(nm)+(J_)))
#define AI(I_,J_)	(*(ai+(I_)*(nm)+(J_)))
#define ZR(I_,J_)	(*(zr+(I_)*(nm)+(J_)))
#define ZI(I_,J_)	(*(zi+(I_)*(nm)+(J_)))
	int i, i_, j, j_, k, k_, l;
	double h, s, si;



	/*     THIS SUBROUTINE IS A TRANSLATION OF A COMPLEX ANALOGUE OF 
	 *     THE ALGOL PROCEDURE TRBAK1, NUM. MATH. 11, 181-195(1968)
	 *     BY MARTIN, REINSCH, AND WILKINSON.
	 *     HANDBOOK FOR AUTO. COMP., VOL.II-LINEAR ALGEBRA, 212-226(1971). 
	 *
	 *     THIS SUBROUTINE FORMS THE EIGENVECTORS OF A COMPLEX HERMITIAN 
	 *     MATRIX BY BACK TRANSFORMING THOSE OF THE CORRESPONDING
	 *     REAL SYMMETRIC TRIDIAGONAL MATRIX DETERMINED BY  HTRIDI.
	 *
	 *     ON INPUT. 
	 *
	 *        NM MUST BE SET TO THE ROW DIMENSION OF TWO-DIMENSIONAL 
	 *          ARRAY PARAMETERS AS DECLARED IN THE CALLING PROGRAM
	 *          DIMENSION STATEMENT. 
	 *
	 *        N IS THE ORDER OF THE MATRIX.
	 *
	 *        AR AND AI CONTAIN INFORMATION ABOUT THE UNITARY TRANS- 
	 *          FORMATIONS USED IN THE REDUCTION BY  HTRIDI  IN THEIR
	 *          FULL LOWER TRIANGLES EXCEPT FOR THE DIAGONAL OF AR.
	 *
	 *        TAU CONTAINS FURTHER INFORMATION ABOUT THE TRANSFORMATIONS.
	 *
	 *        M IS THE NUMBER OF EIGENVECTORS TO BE BACK TRANSFORMED.
	 *
	 *        ZR CONTAINS THE EIGENVECTORS TO BE BACK TRANSFORMED
	 *          IN ITS FIRST M COLUMNS.
	 *
	 *     ON OUTPUT.
	 *
	 *        ZR AND ZI CONTAIN THE REAL AND IMAGINARY PARTS,
	 *          RESPECTIVELY, OF THE TRANSFORMED EIGENVECTORS
	 *          IN THEIR FIRST M COLUMNS.
	 *
	 *     NOTE THAT THE LAST COMPONENT OF EACH RETURNED VECTOR
	 *     IS REAL AND THAT VECTOR EUCLIDEAN NORMS ARE PRESERVED.
	 *
	 *     QUESTIONS AND COMMENTS SHOULD BE DIRECTED TO B. S. GARBOW,
	 *     APPLIED MATHEMATICS DIVISION, ARGONNE NATIONAL LABORATORY 
	 *
	 *     ------------------------------------------------------------------
	 * */
	if( m == 0 )
		goto L_200;
	/*     .......... TRANSFORM THE EIGENVECTORS OF THE REAL SYMMETRIC 
	 *                TRIDIAGONAL MATRIX TO THOSE OF THE HERMITIAN 
	 *                TRIDIAGONAL MATRIX. ..........  */
	for( k = 1; k <= n; k++ ){
		k_ = k - 1;

		for( j = 1; j <= m; j++ ){
			j_ = j - 1;
			ZI(j_,k_) = -ZR(j_,k_)*tau[k_][1];
			ZR(j_,k_) = ZR(j_,k_)*tau[k_][0];
			}
		}

	if( n == 1 )
		goto L_200;
	/*     .......... RECOVER AND APPLY THE HOUSEHOLDER MATRICES .......... */
	for( i = 2; i <= n; i++ ){
		i_ = i - 1;
		l = i - 1;
		h = AI(i_,i_);
		if( h == 0.0e0 )
			goto L_140;

		for( j = 1; j <= m; j++ ){
			j_ = j - 1;
			s = 0.0e0;
			si = 0.0e0;

			for( k = 1; k <= l; k++ ){
				k_ = k - 1;
				s = s + AR(k_,i_)*ZR(j_,k_) - AI(k_,i_)*ZI(j_,k_);
				si = si + AR(k_,i_)*ZI(j_,k_) + AI(k_,i_)*ZR(j_,k_);
				}
			/*     .......... DOUBLE DIVISIONS AVOID POSSIBLE UNDERFLOW ..........  */
			s = (s/h)/h;
			si = (si/h)/h;

			for( k = 1; k <= l; k++ ){
				k_ = k - 1;
				ZR(j_,k_) = ZR(j_,k_) - s*AR(k_,i_) - si*AI(k_,i_);
				ZI(j_,k_) = ZI(j_,k_) - si*AR(k_,i_) + s*AI(k_,i_);
				}

			}

L_140:
		;
		}

L_200:
	return;
#undef	ZI
#undef	ZR
#undef	AI
#undef	AR
} /* end of function */


struct t_iop {
  int ddt, err, fmt, lct[4], lin[1024], lpt[6], hio, rio, wio, rte;
  FILE *wte;
} iop;

double 
pythag(double a, 
       double b)
{
	double p, pythag_v, q, r, s, t;

	p = fmax( fabs( a ), fabs( b ) );
	q = fmin( fabs( a ), fabs( b ) );
	if( q == 0.0e0 )
		goto L_20;
/* There is no file opened on the following FILE *
   if this is to be used, it needs an open somewhere
	if( iop.ddt == 25 )
		{
                fprintf(iop.wte, "%s\n", "PYTHAG");
		}
	if( iop.ddt == 25 )
		{
                fprintf(iop.wte,"%23.15g%23.15g\n",p,q);
		}
*/
L_10:
	r = powi(q/p,2);
	t = 4.0e0 + r;
	if( t == 4.0e0 )
		goto L_20;
	s = r/t;
	p = p + 2.0e0*p*s;
	q = q*s;
/* Same problem as above
	if( iop.ddt == 25 )
		{
                fprintf(iop.wte,"%23.15g%23.15g\n",p,q);
		}
*/
	goto L_10;
L_20:
	pythag_v = p;
	return( pythag_v );
} /* end of function */


void 
tql2(int      nm, 
     int      n, 
     double  *d, 
     double  *e, 
     double  *z, 
     int     *ierr)
{
#define Z(I_,J_)	(*(z+(I_)*(nm)+(J_)))
	int i, ii, j, j_, k, k_, l, l1, m, mml;
	double b, c, f, g, h, machep, p, r, s;

	double *const D = &d[0] - 1;
	double *const E = &e[0] - 1;



	/*     REAL*8 DSQRT,DABS,DDSIGN
	 *
	 *     THIS SUBROUTINE IS A TRANSLATION OF THE ALGOL PROCEDURE TQL2,
	 *     NUM. MATH. 11, 293-306(1968) BY BOWDLER, MARTIN, REINSCH, AND
	 *     WILKINSON.
	 *     HANDBOOK FOR AUTO. COMP., VOL.II-LINEAR ALGEBRA, 227-240(1971).
	 *
	 *     THIS SUBROUTINE FINDS THE EIGENVALUES AND EIGENVECTORS
	 *     OF A SYMMETRIC TRIDIAGONAL MATRIX BY THE QL METHOD.
	 *     THE EIGENVECTORS OF A FULL SYMMETRIC MATRIX CAN ALSO
	 *     BE FOUND IF  TRED2  HAS BEEN USED TO REDUCE THIS
	 *     FULL MATRIX TO TRIDIAGONAL FORM.
	 *
	 *     ON INPUT-
	 *
	 *        NM MUST BE SET TO THE ROW DIMENSION OF TWO-DIMENSIONAL
	 *          ARRAY PARAMETERS AS DECLARED IN THE OPCALLING PROGRAM
	 *          DIMENSION STATEMENT,
	 *
	 *        N IS THE ORDER OF THE MATRIX,
	 *
	 *        D CONTAINS THE DIAGONAL ELEMENTS OF THE INPUT MATRIX,
	 *
	 *        E CONTAINS THE SUBDIAGONAL ELEMENTS OF THE INPUT MATRIX
	 *          IN ITS LAST N-1 POSITIONS.  E(1) IS ARBITRARY,
	 *
	 *        Z CONTAINS THE TRANSFORMATION MATRIX PRODUCED IN THE
	 *          REDUCTION BY  TRED2, IF PERFORMED.  IF THE EIGENVECTORS
	 *          OF THE TRIDIAGONAL MATRIX ARE DESIRED, Z MUST CONTAIN
	 *          THE IDENTITY MATRIX.
	 *
	 *      ON OUTPUT-
	 *
	 *        D CONTAINS THE EIGENVALUES IN ASCENDING ORDER.  IF AN
	 *          ERROR EXIT IS MADE, THE EIGENVALUES ARE CORRECT BUT
	 *          UNORDERED FOR INDICES 1,2,...,IERR-1,
	 *
	 *        E HAS BEEN DESTROYED,
	 *
	 *        Z CONTAINS ORTHONORMAL EIGENVECTORS OF THE SYMMETRIC
	 *          TRIDIAGONAL (OR FULL) MATRIX.  IF AN ERROR EXIT IS MADE,
	 *          Z CONTAINS THE EIGENVECTORS ASSOCIATED WITH THE STORED
	 *          EIGENVALUES,
	 *
	 *        IERR IS SET TO
	 *          ZERO       FOR NORMAL RETURN,
	 *          J          IF THE J-TH EIGENVALUE HAS NOT BEEN
	 *                     DETERMINED AFTER 30 ITERATIONS.
	 *
	 *     QUESTIONS AND COMMENTS SHOULD BE DIRECTED TO B. S. GARBOW,
	 *     APPLIED MATHEMATICS DIVISION, ARGONNE NATIONAL LABORATORY
	 *
	 *     ------------------------------------------------------------------
	 *
	 *C+---------------------------------------------------------------------
	 *C+
	 *C+                      LAWRENCE LIVERMORE LABORATORY
	 *C+       NUMERICAL MATHEMATICS GROUP -- MATHEMATICAL SOFTWARE LIBRARY
	 *C+
	 *C+---------------------------------------------------------------------
	 *C+
	 *C+                 CLASS ONE ROUTINE:        TQL2
	 *C+                           RELEASE:  2
	 *C+                 DATE LAST CHANGED:  76-10-28
	 *C+
	 *C+   EACH  CLASS ONE  ROUTINE HAS BEEN THOROUGHLY TESTED BY NMG AND MEE
	 *C+   CERTAIN DOCUMENTATION AND PROGRAMMING STANDARDS.
	 *C+
	 *C+   AT LEAST ONE CONSULTANT IS AVAILABLE TO ANSWER QUESTIONS AND RESPO
	 *C+   TO REPORTED ERRORS OR INADEQUACIES IN A  CLASS ONE  ROUTINE.
	 *C+
	 *C+   +-----------------------------------------------------------------
	 *C+   +                           N O T I C E
	 *C+   +
	 *C+   +  THIS ROUTINE IS PART  OF THE NATS PRODUCED PACKAGE 'EISPACK'.
	 *C+   +  NATS IS   THE   NSF  FUNDED  NATIONAL  ACTIVITY  FOR  TESTING
	 *C+   +  SOFTWARE.  THE NATS PROJECT  FULLY  SUPPORTS  THIS  CERTIFIED
	 *C+   +  ROUTINE IN THE SENSE THAT DETAILED INFORMATION ON THE TESTING
	 *C+   +  PROCEDURES IS AVAILABLE  AND  REPORTS  OF  POOR  OR INCORRECT
	 *C+   +  PERFORMANCE ON  AT  LEAST  THE COMPUTER SYSTEMS FOR WHICH THE
	 *C+   +  PACKAGE HAS BEEN CERTIFIED WILL GAIN IMMEDIATE ATTENTION FROM
	 *C+   +  THE DEVELOPERS.   LLL  USERS  SHOULD  CONSULT  UCID-30077 FOR
	 *C+   +  LOCAL ASSISTANCE.  ALL OTHER QUESTIONS AND COMMENTS SHOULD BE
	 *C+   +  DIRECTED TO :
	 *C+   +
	 *C+   +                BURTON S. GARBOW
	 *C+   +                ARGONNE NATIONAL LABORATORY
	 *C+   +                APPLIED MATHEMATICS DIVISION
	 *C+   +                9700 SOUTH CASS AVENUE
	 *C+   +                ARGONNE, IL. 60439
	 *C+   +
	 *C+   +-----------------------------------------------------------------
	 *C+
	 *C+   PLEASE REPORT ANY SUSPECTED ERRORS IN THIS ROUTINE IMMEDIATELY TO
	 *C+   EXT. 3957, 3329, OR 3288.
	 *C+
	 *C+---------------------------------------------------------------------
	 *
	 *
	 *     ********** MACHEP IS A MACHINE DEPENDENT PARAMETER SPECIFYING */
	/*                THE RELATIVE PRECISION OF FLOATING POINT ARITHMETIC.
	 *
	 *                ********** */
	machep = powi(2.0e00,-55);

	*ierr = 0;
	if( n == 1 )
		goto L_1001;

	for( i = 2; i <= n; i++ ){
		E[i - 1] = E[i];
		}

	f = 0.0e0;
	b = 0.0e0;
	E[n] = 0.0e0;

	for( l = 1; l <= n; l++ ){
		j = 0;
		h = machep*(fabs( D[l] ) + fabs( E[l] ));
		if( b < h )
			b = h;
		/*     ********** LOOK FOR SMALL SUB-DIAGONAL ELEMENT ********** */
		for( m = l; m <= n; m++ ){
			if( fabs( E[m] ) <= b )
				goto L_120;
			/*     ********** E(N) IS ALWAYS ZERO, SO THERE IS NO EXIT
			 *                THROUGH THE BOTTOM OF THE LOOP ********** */
			}

L_120:
		if( m == l )
			goto L_220;
L_130:
		if( j == 30 )
			goto L_1000;
		j = j + 1;
		/*     ********** FORM SHIFT ********** */
		l1 = l + 1;
		g = D[l];
		p = (D[l1] - g)/(2.0e0*E[l]);
		r = sqrt( p*p + 1.0e0 );
		D[l] = E[l]/(p + sign( r, p ));
		h = g - D[l];

		for( i = l1; i <= n; i++ ){
			D[i] = D[i] - h;
			}

		f = f + h;
		/*     ********** QL TRANSFORMATION ********** */
		p = D[m];
		c = 1.0e0;
		s = 0.0e0;
		mml = m - l;
		/*     ********** FOR I=M-1 STEP -1 UNTIL L DO -- ********** */
		for( ii = 1; ii <= mml; ii++ ){
			i = m - ii;
			g = c*E[i];
			h = c*p;
			if( fabs( p ) < fabs( E[i] ) )
				goto L_150;
			c = E[i]/p;
			r = sqrt( c*c + 1.0e0 );
			E[i + 1] = s*p*r;
			s = c/r;
			c = 1.0e0/r;
			goto L_160;
L_150:
			c = p/E[i];
			r = sqrt( c*c + 1.0e0 );
			E[i + 1] = s*E[i]*r;
			s = 1.0e0/r;
			c = c*s;
L_160:
			p = c*D[i] - s*g;
			D[i + 1] = h + s*(c*g + s*D[i]);
			/*     ********** FORM VECTOR ********** */
			for( k = 1; k <= n; k++ ){
				k_ = k - 1;
				h = Z(i,k_);
				Z(i,k_) = s*Z(i - 1,k_) + c*h;
				Z(i - 1,k_) = c*Z(i - 1,k_) - s*h;
				}

			}

		E[l] = s*p;
		D[l] = c*p;
		if( fabs( E[l] ) > b )
			goto L_130;
L_220:
		D[l] = D[l] + f;
		}
	/*     ********** ORDER EIGENVALUES AND EIGENVECTORS ********** */
	for( ii = 2; ii <= n; ii++ ){
		i = ii - 1;
		k = i;
		p = D[i];

		for( j = ii; j <= n; j++ ){
			if( D[j] >= p )
				goto L_260;
			k = j;
			p = D[j];
L_260:
			;
			}

		if( k == i )
			goto L_300;
		D[k] = D[i];
		D[i] = p;

		for( j = 1; j <= n; j++ ){
			j_ = j - 1;
			p = Z(i - 1,j_);
			Z(i - 1,j_) = Z(k - 1,j_);
			Z(k - 1,j_) = p;
			}

L_300:
		;
		}

	goto L_1001;
	/*     ********** SET ERROR -- NO CONVERGENCE TO AN
	 *                EIGENVALUE AFTER 30 ITERATIONS ********** */
L_1000:
	*ierr = l;
L_1001:
	return;
	/*     ********** LAST CARD OF TQL2 ********** */
#undef	Z
} /* end of function */


