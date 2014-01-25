
#include <string.h>

#include "ssi.h"
#include "hdr.h"
#include "dfm.h"


#include "msg.h"
#include "dff.h"

void SacHeaderToDB ( header , whichHeaders , idfl )
struct SACheader *header ;
int whichHeaders ;
int idfl ;
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
	if ( idfl > 0 )
	    putfil ( idfl , &err ) ;
	if ( err && err != 1365 ) {
	    setmsg ( "ERROR" , err ) ;
	    outmsg () ;
	}
    }

    /* Copy each variable from the SAC header into header. */
    if ( whichHeaders == eventHeader || whichHeaders == allHeader ) {
	header->b = *begin ;
	header->e = *ennd;
	header->o = *origin ;
	header->a = *arrivl ;
	header->t0 = *t0 ;
	header->t1 = *t1 ;
	header->t2 = *t2 ;
	header->t3 = *t3 ;
	header->t4 = *t4 ;
	header->t5 = *t5 ;
	header->t6 = *t6 ;
	header->t7 = *t7 ;
	header->t8 = *t8 ;
	header->t9 = *t9 ;
	header->f = *fini ;
	header->evla = *evla ;
	header->evlo = *evlo ;
	header->evel = *evel ;
	header->evdp = *evdp ;
	header->mag = *mag ;
	header->user0 = *user0 ;
	header->user1 = *user1 ;
	header->user2 = *user2 ;
	header->user3 = *user3 ;
	header->user4 = *user4 ;
	header->user5 = *user5 ;
	header->user6 = *user6 ;
	header->user7 = *user7 ;
	header->user8 = *user8 ;
	header->user9 = *user9 ;
	header->dist = *dist ;
	header->az = *az ;
	header->baz = *baz ;
	header->gcarc = *gcarc ;
	header->sb = *sb ;
	header->sdelta = *sdelta ;
	header->fmt = *fmt ;
	header->resp0 = *resp0 ;
	header->resp1 = *resp1 ;
	header->resp2 = *resp2 ;
	header->resp3 = *resp3 ;
	header->resp4 = *resp4 ;
	header->resp5 = *resp5 ;
	header->resp6 = *resp6 ;
	header->resp7 = *resp7 ;
	header->resp8 = *resp8 ;
	header->resp9 = *resp9 ;
	header->unused6 = *fhdr64 ;
	header->unused7 = *fhdr65 ;
	header->unused8 = *fhdr66 ;
	header->unused9 = *fhdr67 ;
	header->unused10 = *fhdr68 ;
	header->unused11 = *fhdr69 ;
	header->unused12 = *fhdr70 ;

	header->ievtyp = *ievtyp ;
	header->imagtyp = *imagtyp ;
	header->imagsrc = *imagsrc ;
	header->unused16 = *ihdr4 ;
	header->ievreg = *ievreg ;
	header->iqual = *iqual ;
	header->unused19 = *ihdr13 ;
	header->unused20 = *ihdr14 ;
	header->unused21 = *ihdr15 ;
	header->unused22 = *ihdr16 ;
	header->unused23 = *ihdr17 ;
	header->unused24 = *ihdr18 ;
	header->unused25 = *ihdr19 ;
	header->unused26 = *ihdr20 ;

	strcpy ( header->kdatrd , kdatrd );
	strcpy ( header->kevnm , kevnm );
	strcpy ( header->ko , ko );
	strcpy ( header->ka , ka );
	strcpy ( header->kt0 , kt0 );
	strcpy ( header->kt1 , kt1 );
	strcpy ( header->kt2 , kt2 );
	strcpy ( header->kt3 , kt3 );
	strcpy ( header->kt4 , kt4 );
	strcpy ( header->kt5 , kt5 );
	strcpy ( header->kt6 , kt6 );
	strcpy ( header->kt7 , kt7 );
	strcpy ( header->kt8 , kt8 );
	strcpy ( header->kt9 , kt9 );
	strcpy ( header->kf , kf );
	strcpy ( header->kuser0 , kuser0 );
	strcpy ( header->kuser1 , kuser1 );
	strcpy ( header->kuser2 , kuser2 );

	header->lovrok = *lovrok ;
	header->lcalda = *lcalda ;
	header->unused27 = *lhdr5 ;

	header->nzyear = *nzyear ;
	header->nzjday = *nzjday ;
	header->nzhour = *nzhour ;
	header->nzmin = *nzmin ;
	header->nzsec = *nzsec ;
	header->nzmsec = *nzmsec ;
	header->nsnpts = *nsnpts ;
	header->unused15 = *nhdr15 ;
    } /* end if ( whichHeaders == eventHeader || whichHeaders == allHeader ) */

    if ( whichHeaders == wfHeader || whichHeaders == allHeader ) {
	header->delta = *delta;
        header->depmin = *depmin;
        header->depmax = *depmax;
	header->scale = *scale ;
	header->odelta = *odelta;
	header->stla = *stla ;
	header->stlo = *stlo ;
	header->stel = *stel ;
	header->stdp = *stdp ;
        header->depmen = *depmen ;
	header->cmpaz = *cmpaz ;
	header->cmpinc = *cmpinc ;
	header->xminimum = *xminimum ;
	header->xmaximum = *xmaximum ;
	header->yminimum = *yminimum ;
	header->ymaximum = *ymaximum ;

	header->iftype = *iftype ;
	header->idep = *idep ;
	header->iinst = *iinst ;
	header->istreg = *istreg ;
	header->iztype = *iztype ;
	header->isynth = *isynth ;

	strcpy ( header->kstnm , kstnm );
	strcpy ( header->khole , khole );
	strcpy ( header->kcmpnm , kcmpnm );
	strcpy ( header->knetwk , knetwk );
	strcpy ( header->kinst , kinst );

	header->leven = *leven ;
	header->lpspol = *lpspol ;

	header->nvhdr = *nvhdr ;
	header->norid = *norid ;
	header->nevid = *nevid ;
	header->npts = *npts ;
	header->nwfid = *nwfid ;
	header->nxsize = *nxsize ;
	header->nysize = *nysize ;

    } /* end if ( whichHeaders == wfHeader || whichHeaders == allHeader ) */
    else {
	if ( !strcmp ( header->kstnm , kmhdr.kundef ) && strcmp ( kstnm , kmhdr.kundef ) )
	    strcpy ( header->kstnm , kstnm );
	if ( !strcmp ( header->kcmpnm , kmhdr.kundef ) && strcmp ( kcmpnm , kmhdr.kundef ) )
	    strcpy ( header->kcmpnm , kcmpnm );
	if ( !strcmp ( header->khole , kmhdr.kundef ) && strcmp ( khole , kmhdr.kundef ) )
	     strcpy ( header->khole , khole );
	if ( !strcmp ( header->knetwk , kmhdr.kundef ) && strcmp ( knetwk , kmhdr.kundef ) )
	    strcpy ( header->knetwk , knetwk );

	if ( header->nvhdr == cmhdr.nundef && *nvhdr != cmhdr.nundef )
	    header->nvhdr = *nvhdr ;
	if ( header->norid == cmhdr.nundef && *norid != cmhdr.nundef )
	    header->norid = *norid ;
	if ( header->nevid == cmhdr.nundef && *nevid != cmhdr.nundef )
	    header->nevid = *nevid ;
	if ( header->nwfid == cmhdr.nundef && *nwfid != cmhdr.nundef )
	    header->nwfid = *nwfid ;
    }

} /* end SacHeaderToDB () */
