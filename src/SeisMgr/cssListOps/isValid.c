#include <stdio.h>
#include <string.h>

#define NULL_AFFILIATION
#define NULL_ARRIVAL
#define NULL_ASSOC
#define NULL_EVENT
#define NULL_GREGION
#define NULL_INSTRUMENT
#define NULL_ORIGIN_ERROR
#define NULL_ORIGIN
#define NULL_REMARK
#define NULL_SENSOR
#define NULL_SITE
#define NULL_SITECHAN
#define NULL_STASSOC
#define NULL_WFDISC
#define NULL_WFTAG
#include "cssListOps/cssStrucs.h"
#include "cssListOps/cssListStrucs.h"
#include "cssListOps/dblPublicDefs.h"
#include "cssListOps/cssListOps.h"



void * findNullForAffiliationField ( int field )
{
    void *nullPtr = NULL ;

    switch ( field ) {
        case dbl_AFFI_NET:                   /* (affiliation) Network Identifier (attribute) */
	    nullPtr = (void *) &nullAffiliation.net ;
	    break ;
        case dbl_AFFI_STA:                   /* (affiliation) Station Code (attribute) */
	    nullPtr = (void *) &nullAffiliation.sta ;
	    break ;
        case dbl_AFFI_LDDATE:                /* (affiliation) Load Date (attribute) */
	    nullPtr = (void *) &nullAffiliation.lddate ;
	    break ;
    } /* end switch */

    return nullPtr ;
}

void * findNullForArrivalField ( int field )
{
    void *nullPtr = NULL ;

    switch ( field ) {
        case dbl_ARRIV_AMP:                  /* (arrival) signal amplitude (attribute) */
	    nullPtr = (void *) &nullArrival.amp ;
	    break ;
        case dbl_ARRIV_ARID:                 /* (arrival) Arrival identifier (attribute) */
	    nullPtr = (void *) &nullArrival.arid ;
	    break ;
        case dbl_ARRIV_AUTH:                 /* (arrival) Author (attribute) */
	    nullPtr = (void *) &nullArrival.auth ;
	    break ;
        case dbl_ARRIV_AZIMUTH:              /* (arrival) Observed Azimuth (attribute) */
	    nullPtr = (void *) &nullArrival.azimuth ;
	    break ;
        case dbl_ARRIV_CHAN:                 /* (arrival) channel identifier (attribute) */
	    nullPtr = (void *) &nullArrival.chan ;
	    break ;
        case dbl_ARRIV_CLIP:                 /* (arrival) clipped data flag (attribute) */
	    nullPtr = (void *) &nullArrival.clip ;
	    break ;
        case dbl_ARRIV_COMMID:               /* (arrival) comment ID (attribute) */
	    nullPtr = (void *) &nullArrival.commid ;
	    break ;
        case dbl_ARRIV_CHANID:               /* (arrival) Chan Recording ID (attribute) */
	    nullPtr = (void *) &nullArrival.chanid ;
	    break ;
        case dbl_ARRIV_DELAZ:                /* (arrival) Delta Azimuth (attribute) */
	    nullPtr = (void *) &nullArrival.delaz ;
	    break ;
        case dbl_ARRIV_DELSLO:               /* (arrival) Delta Slowness (attribute) */
	    nullPtr = (void *) &nullArrival.delslo ;
	    break ;
        case dbl_ARRIV_EMA:                  /* (arrival) Emergence Angle (attribute) */
	    nullPtr = (void *) &nullArrival.ema ;
	    break ;
        case dbl_ARRIV_DELTIM:               /* (arrival) Delta time (attribute) */
	    nullPtr = (void *) &nullArrival.deltim ;
	    break ;
        case dbl_ARRIV_FM:                   /* (arrival) First Motion (attribute) */
	    nullPtr = (void *) &nullArrival.fm ;
	    break ;
        case dbl_ARRIV_IPHASE:               /* (arrival) Reported Phase (attribute) */
	    nullPtr = (void *) &nullArrival.iphase ;
	    break ;
        case dbl_ARRIV_JDATE:                /* (arrival) Julian Date (attribute) */
	    nullPtr = (void *) &nullArrival.jdate ;
	    break ;
        case dbl_ARRIV_LOGAT:                /* (arrival) Log(amplitude)/period (attribute) */
	    nullPtr = (void *) &nullArrival.logat ;
	    break ;
        case dbl_ARRIV_PER:                  /* (arrival) Signal Period (attribute) */
	    nullPtr = (void *) &nullArrival.per ;
	    break ;
        case dbl_ARRIV_QUAL:                 /* (arrival) Onset Quality (attribute) */
	    nullPtr = (void *) &nullArrival.qual ;
	    break ;
        case dbl_ARRIV_RECT:                 /* (arrival) Rectilinearity (attribute) */
	    nullPtr = (void *) &nullArrival.rect ;
	    break ;
        case dbl_ARRIV_SLOW:                 /* (arrival) Observed Slowness (attribute) */
	    nullPtr = (void *) &nullArrival.slow ;
	    break ;
        case dbl_ARRIV_SNR:                  /* (arrival) Signal-noise Ratio (attribute) */
	    nullPtr = (void *) &nullArrival.snr ;
	    break ;
        case dbl_ARRIV_STA:                  /* (arrival) Station Code (attribute) */
	    nullPtr = (void *) &nullArrival.sta ;
	    break ;
        case dbl_ARRIV_STASSID:              /* (arrival) Sta-assoc ID (attribute) */
	    nullPtr = (void *) &nullArrival.stassid ;
	    break ;
        case dbl_ARRIV_STYPE:                /* (arrival) Signal Type (attribute) */
	    nullPtr = (void *) &nullArrival.stype ;
	    break ;
        case dbl_ARRIV_TIME:                 /* (arrival) Epoch Time (attribute) */
	    nullPtr = (void *) &nullArrival.time ;
	    break ;
        case dbl_ARRIV_LDDATE:               /* (arrival) Load Date (attribute) */
	    nullPtr = (void *) &nullArrival.lddate ;
	    break ;
    } /* end switch */

    return nullPtr ;
}


