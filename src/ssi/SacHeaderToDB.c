
#include <string.h>

#include "ssi.h"
#include "hdr.h"
#include "dfm.h"

#include "amf.h"
#include "msg.h"
#include "dff.h"

void SacHeaderToDB ( header , whichHeaders , idfl )
struct SACheader *header ;
int whichHeaders ;
int idfl ;
{

    int idx , err = 0 ;
    sac *s;
   /*=====================================================================
    * PURPOSE:  To copy a SAC header into SeisMgr. 
    *=====================================================================
    * OUtPUT ARGUMENTS:
    *    header:  SAC formatted header in SeisMgr.
    *=====================================================================
    * MODIFICATION HISTORY:
    *    980803:  Original version.  maf plagerized from DBheaderToSac.c
    *===================================================================== */
    UNUSED(idfl);
    
    s = sacget_current();
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
        //	if ( idfl > 0 )
	if ( err && err != 1365 ) {
	    setmsg ( "ERROR" , err ) ;
	    outmsg () ;
	}
    }

    /* Copy each variable from the SAC header into header. */
    if ( whichHeaders == eventHeader || whichHeaders == allHeader ) {
	header->b = s->h->b ;
	header->e = s->h->e;
	header->o = s->h->o ;
	header->a = s->h->a ;
	header->t0 = s->h->t0 ;
	header->t1 = s->h->t1 ;
	header->t2 = s->h->t2 ;
	header->t3 = s->h->t3 ;
	header->t4 = s->h->t4 ;
	header->t5 = s->h->t5 ;
	header->t6 = s->h->t6 ;
	header->t7 = s->h->t7 ;
	header->t8 = s->h->t8 ;
	header->t9 = s->h->t9 ;
	header->f = s->h->f ;
	header->evla = s->h->evla ;
	header->evlo = s->h->evlo ;
	header->evel = s->h->evel ;
	header->evdp = s->h->evdp ;
	header->mag = s->h->mag ;
	header->user0 = s->h->user0 ;
	header->user1 = s->h->user1 ;
	header->user2 = s->h->user2 ;
	header->user3 = s->h->user3 ;
	header->user4 = s->h->user4 ;
	header->user5 = s->h->user5 ;
	header->user6 = s->h->user6 ;
	header->user7 = s->h->user7 ;
	header->user8 = s->h->user8 ;
	header->user9 = s->h->user9 ;
	header->dist = s->h->dist ;
	header->az = s->h->az ;
	header->baz = s->h->baz ;
	header->gcarc = s->h->gcarc ;
	header->sb = s->h->sb ;
	header->sdelta = s->h->sdelta ;
	header->fmt = s->h->fmt ;
	header->resp0 = s->h->resp0 ;
	header->resp1 = s->h->resp1 ;
	header->resp2 = s->h->resp2 ;
	header->resp3 = s->h->resp3 ;
	header->resp4 = s->h->resp4 ;
	header->resp5 = s->h->resp5 ;
	header->resp6 = s->h->resp6 ;
	header->resp7 = s->h->resp7 ;
	header->resp8 = s->h->resp8 ;
	header->resp9 = s->h->resp9 ;
	header->unused6 = s->h->unused6;
	header->unused7 = s->h->unused7 ;
	header->unused8 = s->h->unused8;
	header->unused9 = s->h->unused9 ;
	header->unused10 = s->h->unused10 ;
	header->unused11 = s->h->unused11 ;
	header->unused12 = s->h->unused12 ;

	header->ievtyp = s->h->ievtyp ;
	header->imagtyp = s->h->imagtyp ;
	header->imagsrc = s->h->imagsrc ;
	header->unused16 = s->h->unused16 ;
	header->ievreg = s->h->ievreg ;
	header->iqual = s->h->iqual ;
	header->unused19 = s->h->unused19 ;
	header->unused20 = s->h->unused20 ;
	header->unused21 = s->h->unused21 ;
	header->unused22 = s->h->unused22 ;
	header->unused23 = s->h->unused23 ;
	header->unused24 = s->h->unused24 ;
	header->unused25 = s->h->unused25 ;
	header->unused26 = s->h->unused26 ;

	strcpy ( header->kdatrd , s->h->kdatrd );
	strcpy ( header->kevnm , s->h->kevnm );
	strcpy ( header->ko , s->h->ko );
	strcpy ( header->ka , s->h->ka );
	strcpy ( header->kt0 , s->h->kt0 );
	strcpy ( header->kt1 , s->h->kt1 );
	strcpy ( header->kt2 , s->h->kt2 );
	strcpy ( header->kt3 , s->h->kt3 );
	strcpy ( header->kt4 , s->h->kt4 );
	strcpy ( header->kt5 , s->h->kt5 );
	strcpy ( header->kt6 , s->h->kt6 );
	strcpy ( header->kt7 , s->h->kt7 );
	strcpy ( header->kt8 , s->h->kt8 );
	strcpy ( header->kt9 , s->h->kt9 );
	strcpy ( header->kf , s->h->kf );
	strcpy ( header->kuser0 , s->h->kuser0 );
	strcpy ( header->kuser1 , s->h->kuser1 );
	strcpy ( header->kuser2 , s->h->kuser2 );

	header->lovrok = s->h->lovrok ;
	header->lcalda = s->h->lcalda ;
	header->unused27 = s->h->unused27 ;

	header->nzyear = s->h->nzyear ;
	header->nzjday = s->h->nzjday ;
	header->nzhour = s->h->nzhour ;
	header->nzmin = s->h->nzmin ;
	header->nzsec = s->h->nzsec ;
	header->nzmsec = s->h->nzmsec ;
	header->nsnpts = s->h->nsnpts ;
	header->unused15 = s->h->unused15 ;
    } /* end if ( whichHeaders == eventHeader || whichHeaders == allHeader ) */

    if ( whichHeaders == wfHeader || whichHeaders == allHeader ) {
	header->delta = s->h->delta;
        header->depmin = s->h->depmin;
        header->depmax = s->h->depmax;
	header->scale = s->h->scale ;
	header->odelta = s->h->odelta;
	header->stla = s->h->stla ;
	header->stlo = s->h->stlo ;
	header->stel = s->h->stel ;
	header->stdp = s->h->stdp ;
        header->depmen = s->h->depmen ;
	header->cmpaz = s->h->cmpaz ;
	header->cmpinc = s->h->cmpinc ;
	header->xminimum = s->h->xminimum ;
	header->xmaximum = s->h->xmaximum ;
	header->yminimum = s->h->yminimum ;
	header->ymaximum = s->h->ymaximum ;

	header->iftype = s->h->iftype ;
	header->idep = s->h->idep ;
	header->iinst = s->h->iinst ;
	header->istreg = s->h->istreg ;
	header->iztype = s->h->iztype ;
	header->isynth = s->h->isynth ;

	strcpy ( header->kstnm , s->h->kstnm );
	strcpy ( header->khole , s->h->khole );
	strcpy ( header->kcmpnm , s->h->kcmpnm );
	strcpy ( header->knetwk , s->h->knetwk );
	strcpy ( header->kinst , s->h->kinst );

	header->leven = s->h->leven ;
	header->lpspol = s->h->lpspol ;

	header->nvhdr = s->h->nvhdr ;
	header->norid = s->h->norid ;
	header->nevid = s->h->nevid ;
	header->npts = s->h->npts ;
	header->nwfid = s->h->nwfid ;
	header->nxsize = s->h->nxsize ;
	header->nysize = s->h->nysize ;

    } /* end if ( whichHeaders == wfHeader || whichHeaders == allHeader ) */
    else {
	if ( !strcmp ( header->kstnm , SAC_CHAR_UNDEFINED ) && strcmp ( s->h->kstnm , SAC_CHAR_UNDEFINED ) )
	    strcpy ( header->kstnm , s->h->kstnm );
	if ( !strcmp ( header->kcmpnm , SAC_CHAR_UNDEFINED ) && strcmp ( s->h->kcmpnm , SAC_CHAR_UNDEFINED ) )
	    strcpy ( header->kcmpnm , s->h->kcmpnm );
	if ( !strcmp ( header->khole , SAC_CHAR_UNDEFINED ) && strcmp ( s->h->khole , SAC_CHAR_UNDEFINED ) )
	     strcpy ( header->khole , s->h->khole );
	if ( !strcmp ( header->knetwk , SAC_CHAR_UNDEFINED ) && strcmp ( s->h->knetwk , SAC_CHAR_UNDEFINED ) )
	    strcpy ( header->knetwk , s->h->knetwk );

	if ( header->nvhdr == cmhdr.nundef && s->h->nvhdr != cmhdr.nundef )
	    header->nvhdr = s->h->nvhdr ;
	if ( header->norid == cmhdr.nundef && s->h->norid != cmhdr.nundef )
	    header->norid = s->h->norid ;
	if ( header->nevid == cmhdr.nundef && s->h->nevid != cmhdr.nundef )
	    header->nevid = s->h->nevid ;
	if ( header->nwfid == cmhdr.nundef && s->h->nwfid != cmhdr.nundef )
	    header->nwfid = s->h->nwfid ;
    }

} /* end SacHeaderToDB () */
