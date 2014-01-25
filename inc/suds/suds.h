#ifndef _SUDS_
#define _SUDS_

#ifndef TRUE
#define TRUE           1
#endif
#ifndef FALSE
#define FALSE          0
#endif

/* Define code for type of machine suds is being compiled on. */
/* '6' is 80x86 machine */
#define MACHINE         '6'

#define NODATA          -32767
#define NOTIME          -2147472000 
#define NOCHAR          '_'
#define NOSTRG          ""
#define NOLIST          0


/* Structure types or identifiers */
#define NO_STRUCT       0
#define STAT_IDENT      1
#define STRUCTTAG       2
#define TERMINATOR      3
#define EQUIPMENT       4
#define STATIONCOMP     5
#define MUXDATA         6
#define DESCRIPTRACE    7
#define LOCTRACE        8
#define CALIBRATION     9
#define FEATURE         10
#define RESIDUAL        11
#define EVENT           12
#define EV_DESCRIPT     13
#define ORIGIN          14
#define ERROR           15
#define FOCALMECH       16
#define MOMENT          17
#define VELMODEL        18
#define LAYERS          19
#define COMMENT         20
#define PROFILE         21
#define SHOTGATHER      22
#define CALIB           23
#define COMPLEX         24
#define TRIGGERS        25
#define TRIGSETTING     26
#define EVENTSETTING    27
#define DETECTOR        28
#define ATODINFO        29
#define TIMECORRECTION  30
#define INSTRUMENT      31
#define CHANSET         32

#define TOTAL_STRUCTS   32





typedef struct {
   float   fx;
   float   fy;
} VECTOR;
typedef struct {
   float   cr;
   float   ci;
} COMPLEXX;
typedef struct {
   double  dr;
   double  di;
} D_COMPLEX;
typedef struct {
   float   xx;
   float   yy;
   float   xy;
} TENSOR;


/*  STRUCTURE DEFINITIONS */

/*  SUDS_FORM:  Information on each suds structure used for input from ascii, */
/*              output to ascii, loading or unloading the database, etc. */

typedef struct {
   int num;
   char *meaning;
} SUDS_CODES;

typedef struct {
   int  fstype;               /* Structure types or identifiers */
   char  *fname;               /* Name used to identify structure item */
   int  ftype;                /* Type of variable in field */
   int  flength;              /* Length of variable in the field */
   int  offset;               /* Offset of variable pointer from beginning of */
                               /* structure measured in bytes, where first byte */
                               /* of structure=0. */
   char  *initval;             /* Value to initialize structure */
   char  *fformat;             /* Printf type format to read + write field */
   int  nextfstype;           /* If structure, this is fstype */
} SUDS_FORM;


 /* SUDS_STRUCTTAG:  Structure to identify structures when archived together */

#define ST_MAGIC   'S'        /* magic character for sync in structtag */

typedef struct {
   char sync;                 /* The letter S. If not present, error exists. */
                              /* Use to unscramble damaged files or tapes. */
   char machine;              /* code for machine writing binary file for use */
                              /* in identifying byte order and encoding. */
   short id_struct;           /* structure identifier: numbers defined above */
   int len_struct;           /* structure length in bytes for fast reading */
                              /* and to identify new versions of the structure */
   int len_data;             /* length of data following structure in bytes */
} SUDS_STRUCTTAG;


 /* SUDS_STATIDENT:  Station identification. */

typedef struct {              /* station component identifier */
   char  network[5];          /* network name */
   char  st_name[5];          /* name of station where equipment is located */
   char    component;         /* component v,n,e */
   short  inst_type;          /* instrument type */
} SUDS_STATIDENT;
#define ST_LEN 12
typedef struct ST{
  SUDS_STATIDENT *st; 
  struct ST *next;
}stList;



 /* SUDS_ATODINFO: Information on the A to D converter */

typedef struct {
   short  base_address;       /*  base I/O address of this device */
   short  device_id;          /* device identifier */
   unsigned short  device_flags;      /* device flags */
   short  extended_bufs;      /* number of extended buffers used */
   short  external_mux;       /* AtoD external mux control word */
   char    timing_source;     /* AtoD timing source: i=internal, e=external */
   char    trigger_source;    /*  AtoD trigger source: i=internal, e=external */
} SUDS_ATODINFO;
#define AD_LEN 12
typedef struct AD{
  SUDS_ATODINFO *ad; 
  struct AD *next;
}adList;


 /* SUDS_CALIBRATION: Calibration information for a station component */

#define NOCALPTS 30

typedef struct {
   COMPLEXX   pole;         
   COMPLEXX   zero;         
} SUDS_CALIBR;