void * findNullForAssocField ( int field )
{
    void *nullPtr = NULL ;

    switch ( field ) {
        case dbl_ASSOC_ARID:                 /* (assoc) Arrival identifier (attribute) */
	    nullPtr = (void *) &nullAssoc.arid ;
	    break ;
        case dbl_ASSOC_AZDEF:                /* (assoc) Azimuth defining code (attribute) */
	    nullPtr = (void *) &nullAssoc.azdef ;
	    break ;
        case dbl_ASSOC_AZRES:                /* (assoc) Azimuth residual (attribute) */
	    nullPtr = (void *) &nullAssoc.azres ;
	    break ;
        case dbl_ASSOC_BELIEF:               /* (assoc) Phase ID confidence (attribute) */
	    nullPtr = (void *) &nullAssoc.belief ;
	    break ;
        case dbl_ASSOC_COMMID:               /* (assoc) comment ID (attribute) */
	    nullPtr = (void *) &nullAssoc.commid ;
	    break ;
        case dbl_ASSOC_DELTA:                /* (assoc) Src-Receiver dist (attribute) */
	    nullPtr = (void *) &nullAssoc.delta ;
	    break ;
        case dbl_ASSOC_EMARES:               /* (assoc) Emergence Angle Residual (attribute) */
	    nullPtr = (void *) &nullAssoc.emares ;
	    break ;
        case dbl_ASSOC_ESAZ:                 /* (assoc) Event-station azimuth (attribute) */
	    nullPtr = (void *) &nullAssoc.esaz ;
	    break ;
        case dbl_ASSOC_ORID:                 /* (assoc) Origin Identification (attribute) */
	    nullPtr = (void *) &nullAssoc.orid ;
	    break ;
        case dbl_ASSOC_PHASE:                /* (assoc) Associated Phase (attribute) */
	    nullPtr = (void *) &nullAssoc.phase ;
	    break ;
        case dbl_ASSOC_SEAZ:                 /* (assoc) Sta-Event Azimuth (attribute) */
	    nullPtr = (void *) &nullAssoc.seaz ;
	    break ;
        case dbl_ASSOC_SLODEF:               /* (assoc) Slowness Defining code (attribute) */
	    nullPtr = (void *) &nullAssoc.slodef ;
	    break ;
        case dbl_ASSOC_SLORES:               /* (assoc) Slowness Residual (attribute) */
	    nullPtr = (void *) &nullAssoc.slores ;
	    break ;
        case dbl_ASSOC_STA:                  /* (assoc) Station Code (attribute) */
	    nullPtr = (void *) &nullAssoc.sta ;
	    break ;
        case dbl_ASSOC_TIMEDEF:              /* (assoc) Time-defining Code (attribute) */
	    nullPtr = (void *) &nullAssoc.timedef ;
	    break ;
        case dbl_ASSOC_TIMERES:              /* (assoc) Time Residual (attribute) */
	    nullPtr = (void *) &nullAssoc.timeres ;
	    break ;
        case dbl_ASSOC_VMODEL:               /* (assoc) Velocity Model (attribute) */
	    nullPtr = (void *) &nullAssoc.vmodel ;
	    break ;
        case dbl_ASSOC_WGT:                  /* (assoc) Location Weight (attribute) */
	    nullPtr = (void *) &nullAssoc.wgt ;
	    break ;
        case dbl_ASSOC_LDDATE:               /* (assoc) Load Date (attribute) */
	    nullPtr = (void *) &nullAssoc.lddate ;
	    break ;
    } /* end switch */

    return nullPtr ;
}

void * findNullForEventField ( int field )
{
    void *nullPtr = NULL ;

    switch ( field ) {
        case dbl_EVENT_AUTH:                 /* (event) Author (attribute) */
	    nullPtr = (void *) &nullEvent.auth ;
	    break ;
        case dbl_EVENT_COMMID:               /* (event) comment ID (attribute) */
	    nullPtr = (void *) &nullEvent.commid ;
	    break ;
        case dbl_EVENT_EVID:                 /* (event) Event Identifier (attribute) */
	    nullPtr = (void *) &nullEvent.evid ;
	    break ;
        case dbl_EVENT_EVNAME:               /* (event) Event Name (attribute) */
	    nullPtr = (void *) &nullEvent.evname ;
	    break ;
        case dbl_EVENT_PREFOR:               /* (event) Preferred Origin (attribute) */
	    nullPtr = (void *) &nullEvent.prefor ;
	    break ;
        case dbl_EVENT_LDDATE:               /* (event) Load Date (attribute) */
	    nullPtr = (void *) &nullEvent.lddate ;
	    break ;
    } /* end switch */

    return nullPtr ;
}

