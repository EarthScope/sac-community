/**
 * @file sacio.h
 *
 * @brief SAC Input/Output Functions
 */

#ifndef __SACIO_H__
#define __SACIO_H__

/** 
 * Define an automatic string length from C
 *
 */
#define SAC_STRING_LENGTH   -1

#ifndef TRUE
#define TRUE  1
#endif /* TRUE */

#ifndef FALSE
#define FALSE 0
#endif /* FALSE */

#define SAC_FLOAT_UNDEFINED      -12345.0
#define SAC_REAL_UNDEFINED       SAC_FLOAT_UNDEFINED
#define SAC_INT_UNDEFINED        -12345
#define SAC_INTEGER_UNDEFINED    SAC_INT_UNDEFINED
#define SAC_NUMBER_UNDEFINED     SAC_INT_UNDEFINED
#define SAC_CHAR_UNDEFINED       "-12345  "
#define SAC_CHARACTER_UNDEFINED  SAC_CHAR_UNDEFINED

void getfhv(char      *kname, 
            float     *fvalue, 
            int       *nerr, 
            int        kname_s);

void getihv(char      *kname, 
            char      *kvalue, 
            int       *nerr, 
            int        kname_s, 
            int        kvalue_s);

void getkhv(char      *kname, 
            char      *kvalue, 
            int       *nerr, 
            int        kname_s, 
            int        kvalue_s);

void getlhv(char      *kname, 
            int       *lvalue, 
            int       *nerr, 
            int        kname_s);

void getnhv(char      *kname, 
            int       *nvalue, 
            int       *nerr, 
            int        kname_s);

void newhdr (void);

void rsac1(char      *kname, 
           float      yarray[], 
           int       *nlen, 
           float     *beg, 
           float     *del, 
           int       *max_, 
           int       *nerr, 
           int        kname_s);

void rsac2(char      *kname, 
           float      yarray[], 
           int       *nlen, 
           float      xarray[], 
           int       *max_, 
           int       *nerr, 
           int        kname_s);

void rsach(char      *kname,
           int       *nerr,
           int        kname_s);

void setfhv(char      *kname, 
            float     *fvalue, 
            int       *nerr, 
            int        kname_s);

void setihv(char      *kname, 
            char      *kvalue, 
            int       *nerr, 
            int        kname_s,
            int        kvalue_s);

void setkhv(char      *kname, 
            char      *kvalue, 
            int       *nerr, 
            int        kname_s,
            int        kvalue_s);

void setlhv(char      *kname, 
            int       *lvalue, 
            int       *nerr, 
            int        kname_s);

void setnhv(char      *kname, 
            int       *nvalue, 
            int       *nerr, 
            int        kname_s);

void wsac0(char      *kname, 
           float     *xarray, 
           float     *yarray,
           int       *nerr,
           int        kname_s);

void wsac1(char      *kname, 
           float     *yarray, 
           int       *nlen, 
           float     *beg, 
           float     *del, 
           int       *nerr, 
           int        kname_s);

void wsac2(char      *kname, 
           float     *yarray, 
           int       *nlen, 
           float     *xarray, 
           int       *nerr, 
           int        kname_s);

void wsac3(char      *kname, 
           float     *xarray, 
           float     *yarray,
           int       *nerr,
           int        kname_s);

void getbbv(char      *kname, 
            char      *kvalue, 
            int       *nerr, 
            int        kname_s, 
            int        kvalue_s);

void writebbf(char      *kname, 
              int       *nerr, 
              int        kname_s);

void readbbf(char      *kname, 
             int       *nerr, 
             int        kname_s);


void setbbv(char      *kname, 
            char      *kvalue, 
            int       *nerr, 
            int        kname_s, 
            int        kvalue_s);

void sac_warning_stdout(void);
void sac_warning_stderr(void);
void sac_warning_off(void);

void sac_error_stdout(void);
void sac_error_stderr(void);
void sac_error_off(void);

void sac_output_stdout(void);
void sac_output_stderr(void);
void sac_output_off(void);



