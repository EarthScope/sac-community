
#include <stdio.h>

#include "ssi.h"
#include "dfm.h"
#include "bool.h"

#include "smDataIO.h"
#include "cssListOps/cssListOps.h"
#include "cssListOps/dblErrors.h"

#include "msg.h"
#include "dff.h"

void rollback (int whichHeaders , int * nerr)
{

    DBlist tree ;
    struct wfdiscList *wfL = NULL ;
    int check ;
    int notused1 , notused2 , notused3 ;

    *nerr = 0 ;

    /* Initialize SeisMgr error handler */
    dblClearErrorList () ;

    /* get tree for default wordset */
    tree = smGetDefaultTree () ;

    if ( whichHeaders != allHeader ) {
        int jdfl ;

        for ( jdfl = 0 ; jdfl < cmdfm.ndfl ; jdfl ++ ) {
	    getfil ( jdfl + 1 , FALSE , &notused1 , &notused2 , &notused3 , nerr ) ;
            SacHeaderToDB ( &( globalSacHeader[ jdfl ] ) , -whichHeaders , 0 ) ;
	}
    }

    check = deleteAllSacFiles ( nerr , FALSE ) ;
    if ( *nerr ) {
	if ( check < cmdfm.ndfl )
	    *nerr = 1401 ;
	else
	    *nerr = 1402 ;
	return ;
    }
    do {
        /* Get next waveform. */
        if ( ! ( wfL = dblNextTableInstance ( wfL , tree , dbl_LIST_WFDISC ) ) )
            break ;

	cmdfm.ndfl ++;
        CSStoSAC ( cmdfm.ndfl, &( globalSacHeader[ cmdfm.ndfl -1 ] ) ,
                   wfL->seis , FALSE , FALSE , nerr ) ;
	if ( *nerr ) {
	    setmsg ( "ERROR" , *nerr ) ;
	    outmsg () ;
	    clrmsg () ;
	    *nerr = 1401 ;
	    break ;
	}
    } while ( wfL ) ;
} /* end rollback */