void * findNullForGregionField ( int field )
{
    void *nullPtr = NULL ;

    switch ( field ) {
        case dbl_GR_GRN:                     /* (gregion) Geographic region number (attribute) */
	    nullPtr = (void *) &nullGregion.grn ;
	    break ;
        case dbl_GR_GRNAME:                  /* (gregion) Geographic region name (attribute) */
	    nullPtr = (void *) &nullGregion.grname ;
	    break ;
        case dbl_GR_LDDATE:                  /* (gregion) Load Date (attribute) */
	    nullPtr = (void *) &nullGregion.lddate ;
	    break ;
    } /* end switch */

    return nullPtr ;
}

void * findNullForInstrumentField ( int field )
{
    void *nullPtr = NULL ;

    switch ( field ) {
        case dbl_INS_BAND:                   /* (instrument) Frequency Band (attribute) */
	    nullPtr = (void *) &nullInstrument.band ;
	    break ;
        case dbl_INS_DFILE:                  /* (instrument) Data file (attribute) */
	    nullPtr = (void *) &nullInstrument.dfile ;
	    break ;
        case dbl_INS_DIGITAL:                /* (instrument) Digital/Analog flag (attribute) */
	    nullPtr = (void *) &nullInstrument.digital ;
	    break ;
        case dbl_INS_DIR:                    /* (instrument) directory (attribute) */
	    nullPtr = (void *) &nullInstrument.dir ;
	    break ;
        case dbl_INS_INID:                   /* (instrument) Instrument Identifier (attribute) */
	    nullPtr = (void *) &nullInstrument.inid ;
	    break ;
        case dbl_INS_INSNAME:                /* (instrument) Instrument Name (attribute) */
	    nullPtr = (void *) &nullInstrument.insname ;
	    break ;
        case dbl_INS_INSTYPE:                /* (instrument) Instrument Type (attribute) */
	    nullPtr = (void *) &nullInstrument.instype ;
	    break ;
        case dbl_INS_NCALIB:                 /* (instrument) Nominal Calib. Factor (attribute) */
	    nullPtr = (void *) &nullInstrument.ncalib ;
	    break ;
        case dbl_INS_NCALPER:                /* (instrument) Calibration Period (attribute) */
	    nullPtr = (void *) &nullInstrument.ncalper ;
	    break ;
        case dbl_INS_RSPTYPE:                /* (instrument) Response Type (attribute) */
	    nullPtr = (void *) &nullInstrument.rsptype ;
	    break ;
        case dbl_INS_SAMPRATE:               /* (instrument) Sampling Rate (attribute) */
	    nullPtr = (void *) &nullInstrument.samprate ;
	    break ;
        case dbl_INS_LDDATE:                 /* (instrument) Load Date (attribute) */
	    nullPtr = (void *) &nullInstrument.lddate ;
	    break ;
    } /* end switch */

    return nullPtr ;
}


