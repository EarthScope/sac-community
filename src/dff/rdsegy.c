/** 
 * @file   rdsegy.c
 * 
 * @brief  Read a SEG-Y file
 * 
 */

#include <string.h>
#include <math.h>

#include "proto.h"
#include "dff.h"
#include "bool.h"
#include "amf.h"
#include "hdr.h"
#include "msg.h"
#include "clf.h"
#include "dfm.h"
#include "ncpf.h"
#include "segy.h"
#include "SacHeader.h"
#include "ucf.h"
#include "errors.h"
#include "debug.h"

#include <fstr.h>

DFM_EXTERN

void timecheck_short(short *year, short *day, short *hour, short *min,
                     short *sec, short *ms);
int
  DoTime(short *year, short *day, short *hour, short *min, short *sec,
         short *ms, short *ay, short *ad, short *ah, short *am, short *as,
         short *ams, float *reference, float *alternate, char *kfile, sac * s);

struct field_doc SegyHeaderDoc[] = {
    {"jobid", SEGY_TYPE_INT},
    {"line", SEGY_TYPE_INT},
    {"reel", SEGY_TYPE_INT},
    {"data_traces", SEGY_TYPE_SHORT},
    {"aux_traces", SEGY_TYPE_SHORT},
    {"samplerate_milli", SEGY_TYPE_SHORT},
    {"samplerate_micro", SEGY_TYPE_SHORT},
    {"nsamples", SEGY_TYPE_SHORT},
    {"nsamples_field", SEGY_TYPE_SHORT},
    {"format", SEGY_TYPE_SHORT},
    {"ensemble_fold", SEGY_TYPE_SHORT},
    {"trace_sorting", SEGY_TYPE_SHORT},
    {"vertical_sum", SEGY_TYPE_SHORT},
    {"sweep_freq_start", SEGY_TYPE_SHORT},
    {"sweep_freq_end", SEGY_TYPE_SHORT},
    {"sweep_lentth", SEGY_TYPE_SHORT},
    {"sweep_type", SEGY_TYPE_SHORT},
    {"sweep_traces", SEGY_TYPE_SHORT},
    {"sweep_taper_start", SEGY_TYPE_SHORT},
    {"sweep_taper_end", SEGY_TYPE_SHORT},
    {"taper_type", SEGY_TYPE_SHORT},
    {"correlated", SEGY_TYPE_SHORT},
    {"binary_gain_recovered", SEGY_TYPE_SHORT},
    {"amplitude_recovery_method", SEGY_TYPE_SHORT},
    {"units", SEGY_TYPE_SHORT},
    {"polarity", SEGY_TYPE_SHORT},
    {"polarity_code", SEGY_TYPE_SHORT},
    {"empty", -240},
    {"segy_format_revision_number", SEGY_TYPE_SHORT},
    {"fixed_trace_length", SEGY_TYPE_SHORT},
    {"extended_text_headers", SEGY_TYPE_SHORT},
    {"empty", -94},
    {NULL, SEGY_TYPE_END}
};

