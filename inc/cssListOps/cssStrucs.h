/** 
 * @file   cssStrucs.h
 *
 * @brief  CSS Format
 * 
 */

#ifndef _CSS_STRUCS_H_
#define _CSS_STRUCS_H_

#define CALIB_UNDEF   0.0
#define CALPER_UNDEF -1.0

/** 
 * @struct affiliation
 *    Affiliation
 */
struct affiliation{
  char	net[9];	
  char	sta[7];
  char	lddate[18];
  int    wfid;       /*Used going from SAC to CSS when missing keys. */
};

/** 
 * @struct affiliationList
 *    List of Affiliations
 */
struct affiliationList{
  struct affiliation *element;	
  struct affiliationList *prev;
  struct affiliationList *next;
  int index;
};

#ifdef NULL_AFFILIATION
/* Null affiliation structure. */
static struct affiliation nullAffiliation =
{
	"-",				/* net     */
	"-",				/* sta     */
	"-",				/* lddate  */
	  0,                            /* wfid    */
}; 
#endif
/** 
 * @struct arrival
 *    Arrivals
 */
struct arrival {
	char	sta[7];
	double	time;
	int	arid;
	int	jdate;
	int	stassid;
	int	chanid;
	char	chan[9];
	char	iphase[9];
	char	stype[2];
	double	deltim;
	double	azimuth;
	double	delaz;
	double	slow;
	double	delslo;
	double	ema;
	double	rect;
	double	amp;
	double	per;
	double	logat;
	char	clip[2];
	char	fm[3];
	double	snr;
	char	qual[2];
	char	auth[16];
	int	commid;
	char	lddate[18];
	char    SACfield[3];
	int    wfid;       /*Used going from SAC to CSS when missing keys. */
};

/** 
 * @struct arrivalList
 *    List of Arrivals 
 */
struct arrivalList{
  struct arrival *element;
  struct arrivalList *prev;
  struct arrivalList *next;
  int index;
};

#ifdef NULL_ARRIVAL
/* Null arrival structure. */
static struct arrival nullArrival =
{
        "-",                            /* sta     */
        -9999999999.999,                /* time */
        -1,                             /* arid */
        -1,                             /* jdate */
        -1,                             /* stassid */
        -1,                             /* chanid */
        "-",                            /* chan */
        "-",                            /* iphase */
        "-",                            /* stype */
        -1,                             /* deltim */
        -1,                             /* azimuth */
        -1,                             /* delaz */
        -1,                             /* slow */
        -1,                             /* delslo */
        -1,                             /* ema */
        -1,                             /* rect */
        -1,                             /* amp */
        -1,                             /* per */
        -1,                             /* logat */
        "-",                            /* clip */
        "-",                            /* fm */
        -1,                             /* snr */
        "-",                            /* qual */
        "-",                            /* auth */
        -1,                             /*commid */
        "-",                            /* lddate  */
        "-",                            /* SACfield  */
	  0,                            /* wfid    */
}; 
#endif

/** 
 * @struct assoc
 *    Associations
 */
struct assoc {
	int	arid;
	int	orid;
	char	sta[7];
	char	phase[9];
	double	belief;
	double	delta;
	double	seaz;
	double	esaz;
	double	timeres;
	char	timedef[2];
	double	azres;
	char	azdef[2];
	double	slores;
	char	slodef[2];
	double	emares;
	double	wgt;
	char	vmodel[16];
	int	commid;
	char	lddate[18];
	int    wfid;       /*Used going from SAC to CSS when missing keys. */
};

/** 
 * @struct assoc
 *    List of Associations
 */
struct assocList{
        struct assoc *element;
        struct assocList *prev;
        struct assocList *next;
        int index;
};

#ifdef NULL_ASSOC
static struct assoc nullAssoc =
{
        -1,	                              /* arid */
	-1,	                              /* orid */
	"-",	                              /* sta */
	"-",	                              /* phase */
	-1,	                              /* belief */
	-1,	                              /* delta */
	-999.0,	                              /* seaz */
	-999.0,	                              /* esaz */
	-999.0,	                              /* timeres */
	"-",	                              /* timedef */
	-999.0,	                              /* azres */
	"-",	                              /* azdef */
	-99999.0,	                      /* slores */
	"-",	                              /* slodef */
	-999.0,	                              /* emares */
	-1,	                              /* wgt */
	"-",	                              /* vmodel */
	-1,	                              /* commid */
	"-",	                              /* lddate */
	  0,                                  /* wfid    */
};
#endif
/** 
 * @struct event
 *    Events
 */
struct event {
	int	evid;
	char	evname[16];
	int	prefor;
	char	auth[16];
	int	commid;
	char	lddate[18];
	int    wfid;       /*Used going from SAC to CSS when missing keys. */
};

