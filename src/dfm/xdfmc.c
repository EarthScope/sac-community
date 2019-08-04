/** 
 * @file   xdfmc.c
 * 
 * @brief  Execute a Data file list Command
 * 
 */

#include <stdio.h>

#include "dfm.h"
#include "bool.h"

#include "errors.h"

#include "msg.h"
#include "ssi.h"
#include "cpf.h"

DFM_EXTERN

#include "hdr.h"
#include "amf.h"

#include "octopus.h"

char *rstrip (char *s);

int   lkdur(char *kkey, duration **d);
int   levent(Event **e);
int   lktp(char *kkey, timespec64 *t1, timespec64 *t2);
int   lktn(char *kkey, timespec64 ***t);

void  event_request(int *nerr);
void  station_request(int *nerr);
void  data_request_f(int *nerr);
void  meta_request(int *nerr);


static int str_is(char *a, char *b) {
    return strcmp(a, b) == 0;
}
static int
sac_str_def(char *s) {
    return ! str_is(s, SAC_CHAR_UNDEFINED) ;
}

void
sacpz_request(int *nerr) {
    char file[2048] = {0};
    char nslc[128] = { 0 };
    char keys[2][9] = {"sacpz\0\0\0",
                       "resp\0\0\0\0"};
    int kind = 1;
    timespec64 t = {0};
    timespec64 **ts = NULL;
    request *pz = sacpz_new();
    result *r = NULL;
    *nerr = SAC_OK;
    while( lcmore(nerr) ) {
        if(lclist((char *)keys, 9, 2, &kind)) {
            sacpz_set_kind(pz, kind);
        }
        else if(lckey("verbose$",  -1)){
            request_set_verbose(pz, TRUE);
        }
        else if(lkchar2("STA#TION$",  nslc, sizeof(nslc))) {
            sacpz_set_station(pz, nslc);
        }
        else if(lkchar2("NET#WORK$",  nslc, sizeof(nslc))) {
            sacpz_set_network(pz, nslc);
        }
        else if(lkchar2("CHA#NNEL$",  nslc, sizeof(nslc))) {
            sacpz_set_channel(pz, nslc);
        }
        else if(lkchar2("LOC#ATION$", nslc, sizeof(nslc))) {
            sacpz_set_location(pz, nslc);
        }
        else if(lktn("TIME$", &ts)) {
            if(xarray_length(ts) == 1) {
                sacpz_set_time(pz, *ts[0] );
            } else if (xarray_length(ts) == 2) {
                sacpz_set_start(pz, *ts[0] );
                sacpz_set_end(pz, *ts[1] );
            }
        }
        else {
            cfmt("ILLEGAL OPTION:", 17);
            cresp();
        }
    }
    if(*nerr != SAC_OK) {
        goto error;
    }
    if(sacpz_is_ok(pz)) {
        r = request_get(pz);
        if(!result_is_ok(r)) {
            printf("%s", result_error_msg(r));
            goto error;
        }
        result_write_to_file_show(r, sacpz_filename(pz, file, sizeof(file)));
    } else if(saclen() > 0) {
        REQUEST_FREE(pz);
        for( int i = 0 ; i < saclen(); i++) {
            /* -- Get the next file in DFL, moving header to CMHDR. */
            sac *s;
            if (!(s = sacget(i, TRUE, nerr))) {
                goto error;
            }
            if(sac_str_def(s->h->knetwk) &&
               sac_str_def(s->h->kstnm) &&
               sac_str_def(s->h->kcmpnm)) {
                char p[16];

                if(!sac_str_def(s->h->khole) || strlen(s->h->khole) == 0) {
                    strlcpy(p, "--", sizeof(p));
                } else {
                    strlcpy(p, s->h->khole, sizeof(p));
                }
                pz = sacpz_new();
                sacpz_set_kind(pz, kind);
                sacpz_set_location(pz, p);

                strlcpy(p, s->h->knetwk, sizeof(p));
                sacpz_set_network (pz, rstrip(p));

                strlcpy(p, s->h->kstnm, sizeof(p));
                sacpz_set_station (pz, rstrip(p));

                strlcpy(p, s->h->kcmpnm, sizeof(p));
                sacpz_set_channel (pz, rstrip(p));

                if(sac_get_time(s, SAC_B, &t)) {
                    sacpz_set_start(pz, t);
                }
                if(sac_get_time(s, SAC_E, &t)) {
                    sacpz_set_end(pz, t);
                }
                r = request_get(pz);
                if(!result_is_ok(r)) {
                    printf("%s", result_error_msg(r));
                    goto error;
                }
                result_write_to_file_show(r,
                           sacpz_filename(pz, file, sizeof(file)));
                RESULT_FREE(r);
                REQUEST_FREE(pz);
            } else {
                printf("net: '%s'\n", s->h->knetwk);
                printf("sta: '%s'\n", s->h->kstnm);
                printf("loc: '%s'\n", s->h->khole);
                printf("cha: '%s'\n", s->h->kcmpnm);
                error(*nerr = 3264,
                      "Response request either requires a data file with meta data\n"
                      "    knetwm, kstnm, khole, kcmpnm [kzdate/kztime]\n"
                      "    or net, sta, loc, and cha [time/start/end]");
            }
        }
    } else {
        error(*nerr = 3264, "Response request either requires a data file with meta data\n"
              "    knetwm, kstnm, khole, kcmpnm [kzdate/kztime]\n"
              "    or net, sta, loc, and cha [time/start/end]");
        goto error;
    }
 error:
    REQUEST_FREE(pz);
    RESULT_FREE(r);

    return;
}