typedef struct {
   SUDS_STATIDENT ca_name;    /* station component identification */
   float maxgain;             /* maximum gain of calibration curve */
   float normaliz;            /* factor to multiply standard calib by to make */
                              /* peak at given frequency=1 */
   SUDS_CALIBR cal[NOCALPTS]; /* calibration info */
   int   begint;             /* time this calibration becomes effective */
   int   endt;               /* time this calibration is no longer effective */
} SUDS_CALIBRATION;
#define CA_LEN 508
typedef struct CA{
  SUDS_CALIBRATION *ca;
  struct CA *next;
}caList;


 /* SUDS_COMMENT:  Comment tag to be followed by the bytes of comment */

typedef struct {
   short   refer;            /* structure identifier comment refers to */
   short   item;             /* item in structure comment refers to */
   short   length;           /* number of bytes in comment */
   short   unused;
} SUDS_COMMENT;
#define CO_LEN 8

typedef struct CO{
  SUDS_COMMENT *co;
  char *text; 
  struct CO *next;
}coList;


 /* SUDS_CHANSET:  Associate station/components into sets. */

typedef struct {
   short  type;              /* Set type; 0=single channel(s), 1=orthogonal vector */
   short  entries;           /* Number of entries in set (these follow as data) */
   char  network[4];         /* Network name */
   char  name[5];            /* Set name */
   int active;              /* Set is defined after this time */
   int inactive;            /* Set is not defined after this time */
} SUDS_CHANSET;
#define CS_LEN 21


 /* Entries of this form follow SUDS_CHANSET struct. */
typedef struct _CHANSETENTRY {
   int  inst_num;           /* Instrument serial number */
   short  stream_num;        /* Stream of instrument */
   short  chan_num;          /* Channel of stream */
   SUDS_STATIDENT st;        /* Station/component identifier */
} CHANSETENTRY;
#define CSE_LEN 20

typedef struct CS{
  SUDS_CHANSET *cs;
  CHANSETENTRY *entry; 
  struct CS *next;
}csList;



 /* SUDS_DESCRIPTRACE:  Descriptive information about a seismic trace. */
                    /*  Normally followed by waveform. */

typedef struct {
   SUDS_STATIDENT dt_name;   /* station component identification */
   double begintime;         /* time of first data sample */
   short  localtime;         /* minutes to add to GMT to get local time */
   char    datatype;         /* s = 12 bit unsigned stored as short int, 0 to 4096, */
                             /* q = 12 bit signed stored as short int, -2048 to 2048, */
                             /* u = 16 bit unsigned stored as short int, 0 to 65536 */
                             /* i = 16 bit signed stored as short int, -32767 to 32767, */
                             /* 2 = 24 bit signed integer stored as long, */
                             /* l = 32 bit signed integer stored as long, */
                             /*  r = 12 bit data, 4 lsb time stored as short int, */
                             /* f = float (32 bit IEEE real), */
                             /* d = double (64 bit IEEE real), */
                             /*  c = complex, */
                             /* v = vector, */
                             /* t = tensor */
   char    descriptor;       /* g=good, t=telemetry noise, c=calibration, etc */
   short  digi_by;           /* agency code who digitized record; 0=original */
   short  processed;         /*  processing done on this waveform */
   int  length;             /* number of samples in trace */
   float   rate;             /* samples per second */
   float   mindata;          /* minimum value of data (type s,l,f only) */
   float   maxdata;          /* maximum value of data (type s,l,f only) */
   float   avenoise;         /* average value of first 200 samples (type s,l,f only) */
   int  numclip;            /* number of clipped datapoints */
   double time_correct;      /* time correction to be added to begintime */
   float   rate_correct;     /* rate correction to be added to rate */
} SUDS_DESCRIPTRACE;
#define DT_LEN 64

typedef struct DT{
  SUDS_DESCRIPTRACE *dt;
  short  *i2data; 
  int    *i4data;
  float  *r4data;
  double *r8data; 
  struct DT *next;
}dtList;


 /* SUDS_DETECTOR:  Information on detector program being used */

typedef struct {
   char    dalgorithm;       /* triggering algorithm: x=xdetect, m=mdetect */
                             /* e=eqdetect */
   char    event_type;       /* c=calibration, e=earthquake, E=explosion, */
                             /*  f=free run, n=noise, etc. */
   char    net_node_id[11];  /* network node identification */
   float   versionnum;       /* software version number */
   int  event_number;       /* unique event number assigned locally. */
   int  spareL;             /* spare */
} SUDS_DETECTOR;
#define DE_LEN 24


typedef struct DE{
  SUDS_DETECTOR *de;
  struct DE *next;
}deList;


 /* SUDS_EQUIPMENT:  Equipment making up a station/component. Primarily used for */
                  /* maintenance but may be referenced by researcher. One or more */
                 /*  structures exist for each piece of equipment making up a */
                  /* station/component. */

