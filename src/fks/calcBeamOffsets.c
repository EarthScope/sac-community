/** 
 * @file   calcBeamOffsets.c
 * 
 * @brief  Calculate Beam Offsets
 * 
 */
#include <string.h>
#include <math.h>

#include "fks.h"
#include "bool.h"
#include "hdr.h"


#include "dff.h"

#define PI M_PI

#define	MXLENB	40000

void cascade() ;
int  isInfoThere() ;
void refOffsets () ;
void userOffsets () ;
void eventOffsets () ;

void 
calcBeamOffsets(int    ns, 
                int    elevc, 
                float *xr, 
                float *yr, 
                float *zr, 
                int   *nerr) {
        int ndx1, ndx2, idummy;

	/* ====================================================================
	 * PURPOSE: to compute x, y and (future) z offsets from reference
         *          station, for use in beam calculation.  The first station
         *          in the list is assumed to be the reference station, unless
	 *          the reference option is set.
	 *=====================================================================
	 * INPUT ARGUMENTS:
         *      ns:    Number of stations.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
         *      xr:    x offset.
         *      yr:    y offset.
         *      zr:    z offset.
	 *    nerr:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers:
	 *=====================================================================
	 * MODULE/LEVEL:   fks/2
	 *=====================================================================
	 * GLOBAL INPUT:  (to be updated)
	 *    dfm:     ndfl
	 *    hdr:     delta, begin
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    dfm:     ndfl
	 *=====================================================================
	 * SUBROUTINES CALLED: (to be updated)
	 *    saclib: 
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    970306:  Entirely rewritten.  calcBeamOffsets() no longer calls
	 *             calcoffsets().  It allows the user to specify the method
	 *             of determining offsets.  If the user doesn't specify,
	 *             there is a well defined set of rules to determine the 
	 *             method.  maf
         *    970207:  Original version (copied from calcoffsets).  maf
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  
	 *=====================================================================  */

	*nerr = 0 ;

	switch ( cmfks.flagOffset ) 
	{
	    case OCASCADE :
		cascade ( ns, elevc , xr , yr , zr , nerr ) ;

		break ;

	    case OREFERENCE :
		if ( cmfks.lReference )
		    refOffsets ( ns , cmfks.rReference , xr , yr , zr , nerr ) ;
		else
		    *nerr = 5303 ;

		break ;

	    case OUSER :
		if ( isInfoThere ( ns, elevc , nerr ) & IUSR ) 
		    userOffsets ( ns , xr , yr , zr , nerr ) ;
		else
		    *nerr = 5304 ;

		break ;

	    case OSTATION :
		if ( isInfoThere ( ns, elevc , nerr ) & ISTATION ) {
		    float reference[3] ;
/*		    int ndx1 , ndx2 , idummy ;		 */

		    getfil( 1, FALSE, &ndx1, &ndx2, &idummy, nerr);
		    if ( *nerr != 0 )
			return ;

		    reference[0] = *stla ;
		    reference[1] = *stlo ;
		    reference[2] = *stel ;

		    refOffsets ( ns , reference , xr , yr , zr , nerr ) ;
		}
		else
		    *nerr = 5305 ;

		break ;

	    case OEVENT :
		if ( isInfoThere ( ns, elevc , nerr ) & IEVENT ) 
		    eventOffsets ( ns , xr , yr , zr , nerr ) ;
		else
		    *nerr = 5306 ;

		break ;

	    default :
		*nerr = 5307 ;

		break ;

	} /* end switch */

} /* end calcBeamOffsets */




void 
cascade (int    ns, 
         int    elevc , 
         float *xr, 
         float *yr, 
         float *zr, 
         int   *nerr ) {

	int availableInfo ;

         /* If Reference option is set, use reference lat and lon to calculate 
	    x and y offsets */
	if ( cmfks.lReference ) {
	    refOffsets ( ns , cmfks.rReference , xr , yr , zr , nerr ) ;    

	    return ;
	} /* end if ( cmfks.lReference ) */

	availableInfo = isInfoThere ( ns, elevc , nerr ) ;

	/* - Below are three methods of determining offsets, use the one
	     set in the OFFSETS option, or if none were set, use the first
	     method for which all the information is available. */

	/* Get offsets from USER7, -8, and -9 */
	if ( availableInfo & IUSR ) {
	    userOffsets ( ns , xr , yr , zr , nerr ) ;

	    return ;
	} 

	/* Calculate offsets with respect to the first station location. */
	if ( availableInfo & ISTATION ) {
	    float reference[3];
	    int ndx1 , ndx2 , idummy ;

	    getfil( 1, FALSE, &ndx1, &ndx2, &idummy, nerr);
	    if ( *nerr != 0 ) 
		return ;

	    reference[0] = *stla ;
	    reference[1] = *stlo ;
	    reference[2] = *stel ;

	    refOffsets ( ns , reference , xr , yr , zr , nerr ) ;

	    return ;
	}

	/* Calculate offsets with respect to the first event location */
	if ( availableInfo & IEVENT ) {
	    eventOffsets ( ns , xr , yr , zr , nerr ) ;

	    return ;
	}


	/* If we get to here, we didn't have the information we needed to 
	   make the beam.  Pass back an error */
	*nerr = 5302 ;
} /* end cascade */



