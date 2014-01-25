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
        int jdfl, ndx1, ndx2, idummy, idfl, count;
        int lrefset, lstaset, luserset, levset;
        char refsta[9];

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
          getfil(jdfl, FALSE, &ndx1, &ndx2, &idummy, nerr);
          if(*nerr != 0) goto L_9999;

          if(jdfl == 1){
            if( strncmp(kuser1, "-12345", 6) == 0){
              lrefset = FALSE;
	    }
            strcpy(refsta,kuser1);
	  }
          if(strcmp(kuser1,refsta) != 0)                           lrefset = FALSE;
          if((*stla == cmhdr.fundef) || (*stlo == cmhdr.fundef))   lstaset = FALSE;
          if((*user7 == cmhdr.fundef) || (*user8 == cmhdr.fundef)) luserset = FALSE;
          if((*evla == cmhdr.fundef) || (*evlo == cmhdr.fundef))   levset = FALSE;

	}


        if(lrefset && luserset){ 
           /* use user7 and user8 for x and y offsets */
          for(jdfl=1; jdfl <= ns; jdfl++){
            getfil(jdfl, FALSE, &ndx1, &ndx2, &idummy, nerr);
            if(*nerr != 0) goto L_9999;

            xr[jdfl-1] = *user7;
            yr[jdfl-1] = *user8;
            zr[jdfl-1] = *user9; /* not used at present */
	  }
	}else if( lstaset ){
           /* use station lat and lon to calculate x and y offsets */
          for(jdfl=1; jdfl <= ns; jdfl++){
            getfil(jdfl, FALSE, &ndx1, &ndx2, &idummy, nerr);
            if(*nerr != 0) goto L_9999;
 
            if(jdfl == 1 ){
              xr[jdfl-1] = 0.0;
              yr[jdfl-1] = 0.0;
              zr[jdfl-1] = 0.0;
              reflat = *stla;
              reflon = *stlo; 
              refel  = *stel;
	    }else{
              dlat = *stla - reflat;
              dlon = *stlo - reflon;
              avlat = (reflat + *stla) / 2.0;
              xr[jdfl-1] = 111.19 * dlon * cos( M_PI * avlat / 180.0);
              yr[jdfl-1] = 111.19 * dlat;
              zr[jdfl-1] = refel - *stel;
	    }
	  }
	}else if( luserset ){
           /* use user7 and user8 for x and y offsets */
          for(jdfl=1; jdfl <= ns; jdfl++){
            getfil(jdfl, FALSE, &ndx1, &ndx2, &idummy, nerr);
            if(*nerr != 0) goto L_9999;

            xr[jdfl-1] = *user7;
            yr[jdfl-1] = *user8;
            zr[jdfl-1] = *user9; /* not used at present */
	  }
	}else if( levset ){
           /* use event lat and lon to calculate x and y offsets */
          for(jdfl=1; jdfl <= ns; jdfl++){
            getfil(jdfl, FALSE, &ndx1, &ndx2, &idummy, nerr);
            if(*nerr != 0) goto L_9999;
 
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