typedef struct {
   SUDS_STATIDENT thisSID;      /* identifier of this piece of equipment */
   SUDS_STATIDENT previous;  /* next piece of equipment toward sensor */
   SUDS_STATIDENT next;      /*  next piece of equipment toward recorder */
   char  serial[8];          /* serial number */
   short  model;             /* model such as L4, HS10, etc. */
   short  knob1;             /* knob setting or series resistor value of Lpad */
   short  knob2;             /* knob setting or shunt  resistor value of Lpad */
   short  reason;            /* reason change was made */
   float   frequency;        /* sensor corner frequency, vco freq, transmitter */
                             /* frequency, etc. */
   int effective;           /* date/time these values became effective */
} SUDS_EQUIPMENT;
#define EQ_LEN 60

typedef struct EQ{
  SUDS_EQUIPMENT *eq;
  struct EQ *next;
}eqList;


 /* SUDS_ERROR:  Error matrix */

typedef struct {
   float   covarr[10];        /* covariance matrix */
} SUDS_ERROR;
#define ER_LEN 40

typedef struct ER{
  SUDS_ERROR *er;
  struct ER *next;
}erList;


 /* SUDS_EVENT:  General information about an event. */

typedef struct {
   short  authority;        /* organization processing the data */
   int  number;            /* unique event number assigned by organization */
   short  felt;             /* number of felt reports */
   char    mintensity;      /*  maximum Modified Mercali Intensity */
   char    ev_type;         /* e=earthquake, E=explosion, n=nuclear, */
                            /*  i=icequake, b=b_type, n=net, r=regional, */
                            /* t=teleseism, c=calibration, n=noise */
   char    tectonism;       /* observed u=uplift, s=subsidence, S=strikeslip */
                            /* faulting, N=normal faulting, T=thrust */
   char    waterwave;       /* seiche, tsunami, etc. */
   char    mechanism;       /* t=thrust, s=strike-slip, n=normal, e=explosive */
   char    medium;          /* medium containing explosion or event */
   float   size;            /* magnitude or pounds TNT for explosions */
} SUDS_EVENT;
#define EV_LEN 18

typedef struct EV{
  SUDS_EVENT *ev;
  struct EV *next;
}evList;


 /* SUDS_EVENTSETTING:  Settings for earthquake trigger system */

typedef struct {
   char  netwname[4];       /* network name */
   double beginttime;       /* time these values in effect */
   short  const1;           /* trigger constant 1 */
   short  const2;           /* trigger constant 2 */
   short  threshold;        /* trigger threshold */
   short  const3;           /* trigger constant 3 */
   float   minduration;     /* minimum duration for event */
   float   maxduration;     /* maximum duration for event */
   char    algorithm;       /* triggering algorithm: x=xdetect, m=mdetect */
                            /* e=eqdetect */
   char    spareK;          /* spare */
   short  spareI;           /* spare */
} SUDS_EVENTSETTING;
#define ES_LEN 32

typedef struct ES{
  SUDS_EVENTSETTING *es;
  struct ES *next;
}esList;


 /* SUDS_EVDESCR:  Descriptive information about an event typically used for */
                /* major, destructive earthquakes. This structure is typically */
                /* associated with EVENT structure. */

typedef struct {
   char  eqname[20];        /* Popular name used to refer to this earthquake */
   char  country[16];       /* country of earthquake */
   char  state[16];         /* state, province or other political subdivision */
   short  localtime;        /* hours to add to GMT to get local time */
   short  spareB;
} SUDS_EVDESCR;
#define ED_LEN 56

typedef struct ED{
  SUDS_EVDESCR *ed;
  struct ED *next;
}edList;

 /* SUDS_FEATURE:  Observed phase arrival time, amplitude, and period. */

typedef struct {
   SUDS_STATIDENT fe_name;   /* station component identification */
   short  obs_phase;         /* observed phase code */
   char    onset;            /* wave onset descriptor, i or e */
   char    direction;        /* first motion: U,D,+,- */
   short  sig_noise;         /* ratio ampl. of first peak or trough to noise */
   char    data_source;      /* i=interactive,a=automatic,r=rtp, or user code */
   char    tim_qual;         /* timing quality given by analyst: 0-4, etc. */
                             /* n=ignore timing */
   char    amp_qual;         /* amplitude quality given by analyst: 0-4, etc. */
                             /* n=ignor amplitude information */
   char    ampunits;         /* units amplitude measured in: d=digital counts */
                             /* m=mm on develocorder, etc. */
   short  gain_range;        /* 1 or gain multiplier if gain range in effect */
   double time;              /* phase time, x value where pick was made */
   float   amplitude;        /* peak-to-peak amplitude of phase */
   float   period;           /* period of waveform measured */
   int time_of_pick;        /* time this pick was made */
   short  pick_authority;    /* organization processing the data */
   short  pick_reader;       /* person making this pick */
} SUDS_FEATURE;
#define FE_LEN 48

typedef struct FE{
  SUDS_FEATURE *fe;
  struct FE *next;
}feList;


 /* SUDS_FOCALMECH:  General information about a focal mechanism. */

