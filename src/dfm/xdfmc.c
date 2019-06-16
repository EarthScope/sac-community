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

int   lkdur(char *kkey, duration **d);
int   levent(Event **e);
int   lktp(char *kkey, datetime **t1, datetime **t2);

void  event_request(int *nerr);
void  station_request(int *nerr);

char * datetime_strptime(char *buf, char *fmt, datetime * t);

static datetime *
datetime_get(char *time, int *nerr) {
    char *p;
    datetime t;
    datetime_init(&t);
    if(((p = datetime_strptime(time, "%FT%R.%P", &t)) != NULL) ||
       ((p = datetime_strptime(time, "%FT%T", &t)) != NULL) ||
       ((p = datetime_strptime(time, "%F", &t)) != NULL)) {
        if(p && *p == 0) {
            return datetime_copy(&t);
        }
    }
    error(*nerr = 3264,
          "Datetime format must be like: \n"
          "     - yyyy-mm-ddTHH:MM:SS.ppp  or\n"
          "     - yyyy-mm-ddTHH:MM:SS  or\n"
          "     - yyyy-mm-dd\n"
          "       input: '%s'", time);
    return NULL;
}


double *
stat_meta(char *data, size_t data_len,
          char *net, char *sta, char *loc, char *cha,
          datetime *start, datetime *end,
          char **values, int verbose, int *nerr) {
    xmlDoc *doc = NULL;
    xmlNode *nslc = NULL;
    *nerr = 0;
    if(verbose) {
        printf("   Parsing station.xml data\n");
    }
    if(!(doc = xml_init(data, data_len))) {
        *nerr = 3264;
    }

    if(verbose) {
        printf("   Looking in station.xml data for %s.%s.%s.%s\n", net, sta, loc, cha);
    }
    if(!(nslc = xml_get_nslc(doc, doc->children, net, sta, loc, cha, start, end))) {
        *nerr = 404;
        return NULL;
    }
    if(verbose) {
        printf("   Extracting station.xml values\n");
    }
    // Count number of values 
    size_t i = 0;
    while(values[i]) { i++; };

    // Allocate memory for output
    double *out = calloc(i+1, sizeof(double));

    i = 0;
    while(values[i]) {
        if(!(xml_find_double(doc, nslc, values[i], NULL, &out[i]))) {
            *nerr = 404;
            printf("   %20s: value not found\n", values[i]);
            FREE(out);
            goto error;
        }
        if(verbose) {
            printf("   %20s: %f\n", values[i], out[i]);
        }
        i++;
    }
 error:
    if(doc) {
        xmlFreeDoc(doc);
    }
    return out;
}


result *
station_meta_request(station_meta *sm) {
    char *url;
    if((url = station_meta_to_url(sm))) {
        return request(url);
    }
    return result_error(3264, "Error constructing request URL");
}


double *
station_meta_get(char *net, char *sta, char *loc, char *cha,
                 datetime *start, datetime *end,
                 char **values, int verbose,
                 int *nerr) {
    station_meta *sm = station_meta_from_parts(net, sta, loc, cha, start, end);
    result *r = station_meta_request(sm);

    if(result_is_ok(r)) {
        return stat_meta(result_data(r), result_len(r),
                         net, sta, loc, cha,
                         start, end, values, verbose,
                         nerr);
    } else {
        printf("%s", result_error_msg(r));
        *nerr = result_code(r);
    }
    return NULL;
}



