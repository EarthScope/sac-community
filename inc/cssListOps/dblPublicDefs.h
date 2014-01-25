
#ifndef _DBL_PUBLIC_DEFS_
#define _DBL_PUBLIC_DEFS_

#ifndef OverWriteUserData
#define OverWriteUserData TRUE
#endif
#ifndef AppendToUserData
#define AppendToUserData FALSE
#endif

   
/* The access functions return void * pointers to db lists, table instances,
   and data objects. To clarify the function prototypes, the following
   typedefs are used: */
   
typedef void * DBobj;
typedef void * DBlist;
typedef void * DBtable;
typedef void * DBelement;
typedef void * DBresult;
typedef void * DBvalue;
typedef void * DBdataComm;
typedef void * DBreal;
typedef void * DBimag;
typedef void * DBcomplex;


/* To specify which object is to be accessed we need specifiers for
   every object type that can be accessed. The following enumeration
   contains entries for lists, relations, and attributes.*/
   
   
enum dblObjects {
	dbl_DBLIST,			/*List Object */

/* These are the specifiers for the relations */	
	dbl_LIST_AFFILIATION,		/* Affiliation Relation */
	dbl_LIST_ARRIVAL,		/* Arrival Relation */
	dbl_LIST_ASSOC,			/* Assoc Relation */
	dbl_LIST_EVENT,			/* Event Relation */
	dbl_LIST_GREGION,		/* Gregion Relation */
	dbl_LIST_INSTRUMENT,		/* Instrument Relation */
	dbl_LIST_ORIGERR,		/* Origerr Relation */
	dbl_LIST_ORIGIN,		/* Origin Relation */
	dbl_LIST_REMARK,		/* Remark Relation */
	dbl_LIST_SENSOR,		/* Sensor Relation */
	dbl_LIST_SITE,			/* Site Relation */
	dbl_LIST_SITECHAN,		/* Sitechan Relation */
	dbl_LIST_STASSOC,		/* Stassoc Relation */
	dbl_LIST_USERDATA,		/* Userdata Relation */
	dbl_LIST_WFDISC,		/* Wfdisc Relation */
	dbl_LIST_WFTAG,	  	        /* Wftag Relation */
	dbl_LIST_SACDATA,       	/* Sacdata Relation */



/* These are the specifiers for the attributes of the affiliation relation */
	dbl_AFFI_NET,			/* (affiliation) Network Identifier (attribute) */
	dbl_AFFI_STA,			/* (affiliation) Station Code (attribute) */
	dbl_AFFI_LDDATE,		/* (affiliation) Load Date (attribute) */

/* These are the specifiers for the attributes of the arrival relation */
	dbl_ARRIV_AMP,			/* (arrival) signal amplitude (attribute) */
	dbl_ARRIV_ARID,			/* (arrival) Arrival identifier (attribute) */
	dbl_ARRIV_AUTH,			/* (arrival) Author (attribute) */
	dbl_ARRIV_AZIMUTH,		/* (arrival) Observed Azimuth (attribute) */
	dbl_ARRIV_CHAN,			/* (arrival) channel identifier (attribute) */
	dbl_ARRIV_CLIP,			/* (arrival) clipped data flag (attribute) */
	dbl_ARRIV_COMMID,		/* (arrival) comment ID (attribute) */
	dbl_ARRIV_CHANID,		/* (arrival) Chan Recording ID (attribute) */
	dbl_ARRIV_DELAZ,		/* (arrival) Delta Azimuth (attribute) */
	dbl_ARRIV_DELSLO,		/* (arrival) Delta Slowness (attribute) */
	dbl_ARRIV_EMA,			/* (arrival) Emergence Angle (attribute) */
	dbl_ARRIV_DELTIM,		/* (arrival) Delta time (attribute) */
	dbl_ARRIV_FM,			/* (arrival) First Motion (attribute) */
	dbl_ARRIV_IPHASE,		/* (arrival) Reported Phase (attribute) */
	dbl_ARRIV_JDATE,		/* (arrival) Julian Date (attribute) */
	dbl_ARRIV_LOGAT,		/* (arrival) Log(amplitude)/period (attribute) */
	dbl_ARRIV_PER,			/* (arrival) Signal Period (attribute) */
	dbl_ARRIV_QUAL,			/* (arrival) Onset Quality (attribute) */
	dbl_ARRIV_RECT,			/* (arrival) Rectilinearity (attribute) */
	dbl_ARRIV_SLOW,			/* (arrival) Observed Slowness (attribute) */
	dbl_ARRIV_SNR,			/* (arrival) Signal-noise Ratio (attribute) */
	dbl_ARRIV_STA,			/* (arrival) Station Code (attribute) */
	dbl_ARRIV_STASSID,		/* (arrival) Sta-assoc ID (attribute) */
	dbl_ARRIV_STYPE,		/* (arrival) Signal Type (attribute) */
	dbl_ARRIV_TIME,			/* (arrival) Epoch Time (attribute) */
	dbl_ARRIV_LDDATE,		/* (arrival) Load Date (attribute) */


/* These are the specifiers for the attributes of the assoc relation */
	dbl_ASSOC_ARID,			/* (assoc) Arrival identifier (attribute) */
	dbl_ASSOC_AZDEF,		/* (assoc) Azimuth defining code (attribute) */
	dbl_ASSOC_AZRES,		/* (assoc) Azimuth residual (attribute) */
	dbl_ASSOC_BELIEF,		/* (assoc) Phase ID confidence (attribute) */
	dbl_ASSOC_COMMID,		/* (assoc) comment ID (attribute) */
	dbl_ASSOC_DELTA,		/* (assoc) Src-Receiver dist (attribute) */
	dbl_ASSOC_EMARES,		/* (assoc) Emergence Angle Residual (attribute) */
	dbl_ASSOC_ESAZ,			/* (assoc) Event-station azimuth (attribute) */
	dbl_ASSOC_ORID,			/* (assoc) Origin Identification (attribute) */
	dbl_ASSOC_PHASE,		/* (assoc) Associated Phase (attribute) */
	dbl_ASSOC_SEAZ,			/* (assoc) Sta-Event Azimuth (attribute) */
	dbl_ASSOC_SLODEF,		/* (assoc) Slowness Defining code (attribute) */
	dbl_ASSOC_SLORES,		/* (assoc) Slowness Residual (attribute) */
	dbl_ASSOC_STA,			/* (assoc) Station Code (attribute) */
	dbl_ASSOC_TIMEDEF,		/* (assoc) Time-defining Code (attribute) */
	dbl_ASSOC_TIMERES,		/* (assoc) Time Residual (attribute) */
	dbl_ASSOC_VMODEL,		/* (assoc) Velocity Model (attribute) */
	dbl_ASSOC_WGT,			/* (assoc) Location Weight (attribute) */
	dbl_ASSOC_LDDATE,		/* (assoc) Load Date (attribute) */

/* These are the specifiers for the attributes of the event relation */
	dbl_EVENT_AUTH,			/* (event) Author (attribute) */
	dbl_EVENT_COMMID,		/* (event) comment ID (attribute) */
	dbl_EVENT_EVID,			/* (event) Event Identifier (attribute) */
	dbl_EVENT_EVNAME,		/* (event) Event Name (attribute) */
	dbl_EVENT_PREFOR,		/* (event) Preferred Origin (attribute) */
	dbl_EVENT_LDDATE,		/* (event) Load Date (attribute) */

/* These are the specifiers for the attributes of the gregion relation */
	dbl_GR_GRN,			/* (gregion) Geographic region number (attribute) */
	dbl_GR_GRNAME,			/* (gregion) Geographic region name (attribute) */
	dbl_GR_LDDATE,			/* (gregion) Load Date (attribute) */

/* These are the specifiers for the attributes of the instrument relation */
	dbl_INS_BAND,			/* (instrument) Frequency Band (attribute) */
	dbl_INS_DFILE,			/* (instrument) Data file (attribute) */
	dbl_INS_DIGITAL,		/* (instrument) Digital/Analog flag (attribute) */
	dbl_INS_DIR,			/* (instrument) directory (attribute) */
	dbl_INS_INID,			/* (instrument) Instrument Identifier (attribute) */
	dbl_INS_INSNAME,		/* (instrument) Instrument Name (attribute) */
	dbl_INS_INSTYPE,		/* (instrument) Instrument Type (attribute) */
	dbl_INS_NCALIB,			/* (instrument) Nominal Calib. Factor (attribute) */
	dbl_INS_NCALPER,		/* (instrument) Calibration Period (attribute) */
	dbl_INS_RSPTYPE,		/* (instrument) Response Type (attribute) */
	dbl_INS_SAMPRATE,		/* (instrument) Sampling Rate (attribute) */
	dbl_INS_LDDATE,			/* (instrument) Load Date (attribute) */

/* These are the specifiers for the attributes of the network relation */
	dbl_NETWO_AUTH,			/* (network) Author (attribute) */
	dbl_NETWO_COMMID,		/* (network) comment ID (attribute) */
	dbl_NETWO_NET,			/* (network) Network Identifier (attribute) */
	dbl_NETWO_NETNAME,		/* (network) Network Name (attribute) */
	dbl_NETWO_NETTYPE,		/* (network) Network Type (attribute) */
	dbl_NETWO_LDDATE,		/* (network) Load Date (attribute) */

/* These are the specifiers for the attributes of the origerr relation */
	dbl_ORIGE_COMMID,		/* (origerr) comment ID (attribute) */
	dbl_ORIGE_CONF,			/* (origerr) error confidence (attribute) */
	dbl_ORIGE_ORID,			/* (origerr) Origin Identification (attribute) */
	dbl_ORIGE_SDEPTH,		/* (origerr) Depth Error (attribute) */
	dbl_ORIGE_SDOBS,		/* (origerr) Std. Error of Observation (attribute) */
	dbl_ORIGE_SMAJAX,		/* (origerr) Semi-major axis of Err. ellipse (attribute) */
	dbl_ORIGE_SMINAX,		/* (origerr) Semi-minor axis of Err. ellipse (attribute) */
	dbl_ORIGE_STIME,		/* (origerr) Origin Time error (attribute) */
	dbl_ORIGE_STRIKE,		/* (origerr) Strike of major axis of ellipse (attribute) */
	dbl_ORIGE_STX,			/* (origerr) Element of Covariance matrix (attribute) */
	dbl_ORIGE_STY,			/* (origerr) Element of Covariance matrix (attribute) */
	dbl_ORIGE_STZ,			/* (origerr) Element of Covariance matrix (attribute) */
	dbl_ORIGE_SXX,			/* (origerr) Element of Covariance matrix (attribute) */
	dbl_ORIGE_SXY,			/* (origerr) Element of Covariance matrix (attribute) */
	dbl_ORIGE_SXZ,			/* (origerr) Element of Covariance matrix (attribute) */
	dbl_ORIGE_SYY,			/* (origerr) Element of Covariance matrix (attribute) */
	dbl_ORIGE_SYZ,			/* (origerr) Element of Covariance matrix (attribute) */
	dbl_ORIGE_STT,			/* (origerr) Element of Covariance matrix (attribute) */
	dbl_ORIGE_SZZ,			/* (origerr) Element of Covariance matrix (attribute) */
	dbl_ORIGE_LDDATE,		/* (origerr) Load Date (attribute) */

/* These are the specifiers for the attributes of the origin relation */
	dbl_ORIGI_ALGORITHM,		/* (origin) location algorithm (attribute) */
	dbl_ORIGI_AUTH,			/* (origin) Author (attribute) */
	dbl_ORIGI_COMMID,		/* (origin) comment ID (attribute) */
	dbl_ORIGI_DEPDP,		/* (origin) Depth from phases (attribute) */
	dbl_ORIGI_DEPTH,		/* (origin) Source Depth (attribute) */
	dbl_ORIGI_DTYPE,		/* (origin) Depth Determination flag (attribute) */
	dbl_ORIGI_ETYPE,		/* (origin) Event type (attribute) */
	dbl_ORIGI_EVID,			/* (origin) Event Identifier (attribute) */
	dbl_ORIGI_GRN,			/* (origin) Geographic region number (attribute) */
	dbl_ORIGI_JDATE,		/* (origin) Julian Date (attribute) */
	dbl_ORIGI_LAT,			/* (origin) Latitude (attribute) */
	dbl_ORIGI_LON,			/* (origin) Longitude (attribute) */
	dbl_ORIGI_MB,			/* (origin) Body Wave Mag. (attribute) */
	dbl_ORIGI_MBID,			/* (origin) Mag. ID for Mb (attribute) */
	dbl_ORIGI_ML,			/* (origin) Local Magnitude (attribute) */
	dbl_ORIGI_MLID,			/* (origin) Mad. ID for Ml (attribute) */
	dbl_ORIGI_MS,			/* (origin) Surface Wave Mag. (attribute) */
	dbl_ORIGI_MSID,			/* (origin) Mag. ID for MS (attribute) */
	dbl_ORIGI_NASS,			/* (origin) Number of Associated Arrivals (attribute) */
	dbl_ORIGI_NDEF,			/* (origin) Number of arrival used (attribute) */
	dbl_ORIGI_NDP,			/* (origin) Number of Depth Phases (attribute) */
	dbl_ORIGI_ORID,			/* (origin) Origin Identification (attribute) */
	dbl_ORIGI_SRN,			/* (origin) Region Number (attribute) */
	dbl_ORIGI_TIME,			/* (origin) Epoch Time (attribute) */
	dbl_ORIGI_LDDATE,		/* (origin) Load Date (attribute) */

/* These are the specifiers for the attributes of the remark relation */
	dbl_REMA_COMMID,		/* (remark) comment ID (attribute) */
	dbl_REMA_LINENO,		/* (remark) comment line number (attribute) */
	dbl_REMA_REMARK,		/* (remark) comment string (attribute) */
	dbl_REMA_LDDATE,		/* (remark) load date (attribute) */

/* These are the specifiers for the attributes of the sensor relation */
	dbl_SENS_CALPER,		/* (sensor) Calibration Period (attribute) */
	dbl_SENS_CALRATIO,		/* (sensor) Calib conversion ratio (attribute) */
	dbl_SENS_CHAN,			/* (sensor) channel identifier (attribute) */
	dbl_SENS_CHANID,		/* (sensor) Chan Recording ID (attribute) */
	dbl_SENS_ENDTIME,		/* (sensor) Time of last datum (attribute) */
	dbl_SENS_INID,			/* (sensor) Instrument Identifier (attribute) */
	dbl_SENS_INSTANT,		/* (sensor) Snapshot indicator (attribute) */
	dbl_SENS_JDATE,			/* (sensor) Julian Date (attribute) */
	dbl_SENS_STA,			/* (sensor) Station Code (attribute) */
	dbl_SENS_TIME,			/* (sensor) Epoch Time (attribute) */
	dbl_SENS_TSHIFT,		/* (sensor) Correction for Clock Errors (attribute) */
	dbl_SENS_LDDATE,		/* (sensor) Load Date (attribute) */

/* These are the specifiers for the attributes of the site relation */
	dbl_SITE_DEAST,			/* (site) Distance East (attribute) */
	dbl_SITE_DNORTH,		/* (site) Distance North (attribute) */
	dbl_SITE_ELEV,			/* (site) Elevation (attribute) */
	dbl_SITE_LAT,			/* (site) Latitude (attribute) */
	dbl_SITE_LON,			/* (site) Longitude (attribute) */
	dbl_SITE_OFFDATE,		/* (site) Turn-off Date (attribute) */
	dbl_SITE_ONDATE,		/* (site) Turn-on Date (attribute) */
	dbl_SITE_REFSTA,		/* (site) Reference Station (attribute) */
	dbl_SITE_STA,			/* (site) Station Code (attribute) */
	dbl_SITE_STANAME,		/* (site) Station Name (attribute) */
	dbl_SITE_STATYPE,		/* (site) Station Type (attribute) */
	dbl_SITE_LDDATE,		/* (site) Load Date (attribute) */

/* These are the specifiers for the attributes of the sitechan relation */
	dbl_SITEC_CHAN,			/* (sitechan) channel identifier (attribute) */
	dbl_SITEC_CHANID,		/* (sitechan) Chan Recording ID (attribute) */
	dbl_SITEC_CTYPE,		/* (sitechan) Channel Type (attribute) */
	dbl_SITEC_DESCRIP,		/* (sitechan) Channel Description (attribute) */
	dbl_SITEC_EDEPTH,		/* (sitechan) Emplacement Depth (attribute) */
	dbl_SITEC_HANG,			/* (sitechan) Horizontal orientation (attribute) */
	dbl_SITEC_OFFDATE,		/* (sitechan) Turn-off Date (attribute) */
	dbl_SITEC_ONDATE,		/* (sitechan) Turn-on Date (attribute) */
	dbl_SITEC_STA,			/* (sitechan) Station Code (attribute) */
	dbl_SITEC_VANG,			/* (sitechan) Vert. Orientation of Seis.  (attribute) */
	dbl_SITEC_LDDATE,		/* (sitechan) Load Date (attribute) */

/* These are the specifiers for the attributes of the sregion relation */
	dbl_SR_SRN,			/* (sregion) Region Number (attribute) */
	dbl_SR_SRNAME,			/* (sregion) Seismic region Name (attribute) */
	dbl_SR_LDDATE,			/* (sregion) Load Date (attribute) */
	
/* These are the specifiers for the attributes of the stassoc relation */
	dbl_STASS_AUTH,			/* (stassoc) Author (attribute) */
	dbl_STASS_AZIMUTH,		/* (stassoc) Observed Azimuth (attribute) */
	dbl_STASS_COMMID,		/* (stassoc) comment ID (attribute) */
	dbl_STASS_DEPTH,		/* (stassoc) Source Depth (attribute) */
	dbl_STASS_DIST,			/* (stassoc) Estimated Distance (attribute) */
	dbl_STASS_ETYPE,		/* (stassoc) Event type (attribute) */
	dbl_STASS_IMB,			/* (stassoc) Initial Body wave Mag. (attribute) */
	dbl_STASS_IML,			/* (stassoc) Initial Local Mag. (attribute) */
	dbl_STASS_IMS,			/* (stassoc) Initial Surface wave Mag. (attribute) */
	dbl_STASS_LAT,			/* (stassoc) Latitude (attribute) */
	dbl_STASS_LOCATION,		/* (stassoc) Location Description. (attribute) */
	dbl_STASS_LON,			/* (stassoc) Longitude (attribute) */
	dbl_STASS_STA,			/* (stassoc) Station Code (attribute) */
	dbl_STASS_STASSID,		/* (stassoc) Sta-assoc ID (attribute) */
	dbl_STASS_TIME,			/* (stassoc) Epoch Time (attribute) */
	dbl_STASS_LDDATE,		/* (stassoc) Load Date (attribute) */
	
/* These are the specifiers for the attributes of the wfdisc relation */
	dbl_WFDIS_CALIB,		/* (wfdisc) Calibration Factor (attribute) */
	dbl_WFDIS_CALPER,		/* (wfdisc) Calibration Period (attribute) */
	dbl_WFDIS_CHAN,			/* (wfdisc) channel identifier (attribute) */
	dbl_WFDIS_CHANID,		/* (wfdisc) Chan Recording ID (attribute) */
	dbl_WFDIS_CLIP,			/* (wfdisc) clipped data flag (attribute) */
	dbl_WFDIS_COMMID,		/* (wfdisc) comment ID (attribute) */
	dbl_WFDIS_DATATYPE,		/* (wfdisc) Numeric storage (attribute) */
	dbl_WFDIS_DFILE,		/* (wfdisc) Data file (attribute) */
	dbl_WFDIS_DIR,			/* (wfdisc) directory (attribute) */
	dbl_WFDIS_ENDTIME,		/* (wfdisc) Time of last datum (attribute) */
	dbl_WFDIS_FOFF,			/* (wfdisc) File offset (attribute) */
	dbl_WFDIS_INSTYPE,		/* (wfdisc) Instrument Type (attribute) */
	dbl_WFDIS_JDATE,		/* (wfdisc) Julian Date (attribute) */
	dbl_WFDIS_NSAMP,		/* (wfdisc) Number of Samples (attribute) */
	dbl_WFDIS_SAMPRATE,		/* (wfdisc) Sampling Rate (attribute) */
	dbl_WFDIS_SEGTYPE,		/* (wfdisc) Segment Type (attribute) */
	dbl_WFDIS_STA,			/* (wfdisc) Station Code (attribute) */
	dbl_WFDIS_TIME,			/* (wfdisc) Epoch Time (attribute) */
	dbl_WFDIS_WFID,			/* (wfdisc) Waveform Identifier (attribute) */
	dbl_WFDIS_LDDATE,		/* (wfdisc) Load Date (attribute) */
	dbl_WFDIS_REAL_TRC,		/* (wfdisc) pointer to real trace (attribute) */
	dbl_WFDIS_IMAG_TRC,		/* (wfdisc) pointer to imag trace (attribute) */
	dbl_WFDIS_COMPLEX,		/* (wfdisc) complex flag (attribute) */

	
/* These are the specifiers for the attributes of the wftag relation */
	dbl_WFTAG_TAGNAME,		/* (wftag) key (arid, orid,...) (attribute) */
	dbl_WFTAG_TAGID,		/* (wftag) tagname value (attribute) */
	dbl_WFTAG_WFID,			/* (wftag) waveform ID (attribute) */
	dbl_WFTAG_LDDATE,		/* (wftag) load date (attribute) */

	
/* These are the specifiers for the attributes of the sacdata relation */
	dbl_SACD_UDATA,			/* (sacdata)  */
	dbl_SACD_SYNTHFLG,		/* (sacdata)  */
	dbl_SACD_LPSPOL,		/* (sacdata)  */
	dbl_SACD_IZTYPE,		/* (sacdata)  */
	dbl_SACD_IDEP,			/* (sacdata)  */
	dbl_SACD_IFTYPE,		/* (sacdata)  */
	dbl_SACD_WFID, 			/* (sacdata)  */
	dbl_SACD_ODELTA,
	dbl_SACD_EVEL,
        dbl_SACD_IINST,
        dbl_SACD_ISTREG,
        dbl_SACD_IQUAL,
        dbl_SACD_LOVROK,
        dbl_SACD_LCALDA,
        dbl_SACD_KHOLE,
        dbl_SACD_KO,
        dbl_SACD_KDATRD
	};
