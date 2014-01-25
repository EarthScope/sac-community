
#include <math.h>
#include <float.h>

#include "icm.h"
#include "bool.h"

/* balance and support routines are based on the ALGOL
   routine published by B. N. Parlett and C. Reinsch
   in Numerische Mathematik vol. 13 (1969).  Article titled:
    Balancing a Matrix for Calculation of Eigenvalues and Eigenvectors */

/* exchange two rows and two columns in a square matrix. */
void exchange ( double **matrix , int kdx , int mdx , int order )
{
    int idx ;
    double dumb ;

    for ( idx = 0 ; idx < order ; idx++ ) {
	dumb = matrix[ idx ][ mdx ] ;
	matrix[ idx ][ mdx ] = matrix[ idx ][ kdx ] ;
	matrix[ idx ][ kdx ] = dumb ;

	dumb = matrix[ mdx ][ idx ] ;
	matrix[ mdx ][ idx ] = matrix[ kdx ][ idx ] ;
	matrix[ kdx ][ idx ] = dumb ;
    }
} /* end exchange */

/* return TRUE if specified row or column have zero norm. 
         FALSE if both have whole number norms */
int p1norm0 ( double **matrix , int kdx , int order )
{
    int idx ;
    double rNorm = 0 , cNorm = 0 ;

    for ( idx = 0 ; idx < order ; idx++ ) {
	if ( idx != kdx ) {
	    rNorm += fabs( matrix[ kdx ][ idx ] ) ;
	    cNorm += fabs( matrix[ idx ][ kdx ] ) ;
	}
    }

    return ( rNorm && cNorm ? FALSE : TRUE ) ;
    
} /* end p1norm0 */


/* The first step is to locate rows or columns with null off-diagonal elements.
   The diagonal elements of these are eigenvalues.  These rows and columns (and
   the corresponding columns and rows) are move to the bottom and the right of
   the matrix.  In cases where null columns and rows are not expected, this can
   serve as an error check.  In cases were null columns and rows are possible,
   the return value separates the rows and columns which need hqr to determine
   eigenvalues, and those wherein the eigenvalues are simply the diagonal
   elements. */

int balance ( double **matrix , int order )
{
    int idx , jdx , mOrder ;
    double cNorm , f , g , rNorm , s ;
    const double radix = FLT_RADIX , radix2 = FLT_RADIX * FLT_RADIX ;
    const double gamma = 0.95 ;
    int noconv ;

    /* initialize values. */
    mOrder = order ;

    /* if a row or column's off diagonal elements are all zero,
       the diagonal element is an eigenvalue, and the row and
       column can be removed from the matrix. Here, we push them
       to the right or bottom of the matrix. */
    for ( idx = order-1 ; idx >= 0 ; idx-- ) {
	if ( p1norm0 ( matrix , idx , order ) ) {
	    mOrder-- ;
	    if ( idx < mOrder )
		exchange ( matrix , idx , mOrder , order ) ;
	} /* end if ( p1norm0 ) */
    } /* end for ( idx ) */

    /* Now balance submatrix in rows 0 thru mOrder */

    do {
	noconv = FALSE ; /* flag to exit loop */

	/* loop over digonal elements. */
	for ( idx = 0 ; idx < mOrder ; idx++ )
	{
	    cNorm = rNorm = 0 ;

	    /* loop over off diagonal elements in given row and column
	       to determine row and column norms. */
	    for ( jdx = 0 ; jdx < mOrder ; jdx++ ) {
		if ( jdx == idx ) 
		    continue ;

		cNorm += fabs ( matrix[ jdx ][ idx ] ) ;
		rNorm += fabs ( matrix[ idx ][ jdx ] ) ;
	    } /* end for ( jdx ) */

	    /* initialize values for coming calculations. */
	    g = rNorm / radix ;
	    f = 1 ;
	    s = cNorm + rNorm ;

	    /* cNorm is modified by the radix squared to meet the 
		conditions of the equation.  (see the paper). */
	    while ( cNorm < g ) {	/* make sure cNorm is big enough */
		f *= radix ;
		cNorm *= radix2 ;
	    }

	    g = rNorm * radix ;

	    while ( cNorm >= g ) {	/* make sure cNorm is small enough */
		f /= radix ;
		cNorm /= radix2 ;
	    }

	    /* If matrix isn't balance, set noconv to indicate so, and 
		do the modification at hand */
	    if ( ( cNorm + rNorm ) / f < gamma * s ) {
		noconv = TRUE ;
		g = 1 / f ;

		/* similarity transforms */
		for ( jdx = 0 ; jdx < mOrder ; jdx++ ) {
		    matrix[ idx ][ jdx ] *= g ;
		    matrix[ jdx ][ idx ] *= f ;
		} /* end for ( jdx ) */
	    } /* end if ( ( cNorm + rNorm ) / f < gamma * s ) */
	} /* end for ( idx ) */

    } while ( noconv ) ;

    return mOrder ;

} /* end balance */