/* SAC header structure */
struct SACheader {
    float delta;                /* RF time increment, sec    */
    float depmin;               /*    minimum amplitude      */
    float depmax;               /*    maximum amplitude      */
    float scale;                /*    amplitude scale factor */
    float odelta;               /*    observed time inc      */
    float b;                    /* RD initial value, time    */
    float e;                    /* RD final value, time      */
    float o;                    /*    event start, sec < nz. */
    float a;                    /*    1st arrival time       */
    float fmt;                  /*    internal use           */
    float t0;                   /*    user-defined time pick */
    float t1;                   /*    user-defined time pick */
    float t2;                   /*    user-defined time pick */
    float t3;                   /*    user-defined time pick */
    float t4;                   /*    user-defined time pick */
    float t5;                   /*    user-defined time pick */
    float t6;                   /*    user-defined time pick */
    float t7;                   /*    user-defined time pick */
    float t8;                   /*    user-defined time pick */
    float t9;                   /*    user-defined time pick */
    float f;                    /*    event end, sec > nz    */
    float resp0;                /*    instrument respnse parm */
    float resp1;                /*    instrument respnse parm */
    float resp2;                /*    instrument respnse parm */
    float resp3;                /*    instrument respnse parm */
    float resp4;                /*    instrument respnse parm */
    float resp5;                /*    instrument respnse parm */
    float resp6;                /*    instrument respnse parm */
    float resp7;                /*    instrument respnse parm */
    float resp8;                /*    instrument respnse parm */
    float resp9;                /*    instrument respnse parm */
    float stla;                 /*  T station latititude     */
    float stlo;                 /*  T station longitude      */
    float stel;                 /*  T station elevation, m   */
    float stdp;                 /*  T station depth, m      */
    float evla;                 /*    event latitude         */
    float evlo;                 /*    event longitude        */
    float evel;                 /*    event elevation        */
    float evdp;                 /*    event depth            */
    float mag;                  /*    reserved for future use */
    float user0;                /*    available to user      */
    float user1;                /*    available to user      */
    float user2;                /*    available to user      */
    float user3;                /*    available to user      */
    float user4;                /*    available to user      */
    float user5;                /*    available to user      */
    float user6;                /*    available to user      */
    float user7;                /*    available to user      */
    float user8;                /*    available to user      */
    float user9;                /*    available to user      */
    float dist;                 /*    stn-event distance, km */
    float az;                   /*    event-stn azimuth      */
    float baz;                  /*    stn-event azimuth      */
    float gcarc;                /*    stn-event dist, degrees */
    float sb;                   /*    internal use           */
    float sdelta;               /*    internal use           */
    float depmen;               /*    mean value, amplitude  */
    float cmpaz;                /*  T component azimuth     */
    float cmpinc;               /*  T component inclination */
    float xminimum;             /*    reserved for future use */
    float xmaximum;             /*    reserved for future use */
    float yminimum;             /*    reserved for future use */
    float ymaximum;             /*    reserved for future use */
    float unused6;              /*    reserved for future use */
    float unused7;              /*    reserved for future use */
    float unused8;              /*    reserved for future use */
    float unused9;              /*    reserved for future use */
    float unused10;             /*    reserved for future use */
    float unused11;             /*    reserved for future use */
    float unused12;             /*    reserved for future use */
    int nzyear;                 /*  F zero time of file, yr  */
    int nzjday;                 /*  F zero time of file, day */
    int nzhour;                 /*  F zero time of file, hr  */
    int nzmin;                  /*  F zero time of file, min */
    int nzsec;                  /*  F zero time of file, sec */
    int nzmsec;                 /*  F zero time of file, msec */
    int nvhdr;                  /*    internal use           */
    int norid;                  /*    origin ID              */
    int nevid;                  /*    event ID               */
    int npts;                   /* RF number of samples      */
    int nsnpts;                 /*    internal use           */
    int nwfid;                  /*    waveform ID            */
    int nxsize;                 /*    reserved for future use */
    int nysize;                 /*    reserved for future use */
    int unused15;               /*    reserved for future use */
    int iftype;                 /* RA type of file          */
    int idep;                   /*    type of amplitude      */
    int iztype;                 /*    zero time equivalence  */
    int unused16;               /*    reserved for future use */
    int iinst;                  /*    recording instrument   */
    int istreg;                 /*    stn geographic region  */
    int ievreg;                 /*    event geographic region */
    int ievtyp;                 /*    event type             */
    int iqual;                  /*    quality of data        */
    int isynth;                 /*    synthetic data flag    */
    int imagtyp;                /*    reserved for future use */
    int imagsrc;                /*    reserved for future use */
    int unused19;               /*    reserved for future use */
    int unused20;               /*    reserved for future use */
    int unused21;               /*    reserved for future use */
    int unused22;               /*    reserved for future use */
    int unused23;               /*    reserved for future use */
    int unused24;               /*    reserved for future use */
    int unused25;               /*    reserved for future use */
    int unused26;               /*    reserved for future use */
    int leven;                  /* RA data-evenly-spaced flag */
    int lpspol;                 /*    station polarity flag  */
    int lovrok;                 /*    overwrite permission   */
    int lcalda;                 /*    calc distance, azimuth */
    int unused27;               /*    reserved for future use */
    char kstnm[9];              /*  F station name           */
    char kevnm[18];             /*    event name             */
    char khole[9];              /*    man-made event name    */
    char ko[9];                 /*    event origin time id   */
    char ka[9];                 /*    1st arrival time ident */
    char kt0[9];                /*    time pick 0 ident      */
    char kt1[9];                /*    time pick 1 ident      */
    char kt2[9];                /*    time pick 2 ident      */
    char kt3[9];                /*    time pick 3 ident      */
    char kt4[9];                /*    time pick 4 ident      */
    char kt5[9];                /*    time pick 5 ident      */
    char kt6[9];                /*    time pick 6 ident      */
    char kt7[9];                /*    time pick 7 ident      */
    char kt8[9];                /*    time pick 8 ident      */
    char kt9[9];                /*    time pick 9 ident      */
    char kf[9];                 /*    end of event ident     */
    char kuser0[9];             /*    available to user      */
    char kuser1[9];             /*    available to user      */
    char kuser2[9];             /*    available to user      */
    char kcmpnm[9];             /*  F component name         */
    char knetwk[9];             /*    network name           */
    char kdatrd[9];             /*    date data read         */
    char kinst[9];              /*    instrument name        */
};
#define CASSERT(predicate, file) _impl_CASSERT_LINE(predicate,__LINE__,file)

