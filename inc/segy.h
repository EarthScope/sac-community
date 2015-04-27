#ifndef SEGY_H
#define SEGY_H

enum {
    SEGY_DATA_4BYTE_IBM_FLOAT = 1,
    SEGY_DATA_4BYTE_INT = 2,
    SEGY_DATA_2BYTE_INT = 3,
    SEGY_DATA_4BYTE_IEEE_FLOAT = 5,
};

struct SegyFileHeader {
    int jobid;
    int line;
    int reel;
    short data_traces;
    short aux_traces;
    short samplerate_milli;
    short samplerate_micro;
    short nsamples;
    short nsamples_field;
    short format;
    short ensemble_fold;
    short trace_sorting;
    short vertical_sum;
    short sweep_freq_start;
    short sweep_freq_end;
    short sweep_length;
    short sweep_type;
    short sweep_traces;
    short sweep_taper_start;
    short sweep_taper_end;
    short taper_type;
    short correlated;
    short binary_gain_recovered;
    short amplitude_recovery_method;
    short units;
    short polarity;
    short polarity_code;
    char unassigned[240];
    short segy_format_revision_number;
    short fixed_trace_length;
    short extended_text_headers;
    char unassigned2[94];
};

#define SEGY_TEXT_FILE_HEADER_SIZE   3200
#define SEGY_BINARY_FILE_HEADER_SIZE  400
#define SEGY_TRACE_HEADER_SIZE        240
#define SEGY_TRACE_MAX_NPTS         32767

/* This is the header for the PASSCAL SEGY trace data.  
 *
 * The PASSCAL SEGY trace format is a modified form of the SEG-Y
 * trace format.  The modification comes is because we use some of
 * the unspecified header words to store information pertinent to 
 * the PASSCAL data.  The data values for each trace are preceded
 * by a 240 byte header. This format is given below.  All integer 
 * values are stored with the most significant byte first.  Data
 * values are either 16 0r 32 bit integers depending on byte 206
 * of the header, the field named "data_form".
 * 
 * SEGYHEAD is now typedef'ed 
 *
 * Reading bytes  directly into this header will allow access to
 * all of the fields.  The number in the comment is the byte offset
 * into the segy file.  An "X" in the comment indicates that field
 * is NEVER set.  Fields that are set to default values contain that
 * value and follow a ":" in the comment ("grep : segy.h" will spit
 * all default fields out).  Two pairs of fields exist to cover an
 * inherited limitation; sampleLength/num_samps and deltaSample/samp_rate.
 * When the value is too large to fit in a short, sampleLength or
 * deltaSample become flags and require their long counterparts,
 * num_samps and samp_rate, to contain that value.
 */