void * findNullForOrigerrField ( int field )
{
    void *nullPtr = NULL ;

    switch ( field ) {
        case dbl_ORIGE_COMMID:               /* (origerr) comment ID (attribute) */
	    nullPtr = (void *) &nullOrigerr.commid ;
	    break ;
        case dbl_ORIGE_CONF:                 /* (origerr) error confidence (attribute) */
	    nullPtr = (void *) &nullOrigerr.conf ;
	    break ;
        case dbl_ORIGE_ORID:                 /* (origerr) Origin Identification (attribute) */
	    nullPtr = (void *) &nullOrigerr.orid ;
	    break ;
        case dbl_ORIGE_SDEPTH:               /* (origerr) Depth Error (attribute) */
	    nullPtr = (void *) &nullOrigerr.sdepth ;
	    break ;
        case dbl_ORIGE_SDOBS:                /* (origerr) Std. Error of Observation (attribute) */
	    nullPtr = (void *) &nullOrigerr.sdobs ;
	    break ;
        case dbl_ORIGE_SMAJAX:               /* (origerr) Semi-major axis of Err. ellipse (attribute) */
	    nullPtr = (void *) &nullOrigerr.smajax ;
	    break ;
        case dbl_ORIGE_SMINAX:               /* (origerr) Semi-minor axis of Err. ellipse (attribute) */
	    nullPtr = (void *) &nullOrigerr.sminax ;
	    break ;
        case dbl_ORIGE_STIME:                /* (origerr) Origin Time error (attribute) */
	    nullPtr = (void *) &nullOrigerr.stime ;
	    break ;
        case dbl_ORIGE_STRIKE:               /* (origerr) Strike of major axis of ellipse (attribute) */
	    nullPtr = (void *) &nullOrigerr.strike ;
	    break ;
        case dbl_ORIGE_STX:                  /* (origerr) Element of Covariance matrix (attribute) */
	    nullPtr = (void *) &nullOrigerr.stx ;
	    break ;
        case dbl_ORIGE_STY:                  /* (origerr) Element of Covariance matrix (attribute) */
	    nullPtr = (void *) &nullOrigerr.sty ;
	    break ;
        case dbl_ORIGE_STZ:                  /* (origerr) Element of Covariance matrix (attribute) */
	    nullPtr = (void *) &nullOrigerr.stz ;
	    break ;
        case dbl_ORIGE_SXX:                  /* (origerr) Element of Covariance matrix (attribute) */
	    nullPtr = (void *) &nullOrigerr.sxx ;
	    break ;
        case dbl_ORIGE_SXY:                  /* (origerr) Element of Covariance matrix (attribute) */
	    nullPtr = (void *) &nullOrigerr.sxy ;
	    break ;
        case dbl_ORIGE_SXZ:                  /* (origerr) Element of Covariance matrix (attribute) */
	    nullPtr = (void *) &nullOrigerr.sxz ;
	    break ;
        case dbl_ORIGE_SYY:                  /* (origerr) Element of Covariance matrix (attribute) */
	    nullPtr = (void *) &nullOrigerr.syy ;
	    break ;
        case dbl_ORIGE_SYZ:                  /* (origerr) Element of Covariance matrix (attribute) */
	    nullPtr = (void *) &nullOrigerr.syz ;
	    break ;
        case dbl_ORIGE_STT:                  /* (origerr) Element of Covariance matrix (attribute) */
	    nullPtr = (void *) &nullOrigerr.stt ;
	    break ;
        case dbl_ORIGE_SZZ:                  /* (origerr) Element of Covariance matrix (attribute) */
	    nullPtr = (void *) &nullOrigerr.szz ;
	    break ;
        case dbl_ORIGE_LDDATE:               /* (origerr) Load Date (attribute) */
	    nullPtr = (void *) &nullOrigerr.lddate ;
	    break ;
    } /* end switch */

    return nullPtr ;
}

void * findNullForOriginField ( int field )
{
    void *nullPtr = NULL ;

    switch ( field ) {
        case dbl_ORIGI_ALGORITHM:            /* (origin) location algorithm (attribute) */
	    nullPtr = (void *) &nullOrigin.algorithm ;
	    break ;
        case dbl_ORIGI_AUTH:                 /* (origin) Author (attribute) */
	    nullPtr = (void *) &nullOrigin.auth ;
	    break ;
        case dbl_ORIGI_COMMID:               /* (origin) comment ID (attribute) */
	    nullPtr = (void *) &nullOrigin.commid ;
	    break ;
        case dbl_ORIGI_DEPDP:                /* (origin) Depth from phases (attribute) */
	    nullPtr = (void *) &nullOrigin.depdp ;
	    break ;
        case dbl_ORIGI_DEPTH:                /* (origin) Source Depth (attribute) */
	    nullPtr = (void *) &nullOrigin.depth ;
	    break ;
        case dbl_ORIGI_DTYPE:                /* (origin) Depth Determination flag (attribute) */
	    nullPtr = (void *) &nullOrigin.dtype ;
	    break ;
        case dbl_ORIGI_ETYPE:                /* (origin) Event type (attribute) */
	    nullPtr = (void *) &nullOrigin.etype ;
	    break ;
        case dbl_ORIGI_EVID:                 /* (origin) Event Identifier (attribute) */
	    nullPtr = (void *) &nullOrigin.evid ;
	    break ;
        case dbl_ORIGI_GRN:                  /* (origin) Geographic region number (attribute) */
	    nullPtr = (void *) &nullOrigin.grn ;
	    break ;
        case dbl_ORIGI_JDATE:                /* (origin) Julian Date (attribute) */
	    nullPtr = (void *) &nullOrigin.jdate ;
	    break ;
        case dbl_ORIGI_LAT:                  /* (origin) Latitude (attribute) */
	    nullPtr = (void *) &nullOrigin.lat ;
	    break ;
        case dbl_ORIGI_LON:                  /* (origin) Longitude (attribute) */
	    nullPtr = (void *) &nullOrigin.lon ;
	    break ;
        case dbl_ORIGI_MB:                   /* (origin) Body Wave Mag. (attribute) */
	    nullPtr = (void *) &nullOrigin.mb ;
	    break ;
        case dbl_ORIGI_MBID:                 /* (origin) Mag. ID for Mb (attribute) */
	    nullPtr = (void *) &nullOrigin.mbid ;
	    break ;
        case dbl_ORIGI_ML:                   /* (origin) Local Magnitude (attribute) */
	    nullPtr = (void *) &nullOrigin.ml ;
	    break ;
        case dbl_ORIGI_MLID:                 /* (origin) Mad. ID for Ml (attribute) */
	    nullPtr = (void *) &nullOrigin.mlid ;
	    break ;
        case dbl_ORIGI_MS:                   /* (origin) Surface Wave Mag. (attribute) */
	    nullPtr = (void *) &nullOrigin.ms ;
	    break ;
        case dbl_ORIGI_MSID:                 /* (origin) Mag. ID for MS (attribute) */
	    nullPtr = (void *) &nullOrigin.msid ;
	    break ;
        case dbl_ORIGI_NASS:                 /* (origin) Number of Associated Arrivals (attribute) */
	    nullPtr = (void *) &nullOrigin.nass ;
	    break ;
        case dbl_ORIGI_NDEF:                 /* (origin) Number of arrival used (attribute) */
	    nullPtr = (void *) &nullOrigin.ndef ;
	    break ;
        case dbl_ORIGI_NDP:                  /* (origin) Number of Depth Phases (attribute) */
	    nullPtr = (void *) &nullOrigin.ndp ;
	    break ;
        case dbl_ORIGI_ORID:                 /* (origin) Origin Identification (attribute) */
	    nullPtr = (void *) &nullOrigin.orid ;
	    break ;
        case dbl_ORIGI_SRN:                  /* (origin) Region Number (attribute) */
	    nullPtr = (void *) &nullOrigin.srn ;
	    break ;
        case dbl_ORIGI_TIME:                 /* (origin) Epoch Time (attribute) */
	    nullPtr = (void *) &nullOrigin.time ;
	    break ;
        case dbl_ORIGI_LDDATE:               /* (origin) Load Date (attribute) */
	    nullPtr = (void *) &nullOrigin.lddate ;
	    break ;
    } /* end switch */

    return nullPtr ;
}

