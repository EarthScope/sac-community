
#include <stdio.h>

#include "ssi.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"
#include "sm_gc.h"

#include "sacIO/sacIO.h"
#include "smDataIO.h"
#include "cssListOps/dblErrors.h"
#include "cssListOps/dblPublicDefs.h"
#include "cssListOps/cssListOps.h"

void SeisMgrToSac ( DBlist tree , int lname , int * nerr,
                    int Verbose , int lcutnow , int takeEvid )
{
    /* Declare Variables. */
    int refTimeType = cmdfm.lshift ? IO : IB ;
    double refTime ;  /* subract this from the picks. */
    int dots = 0;
    int originalNDFL ;

    MagType mType = cmdfm.nMagSpec ;
    struct wfdiscList *wfL = NULL ;

    *nerr = 0 ;
 
    /* Initialize SeisMgr error handler */
    dblClearErrorList () ;

    /* Delete previous Sac data. */
    originalNDFL = deleteAllSacFiles ( nerr , lname ) ;

    if ( *nerr ) {
	if ( originalNDFL < cmdfm.ndfl )
	    *nerr = 1401 ;
	else
	    *nerr = 1402 ;
	return ;
    }

    /* Loop through waveforms in the tree, write them into Sac one at a time. */
    do {
	int lcuttemp = lcutnow ;	/* to further discern when to cut */

        /* Get next waveform. */
        if ( ! ( wfL = dblNextTableInstance ( wfL , tree , dbl_LIST_WFDISC ) ) )
            break ;

      /* Progress indicator in verbose mode. */
       if(Verbose){
          putchar('.');
          dots++;
          if(dots == 80){
             putchar('\n');
             dots = 0;
          }
          fflush(stdout);
       }


        /* Get the header to go with the waveform */
        sacHeaderFromCSS( tree, &( globalSacHeader[ cmdfm.ndfl ] ),
			  wfL, refTimeType, &refTime, mType) ;

        /* Get picks according to the preferences file and
	   pickauth and pickphase commands. */
        cmdfm.ndfl++ ;

	if( cmdfm.lpref ) {
            prefPicksToHeader( &( globalSacHeader[ cmdfm.ndfl - 1 ] ),
			       cmdfm.ndfl, wfL->element, tree, refTime, nerr ) ;

            if ( *nerr ) {
                *nerr = 1401 ;
	        break ;
	    }
	} /* end if( cmdfm.lpref ) */

	if ( !lname && cmdfm.ndfl > originalNDFL )
	    lname = TRUE ;

	/*if ( cmdfm.ndfl <= originalNDFL )*/
	if ( !lname )
	    lcuttemp = FALSE ;

        /* Create a SAC file, fill the header and waveform. */
        CSStoSAC( cmdfm.ndfl, &( globalSacHeader[ cmdfm.ndfl-1 ] ),
		  wfL->seis, lname , lcuttemp , nerr ) ;
	if ( *nerr ) {
	    *nerr = 1402 ;
	    break ;
	}

	/* If the waveform was cut on the way into SAC,
	   send the cut version back to SeisMgr. */
	if ( cmdfm.lcut && lcuttemp ) {
	    sacSACdata newData ;

	    newData.dataType = globalSacHeader[ cmdfm.ndfl-1 ].iftype ;
	    newData.xarray   = cmmem.sacmem[ cmdfm.ndxdta[ cmdfm.ndfl-1 ][ 1 ] ] ;
	    newData.yarray   = cmmem.sacmem[ cmdfm.ndxdta[ cmdfm.ndfl-1 ][ 0 ] ] ;
	    globalSacHeader[ cmdfm.ndfl-1 ].b = *begin ;
	    globalSacHeader[ cmdfm.ndfl-1 ].e = *ennd ;
	    globalSacHeader[ cmdfm.ndfl-1 ].npts = *npts ;


	    sacLoadFromHeaderAndData ( &( globalSacHeader[ cmdfm.ndfl-1 ] ) ,
				       &newData , smGetDefaultWorksetName() ,
				       FALSE , cmdfm.ndfl-1 , TRUE, takeEvid ) ;
	}
    } while ( wfL ) ;

    if(Verbose)printf("\n");

    gcCollect ( tree ) ;

} /* end SeisMgrToSac */
