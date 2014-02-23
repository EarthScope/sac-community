
#include <stdio.h>

#include "amf.h"
#include "ssi.h"
#include "dfm.h"
#include "bool.h"

#include "smDataIO.h"
#include "cssListOps/cssListOps.h"
#include "cssListOps/dblErrors.h"
#include "SacHeader.h"
#include "msg.h"
#include "dff.h"
#include "array.h"

void rollback (int whichHeaders , int * nerr)
{

    DBlist tree ;
    struct wfdiscList *wfL = NULL ;
    int check ;
    sac *s;
    int i;
    *nerr = 0 ;

    struct SACheader **header;
    
    /* Initialize SeisMgr error handler */
    dblClearErrorList () ;

    /* get tree for default wordset */
    tree = smGetDefaultTree () ;

    header = xarray_new_with_len('p', saclen());
    
    if ( whichHeaders != allHeader ) {
        int jdfl ;

        for ( jdfl = 0 ; jdfl < saclen() ; jdfl ++ ) {
          if(!(s = sacget(jdfl, TRUE, nerr))) {
            goto L_8888;
          }
          //getfil ( jdfl + 1 , FALSE , &notused1 , &notused2 , &notused3 , nerr ) ;
          header[jdfl] = (struct SACheader *) malloc(sizeof(struct SACheader));
          SacHeaderToDB ( header[ jdfl ] , -whichHeaders , 0 ) ;
	}
    }

    check = deleteAllSacFiles ( nerr , FALSE ) ;
    if ( *nerr ) {
	if ( check < saclen() )
	    *nerr = 1401 ;
	else
	    *nerr = 1402 ;
  goto L_9999;
    }
    do {
        /* Get next waveform. */
        if ( ! ( wfL = dblNextTableInstance ( wfL , tree , dbl_LIST_WFDISC ) ) )
            break ;

	cmdfm.ndfl ++;
        CSStoSAC ( saclen(), header[ saclen() -1 ],
                   wfL->seis , FALSE , FALSE , nerr ) ;
    L_8888:
        if ( *nerr ) {
	    setmsg ( "ERROR" , *nerr ) ;
	    outmsg () ;
	    clrmsg () ;
	    *nerr = 1401 ;
	    break ;
	}
    } while ( wfL ) ;

 L_9999:
    for(i = 0; i < (int)xarray_length(header); i++) {
      free(header[i]);
      header = NULL;
    }
    xarray_free(header);
} /* end rollback */