struct field_doc SegyDoc[] = {
    {"lineSeq", SEGY_TYPE_INT},
    {"reelSeq", SEGY_TYPE_INT},
    {"event_number", SEGY_TYPE_INT},
    {"channe_number", SEGY_TYPE_INT},
    {"energySourcePt", SEGY_TYPE_INT},
    {"cdpEns", SEGY_TYPE_INT},
    {"traceInEnsemble", SEGY_TYPE_INT},
    {"traceID", SEGY_TYPE_SHORT},
    {"vertSum", SEGY_TYPE_SHORT},
    {"horSum", SEGY_TYPE_SHORT},
    {"dataUse", SEGY_TYPE_SHORT},       /*  34 X */
    {"sourceToRecDist", SEGY_TYPE_INT}, /*  36 X */
    {"recElevation", SEGY_TYPE_INT},    /*  40 X */
    {"sourceSurfaceElevation", SEGY_TYPE_INT},  /*  44 X */
    {"sourceDepth", SEGY_TYPE_INT},     /*  48 X */
    {"datumElevRec", SEGY_TYPE_INT},    /*  52 X */
    {"datumElevSource", SEGY_TYPE_INT}, /*  56 X */
    {"sourceWaterDepth", SEGY_TYPE_INT},        /*  60 X */
    {"recWaterDepth", SEGY_TYPE_INT},   /*  64 X */
    {"elevationScale", SEGY_TYPE_SHORT},        /*  68 Elevation Scaler: scale = 1 */
    {"coordScale", SEGY_TYPE_SHORT},    /*  70 Coordinate Scaler: scale = 1 */
    {"sourceLongOrX", SEGY_TYPE_INT},   /*  72 X */
    {"sourceLatOrY", SEGY_TYPE_INT},    /*  76 X */
    {"recLongOrX", SEGY_TYPE_INT},      /*  80 X */
    {"recLatOrY", SEGY_TYPE_INT},       /*  84 X */
    {"coordUnits", SEGY_TYPE_SHORT},    /*  88 Coordinate Units:  = 2 (Lat/Long) */
    {"weatheringVelocity", SEGY_TYPE_SHORT},    /*  90 X */
    {"subWeatheringVelocity", SEGY_TYPE_SHORT}, /*  92 X */
    {"sourceUpholeTime", SEGY_TYPE_SHORT},      /*  94 X */
    {"recUpholeTime", SEGY_TYPE_SHORT}, /*  96 X */
    {"sourceStaticCor", SEGY_TYPE_SHORT},       /*  98 X */
    {"recStaticCor", SEGY_TYPE_SHORT},  /* 100 X */
    {"totalStatic", SEGY_TYPE_SHORT},   /* 102 Total Static in MILLISECS added to Trace Start Time (lower 2 bytes) */
    {"lagTimeA", SEGY_TYPE_SHORT},      /* 104 X */
    {"lagTimeB", SEGY_TYPE_SHORT},      /* 106 X */
    {"delay", SEGY_TYPE_SHORT}, /* 108 X */
    {"muteStart", SEGY_TYPE_SHORT},     /* 110 X */
    {"muteEnd", SEGY_TYPE_SHORT},       /* 112 X */
    {"sampleLength", SEGY_TYPE_SHORT},  /* 114 Number of samples in this trace (unless == 32767) */
    {"deltaSample", SEGY_TYPE_SHORT},   /* 116 Sampling interval in MICROSECONDS (unless == 1) */
    {"gainType", SEGY_TYPE_SHORT},      /* 118 Gain Type: 1 = Fixed Gain */
    {"gainConst", SEGY_TYPE_SHORT},     /* 120 Gain of amplifier */
    {"initialGain", SEGY_TYPE_SHORT},   /* 122 X */
    {"correlated", SEGY_TYPE_SHORT},    /* 124 X */
    {"sweepStart", SEGY_TYPE_SHORT},    /* 126 X */
    {"sweepEnd", SEGY_TYPE_SHORT},      /* 128 X */
    {"sweepLength", SEGY_TYPE_SHORT},   /* 130 X */
    {"sweepType", SEGY_TYPE_SHORT},     /* 132 X */
    {"sweepTaperAtStart", SEGY_TYPE_SHORT},     /* 134 X */
    {"sweepTaperAtEnd", SEGY_TYPE_SHORT},       /* 136 X */
    {"taperType", SEGY_TYPE_SHORT},     /* 138 X */
    {"aliasFreq", SEGY_TYPE_SHORT},     /* 140 X */
    {"aliasSlope", SEGY_TYPE_SHORT},    /* 142 X */
    {"notchFreq", SEGY_TYPE_SHORT},     /* 144 X */
    {"notchSlope", SEGY_TYPE_SHORT},    /* 146 X */
    {"lowCutFreq", SEGY_TYPE_SHORT},    /* 148 X */
    {"hiCutFreq", SEGY_TYPE_SHORT},     /* 150 X */
    {"lowCutSlope", SEGY_TYPE_SHORT},   /* 152 X */
    {"hiCutSlope", SEGY_TYPE_SHORT},    /* 154 X */
    {"year", SEGY_TYPE_SHORT},  /* 156 year of Start of trace */
    {"day", SEGY_TYPE_SHORT},   /* 158 day of year at Start of trace */
    {"hour", SEGY_TYPE_SHORT},  /* 160 hour of day at Start of trace */
    {"minute", SEGY_TYPE_SHORT},        /* 162 minute of hour at Start of trace */
    {"second", SEGY_TYPE_SHORT},        /* 164 second of minute at Start of trace */
    {"timeBasisCode", SEGY_TYPE_SHORT}, /* 166 Time basis code: 2 = GMT */
    {"traceWeightingFactor", SEGY_TYPE_SHORT},  /* 168 X */
    {"phoneRollPos1", SEGY_TYPE_SHORT}, /* 170 X */
    {"phoneFirstTrace", SEGY_TYPE_SHORT},       /* 172 X */
    {"phoneLastTrace", SEGY_TYPE_SHORT},        /* 174 X */
    {"gapSize", SEGY_TYPE_SHORT},       /* 176 X */
    {"taperOvertravel", SEGY_TYPE_SHORT},       /* 178 X */
    {"station_name[6]", SEGY_TYPE_CHAR6},       /* 180 Station Name code (5 chars + \0) */
    {"sensor_serial[8]", SEGY_TYPE_CHAR8},      /* 186 Sensor Serial code (7 chars + \0) */
    {"channel_name[4]", SEGY_TYPE_CHAR4},       /* 194 Channel Name code (3 chars + \0) */
    {"totalStaticHi", SEGY_TYPE_SHORT}, /* 198 Total Static in MILLISECS added to Trace Start Time (high 2 bytes) */
    {"samp_rate", SEGY_TYPE_INT},       /* 200 Sample interval in MICROSECS as a 32 bit integer */
    {"data_form", SEGY_TYPE_SHORT},     /* 204 Data Format flag: 0=16 bit, 1=32 bit integer */
    {"m_secs", SEGY_TYPE_SHORT},        /* 206 MILLISECONDS of seconds of Start of trace */
    {"trigyear", SEGY_TYPE_SHORT},      /* 208 year of Trigger time */
    {"trigday", SEGY_TYPE_SHORT},       /* 210 day of year at Trigger time */
    {"trighour", SEGY_TYPE_SHORT},      /* 212 hour of day at Trigger time */
    {"trigminute", SEGY_TYPE_SHORT},    /* 214 minute of hour at Trigger time */
    {"trigsecond", SEGY_TYPE_SHORT},    /* 216 second of minute at Trigger time */
    {"trigmills", SEGY_TYPE_SHORT},     /* 218 MILLISECONDS of seconds of Trigger time */
    {"scale_fac", SEGY_TYPE_FLOAT},     /* 220 Scale Factor (IEEE 32 bit float) */
    {"inst_no", SEGY_TYPE_SHORT},       /* 224 Instrument Serial Number */
    {"not_to_be_used", SEGY_TYPE_SHORT},        /* 226 X */
    {"num_samps", SEGY_TYPE_INT},       /* 228 Number of Samples as a 32 bit integer (when sampleLength == 32767) */
    {"max", SEGY_TYPE_INT},     /* 232 Maximum value in Counts */
    {"min", SEGY_TYPE_INT},     /* 236 Minimum value in Counts */
    {NULL, SEGY_TYPE_END}
};

