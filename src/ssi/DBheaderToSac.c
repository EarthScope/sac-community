
#include <string.h>

#include "ssi.h"
#include "hdr.h"


#include "amf.h"
#include "msg.h"
#include "ucf.h"
#include "dff.h"

void DBheaderToSac ( header , lall, s )
struct SACheader *header ;
int lall ;
sac *s;
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
	s->h->b	= header->b ;
	s->h->e	= header->e ;
	s->h->o	= header->o ;
	s->h->a	= header->a ;
	s->h->t0	= header->t0 ;
	s->h->t1	= header->t1 ;
	s->h->t2	= header->t2 ;
	s->h->t3	= header->t3 ;
	s->h->t4	= header->t4 ;
	s->h->t5	= header->t5 ;
	s->h->t6	= header->t6 ;
	s->h->t7	= header->t7 ;
	s->h->t8	= header->t8 ;
	s->h->t9	= header->t9 ;
	s->h->f	= header->f ;
	s->h->evla	= header->evla ;
	s->h->evlo	= header->evlo ;
	s->h->evel	= header->evel ;
	s->h->evdp	= header->evdp ;
	s->h->mag	= header->mag ;
	s->h->user0	= header->user0 ;
	s->h->user1	= header->user1 ;
	s->h->user2	= header->user2 ;
	s->h->user3	= header->user3 ;
	s->h->user4	= header->user4 ;
	s->h->user5	= header->user5 ;
	s->h->user6	= header->user6 ;
	s->h->user7	= header->user7 ;
	s->h->user8	= header->user8 ;
	s->h->user9	= header->user9 ;
	s->h->dist	= header->dist ;
	s->h->az	= header->az ;
	s->h->baz	= header->baz ;
	s->h->gcarc	= header->gcarc ;
	s->h->sb	= header->sb ;
	s->h->sdelta	= header->sdelta ;
	s->h->fmt	= header->fmt ;
	s->h->resp0	= header->resp0 ;
	s->h->resp1	= header->resp1 ;
	s->h->resp2	= header->resp2 ;
	s->h->resp3	= header->resp3 ;
	s->h->resp4	= header->resp4 ;
	s->h->resp5	= header->resp5 ;
	s->h->resp6	= header->resp6 ;
	s->h->resp7	= header->resp7 ;
	s->h->resp8	= header->resp8 ;
	s->h->resp9	= header->resp9 ;
	s->h->unused6	= header->unused6 ;
	s->h->unused7	= header->unused7 ;
	s->h->unused8	= header->unused8 ;
	s->h->unused9	= header->unused9 ;
	s->h->unused10	= header->unused10 ;
	s->h->unused11	= header->unused11 ;
	s->h->unused12	= header->unused12 ;

	s->h->ievtyp	= header->ievtyp ;
	s->h->imagtyp= header->imagtyp ;
	s->h->imagsrc= header->imagsrc ;
	s->h->unused16	= header->unused16 ;
	s->h->ievreg	= header->ievreg ;
	s->h->iqual	= header->iqual ;
	s->h->unused19	= header->unused19 ;
	s->h->unused20	= header->unused20 ;
	s->h->unused21	= header->unused21 ;
	s->h->unused22	= header->unused22 ;
	s->h->unused23	= header->unused23 ;
	s->h->unused24	= header->unused24 ;
	s->h->unused25	= header->unused25 ;
	s->h->unused26	= header->unused26 ;

	strcpy ( s->h->kdatrd	, header->kdatrd );
	strcpy ( s->h->kevnm  , "                 " ) ;
	strncpy( s->h->kevnm	, header->kevnm , strlen ( header->kevnm ) );
	strcpy ( s->h->ko	, header->ko );
	strcpy ( s->h->ka	, header->ka );
	strcpy ( s->h->kt0	, header->kt0 );
	strcpy ( s->h->kt1	, header->kt1 );
	strcpy ( s->h->kt2	, header->kt2 );
	strcpy ( s->h->kt3	, header->kt3 );
	strcpy ( s->h->kt4	, header->kt4 );
	strcpy ( s->h->kt5	, header->kt5 );
	strcpy ( s->h->kt6	, header->kt6 );
	strcpy ( s->h->kt7	, header->kt7 );
	strcpy ( s->h->kt8	, header->kt8 );
	strcpy ( s->h->kt9	, header->kt9 );
	strcpy ( s->h->kf	, header->kf );
	strcpy ( s->h->kuser0	, header->kuser0 );
	strcpy ( s->h->kuser1	, header->kuser1 );
	strcpy ( s->h->kuser2	, header->kuser2 );

	s->h->lovrok	= header->lovrok ;
	s->h->lcalda	= header->lcalda ;
	s->h->unused27	= header->unused27 ;

	s->h->nzyear	= header->nzyear ;
	s->h->nzjday	= header->nzjday ;
	s->h->nzhour	= header->nzhour ;
	s->h->nzmin	= header->nzmin ;
	s->h->nzsec	= header->nzsec ;
	s->h->nzmsec	= header->nzmsec ;
	s->h->nsnpts	= header->nsnpts ;
	s->h->unused15	= header->unused15 ;
    } /* end if ( lall ) */

    s->h->delta   = header->delta ;
    s->h->depmin  = header->depmin ;
    s->h->depmax  = header->depmax ;
    s->h->scale   = header->scale ;
    s->h->odelta  = header->odelta ;
    s->h->stla    = header->stla ;
    s->h->stlo    = header->stlo ;
    s->h->stel    = header->stel ;
    s->h->stdp    = header->stdp ;
    s->h->depmen  = header->depmen ;
    s->h->cmpaz   = header->cmpaz ;
    s->h->cmpinc  = header->cmpinc ;
    s->h->xminimum= header->xminimum ;
    s->h->xmaximum= header->xmaximum ;
    s->h->yminimum= header->yminimum ;
    s->h->ymaximum= header->ymaximum ;

    s->h->iftype  = header->iftype ;
    s->h->idep    = header->idep ;
    s->h->iinst   = header->iinst ;
    s->h->istreg  = header->istreg ;
    s->h->iztype  = header->iztype ;
    s->h->isynth  = header->isynth ;

    strcpy ( s->h->kinst , header->kinst );
    strcpy ( s->h->kstnm , header->kstnm );
    strcpy ( s->h->kcmpnm , header->kcmpnm );
    strcpy ( s->h->khole , header->khole );
    strcpy ( s->h->knetwk , header->knetwk );

    s->h->leven   = header->leven ;
    s->h->lpspol  = header->lpspol ;

    s->h->npts    = header->npts ;
    s->h->nxsize  = header->nxsize ;
    s->h->nysize  = header->nysize ;
    s->h->nvhdr   = header->nvhdr ;
    s->h->norid   = header->norid ;
    s->h->nevid   = header->nevid ;
    s->h->nwfid   = header->nwfid ;

    /* calculate dist, az, baz, and gcarc if appropriate */
    update_distaz(s);

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
