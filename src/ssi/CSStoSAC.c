
#include <stdio.h>
#include <string.h>

#include "ssi.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"

#include "msg.h"
#include "clf.h"
#include "bot.h"
#include "dff.h"

extern sac *cut_file;

void
CSStoSAC(idfl, header, seis, lname, lcutnow, nerr)
     int idfl, *nerr;
     struct trace *seis;
     struct SACheader *header;
     int lname, lcutnow;
{
    /* Declare Variables. */
    char kfile[MCPFN + 1];
    sac *s;
    /*=====================================================================
     * PURPOSE:  Called by xreaddb() to get CSS formated data from SeisMgr
     *           into SAC.  
     *=====================================================================
     * INPUT ARGUMENTS:
     *    idfl:      file number of first file being handled currently
     *    header:    SeisMgr struct containing a SAC formated header
     *    seis:      seismogram
     *=====================================================================
     * OUTPUT ARGUMENTS:
     *    nerr:    Error flag. Set to 0 if no error occurred.
     *=====================================================================
     * MODIFICATION HISTORY:
     *    971202:  Original version.  maf  much code plagerized form readfl
     *                                     and associated functions.
     *===================================================================== */

    *nerr = 0;
    s = sac_new();

    /* -- Get header. */
    DBheaderToSac(header, TRUE, s);

    /* Give the file a name internally */
    if (lname) {
        char tempSta[9], tempChan[9];

        if (!strcmp(header->kstnm, SAC_CHAR_UNDEFINED))
            sprintf(tempSta, "s%03d", idfl);
        else
            strcpy(tempSta, header->kstnm);

        if (!strcmp(header->kcmpnm, SAC_CHAR_UNDEFINED))
            sprintf(tempChan, "c%03d", idfl);
        else
            strcpy(tempChan, header->kcmpnm);

        terminate(tempSta);
        terminate(tempChan);

        sprintf(kfile, "%s.%s.%04d%03d%02d%02d%02d", tempSta, tempChan,
                s->h->nzyear, s->h->nzjday, s->h->nzhour, s->h->nzmin,
                s->h->nzsec);

        s->m->filename = fstrdup(kfile, strlen(kfile) + 1);
    } else {
        if (cut_file && cut_file->m->filename) {
            s->m->filename = strdup(cut_file->m->filename);
        }
    }

    sacput(s);
    idfl = saclen();

    /* -- Prepare to get waveform */
    defmem(idfl, lcutnow, nerr);
    if (*nerr)
        goto L_8888;

    sac_alloc(s);

    /* Get waveform */
    DBwfToSac(s, seis, nerr);
    if (*nerr)
        goto L_8888;

  L_8888:
    if (*nerr) {
        setmsg("ERROR", *nerr);
        outmsg();
        clrmsg();
    }

    return;
}                               /* end CSStoSAC */