void * findNullForRemarkField ( int field )
{
    void *nullPtr = NULL ;

    switch ( field ) {
        case dbl_REMA_COMMID:                /* (remark) comment ID (attribute) */
	    nullPtr = (void *) &nullRemark.commid ;
	    break ;
        case dbl_REMA_LINENO:                /* (remark) comment line number (attribute) */
	    nullPtr = (void *) &nullRemark.lineno ;
	    break ;
        case dbl_REMA_REMARK:                /* (remark) comment string (attribute) */
	    nullPtr = (void *) &nullRemark.remark ;
	    break ;
        case dbl_REMA_LDDATE:                /* (remark) load date (attribute) */
	    nullPtr = (void *) &nullRemark.lddate ;
	    break ;
    } /* end switch */

    return nullPtr ;
}

void * findNullForSensorField ( int field )
{
    void *nullPtr = NULL ;

    switch ( field ) {
        case dbl_SENS_CALPER:                /* (sensor) Calibration Period (attribute) */
	    nullPtr = (void *) &nullSensor.calper ;
	    break ;
        case dbl_SENS_CALRATIO:              /* (sensor) Calib conversion ratio (attribute) */
	    nullPtr = (void *) &nullSensor.calratio ;
	    break ;
        case dbl_SENS_CHAN:                  /* (sensor) channel identifier (attribute) */
	    nullPtr = (void *) &nullSensor.chan ;
	    break ;
        case dbl_SENS_CHANID:                /* (sensor) Chan Recording ID (attribute) */
	    nullPtr = (void *) &nullSensor.chanid ;
	    break ;
        case dbl_SENS_ENDTIME:               /* (sensor) Time of last datum (attribute) */
	    nullPtr = (void *) &nullSensor.endtime ;
	    break ;
        case dbl_SENS_INID:                  /* (sensor) Instrument Identifier (attribute) */
	    nullPtr = (void *) &nullSensor.inid ;
	    break ;
        case dbl_SENS_INSTANT:               /* (sensor) Snapshot indicator (attribute) */
	    nullPtr = (void *) &nullSensor.instant ;
	    break ;
        case dbl_SENS_JDATE:                 /* (sensor) Julian Date (attribute) */
	    nullPtr = (void *) &nullSensor.jdate ;
	    break ;
        case dbl_SENS_STA:                   /* (sensor) Station Code (attribute) */
	    nullPtr = (void *) &nullSensor.sta ;
	    break ;
        case dbl_SENS_TIME:                  /* (sensor) Epoch Time (attribute) */
	    nullPtr = (void *) &nullSensor.time ;
	    break ;
        case dbl_SENS_TSHIFT:                /* (sensor) Correction for Clock Errors (attribute) */
	    nullPtr = (void *) &nullSensor.tshift ;
	    break ;
        case dbl_SENS_LDDATE:                /* (sensor) Load Date (attribute) */
	    nullPtr = (void *) &nullSensor.lddate ;
	    break ;
    } /* end switch */

    return nullPtr ;
}