#define NINE 9
void
data_request(int *nerr) {
    char net[128] = { 0 }, sta[128] = { 0 }, loc[128] = { 0 }, cha[128] = { 0 };
    char infile[256] = { 0 };
    char outfile[256] = { 0 };
    int nr = 0;
    double reg[4] = {0., 0., 0., 0. };

    Event *ev = NULL;
    duration *d = NULL;
    DataReq *dr = NULL;
    StationReq *sr = NULL;

#define NACTIONS 2
    int action = 2;
    datetime *t1 = NULL, *t2 = NULL;
    char actions[NACTIONS][NINE] = {"download",
                                    "avail   "};
#define NQUALS  8
    int qual = 5;
    char qualities[NQUALS][NINE] = {"D       ",
                                    "Raw     ",
                                    "Quality ",
                                    "Modified",
                                    "Best    ",
                                    "Merged  ",
                                    "QC      ",
                                    "Unknown ",
    };
    dr = data_req_new();
    sr = station_req_new();
    *nerr = 0;

    while(lcmore(nerr)) {
        if(0) { }
        else if(lclist((char *)actions, NINE, NACTIONS, &action)) { }
        else if(lclist((char *)qualities, NINE, NQUALS, &qual)) {
            data_req_set_quality(dr, qual);
        }
        else if(levent(&ev)) {
            datetime *ts = datetime_copy(ev->time);
            datetime *te = datetime_copy(ev->time);
            data_req_set_time_range(dr, ts, te);
            datetime *ta = datetime_copy(ev->time);
            datetime *tb = datetime_copy(ev->time);
            station_req_set_time_range(sr, ta, tb);
            station_req_set_origin(sr, ev->evlo, ev->evla);
        }
        else if(lkdur("dur#ation$", &d)) { }
        else if(lkra("reg#ion$", -1, 4, 4, reg, &nr)) {
            station_req_set_region(sr, reg[0], reg[1], reg[2], reg[3]);
        }
        else if(lkra("epi#center$", -1, 2, 2, reg, &nr)) {
            station_req_set_origin(sr, reg[0], reg[1]);
        }
        else if(lkra("rad#ius$", -1, 2, 2, reg, &nr)) {
            station_req_set_radius(sr, reg[0], reg[1]);
        }
        else if(lktp("T#IME$", &t1, &t2)) {
            data_req_set_time_range(dr, t1, t2);
        }
        else if(lkchar2("STA#TION$",  sta, sizeof(sta))) { }
        else if(lkchar2("NET#WORK$",  net, sizeof(net))) { }
        else if(lkchar2("CHA#NNEL$",  cha, sizeof(cha))) { }
        else if(lkchar2("LOC#ATION$", loc, sizeof(loc))) { }
        else if(lkchar2("IN$", infile, sizeof(infile))) { }
        else if(lkchar2("OUT$", outfile, sizeof(outfile))) { }
        else {
            cfmt("ILLEGAL OPTION:", 17);
            cresp();
        }
    }
    data_req_set_nslc(dr, net, sta, loc, cha);
    if(! data_req_is_ok(dr, strlen(infile) == 0)) {
        printf("Time range, net, sta, and cha must be set\n");
        *nerr = 3264;
    }
    if(d) {
        data_req_use_duration(dr, d);
    }
    if(*nerr != SAC_OK) {
        goto error;
    }
    result *r = NULL;

    // Availability Request
    if(strlen(infile) > 0 ) {
        char *req = NULL;
        char *url = "https://service.iris.edu/irisws/availability/1/extent?nodata=404";
        if(!(req = data_req_from_station_file(dr, infile))) {
            goto error;
        }
        r = request_full(url, req);
        FREE(req);
    } else {
        char *url = data_req_to_url(dr, action);
        r = result_new();
        FREE(url);
    }

    if(result_is_ok(r)) {
        result_write_to_file(r, "request.txt");
    } else {
        printf("%s", result_error_msg(r));
        goto error;
    }


    // Data Request
    char *url = "https://service.iris.edu/fdsnws/dataselect/1/query?format=miniseed&nodata=404";
    result *r2 = request_full(url, result_data(r));
    if(result_is_ok(r2)) {
        char *file = NULL;
        if(strlen(outfile) > 0) {
            asprintf(&file, "%s.mseed", outfile);
        }
        result_write_to_file(r2, file);
        FREE(file);
    } else {
        printf("%s", result_error_msg(r2));
        goto error;
    }
    {
        char *url = "http://service.iris.edu/fdsnws/station/1/query";
        string data;
        string_init(&data);
        string_printf_append(&data, "format=xml\n");
        string_printf_append(&data, "level=channel\n");
        string_printf_append(&data, "%s", result_data(r));
        result *r2 = request_full(url, data.str);
        FREE(data.str);
        if(result_is_ok(r2)) {
            FILE *fp;
            char tmp[32];
            char *file = NULL;
            if(strlen(outfile) > 0) {
                asprintf(&file, "%s.xml", outfile);
            } else {
                file = result_filename(r2);
                if(!file || strlen(file) <= 0) {
                    file = strdup("station.xml");
                }
            }

            printf("Writing data to %s [%s]\n", file, data_size(result_len(r2),tmp,sizeof(tmp)));
            if(!(fp = fopen(file, "w"))) {
                printf("Error opening file for writing: %s\n", file);
                goto error;
            }
            if(fwrite(result_data(r2), result_len(r2), 1, fp) != 1) {
                printf("Error writing data to file: %s\n", file);
                goto error;
            }
            fclose(fp);
        } else {
            printf("%s", result_error_msg(r2));
            goto error;
        }
    }

 error:
    return;
}