/*********************************************************************/
/* convlat(&xlat,&xlon)                                 */
/*********************************************************************/
/** 
 * Subroutine to convert x-y coordinates to lat long assume a spherical earth
 * simple model xlat is y coordinate on input returned as latitude xlon is x
 * coordinate on input returned as longitude
 *
 * @param xlat
 *    Latitude, positive to north
 * @param xlon
 *    Longitude, positive to east
 *
 */

void
convlat(float *xlat, float *xlon) {

    double d, D, L, mpd, tpi;
    int iflg;

    iflg = 1;
    if (*xlon < 0.)
        iflg = -1;

    tpi = 3.1415927;
    mpd = (6080.2 * 60 * .9144) / (3);  /* meters per degree lat */
    d = (double) ((*xlat * *xlat) + (*xlon * *xlon));
    d = sqrt(d);
    D = d / mpd;                /* total distance */

    *xlat = *xlat / mpd;        /* get latitude */

    d = cos(D * tpi / 180.);
    L = cos(*xlat * tpi / 180.);

    D = acos(d / L);            /* get longitude */
    D = (double) iflg *D;       /* set direction */
    *xlon = (float) (D * 180. / tpi);
    return;
}

void
swap_array(char *a, struct field_doc *doc) {
    int i;
    char *p;
    p = a;
    i = 0;

    while (doc[i].length != 0) {
        if (doc[i].length > 0) {
            byteswap(p, doc[i].length);
            p += doc[i].length;
        } else {
            p += -doc[i].length;
        }
        i++;
    }
}

