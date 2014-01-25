
#include <stdlib.h>

#include "icm.h"

#include "wtofd.h"


int balance ( double **matrix , int order ) ;
void hqr ( double **matrix, int order, struct roots theseRoots[], int *nerr );

void getroots ( float in[] , int order , struct roots theseRoots[] , int *nerr )
{
    int idx , jdx ;
    double **hessenberg ;

    /* allocate hessenberg matrix */
    hessenberg = (double **) malloc ( order * sizeof ( double * ) ) ;

    if ( hessenberg == NULL ) {
	*nerr = 301 ;
	return ;
    }

    for ( idx = 0 ; idx < order ; idx++ ) {
	hessenberg[ idx ] = (double *) calloc ( order , sizeof ( double ) ) ;

	if ( hessenberg[ idx ] == NULL ) {
	    *nerr = 301 ;

	    for ( ; idx >= 0 ; idx-- )
		free ( hessenberg[ idx ] ) ;

	    free ( hessenberg ) ;

	    return ;
	}
    } /* end for */

    /* fill the hessenberg matrix, one column at a time. */
    /* idx indexes the column, jdx indexes the row. */
    for ( idx = 0 ; idx < order ; idx++ ) {
	/* first row first */
	hessenberg[ 0 ][ idx ] = in[ idx ] / in[ order ] ;
	/* then the rest of the column */
	for ( jdx = 1 ; jdx < order ; jdx++ ) {
	    hessenberg[ jdx ][ idx ] = jdx-1 == idx ? 1 : 0 ;
	} /* end for ( jdx ) */
    } /* end for ( idx ) */

    /* balance the matrix to reduce error */
    balance ( hessenberg , order ) ;

    /* get the polinomial */
    hqr ( hessenberg , order , theseRoots , nerr ) ;
} /* end getroots */