typedef enum dblObjects dblObject;	

enum TracePart { REALDATA, IMAGDATA, COMPLEXDATA};
typedef enum TracePart DataType;

#define LIST_CASES 	case dbl_LIST_AFFILIATION: \
	case dbl_LIST_ARRIVAL: \
	case dbl_LIST_ASSOC: \
	case dbl_LIST_EVENT: \
	case dbl_LIST_GREGION: \
	case dbl_LIST_INSTRUMENT: \
	case dbl_LIST_ORIGERR: \
	case dbl_LIST_ORIGIN: \
	case dbl_LIST_REMARK: \
	case dbl_LIST_SENSOR: \
	case dbl_LIST_SITE: \
	case dbl_LIST_SITECHAN: \
	case dbl_LIST_STASSOC: \
	case dbl_LIST_USERDATA: \
	case dbl_LIST_WFDISC: \
	case dbl_LIST_WFTAG: \
    case dbl_LIST_SACDATA:


#define AFFI_CASES    case dbl_AFFI_NET: \
   case dbl_AFFI_STA: \
   case dbl_AFFI_LDDATE:


#define ARRIVAL_CASES   case dbl_ARRIV_AMP: \
   case dbl_ARRIV_ARID: \
   case dbl_ARRIV_AUTH: \
   case dbl_ARRIV_AZIMUTH: \
   case dbl_ARRIV_CLIP: \
   case dbl_ARRIV_COMMID: \
   case dbl_ARRIV_CHAN: \
   case dbl_ARRIV_CHANID: \
   case dbl_ARRIV_DELAZ: \
   case dbl_ARRIV_DELSLO: \
   case dbl_ARRIV_EMA: \
   case dbl_ARRIV_DELTIM: \
   case dbl_ARRIV_FM: \
   case dbl_ARRIV_IPHASE: \
   case dbl_ARRIV_JDATE: \
   case dbl_ARRIV_LOGAT: \
   case dbl_ARRIV_PER: \
   case dbl_ARRIV_QUAL: \
   case dbl_ARRIV_RECT: \
   case dbl_ARRIV_SLOW: \
   case dbl_ARRIV_SNR: \
   case dbl_ARRIV_STA: \
   case dbl_ARRIV_STASSID: \
   case dbl_ARRIV_STYPE: \
   case dbl_ARRIV_TIME: \
   case dbl_ARRIV_LDDATE: 