typedef struct {
   float   astrike;           /* strike of plane a */
   float   adip;              /* dip of plane a */
   float   arake;             /* rake of plane a */
   float   bstrike;           /* strike of plane b */
   float   bdip;              /* dip of plane b */
   float   brake;             /* rake of plane b */
   char    prefplane;         /* preferred plane a or b or blank */
   char    spareC[3];
} SUDS_FOCALMECH;
#define FO_LEN 28

typedef struct FO{
  SUDS_FOCALMECH *fo;
  struct FO *next;
}foList;

 /* SUDS_INSTRUMENT: Instrument hardware settings, mainly PADS related */
                  /* added by R. Banfill, Jan 1991 */

typedef struct {
   SUDS_STATIDENT in_name;    /* Station component identification */
   short  in_serial;          /* Instrument serial number */
   short  comps;              /* Number of components recorded by instrument */
   short  channel;            /* Actual channel number on recorder */
   char    sens_type;         /* Sensor type; a=accel, v=vel, d=disp... */
   char    datatype;          /* see SUDS_DESCRIPTRACE.datatype */
   int  void_samp;           /* Invalid or void sample value */
   float   dig_con;           /* Digitizing constant (counts / volt) */
   float   aa_corner;         /* Anti-alias filter corner frequency (Hz) */
   float   aa_poles;          /* Anti-alias filter poles */
   float   nat_freq;          /* Transducer natural frequency (Hz ) */
   float   damping;           /* Transducer damping coeff. */
   float   mot_con;           /* Transducer motion constant (volts / GMU) */
   float   gain;              /*  Amplifier gain (dB) */
   float   local_x;           /* Local coordinate X (meters) */
   float   local_y;           /* Local coordinate Y (meters) */
   float   local_z;           /* Local coordinate Z (meters) */
   int effective;            /* Time these setting took effect */
   float   pre_event;         /*  Pre-event length (IST+pre_event=trigger time) */
   short  trig_num;           /* Trigger number on instrument */
   char  study[6];            /* Study name, used to insure unique station names */
   short  sn_serial;          /* Sensor serial number */
} SUDS_INSTRUMENT;
#define IN_LEN 82

typedef struct IN{
  SUDS_INSTRUMENT *in;
  struct IN *next;
}inList;


 /* SUDS_LAYERS:  Velocity layers. */

typedef struct {
   float   thickness;         /* thickness in kilometers */
   float   pveltop;           /* p velocity at top of layer */
   float   pvelbase;          /* p velocity at base of layer */
   float   sveltop;           /* s velocity at top of layer */
   float   svelbase;          /* s velocity at base of layer */
   short  function;           /* velocity function in layer: 0=constant, */
                              /* 1=linear, 2=exponential, etc. */
   short  spareF;
} SUDS_LAYERS;
#define LA_LEN 24

typedef struct LA{
  SUDS_LAYERS *la;
  struct LA *next;
}laList;


 /* SUDS_LOCTRACE:  Location of trace. */

typedef struct {
   SUDS_STATIDENT lt_name;    /* station component identification */
   char  *fileloc;            /* pointer to pathname in file system */
   char  *tapeloc;            /* pointer to name of tape or offline storage */
   int  beginloc;            /* bytes from begining of file to trace */
} SUDS_LOCTRACE;
#define LO_LEN 24

typedef struct LO{
  SUDS_LOCTRACE *lo;
  struct LO *next;
}loList;


 /* SUDS_MOMENT:  Moment tensor information. */

typedef struct {
   unsigned char   datatypes; /* sum of: 1=polarities, 2=amplitudes, */
                              /* 4=waveforms, etc. */
   char    constraints;       /* solution constrained: d=deviatoric, */
                              /* c=double couple */
   char    spareD[2];
   float   sc_moment;         /* scalar moment */
   float   norm_ten[6];       /* normalized moment tensor */
} SUDS_MOMENT;
#define MO_LEN 32

typedef struct MO{
  SUDS_MOMENT *mo;
  struct MO *next;
}moList;



 /* SUDS_MUXDATA:  Header for multiplexed data */

typedef struct {
   char  netname[4];         /* network name */
   double begintime;         /* time of first data sample */
   short  loctime;           /* minutes to add to GMT to get local time */
   short  numchans;          /* number of channels: if !=1 then multiplexed */
   float   dig_rate;         /* samples per second */
   char    typedata;         /* see SUDS_DESCRIPTRACE.datatype */
   char    descript;         /* g=good, t=telemetry noise, c=calibration, etc */
   short  spareG;            /* spare */
   int  numsamps;           /* number of sample sweeps. Typically not known */
                             /* when header is written, but can be added later */
   int  blocksize;          /* number of demultiplexed samples per channel if */
                             /* data is partially demultiplexed, otherwise=0 */
} SUDS_MUXDATA;
#define MU_LEN 32

typedef struct MU{
  SUDS_MUXDATA *mu;
  short  *i2data; 
  int    *i4data;
  float  *r4data;
  double *r8data; 
  struct MU *next;
}muList;