void * findNullForSiteField ( int field )
{
    void *nullPtr = NULL ;

    switch ( field ) {
        case dbl_SITE_DEAST:                 /* (site) Distance East (attribute) */
	    nullPtr = (void *) &nullSite.deast ;
	    break ;
        case dbl_SITE_DNORTH:                /* (site) Distance North (attribute) */
	    nullPtr = (void *) &nullSite.dnorth ;
	    break ;
        case dbl_SITE_ELEV:                  /* (site) Elevation (attribute) */
	    nullPtr = (void *) &nullSite.elev ;
	    break ;
        case dbl_SITE_LAT:                   /* (site) Latitude (attribute) */
	    nullPtr = (void *) &nullSite.lat ;
	    break ;
        case dbl_SITE_LON:                   /* (site) Longitude (attribute) */
	    nullPtr = (void *) &nullSite.lon ;
	    break ;
        case dbl_SITE_OFFDATE:               /* (site) Turn-off Date (attribute) */
	    nullPtr = (void *) &nullSite.offdate ;
	    break ;
        case dbl_SITE_ONDATE:                /* (site) Turn-on Date (attribute) */
	    nullPtr = (void *) &nullSite.ondate ;
	    break ;
        case dbl_SITE_REFSTA:                /* (site) Reference Station (attribute) */
	    nullPtr = (void *) &nullSite.refsta ;
	    break ;
        case dbl_SITE_STA:                   /* (site) Station Code (attribute) */
	    nullPtr = (void *) &nullSite.sta ;
	    break ;
        case dbl_SITE_STANAME:               /* (site) Station Name (attribute) */
	    nullPtr = (void *) &nullSite.staname ;
	    break ;
        case dbl_SITE_STATYPE:               /* (site) Station Type (attribute) */
	    nullPtr = (void *) &nullSite.statype ;
	    break ;
        case dbl_SITE_LDDATE:                /* (site) Load Date (attribute) */
	    nullPtr = (void *) &nullSite.lddate ;
	    break ;
    } /* end switch */

    return nullPtr ;
}

void * findNullForSitechanField ( int field )
{
    void *nullPtr = NULL ;

    switch ( field ) {
        case dbl_SITEC_CHAN:                 /* (sitechan) channel identifier (attribute) */
	    nullPtr = (void *) &nullSitechan.chan ;
	    break ;
        case dbl_SITEC_CHANID:               /* (sitechan) Chan Recording ID (attribute) */
	    nullPtr = (void *) &nullSitechan.chanid ;
	    break ;
        case dbl_SITEC_CTYPE:                /* (sitechan) Channel Type (attribute) */
	    nullPtr = (void *) &nullSitechan.ctype ;
	    break ;
        case dbl_SITEC_DESCRIP:              /* (sitechan) Channel Description (attribute) */
	    nullPtr = (void *) &nullSitechan.descrip ;
	    break ;
        case dbl_SITEC_EDEPTH:               /* (sitechan) Emplacement Depth (attribute) */
	    nullPtr = (void *) &nullSitechan.edepth ;
	    break ;
        case dbl_SITEC_HANG:                 /* (sitechan) Horizontal orientation (attribute) */
	    nullPtr = (void *) &nullSitechan.hang ;
	    break ;
        case dbl_SITEC_OFFDATE:              /* (sitechan) Turn-off Date (attribute) */
	    nullPtr = (void *) &nullSitechan.offdate ;
	    break ;
        case dbl_SITEC_ONDATE:               /* (sitechan) Turn-on Date (attribute) */
	    nullPtr = (void *) &nullSitechan.ondate ;
	    break ;
        case dbl_SITEC_STA:                  /* (sitechan) Station Code (attribute) */
	    nullPtr = (void *) &nullSitechan.sta ;
	    break ;
        case dbl_SITEC_VANG:                 /* (sitechan) Vert. Orientation of Seis.  (attribute) */
	    nullPtr = (void *) &nullSitechan.vang ;
	    break ;
        case dbl_SITEC_LDDATE:               /* (sitechan) Load Date (attribute) */
	    nullPtr = (void *) &nullSitechan.lddate ;
	    break ;
    } /* end switch */

    return nullPtr ;
}


void * findNullForStassocField ( int field )
{
    void *nullPtr = NULL ;

    switch ( field ) {
        case dbl_STASS_AUTH:                 /* (stassoc) Author (attribute) */
	    nullPtr = (void *) &nullStassoc.auth ;
	    break ;
        case dbl_STASS_AZIMUTH:              /* (stassoc) Observed Azimuth (attribute) */
	    nullPtr = (void *) &nullStassoc.azimuth ;
	    break ;
        case dbl_STASS_COMMID:               /* (stassoc) comment ID (attribute) */
	    nullPtr = (void *) &nullStassoc.commid ;
	    break ;
        case dbl_STASS_DEPTH:                /* (stassoc) Source Depth (attribute) */
	    nullPtr = (void *) &nullStassoc.depth ;
	    break ;
        case dbl_STASS_DIST:                 /* (stassoc) Estimated Distance (attribute) */
	    nullPtr = (void *) &nullStassoc.dist ;
	    break ;
        case dbl_STASS_ETYPE:                /* (stassoc) Event type (attribute) */
	    nullPtr = (void *) &nullStassoc.etype ;
	    break ;
        case dbl_STASS_IMB:                  /* (stassoc) Initial Body wave Mag. (attribute) */
	    nullPtr = (void *) &nullStassoc.imb ;
	    break ;
        case dbl_STASS_IML:                  /* (stassoc) Initial Local Mag. (attribute) */
	    nullPtr = (void *) &nullStassoc.iml ;
	    break ;
        case dbl_STASS_IMS:                  /* (stassoc) Initial Surface wave Mag. (attribute) */
	    nullPtr = (void *) &nullStassoc.ims ;
	    break ;
        case dbl_STASS_LAT:                  /* (stassoc) Latitude (attribute) */
	    nullPtr = (void *) &nullStassoc.lat ;
	    break ;
        case dbl_STASS_LOCATION:             /* (stassoc) Location Description. (attribute) */
	    nullPtr = (void *) &nullStassoc.location ;
	    break ;
        case dbl_STASS_LON:                  /* (stassoc) Longitude (attribute) */
	    nullPtr = (void *) &nullStassoc.lon ;
	    break ;
        case dbl_STASS_STA:                  /* (stassoc) Station Code (attribute) */
	    nullPtr = (void *) &nullStassoc.sta ;
	    break ;
        case dbl_STASS_STASSID:              /* (stassoc) Sta-assoc ID (attribute) */
	    nullPtr = (void *) &nullStassoc.stassid ;
	    break ;
        case dbl_STASS_TIME:                 /* (stassoc) Epoch Time (attribute) */
	    nullPtr = (void *) &nullStassoc.time ;
	    break ;
        case dbl_STASS_LDDATE:               /* (stassoc) Load Date (attribute) */
	    nullPtr = (void *) &nullStassoc.lddate ;
	    break ;
    } /* end switch */

    return nullPtr ;
}