#define ASSOC_CASES    case dbl_ASSOC_ARID: \
   case dbl_ASSOC_AZDEF: \
   case dbl_ASSOC_AZRES: \
   case dbl_ASSOC_BELIEF: \
   case dbl_ASSOC_COMMID: \
   case dbl_ASSOC_DELTA: \
   case dbl_ASSOC_EMARES: \
   case dbl_ASSOC_ESAZ: \
   case dbl_ASSOC_ORID: \
   case dbl_ASSOC_PHASE: \
   case dbl_ASSOC_SEAZ: \
   case dbl_ASSOC_SLODEF: \
   case dbl_ASSOC_SLORES: \
   case dbl_ASSOC_STA: \
   case dbl_ASSOC_TIMEDEF: \
   case dbl_ASSOC_TIMERES: \
   case dbl_ASSOC_VMODEL: \
   case dbl_ASSOC_WGT: \
   case dbl_ASSOC_LDDATE: 

#define EVENT_CASES 	case dbl_EVENT_AUTH: \
	case dbl_EVENT_COMMID:                        \
	case dbl_EVENT_EVID: \
	case dbl_EVENT_EVNAME: \
	case dbl_EVENT_PREFOR: \
	case dbl_EVENT_LDDATE:

#define GR_CASES	case dbl_GR_GRN: \
	case dbl_GR_GRNAME: \
	case dbl_GR_LDDATE: 