#define _impl_PASTE(a,b) a##b
#define _impl_CASSERT_LINE(predicate, line, file) \
  typedef char _impl_PASTE(assertion_failed_##file##_,line)[2*!!(predicate)-1];

CASSERT(sizeof(struct SACheader) == 656, SacHeader_h)

typedef struct _sacmeta sacmeta;
struct _sacmeta {
    int swap;
    char *filename;
    int data_read;
    int nstop;
    int nstart;
    int nfillb;
    int nfille;
    int ntotal;
};

typedef struct _sac sac;
struct _sac {
    struct SACheader *h;
    int n;
    float *y;
    float *x;
    sacmeta *m;
    int *sddhdr;           /* SDD Header - Length MWESHD - 164 */
};

typedef struct complexf_t complexf;
typedef struct complexd_t complexd;

struct complexf_t {
    float re;      /** Real Part */
    float im;      /** Imaginary Part */
};
struct complexd_t {
    double re;      /** Real Part */
    double im;      /** Imaginary Part */
};

typedef struct _datetime datetime;

struct _datetime {
    int year;                   /* Year */
    int month;                  /* Month */
    int day;                    /* Day of the month: */
    int doy;                    /* Day of the year: 1 - 366 */
    int hour;                   /* Hour:   0 - 23 */
    int minute;                 /* Minute: 0 - 59 */
    int second;                 /* Second: 0 - 59 */
    long long int nanosecond;   /* Nanoseconds */
    long long int time;
    int set;
};

sac * sac_new(void);
void  sac_free(sac *s);
sac * sac_read(char *filename, int *nerr);
sac * sac_read_header(char *filename, int *nerr);
void  sac_write(sac *s, char *filename, int *nerr);
sac * sac_copy(sac *s);
void  sac_extrema(sac *s);
int   sac_comps(sac * s);

char *rstrip(char *s);

#define SAC_WRITE_HEADER_AND_DATA 1
#define SAC_READ_HEADER_AND_DATA  1
#define SAC_WRITE_HEADER          0
#define SAC_READ_HEADER           0
#define SAC_NO_BYTESWAP_FILE      0
#define SAC_BYTESWAP_FILE         1

void update_distaz(sac * s);
void distaz(double the, double phe, float *ths, float *phs,
            int ns, float *dist, float *az, float *baz, float *xdeg,
            int *nerr);

#endif /* __SACIO_H__ */