void
sacpz_request(int *nerr) {
    char sta[16], net[16], loc[16], cha[16], time[64];
    char keys[2][9] = {"sacpz\0\0\0",
                       "resp\0\0\0\0"};
    int kind = 1;
    sacpz *pz = sacpz_new();
    char *url = NULL;
    memset(sta, 0, sizeof(sta));
    memset(cha, 0, sizeof(cha));
    memset(net, 0, sizeof(net));
    memset(loc, 0, sizeof(loc));
    memset(time, 0, sizeof(time));
    int n = 0;
    *nerr = SAC_OK;
    while( lcmore(nerr) ) {
        if(lclist((char *)keys, 9, 2, &kind)) {
        }
        else if(lkchar("STA#TION$", 10, sizeof(sta), sta, sizeof(sta), &n)) { }
        else if(lkchar("NET#WORK$", 10, sizeof(net), net, sizeof(net), &n)) { }
        else if(lkchar("CHA#NNEL$", 10, sizeof(cha), cha, sizeof(cha), &n)) { }
        else if(lkchar("LOC#ATION$", 11, sizeof(loc), loc, sizeof(loc), &n)) { }
        else if(lkchar("TIME$", 6, sizeof(time), time, sizeof(time), &n)) {
            sacpz_set_time(pz, datetime_get(time, nerr) );
            if(*nerr) { goto ERROR; }
        }
        else if(lkchar("START#TIME$", -1, sizeof(time), time, sizeof(time), &n)) {
            sacpz_set_start(pz, datetime_get(time, nerr) );
            if(*nerr) { goto ERROR; }
        }
        else if(lkchar("END#TIME$", -1, sizeof(time), time, sizeof(time), &n)) {
            sacpz_set_end(pz, datetime_get(time, nerr) );
            if(*nerr) { goto ERROR; }
        }
        else {
            cfmt("ILLEGAL OPTION:", 17);
            cresp();
        }
    }
    if(*nerr != SAC_OK) {
        goto ERROR;
    }
    sacpz_set_nslc(pz, net, sta, loc, cha);
    if(sacpz_nslc_is_ok(pz)) {
        url = sacpz_to_url(pz, keys[kind-1]);
        printf("URL: '%s'\n", url);
        result *r = request(url);
        if(result_is_ok(r)) {
            printf("FILE: %s\n", sacpz_filename(pz, keys[kind-1]));
            printf("%s", result_data(r));
            //if(r.data[r.n-1] != '\n') {
            //    printf("\n");
            //}
        } else {
            printf("%s", result_error_msg(r));
        }
    } else if(saclen() > 0) {
        sac *s;
        for( int i = 0 ; i < saclen(); i++) {
            /* -- Get the next file in DFL, moving header to CMHDR. */
            if (!(s = sacget(i, TRUE, nerr))) {
                goto ERROR;
            }
            if(strcmp(s->h->knetwk, SAC_CHAR_UNDEFINED) != 0 &&
               strcmp(s->h->kstnm, SAC_CHAR_UNDEFINED) != 0 &&
               strcmp(s->h->kcmpnm, SAC_CHAR_UNDEFINED) != 0) {
                char p[16];
                if(strcmp(s->h->khole, SAC_CHAR_UNDEFINED) == 0) {
                    strlcpy(p, "--", sizeof(p));
                } else {
                    strlcpy(p, s->h->khole, sizeof(p));
                }
                sacpz_set_nslc(pz, s->h->knetwk, s->h->kstnm, p, s->h->kcmpnm);
                if(s->h->nzyear != SAC_INT_UNDEFINED &&
                   s->h->nzjday != SAC_INT_UNDEFINED &&
                   s->h->nzhour != SAC_INT_UNDEFINED &&
                   s->h->nzmin  != SAC_INT_UNDEFINED &&
                   s->h->nzsec  != SAC_INT_UNDEFINED &&
                   s->h->nzmsec != SAC_INT_UNDEFINED) {
                    datetime *t = datetime_new();
                    datetime_set_year(t,s->h->nzyear);
                    datetime_set_doy(t,s->h->nzjday);
                    datetime_add_hms(t,s->h->nzhour, s->h->nzmin, s->h->nzsec);
                    datetime_set_nanosecond(t, s->h->nzmsec * 1e6);
                    datetime_normalize(t);
                    sacpz_set_start(pz, t);
                    sacpz_set_start(pz, datetime_copy(t));
                }
                url = sacpz_to_url(pz, keys[kind-1]);
                printf("URL: '%s'\n", url);
                result *r = request(url);
                if(result_is_ok(r)) {
                    printf("FILE: %s\n", sacpz_filename(pz, keys[kind-1]));
                    printf("%s", result_data(r));
                    break;
                } else {
                    printf("%s", result_error_msg(r));
                }
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
        goto ERROR;
    }
 ERROR:
    if(url) {
        free(url);
    }

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
    data_request(nerr);
    goto L_8888;
 L_3700:
    station_request(nerr);
    goto L_8888;

  L_8888:
    return;
}