#define INS_CASES 	case dbl_INS_BAND: \
	case dbl_INS_DFILE:                \
	case dbl_INS_DIGITAL:              \
	case dbl_INS_DIR:                  \
	case dbl_INS_INID:                 \
	case dbl_INS_INSNAME:              \
	case dbl_INS_INSTYPE:              \
	case dbl_INS_NCALIB:               \
	case dbl_INS_NCALPER:              \
	case dbl_INS_RSPTYPE:              \
	case dbl_INS_SAMPRATE:             \
	case dbl_INS_LDDATE:

#define NET_CASES case dbl_NETWO_AUTH: \
 case dbl_NETWO_COMMID:                \
 case dbl_NETWO_NET:                   \
 case dbl_NETWO_NETNAME:               \
 case dbl_NETWO_NETTYPE:               \
 case dbl_NETWO_LDDATE: 

#define ORIGERR_CASES 	case dbl_ORIGE_COMMID: \
	case dbl_ORIGE_CONF: \
	case dbl_ORIGE_ORID: \
	case dbl_ORIGE_SDEPTH: \
	case dbl_ORIGE_SDOBS: \
	case dbl_ORIGE_SMAJAX: \
	case dbl_ORIGE_SMINAX: \
	case dbl_ORIGE_STIME: \
	case dbl_ORIGE_STRIKE: \
	case dbl_ORIGE_STX: \
	case dbl_ORIGE_STY: \
	case dbl_ORIGE_STZ: \
	case dbl_ORIGE_SXX: \
	case dbl_ORIGE_SXY: \
	case dbl_ORIGE_SXZ: \
	case dbl_ORIGE_SYY: \
	case dbl_ORIGE_SYZ: \
	case dbl_ORIGE_STT: \
	case dbl_ORIGE_SZZ: \
    case dbl_ORIGE_LDDATE:

