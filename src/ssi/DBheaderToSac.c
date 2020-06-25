
#include <string.h>

#include "ssi.h"
#include "hdr.h"

#include "amf.h"
#include "msg.h"
#include "ucf.h"
#include "dff.h"

void
DBheaderToSac(header, lall, s)
     struct SACheader *header;
     int lall;
     sac *s;
{

    int idx, err = 0;

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
    if (lall) {
        sac_set_float(s, SAC_B, header->_b);
        sac_set_float(s, SAC_E, header->_e);
        sac_set_float(s, SAC_O, header->_o);
        sac_set_float(s, SAC_A, header->_a);
        sac_set_float(s, SAC_T0, header->_t0);
        sac_set_float(s, SAC_T1, header->_t1);
        sac_set_float(s, SAC_T2, header->_t2);
        sac_set_float(s, SAC_T3, header->_t3);
        sac_set_float(s, SAC_T4, header->_t4);
        sac_set_float(s, SAC_T5, header->_t5);
        sac_set_float(s, SAC_T6, header->_t6);
        sac_set_float(s, SAC_T7, header->_t7);
        sac_set_float(s, SAC_T8, header->_t8);
        sac_set_float(s, SAC_T9, header->_t9);
        sac_set_float(s, SAC_F, header->_f);

        sac_set_float(s, SAC_EVLA, header->_evla);
        sac_set_float(s, SAC_EVLO, header->_evlo);
        sac_set_float(s, SAC_EVEL, header->evel);
        sac_set_float(s, SAC_EVDP, header->evdp);

        sac_set_float(s, SAC_MAG, header->mag);

        sac_set_float(s, SAC_USER0, header->user0);
        sac_set_float(s, SAC_USER1, header->user1);
        sac_set_float(s, SAC_USER2, header->user2);
        sac_set_float(s, SAC_USER3, header->user3);
        sac_set_float(s, SAC_USER4, header->user4);
        sac_set_float(s, SAC_USER5, header->user5);
        sac_set_float(s, SAC_USER6, header->user6);
        sac_set_float(s, SAC_USER7, header->user7);
        sac_set_float(s, SAC_USER8, header->user8);
        sac_set_float(s, SAC_USER9, header->user9);

        sac_set_float(s, SAC_DIST, header->dist);
        sac_set_float(s, SAC_AZ, header->az);
        sac_set_float(s, SAC_BAZ, header->baz);
        sac_set_float(s, SAC_GCARC, header->gcarc);

        sac_set_float(s, SAC_SB, header->_sb);
        sac_set_float(s, SAC_SDELTA, header->_sdelta);

        sac_set_float(s, SAC_FMT, header->fmt);

        sac_set_float(s, SAC_RESP0, header->resp0);
        sac_set_float(s, SAC_RESP1, header->resp1);
        sac_set_float(s, SAC_RESP2, header->resp2);
        sac_set_float(s, SAC_RESP3, header->resp3);
        sac_set_float(s, SAC_RESP4, header->resp4);
        sac_set_float(s, SAC_RESP5, header->resp5);
        sac_set_float(s, SAC_RESP6, header->resp6);
        sac_set_float(s, SAC_RESP7, header->resp7);
        sac_set_float(s, SAC_RESP8, header->resp8);
        sac_set_float(s, SAC_RESP9, header->resp9);

        sac_set_float(s, SAC_UN64, header->unused6);
        sac_set_float(s, SAC_UN65, header->unused7);
        sac_set_float(s, SAC_UN66, header->unused8);
        sac_set_float(s, SAC_UN67, header->unused9);
        sac_set_float(s, SAC_UN68, header->unused10);
        sac_set_float(s, SAC_UN69, header->unused11);
        sac_set_float(s, SAC_UN70, header->unused12);

        s->h->ievtyp = header->ievtyp;
        s->h->imagtyp = header->imagtyp;
        s->h->imagsrc = header->imagsrc;
        s->h->unused16 = header->unused16;
        s->h->ievreg = header->ievreg;
        s->h->iqual = header->iqual;
        s->h->ibody = header->ibody;
        s->h->unused20 = header->unused20;
        s->h->unused21 = header->unused21;
        s->h->unused22 = header->unused22;
        s->h->unused23 = header->unused23;
        s->h->unused24 = header->unused24;
        s->h->unused25 = header->unused25;
        s->h->unused26 = header->unused26;

        strcpy(s->h->kdatrd, header->kdatrd);
        strcpy(s->h->kevnm, "                 ");
        strncpy(s->h->kevnm, header->kevnm, strlen(header->kevnm));
        strcpy(s->h->ko, header->ko);
        strcpy(s->h->ka, header->ka);
        strcpy(s->h->kt0, header->kt0);
        strcpy(s->h->kt1, header->kt1);
        strcpy(s->h->kt2, header->kt2);
        strcpy(s->h->kt3, header->kt3);
        strcpy(s->h->kt4, header->kt4);
        strcpy(s->h->kt5, header->kt5);
        strcpy(s->h->kt6, header->kt6);
        strcpy(s->h->kt7, header->kt7);
        strcpy(s->h->kt8, header->kt8);
        strcpy(s->h->kt9, header->kt9);
        strcpy(s->h->kf, header->kf);
        strcpy(s->h->kuser0, header->kuser0);
        strcpy(s->h->kuser1, header->kuser1);
        strcpy(s->h->kuser2, header->kuser2);

        s->h->lovrok = header->lovrok;
        s->h->lcalda = header->lcalda;
        s->h->unused27 = header->unused27;

        s->h->nzyear = header->nzyear;
        s->h->nzjday = header->nzjday;
        s->h->nzhour = header->nzhour;
        s->h->nzmin = header->nzmin;
        s->h->nzsec = header->nzsec;
        s->h->nzmsec = header->nzmsec;
        s->h->nsnpts = header->nsnpts;
        s->h->unused15 = header->unused15;
    }
    /* end if ( lall ) */
    sac_set_float(s, SAC_DELTA, header->_delta);
    sac_set_float(s, SAC_DEPMIN, header->depmin);
    sac_set_float(s, SAC_DEPMAX, header->depmax);
    sac_set_float(s, SAC_SCALE, header->scale);
    sac_set_float(s, SAC_ODELTA, header->odelta);
    sac_set_float(s, SAC_STLA, header->_stla);
    sac_set_float(s, SAC_STLO, header->_stlo);

    sac_set_float(s, SAC_STEL, header->stel);
    sac_set_float(s, SAC_STDP, header->stdp);
    sac_set_float(s, SAC_DEPMEN, header->depmen);
    sac_set_float(s, SAC_CMPAZ, header->cmpaz);
    sac_set_float(s, SAC_CMPINC, header->cmpinc);

    sac_set_float(s, SAC_XMIN, header->xminimum);
    sac_set_float(s, SAC_XMAX, header->xmaximum);
    sac_set_float(s, SAC_YMIN, header->yminimum);
    sac_set_float(s, SAC_YMAX, header->ymaximum);

    s->h->iftype = header->iftype;
    s->h->idep = header->idep;
    s->h->iinst = header->iinst;
    s->h->istreg = header->istreg;
    s->h->iztype = header->iztype;
    s->h->isynth = header->isynth;

    strcpy(s->h->kinst, header->kinst);
    strcpy(s->h->kstnm, header->kstnm);
    strcpy(s->h->kcmpnm, header->kcmpnm);
    strcpy(s->h->khole, header->khole);
    strcpy(s->h->knetwk, header->knetwk);

    s->h->leven = header->leven;
    s->h->lpspol = header->lpspol;

    s->h->npts = header->npts;
    s->h->nxsize = header->nxsize;
    s->h->nysize = header->nysize;
    s->h->nvhdr = header->nvhdr;
    s->h->norid = header->norid;
    s->h->nevid = header->nevid;
    s->h->nwfid = header->nwfid;

    /* calculate dist, az, baz, and gcarc if appropriate */
    update_distaz(s);

    /* Check the ranges of the enumerated header variables */
    for (idx = 0; idx < SAC_HEADER_ENUMS; idx++) {
        int k = VALUE(ihdr(s, idx));
        if (k > SAC_ENUMS || (k < 0 && k != SAC_INT_UNDEFINED)) {
            VALUE(ihdr(s, idx)) = SAC_INT_UNDEFINED;
            err = 1365;
        }
    }

    if (err) {
        setmsg("WARNING", err);
        apcmsg("reset to undefined", 19);
        outmsg();
        clrmsg();
    }

}                               /* end DBheaderToSac () */