void * findNullForWfdiscField ( int field )
{
    void *nullPtr = NULL ;

    switch ( field ) {
        case dbl_WFDIS_CALIB:                /* (wfdisc) Calibration Factor (attribute) */
	    nullPtr = (void *) &wfdisc_null.calib ;
	    break ;
        case dbl_WFDIS_CALPER:               /* (wfdisc) Calibration Period (attribute) */
	    nullPtr = (void *) &wfdisc_null.calper ;
	    break ;
        case dbl_WFDIS_CHAN:                 /* (wfdisc) channel identifier (attribute) */
	    nullPtr = (void *) &wfdisc_null.chan ;
	    break ;
        case dbl_WFDIS_CHANID:               /* (wfdisc) Chan Recording ID (attribute) */
	    nullPtr = (void *) &wfdisc_null.chanid ;
	    break ;
        case dbl_WFDIS_CLIP:                 /* (wfdisc) clipped data flag (attribute) */
	    nullPtr = (void *) &wfdisc_null.clip ;
	    break ;
        case dbl_WFDIS_COMMID:               /* (wfdisc) comment ID (attribute) */
	    nullPtr = (void *) &wfdisc_null.commid ;
	    break ;
        case dbl_WFDIS_DATATYPE:             /* (wfdisc) Numeric storage (attribute) */
	    nullPtr = (void *) &wfdisc_null.dattype ;
	    break ;
        case dbl_WFDIS_DFILE:                /* (wfdisc) Data file (attribute) */
	    nullPtr = (void *) &wfdisc_null.dfile ;
	    break ;
        case dbl_WFDIS_DIR:                  /* (wfdisc) directory (attribute) */
	    nullPtr = (void *) &wfdisc_null.dir ;
	    break ;
        case dbl_WFDIS_ENDTIME:              /* (wfdisc) Time of last datum (attribute) */
	    nullPtr = (void *) &wfdisc_null.endtime ;
	    break ;
        case dbl_WFDIS_FOFF:                 /* (wfdisc) File offset (attribute) */
	    nullPtr = (void *) &wfdisc_null.foff ;
	    break ;
        case dbl_WFDIS_INSTYPE:              /* (wfdisc) Instrument Type (attribute) */
	    nullPtr = (void *) &wfdisc_null.instype ;
	    break ;
        case dbl_WFDIS_JDATE:                /* (wfdisc) Julian Date (attribute) */
	    nullPtr = (void *) &wfdisc_null.jdate ;
	    break ;
        case dbl_WFDIS_NSAMP:                /* (wfdisc) Number of Samples (attribute) */
	    nullPtr = (void *) &wfdisc_null.nsamp ;
	    break ;
        case dbl_WFDIS_SAMPRATE:             /* (wfdisc) Sampling Rate (attribute) */
	    nullPtr = (void *) &wfdisc_null.samprate ;
	    break ;
        case dbl_WFDIS_SEGTYPE:              /* (wfdisc) Segment Type (attribute) */
	    nullPtr = (void *) &wfdisc_null.segtype ;
	    break ;
        case dbl_WFDIS_STA:                  /* (wfdisc) Station Code (attribute) */
	    nullPtr = (void *) &wfdisc_null.sta ;
	    break ;
        case dbl_WFDIS_TIME:                 /* (wfdisc) Epoch Time (attribute) */
	    nullPtr = (void *) &wfdisc_null.time ;
	    break ;
        case dbl_WFDIS_WFID:                 /* (wfdisc) Waveform Identifier (attribute) */
	    nullPtr = (void *) &wfdisc_null.wfid ;
	    break ;
        case dbl_WFDIS_LDDATE:               /* (wfdisc) Load Date (attribute) */
	    nullPtr = (void *) &wfdisc_null.lddate ;
	    break ;
/*        case dbl_WFDIS_REAL_TRC:             * (wfdisc) pointer to real trace (attribute) *
	    nullPtr = NULL ;
	    break ;
        case dbl_WFDIS_IMAG_TRC:             * (wfdisc) pointer to imag trace (attribute) *
	    nullPtr = NULL ;
	    break ;
        case dbl_WFDIS_COMPLEX:              * (wfdisc) complex flag (attribute) *
	    nullPtr = NULL ;
	    break ;		*/
    } /* end switch */

    return nullPtr ;
}


