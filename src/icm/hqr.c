
#include <math.h>
#include <float.h>

#include "icm.h"

#include "wtofd.h"



#include "msg.h"

/* Translated from an ALGOL routine published by R.S. Martin, G.Peters, and
   J.H. Wilkinson in Numerische Mathematik vol. 14 (1970).  Article titled:
     The QR Algorithm for RealHessenberg Matrices. */

void hqr ( double **matrix , int order , struct roots theseRoots[] , /*int *cnt ,*/ int *nerr )
{
    /* Finds eigen values of a real upper Hessenberg matrix, and stores the
	real parts and imaginary parts in the structure theseRoots.  
	macheps is the relative machine precision.  The procedure fails if
	any eigenvalue takes more than 30 iterations.  */

    int idx, jdx, kdx, ldx, mdx, na, na_, numw, order_, its ;
    int notlast ;
    double p, q, r, ss, t = 0.0 , w, x, y, z;
    const double macheps = DBL_EPSILON ;

    *nerr = 0 ;

    p = 0;
    q = 0;
    r = 0;

    if ( order == 0 ) {
	*nerr = 1618 ;
	setmsg ( "ERROR" , *nerr ) ;
	outmsg () ;
	clrmsg () ;
	return ;
    }

    while ( order > 0 ) {

	its = 0 ;
	order_ = na = order - 1 ;
	na_ = na - 1 ;

	while( 1 ) {   /* look for single small sub-diagonal element */
	    for ( ldx = order_ ; ldx > 0 ; ldx-- ) {
		if ( fabs ( matrix[ ldx ][ ldx-1 ] ) <=
		     macheps * fabs ( matrix[ ldx-1 ][ ldx-1 ] ) +
		     fabs ( matrix[ ldx ][ ldx ] ) )
		    break ;
	    } /* end for ( ldx ) */


	    x = matrix[ order_ ][ order_ ] ;
	    if ( ldx == order_ ) {
		numw = 1 ;
		break ;
	    }
	    y = matrix[ na_ ][ na_ ] ;
	    w = matrix[ order_ ][ na_ ] * matrix[ na_ ][ order_ ] ;
	    if ( ldx == na_ ) {
		numw = 2 ;
		break ;
	    }
	    if ( its >= 30 ) {
		*nerr = 1619 ;
		setmsg ( "ERROR" , *nerr ) ;
		outmsg () ;
		clrmsg () ;
		return ;
	    }

	    if ( its == 10 || its == 20 ) {
		/* form exceptional shift */
		t += x ;
		for ( idx = 0 ; idx < order ; idx++ )
		    matrix[ idx ][ idx ] -= x ;
		ss = fabs ( matrix[ order_ ][ na_] ) +
		     fabs ( matrix[ na_ ][ order_ - 2 ] ) ;
		x = y = 0.75 * ss ;
		w = -0.4375 * ss * ss ;
	    } /* end if ( its == 10 || its == 20 ) */

	    its++ ;

	    /* look for two consecutive small sub-diagonal elements. */
	    for ( mdx = order_ - 2 ; mdx >= ldx ; mdx-- ) {
		z = matrix[ mdx ][ mdx ] ;
		r = x - z ;
		ss = y - z ;
		p = ( r * ss - w ) / matrix[ mdx+1 ][ mdx ] +
				    matrix[ mdx ][ mdx+1 ] ;
		q = matrix[ mdx+1 ][ mdx+1 ] - z - r - ss ;
		r = matrix[ mdx+2 ][ mdx+1 ] ;
		ss = fabs ( p ) + fabs ( q ) + fabs ( r ) ;
		p /= ss ;
		q /= ss ;
		r /= ss ;
		if ( mdx == ldx )
		    break ;
		if ( fabs( matrix[ mdx ][ mdx-1 ] ) * ( fabs(q) + fabs(r) ) <=
		   ( fabs( matrix[ mdx-1 ][ mdx-1 ] ) + fabs( z ) +
		     fabs( matrix[ mdx+1 ][ mdx+1 ] ) ) * fabs( p ) * macheps ) 
		    break ;
	    } /* end for ( mdx ) */


	    for ( idx = mdx + 2 ; idx < order_ ; idx++ )
		matrix[ idx ][ idx-2 ] = 0.0 ;
	    for ( idx = mdx + 3 ; idx < order_ ; idx++ )
		matrix[ idx ][ idx-3 ] = 0.0 ;

	    /* double QR step involving rows ldx to order and
					columns mdx to order */
	    for ( kdx = mdx ; kdx <= na_ ; kdx++ ) {
		notlast = kdx != na_ ;
		if ( kdx != mdx ) {
		    r = notlast ? matrix[ kdx+2 ][ kdx-1 ] : 0.0 ;
		    x = fabs( p ) + fabs( q ) + fabs( r ) ;
		    if ( !x ) 
			continue ;
		    p /= x ;
		    q /= x ;
		    r /= x ;
		} /* end if ( kdx != mdx ) */
		ss = sqrt( p*p + q*q + r*r ) ;
		if ( p < 0 ) 
		    ss = -ss ;
		if ( kdx != mdx )
		    matrix[ kdx ][ kdx-1 ] = -ss * x ;
		else if ( ldx != mdx )
		    matrix[ kdx ][ kdx-1 ] = -matrix[ kdx ][ kdx-1 ] ;
		p += ss ;
		x = p / ss ;
		y = q / ss ;
		z = r / ss ;
		q /= p ;
		r /= p ;

		/* row modification */
		for ( jdx = kdx ; jdx <= order_ ; jdx++ ) {
		    p = matrix [ kdx ][ jdx ] + q * matrix[ kdx+1 ][ jdx ] ;
		    if( notlast ) {
			p += r * matrix[ kdx+2 ][ jdx ] ;
			matrix[ kdx+2 ][ jdx ] -= p * z ;
		    } /* end if ( notlast ) */
		    matrix[ kdx+1 ][ jdx ] -= p * y ;
		    matrix[ kdx ][ jdx ] -= p * x ;
		} /* end for ( jdx ) */

		jdx = kdx+3 < order_ ? kdx+3 : order_ ;

		/* column modification */
		for ( idx = ldx ; idx <= jdx ; idx++ ) {
		    p = x * matrix[ idx ][ kdx ] + y * matrix[ idx ][ kdx+1 ] ;
		    if ( notlast ) {
			p += z * matrix[ idx ][ kdx+2 ] ;
			matrix[ idx ][ kdx+2 ] -= p * r ;
		    }
		    matrix[ idx ][ kdx+1 ] -= p * q ;
		    matrix[ idx ][ kdx ] -= p ;
		} /* end for( idx ) */

	    } /* end for( kdx ) */

	} /* end while( 1 ) */

	if ( numw == 1 ) {
	    /* one root found */
	    theseRoots[ order_ ].real = x + t ;
	    theseRoots[ order_ ].imag = 0 ;
	   /* cnt[ order_ ] = its ; */
	    order-- ;
	}

	else if ( numw == 2 ) {
	    /* two roots found */
	    p = ( y - x ) / 2 ;
	    q = p*p + w ;
	    y = sqrt( fabs( q ) ) ;
	    /*cnt[ order_ ] = -its ; */
	    /*cnt[ na_ ] = its ; */
	    x += t ;
	    if ( q > 0 ) {
		/* real pair */
		if ( p < 0 )
		    y = -y ;
		y += p ;
		theseRoots[ na_ ].real = x + y ;
		theseRoots[ order_ ].real = x - w / y ;
		theseRoots[ na_ ].imag = theseRoots[ order_ ].imag = 0.0 ;
	    } /* end if ( p < 0 ) */
	    else {
		/* complex pair */
		theseRoots[ na_ ].real = theseRoots[ order_ ].real = x + p ;
		theseRoots[ na_ ].imag = y ;
		theseRoots[ order_ ].imag = -y ;
	    } /* end else */

	    order -= 2 ;
	} /* end else if ( numw == 2 ) */
    } /* end while ( order > 0 ) */

} /* end hqr */