typedef struct SegyHead {       /*  Offset Description  */
    int lineSeq;                /*   0 Sequence numbers within line */
    int reelSeq;                /*   4 Sequence numbers within reel */
    int event_number;           /*   8 Original field record number or trigger number */
    int channel_number;         /*  12 Trace channel number within the original field record */
    int energySourcePt;         /*  16 X */
    int cdpEns;                 /*  20 X */
    int traceInEnsemble;        /*  24 X */
    short traceID;              /*  28 Trace identification code: seismic data = 1 */
    short vertSum;              /*  30 X */
    short horSum;               /*  32 X */
    short dataUse;              /*  34 X */
    int sourceToRecDist;        /*  36 X */
    int recElevation;           /*  40 X */
    int sourceSurfaceElevation; /*  44 X */
    int sourceDepth;            /*  48 X */
    int datumElevRec;           /*  52 X */
    int datumElevSource;        /*  56 X */
    int sourceWaterDepth;       /*  60 X */
    int recWaterDepth;          /*  64 X */
    short elevationScale;       /*  68 Elevation Scaler: scale = 1 */
    short coordScale;           /*  70 Coordinate Scaler: scale = 1 */
    int sourceLongOrX;          /*  72 X */
    int sourceLatOrY;           /*  76 X */
    int recLongOrX;             /*  80 X */
    int recLatOrY;              /*  84 X */
    short coordUnits;           /*  88 Coordinate Units:  = 2 (Lat/Long) */
    short weatheringVelocity;   /*  90 X */
    short subWeatheringVelocity;        /*  92 X */
    short sourceUpholeTime;     /*  94 X */
    short recUpholeTime;        /*  96 X */
    short sourceStaticCor;      /*  98 X */
    short recStaticCor;         /* 100 X */
    short totalStatic;          /* 102 Total Static in MILLISECS added to Trace Start Time (lower 2 bytes) */
    short lagTimeA;             /* 104 X */
    short lagTimeB;             /* 106 X */
    short delay;                /* 108 X */
    short muteStart;            /* 110 X */
    short muteEnd;              /* 112 X */
    short sampleLength;         /* 114 Number of samples in this trace (unless == 32767) */
    short deltaSample;          /* 116 Sampling interval in MICROSECONDS (unless == 1) */
    short gainType;             /* 118 Gain Type: 1 = Fixed Gain */
    short gainConst;            /* 120 Gain of amplifier */
    short initialGain;          /* 122 X */
    short correlated;           /* 124 X */
    short sweepStart;           /* 126 X */
    short sweepEnd;             /* 128 X */
    short sweepLength;          /* 130 X */
    short sweepType;            /* 132 X */
    short sweepTaperAtStart;    /* 134 X */
    short sweepTaperAtEnd;      /* 136 X */
    short taperType;            /* 138 X */
    short aliasFreq;            /* 140 X */
    short aliasSlope;           /* 142 X */
    short notchFreq;            /* 144 X */
    short notchSlope;           /* 146 X */
    short lowCutFreq;           /* 148 X */
    short hiCutFreq;            /* 150 X */
    short lowCutSlope;          /* 152 X */
    short hiCutSlope;           /* 154 X */
    short year;                 /* 156 year of Start of trace */
    short day;                  /* 158 day of year at Start of trace */
    short hour;                 /* 160 hour of day at Start of trace */
    short minute;               /* 162 minute of hour at Start of trace */
    short second;               /* 164 second of minute at Start of trace */
    short timeBasisCode;        /* 166 Time basis code: 2 = GMT */
    short traceWeightingFactor; /* 168 X */
    short phoneRollPos1;        /* 170 X */
    short phoneFirstTrace;      /* 172 X */
    short phoneLastTrace;       /* 174 X */
    short gapSize;              /* 176 X */
    short taperOvertravel;      /* 178 X */
    char station_name[6];       /* 180 Station Name code (5 chars + \0) */
    char sensor_serial[8];      /* 186 Sensor Serial code (7 chars + \0) */
    char channel_name[4];       /* 194 Channel Name code (3 chars + \0) */
    short totalStaticHi;        /* 198 Total Static in MILLISECS added to Trace Start Time (high 2 bytes) */
    int samp_rate;              /* 200 Sample interval in MICROSECS as a 32 bit integer */
    short data_form;            /* 204 Data Format flag: 0=16 bit, 1=32 bit integer */
    short m_secs;               /* 206 MILLISECONDS of seconds of Start of trace */
    short trigyear;             /* 208 year of Trigger time */
    short trigday;              /* 210 day of year at Trigger time */
    short trighour;             /* 212 hour of day at Trigger time */
    short trigminute;           /* 214 minute of hour at Trigger time */
    short trigsecond;           /* 216 second of minute at Trigger time */
    short trigmills;            /* 218 MILLISECONDS of seconds of Trigger time */
    float scale_fac;            /* 220 Scale Factor (IEEE 32 bit float) */
    short inst_no;              /* 224 Instrument Serial Number */
    short not_to_be_used;       /* 226 X */
    int num_samps;              /* 228 Number of Samples as a 32 bit integer
                                 * (when sampleLength == 32767) */
    int max;                    /* 232 Maximum value in Counts */
    int min;                    /* 236 Minimum value in Counts */
} SEGYHEAD;                     /* end of segy trace header */

struct field_doc {
    char *name;
    int length;
};

#define  SEGY_TYPE_END    0
#define  SEGY_TYPE_SHORT  2
#define  SEGY_TYPE_INT    4
#define  SEGY_TYPE_FLOAT  4
#define  SEGY_TYPE_CHAR4 -4
#define  SEGY_TYPE_CHAR6 -6
#define  SEGY_TYPE_CHAR8 -8

#endif /* SEGY_H        */