/** 
 * @struct eventList
 *    List of Events
 */
struct eventList{
        struct event *element;
        struct eventList *prev;
        struct eventList *next;
        int index;
};

#ifdef NULL_EVENT
static struct event nullEvent =
{
	-1,	                          /* evid */
	"-",	                          /* evname */
	-1,	                          /* prefor */
	"-",	                          /* auth */
	-1,	                          /* commid */
	"-",	                          /* lddate */
	  0,                              /* wfid    */
};
#endif
/** 
 * @struct gregion
 *    Region
 */
struct gregion {
	int	grn;
	char	grname[41];
	char	lddate[18];
	int    wfid;       /*Used going from SAC to CSS when missing keys. */
};

/** 
 * @struct gregionList
 *    List of Regions
 */
struct gregionList{
        struct gregion *element;
        struct gregionList *prev;
        struct gregionList *next;
        int index;
};

#ifdef NULL_GREGION
static struct gregion nullGregion =
{
	-1,	                         /* grn */
	"-",	                         /* grname */
	"-",	                         /* lddate */
	  0,                             /* wfid    */
};
#endif
/** 
 * @struct instrument
 *    Instruments
 */
struct instrument {
	int	inid;
	char	insname[51];
	char	instype[7];
	char	band[2];
	char	digital[2];
	double	samprate;
	double	ncalib;
	double	ncalper;
	char	dir[65];
	char	dfile[33];
	char	rsptype[7];
	char	lddate[18];
	int    wfid;       /*Used going from SAC to CSS when missing keys. */
};

/** 
 * @struct instrumentList
 *    List of Instruments
 */
struct instrumentList{
        struct instrument *element;
        struct instrumentList *prev;
        struct instrumentList *next;
        int index;
};

#ifdef NULL_INSTRUMENT
static struct instrument nullInstrument =
{
	-1,	                         /* inid */
	"-",	                         /* insname */
	"-",	                         /* instype */
	"-",	                         /* band */
	"-",	                         /* digital */
	-1,	                         /* samprate */
	-1,	                         /* ncalib */
	-1,	                         /* ncalper */
	"-",	                         /* dir */
	"-",    	                 /* dfile */
	"-",	                         /* rsptype */
	"-",	                         /* lddate */
	  0,                             /* wfid    */
};
#endif

/** 
 * @struct origerr
 *    Origin Error
 */
struct origerr {
	int	orid;
	double	sxx;
	double	syy;
	double	szz;
	double	stt;
	double	sxy;
	double	sxz;
	double	syz;
	double	stx;
	double	sty;
	double	stz;
	double	sdobs;
	double	smajax;
	double	sminax;
	double	strike;
	double	sdepth;
	double	stime;
	double	conf;
	int	commid;
	char	lddate[18];
	int    wfid;       /*Used going from SAC to CSS when missing keys. */
};

/** 
 * @struct origerrList
 *    List of origin errors
 */
struct origerrList{
        struct origerr *element;
        struct origerrList *prev;
        struct origerrList *next;
        int index;
};

#ifdef NULL_ORIGIN_ERROR
static struct origerr nullOrigerr =
{
	-1,	                       /* orid */
	-1,	                       /* sxx */
	-1,	                       /* syy */
	-1,	                       /* szz */
	-1,	                       /* stt */
	-1,	                       /* sxy */
	-1,	                       /* sxz */
	-1,	                       /* syz */
	-1,	                       /* stx */
	-1,	                       /* sty */
	-1,	                       /* stz */
	-1,	                       /* sdobs */
	-1,	                       /* smajax */
	-1,	                       /* sminax */
	-1,	                       /* strike */
	-1,	                       /* sdepth */
	-1,	                       /* stime */
	-1,	                       /* conf */
	-1,	                       /* commid */
	"-",	                       /* lddate */
	  0,                           /* wfid    */
};
#endif

/** 
 * @struct origin
 *   Origin 
 */
struct origin {
	double	lat;
	double	lon;
	double	depth;
	double	time;
	int	orid;
	int	evid;
	int	jdate;
	int	nass;
	int	ndef;
	int	ndp;
	int	grn;
	int	srn;
	char	etype[8];
	double	depdp;
	char	dtype[2];
	double	mb;
	int	mbid;
	double	ms;
	int	msid;
	double	ml;
	int	mlid;
	char	algorithm[16];
	char	auth[16];
	int	commid;
	char	lddate[18];
	int    wfid;       /*Used going from SAC to CSS when missing keys. */
};

/** 
 * @struct originList
 *   List of Origin 
 */
struct originList{
        struct origin *element;
        struct originList *prev;
        struct originList *next;
        int index;
};

