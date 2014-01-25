
#include "sss.h"
#include "bool.h"

    /*=====================================================================
     * PURPOSE:  Returns time value of ttime curve at a given distance.
     *=====================================================================
     * INPUT ARGUMENTS:
     *	float * distArray:  array of distances of the traveltime curve.
     *	float * time:       array of times of the traveltime curve.
     *	int nPoints:        number of points in traveltime curve.
     *	float distPoint:    given distance, the matching time for which is 
     *			    is returned.
     *=====================================================================
     * MODIFICATION HISTORY:
     *	990426:	Original version.
     *=====================================================================
*/

float timecrossing ( float * distArray , float distPoint , float * time , int nPoints )
{
    /* Declare Local Variables */
    float *xPtr = distArray , *yPtr = time , ttValue ;
    int idx , lvalid = FALSE ;


    /* loop between datapoints. */
    for ( idx = 1 ; idx < nPoints ; idx++ , xPtr++ , yPtr++ )
    {
	/* if we are at the distance, or (more likely) near it, take it */
	if ( *xPtr == distPoint || ( *(xPtr-1) < distPoint && *xPtr > distPoint ) )
	{
	    lvalid = TRUE ;
	    break ;
	}
    }

    if ( lvalid ) {
	/* interpolate the correct time if necessary (and if possible) */
	if ( *(xPtr-1) != *xPtr && *xPtr != distPoint ) {
	    if ( *yPtr == -1 || *(yPtr-1) == -1 )
		ttValue = -1 ;
	    else
		ttValue = ( ( *xPtr - distPoint ) * ( *yPtr - *(yPtr-1) ) /
			    ( *(xPtr-1) - *xPtr ) ) + *yPtr ;
	}
	else
	    ttValue = *yPtr ;

	/* If it's null, use standard null response. */
	if ( ttValue == -1 )
	    ttValue = -12345.0 ;
    }

    else 
	ttValue = -12345.0 ;

    return ttValue ;
}
