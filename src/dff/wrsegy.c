/** 
 * @file   wrsegy.c
 * 
 * @brief  Write a SEG-Y file
 * 
 */
#include "config.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "unistdx.h"

#include <limits.h>

#include "proto.h"
#include "amf.h"
#include "dff.h"
#include "segy.h"
#include "hdr.h"
#include "bool.h"
#include "dfm.h"
#include "msg.h"
#include "amf.h"

#include "SacHeader.h"

#include "errors.h"
#include "debug.h"

#include "ucf.h"

#ifdef WIN32
#define SEGY_CREATE_MODE (O_CREAT | O_TRUNC | O_WRONLY | O_BINARY)
#define SEGY_PERMISSION_MODE _S_IREAD | _S_IWRITE
#else
#define SEGY_CREATE_MODE (O_CREAT | O_TRUNC | O_WRONLY )
#define SEGY_PERMISSION_MODE 0666
#endif
void timecheck_short(short *year, short *day, short *hour, short *min,
                     short *sec, short *ms);
void swap_array(char *a, struct field_doc *doc);
void swap_array_v(char *a, int n, int size);
extern struct field_doc SegyDoc[];

/** 
 * Write a SEG-Y file from memory to disk
 * 
 * @param idfl 
 *    Data file list index number
 * @param filename 
 *    File name to write
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_CREATING_FILE
 *    - ERROR_WRITING_FILE
 */