/*  SUDS_ORIGIN: Information about a specific solution for a given event */

typedef struct {
   int  number;             /* unique event number assigned by organization */
   short  authority;         /* organization processing the data */
   char    version;          /* version of solution within organization */
   char    or_status;        /* processing status: f=final, a=automatic, etc */
   char    preferred;        /* p=preferred location */
   char    program;          /* name of processing program  h=hypo71, */
                             /* l=hypolayer, i=isc, c=centroid, etc. */
   char    depcontrl;        /* depth control: f=fixed, etc. */
   char    convergence;      /* hypocentral convergence character */
   int  region;             /* geographic region code assigned locally */
   double orgtime;           /* origin time */
   double  or_lat;           /* latitude, north is plus */
   double  or_long;          /* longitude, east is plus */
   float   depth;            /* depth in kilometers, + down */
   float   err_horiz;        /* horizontal error in km */
   float   err_depth;        /* vertical error in km */
   float   res_rms;          /* rms of residuals */
   char  crustmodel[6];      /* code for model used in this location */
   short  gap;               /* azimuthal gap in degrees */
   float   nearstat;         /* distance in km to nearest station */
   short  num_stats;         /* number of stations reporting phases */
   short  rep_p;             /* number of p phases reported */
   short  used_p;            /* number of p times used in the solution */
   short  rep_s;             /* number of s phases reported */
   short  used_s;            /* number of s times used in the solution */
   short  mag_type;          /* magnitude type: coda,tau,xmag ml,mb,ms,mw */
   short  rep_m;             /* number of magnitude readings reported */
   short  used_m;            /* number of magnitude readings used */
   float   magnitude;        /* magnitude value */
   float   weight;           /* average magnitude weight */
   float   mag_rms;          /* rms of magnitude */
   int effective;           /* time this solution was calculated */
} SUDS_ORIGIN;
#define OR_LEN 100


typedef struct OR{
  SUDS_ORIGIN *orig;
  struct OR *next;
}orList;



 /* SUDS_PROFILE:  Grouping of shotgathers by profile. */

typedef struct {
   short junk1;
    /* What is your suggestion? */
} SUDS_PROFILE;
#define PR_LEN 2

typedef struct PR{
  SUDS_PROFILE *pr;
  struct PR *next;
}prList;


 /* SUDS_RESIDUAL:  Calculated residuals for arrival times, magnitudes, etc. */

typedef struct {
   int  event_num;           /* unique event number */
   SUDS_STATIDENT re_name;    /* station component identification */
   short  set_phase;          /* phase code set for this solution */
   char    set_tim_qual;      /* timing quality assigned for this soln: 0-4 */
   char    set_amp_qual;      /* amplitude quality assigned for this soln: 0-4 */
   float   residual;          /* traveltime residual or phase magnitude */
   float   weight_used;       /* weight used in this solution */
   float   delay;             /* delay time or station correction used */
   float   azimuth;           /* azimuth event to station, 0 north */
   float   distance;          /* distance in km event to station */
   float   emergence;         /* angle of emergence from source, 0=down,180=up */
} SUDS_RESIDUAL;
#define RE_LEN 44

typedef struct RE{
  SUDS_RESIDUAL *re;
  struct RE *next;
}reList;



 /* SUDS_SHOTGATHER:  Grouping of waveforms by source event */

typedef struct {
   short junk2;
    /* What is your suggestion? */
} SUDS_SHOTGATHER;
#define SH_LEN 2

typedef struct SH{
  SUDS_SHOTGATHER *sh;
  struct SH *next;
}shList;



 /* SUDS_STATIONCOMP:  Generic station component information */

typedef struct {
   SUDS_STATIDENT sc_name;    /* station component identification */
   short  azim;               /* component azimuth clockwise from north */
   short  incid;              /* component angle of incidence from vertical */
                              /*  0 is vertical, 90 is horizontal */
   double  st_lat;            /* latitude, north is plus */
   double  st_long;           /* longitude, east is plus */
   float   elev;              /* elevation in meters */
   char    enclosure;         /*  d=dam, n=nuclear power plant, v=underground */
                              /* vault, b=buried, s=on surface, etc. */
   char    annotation;        /* annotated comment code */
   char    recorder;          /* type device data recorded on */
   char    rockclass;         /* i=igneous, m=metamorphic, s=sedimentary */
   short  rocktype;           /* code for type of rock */
   char    sitecondition;     /* p=permafrost, etc. */
   char    sensor_type;       /* sensor type: d=displacement, v=velocity, */
                              /* a=acceleration, t=time code */
   char    data_type;         /* see SUDS_DESCRIPTRACE.datatype */
   char    data_units;        /* data units: d=digital counts, v=millivolts, */
                              /* n=nanometers (/sec or /sec/sec) */
   char    polarity;          /* n=normal, r=reversed */
   char    st_status;         /* d=dead, g=good */
   float   max_gain;          /* maximum gain of the amplifier */
   float   clip_value;        /* +-value of data where clipping begins */
   float   con_mvolts;        /* conversion factor to millivolts: mv per counts */
                              /* 0 means not defined or not appropriate */
                              /* max_ground_motion=digital_sample*con_mvolts* */
                              /* max_gain */
   short  channel;            /* a2d channel number */
   short  atod_gain;          /* gain of analog to digital converter */
   int effective;            /* date/time these values became effective */
   float   clock_correct;     /*  clock correction in seconds. */
   float   station_delay;     /*  seismological station delay. */
} SUDS_STATIONCOMP;
#define SC_LEN 76