#ifdef NULL_ORIGIN
/* Null origin structure. */
static struct origin nullOrigin =
{
        -999.0,                 /* lat       */
        -999.0,                 /* lon       */
        -999.0,                 /* depth     */
        -9999999999.999,        /* time      */
        -1,                     /* orid      */
        -1,                     /* evid      */
        -1,                     /* jdate     */
        -1,                     /* nass      */
        -1,                     /* ndef      */
        -1,                     /* ndp       */
        -1,                     /* grn       */
        -1,                     /* srn       */
        "-",                    /* etype     */
        -999.0,                 /* depdp     */
        "-",                    /* dtype     */
        -999.0,                 /* mb        */
        -1,                     /* mbid      */
        -999.0,                 /* ms        */
        -1,                     /* msid      */
        -999.0,                 /* ml        */
        -1,                     /* mlid      */
        "-",                    /* algorithm */
        "-",                    /* auth      */
        -1,                     /* commid    */
        "-",                    /* lddate    */
	  0,                    /* wfid    */
};
#endif

struct remark {
	int	commid;
	int	lineno;
	char	remark[81];
	char	lddate[18];
};

struct remarkList{
        struct remark *element;
        struct remarkList *prev;
        struct remarkList *next;
        int index;
};

#ifdef NULL_REMARK
static struct remark nullRemark =
{
	-1,	                   /* commid */
	-1,	                   /* lineno */
	"-",	                   /* remark */
	"-",	                   /* lddate */
};
#endif
struct sensor {
	char	sta[7];
	char	chan[9];
	double	time;
	double	endtime;
	int	inid;
	int	chanid;
	int	jdate;
	double	calratio;
	double	calper;
	double	tshift;
	char	instant[2];
	char	lddate[18];
	int    wfid;       /*Used going from SAC to CSS when missing keys. */
};

struct sensorList{
        struct sensor *element;
        struct sensorList *prev;
        struct sensorList *next;
        int index;
};

#ifdef NULL_SENSOR
static struct sensor nullSensor =
{
	"-",	                     /* sta */
	"-",	                     /* chan */
        -9999999999.999,             /* time      */
	9999999999.999,              /* endtime */
	-1,	                     /* inid */
	-1,	                     /* chanid */
	-1, 	                     /* jdate */
	-1,	                     /* calratio */
	-1,	                     /* calper */
	-1, 	                     /* tshift */
	"-",	                     /* instant */
	"-",	                     /* lddate */
	  0,                         /* wfid    */
};
#endif

struct site {
	char	sta[7];
	int	ondate;
	int	offdate;
	double	lat;
	double	lon;
	double	elev;
	char	staname[51];
	char	statype[5];
	char	refsta[7];
	double	dnorth;
	double	deast;
	char	lddate[18];
	int    wfid;       /*Used going from SAC to CSS when missing keys. */
};

struct siteList{
        struct site *element;
        struct siteList *prev;
        struct siteList *next;
        int index;
};

#ifdef NULL_SITE
static struct site nullSite =
{
	"-",	                       /* sta */
	-1,	                       /* ondate */
	-1,	                       /* offdate */
	-999.0,	                       /* lat */
	-999.0,	                       /* lon */
	-999.0,	                       /* elev */
	"-", 	                       /* staname */
	"-", 	                       /* statype */
	"-",	                       /* refsta */
	0.0, 	                       /* dnorth */
	0.0,	                       /* deast */
	"-",	                       /* lddate */
	  0,                           /* wfid    */
};
#endif

struct sitechan {
	char	sta[7];
	char	chan[9];
	int	ondate;
	int	chanid;
	int	offdate;
	char	ctype[5];
	double	edepth;
	double	hang;
	double	vang;
	char	descrip[51];
	char	lddate[18];
	int    wfid;       /*Used going from SAC to CSS when missing keys. */
};

struct sitechanList{
        struct sitechan *element;
        struct sitechanList *prev;
        struct sitechanList *next;
        int index;
};

#ifdef NULL_SITECHAN
static struct sitechan nullSitechan =
{
	"-",	                   /* sta */
	"-",	                   /* chan */
	-1,	                   /* ondate */
	-1,	                   /* chanid */
	-1,	                   /* offdate */
	"-",	                   /* ctype */
	0.0,	                   /* edepth */
	-9999999999.999,	   /* hang */
	-9999999999.999,	   /* vang */
	"-",	                   /* descrip */
	"-",	                   /* lddate */
	  0,                       /* wfid    */
};
#endif

struct stassoc {
	int	stassid;
	char	sta[7];
	char	etype[8];
	char	location[33];
	double	dist;
	double	azimuth;
	double	lat;
	double	lon;
	double	depth;
	double	time;
	double	imb;
	double	ims;
	double	iml;
	char	auth[16];
	int	commid;
	char	lddate[18];
	int    wfid;       /*Used going from SAC to CSS when missing keys. */
};