/**
 * Execute a Data File List Command given its index number
 *
 * @param index
 *    Index number of the command
 * @param nerr
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_SAC_LOGIC_ERROR
 *
 * @date   981030:  Removed dataset commands, added READGSE, READSUDS and SORT
 * @date   970409:  Added PICKAUTHOR and PICIPHASE for flexibility in
 *                  reading picks with readcss. maf 970409
 * @date   970203:  Added DELETECHANNEL to remove a file from memory.  maf
 * @date   911107:  Added GROUP command to group current data-sets to new ds.
 * @date   910916:  Added LDS command for listing defined and current data-sets.
 * @date   910827:  Added COPY command for copying a data-set
 * @date   910827:  Removed include files hdr and mem.
 * @date   910826:  Added RENAME and DELETE commands for multiple data-sets.
 * @date   910823:  Added CDS - Current Data Set.
 * @date   910430:  Added READCSS command.
 * @date   900905:  Added READSDD and WRITESDD command.
 * @date   870615:  Added DATAGEN command.
 * @date   870209:  Added COPYHDR command.
 * @date   870203:  Deleted DESAMP and MEMORY commands.
 *                  Changed SMOOTH to an action command and moved to SCM.
 *                  Renumbered some of the remaining commands.
 * @date   860917:  Added READALPHA and WILD commands.
 * @date   860317:  Changed command numbering for this module.
 * @date   831020:  Added SYNCH command.
 * @date   820801:  Original version.
 *
 */