#define ORIGIN_CASES 	case dbl_ORIGI_ALGORITHM: \
	case dbl_ORIGI_AUTH: \
	case dbl_ORIGI_COMMID: \
	case dbl_ORIGI_DEPDP: \
	case dbl_ORIGI_DEPTH: \
	case dbl_ORIGI_DTYPE: \
	case dbl_ORIGI_ETYPE: \
	case dbl_ORIGI_EVID: \
	case dbl_ORIGI_GRN: \
	case dbl_ORIGI_JDATE: \
	case dbl_ORIGI_LAT: \
	case dbl_ORIGI_LON: \
	case dbl_ORIGI_MB: \
	case dbl_ORIGI_MBID: \
	case dbl_ORIGI_ML: \
	case dbl_ORIGI_MLID: \
	case dbl_ORIGI_MS: \
	case dbl_ORIGI_MSID: \
	case dbl_ORIGI_NASS: \
	case dbl_ORIGI_NDEF: \
	case dbl_ORIGI_NDP: \
	case dbl_ORIGI_ORID: \
	case dbl_ORIGI_SRN: \
	case dbl_ORIGI_TIME: \
	case dbl_ORIGI_LDDATE: 

#define REMA_CASES 	case dbl_REMA_COMMID: \
	case dbl_REMA_LINENO: \
	case dbl_REMA_REMARK: \
	case dbl_REMA_LDDATE: 