void
swap_array_v(char *a, int n, int size) {
    int i;
    char *p;
    p = a;
    for (i = 0; i < n; i++) {
        byteswap(p, size);
        p += size;
    }
}

int
segy_read_file_header(FILE * fp, struct SegyFileHeader *h) {
    char p[3];
    if (fread(&p, 3, 1, fp) != 1) {
        return FALSE;
    }
    memset(h, 0, SEGY_BINARY_FILE_HEADER_SIZE);
    /* Check for 3600 (3200 + 400) File Header */
    if ((*p == '@' && *(p + 1) == '@' && *(p + 2) == '@') ||
        (*p == -1 && *(p + 1) == -1 && *(p + 2) == -1) || (*p == 'C' &&
                                                           *(p + 1) == ' ' &&
                                                           *(p + 2) == '1')) {
        /* Skip Textual Header */
        fseek(fp, SEGY_TEXT_FILE_HEADER_SIZE, SEEK_SET);
        if (fread(h, SEGY_BINARY_FILE_HEADER_SIZE, 1, fp) != 1) {
            return FALSE;
        }
    } else {
        fseek(fp, 0, SEEK_SET);
    }
    return TRUE;
}

int
segy_read_trace_header(FILE * fp, struct SegyHead *trace) {
    if (fread(trace, SEGY_TRACE_HEADER_SIZE, 1, fp) != 1) {
        return FALSE;
    }
    return TRUE;
}

#define MSK_MANT_IBM 0xffffff
#define MSK_EXP_IBM  0x7f000000
#define MSK_SIGN_IBM 0x80000000
#define MSK_NORM_IEEE 0x800000
#define MSK_NO_SIGN  0x7fffffff

int
ibm2ieee(int *fpn, int nval) {
    register int exponent, mantissa, i, sign;

    for (i = 0; i < nval; i++) {
        if ((*fpn & MSK_MANT_IBM) == 0)
            *fpn = 0;
        else {
            mantissa = (*fpn & MSK_MANT_IBM);
            exponent = ((((*fpn & MSK_EXP_IBM) >> 24) - 64) << 2);
            sign = (*fpn & MSK_SIGN_IBM);
            while ((mantissa & MSK_NORM_IEEE) == 0) {
                mantissa <<= 1; /* normalize */
                exponent--;
            }
            mantissa = mantissa & 0x7fffff;     /* shift understood one out */
            exponent += 126;
            if ((exponent < 0) || (exponent > 255)) {
                printf("IBM floating point exponent out of range %d \n",
                       exponent);
                exponent = 0;
            }
            exponent <<= 23;
            *fpn = sign | exponent | mantissa;
        }
        fpn++;
    }
    return 0;
}