struct stassocList{
        struct stassoc *element;
        struct stassocList *prev;
        struct stassocList *next;
        int index;
};

#ifdef NULL_STASSOC
static struct stassoc nullStassoc =
{
	-1,	                       /* stassid */
	"-",	                       /* sta */
	"-",	                       /* etype */
	"-",	                       /* location */
	-1,	                       /* dist */
	-1,	                       /* azimuth */
	-999.0,	                       /* lat */
	-999.0,	                       /* lon */
	-999.0,	                       /* depth */
	-9999999999.999,	       /* time */
	-999.0, 	                       /* imb */
	-999.0,	                       /* ims */
	-999.0,	                       /* iml */
	"-",	                       /* auth */
	-1,	                       /* commid */
	"-",	                       /* lddate */
	  0,                           /* wfid    */
};
#endif

struct wfdisc
{
        char    sta[8];
        char    chan[9];
        double  time;
        int    wfid;
        int    chanid;
        int    jdate;
        double  endtime;
        int    nsamp;
        float   samprate;
        float   calib;
        float   calper;
        char    instype[7];
        char    segtype[2];
        char    dattype[3];
        char    clip[2];
        char    dir[65];
        char    dfile[33];
        int    foff;
        int    commid;
        char    lddate[18];
};

struct trace{
   float *r;
   float *i;
   int Cmplx;
};
	
struct wfdiscList{
        struct wfdisc *element;
        struct wfdiscList *prev;
        struct wfdiscList *next;
        struct trace *seis;
        int index;
};

#ifdef NULL_WFDISC
/* Null wfdisc structure. */
static struct wfdisc wfdisc_null =
{
        "-",                            /* sta     */
        "-",                            /* chan    */
        -9999999999.999,                /* time    */
        -1,                             /* wfid    */
        -1,                             /* chanid  */
        -1,                             /* jdate   */
        9999999999.999,                 /* endtime */
        -1,                             /* nsamp   */
        -1,                             /* samprate*/
        CALIB_UNDEF,                    /* calib   */
        CALPER_UNDEF,                   /* calper  */
        "-",                            /* instype */
        "-",                            /* segtype */
        "-",                            /* dattype */
        "-",                            /* clip    */
        ".",                            /* dir     */
        "-",                            /* dfile   */
        -1,                             /* foff    */
        -1,                             /* commid  */
        "-",                            /* lddate  */
}; 
#endif

struct wftag {
	char	tagname[9];
	int	tagid;
	int	wfid;
	char	lddate[18];
};

struct wftagList{
        struct wftag *element;
        struct wftagList *prev;
        struct wftagList *next;
        int index;
};

#ifdef NULL_WFTAG
static struct wftag nullWftag =
{
	"-",	                   /* key (arid, orid, evid) */
	-1,	                   /* tagname value */
	-1,	                   /* waveform ID */
	"-",	                   /* lddate */
};
#endif

struct UD{
   float value[10];
   char  label[3][9];
};   

struct sacdata
{
   struct UD userdata;
   int synthflag;
   int lpspol;
   int iztype;
   int idep;
   int iftype;
   int wfid;
   int nsnpts;
   int nxsize;
   int nysize;
   int leven;
   float fmt;
   float sb;
   float sdelta;
   float xminimum;
   float xmaximum;
   float yminimum;
   float ymaximum;
   
   float odelta;
   float resp[10];
   float evel;
   float az;
   float baz;
   float gcarc;
   float dist;
   int iinst;
   int istreg;
   int iqual;
   int lovrok;
   int lcalda;
   char khole[9];
   char ko[9];
   char kdatrd[9];
};

	
struct sacdataList{
        struct sacdata *element;
        struct sacdataList *prev;
        struct sacdataList *next;
        int index;
};


#ifdef NULL_SAC_DATA
/* Null sacdata structure. */
static struct sacdata nullSacdata =
{
   { {-12345, -12345, -12345, -12345, -12345, -12345, -12345, -12345, -12345, -12345},
     {"-12345", "-12345", "-12345"} },
    -12345,
    -12345,
    -12345,
    -12345,
    -12345,
    0,
    -12345,
    -12345,
    -12345,
    -12345,
    -12345.0,
    -12345.0,
    -12345.0,
    -12345.0,
    -12345.0,
    -12345.0,
    -12345.0,
    -12345.0,
    {-12345.0, -12345.0, -12345.0, -12345.0, -12345.0, -12345.0, -12345.0, -12345.0,
     -12345.0, -12345.0},
    -12345.0,
    -12345.0,
    -12345.0,
    -12345.0,
    -12345.0,
    -12345,
    -12345,
    -12345,
    -12345,
    -12345,
    "-12345",
    "-12345",
    "-12345"
}; 
#endif



#endif 
