/**
 * @file
 *
 * @brief SAC Input/Output Functions
 *
 */

#ifndef __SACIO_H__
#define __SACIO_H__

#include <stdlib.h>
#include <sys/stat.h>
#include "timespec.h"

/** \cond NO_DOCS */
#define SAC_STRING_LENGTH   -1

#ifndef TRUE
#define TRUE  1
#endif /* TRUE */

#ifndef FALSE
#define FALSE 0
#endif /* FALSE */
/** \endcond */

/**
 * @brief Sac Cutting Behavior
 *
 * @memberof sac
 * @ingroup sac
 *
 */
enum CutAction {
    CutNone     = 0,  /**< @brief Cut Action is not defined */
    CutFatal    = 1,  /**< @brief Cut window is only allowed within data region */
    CutUseBE    = 2,  /**< @brief Cut windows larger than data region are corrected to either the begin and end values */
    CutFillZero = 3,  /**< @brief Cut windows larger than data region are filled with zeros */
};

typedef struct Spherioid Spheroid;
struct Spherioid {
    char name[32]; // Descriptive name of the Spheroid
    char code[32]; // EPGS or IAU code including prefix
    double a;      // Semi-major axis in meters
    double f;      // flattening (a-b)/a ; where a = semi-major axis, b = semi-minor axis
    int ibody;     // Sac Code for Spherioid
};

/**
 * @brief Check Byte Order Flag
 *
 * @memberof sac
 * @ingroup sac
 *
 */
typedef enum CheckByteOrderFlag CheckByteOrderFlag;
enum CheckByteOrderFlag {
    CheckByteOrderOff = FALSE, /**< @brief Do not check byte order on comparison, aliases to 0 */
    CheckByteOrderOn  = TRUE,  /**< @brief Check byte order on comparison, aliases to 1 */
};

/**
 * @brief Verbose Flag
 *
 * @memberof sac 
 * @ingroup sac
 *
 */
typedef enum Verbose Verbose;
enum Verbose {
    VerboseOff = FALSE,  /**< @brief Do not be verbose, aliases to 0 */
    VerboseOn  = TRUE,   /**< @brief Be verbose when called for, aliases to 1 */
};

#define SAC_FLOAT_UNDEFINED      -12345.0            /**< @brief Float undefined value */
#define SAC_INT_UNDEFINED        -12345              /**< @brief Integer undefined value */
#define SAC_CHAR_UNDEFINED       "-12345  "          /**< @brief Character string undefined */
#define SAC_CHAR_UNDEFINED_2     "-12345          "  /**< @brief Character string undefined - 16 characters */
#define SAC_REAL_UNDEFINED       SAC_FLOAT_UNDEFINED /**< @brief Float undefined value */
#define SAC_INTEGER_UNDEFINED    SAC_INT_UNDEFINED   /**< @brief Integer undefined value */
#define SAC_NUMBER_UNDEFINED     SAC_INT_UNDEFINED   /**< @brief Integer undefined value */
#define SAC_CHARACTER_UNDEFINED  SAC_CHAR_UNDEFINED  /**< @brief Character string undefined value */
#define SAC_ENUM_UNDEFINED       SAC_INT_UNDEFINED   /**< @brief Enum undefined value */
#define SAC_LOGICAL_UNDEFINED    SAC_INT_UNDEFINED   /**< @brief Logical undefined value */

/** \cond NO_DOCS */
/** @brief Get a float value from a sac header */
void getfhv(char      *kname,
            float     *fvalue,
            int       *nerr,
            int        kname_s);
/** @brief Get a float value from a sac header */
void getdhv(char      *kname,
            double    *fvalue,
            int       *nerr,
            int        kname_s);
/** @brief Get a enum value from a sac header */
void getihv(char      *kname,
            char      *kvalue,
            int       *nerr,
            int        kname_s,
            int        kvalue_s);

/** @brief Get a character string from a sac header */
void getkhv(char      *kname,
            char      *kvalue,
            int       *nerr,
            int        kname_s,
            int        kvalue_s);
/** @brief Get a logical value from a sac header */
void getlhv(char      *kname,
            int       *lvalue,
            int       *nerr,
            int        kname_s);

/** @brief Get a integer value from a sac header */
void getnhv(char      *kname,
            int       *nvalue,
            int       *nerr,
            int        kname_s);

/** @brief Create a new sac header */
void newhdr (void);

/** @brief Read a evenly spaced file */
void rsac1(char      *kname,
           float      yarray[],
           int       *nlen,
           float     *beg,
           float     *del,
           int       *max_,
           int       *nerr,
           int        kname_s);
/** @brief Read a unevenly spaced file */
void rsac2(char      *kname,
           float      yarray[],
           int       *nlen,
           float      xarray[],
           int       *max_,
           int       *nerr,
           int        kname_s);
/** @brief Read a sac header */
void rsach(char      *kname,
           int       *nerr,
           int        kname_s);
/** @brief Set a float value in a sac header */
void setfhv(char      *kname,
            float     *fvalue,
            int       *nerr,
            int        kname_s);
/** @brief Set a float value in a sac header */
void setdhv(char      *kname,
            double    *fvalue,
            int       *nerr,
            int        kname_s);

/** @brief Set a enum value in a sac header */
void setihv(char      *kname,
            char      *kvalue,
            int       *nerr,
            int        kname_s,
            int        kvalue_s);
/** @brief Set a character string value in a sac header */
void setkhv(char      *kname,
            char      *kvalue,
            int       *nerr,
            int        kname_s,
            int        kvalue_s);
/** @brief Set a logical value in a sac header */
void setlhv(char      *kname,
            int       *lvalue,
            int       *nerr,
            int        kname_s);
/** @brief Set a integer value in a sac header */
void setnhv(char      *kname,
            int       *nvalue,
            int       *nerr,
            int        kname_s);
/** @brief Write a sac file */
void wsac0(char      *kname,
           float     *xarray,
           float     *yarray,
           int       *nerr,
           int        kname_s);
/** @brief Write an evenly spaced sac file */
void wsac1(char      *kname,
           float     *yarray,
           int       *nlen,
           float     *beg,
           float     *del,
           int       *nerr,
           int        kname_s);
/** @brief Write an unevenly spaced sac file */
void wsac2(char      *kname,
           float     *yarray,
           int       *nlen,
           float     *xarray,
           int       *nerr,
           int        kname_s);
/** @brief Write a sac file, update distaz in the process */
void wsac3(char      *kname,
           float     *xarray,
           float     *yarray,
           int       *nerr,
           int        kname_s);
/*
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
*/
/** \endcond */


/**
 * @see SACheader
 */