void
wrsegy(int idfl, char *filename, int *nerr) {

    double data_roof, data_max;

    double scale, value;
    int swap;
    int *idata, idx, bytesOdata, segyFile = -1, check;
    sac *s;
    /* initialize segy header to zeros */
    SEGYHEAD outHdr = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, "\0\0\0\0\0\0", "\0\0\0\0\0\0\0\0", "\0\0\0\0",
        0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0.0, 0, 0, 0, 0, 0
    };

    swap = !(CheckByteOrder() == ENDIAN_BIG);

    if (!(s = sacget(idfl - 1, TRUE, nerr))) {
        return;
    }
    /* Check that the data is TIME SERIES data and is evenly spaced. */
    if (s->h->iftype != ITIME || s->h->leven != TRUE) {
        setmsg("WARNING", 1306);
        apcmsg("or spectral file.  File: ", 26);
        apcmsg(filename, strlen(filename));
        apcmsg(" filenumber: ", 14);
        apimsg(idfl);
        outmsg();
        clrmsg();
    }

    /* do kevnm only if it completely numeric, and has less than 10 digits. */
    if (isdigit(s->h->kevnm[0]) ||
        (s->h->kevnm[0] == '-' && isdigit(s->h->kevnm[1]))) {
        char temp[18];
        strcpy(temp, s->h->kevnm);
        for (idx = strlen(temp) - 1; idx >= 0; idx--) {
            if (!isspace(temp[idx])) {
                temp[idx + 1] = '\0';
                break;
            }
        }
        if (strlen(temp) <= 9) {
            int lint = TRUE;

            for (idx = 1; idx < 9; idx++) {
                if (!isdigit(temp[idx])) {
                    lint = FALSE;
                    break;
                }
            }

            if (lint)
                outHdr.event_number = atoi(s->h->kevnm);
        }
    }

    outHdr.traceID = 1;         /* there is only one trace per file */
    outHdr.sourceToRecDist = s->h->dist == -12345. ? 0 : s->h->dist + 0.5;
    outHdr.recElevation =
        s->h->stel == -12345. ? 0 : s->h->stel + (s->h->stel < 0 ? -.5 : .5);
    outHdr.sourceSurfaceElevation =
        s->h->evel == -12345. ? 0 : s->h->evel + (s->h->evel < 0 ? -.5 : .5);
    outHdr.sourceDepth = s->h->evdp == -12345. ? 0 : s->h->evdp + 0.5;
    outHdr.elevationScale = 1;
    outHdr.coordScale = 1;
    outHdr.sourceLongOrX =
        s->h->evlo == -12345. ? 0 : (s->h->evlo * 3600) + (s->h->evlo <
                                                           0 ? -.5 : .5);
    outHdr.sourceLatOrY =
        s->h->evla == -12345. ? 0 : (s->h->evla * 3600) + (s->h->evla <
                                                           0 ? -.5 : .5);
    outHdr.recLongOrX =
        s->h->stlo == -12345. ? 0 : (s->h->stlo * 3600.0) + (s->h->stlo <
                                                             0 ? -.5 : .5);
    outHdr.recLatOrY =
        s->h->stla == -12345. ? 0 : (s->h->stla * 3600.0) + (s->h->stla <
                                                             0 ? -.5 : .5);
    outHdr.coordUnits = 2;
    outHdr.sampleLength =
        s->h->npts == -12345. ? 0 : (s->h->npts >= 32767 ? 32767 : s->h->npts);
    outHdr.num_samps = s->h->npts == -12345. ? 0 : s->h->npts;
    outHdr.deltaSample =
        s->h->delta == -12345. ? 0 : (s->h->delta * 1000000 >=
                                      32767 ? 1 : (s->h->delta * 1000000) +
                                      0.5);
    outHdr.samp_rate =
        s->h->delta == -12345. ? 0 : (s->h->delta * 1000000) + 0.5;

    outHdr.gainType = 1;
    outHdr.gainConst = 1;

    /* Set begin time of the trace */
    outHdr.year = s->h->nzyear == -12345. ? 0 : (short) s->h->nzyear;
    outHdr.day = s->h->nzjday == -12345. ? 0 : (short) s->h->nzjday;
    outHdr.hour = s->h->nzhour == -12345. ? 0 : (short) s->h->nzhour;
    outHdr.minute = s->h->nzmin == -12345. ? 0 : (short) s->h->nzmin;

    if (s->h->b != -12345.) {
        outHdr.second = (int) (s->h->b);
        outHdr.m_secs =
            ((s->h->b - (float) outHdr.second) * 1000.) + (s->h->b >
                                                           0 ? 0.5 : -0.5);
        outHdr.second += s->h->nzsec == -12345. ? 0 : s->h->nzsec;
        outHdr.m_secs += s->h->nzmsec == -12345. ? 0 : s->h->nzmsec;

        timecheck_short(&outHdr.year, &outHdr.day, &outHdr.hour, &outHdr.minute,
                        &outHdr.second, &outHdr.m_secs);
    } /* end if( *b != -12345. ) */
    else {
        outHdr.second = s->h->nzsec == -12345. ? 0 : s->h->nzsec;
        outHdr.m_secs = s->h->nzmsec == -12345. ? 0 : s->h->nzmsec;
    }

    /* Set origin time of the trace */
    outHdr.trigyear = s->h->nzyear == -12345. ? 0 : s->h->nzyear;
    outHdr.trigday = s->h->nzjday == -12345. ? 0 : s->h->nzjday;
    outHdr.trighour = s->h->nzhour == -12345. ? 0 : s->h->nzhour;
    outHdr.trigminute = s->h->nzmin == -12345. ? 0 : s->h->nzmin;
    if (s->h->o != -12345.) {
        outHdr.trigsecond = (int) (s->h->o);
        outHdr.trigmills =
            ((s->h->o - (float) outHdr.trigsecond) * 1000.) + (s->h->o >
                                                               0 ? 0.5 : -0.5);
        outHdr.trigsecond += s->h->nzsec == -12345. ? 0 : s->h->nzsec;
        outHdr.trigmills += s->h->nzmsec == -12345. ? 0 : s->h->nzmsec;

        timecheck_short(&outHdr.trigyear, &outHdr.trigday, &outHdr.trighour,
                        &outHdr.trigminute, &outHdr.trigsecond,
                        &outHdr.trigmills);

    } /* end if( *o != -12345. ) */
    else {
        outHdr.trigsecond = s->h->nzsec == -12345. ? 0 : s->h->nzsec;
        outHdr.trigmills = s->h->nzmsec == -12345. ? 0 : s->h->nzmsec;
    }

    if (strncmp(s->h->kstnm, "-12345", 6))
        strncpy(outHdr.station_name, s->h->kstnm, 6);
    if (strncmp(s->h->kcmpnm, "-12345", 6))
        strncpy(outHdr.channel_name, s->h->kcmpnm, 4);
    outHdr.data_form = 1;       /* 32 bit integer */

    /* Zero out the filename */
    for (idx = 0; idx < MCPFN; idx++)
        if (isspace(filename[idx]) || filename[idx] == '\0')
            break;

    filename[idx] = '\0';

    /* Establish the data buffers */
    bytesOdata = s->h->npts * sizeof(int);
    idata = (int *) malloc(bytesOdata);
    //nlcmem = cmdfm.ndxdta[ idfl - 1 ][ 0 ] ;

    /* Figure out the scaling */
    /* data_roof = 2147483600.0 ; *//* Original Value, did not work */
    /* data_roof = 0x7FFF0000; *//* Float, Single Prevision */
    data_roof = INT_MAX;        /* Float, Double Prevision */
    data_max = 0.0;
    for (idx = 0; idx < s->h->npts; idx++)
        if (fabs(s->y[idx]) > data_max)
            data_max = fabs(s->y[idx]);

    if (data_max != 0.0)
        scale = data_roof / data_max;
    else
        scale = 1.0;

    outHdr.scale_fac = (1.0 / scale);

    /* Scale the data */
    for (idx = 0; idx < s->h->npts; idx++) {
        value = (s->y[idx] * scale);
        value += (value < 0 ? -0.5 : 0.5);
        idata[idx] = (int) (value);
    }

    value = s->h->depmax * scale;
    value += (value < 0 ? -0.5 : 0.5);
    outHdr.max = value;
    value = s->h->depmin * scale;
    value += (value < 0 ? -0.5 : 0.5);
    outHdr.min = value;

    /* Open a new file for segy formated header and trace. */
    segyFile = open(filename, SEGY_CREATE_MODE, SEGY_PERMISSION_MODE);
    if (segyFile == -1) {
        /* error handling */
        *nerr = ERROR_CREATING_FILE;
        setmsg("ERROR", *nerr);
        apcmsg(filename, strlen(filename));
        outmsg();
        free(idata);
        return;
    }

    if (swap) {
        swap_array((char *) &outHdr, SegyDoc);
    }

    /* Write the segy header to the file. */
    check = write(segyFile, (char *) (&outHdr.lineSeq), 240);
    if (check != 240) {
        /* error handling */
        *nerr = ERROR_WRITING_FILE;
        setmsg("ERROR", *nerr);
        apcmsg(filename, strlen(filename));
        apcmsg(": Trouble writing SEGY header", 30);
        outmsg();
        free(idata);
        close(segyFile);
        return;
    }

    if (swap) {
        swap_array_v((char *) idata, s->h->npts, sizeof(int));
    }

    /* Write the segy data to the file. */
    check = write(segyFile, (char *) idata, bytesOdata);
    if (check != bytesOdata) {
        /* error handling */
        *nerr = ERROR_WRITING_FILE;
        setmsg("ERROR", *nerr);
        apcmsg(filename, strlen(filename));
        apcmsg(": Trouble writing SEGY data", 28);
        outmsg();
    }

    free(idata);
    close(segyFile);
}

void
timecheck_short(short *year, short *day, short *hour, short *min, short *sec,
                short *ms) {
    int iyear, iday, ihour, imin, isec, ims;

    iyear = (int) *year;
    iday = (int) *day;
    ihour = (int) *hour;
    imin = (int) *min;
    isec = (int) *sec;
    ims = (int) *ms;

    timecheck(&iyear, &iday, &ihour, &imin, &isec, &ims);

    *year = (short) iyear;
    *day = (short) iday;
    *hour = (short) ihour;
    *min = (short) imin;
    *sec = (short) isec;
    *ms = (short) ims;
}
