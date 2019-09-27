
#include <stdlib.h>
#include <math.h>

#include <sacio/sacio.h>
#include <sacio/timespec.h>

#include <libmseed/libmseed.h>

#include "miniseed_sac.h"
#include "event.h"
#include "array.h"
#include "cprint.h"
#include "datareq.h"
#include "station.h"

#include "xml.h"
#include "defs.h"
#include "slurp.h"
#include "strip.h"

/**
 * @defgroup miniseed miniseed
 * @brief  Miniseed and Sac Conversions
 */

/**
 * @brief      Read a miniseed file
 *
 * @ingroup    miniseed
 *
 * @details    Read a miniseed file into a Miniseed Trace List
 *
 * @param      mst3k    Miniseed Trace List to read file into
 * @param      file     File to read in
 *
 * @return     1 on success, 0 on failure
 */
int
read_miniseed_file(MS3TraceList *mst3k, char *file) {
    int8_t verbose = 0;
    uint32_t flags     = MSF_SKIPNOTDATA | MSF_UNPACKDATA | MSF_VALIDATECRC ;
    MS3Tolerance tolerance;
    tolerance.time     = NULL; // time_tolerance_func;
    tolerance.samprate = NULL; // samprate_tolerance_func;
    MS3Selections *selections = NULL;
    int8_t split_version = 0;
    int retcode = ms3_readtracelist_selection (&mst3k, file, &tolerance, selections,
                                               split_version, flags, verbose);
    if(retcode != MS_NOERROR) {
        printf("Error reading in %s: %s\n", file, ms_errorstr(retcode));
        return 0;
    }
    return 1;
}

/**
 * @brief      Read miniseed data from memory
 *
 * @details    Read miniseed data from memory into a Miniseed Trace List
 *
 * @ingroup    miniseed
 *
 * @param      mst3k    Miniseed Trace List to read into
 * @param      buffer   buffer containing miniseed data
 * @param      len      length of buffer
 *
 * @return     1 on success, 0 on failure
 */
int64_t
read_miniseed_memory(MS3TraceList *mst3k, char *buffer, uint64_t len) {
    int8_t verbose = 0;
    uint32_t flags     = MSF_SKIPNOTDATA | MSF_UNPACKDATA | MSF_VALIDATECRC ;
    MS3Tolerance tolerance;
    tolerance.time     = NULL; // time_tolerance_func;
    tolerance.samprate = NULL; // samprate_tolerance_func;
    int8_t split_version = 0;
    int64_t retcode = mstl3_readbuffer(&mst3k, buffer, len,
                                   split_version, flags, &tolerance,
                                   verbose);
    if(retcode < 0) {
        printf("Error reading from memory[%" PRId64 "]: %s\n", retcode, ms_errorstr((int)retcode));
    }
    return retcode;
}

/**
 * @brief      Convert a Miniseed Trace List to a set of sac files
 *
 * @details    Convert a Miniseed Trace List into a set of sac files.
 *             Everything possible is pulled from the miniseed data file
 *             including
 *                 - Station Network, Name, Component, and Location
 *                 - Data start and end times
 *                 - Filename:  Net.Sta.Loc.Cha.Qual.Year.Day.HMS.sac
 *
 * @ingroup    miniseed
 *
 * @param      mst3k   Miniseed Trace List 
 *
 * @return     arary of pointers to sac files enclosed in an \ref xarray
 */
sac **
miniseed_trace_list_to_sac(MS3TraceList *mst3k) {
    int8_t verbose = 0;
    int8_t gaps = 1;
    sac **out = NULL;
    if(mst3k->numtraces == 0) {
        return NULL;
    }
    // Show the result of reading in all the files

    mstl3_printtracelist (mst3k, ISOMONTHDAY , verbose, gaps);
    //clrmsg();
    // Convert the TraceList to SAC Files 
    //int n = 0;
    out = xarray_new('p');
    MS3TraceID *t = mst3k->traces;
    for(uint32_t i = 0; i < mst3k->numtraces; i++) {
        MS3TraceSeg *seg = t->first;
        while(seg) {
            if(seg->samprate != 0.0 && seg->numsamples > 0) {
                char qual[6] = " RDQM";
                sac *s;
                uint16_t year, doy;
                uint8_t hour, min, sec;
                uint32_t nsec;
                year = doy = 0;
                hour = min = sec = 0;
                nsec = 0;

                s = sac_new();
                sac_set_float(s, SAC_DELTA, 1.0 / seg->samprate);
                s->h->npts = (int) seg->numsamples;
                s->h->leven = TRUE;
                s->h->iftype = ITIME;

                ms_sid2nslc(t->sid, s->h->knetwk, s->h->kstnm, s->h->khole, s->h->kcmpnm);

                ms_nstime2time(seg->starttime, &year, &doy, &hour, &min, &sec, &nsec);
                s->h->nzyear = year;
                s->h->nzjday = doy;
                s->h->nzhour = hour;
                s->h->nzmin  = min;
                s->h->nzsec  = sec;
                s->h->nzmsec = nsec / 1000000;

                nstime_t dt = seg->starttime - ms_time2nstime(s->h->nzyear, s->h->nzjday,
                                                              s->h->nzhour, s->h->nzmin,
                                                              s->h->nzsec,
                                                              (uint32_t) s->h->nzmsec * 1000000);
                sac_set_float(s, SAC_B, (double) dt / (double)NSTMODULUS);

                fern_asprintf(&s->m->filename,
                         "%s.%s.%s.%s.%c.%04d.%03d.%02d%02d%02d.sac",
                         s->h->knetwk, s->h->kstnm, s->h->khole, s->h->kcmpnm,
                         qual[t->pubversion], s->h->nzyear, s->h->nzjday,
                         s->h->nzhour, s->h->nzmin, s->h->nzsec);

                // Data
                s->y = calloc((size_t) s->h->npts, sizeof(float));
                switch(seg->sampletype) {
                case 'f': memcpy(s->y, seg->datasamples, sizeof(float) * (size_t) s->h->npts); break;
                case 'd': {
                    double *data = (double *) seg->datasamples;
                    for(int j = 0; j < seg->numsamples; j++) {
                        s->y[j] = (float) data[j];
                    }
                }
                    break;
                case 'i': {
                    int *data = (int *) seg->datasamples;
                    for(int j = 0; j < seg->numsamples; j++) {
                        s->y[j] = (float) data[j];
                    }
                }
                    break;
                default:
                    cprintf("red,bold", " WARNING: Unknown sample type: %c\n", seg->sampletype);
                    break;
                }
                sac_extrema(s);
                sac_be(s);
                //sacput(s);
                out = xarray_append(out, s);
                //n++;
            }
            seg = seg->next;
        }
        t = t->next;
    }
    return out;
}