typedef struct sac_hdr sac_hdr;

/**
 * @brief sac header
 *
 *  References:
 *          - O'Neill, D. (1987).  IRIS Interim Data Distribution Format
 *                  (SAC ASCII), Version 1.0 (12 November 1987).  Incorporated
 *                  Research Institutions for Seismology, 1616 North Fort Myer
 *                  Drive, Suite 1440, Arlington, Virginia 22209.  11 pp.
 *          - Tull, J. (1987).  SAC User's Manual, Version 10.2, October 7, 1987.
 *                  Lawrence Livermore National Laboratory, L-205,
 *                  Livermore, California 94550.  ??? pp.
 *
 *  <pre>
 *       Column 1:
 *                R       required by SAC
 *         (blank)       optional
 *       Column 2:
 *               A = settable from a priori knowledge
 *               D = available in data
 *               F = available in or derivable from SEED fixed data header
 *               T = available in SEED header tables
 *         (blank) = not directly available from SEED data, header
 *                   tables, or elsewhere
 *   </pre>
 */
struct sac_hdr {
    float _delta;                /**< @brief time increment, sec <b>Required</b>*/
    float depmin;               /**< @brief minimum amplitude      */
    float depmax;               /**< @brief maximum amplitude      */
    float scale;                /**< @brief amplitude scale factor */
    float odelta;               /**< @brief observed time inc      */
    float _b;                    /**< @brief initial value, time  <b>Required, available in data</b>  */
    float _e;                    /**< @brief final value, time   <b>Required, available in data</b>   */
    float _o;                    /**< @brief      event start, sec < nz. */
    float _a;                    /**< @brief      1st arrival time       */
    float fmt;                  /**< @brief      internal use @private           */
    float _t0;                   /**< @brief      user-defined time pick */
    float _t1;                   /**< @brief      user-defined time pick */
    float _t2;                   /**< @brief      user-defined time pick */
    float _t3;                   /**< @brief      user-defined time pick */
    float _t4;                   /**< @brief      user-defined time pick */
    float _t5;                   /**< @brief      user-defined time pick */
    float _t6;                   /**< @brief      user-defined time pick */
    float _t7;                   /**< @brief      user-defined time pick */
    float _t8;                   /**< @brief      user-defined time pick */
    float _t9;                   /**< @brief      user-defined time pick */
    float _f;                    /**< @brief      event end, sec > nz    */
    float resp0;                /**< @brief      instrument respnse parm */
    float resp1;                /**< @brief      instrument respnse parm */
    float resp2;                /**< @brief      instrument respnse parm */
    float resp3;                /**< @brief      instrument respnse parm */
    float resp4;                /**< @brief      instrument respnse parm */
    float resp5;                /**< @brief      instrument respnse parm */
    float resp6;                /**< @brief      instrument respnse parm */
    float resp7;                /**< @brief      instrument respnse parm */
    float resp8;                /**< @brief      instrument respnse parm */
    float resp9;                /**< @brief      instrument respnse parm */
    float _stla;                 /**< @brief    station latititude <b>available from SEED</b>      */
    float _stlo;                 /**< @brief    station longitude <b>available from SEED</b>     */
    float stel;                 /**< @brief    station elevation, m <b>available from SEED</b>  */
    float stdp;                 /**< @brief    station depth, m <b>available from SEED</b>     */
    float _evla;                 /**< @brief      event latitude         */
    float _evlo;                 /**< @brief      event longitude        */
    float evel;                 /**< @brief      event elevation        */
    float evdp;                 /**< @brief      event depth            */
    float mag;                  /**< @brief      event magnitude */
    float user0;                /**< @brief      available to user      */
    float user1;                /**< @brief      available to user      */
    float user2;                /**< @brief      available to user      */
    float user3;                /**< @brief      available to user      */
    float user4;                /**< @brief      available to user      */
    float user5;                /**< @brief      available to user      */
    float user6;                /**< @brief      available to user      */
    float user7;                /**< @brief      available to user      */
    float user8;                /**< @brief      available to user      */
    float user9;                /**< @brief      available to user      */
    float dist;                 /**< @brief      station-event distance, km */
    float az;                   /**< @brief      event-station azimuth      */
    float baz;                  /**< @brief      station-event azimuth      */
    float gcarc;                /**< @brief      station-event dist, degrees */
    float _sb;                  /**< @brief      saved b value, for spectral files           */
    float _sdelta;              /**< @brief      saved delta, for spectral files    */
    float depmen;               /**< @brief      mean value, amplitude  */
    float cmpaz;                /**< @brief    component azimuth  <b>available from SEED</b>
                                 * - 0: North
                                 * - 90 East
                                 * - 180: South
                                 * - 270: West
                                 */
    float cmpinc;               /**< @brief    component inclination <b>available from SEED</b>
                                 * - 0: Up,Vertical
                                 * - 90 Horizontal
                                 * @warning This convention is different from the SEED convention where
                                 *          the incliination is specified as a \p dip, or the degrees
                                 *          down from the horizontal.  This would make up, vertical, -90.0
                                 */
    float xminimum;             /**< @brief     min x value for IXY files */
    float xmaximum;             /**< @brief     max x value for IXY files */
    float yminimum;             /**< @brief     min y value for IXY files */
    float ymaximum;             /**< @brief     max y value for IXY files */
    float unused6;              /**< @brief      reserved for future use @private */
    float unused7;              /**< @brief      reserved for future use @private */
    float unused8;              /**< @brief      reserved for future use @private */
    float unused9;              /**< @brief      reserved for future use @private */
    float unused10;             /**< @brief      reserved for future use @private */
    float unused11;             /**< @brief      reserved for future use @private */
    float unused12;             /**< @brief      reserved for future use @private */
    int nzyear;                 /**< @brief    zero time of file, yr <b>available from SEED</b> */
    int nzjday;                 /**< @brief    zero time of file, day <b>available from SEED</b> */
    int nzhour;                 /**< @brief    zero time of file, hr <b>available from SEED</b>  */
    int nzmin;                  /**< @brief    zero time of file, min <b>available from SEED</b> */
    int nzsec;                  /**< @brief    zero time of file, sec <b>available from SEED</b> */
    int nzmsec;                 /**< @brief    zero time of file, msec <b>available from SEED</b> */
    int nvhdr;                  /**< @brief    header version           */
    int norid;                  /**< @brief      origin ID              */
    int nevid;                  /**< @brief      event ID               */
    int npts;                   /**< @brief   number of samples <b>Required, available from SEED</b>     */
    int nsnpts;                 /**< @brief   save npts, for spectral files           */
    int nwfid;                  /**< @brief      waveform ID            */
    int nxsize;                 /**< @brief      size in x direction for IXY files */
    int nysize;                 /**< @brief      size in y direction for IXY files */
    int unused15;               /**< @brief      reserved for future use @private */
    int iftype;                 /**< 
                                 * @brief  type of file       <b>Required, settable from a priori knowledge</b>
                                 * - \ref ITIME
                                 * - \ref IXY
                                 * - \ref IAMPH
                                 * - \ref IRLIM
                                 * - \ref IXYZ
                                 *
                                 */
    int idep;                   /**< @brief      type of amplitude      */
    int iztype;                 /**< @brief      zero time equivalence  */
    int unused16;               /**< @brief      reserved for future use  @private */
    int iinst;                  /**< @brief      recording instrument   */
    int istreg;                 /**< @brief      stn geographic region  */
    int ievreg;                 /**< @brief      event geographic region */
    int ievtyp;                 /**< @brief      event type             */
    int iqual;                  /**< @brief      quality of data        */
    int isynth;                 /**< @brief      synthetic data flag    */
    int imagtyp;                /**< @brief      magnitude type */
    int imagsrc;                /**< @brief      magnitude source  */
    int ibody;                  /**< @brief      Planet / Spheroid */
    int unused20;               /**< @brief      reserved for future use @private */
    int unused21;               /**< @brief      reserved for future use @private */
    int unused22;               /**< @brief      reserved for future use @private */
    int unused23;               /**< @brief      reserved for future use @private */
    int unused24;               /**< @brief      reserved for future use @private  */
    int unused25;               /**< @brief      reserved for future use @private */
    int unused26;               /**< @brief      reserved for future use @private */
    int leven;                  /**< @brief   data-evenly-spaced flag <b>Required, settable from a priori knowledge</b> */
    int lpspol;                 /**< @brief      station polarity flag  */
    int lovrok;                 /**< @brief      overwrite permission   */
    int lcalda;                 /**< @brief      calc distance, azimuth */
    int unused27;               /**< @brief      reserved for future use @private */
    char kstnm[9];              /**< @brief    station name  <b>available from SEED</b>         */
    char kevnm[18];             /**< @brief    event name             */
    char khole[9];              /**< @brief    location <b>available from SEED</b>    */
    char ko[9];                 /**< @brief    event origin time id   */
    char ka[9];                 /**< @brief    1st arrival time ident */
    char kt0[9];                /**< @brief    time pick 0 ident      */
    char kt1[9];                /**< @brief    time pick 1 ident      */
    char kt2[9];                /**< @brief    time pick 2 ident      */
    char kt3[9];                /**< @brief    time pick 3 ident      */
    char kt4[9];                /**< @brief    time pick 4 ident      */
    char kt5[9];                /**< @brief    time pick 5 ident      */
    char kt6[9];                /**< @brief    time pick 6 ident      */
    char kt7[9];                /**< @brief    time pick 7 ident      */
    char kt8[9];                /**< @brief    time pick 8 ident      */
    char kt9[9];                /**< @brief    time pick 9 ident      */
    char kf[9];                 /**< @brief    end of event ident     */
    char kuser0[9];             /**< @brief    available to user      */
    char kuser1[9];             /**< @brief    available to user      */
    char kuser2[9];             /**< @brief    available to user      */
    char kcmpnm[9];             /**< @brief    component name  <b>available from SEED </b>     */
    char knetwk[9];             /**< @brief    network name   <b> available from SEED </b>          */
    char kdatrd[9];             /**< @brief    date data read         */
    char kinst[9];              /**< @brief    instrument name        */
};