typedef struct SC{
  SUDS_STATIONCOMP *sc;
  struct SC *next;
}scList;



 /* SUDS_TERMINATOR:  Structure to end a sequence of related structures when */
                   /* loaded in a serial file or on a serial device. */

typedef struct {
   short  structid;          /* id for structure at beginning of this sequence */
   short  spareA;
} SUDS_TERMINATOR;
#define TE_LEN 4

typedef struct TE{
  SUDS_TERMINATOR *te;
  struct TE *next;
}teList;


 /* SUDS_TIMECORRECTION:  Time correction information. */

typedef struct {
   SUDS_STATIDENT tm_name;    /* time trace station id used to determine */
                              /* correction. */
   double time_correct;       /* time correction to be added to begintime */
   float   rate_correct;      /* rate correction to be added to rate */
   char    sync_code;         /* synchronization code as follows: */
                              /* 0 = total failure,   1 = 1 second synch, */
                              /* 2 = 10 second synch, 3 = minute synch, */
                              /* 4, 5 = successful decode. */
   char    program;           /* program used to decode time: */
                              /* e = irige, c = irigc */
   int effective_time;       /* time this correction was calculated */
   short  spareM;
} SUDS_TIMECORRECTION;
#define TC_LEN 32

typedef struct TC{
  SUDS_TIMECORRECTION *tc;
  struct TC *next;
}tcList;


 /* SUDS_TRIGGERS:  Earthquake detector trigger statistics */

typedef struct {
   SUDS_STATIDENT tr_name;    /* station component identification */
   short  sta;                /* short term average */
   short  lta;                /* long term average; pre_lta for xdetect */
   short  abs_sta;            /* short term absolute average */
   short  abs_lta;            /* long term absolute average */
   short  trig_value;         /* value of trigger level (eta) */
   short  num_triggers;       /* number of times triggered during this event */
   double trig_time;          /* time of first trigger */
} SUDS_TRIGGERS;
#define TR_LEN 32

typedef struct TR{
  SUDS_TRIGGERS *tr;
  struct TR *next;
}trList;


 /* SUDS_TRIGSETTING:  Settings for earthquake trigger system */

typedef struct {
   char  netwname[5];        /* network name */
   double beginttime;        /* time these values in effect */
   short  const1;            /* trigger constant 1 */
   short  const2;            /* trigger constant 2 */
   short  threshold;         /* trigger threshold */
   short  const3;            /* trigger constant 3 */
   short  const4;            /* trigger constant 4 */
   short  wav_inc;           /* weighted average increment */
   float   sweep;            /* trigger sweep time in seconds */
   float   aperture;         /* seconds for coincident station triggers */
   char    algorithm;        /* triggering algorithm: x=xdetect, m=mdetect */
                             /*  e=eqdetect */
   char    spareJ;           /* spare */
   short  spareI;            /*  spare */
} SUDS_TRIGSETTING;
#define TS_LEN 36

typedef struct TS{
  SUDS_TRIGSETTING *ts; 
  struct TS *next;
}tsList;



 /* SUDS_VELMODEL:  Velocity model */

typedef struct {
   char  netname[4];         /* network name */
   char  modelname[6];       /* model name */
   char    spareE;
   char    modeltype;        /* p=profile A to B, a=area within corners A B */
   double  latA;             /* latitude of point A, north is plus */
   double  longA;            /* longitude of point A, east is plus */
   double  latB;             /* latitude of point B, north is plus */
   double  longB;            /* longitude of point B, east is plus */
   int time_effective;      /* time this model was created */
} SUDS_VELMODEL;
#define VM_LEN 48

typedef struct VM{
  SUDS_VELMODEL *vm; 
  struct VM *next;
}vmList;