#define SENS_CASES 	case dbl_SENS_CALPER: \
	case dbl_SENS_CALRATIO: \
	case dbl_SENS_CHAN: \
	case dbl_SENS_CHANID: \
	case dbl_SENS_ENDTIME: \
	case dbl_SENS_INID: \
	case dbl_SENS_INSTANT: \
	case dbl_SENS_JDATE: \
	case dbl_SENS_STA: \
	case dbl_SENS_TIME: \
	case dbl_SENS_TSHIFT: \
	case dbl_SENS_LDDATE: 

#define SITE_CASES 	case dbl_SITE_DEAST: \
	case dbl_SITE_DNORTH: \
	case dbl_SITE_ELEV: \
	case dbl_SITE_LAT: \
	case dbl_SITE_LON: \
	case dbl_SITE_OFFDATE: \
	case dbl_SITE_ONDATE: \
	case dbl_SITE_REFSTA: \
	case dbl_SITE_STA: \
	case dbl_SITE_STANAME: \
	case dbl_SITE_STATYPE: \
	case dbl_SITE_LDDATE: 

#define SITECHAN_CASES 	case dbl_SITEC_CHAN: \
	case dbl_SITEC_CHANID: \
	case dbl_SITEC_CTYPE: \
	case dbl_SITEC_DESCRIP: \
	case dbl_SITEC_EDEPTH: \
	case dbl_SITEC_HANG: \
	case dbl_SITEC_OFFDATE: \
	case dbl_SITEC_ONDATE: \
	case dbl_SITEC_STA: \
	case dbl_SITEC_VANG: \
	case dbl_SITEC_LDDATE: 