void
rdsegy(int idfl, char *kfile, int *nlen, int *ndx1, int *ndx2, int *nerr) {

    unsigned char *cptr;
    short short_hold;
    int num_sam, sam_rate, int_hold, idx;
    int llon, llat, check;
    float xs, xlat, xlon, elat, elon;
    double useScale;
    sac *s;
    int swap;
    FILE *fpin = NULL;
    struct SegyHead trace;
    struct SegyFileHeader fileheader;
    UNUSED(nlen);
    UNUSED(ndx1);
    UNUSED(ndx2);

    if (!(fpin = fopen(kfile, "rb"))) {
        *nerr = ERROR_OPENING_FILE;
        setmsg("ERROR", *nerr);
        apcmsg(kfile, strlen(kfile));
        outmsg();
        return;
    }
    if (!(s = sacget(idfl - 1, TRUE, nerr))) {
        return;
    }
    /* start reading the data file   */
    /* first read the SEGY trace header    */

    swap = !(CheckByteOrder() == ENDIAN_BIG);

    memset(&trace, 0, SEGY_TRACE_HEADER_SIZE);
    if (!segy_read_file_header(fpin, &fileheader) ||
        !segy_read_trace_header(fpin, &trace)) {
        *nerr = ERROR_READING_FILE;
        error(*nerr, "%s", kfile);
        outmsg();
        fclose(fpin);
        return;
    }

    if (swap) {
        swap_array((char *) &fileheader, SegyHeaderDoc);
        swap_array((char *) &trace, SegyDoc);
    }

    if (trace.sampleLength == SEGY_TRACE_MAX_NPTS || trace.sampleLength == 0) {
        printf
            ("segy: No File Header Present, assuming PASSCAL style segy file\n");
        num_sam = trace.num_samps == 0 ? -12345 : trace.num_samps;
    } else {
        num_sam = trace.sampleLength;
    }

    if (fileheader.format == 0) {
        /* Convert PASCALL segy version to SEGY fileheader format */
        if (trace.data_form == 0) {
            fileheader.format = SEGY_DATA_2BYTE_INT;
        } else if (trace.data_form == 1) {
            fileheader.format = SEGY_DATA_4BYTE_INT;
        }
    }
    //printf("segy: npts: %d\n", num_sam);
    //printf("segy: format: %d\n", fileheader.format);

    /* Check number of samples */
    if (num_sam < 0) {
        fclose(fpin);
        *nerr = ERROR_DATA_POINTS_OUTSIDE_OF_RANGE;
        setmsg("ERROR", *nerr);
        apcmsg(kfile, strlen(kfile));
        outmsg();
        return;
    }

    s->m->filename = fstrdup(kfile, strlen(kfile));
    if (*nerr) {
        fclose(fpin);
        *nerr = ERROR_NO_FILENAME_SPECIFIED;
        setmsg("ERROR", *nerr);
        outmsg();
        return;
    }

    s->h->iftype = ITIME;
    s->h->nvhdr = SAC_HEADER_MAJOR_VERSION;
    s->h->npts = num_sam;

    s->y = (float *) malloc(sizeof(float) * s->h->npts);
    if (!s->y) {
        fclose(fpin);
        return;
    }

    s->h->dist = trace.sourceToRecDist == 0 ? -12345. : trace.sourceToRecDist;

    if (trace.deltaSample == 1 || trace.deltaSample == 0) {
        sam_rate = trace.samp_rate;
    } else {
        sam_rate = trace.deltaSample;
    }
    s->h->delta = (float) sam_rate / 1000000.0;

    /* get scale factor for data  */
    if (trace.scale_fac == 0) {
        s->h->scale = 1.0;
    } else
        s->h->scale =
            trace.scale_fac / (trace.gainConst == 0 ? 1.0 : trace.gainConst);

    if (cmdfm.lscale) {
        useScale = s->h->scale;
        s->h->scale = 1.0;
    } else {
        useScale = 1.0;
    }

    s->h->depmax = trace.max * useScale;
    s->h->depmin = trace.min * useScale;

    /* now go get the trace data */
    //printf("segy: scale: %e\n", useScale);
    if (fileheader.format == SEGY_DATA_4BYTE_IBM_FLOAT) {       /* 32 bit IBM float data */
        float f;
        for (idx = 0; idx < num_sam; idx++) {
            fread(&f, 4, 1, fpin);
            if (swap) {
                byteswap(&f, 4);
            }
            ibm2ieee((int *) &f, 1);
            s->y[idx] = ((float) f) * useScale;
        }
    } else if (fileheader.format == SEGY_DATA_4BYTE_INT) {      /* 32 bit integer data */
        cptr = (unsigned char *) &int_hold;
        for (idx = 0; idx < num_sam; idx++) {
            fread(cptr, 4, 1, fpin);
            if (swap) {
                byteswap(&int_hold, 4);
            }
            s->y[idx] = ((float) int_hold) * useScale;
        }
    } else if (fileheader.format == SEGY_DATA_2BYTE_INT) {      /* 16 bit integer data */
        cptr = (unsigned char *) &short_hold;
        for (idx = 0; idx < num_sam; idx++) {
            fread(cptr, 2, 1, fpin);
            if (swap) {
                byteswap(&short_hold, 2);
            }
            s->y[idx] = ((float) short_hold) * useScale;
        }
    } else if (fileheader.format == SEGY_DATA_4BYTE_IEEE_FLOAT) {
        fread(s->y, 4, num_sam, fpin);
        swap_array_v((char *) s->y, num_sam, 4);
        for (idx = 0; idx < num_sam; idx++) {
            s->y[idx] = s->y[idx] * useScale;
        }
    } else {
        *nerr = ERROR_DATA_POINTS_OUTSIDE_OF_RANGE;
        error(*nerr, "%s - %s [%d]", kfile, "Unknown data format",
              trace.data_form);
        outmsg();
        return;
    }
    fclose(fpin);
    sac_extrema(s);

    /* Fill SAC Header fields */

    /* time */
    if (cmdfm.iztype == IO) {
        check =
            DoTime(&trace.trigyear, &trace.trigday, &trace.trighour,
                   &trace.trigminute, &trace.trigsecond, &trace.trigmills,
                   &trace.year, &trace.day, &trace.hour, &trace.minute,
                   &trace.second, &trace.m_secs, &s->h->o, &s->h->b, kfile, s);
        if (!check) {
            check =
                DoTime(&trace.year, &trace.day, &trace.hour, &trace.minute,
                       &trace.second, &trace.m_secs, &trace.trigyear,
                       &trace.trigday, &trace.trighour, &trace.trigminute,
                       &trace.trigsecond, &trace.trigmills, &s->h->b, &s->h->o,
                       kfile, s);
        }
    } else {
        check =
            DoTime(&trace.year, &trace.day, &trace.hour, &trace.minute,
                   &trace.second, &trace.m_secs, &trace.trigyear,
                   &trace.trigday, &trace.trighour, &trace.trigminute,
                   &trace.trigsecond, &trace.trigmills, &s->h->b, &s->h->o,
                   kfile, s);
        if (!check) {
            check =
                DoTime(&trace.trigyear, &trace.trigday, &trace.trighour,
                       &trace.trigminute, &trace.trigsecond, &trace.trigmills,
                       &trace.year, &trace.day, &trace.hour, &trace.minute,
                       &trace.second, &trace.m_secs, &s->h->o, &s->h->b, kfile,
                       s);

        }
    }

    if (!check) {
        *nerr = ERROR_IN_TIME_FIELD;
        setmsg("ERROR", *nerr);
        apcmsg(" Cannot determine BEGIN or ORIGIN time for ", 44);
        apcmsg(kfile, strlen(kfile) + 1);
        apcmsg("\nSkipping file.", 16);
        outmsg();
        clrmsg();

        fclose(fpin);
        FREE(s->y);
        return;
    }

    sac_be(s);

    sprintf(s->h->kcmpnm, "      %d", trace.channel_number);

    /* strings */
    strncpy(s->h->kstnm, trace.station_name, 6);
    if (trace.channel_name[0] != '\0' && strncmp(trace.channel_name, "    ", 4))
        sprintf(s->h->kcmpnm, "%-4.4s    ", trace.channel_name);
    if (trace.event_number)
        sprintf(s->h->kevnm, "%d", trace.event_number);

    /* now some coordinate information   */
    xs = (float) trace.coordScale;      /* scale factor  */
    if (xs < 0)
        xs = -1 / xs;
    llon = trace.recLongOrX;
    xlon = xs * (float) llon;
    llat = trace.recLatOrY;
    xlat = xs * (float) llat;
    elon = xs * (float) trace.sourceLongOrX;
    elat = xs * (float) trace.sourceLatOrY;
    if (trace.coordUnits == 1) {
        convlat(&xlat, &xlon);
        convlat(&elat, &elon);
    } else {
        xlat = xlat / 3600;
        xlon = xlon / 3600;
        elat = elat / 3600;
        elon = elon / 3600;
    }
    s->h->stla = (float) xlat;
    s->h->stlo = (float) xlon;
    s->h->stel = (trace.recElevation == 0 ? -12345. : trace.recElevation);
    s->h->evla = elat;
    s->h->evlo = elon;
    s->h->evel =
        trace.sourceSurfaceElevation ==
        0 ? -12345. : trace.sourceSurfaceElevation;
    s->h->evdp = trace.sourceDepth == 0 ? -12345. : trace.sourceDepth;

    s->h->leven = TRUE;
    s->h->lpspol = TRUE;
    s->h->lovrok = TRUE;
    s->h->lcalda = (s->h->evla == -12345. || s->h->evlo == -12345. ||
                    s->h->stla == -12345. ||
                    s->h->stlo == -12345.) ? FALSE : TRUE;

}

