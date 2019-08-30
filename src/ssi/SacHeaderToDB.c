
#include <string.h>

#include "ssi.h"
#include "hdr.h"
#include "dfm.h"

#include "amf.h"
#include "msg.h"
#include "dff.h"
#include "defs.h"

int const wfHeader = -1;
int const allHeader = 0;
int const eventHeader = 1;
float MaxMem = 0.3;

void
SacHeaderToDB(header, whichHeaders, idfl)
     struct SACheader *header;
     int whichHeaders;
     int idfl;
{

    int idx, err = 0;
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
    for (idx = 0; idx < SAC_HEADER_ENUMS; idx++) {
        int k = VALUE(ihdr(s, idx));
        if (k > SAC_ENUMS || (k < 0 && k != SAC_ENUM_UNDEFINED)) {
            VALUE(ihdr(s, idx)) = SAC_ENUM_UNDEFINED;
            err = 1365;
        }
    }

    if (err) {
        setmsg("WARNING", err);
        apcmsg("reset to undefined", 19);
        outmsg();
        clrmsg();
        //      if ( idfl > 0 )
        if (err && err != 1365) {
            setmsg("ERROR", err);
            outmsg();
        }
    }

    /* Copy each variable from the SAC header into header. */
    if (whichHeaders == eventHeader || whichHeaders == allHeader) {
        header->_b = (float) B(s);
        header->_e = (float) E(s);
        header->_o = (float) O(s);
        header->_a = (float) A(s);
        header->_t0 = sac_float(s, SAC_T0);
        header->_t1 = sac_float(s, SAC_T1);
        header->_t2 = sac_float(s, SAC_T2);
        header->_t3 = sac_float(s, SAC_T3);
        header->_t4 = sac_float(s, SAC_T4);
        header->_t5 = sac_float(s, SAC_T5);
        header->_t6 = sac_float(s, SAC_T6);
        header->_t7 = sac_float(s, SAC_T7);
        header->_t8 = sac_float(s, SAC_T8);
        header->_t9 = sac_float(s, SAC_T9);
        header->_f = sac_float(s, SAC_F);
        header->_evla = sac_float(s, SAC_EVLA);
        header->_evlo = sac_float(s, SAC_EVLO);
        header->evel = sac_float(s, SAC_EVEL);
        header->evdp = sac_float(s, SAC_EVDP);
        header->mag = sac_float(s, SAC_MAG);
        header->user0 = sac_float(s, SAC_USER0);
        header->user1 = sac_float(s, SAC_USER1);
        header->user2 = sac_float(s, SAC_USER2);
        header->user3 = sac_float(s, SAC_USER3);
        header->user4 = sac_float(s, SAC_USER4);
        header->user5 = sac_float(s, SAC_USER5);
        header->user6 = sac_float(s, SAC_USER6);
        header->user7 = sac_float(s, SAC_USER7);
        header->user8 = sac_float(s, SAC_USER8);
        header->user9 = sac_float(s, SAC_USER9);
        header->dist = sac_float(s, SAC_DIST);
        header->az = sac_float(s, SAC_AZ);
        header->baz = sac_float(s, SAC_BAZ);
        header->gcarc = sac_float(s, SAC_GCARC);
        header->_sb = sac_float(s, SAC_SB);
        header->_sdelta = sac_float(s, SAC_SDELTA);
        header->fmt = sac_float(s, SAC_FMT);
        header->resp0 = sac_float(s, SAC_RESP0);
        header->resp1 = sac_float(s, SAC_RESP1);
        header->resp2 = sac_float(s, SAC_RESP2);
        header->resp3 = sac_float(s, SAC_RESP3);
        header->resp4 = sac_float(s, SAC_RESP4);
        header->resp5 = sac_float(s, SAC_RESP5);
        header->resp6 = sac_float(s, SAC_RESP6);
        header->resp7 = sac_float(s, SAC_RESP7);
        header->resp8 = sac_float(s, SAC_RESP8);
        header->resp9 = sac_float(s, SAC_RESP9);
        header->unused6 = sac_float(s, SAC_UN64);
        header->unused7 = sac_float(s, SAC_UN65);
        header->unused8 = sac_float(s, SAC_UN66);
        header->unused9 = sac_float(s, SAC_UN67);
        header->unused10 = sac_float(s, SAC_UN68);
        header->unused11 = sac_float(s, SAC_UN69);
        header->unused12 = sac_float(s, SAC_UN70);

        header->ievtyp = s->h->ievtyp;
        header->imagtyp = s->h->imagtyp;
        header->imagsrc = s->h->imagsrc;
        header->unused16 = s->h->unused16;
        header->ievreg = s->h->ievreg;
        header->iqual = s->h->iqual;
        header->unused19 = s->h->unused19;
        header->unused20 = s->h->unused20;
        header->unused21 = s->h->unused21;
        header->unused22 = s->h->unused22;
        header->unused23 = s->h->unused23;
        header->unused24 = s->h->unused24;
        header->unused25 = s->h->unused25;
        header->unused26 = s->h->unused26;

        strcpy(header->kdatrd, s->h->kdatrd);
        strcpy(header->kevnm, s->h->kevnm);
        strcpy(header->ko, s->h->ko);
        strcpy(header->ka, s->h->ka);
        strcpy(header->kt0, s->h->kt0);
        strcpy(header->kt1, s->h->kt1);
        strcpy(header->kt2, s->h->kt2);
        strcpy(header->kt3, s->h->kt3);
        strcpy(header->kt4, s->h->kt4);
        strcpy(header->kt5, s->h->kt5);
        strcpy(header->kt6, s->h->kt6);
        strcpy(header->kt7, s->h->kt7);
        strcpy(header->kt8, s->h->kt8);
        strcpy(header->kt9, s->h->kt9);
        strcpy(header->kf, s->h->kf);
        strcpy(header->kuser0, s->h->kuser0);
        strcpy(header->kuser1, s->h->kuser1);
        strcpy(header->kuser2, s->h->kuser2);

        header->lovrok = s->h->lovrok;
        header->lcalda = s->h->lcalda;
        header->unused27 = s->h->unused27;

        header->nzyear = s->h->nzyear;
        header->nzjday = s->h->nzjday;
        header->nzhour = s->h->nzhour;
        header->nzmin = s->h->nzmin;
        header->nzsec = s->h->nzsec;
        header->nzmsec = s->h->nzmsec;
        header->nsnpts = s->h->nsnpts;
        header->unused15 = s->h->unused15;
    }
    /* end if ( whichHeaders == eventHeader || whichHeaders == allHeader ) */
    if (whichHeaders == wfHeader || whichHeaders == allHeader) {
        header->_delta = sac_float(s, SAC_DELTA);
        header->depmin = s->h->depmin;
        header->depmax = s->h->depmax;
        header->scale = s->h->scale;
        header->odelta = s->h->odelta;
        header->_stla = sac_float(s, SAC_STLA);
        header->_stlo = sac_float(s, SAC_STLO);
        header->stel = s->h->stel;
        header->stdp = s->h->stdp;
        header->depmen = s->h->depmen;
        header->cmpaz = s->h->cmpaz;
        header->cmpinc = s->h->cmpinc;
        header->xminimum = s->h->xminimum;
        header->xmaximum = s->h->xmaximum;
        header->yminimum = s->h->yminimum;
        header->ymaximum = s->h->ymaximum;

        header->iftype = s->h->iftype;
        header->idep = s->h->idep;
        header->iinst = s->h->iinst;
        header->istreg = s->h->istreg;
        header->iztype = s->h->iztype;
        header->isynth = s->h->isynth;

        strcpy(header->kstnm, s->h->kstnm);
        strcpy(header->khole, s->h->khole);
        strcpy(header->kcmpnm, s->h->kcmpnm);
        strcpy(header->knetwk, s->h->knetwk);
        strcpy(header->kinst, s->h->kinst);

        header->leven = s->h->leven;
        header->lpspol = s->h->lpspol;

        header->nvhdr = s->h->nvhdr;
        header->norid = s->h->norid;
        header->nevid = s->h->nevid;
        header->npts = s->h->npts;
        header->nwfid = s->h->nwfid;
        header->nxsize = s->h->nxsize;
        header->nysize = s->h->nysize;

    } /* end if ( whichHeaders == wfHeader || whichHeaders == allHeader ) */
    else {
        if (is_kundef(header->kstnm) &&  !is_kundef(s->h->kstnm)) {
            strcpy(header->kstnm, s->h->kstnm);
        }
        if (is_kundef(header->kcmpnm) && !is_kundef(s->h->kcmpnm)) {
            strcpy(header->kcmpnm, s->h->kcmpnm);
        }
        if (is_kundef(header->khole) && !is_kundef(s->h->khole)) {
            strcpy(header->khole, s->h->khole);
        }
        if (is_kundef(header->knetwk) && !is_kundef(s->h->knetwk)) {
            strcpy(header->knetwk, s->h->knetwk);
        }
        if (header->nvhdr == SAC_INT_UNDEFINED &&
            s->h->nvhdr != SAC_INT_UNDEFINED)
            header->nvhdr = s->h->nvhdr;
        if (header->norid == SAC_INT_UNDEFINED &&
            s->h->norid != SAC_INT_UNDEFINED)
            header->norid = s->h->norid;
        if (header->nevid == SAC_INT_UNDEFINED &&
            s->h->nevid != SAC_INT_UNDEFINED)
            header->nevid = s->h->nevid;
        if (header->nwfid == SAC_INT_UNDEFINED &&
            s->h->nwfid != SAC_INT_UNDEFINED)
            header->nwfid = s->h->nwfid;
    }

}                               /* end SacHeaderToDB () */