#define SR_CASES 	case dbl_SR_SRN: \
	case dbl_SR_SRNAME: \
	case dbl_SR_LDDATE: 

#define STASSOC_CASES 	case dbl_STASS_AUTH: \
	case dbl_STASS_AZIMUTH: \
	case dbl_STASS_COMMID: \
	case dbl_STASS_DEPTH: \
	case dbl_STASS_DIST: \
	case dbl_STASS_ETYPE: \
	case dbl_STASS_IMB: \
	case dbl_STASS_IML: \
	case dbl_STASS_IMS: \
	case dbl_STASS_LAT: \
	case dbl_STASS_LOCATION: \
	case dbl_STASS_LON: \
	case dbl_STASS_STA: \
	case dbl_STASS_STASSID: \
	case dbl_STASS_TIME: \
	case dbl_STASS_LDDATE: 

#define WFDISC_CASES 	case dbl_WFDIS_CALIB: \
	case dbl_WFDIS_CALPER: \
	case dbl_WFDIS_CHAN: \
	case dbl_WFDIS_CHANID: \
	case dbl_WFDIS_CLIP: \
	case dbl_WFDIS_COMMID: \
	case dbl_WFDIS_DATATYPE: \
	case dbl_WFDIS_DFILE: \
	case dbl_WFDIS_DIR: \
	case dbl_WFDIS_ENDTIME: \
	case dbl_WFDIS_FOFF: \
	case dbl_WFDIS_INSTYPE: \
	case dbl_WFDIS_JDATE: \
	case dbl_WFDIS_NSAMP: \
	case dbl_WFDIS_SAMPRATE: \
	case dbl_WFDIS_SEGTYPE: \
	case dbl_WFDIS_STA: \
	case dbl_WFDIS_TIME: \
	case dbl_WFDIS_WFID: \
	case dbl_WFDIS_LDDATE: \
	case dbl_WFDIS_REAL_TRC: \
	case dbl_WFDIS_IMAG_TRC: \
	case dbl_WFDIS_COMPLEX: 

#define WFTAG_CASES 	case dbl_WFTAG_TAGNAME: \
	case dbl_WFTAG_TAGID: \
	case dbl_WFTAG_WFID: \
	case dbl_WFTAG_LDDATE: 



#define SACD_CASES case dbl_SACD_UDATA: \
                   case dbl_SACD_SYNTHFLG: \
                   case dbl_SACD_LPSPOL: \
                   case dbl_SACD_IZTYPE: \
                   case dbl_SACD_IDEP: \
                   case dbl_SACD_IFTYPE: \
                   case dbl_SACD_WFID: \
                   case dbl_SACD_ODELTA: \
                   case dbl_SACD_EVEL: \
                   case dbl_SACD_IINST: \
                   case dbl_SACD_ISTREG: \
                   case dbl_SACD_IQUAL: \
                   case dbl_SACD_LOVROK: \
                   case dbl_SACD_LCALDA: \
                   case dbl_SACD_KHOLE: \
                   case dbl_SACD_KO: \
                   case dbl_SACD_KDATRD:


#endif 	