void * findNullForWftagField ( int field )
{
    void *nullPtr = NULL ;

    switch ( field ) {
        case dbl_WFTAG_TAGNAME:              /* (wftag) key (arid, evid,...) (attribute) */
	    nullPtr = (void *) &nullWftag.tagname ;
	    break ;
        case dbl_WFTAG_TAGID:                /* (wftag) tagname value (attribute) */
	    nullPtr = (void *) &nullWftag.tagid ;
	    break ;
        case dbl_WFTAG_WFID:                 /* (wftag) waveform ID (attribute) */
	    nullPtr = (void *) &nullWftag.wfid ;
	    break ;
        case dbl_WFTAG_LDDATE:               /* (wftag) load date (attribute) */
	    nullPtr = (void *) &nullWftag.lddate ;
	    break ;
    } /* end switch */

    return nullPtr ;
}


void *findNullForField ( int table , int field )
{
    void * nullPtr = NULL ;

    switch ( table )
    {
        case dbl_LIST_AFFILIATION:           /* Affiliation Table */
	    nullPtr = findNullForAffiliationField ( field ) ;
	    break ;
        case dbl_LIST_ARRIVAL:               /* Arrival Table */
	    nullPtr = findNullForArrivalField ( field ) ;
	    break ;
        case dbl_LIST_ASSOC:                 /* Assoc Table */
	    nullPtr = findNullForAssocField ( field ) ;
	    break ;
        case dbl_LIST_EVENT:                 /* Event Table */
	    nullPtr = findNullForEventField ( field ) ;
	    break ;
        case dbl_LIST_GREGION:               /* Gregion Table */
	    nullPtr = findNullForGregionField ( field ) ;
	    break ;
        case dbl_LIST_INSTRUMENT:            /* Instrument Table */
	    nullPtr = findNullForInstrumentField ( field ) ;
	    break ;
        case dbl_LIST_ORIGERR:               /* Origerr Table */
	    nullPtr = findNullForOrigerrField ( field ) ;
	    break ;
        case dbl_LIST_ORIGIN:                /* Origin Table */
	    nullPtr = findNullForOriginField ( field ) ;
	    break ;
        case dbl_LIST_REMARK:                /* Remark Table */
	    nullPtr = findNullForRemarkField ( field ) ;
	    break ;
        case dbl_LIST_SENSOR:                /* Sensor Table */
	    nullPtr = findNullForSensorField ( field ) ;
	    break ;
        case dbl_LIST_SITE:                  /* Site Table */
	    nullPtr = findNullForSiteField ( field ) ;
	    break ;
        case dbl_LIST_SITECHAN:              /* Sitechan Table */
	    nullPtr = findNullForSitechanField ( field ) ;
	    break ;
        case dbl_LIST_STASSOC:               /* Stassoc Table */
	    nullPtr = findNullForStassocField ( field ) ;
	    break ;
        case dbl_LIST_WFDISC:                /* Wfdisc Table */
	    nullPtr = findNullForWfdiscField ( field ) ;
	    break ;
        case dbl_LIST_WFTAG:                 /* Wftag Table */
	    nullPtr = findNullForWftagField ( field ) ;
	    break ;
    } /* end switch */

    return nullPtr ;
} /* end findNullForField */


int isValidFloat ( int table , int field , double value ) 
{
    double * dNull = NULL ;

    if ( table == dbl_LIST_SACDATA ) {
	if ( value == -12345.0 )
	    return FALSE ;
	else
	    return TRUE ;
    }

    dNull = (double *) findNullForField ( table , field ) ;
    if(!dNull) {
        return FALSE;
    }
    if ( value == *dNull )
	return FALSE ;
    else
	return TRUE ;
} /* end isValidFloat */


int isValidInt ( int table , int field , int value )
{
    int * lNull = NULL ;

    if ( table == dbl_LIST_SACDATA ) {
	if ( field == dbl_SACD_WFID && value == 0 )
	    return FALSE ;
	else if ( value == -12345 )
	    return FALSE ;
	else
	    return TRUE ;
    }

    lNull = (int *) findNullForField ( table , field ) ;
    if(!lNull) {
        return FALSE;
    }
    if ( value == *lNull )
	return FALSE ;
    else
	return TRUE ;
} /* end isValidInt */


int isValidString ( int table , int field , char *value )
{
    char * cNull = NULL ;

    if ( table == dbl_LIST_SACDATA ) {
	if ( strncmp ( value , "-12345" , 6 ) )
	    return FALSE ;
	else
	    return TRUE ;
    }

    cNull = (char *) findNullForField ( table , field ) ;
    if(!cNull) {
        return FALSE;
    }
    if ( strcmp ( value , cNull ) )
	return TRUE ;
    else
	return FALSE ;
} /* end isValidString */