void
xdfmc(int index, int *nerr) {

    int lsdd;

    *nerr = 0;

    /* - Jump to correct command based upon its index number. */
    switch (index) {
        case 1:
            goto L_100;
        case 2:
            goto L_200;
        case 3:
            goto L_300;
        case 4:
            goto L_400;
        case 5:
            goto L_500;
        case 6:
            goto L_600;
        case 7:
            goto L_700;
        case 8:
            goto L_800;
        case 9:
            goto L_900;
        case 10:
            goto L_1000;
        case 11:
            goto L_1100;
        case 12:
            goto L_1200;
        case 13:
            goto L_1300;
        case 14:
            goto L_1400;
        case 15:
            goto L_1500;
        case 16:
            goto L_1600;
        case 17:
            goto L_1700;
        case 18:
            goto L_1800;
        case 20:
            goto L_2000;
        case 21:
            goto L_2100;
        case 23:
            goto L_2300;
        case 24:
            goto L_2400;
        case 25:
            goto L_2500;
        case 26:
            goto L_2600;
        case 27:
            goto L_2700;
        case 28:
            goto L_2800;
        case 29:
            goto L_2900;
        case 30:
            goto L_3000;
        case 31:
            goto L_3100;
        case 32:
            goto L_3200;
        case 33:
            goto L_3300;
    case 34:
        goto L_3400;
    case 35:
        goto L_3500;
    case 36:
        goto L_3600;
    case 37:
        goto L_3700;
    case 38:
        goto L_3800;
    }

    /* - Error return if bad index value. */

    *nerr = ERROR_SAC_LOGIC_ERROR;
    setmsg("ERROR", *nerr);
    apcmsg("in XDFMC", 9);
    goto L_8888;

  L_100:
    /* - Command 01: READ */
    xr(nerr);
    goto L_8888;

  L_200:
    /* - Command 02: READERR */
    xrerr(nerr);
    goto L_8888;

  L_300:
    /* - Command 03: WRITE */
    lsdd = FALSE;
    xw(lsdd, nerr);
    goto L_8888;

  L_400:
    /* - Command 04: CONVERT */
    xconv(nerr);
    goto L_8888;

  L_500:
    /* - Command 05: CUT */
    xcrtw(&cmdfm.lcut, (char *) kmdfm.kcut, 9, cmdfm.ocut, nerr);
    goto L_8888;

  L_600:
    /* - Command 06: CUTERR */
    xcuter(nerr);
    goto L_8888;

  L_700:
    /* - Command 07: LISTHDR */
    xlh(nerr);
    goto L_8888;

  L_800:
    /* - Command 08: CHNHDR */
    xch(nerr);
    goto L_8888;

  L_900:
    /* - Command 09: READHDR */
    xrh(nerr);
    goto L_8888;

  L_1000:
    /* - Command 10: WRITEHDR */
    xwh(nerr);
    goto L_8888;

  L_1100:
    /* - Command 11: SYNCHRONIZE */
    xsynch(nerr);
    goto L_8888;

  L_1200:
    /* - Command 12: WILD */
    xwild(nerr);
    goto L_8888;

  L_3300:
    fprintf(stderr, "Command READALPHA is deprecated, please use READTABLE\n");
  L_1300:
    /* - Command 13: READALPHA */

    xrtab(FALSE, nerr);
    goto L_8888;

  L_1400:
    /* - Command 14: COPYHDR */
    xcopyhdr(nerr);
    goto L_8888;

  L_1500:
    /* - Command 15: DATAGEN */
    xdatagen(nerr);
    goto L_8888;

  L_1600:
    /* - Command 16: READSDD */
    xrsdd(nerr);
    goto L_8888;

  L_1700:
    /* - Command 17: WRITESDD */
    lsdd = TRUE;
    xw(lsdd, nerr);
    goto L_8888;

  L_1800:
    /* - Command 18: READCSS */
    xrcss(nerr);
    goto L_8888;

  L_2000:
    /* - Command 20: CUTIM (located in ssi module, included
       here because ssi has alternate command parsing which
       won't work for this command. */
    xcutim(nerr);
    goto L_8888;

    /* - Command 21: WRITETABLE */
  L_2100:
    xwtab(nerr);
    goto L_8888;

  L_2300:
    xpickprefs(nerr);
    goto L_8888;

  L_2400:
    /* - Command 24: write GSE file */
    xwgse(nerr);
    goto L_8888;

  L_2500:
    /* - Command 25: write to CSS flat files or binary files */
    xwcss(nerr);
    goto L_8888;

  L_2600:
    /* removed to compile under sunos without libgdi */
    /*      xrcssdb( nerr ); */
    goto L_8888;

  L_2700:
    /* - Command 27: DELETECHANNEL removes one or more files.  maf 970203 */
    xdeletechannel(nerr);
    goto L_8888;

  L_2800:
    /* - Command 28: PICKAUTHOR allows user to specify authors in
       for readcss to read picks.  Authors are specified in
       priority order. maf 970409 */
    xpickauthor(nerr);
    goto L_8888;

  L_2900:
    /* - Command 29: PICKPHASE allows user to specify phases and
       authors for specific pick slots (t0, t1, etc.). maf 970409 */
    xpickphase(nerr);
    goto L_8888;

  L_3000:
    /* - Command 30: SORT sorts files in memory on two
       numeric header variables.  maf 980812 */
    xsort(nerr);
    goto L_8888;

  L_3100:
    /* - Command 31: READSUDS reads PC-SUDS files into memory */
    xrsuds(nerr);
    goto L_8888;

  L_3200:
    /* - Command 31: READGSE reads GSE files into memory */
    xrgse(nerr);
    goto L_8888;

 L_3400:
    sacpz_request(nerr);
    goto L_8888;
 L_3500:
    event_request(nerr);
    goto L_8888;
 L_3600:
    data_request_f(nerr);
    goto L_8888;
 L_3700:
    station_request(nerr);
    goto L_8888;
 L_3800:
    meta_request(nerr);
    goto L_8888;

  L_8888:
    return;
}