int
DoTime(short *year, short *day, short *hour, short *min, short *sec, short *ms,
       short *ay, short *ad, short *ah, short *am, short *as, short *ams,
       float *reference, float *alternate, char *kfile, sac * s) {

    if (year == 0 || day == 0)
        return FALSE;

    s->h->iztype = cmdfm.iztype;
    *reference = 0.0;

    timecheck_short(year, day, hour, min, sec, ms);
    s->h->nzyear = (int) *year; /* the year */
    s->h->nzjday = (int) *day;
    s->h->nzhour = (int) *hour;
    s->h->nzmin = (int) *min;
    s->h->nzsec = (int) *sec;
    s->h->nzmsec = (int) *ms;

    if (*ay != 0 && *ad != 0) {
        *alternate =
            ((float) (*ams - s->h->nzmsec)) / 1000.0 +
            ((float) (*as - s->h->nzsec)) +
            ((float) (*am - s->h->nzmin)) * 60.0 +
            ((float) (*ah - s->h->nzhour)) * 3600.0 +
            ((float) (*ad - s->h->nzjday)) * 3600.0 * 24.0 +
            ((float) (*ay - s->h->nzyear)) * 3600.0 * 24.0 *
            (isleap(*as < s->h->nzyear ? *ay : s->h->nzyear) ? 366. : 365.);

/*      if( *iztype == IO )
         *alternate = -(*alternate) ; */
    } else {
        setmsg("WARNING", 907);

        if (s->h->iztype == IB) {
            apcmsg(" Cannot determine origin time for ", 35);
            apcmsg(kfile, strlen(kfile));
            apcmsg("\nSetting ORIGIN to BEGIN.", 26);
        } else {
            apcmsg(" Cannot determine begin time for ", 34);
            apcmsg(kfile, strlen(kfile));
            apcmsg("\nSetting BEGIN to ORIGIN.", 26);
        }
        outmsg();
        clrmsg();

        *alternate = 0.0;
    }

    return TRUE;
}