/** \cond NO_DOCS */
/**
 * @brief fancy failing macro
 */
#define CASSERT(predicate, file) _impl_CASSERT_LINE(predicate,__LINE__,file)
/**
 * @brief string concat macro
 */
#define _impl_PASTE(a,b) a##b
/**
 * @brief assertion on a line, compile time
 */
#define _impl_CASSERT_LINE(predicate, line, file)                       \
  typedef char _impl_PASTE(assertion_failed_##file##_,line)[2*!!(predicate)-1];
/** \endcond */
CASSERT(sizeof(struct sac_hdr) == 656, SacHeader_h)

typedef struct _sacmeta sacmeta;
/**
 * Sac Meta Data
 * @private
 */
struct _sacmeta {
    int swap; /**<< \brief If file was swapped on read*/
    char *filename; /**<< \brief filename as given during read */
    int data_read; /**<< \brief If data was read*/
    int nstop;  /**<< \brief Index of point to begin data at, b = 1, e = npts */
    int nstart; /**<< \brief Index of point to end data at, b = 1, e = npts */
    int nfillb; /**<< \brief Points before the first point to read */
    int nfille; /**<< \brief Points after the last point to read  */
    int ntotal; /**<< \brief total number of points */
};

typedef struct _sac_f64 sac_f64;
/**
 * SAC 64 bit floating point "header"
 * @private
 */
struct _sac_f64 {
    double _delta;
    double _b;
    double _e;
    double _o;
    double _a;
    double _t0;
    double _t1;
    double _t2;
    double _t3;
    double _t4;
    double _t5;
    double _t6;
    double _t7;
    double _t8;
    double _t9;
    double _f;
    double _evlo;
    double _evla;
    double _stlo;
    double _stla;
    double _sb;
    double _sdelta;
};


typedef struct sac sac;
/**
 * @brief sac data and header structure
 *
 */
struct sac {
    sac_hdr *h;          /**< @brief  sac header */
    int n;               /**< @brief  @private n value */
    float *y;            /**< @brief  first data component */
    float *x;            /**< @brief  second data component */
    sacmeta *m;          /**< @brief  @private sac meta data */
    sac_f64 *z;          /**< @brief  @private sac 64 bit floating pointer header */
    int *sddhdr;         /**< @brief  @private SDD Header - Length MWESHD - 164 */
};

typedef struct complexf_t complexf;
typedef struct complexd_t complexd;

/**
 * @brief Single precision complex number
 */
struct complexf_t {
    float re;      /**< @brief  Real Part */
    float im;      /**< @brief  Imaginary Part */
};
/**
 * Double precision complex number
 */
struct complexd_t {
    double re;      /**< @brief  Real Part */
    double im;      /**< @brief  Imaginary Part */
};

/** \cond NO_DOCS */
/** @brief Create a new sac object */
sac * sac_new(void);
/** @brief Free a sac object */
void  sac_free(sac *s);
/** @brief Read a sac file */
sac * sac_read(char *filename, int *nerr);
/** @brief Read a sac file in alphanumeric format */
sac * sac_read_alpha(char *filename, int *nerr);
/** @brief Read a sac file within a cut window */
sac * sac_read_with_cut(char *filename, char *c1, double t1, char *c2, double t2, enum CutAction cutact, int *nerr);
/** @brief Cut a sac file returning a new sac file */
sac * sac_cut(sac *sin, char *c1, double t1, char *c2, double t2, enum CutAction cutact, int *nerr);
/** @brief Read a sac header */
sac * sac_read_header(char *filename, int *nerr);
/** @brief Write a sac file */
void  sac_write(sac *s, char *filename, int *nerr);
/** @brief Write a sac file header for an existing file*/
void sac_write_header(sac *s, char *filename, int *nerr);
/** @brief Write a sac file in alphanumeric format */
void  sac_write_alpha(sac *s, char *filename, int *nerr);
/** @brief Copy a sac object  */
sac * sac_copy(sac *s);
/** @brief Compute and set depmin, depmax, depmen */
void  sac_extrema(sac *s);
/** @brief Get the number of components from a sac string */
int   sac_comps(sac * s);
/** @brief Return if the header value represents a time */
int   sac_is_timeval(int hid);

void  sac_meta_copy(sac *to, sac *from);
void  sac_header_copy(sac *to, sac *from);
void  sac_data_copy(sac *to, sac *from);
sac * sac_copy(sac *s);
void  sac_set_v7(sac *s);
void  sac_set_v6(sac *s);

void sac_check_time_precision(sac *s);

/** @brief Get a float value from a sac object */
int sac_get_float(sac *s, int hdr, double *v);
/** @brief SEt a float value from a sac object */
int sac_set_float(sac *s, int hdr, double v);
/** @brief Get a string value from a sac object */
int sac_get_string(sac *s, int hdr, char *v, size_t n);
/** @brief Set a string value in a sac object */
int sac_set_string(sac *s, int hdr, char *v);
/** @brief Get a integer value in a sac object */
int sac_get_int(sac *s, int hdr, int *v);
/** @brief Set a integer value in a sac object */
int sac_set_int(sac *s, int hdr, int v);
/** @brief  Check if sac header values are defined */
int sac_hdr_defined(sac *s, ...);
/** @brief  Format a string with sac header values */
int sac_fmt(char *dst, size_t n, const char *fmt, sac *s);
/** @brief  Get an absolute time from a sac object */
int sac_get_time(sac *s, int hdr, timespec64 *t);
/** @brief  Get an absolute reference time from a sac object */
int sac_get_time_ref(sac *s, timespec64 *t);
/** @brief  Set the reference time for a sac object */
int sac_set_time(sac *s, timespec64 t);
/** @brief  Get the size of a sac file in bytes */
off_t sac_size(sac *s);
/** @brief  Compute and set the begin and end time of a sac object */
void sac_be(sac *s);
/** @brief Set a time pick by number */
int sac_set_pick(sac *s, int n, double v);
/** @brief Get a time pick by number */
int sac_get_pick(sac *s, int n, double *v);
/** @brief Update the dist, az, baz, and gcarc header values */
void update_distaz(sac * s);
/** @brief Allocate space for data, either 1 or 2 components */
void sac_alloc(sac *s);
/** @brief Convert a time value to an index within the data array */
int sac_time_to_index(sac *s, double t);

int sac_compare(sac *s1, sac *s2, double tolerance, CheckByteOrderFlag byte_check, Verbose verbose);
int sac_compare_to_file(char *file, float *y, double tolerance, int byte_order, int verbose);

/** @brief Get corresponding spheroid from a sac ibody value */
Spheroid spheroid(int ibody);

#define SAC_WRITE_HEADER_AND_DATA 1 /**< @brief Write header and data */
#define SAC_READ_HEADER_AND_DATA  1 /**< @brief Read header and data */
#define SAC_WRITE_HEADER          0 /**< @brief Write only header */
#define SAC_READ_HEADER           0 /**< @brief Read only header */
#define SAC_NO_BYTESWAP_FILE      0 /**< @brief Do not byteswap file */
#define SAC_BYTESWAP_FILE         1 /**< @brief Byteswap the file  */

/** \endcond */

/**
 * @enum HeaderID
 * @brief Sac Header IDs
 * @public
 *
 * @memberof sac
 * @ingroup sac
 *
 */
enum HeaderID {
    SAC_DELTA        = 1, /**< @brief delta */
    SAC_DEPMIN       = 2, /**< @brief depmin */
    SAC_DEPMAX       = 3, /**< @brief depmax */
    SAC_SCALE        = 4, /**< @brief scale */
    SAC_ODELTA       = 5, /**< @brief odelta */
    SAC_B            = 6, /**< @brief b */
    SAC_E            = 7, /**< @brief e */
    SAC_O            = 8, /**< @brief o */
    SAC_A            = 9, /**< @brief a */
    SAC_FMT          = 10, /**< @brief fmt */
    SAC_T0           = 11, /**< @brief t0 */
    SAC_T1           = 12, /**< @brief t1 */
    SAC_T2           = 13, /**< @brief t2 */
    SAC_T3           = 14, /**< @brief t3 */
    SAC_T4           = 15, /**< @brief t4 */
    SAC_T5           = 16, /**< @brief t5 */
    SAC_T6           = 17, /**< @brief t6 */
    SAC_T7           = 18, /**< @brief t7 */
    SAC_T8           = 19, /**< @brief t8 */
    SAC_T9           = 20, /**< @brief t9 */
    SAC_F            = 21, /**< @brief f */
    SAC_RESP0        = 22, /**< @brief resp0 */
    SAC_RESP1        = 23, /**< @brief resp1 */
    SAC_RESP2        = 24, /**< @brief resp2 */
    SAC_RESP3        = 25, /**< @brief resp3 */
    SAC_RESP4        = 26, /**< @brief resp4 */
    SAC_RESP5        = 27, /**< @brief resp5 */
    SAC_RESP6        = 28, /**< @brief resp6 */
    SAC_RESP7        = 29, /**< @brief resp7 */
    SAC_RESP8        = 30, /**< @brief resp8 */
    SAC_RESP9        = 31, /**< @brief resp9 */
    SAC_STLA         = 32, /**< @brief stla */
    SAC_STLO         = 33, /**< @brief stlo */
    SAC_STEL         = 34, /**< @brief stel */
    SAC_STDP         = 35, /**< @brief stdp */
    SAC_EVLA         = 36, /**< @brief evla */
    SAC_EVLO         = 37, /**< @brief evlo */
    SAC_EVEL         = 38, /**< @brief evel */
    SAC_EVDP         = 39, /**< @brief evdp */
    SAC_MAG          = 40, /**< @brief mag */
    SAC_USER0        = 41, /**< @brief user0 */
    SAC_USER1        = 42, /**< @brief user1 */
    SAC_USER2        = 43, /**< @brief user2 */
    SAC_USER3        = 44, /**< @brief user3 */
    SAC_USER4        = 45, /**< @brief user4 */
    SAC_USER5        = 46, /**< @brief user5 */
    SAC_USER6        = 47, /**< @brief user6 */
    SAC_USER7        = 48, /**< @brief user7 */
    SAC_USER8        = 49, /**< @brief user8 */
    SAC_USER9        = 50, /**< @brief user9 */
    SAC_DIST         = 51, /**< @brief dist */
    SAC_AZ           = 52, /**< @brief az */
    SAC_BAZ          = 53, /**< @brief baz */
    SAC_GCARC        = 54, /**< @brief gcarc */
    SAC_SB           = 55, /**< @brief sb */
    SAC_SDELTA       = 56, /**< @brief sdelta */
    SAC_DEPMEN       = 57, /**< @brief depmen */
    SAC_CMPAZ        = 58, /**< @brief cmpaz */
    SAC_CMPINC       = 59, /**< @brief cmpinc */
    SAC_XMIN         = 60, /**< @brief xmin */
    SAC_XMAX         = 61, /**< @brief xmax */
    SAC_YMIN         = 62, /**< @brief ymin */
    SAC_YMAX         = 63, /**< @brief ymax */
    SAC_UN64         = 64, /**< @brief un @private */
    SAC_UN65         = 65, /**< @brief un @private */
    SAC_UN66         = 66, /**< @brief un @private */
    SAC_UN67         = 67, /**< @brief un @private */
    SAC_UN68         = 68, /**< @brief un @private */
    SAC_UN69         = 69, /**< @brief un @private */
    SAC_UN70         = 70, /**< @brief un @private */
    SAC_YEAR         = 71, /**< @brief year */
    SAC_DAY          = 72, /**< @brief day of the year */
    SAC_HOUR         = 73, /**< @brief hour */
    SAC_MIN          = 74, /**< @brief min */
    SAC_SEC          = 75, /**< @brief sec */
    SAC_MSEC         = 76, /**< @brief millisecond */
    SAC_HDR          = 77, /**< @brief header version*/
    SAC_ORID         = 78, /**< @brief origin id */
    SAC_EVID         = 79, /**< @brief event id */
    SAC_NPTS         = 80, /**< @brief npts  */
    SAC_NSNPTS       = 81, /**< @brief nsnpts */
    SAC_WFID         = 82, /**< @brief waveform id */
    SAC_NX           = 83, /**< @brief nx */
    SAC_NY           = 84, /**< @brief ny */
    SAC_UN85         = 85, /**< @brief un @private */
    SAC_FILE_TYPE    = 86, /**< @brief file type, iftype */
    SAC_DEP_TYPE     = 87, /**< @brief dependent variable type, idep */
    SAC_ZERO_TIME    = 88, /**< @brief zero time equivalent, iztype */
    SAC_UN89         = 89, /**< @brief un @private */
    SAC_INST_TYPE    = 90, /**< @brief instrument type, iinst */
    SAC_STREG        = 91, /**< @brief station region */
    SAC_EVREG        = 92, /**< @brief event region  */
    SAC_EVENT_TYPE   = 93, /**< @brief event type */
    SAC_QUAL         = 94, /**< @brief quality */
    SAC_SYNTH        = 95, /**< @brief synthetic flag */
    SAC_MAG_TYPE     = 96, /**< @brief magnitude type */
    SAC_MAG_SRC      = 97, /**< @brief magnitide src */
    SAC_BODY_TYPE    = 98, /**< @brief body / spheroid type */
    SAC_UN99         = 99, /**< @brief un @private */
    SAC_UN100        = 100, /**< @brief un @private */
    SAC_UN101        = 101, /**< @brief un @private */
    SAC_UN102        = 102, /**< @brief un @private */
    SAC_UN103        = 103, /**< @brief un @private */
    SAC_UN104        = 104, /**< @brief un @private */
    SAC_UN105        = 105, /**< @brief un @private */
    SAC_EVEN         = 106, /**< @brief evenly spaced, leven */
    SAC_POLARITY     = 107, /**< @brief polarity, lpspol */
    SAC_OVERWRITE    = 108, /**< @brief overwrite, lovrok */
    SAC_CALC_DIST_AZ = 109, /**< @brief calculate distance and azimuth, lcalda */
    SAC_UN110        = 110, /**< @brief un @private */
    SAC_STA          = 111, /**< @brief station */
    SAC_EVENT        = 112, /**< @brief event */
    SAC_EVENT2       = 113, /**< @brief un @private */
    SAC_HOLE         = 114, /**< @brief hole or location */
    SAC_KO           = 115, /**< @brief ko */
    SAC_KA           = 116, /**< @brief ka */
    SAC_KT0          = 117, /**< @brief kt0 */
    SAC_KT1          = 118, /**< @brief kt1 */
    SAC_KT2          = 119, /**< @brief kt2 */
    SAC_KT3          = 120, /**< @brief kt3 */
    SAC_KT4          = 121, /**< @brief kt4 */
    SAC_KT5          = 122, /**< @brief kt5 */
    SAC_KT6          = 123, /**< @brief kt6 */
    SAC_KT7          = 124, /**< @brief kt7 */
    SAC_KT8          = 125, /**< @brief kt8 */
    SAC_KT9          = 126, /**< @brief kt9 */
    SAC_KF           = 127, /**< @brief kf */
    SAC_KUSER0       = 128, /**< @brief kuser0 */
    SAC_KUSER1       = 129, /**< @brief kuser1 */
    SAC_KUSER2       = 130, /**< @brief kuser2 */
    SAC_CHA          = 131, /**< @brief channel */
    SAC_NET          = 132, /**< @brief newtowrk */
    SAC_DATRD        = 133, /**< @brief date data read */
    SAC_INST         = 134, /**< @brief instrument */

    SAC_DATE         = 135, /**< @brief kzdate  */
    SAC_TIME         = 136, /**< @brief kztime */
    SAC_STCMP        = 137, /**< @brief station component */
    SAC_FILENAME     = 138, /**< @brief filename */
    SAC_AMARKER      = 140, /**< @brief amarker */
    SAC_OMARKER      = 141, /**< @brief omarker */
    SAC_FMARKER      = 142, /**< @brief fmarker */
    SAC_T0MARKER     = 143, /**< @brief t0marker */
    SAC_T1MARKER     = 144, /**< @brief t1marker */
    SAC_T2MARKER     = 145, /**< @brief t2marker */
    SAC_T3MARKER     = 146, /**< @brief t3marker */
    SAC_T4MARKER     = 147, /**< @brief t4marker */
    SAC_T5MARKER     = 148, /**< @brief t5marker */
    SAC_T6MARKER     = 149, /**< @brief t6marker */
    SAC_T7MARKER     = 150, /**< @brief t7marker */
    SAC_T8MARKER     = 151, /**< @brief t8marker */
    SAC_T9MARKER     = 152, /**< @brief t9marker */

    SAC_MONTH        = 153, /**< @brief month */
    SAC_MONTH_DAY    = 154, /**< @brief day of the month */
    SAC_DATE_TIME    = 155, /**< @brief full date time references */
};

/**
 * @brief Structure to convert keyword to header value
 * @private
 */
struct hid {
       char *name;
       int  type;
       int  id;
};

/**
 * Type of values found around a Sac Header
 */
enum SacHeaderTypes {
    SAC_FLOAT_TYPE = 1, /**< @brief Floating point header type */
    SAC_INT_TYPE = 2, /**< @brief  Integer header type*/
    SAC_ENUM_TYPE = 3, /**< @brief  Enum header type */
    SAC_BOOL_TYPE = 4, /**< @brief  Logical header type */
    SAC_STRING_TYPE = 5, /**< @brief  String header type */
    SAC_LONG_STRING_TYPE = 6, /**< @brief Long String header type  */
    SAC_AUX_TYPE = 7, /**< @brief  Aux Type */
};

/**
 * @brief Structure to convert keyword to enum value
 */
struct eid {
    char *name;
    int type;
    int id;
};

/** \cond NO_DOCS */
/** @brief Convert a keyword to a header ID*/
struct hid * sac_keyword_to_header(register const char *str,
                                   register unsigned int len);
struct eid * sac_enum_to_id(register const char *str,
                            register unsigned int len);
/** \endcond */

#define SAC_KSTNM  SAC_STA   /**< @brief Alias for Station */
#define SAC_KNETWK SAC_NET   /**< @brief Alias for Network */
#define SAC_KHOLE  SAC_LOC   /**< @brief Alias for Location or Hole */
#define SAC_CHAN   SAC_CHA   /**< @brief Alias for Channel */
#define SAC_KCMPNM SAC_CHA   /**< @brief Alias for Channel */
#define SAC_KEVENT SAC_EVENT /**< @brief Alias for Event Name */
#define SAC_KEVNM SAC_EVENT  /**< @brief Alias for Event Name */
#define SAC_KINST  SAC_INST  /**< @brief Alias for Instrument */
#define SAC_LOC    SAC_HOLE  /**< @brief Alias for Location or Hole */


/**
 * definitions of constants for SAC enumerated data values 
*/
enum SacEnum {
IREAL  = 0,               /**< @brief  undocumented            @private  */
ITIME  = 1,               /**< @brief  sac_hdr.iftype : time series data */
IRLIM  = 2,               /**< @brief  sac_hdr.iftype : real&imag spectrum */
IAMPH  = 3,               /**< @brief  sac_hdr.iftype : ampl&phas spectrum */
IXY    = 4,               /**< @brief  sac_hdr.iftype : gen'l x vs y data */
IXYZ  = 51,               /**< @brief  sac_hdr.iftype : General XYZ (3-D) file */

IUNKN  = 5,               /**< @brief  x data: unknown type
                                 * zero time: unknown
                                 * event type: unknown
                                 @private
                                 */
IDISP  = 6,               /**< @brief  sac_hdr.idep : displacement */
IVEL   = 7,               /**< @brief  sac_hdr.idep : velocity */
IACC   = 8,               /**< @brief  sac_hdr.idep : acceleration */
IRLDTA= 49,               /**< @brief  sac_hdr.idep : data is real data         */
IVOLTS= 50,               /**< @brief  sac_hdr.idep : velocity (volts)    */

IB     = 9,               /**< @brief  sac_hdr.iztype : start of file  */
IDAY  = 10,               /**< @brief  sac_hdr.iztype : 0000 of GMT day  */
IO    = 11,               /**< @brief  sac_hdr.iztype : event origin  */
IA    = 12,               /**< @brief  sac_hdr.iztype : 1st arrival  */
IT0   = 13,               /**< @brief  sac_hdr.iztype : user timepick 0  */
IT1   = 14,               /**< @brief  sac_hdr.iztype : user timepick 1  */
IT2   = 15,               /**< @brief  sac_hdr.iztype : user timepick 2  */
IT3   = 16,               /**< @brief  sac_hdr.iztype : user timepick 3  */
IT4   = 17,               /**< @brief  sac_hdr.iztype : user timepick 4  */
IT5   = 18,               /**< @brief  sac_hdr.iztype : user timepick 5  */
IT6   = 19,               /**< @brief  sac_hdr.iztype : user timepick 6  */
IT7   = 20,               /**< @brief  sac_hdr.iztype : user timepick 7  */
IT8   = 21,               /**< @brief  sac_hdr.iztype : user timepick 8  */
IT9   = 22,               /**< @brief  sac_hdr.iztype : user timepick 9  */
IRADNV= 23,               /**< @brief  undocumented @private               */
ITANNV= 24,               /**< @brief  undocumented @private               */
IRADEV= 25,               /**< @brief  undocumented @private               */
ITANEV= 26,               /**< @brief  undocumented @private               */
INORTH= 27,               /**< @brief  undocumented @private               */
IEAST = 28,               /**< @brief  undocumented @private               */
IHORZA= 29,               /**< @brief  undocumented @private               */
IDOWN = 30,               /**< @brief  undocumented @private               */
IUP   = 31,               /**< @brief  undocumented @private               */
ILLLBB= 32,               /**< @brief  undocumented @private               */
IWWSN1= 33,               /**< @brief  undocumented @private               */
IWWSN2= 34,               /**< @brief  undocumented @private               */
IHGLP = 35,               /**< @brief  undocumented @private               */
ISRO  = 36,               /**< @brief  undocumented @private               */

/* Source types */
INUCL = 37,               /**< @brief  sac_hdr.ievtyp nuclear shot */
IPREN = 38,               /**< @brief  sac_hdr.ievtyp nuke pre-shot */
IPOSTN= 39,               /**< @brief  sac_hdr.ievtyp nuke post-shot */
IQUAKE= 40,               /**< @brief  sac_hdr.ievtyp earthquake    */
IPREQ = 41,               /**< @brief  sac_hdr.ievtyp foreshock     */
IPOSTQ= 42,               /**< @brief  sac_hdr.ievtyp aftershock    */
ICHEM = 43,               /**< @brief  sac_hdr.ievtyp chemical expl */
IOTHER= 44,               /**< @brief  sac_hdr.ievtyp other source  */

/* Data Quality  */
IGOOD = 45,               /**< @brief  sac_hdr.iqual : good        */
IGLCH = 46,               /**< @brief  sac_hdr.iqual : has glitches */
IDROP = 47,               /**< @brief  sac_hdr.iqual : has dropouts */
ILOWSN= 48,               /**< @brief  sac_hdr.iqual : low s/n     */

/* Magnitude type and source */
IMB   = 52,               /**< @brief sac_hdr.imagtyp Bodywave Magnitude */
IMS   = 53,               /**< @brief sac_hdr.imagtyp Surface Magnitude */
IML   = 54,               /**< @brief sac_hdr.imagtyp Local Magnitude  */
IMW   = 55,               /**< @brief sac_hdr.imagtyp Moment Magnitude */
IMD   = 56,               /**< @brief sac_hdr.imagtyp Duration Magnitude */
IMX   = 57,               /**< @brief sac_hdr.imagtyp User Defined Magnitude */
INEIC = 58,               /**< @brief sac_hdr.imagsrc INEIC */
IPDEQ = 59,               /**< @brief sac_hdr.imagsrc IPDE */
IPDEW = 60,               /**< @brief sac_hdr.imagsrc IPDE */
IPDE  = 61,               /**< @brief sac_hdr.imagsrc IPDE */

IISC   =62,               /**< @brief sac_hdr.imagsrc IISC */
IREB   =63,               /**< @brief sac_hdr.imagsrc IREB */
IUSGS  =64,               /**< @brief sac_hdr.imagsrc IUSGS */
IBRK   =65,               /**< @brief sac_hdr.imagsrc IBRK */
ICALTECH= 66,             /**< @brief sac_hdr.imagsrc ICALTECH */
ILLNL  =67,               /**< @brief sac_hdr.imagsrc ILLNL */
IEVLOC =68,               /**< @brief sac_hdr.imagsrc IEVLOC */
IJSOP  =69,               /**< @brief sac_hdr.imagsrc IJSOP */
IUSER  =70,               /**< @brief sac_hdr.imagsrc IUSER */
IUNKNOWN= 71,             /**< @brief sac_hdr.imagsrc IUNKNOWN */

IQB   = 72,               /**< @brief sac_hdr.ievtyp Quarry Blast or mine expl. confirmed by quarry */
IQB1  = 73,               /**< @brief sac_hdr.ievtyp Quarry or mine blast with designed shot information-ripple fired */
IQB2  = 74,               /**< @brief sac_hdr.ievtyp Quarry or mine blast with observed shot information-ripple fired */
IQBX  = 75,               /**< @brief sac_hdr.ievtyp Quarry or mine blast - single shot */
IQMT  = 76,               /**< @brief sac_hdr.ievtyp Quarry or mining-induced events: tremors and rockbursts */
IEQ   = 77,               /**< @brief sac_hdr.ievtyp Earthquake */
IEQ1  = 78,               /**< @brief sac_hdr.ievtyp Earthquakes in a swarm or aftershock sequence */
IEQ2  = 79,               /**< @brief sac_hdr.ievtyp Felt earthquake */
IME   = 80,               /**< @brief sac_hdr.ievtyp Marine explosion */
IEX   = 81,               /**< @brief sac_hdr.ievtyp Other explosion */
INU   = 82,               /**< @brief sac_hdr.ievtyp Nuclear explosion */
INC   = 83,               /**< @brief sac_hdr.ievtyp Nuclear cavity collapse */
IO_   = 84,               /**< @brief sac_hdr.ievtyp Other source of known origin */
IL    = 85,               /**< @brief sac_hdr.ievtyp Local event of unknown origin */
IR    = 86,               /**< @brief sac_hdr.ievtyp Regional event of unknown origin */
IT    = 87,               /**< @brief sac_hdr.ievtyp Teleseismic event of unknown origin */
IU    = 88,               /**< @brief sac_hdr.ievtyp Undetermined or conflicting information  */
IEQ3  = 89,               /**< @brief sac_hdr.ievtyp Damaging earthquake */
IEQ0  = 90,               /**< @brief sac_hdr.ievtyp Probable earthquake */
IEX0  = 91,               /**< @brief sac_hdr.ievtyp Probable explosion */
IQC   = 92,               /**< @brief sac_hdr.ievtyp Mine collapse */
IQB0  = 93,               /**< @brief sac_hdr.ievtyp Probable Mine Blast */
IGEY  = 94,               /**< @brief sac_hdr.ievtyp Geyser */
ILIT  = 95,               /**< @brief sac_hdr.ievtyp Light */
IMET  = 96,               /**< @brief sac_hdr.ievtyp Meteoric Event */
IODOR = 97,               /**< @brief sac_hdr.ievtyp Odors */

ISUN     = 98,            /**< @brief sac_hdr.body Sun */
IMERCURY = 99,            /**< @brief sac_hdr.body Mercury */
IVENUS   = 100,           /**< @brief sac_hdr.body Venus */
IEARTH   = 101,           /**< @brief sac_hdr.body Earth */
IMOON    = 102,           /**< @brief sac_hdr.body Moon */
IMARS    = 103            /**< @brief sac_hdr.body Mars */

/* Values from 200 - 299 reserved for future astronomical body definitions */

};


#ifdef SAC_NULL_HEADER_REQUIRED
/* a SAC structure containing all null values */

static struct sac_hdr NullSacHeader = {
    -12345., -12345., -12345., -12345., -12345.,
    -12345., -12345., -12345., -12345., -12345.,
    -12345., -12345., -12345., -12345., -12345.,
    -12345., -12345., -12345., -12345., -12345.,
    -12345., -12345., -12345., -12345., -12345.,
    -12345., -12345., -12345., -12345., -12345.,
    -12345., -12345., -12345., -12345., -12345.,
    -12345., -12345., -12345., -12345., -12345.,
    -12345., -12345., -12345., -12345., -12345.,
    -12345., -12345., -12345., -12345., -12345.,
    -12345., -12345., -12345., -12345., -12345.,
    -12345., -12345., -12345., -12345., -12345.,
    -12345., -12345., -12345., -12345., -12345.,
    -12345., -12345., -12345., -12345., -12345.,
    -12345, -12345, -12345, -12345, -12345,
    -12345, -12345, -12345, -12345, -12345,
    -12345, -12345, -12345, -12345, -12345,
    -12345, -12345, -12345, -12345, -12345,
    -12345, -12345, -12345, -12345, -12345,
    -12345, -12345, -12345, -12345, -12345,
    -12345, -12345, -12345, -12345, -12345,
    -12345, -12345, -12345, -12345, -12345,
    {'-', '1', '2', '3', '4', '5', ' ', ' '},
    {'-', '1', '2', '3', '4', '5', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
     ' ', ' '},
    {'-', '1', '2', '3', '4', '5', ' ', ' '}, {'-', '1', '2', '3', '4',
                                               '5', ' ', ' '},
    {'-', '1', '2', '3', '4', '5', ' ', ' '}, {'-', '1', '2', '3', '4',
                                               '5', ' ', ' '},
    {'-', '1', '2', '3', '4', '5', ' ', ' '}, {'-', '1', '2', '3', '4',
                                               '5', ' ', ' '},
    {'-', '1', '2', '3', '4', '5', ' ', ' '}, {'-', '1', '2', '3', '4',
                                               '5', ' ', ' '},
    {'-', '1', '2', '3', '4', '5', ' ', ' '}, {'-', '1', '2', '3', '4',
                                               '5', ' ', ' '},
    {'-', '1', '2', '3', '4', '5', ' ', ' '}, {'-', '1', '2', '3', '4',
                                               '5', ' ', ' '},
    {'-', '1', '2', '3', '4', '5', ' ', ' '}, {'-', '1', '2', '3', '4',
                                               '5', ' ', ' '},
    {'-', '1', '2', '3', '4', '5', ' ', ' '}, {'-', '1', '2', '3', '4',
                                               '5', ' ', ' '},
    {'-', '1', '2', '3', '4', '5', ' ', ' '}, {'-', '1', '2', '3', '4',
                                               '5', ' ', ' '},
    {'-', '1', '2', '3', '4', '5', ' ', ' '}, {'-', '1', '2', '3', '4',
                                               '5', ' ', ' '},
    {'-', '1', '2', '3', '4', '5', ' ', ' '}
};
#endif /* SAC_NULL_HEADER_REQUIRED */

/**
 * @brief Region Conversion
 * @private
 */
#define REGCONV    100


/**
 * @brief  X-Macro v7 floating point header values, in order
 * @private
 */
#define SAC_F64           \
    X(DELTA, _delta)       \
    X(B, _b)               \
    X(E, _e)               \
    X(O, _o)               \
    X(A, _a)               \
    X(T0, _t0)             \
    X(T1, _t1)             \
    X(T2, _t2)             \
    X(T3, _t3)             \
    X(T4, _t4)             \
    X(T5, _t5)             \
    X(T6, _t6)             \
    X(T7, _t7)             \
    X(T8, _t8)             \
    X(T9, _t9)             \
    X(F, _f)               \
    X(EVLO, _evlo)         \
    X(EVLA, _evla)         \
    X(STLO, _stlo)         \
    X(STLA, _stla)         \
    X(SB, _sb)             \
    X(SDELTA, _sdelta)             \

/**
 * @brief  X-Macro v6 floating point header values, in order
 * @private
 */
#define SAC_F32           \
    X(DELTA,_delta)        \
    X(DEPMIN, depmin)     \
    X(DEPMAX, depmax)     \
    X(SCALE, scale)       \
    X(ODELTA, odelta)     \
    X(B, _b)               \
    X(E, _e)               \
    X(O, _o)               \
    X(A, _a)               \
    X(FMT, fmt)           \
    X(T0, _t0)             \
    X(T1, _t1)             \
    X(T2, _t2)             \
    X(T3, _t3)             \
    X(T4, _t4)             \
    X(T5, _t5)             \
    X(T6, _t6)             \
    X(T7, _t7)             \
    X(T8, _t8)             \
    X(T9, _t9)             \
    X(F, _f)               \
    X(RESP0, resp0)       \
    X(RESP1, resp1)       \
    X(RESP2, resp2)       \
    X(RESP3, resp3)       \
    X(RESP4, resp4)       \
    X(RESP5, resp5)       \
    X(RESP6, resp6)       \
    X(RESP7, resp7)       \
    X(RESP8, resp8)       \
    X(RESP9, resp9)       \
    X(STLA, _stla)         \
    X(STLO, _stlo)         \
    X(STEL, stel)         \
    X(STDP, stdp)         \
    X(EVLA, _evla)         \
    X(EVLO, _evlo)         \
    X(EVEL, evel)         \
    X(EVDP, evdp)         \
    X(MAG, mag)           \
    X(USER0, user0)       \
    X(USER1, user1)       \
    X(USER2, user2)       \
    X(USER3, user3)       \
    X(USER4, user4)       \
    X(USER5, user5)       \
    X(USER6, user6)       \
    X(USER7, user7)       \
    X(USER8, user8)       \
    X(USER9, user9)       \
    X(DIST, dist)         \
    X(AZ, az)             \
    X(BAZ, baz)           \
    X(GCARC, gcarc)       \
    X(SB, _sb)             \
    X(SDELTA, _sdelta)     \
    X(DEPMEN, depmen)     \
    X(CMPAZ, cmpaz)       \
    X(CMPINC, cmpinc)     \
    X(XMIN, xminimum) \
    X(XMAX, xmaximum) \
    X(YMIN, yminimum) \
    X(YMAX, ymaximum) \
    X(UN64, unused6)   \
    X(UN65, unused7)   \
    X(UN66, unused8)   \
    X(UN67, unused9)   \
    X(UN68, unused10) \
    X(UN69, unused11) \
    X(UN70, unused12)

#define ERROR_CANT_CUT_SPECTRAL_FILE        1321     /**< @brief Error cutting spectral file */
#define ERROR_CUT_TIMES_BEYOND_DATA_LIMITS  13241325 /**< @brief Cut times are beyond data limits: 1324 and 1325 */
#define ERROR_START_TIME_LESS_THAN_BEGIN    1324     /**< @brief Cut start time is less than begin value */
#define ERROR_STOP_TIME_GREATER_THAN_END    1325     /**< @brief Cut stop time is greater than end value */
#define ERROR_START_TIME_GREATER_THAN_END   1326     /**< @brief Cut start time is greater than end value */
#define ERROR_STOP_TIME_LESS_THAN_BEGIN     1327     /**< @brief Cut stop time is less than begin value */
#define ERROR_START_TIME_GREATER_THAN_STOP  1328     /**< @brief Cut time time is greater than stop value */
#define ERROR_CANT_CUT_UNEVENLY_SPACED_FILE 1356     /**< @brief Error cuting unevenly spaced file */
#define ERROR_READING_CARD_IMAGE_HEADER     1319     /**< @brief Error reading an sac alphanumeric file */

#endif /* __SACIO_H__ */