typedef struct _SUDS {
   stList *stHead;
   stList *stTail;
   adList *adHead;
   adList *adTail;
   caList *caHead;
   caList *caTail;
   coList *coHead;
   coList *coTail;
   csList *csHead;
   csList *csTail;
   dtList *dtHead;
   dtList *dtTail;
   deList *deHead;
   deList *deTail;
   eqList *eqHead;
   eqList *eqTail;
   erList *erHead;
   erList *erTail;
   evList *evHead;
   evList *evTail;
   esList *esHead;
   esList *esTail;
   edList *edHead;
   edList *edTail;
   feList *feHead;
   feList *feTail;
   foList *foHead;
   foList *foTail;
   inList *inHead;
   inList *inTail;
   laList *laHead;
   laList *laTail;
   loList *loHead;
   loList *loTail;
   moList *moHead;
   moList *moTail;
   muList *muHead;
   muList *muTail;
   orList *orHead;
   orList *orTail;
   prList *prHead;
   prList *prTail;
   reList *reHead;
   reList *reTail;
   shList *shHead;
   shList *shTail;
   scList *scHead;
   scList *scTail;
   teList *teHead;
   teList *teTail;
   tcList *tcHead;
   tcList *tcTail;
   trList *trHead;
   trList *trTail;
   tsList *tsHead;
   tsList *tsTail;
   vmList *vmHead;
   vmList *vmTail;
} SUDS;


#ifdef NULL_SUDS
static SUDS_STATIDENT NullST    = { NOSTRG, NOSTRG, NOCHAR, NODATA};
static SUDS_ATODINFO  NullAD    = {NODATA, NODATA, 0, NODATA, NODATA, NOCHAR, NOCHAR};
static SUDS_CALIBRATION  NullCA = {{ NOSTRG, NOSTRG, NOCHAR, NODATA}, NODATA, NODATA, {
                                     {{0.0,0.0}, {0.0, 0.0}}, {{0.0,0.0}, {0.0, 0.0}}, 
                                     {{0.0,0.0}, {0.0, 0.0}}, {{0.0,0.0}, {0.0, 0.0}}, 
                                     {{0.0,0.0}, {0.0, 0.0}}, {{0.0,0.0}, {0.0, 0.0}}, 
                                     {{0.0,0.0}, {0.0, 0.0}}, {{0.0,0.0}, {0.0, 0.0}}, 
                                     {{0.0,0.0}, {0.0, 0.0}}, {{0.0,0.0}, {0.0, 0.0}},
                                     {{0.0,0.0}, {0.0, 0.0}}, {{0.0,0.0}, {0.0, 0.0}}, 
                                     {{0.0,0.0}, {0.0, 0.0}}, {{0.0,0.0}, {0.0, 0.0}}, 
                                     {{0.0,0.0}, {0.0, 0.0}}, {{0.0,0.0}, {0.0, 0.0}}, 
                                     {{0.0,0.0}, {0.0, 0.0}}, {{0.0,0.0}, {0.0, 0.0}}, 
                                     {{0.0,0.0}, {0.0, 0.0}}, {{0.0,0.0}, {0.0, 0.0}},
                                     {{0.0,0.0}, {0.0, 0.0}}, {{0.0,0.0}, {0.0, 0.0}}, 
                                     {{0.0,0.0}, {0.0, 0.0}}, {{0.0,0.0}, {0.0, 0.0}}, 
                                     {{0.0,0.0}, {0.0, 0.0}}, {{0.0,0.0}, {0.0, 0.0}}, 
                                     {{0.0,0.0}, {0.0, 0.0}}, {{0.0,0.0}, {0.0, 0.0}}, 
                                     {{0.0,0.0}, {0.0, 0.0}}, {{0.0,0.0}, {0.0, 0.0}}},
                                     NOTIME, NOTIME};  
static SUDS_COMMENT      NullCO   =  {NODATA, NODATA, 0, NODATA};
static SUDS_CHANSET      NullCS   =  {NODATA, 0, NOSTRG, NOSTRG, NOTIME, NOTIME};
/*static CHANSETENTRY      NullCE   =  {NODATA, NODATA, NODATA, { NOSTRG, NOSTRG, NOCHAR, 
                                        NODATA} };*/

static SUDS_DESCRIPTRACE NullDT   =  {{ NOSTRG, NOSTRG, NOCHAR,NODATA}, NOTIME, NODATA,
                                     NOCHAR, NOCHAR, NODATA, NODATA, 0, NODATA,
				      NODATA, NODATA, NODATA, NODATA, 0, 0};  
static SUDS_DETECTOR     NullDE   =  {NOCHAR, NOCHAR, NOSTRG, NODATA, NODATA, NODATA};
static SUDS_EQUIPMENT    NullEQ   =  {{ NOSTRG, NOSTRG, NOCHAR,NODATA}, 
                                     { NOSTRG, NOSTRG, NOCHAR,NODATA}, 
                                     { NOSTRG, NOSTRG, NOCHAR,NODATA}, 
				       NOSTRG, NODATA,
				      NODATA, NODATA, NODATA, NODATA, NODATA};
static SUDS_ERROR        NullER =    { {NODATA, NODATA, NODATA, NODATA, NODATA,
                                        NODATA, NODATA, NODATA, NODATA, NODATA} };
