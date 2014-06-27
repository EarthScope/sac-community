/** 
 * @file   calcoffsets.c
 * 
 * @brief  Calculate Offsets for BBFK 
 * 
 */

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "fks.h"
#include "bool.h"
#include "hdr.h"
#include "amf.h"


#include "dff.h"

#define PI M_PI
#define	MXLENB	40000

void 
calcoffsets(int    ns, 
            float  *xr, 
            float  *yr, 
            float  *zr, 
            int    *nerr) {

        float reflat, reflon, refel, dlat, dlon, avlat;
        int jdfl, idfl, count;
        int lrefset, lstaset, luserset, levset;
        char refsta[9];
        sac *s;
	/* ============================================================
	 * PURPOSE: to compute x, y and (future) z offsets from reference
         *          station, for use in bbfk calculation.  The first station
         *          in the list is assumed to be the reference station.
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
         *    092295:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  
	 *=====================================================================  */

        /* check to see if reference station name is stored in kuser1 and is the same */
        /* for all stations. */

        lrefset  = TRUE; /* reference station field (kuser1) set for all files */
        lstaset  = TRUE; /* station lat and lon set for all files */
        luserset = TRUE; /* user7 and user8 set for all files */
        levset   = TRUE; /* event lat and lon set for all files */
        
        for(jdfl = 1; jdfl <= ns; jdfl++){
          if(!(s = sacget(jdfl-1, TRUE, nerr))) {
            *nerr = ERROR_ILLEGAL_DATA_FILE_LIST_NUMBER;
            goto L_9999;
          }
          //getfil(jdfl, FALSE, &ndx1, &ndx2, &idummy, nerr);

          if(jdfl == 1){
            if( strncmp(s->h->kuser1, "-12345", 6) == 0){
              lrefset = FALSE;
	    }
            strcpy(refsta,s->h->kuser1);
	  }
          if(strcmp(s->h->kuser1,refsta) != 0)                           lrefset = FALSE;
          if((s->h->stla == SAC_FLOAT_UNDEFINED) || (s->h->stlo == SAC_FLOAT_UNDEFINED))   lstaset = FALSE;
          if((s->h->user7 == SAC_FLOAT_UNDEFINED) || (s->h->user8 == SAC_FLOAT_UNDEFINED)) luserset = FALSE;
          if((s->h->evla == SAC_FLOAT_UNDEFINED) || (s->h->evlo == SAC_FLOAT_UNDEFINED))   levset = FALSE;

	}


        if(lrefset && luserset){ 
           /* use user7 and user8 for x and y offsets */
          for(jdfl=1; jdfl <= ns; jdfl++){
            if(!(s = sacget(jdfl-1, TRUE, nerr))) {
              *nerr = ERROR_ILLEGAL_DATA_FILE_LIST_NUMBER;
              goto L_9999;
            }
            //getfil(jdfl, FALSE, &ndx1, &ndx2, &idummy, nerr);

            xr[jdfl-1] = s->h->user7;
            yr[jdfl-1] = s->h->user8;
            zr[jdfl-1] = s->h->user9; /* not used at present */
	  }
	}else if( lstaset ){
           /* use station lat and lon to calculate x and y offsets */
          for(jdfl=1; jdfl <= ns; jdfl++){
            if(!(s = sacget(jdfl-1, TRUE, nerr))) {
              *nerr = ERROR_ILLEGAL_DATA_FILE_LIST_NUMBER;
              goto L_9999;
            }
            //getfil(jdfl, FALSE, &ndx1, &ndx2, &idummy, nerr);
 
            if(jdfl == 1 ){
              xr[jdfl-1] = 0.0;
              yr[jdfl-1] = 0.0;
              zr[jdfl-1] = 0.0;
              reflat = s->h->stla;
              reflon = s->h->stlo; 
              refel  = s->h->stel;
	    }else{
              dlat = s->h->stla - reflat;
              dlon = s->h->stlo - reflon;
              avlat = (reflat + s->h->stla) / 2.0;
              xr[jdfl-1] = 111.19 * dlon * cos( M_PI * avlat / 180.0);
              yr[jdfl-1] = 111.19 * dlat;
              zr[jdfl-1] = refel - s->h->stel;
	    }
	  }
	}else if( luserset ){
           /* use user7 and user8 for x and y offsets */
          for(jdfl=1; jdfl <= ns; jdfl++){
            if(!(s = sacget(jdfl-1, TRUE, nerr))) {
              *nerr = ERROR_ILLEGAL_DATA_FILE_LIST_NUMBER;
              goto L_9999;
            }
            //getfil(jdfl, FALSE, &ndx1, &ndx2, &idummy, nerr);

            xr[jdfl-1] = s->h->user7;
            yr[jdfl-1] = s->h->user8;
            zr[jdfl-1] = s->h->user9; /* not used at present */
	  }
	}else if( levset ){
           /* use event lat and lon to calculate x and y offsets */
          for(jdfl=1; jdfl <= ns; jdfl++){
            if(!(s = sacget(jdfl-1, FALSE, nerr))) {
              goto L_9999;
            }
            //getfil(jdfl, FALSE, &ndx1, &ndx2, &idummy, nerr);

            if(jdfl == 1 ){
              xr[jdfl-1] = 0.0;
              yr[jdfl-1] = 0.0;
              zr[jdfl-1] = 0.0;
              reflat = s->h->evla;
              reflon = s->h->evlo; 
              refel  = s->h->evel;
	    }else{
              dlat = s->h->evla - reflat;
              dlon = s->h->evlo - reflon;
              avlat = (reflat + s->h->evla) / 2.0;
              xr[jdfl-1] = 111.19 * dlon * cos( PI * avlat / 180.0);
              yr[jdfl-1] = 111.19 * dlat;
              zr[jdfl-1] = refel - s->h->evel;
                           /* seismological note:  elevation is subtracted 
                              in the opposite order of latitude and
                              longitude; this signifies that in the Z
                              direction, down is positive.  */
	    }
	  }

	}else{
           /* return error and don't do fk */
          printf("Error:  Not enough header information to calculate bbfk station offsets\n");
          printf("        See BBFK help page for details\n");
          *nerr = 1;
          return;
	}

        count = 0;
        for(jdfl=1; jdfl <= ns; jdfl++){
          for(idfl=jdfl+1; idfl <= ns; idfl++){
            if((xr[jdfl-1] == xr[idfl-1]) && (yr[jdfl-1] == yr[idfl-1]))count++;
	  }
	}

        if(count > 0)printf("Warning:  Co-located stations or events are being used in FK calculation\n");

	/*  Bye                                                                          
	 * */
L_9999:

	return;

} /* end of function */