int 
isInfoThere ( int  nFiles, 
              int  elevc , 
              int *nerr ) {
    int jdfl , ndx1 , ndx2 , idummy ,
	returnValue = 0 ,
	luser = TRUE ,
	lstation = TRUE ,
	levent = TRUE ;

    for(jdfl = 1; jdfl <= nFiles; jdfl++){
	getfil(jdfl, FALSE, &ndx1, &ndx2, &idummy, nerr);
	if(*nerr != 0)
	    return 0 ;

	if ( ( *user7 == cmhdr.fundef ) ||
	     ( *user8 == cmhdr.fundef ) ||
	     ( elevc && *user9 == cmhdr.fundef ) )
	    luser = FALSE;
	if ( ( *stla == cmhdr.fundef ) ||
	     ( *stlo == cmhdr.fundef ) ||
	     ( elevc && *stel == cmhdr.fundef ) )
	    lstation = FALSE;
	if ( ( *evla == cmhdr.fundef ) ||
	     ( *evlo == cmhdr.fundef ) ||
	     ( elevc && *evel == cmhdr.fundef ) )
	    levent = FALSE;
    }

    if ( luser )	returnValue += IUSR ;
    if ( lstation )	returnValue += ISTATION ;
    if ( levent )	returnValue += IEVENT ;

    return returnValue ;
} /* end isInfoThere */


void 
refOffsets ( int     nFiles , 
             float  *referencePosition, 
             float  *xr, 
             float  *yr, 
             float  *zr, 
             int    *nerr ) {
    int jdfl , ndx1 , ndx2 , idummy ;
    float dlat , dlon , avlat ;

    for ( jdfl = 1 ; jdfl <= nFiles ; jdfl++ ) {
	getfil(jdfl, FALSE, &ndx1, &ndx2, &idummy, nerr);
	if(*nerr != 0) return;

	dlat = *stla - referencePosition[0];
	dlon = *stlo - referencePosition[1];
	avlat = (referencePosition[0] + *stla) / 2.0;
	xr[jdfl-1] = 111.19 * dlon * cos( PI * avlat / 180.0);
	yr[jdfl-1] = 111.19 * dlat;
	zr[jdfl-1] = referencePosition[2] - *stel;
			/* seismological note:  elevation is subtracted
			   in the opposite order of latitude and
			   longitude; this signifies that in the Z
			   direction, down is positive.  */
    } /* end for */


} /* end refOffsets */



void 
userOffsets ( int     nFiles, 
              float  *xr, 
              float  *yr, 
              float  *zr, 
              int    *nerr )  {

    int jdfl , ndx1 , ndx2 , idummy ;

    for(jdfl=1; jdfl <= nFiles; jdfl++){
	getfil(jdfl, FALSE, &ndx1, &ndx2, &idummy, nerr);
	if(*nerr != 0) return ;

	xr[jdfl-1] = *user7;
	yr[jdfl-1] = *user8;
	zr[jdfl-1] = *user9; /* not used at present */
    }
}



void 
eventOffsets ( int     nFiles, 
               float  *xr, 
               float  *yr, 
               float  *zr, 
               int    *nerr ) {
    int jdfl , ndx1 , ndx2 , idummy ;
    float dlat , dlon , avlat , reflat , reflon , refel ;

    for(jdfl=1; jdfl <= nFiles ; jdfl++){
	getfil(jdfl, FALSE, &ndx1, &ndx2, &idummy, nerr);
	if(*nerr != 0) return ;

	if(jdfl == 1 ){
	    xr[jdfl-1] = 0.0;
	    yr[jdfl-1] = 0.0;
	    zr[jdfl-1] = 0.0;
	    reflat = *evla;
	    reflon = *evlo;
	    refel  = *evel;
	}else{
	    dlat = *evla - reflat;
	    dlon = *evlo - reflon;
	    avlat = (reflat + *evla) / 2.0;
	    xr[jdfl-1] = 111.19 * dlon * cos( PI * avlat / 180.0);
	    yr[jdfl-1] = 111.19 * dlat;
	    zr[jdfl-1] = refel - *evel;
			/* seismological note:  elevation is subtracted
			   in the opposite order of latitude and
			   longitude; this signifies that in the Z
			   direction, down is positive.  */
	}
    }
} /* end eventOffsets */