static SUDS_EVENT        NullEV   =  {NODATA, NODATA, NODATA, NOCHAR, NOCHAR, NOCHAR,
                                      NOCHAR, NOCHAR, NOCHAR, NODATA};
static SUDS_EVENTSETTING NullES   =  {NOSTRG, NOTIME, NODATA, NODATA, NODATA, NODATA,
                                      NODATA, NODATA, NOCHAR, NOCHAR, NODATA}; 
static SUDS_EVDESCR      NullED   =  {NOSTRG, NOSTRG, NOSTRG, NODATA, NODATA};
static SUDS_FEATURE      NullFE   =  {{NOSTRG, NOSTRG, NOCHAR,NODATA}, NODATA,
                                      NOCHAR, NOCHAR, NODATA, NOCHAR, NOCHAR, NOCHAR, 
                                      NOCHAR, NODATA, NOTIME, NODATA, NODATA, NOTIME,
                                      NODATA, NODATA};
static SUDS_FOCALMECH    NullFO   =  {NODATA, NODATA, NODATA, NODATA, NODATA, NODATA,
                                      NOCHAR, NOSTRG}; 
static SUDS_INSTRUMENT   NullIN   =  {{NOSTRG, NOSTRG, NOCHAR,NODATA}, NODATA, NODATA,
                                      NODATA, NOCHAR, NOCHAR, NODATA, NODATA, NODATA, 
                                      NODATA, NODATA, NODATA, NODATA, NODATA, NODATA, 
                                      NODATA, NODATA, NOTIME, NODATA, NODATA, NOSTRG,
                                      NODATA};
static SUDS_LAYERS       NullLA   =  {NODATA, NODATA, NODATA, NODATA, NODATA, NODATA,
                                      NODATA}; 
static SUDS_LOCTRACE     NullLO   =  {{NOSTRG, NOSTRG, NOCHAR,NODATA}, 0, 0, NODATA}; 
static SUDS_MOMENT       NullMO   =  { 0, NOCHAR, NOSTRG, NODATA, {NODATA, NODATA,  
                                      NODATA, NODATA, NODATA, NODATA} }; 


static SUDS_MUXDATA      NullMU   =  {NOSTRG, NOTIME, 0, NODATA, NODATA, NOCHAR, NOCHAR,
                                      NODATA, NODATA, NODATA}; 
static SUDS_ORIGIN       NullOR   =  {NODATA, NODATA, NOCHAR, NOCHAR, NOCHAR, NOCHAR,
                                      NOCHAR, NOCHAR, NODATA, NOTIME, NODATA, NODATA,
                                      NODATA, NODATA, NODATA, NODATA, NOSTRG, NODATA,
                                      NODATA, NODATA, NODATA, NODATA, NODATA, NODATA, 
                                      NODATA, NODATA, NODATA, NODATA, NODATA, NODATA, 
                                      NOTIME}; 
static SUDS_PROFILE      NullPR   =  {NODATA};                                       
static SUDS_RESIDUAL     NullRE   =  {NODATA,{NOSTRG, NOSTRG, NOCHAR,NODATA}, NODATA,
                                      NOCHAR, NOCHAR, NODATA, NODATA, NODATA, NODATA, 
                                      NODATA, NODATA};  
static SUDS_SHOTGATHER   NullSH   =  {NODATA}; 
static SUDS_STATIONCOMP  NullSC   =  {{NOSTRG, NOSTRG, NOCHAR,NODATA}, NODATA, NODATA,
                                      NODATA, NODATA, NODATA, NOCHAR, NOCHAR, NOCHAR,
                                      NOCHAR, NODATA, NOCHAR, NOCHAR, NOCHAR, NOCHAR, 
                                      NOCHAR, NOCHAR, NODATA, NODATA, NODATA, NODATA, 
                                      NODATA, NOTIME, NODATA, NODATA};
static SUDS_TERMINATOR   NullTE   =  {NODATA, NODATA}; 
static SUDS_TIMECORRECTION NullTC =  {{NOSTRG, NOSTRG, NOCHAR,NODATA}, 0, NODATA,
                                      NOCHAR, NOCHAR, NOTIME, NODATA}; 
static SUDS_TRIGGERS     NullTR   =  {{NOSTRG, NOSTRG, NOCHAR,NODATA}, NODATA, NODATA, 
                                      NODATA, NODATA, NODATA, NODATA, NOTIME}; 
static SUDS_TRIGSETTING  NullTS   =  {NOSTRG, NOTIME, NODATA, NODATA, NODATA, NODATA, 
                                      NODATA, NODATA, NODATA, NODATA, NOCHAR, NOCHAR,
                                      NODATA}; 
static SUDS_VELMODEL     NullVM   =  {NOSTRG, NOSTRG, NOCHAR, NOCHAR, NODATA, NODATA, 
                                      NODATA, NODATA, NOTIME}; 

#endif
 /* SUDS_VELMODEL:  Velocity model */

#endif


