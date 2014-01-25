
#include <string.h>

#include "ssi.h"
#include "hdr.h"


#include "msg.h"
#include "ucf.h"

void DBheaderToSac ( header , lall )
struct SACheader *header ;
int lall ;
{

    int idx , err = 0 ;

   /*=====================================================================
    * PURPOSE:  To copy a SAC header into SeisMgr. 
    *=====================================================================
    * OUtPUT ARGUMENTS:
    *    header:  SAC formatted header in SeisMgr.
    *=====================================================================
    * MODIFICATION HISTORY:
    *    980803:  Original version.  maf plagerized from DBheaderToSac.c
    *===================================================================== */

    /* Copy each variable from the SAC header into header. */
    if ( lall ) {
	*begin	= header->b ;
	*ennd	= header->e ;
	*origin	= header->o ;
	*arrivl	= header->a ;
	*t0	= header->t0 ;
	*t1	= header->t1 ;
	*t2	= header->t2 ;
	*t3	= header->t3 ;
	*t4	= header->t4 ;
	*t5	= header->t5 ;
	*t6	= header->t6 ;
	*t7	= header->t7 ;
	*t8	= header->t8 ;
	*t9	= header->t9 ;
	*fini	= header->f ;
	*evla	= header->evla ;
	*evlo	= header->evlo ;
	*evel	= header->evel ;
	*evdp	= header->evdp ;
	*mag	= header->mag ;
	*user0	= header->user0 ;
	*user1	= header->user1 ;
	*user2	= header->user2 ;
	*user3	= header->user3 ;
	*user4	= header->user4 ;
	*user5	= header->user5 ;
	*user6	= header->user6 ;
	*user7	= header->user7 ;
	*user8	= header->user8 ;
	*user9	= header->user9 ;
	*dist	= header->dist ;
	*az	= header->az ;
	*baz	= header->baz ;
	*gcarc	= header->gcarc ;
	*sb	= header->sb ;
	*sdelta	= header->sdelta ;
	*fmt	= header->fmt ;
	*resp0	= header->resp0 ;
	*resp1	= header->resp1 ;
	*resp2	= header->resp2 ;
	*resp3	= header->resp3 ;
	*resp4	= header->resp4 ;
	*resp5	= header->resp5 ;
	*resp6	= header->resp6 ;
	*resp7	= header->resp7 ;
	*resp8	= header->resp8 ;
	*resp9	= header->resp9 ;
	*fhdr64	= header->unused6 ;
	*fhdr65	= header->unused7 ;
	*fhdr66	= header->unused8 ;
	*fhdr67	= header->unused9 ;
	*fhdr68	= header->unused10 ;
	*fhdr69	= header->unused11 ;
	*fhdr70	= header->unused12 ;

	*ievtyp	= header->ievtyp ;
	*imagtyp= header->imagtyp ;
	*imagsrc= header->imagsrc ;
	*ihdr4	= header->unused16 ;
	*ievreg	= header->ievreg ;
	*iqual	= header->iqual ;
	*ihdr13	= header->unused19 ;
	*ihdr14	= header->unused20 ;
	*ihdr15	= header->unused21 ;
	*ihdr16	= header->unused22 ;
	*ihdr17	= header->unused23 ;
	*ihdr18	= header->unused24 ;
	*ihdr19	= header->unused25 ;
	*ihdr20	= header->unused26 ;

	strcpy ( kdatrd	, header->kdatrd );
	strcpy ( kevnm  , "                 " ) ;
	strncpy( kevnm	, header->kevnm , strlen ( header->kevnm ) );
	strcpy ( ko	, header->ko );
	strcpy ( ka	, header->ka );
	strcpy ( kt0	, header->kt0 );
	strcpy ( kt1	, header->kt1 );
	strcpy ( kt2	, header->kt2 );
	strcpy ( kt3	, header->kt3 );
	strcpy ( kt4	, header->kt4 );
	strcpy ( kt5	, header->kt5 );
	strcpy ( kt6	, header->kt6 );
	strcpy ( kt7	, header->kt7 );
	strcpy ( kt8	, header->kt8 );
	strcpy ( kt9	, header->kt9 );
	strcpy ( kf	, header->kf );
	strcpy ( kuser0	, header->kuser0 );
	strcpy ( kuser1	, header->kuser1 );
	strcpy ( kuser2	, header->kuser2 );

	*lovrok	= header->lovrok ;
	*lcalda	= header->lcalda ;
	*lhdr5	= header->unused27 ;

	*nzyear	= header->nzyear ;
	*nzjday	= header->nzjday ;
	*nzhour	= header->nzhour ;
	*nzmin	= header->nzmin ;
	*nzsec	= header->nzsec ;
	*nzmsec	= header->nzmsec ;
	*nsnpts	= header->nsnpts ;
	*nhdr15	= header->unused15 ;
    } /* end if ( lall ) */

    *delta   = header->delta ;
    *depmin  = header->depmin ;
    *depmax  = header->depmax ;
    *scale   = header->scale ;
    *odelta  = header->odelta ;
    *stla    = header->stla ;
    *stlo    = header->stlo ;
    *stel    = header->stel ;
    *stdp    = header->stdp ;
    *depmen  = header->depmen ;
    *cmpaz   = header->cmpaz ;
    *cmpinc  = header->cmpinc ;
    *xminimum= header->xminimum ;
    *xmaximum= header->xmaximum ;
    *yminimum= header->yminimum ;
    *ymaximum= header->ymaximum ;

    *iftype  = header->iftype ;
    *idep    = header->idep ;
    *iinst   = header->iinst ;
    *istreg  = header->istreg ;
    *iztype  = header->iztype ;
    *isynth  = header->isynth ;

    strcpy ( kinst , header->kinst );
    strcpy ( kstnm , header->kstnm );
    strcpy ( kcmpnm , header->kcmpnm );
    strcpy ( khole , header->khole );
    strcpy ( knetwk , header->knetwk );

    *leven   = header->leven ;
    *lpspol  = header->lpspol ;

    *npts    = header->npts ;
    *nxsize  = header->nxsize ;
    *nysize  = header->nysize ;
    *nvhdr   = header->nvhdr ;
    *norid   = header->norid ;
    *nevid   = header->nevid ;
    *nwfid   = header->nwfid ;

    /* calculate dist, az, baz, and gcarc if appropriate */
    if ( *stla != cmhdr.fundef && *stlo != cmhdr.fundef &&
	 *evla != cmhdr.fundef && *evlo != cmhdr.fundef && *lcalda ) {

	*dist = 0.;
	*az = 0.;
	*baz = 0.;
	*gcarc = 0.;

	distaz( *evla, *evlo, (float*)stla, (float*)stlo, 1, (float*)dist,
		(float*)az, (float*)baz, (float*)gcarc, &err );

	/* handle error */
	if ( err ) {
	    *dist = cmhdr.fundef;
	    *az = cmhdr.fundef;
	    *baz = cmhdr.fundef;
	    *gcarc = cmhdr.fundef;
	    err = 0 ;
	}

	/* deal with the case:  station == event */
	if( (*evla == *stla) && (*evlo == *stlo) ){
	    *az = 0.;
	    *baz = 0.;
	    *dist = 0.;
	    *gcarc = 0.;
	}
    } /* end if ( *stla ... ) */

    /* Check the ranges of the enumerated header variables */
    for ( idx = 0 ; idx < SAC_HEADER_ENUMS ; idx++ ) {
	if ( cmhdr.ihdr[idx] > SAC_ENUMS ||
	   ( cmhdr.ihdr[idx] < 0 && cmhdr.ihdr[idx] != SAC_INT_UNDEFINED ) ) {
	    cmhdr.ihdr[idx] = SAC_INT_UNDEFINED ;
	    err = 1365 ;
	}
    }

    if ( err ) {
	setmsg ( "WARNING" , err ) ;
	apcmsg ( "reset to undefined" , 19 ) ;
	outmsg () ;
	clrmsg () ;
    }

} /* end DBheaderToSac () */
