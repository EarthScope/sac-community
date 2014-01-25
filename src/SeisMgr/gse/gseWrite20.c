#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define NULL_SITECHAN
#include "cssListOps/cssStrucs.h"
#include "cssListOps/cssListStrucs.h"
#include "cssListOps/dblPublicDefs.h"
#include "cssListOps/cssListOps.h"
#include "gse.h"
#include "timefuncs.h"
#include "string_utils.h"


static const int wfError = -2 ;
static const int staError = -3 ;
static const int chanError = -4 ;
static const int arrError = -5 ;
static const int orError = -6 ;


static void dbDelaz(float *slat, float *slon, float *elat, float *elon,
		    float *delt, float *dist, float *azim, float *bazim) ;

int cmprs6( int LX, int *IX, char **CBUFF ) ;
void takeDiff( int *data, int npts ) ;
void remdif1( int *data, int npts);

int stringMatch ( char *first , char *second , int num )
{
	int fiLen, seLen , idx;

	if ( !strncmp ( first , second , num ) )
	    return 1 ;

	fiLen = strlen ( first ) ;
	if ( num < fiLen )
	    fiLen = num ;
	seLen = strlen ( second ) ;
	if ( num < seLen )
	    seLen = num ;

	if ( fiLen > seLen ) {
	    for ( idx = seLen ; idx < fiLen ; idx++ )
		if ( first[ idx ] != ' ' ) 
		    return 0 ;
	}
	else if ( seLen > fiLen ) {
	    for ( idx = fiLen ; idx < seLen ; idx++ )
		if ( second[ idx ] != ' ' )
		    return 0 ;
	}

	if ( !strncmp ( first , second , fiLen < seLen ? fiLen : seLen ) )
	    return 1 ;

	return 0 ;
} /* end stringMatch */


void GetChannelInfo(struct sitechanList *sc, char *Station, char *comp,
                    double *hang, double *vang)
{
    *hang = nullSitechan.hang ;
    *vang = nullSitechan.vang ;

    if(!sc) 
	return;

    while(sc){
	if( stringMatch (Station, sc->element->sta, 5 ) &&
	    stringMatch (comp , sc->element->chan , 3 ) ) {
	    *hang = sc->element->hang;
	    *vang = sc->element->vang;
	    return;
	}

	sc = sc->next;
    }

} /* end GetChannelInfo */


void GetSiteInfo ( struct siteList *si , char *sta , int ondate ,
		   int offdate, float *lat , float *lon , float *elev )
{
    *lat = *lon = *elev = -999.0 ;

    if ( !si )
	return ;

    while ( si ) {
	if ( stringMatch ( sta , si->element->sta , 5 ) &&
	     ondate == si->element->ondate &&
	     offdate == si->element->offdate ) {
		*lat = si->element->lat ;
		*lon = si->element->lon ;
		*elev = si->element->elev ;
		return ;
	}

	si = si->next ;
    }

} /* end GetSiteInfo */


void GetWfdiscInfo ( struct wfdiscList *wf, char *sta , char *chan ,
		     int ondate , int offdate , char * inst ) 
{
    strcpy ( inst , "-" ) ;

    if ( !wf )
	return ;

    while ( wf ) {
	if ( stringMatch ( sta , wf->element->sta , 5 ) &&
	     stringMatch ( chan, wf->element->chan, 3 ) &&
	     ondate  <= wf->element->jdate &&
	     offdate >= wf->element->jdate ) {
		strcpy ( inst , wf->element->instype ) ;
		return ;
	}

	wf = wf->next ;
    }
} /* end GetWfdiscInfo */


void PutIntro ( FILE *ptr , char * datasource ) 
{
    char today[ 25 ] , filedate[ 25 ]  = "0000/00/00" ;
    int  unique = 1 ;
    FILE *togse_date_number = NULL ;

    fprintf(ptr, "BEGIN GSE2.0\n");
    fprintf(ptr, "MSG_TYPE DATA\n");

    strcpy ( today , tmListEpochTime( tmGetEpochTime(), 14 ) );
    today[ 4 ] = '/' ;
    today[ 10 ] = '\0' ;

    togse_date_number = fopen ( "/tmp/togse_date_number" , "r" ) ;
    if ( togse_date_number ) {
	fgetsp ( filedate , 24 , togse_date_number ) ;
	filedate[ 10 ] = '\0' ;
	if ( !strcmp ( filedate , today ) ) {
	    fscanf ( togse_date_number , "%d" , &unique ) ;
	}
	fclose ( togse_date_number ) ;
    }

    if (( togse_date_number = fopen ( "/tmp/togse_date_number" , "w" ) )) {
	fprintf ( togse_date_number , "%s\n%d" , today , unique+1 ) ;
	fclose ( togse_date_number ) ;
    }
    
    fprintf ( ptr , "MSG_ID %s_%04d %s\n" , today , unique , datasource ) ;
} /* end PutIntro */





int PutWaveformData ( FILE *ptr , DBlist tree , int cm6 )
{
    char id	[ 5 ] ,
	time	[ 24 ] ,
	station	[ 6 ] ,
	channel	[ 4 ] ,
	auxid	[ 5 ] ,
	datatype[ 4 ] ,
	samps	[ 9 ] ,
	samprat	[ 12 ] ,
	calib	[ 11 ] ,
	calper	[ 9 ] ;
    char instype[ ] = "      ",
	hang	[ 6 ] ,
	vang	[ 5 ] ;

    double dHang , dVang ;

    int Ntraces = 0 ;

    int dots = 0 ;
    int jdx ;
    int column ;
    int MaxColumns = 15 ;

    struct CSStree *Tree = (struct CSStree *) tree ;
    struct wfdiscList *wfL ;

    printf ( "Converting waveforms " ) ;
    for ( wfL = Tree->wfHead ; wfL ; wfL = wfL->next , Ntraces++ ) {
	int *iData ;
	struct sitechanList *sc = Tree->scHead ;

	/* display dots for the users sake */
	putchar('.');
	dots++;
	if(dots == 80){
	    putchar('\n');
	    dots = 0;
	}
	fflush(stdout);

	/* data_type record */
	fprintf(ptr,"DATA_TYPE WAVEFORM\n");

	/* BEGIN WID2 RECORD */

	/* id */
	strcpy ( id , "WID2" ) ;

	/* date and time */
	if(isValidFloat( dbl_LIST_WFDISC, dbl_WFDIS_TIME, wfL->element->time )){
	    sprintf ( time , "%-23.23s" , tmListEpochTime ( wfL->element->time , 14 ) ) ;
	    time[ 10 ] = ' ' ;
	}
	else
	    strcpy ( time , "                       " ) ;	/* 23 spaces */
	    
	/* station */
	if ( isValidString ( dbl_LIST_WFDISC, dbl_WFDIS_STA , wfL->element->sta ) &&
             strcmp( wfL->element->sta , "-    " ) ) 
	    sprintf ( station , "%-5.5s" , wfL->element->sta ) ;
	else
	    strcpy ( station , "     " ) ;	/* 5 spaces */

	/* channel */
	if ( isValidString ( dbl_LIST_WFDISC, dbl_WFDIS_CHAN , wfL->element->chan ) &&
             strcmp( wfL->element->chan , "-  " )  )
	    sprintf ( channel , "%-3.3s" , wfL->element->chan ) ;
	else
	    strcpy ( channel , "   " ) ;	/* 3 spaces */

	/* auxid */
	strcpy ( auxid , "    " ) ;		/* 4 spaces */

	/* datatype */
        if( cm6 )
	    strcpy ( datatype , "CM6" ) ;       /* 3 spaces */
        else
	    strcpy ( datatype , "INT" ) ;       /* 3 spaces */

	/* samps */
	if ( isValidInt ( dbl_LIST_WFDISC, dbl_WFDIS_NSAMP , wfL->element->nsamp ) )
	    sprintf ( samps , "%8d" , wfL->element->nsamp ) ;
	else {
	    printf ( "Warning:  file %d had no value for samps\n" , Ntraces + 1 ) ;
	    continue ;
	}

	/* samprat */
	if ( isValidFloat ( dbl_LIST_WFDISC, dbl_WFDIS_SAMPRATE , wfL->element->samprate ) )
	    sprintf ( samprat , "%11.6f" , wfL->element->samprate ) ;
	else
	    strcpy ( samprat , "           " ) ;/* 11 spaces */

	/* calib */
	if ( isValidFloat ( dbl_LIST_WFDISC, dbl_WFDIS_CALIB , wfL->element->calib ) )
	    sprintf ( calib , "%10.2e" , wfL->element->calib ) ;
	else
	    strcpy ( calib , "          " ) ;	/* 10 spaces */

	/* calper */
        if ( isValidFloat ( dbl_LIST_WFDISC, dbl_WFDIS_CALPER , wfL->element->calper ) )
            sprintf ( calper , "%7.3f" , wfL->element->calper ) ;
        else
            sprintf ( calper , "%s", "        " ) ;   /* 8 spaces */

	/* instype */
	if ( isValidString ( dbl_LIST_WFDISC, dbl_WFDIS_INSTYPE , wfL->element->instype ) &&
             strcmp( wfL->element->instype , "-     " )  )
	    sprintf ( instype , "%-6.6s" , wfL->element->instype ) ;
	else
	    strcpy ( instype , "      " ) ;	/* 6 spaces */

	/* hang and vang */
	GetChannelInfo(sc, station, channel, &dHang, &dVang);
	if ( isValidFloat ( dbl_LIST_SITECHAN, dbl_SITEC_HANG , dHang ) )
	    sprintf ( hang , "%5.1f" , dHang ) ;
	else
	    strcpy ( hang , "     " ) ;		/* 5 spaces */

	if ( isValidFloat ( dbl_LIST_SITECHAN, dbl_SITEC_VANG , dVang ) )
            sprintf ( vang , "%4.1f" , dVang ) ;
        else
            strcpy ( vang , "    " ) ;         /* 4 spaces */

	fprintf(ptr, "%s %s %s %s %s %s %s %s %s %s %s %s %s\n", id, time, station,
	  channel, auxid, datatype, samps, samprat, calib, calper, instype, hang, vang);
	/* END WID2 RECORD */


	/* dat2 record */
	fprintf(ptr,"DAT2\n");

	column = 0 ;

	if( !wfL->seis ) {
	    printf ( "ERROR: Data not found!\n " ) ;
	    return ( -1 ) ;
	}

	/* waveform */
	iData = (int *) malloc ( wfL->element->nsamp * sizeof( int ) ) ;
	if ( !iData ) {
	    printf ( "Error:  insufficient memory, PutWavefomrData\n" ) ;
	    return 0 ;
	}
	for ( jdx = 0 ; jdx < wfL->element->nsamp ; jdx++ ) {
	    double point = wfL->seis->i[ jdx ] ;

	    point = point >= 0 ? point + 0.5 : point - 0.5 ;
	    iData[ jdx ] = (int) point ;

            if( !cm6 ){    /* INT format, one point at a time. */
	        fprintf ( ptr , "%d " , (int) point ) ;
	        if ( ++column == MaxColumns ) {
		    fprintf ( ptr , "\n" ) ;
		    column = 0 ;
	        }
            }
	}
        if( cm6 ){
	    char *cOut ;  /* data compressed into CM6 format. */

            takeDiff( iData , wfL->element->nsamp ) ;

	    /* call cmprs6 to get string of chars */
	    if( cmprs6( wfL->element->nsamp , iData , &cOut ) ) {
                printf ( "Error:  insufficient memory, PutWavefomrData\n" ) ;
                free( cOut );
                return 0 ;
            }

	    /* write it out (it already has newlines every 80 chars.) */
	    fprintf ( ptr , "%s", cOut ) ;
            fprintf ( ptr , "\n" ) ;

            free( cOut ) ;

            remdif1( iData , wfL->element->nsamp ) ;
            remdif1( iData , wfL->element->nsamp ) ;
        }
	else
	    if ( column ) fprintf ( ptr , "\n" ) ;

	fprintf(ptr,"CHK2  %d\n\n", CheckSumFromIntArray(iData, wfL->element->nsamp) );
	free ( iData ) ;

    } /* end for */

    printf ( "\n" ) ;

    return Ntraces ;
} /* end PutWaveformData */



int PutStationData ( FILE *ptr , DBlist tree )
{
    char sta	[ 6 ] ,
	 statype[ 5 ] ,
	 lat	[ 10 ] ,
	 lon	[ 11 ] ,
	 elev	[ 8 ] ,
	 ondate	[ 25 ] ,
	 offdate[ 25 ] ;

    int dots = 0;

    struct CSStree *Tree = (struct CSStree *) tree ;
    struct siteList *si = Tree->slHead ;

    if(!si)
	return -1 ;

    printf("Writing station data ");
    fprintf(ptr,"DATA_TYPE STATION\n");
    fprintf(ptr, "Sta   Type  Latitude  Longitude    Elev    On Date   Off Date\n");

    while(si){
	putchar('.');
	dots++;
	if(dots == 80){
	    putchar('\n');
	    dots = 0;
	}
	fflush(stdout);

	/* sta */
        if ( isValidString ( dbl_LIST_SITE, dbl_SITE_STA , si->element->sta ) &&
             strcmp( si->element->sta , "-    " )  )
            sprintf ( sta , "%-5.5s" , si->element->sta ) ;
        else
            strcpy ( sta , "     " ) ;   /* 5 spaces */

	/* statype */
	if ( isValidString ( dbl_LIST_SITE, dbl_SITE_STATYPE , si->element->statype ) &&
             strcmp( si->element->statype , "-   " )  )
	    sprintf ( statype , "%-4.4s" , si->element->statype ) ;
	else
            strcpy ( statype , "    " ) ;   /* 4 spaces */

	/* lat */
        if ( isValidFloat ( dbl_LIST_SITE, dbl_SITE_LAT , si->element->lat ) )
            sprintf ( lat , "%9.5f" , si->element->lat ) ;
        else
            strcpy ( lat , "         " ) ;   /* 9 spaces */

	/* lon */
        if ( isValidFloat ( dbl_LIST_SITE, dbl_SITE_LON , si->element->lon ) )
            sprintf ( lon , "%10.5f" , si->element->lon ) ;
        else
            strcpy ( lon , "          " ) ;   /* 10 spaces */

	/* elev */
        if ( isValidFloat ( dbl_LIST_SITE, dbl_SITE_ELEV , si->element->elev ) )
            sprintf ( elev , "%7.3f" , si->element->elev ) ;
        else
            strcpy ( elev , "       " ) ;	/* 7 spaces */

	/* ondate */
	if ( isValidInt ( dbl_LIST_SITE, dbl_SITE_ONDATE , si->element->ondate ) ) {
	    int doy, mm, id, iyyy , jdate = si->element->ondate ;

	    iyyy = jdate / 1000 ;
	    doy  = jdate - ( iyyy * 1000 ) ;

	    mnday ( doy , isleap ( iyyy ) , &mm , &id ) ;

	    sprintf(ondate, "%04d/%02d/%02d" , iyyy, mm, id );
	}
	else
	    strcpy ( ondate , "          " ) ;	/* 10 spaces */

	/* offdate */
	if ( isValidInt ( dbl_LIST_SITE, dbl_SITE_OFFDATE , si->element->offdate ) ) {
            int doy, mm, id, iyyy , jdate = si->element->offdate ;

            iyyy = jdate / 1000 ;
            doy  = jdate - ( iyyy * 1000 ) ;

            mnday ( doy , isleap ( iyyy ) , &mm , &id ) ;

            sprintf(offdate, "%04d/%02d/%02d" , iyyy, mm, id );
	}
	else
	    strcpy ( offdate , "          " ) ;    /* 10 spaces */	

	fprintf(ptr,"%s %s %s %s %s %s %s\n" ,
	  sta, statype, lat, lon, elev , ondate, offdate );
	si = si->next;
    }
    fprintf(ptr,"\n");
    printf("\n");

    return 0 ;
} /* end PutStationData */


int PutChannelData ( FILE *ptr , DBlist tree )
{
   char sta[6],
	chan[4],
	auxid[ 5 ] ,
	lat[ 10 ] ,
	lon[ 11 ] ,
	elev[ 8 ] ,
	depth[ 7 ] ,
	hang[ 7 ],
	vang[ 6 ],
	samprat[ 12 ],
	inst[8],
	ondate[25],
	offdate[25];

    int dots = 0;

    float fLat , fLon , fElev ;

    struct CSStree *Tree = (struct CSStree *) tree ;
    struct sitechanList *sc = Tree->scHead ;


    if(!sc)
	return -1 ;

    printf("Writing channel data ");
    fprintf(ptr,"DATA_TYPE CHANNEL\n");
    fprintf(ptr,
     "Sta  Chan Aux   Latitude  Longitude    Elev  Depth   Hang  Vang Sample_Rate Inst       On Date   Off Date\n");

    while(sc){
	struct siteList *si = Tree->slHead ;
	struct wfdiscList *wf = Tree->wfHead ;

	putchar('.');
	dots++;
	if(dots == 80){
	    putchar('\n');
	    dots = 0;
	}
	fflush(stdout);

	/* sta */
        if ( isValidString ( dbl_LIST_SITECHAN, dbl_SITEC_STA , sc->element->sta ) &&
             strcmp( sc->element->sta , "-    " )  )
            sprintf ( sta , "%-5.5s" , sc->element->sta ) ;
        else
            strcpy ( sta , "     " ) ;         /* 5 spaces */

	/* chan */
        if ( isValidString ( dbl_LIST_SITECHAN, dbl_SITEC_CHAN , sc->element->chan ) &&
             strcmp( sc->element->chan , "-  " )  )
            sprintf ( chan , "%-3.3s" , sc->element->chan ) ;
        else
            strcpy ( chan , "   " ) ;         /* 3 spaces */

	/* auxid */
        strcpy ( auxid , "    " ) ;         /* 4 spaces */

	/* lat, lon, & elev */
        GetSiteInfo ( si , sc->element->sta , sc->element->ondate , sc->element->offdate ,
          &fLat , &fLon , &fElev ) ;

        if ( isValidFloat ( dbl_LIST_SITE, dbl_SITE_LAT , fLat ) )
            sprintf ( lat , "%9.5f" , fLat ) ;
        else
            strcpy ( lat , "         " ) ;         /* 9 spaces */

        if ( isValidFloat ( dbl_LIST_SITE, dbl_SITE_LON , fLon ) )
            sprintf ( lon , "%10.5f" , fLon ) ;
        else
            strcpy ( lon , "          " ) ;         /* 10 spaces */ 

        if ( isValidFloat ( dbl_LIST_SITE, dbl_SITE_ELEV , fElev ) )
            sprintf ( elev , "%7.3f" , fElev ) ;
        else
            strcpy ( elev , "       " ) ;         /* 7 spaces */

	/* depth */
        if ( isValidFloat ( dbl_LIST_SITECHAN, dbl_SITEC_EDEPTH , sc->element->edepth ) )
            sprintf ( depth , "%6.3f" , sc->element->edepth ) ;
        else
            strcpy ( depth , "      " ) ;         /* 6 spaces */

	/* hang & vang */
        if ( isValidFloat ( dbl_LIST_SITECHAN, dbl_SITEC_HANG , sc->element->hang ) )
            sprintf ( hang , "%6.1f" , sc->element->hang ) ;
        else
            strcpy ( hang , "      " ) ;         /* 6 spaces */

        if ( isValidFloat ( dbl_LIST_SITECHAN, dbl_SITEC_VANG , sc->element->vang ) )
            sprintf ( vang , "%5.1f" , sc->element->vang ) ;
        else
            strcpy ( vang , "     " ) ;         /* 5 spaces */

	/* samprat */
	strcpy ( samprat , "           " ) ;	/* 11 spaces */

	/* inst */
	GetWfdiscInfo ( wf, sta , chan , sc->element->ondate , sc->element->offdate , inst ) ;
	if ( isValidString ( dbl_LIST_WFDISC, dbl_WFDIS_INSTYPE , inst ) &&
             strcmp( inst , "-      " )  ) {
	    if ( strlen ( inst ) < 7 ) {
		int kdx ;
		for ( kdx = strlen ( inst ) ; kdx < 7 ; kdx++ ) {
		    inst[ kdx ] = ' ' ;
		}
		inst[ 7 ] = '\0' ;
	    }
	}
	else
	    sprintf ( inst , "       " ) ;	/* 7 spaces */

	/* ondate */
        if ( isValidInt ( dbl_LIST_SITECHAN, dbl_SITEC_ONDATE , sc->element->ondate ) ) {
            int doy, mm, id, iyyy , jdate = sc->element->ondate ;

            iyyy = jdate / 1000 ;
            doy  = jdate - ( iyyy * 1000 ) ;

            mnday ( doy , isleap ( iyyy ) , &mm , &id ) ;

            sprintf(ondate, "%04d/%02d/%02d" , iyyy, mm, id );
        }
        else
            strcpy ( ondate , "          " ) ;  /* 10 spaces */

	/* offdate */
        if ( isValidInt ( dbl_LIST_SITECHAN, dbl_SITEC_OFFDATE , sc->element->offdate ) ) {
            int doy, mm, id, iyyy , jdate = sc->element->offdate ;

            iyyy = jdate / 1000 ;
            doy  = jdate - ( iyyy * 1000 ) ;

            mnday ( doy , isleap ( iyyy ) , &mm , &id ) ;

            sprintf(offdate, "%04d/%02d/%02d" , iyyy, mm, id );
        }
        else
            strcpy ( offdate , "          " ) ;  /* 10 spaces */


	fprintf(ptr,"%s %s %s %s %s %s %s %s %s %s %s %s %s\n", sta, chan, auxid,
              lat, lon, elev, depth, hang, vang, samprat, inst, ondate, offdate );

	sc = sc->next;
    }
    fprintf(ptr,"\n");
    printf("\n");

    return 0 ;
} /* end PutChannelData */


int PutArrivalData ( FILE *ptr , DBlist tree )
{
    char sta[ 6 ] ,
	 dist[ 7 ] ,
	 evaz[ 6 ] ,
	 picktype = ' ' ,
	 direction ,
	 detchar ,
	 phase[ 8 ] ,
	 time[ 25 ] ,
	 tres[] = "     " ,
	 azim[ 6 ] ,
	 azres[ 7 ] ,
	 slow[ 6 ] ,
	 sres[ 6 ] ,
	 tdef ,
	 adef ,
	 sdef ,
	 snr[ 6 ] ,
	 amp[ 10 ] ,
	 per[ 6 ] ,
	 mdef1[ 3 ] ,
	 mag1[ 5 ] ,
	 mdef2[ 3 ] ,
	 mag2[ 5 ] ,
	 id [ 9 ] ;

    struct CSStree *Tree = (struct CSStree *) tree ;
    struct arrivalList *ar = Tree->arHead ;

    int dots = 0;
    char HeaderLine[] = "Sta     Dist  EvAz     Phase      Date       Time     TRes  Azim"
                       "  AzRes  Slow  SRes Def   SNR       Amp   Per   Mag1   Mag2       ID";

    if(!ar)
	return -1 ;


    printf("Writing arrival data ");
    fprintf(ptr,"DATA_TYPE ARRIVAL GSE2.0\n");
    fprintf(ptr, "%s\n", HeaderLine);

    while(ar){
	struct assocList *as = Tree->asHead ;
	struct originList *orig = Tree->orHead ;
	struct siteList *si = Tree->slHead ;

	putchar('.');
	dots++;
	if(dots == 80){
	    putchar('\n');
	    dots = 0;
	}
	fflush(stdout);

	/* sta */
        if ( isValidString ( dbl_LIST_ARRIVAL, dbl_ARRIV_STA , ar->element->sta ) &&
             strcmp( ar->element->sta , "-    " )  )
            sprintf ( sta , "%-5.5s" , ar->element->sta ) ;
        else
            strcpy ( sta , "     " ) ;         /* 5 spaces */

	/* dist & evaz */
	/* find assoc in order to find origin, then find sitechan */
	while ( as ) {
	    if ( as->element->arid == ar->element->arid )
		break ;
	    as = as->next ;
	}

	if ( as )
	    while ( orig ) {
		if ( as->element->orid == orig->element->orid )
		    break ;
		orig = orig->next ;
	    } /* end while ( orig ) */

	while ( si ) {
	    if ( !strcmp ( ar->element->sta , si->element->sta ) &&
		 si->element->ondate <= ar->element->jdate &&
		 (si->element->offdate >= ar->element->jdate ||
		 si->element->offdate == -1 ) )
		break ;

	    si = si->next ;
	} /* end while ( si ) */

	if ( orig && si && 
	     isValidFloat ( dbl_LIST_ORIGIN, dbl_ORIGI_LAT, orig->element->lat ) &&
	     isValidFloat ( dbl_LIST_ORIGIN, dbl_ORIGI_LON, orig->element->lon ) &&
	     isValidFloat ( dbl_LIST_SITE, dbl_SITE_LAT, si->element->lat ) &&
	     isValidFloat ( dbl_LIST_SITE, dbl_SITE_LON, si->element->lon ) ) {

		float slat = si->element->lat ,
		      slon = si->element->lon ,
		      elat = orig->element->lat ,
		      elon = orig->element->lon ,
		      delt, fDist, fAzim, bazim ;

		dbDelaz(&slat, &slon, &elat, &elon, &delt, &fDist, &fAzim, &bazim);

		if ( fDist < 1000 )
		    sprintf ( dist , "%6.2f" , fDist ) ;
		else
		    sprintf ( dist , "%6.1f" , fDist ) ;
		sprintf ( evaz , "%5.1f" , fAzim ) ;
	}
	else {
	    strcpy ( dist , "      " ) ;        /* 6 spaces */
	    strcpy ( evaz , "     " ) ;         /* 5 spaces */
	}

	/* direction */
        if ( isValidString ( dbl_LIST_ARRIVAL, dbl_ARRIV_FM , ar->element->fm ) ) {
	    char *fm = ar->element->fm ;
	    if ( fm[ 0 ] == 'c' || fm[ 1 ] == 'c' )
		direction = 'c' ;
	    else if ( fm[ 0 ] == 'd' || fm[ 1 ] == 'd' )
		direction = 'd' ;
	    else if ( fm[ 0 ] == 'u' || fm[ 1 ] == 'u' )
		direction = 'c' ;
	    else if ( fm[ 0 ] == 'r' || fm[ 1 ] == 'r' )
		direction = 'd' ;
	    else
		direction = ' ' ;
	}
	else
	    direction = ' ' ;

	/* detchar */
        if ( isValidString ( dbl_LIST_ARRIVAL, dbl_ARRIV_QUAL , ar->element->qual ) ) {
	    if ( ar->element->qual[ 0 ] == 'i' || ar->element->qual[ 0 ] == 'e' )
		detchar = ar->element->qual[ 0 ] ;
	    else if ( ar->element->qual[ 0 ] == 'q' || ar->element->qual[ 0 ] == 'w' )
		detchar = 'q' ;
	    else
		detchar = ' ' ;
	}
        else
	    detchar = ' ' ;

	/* phase */
        if ( isValidString ( dbl_LIST_ARRIVAL, dbl_ARRIV_IPHASE , ar->element->iphase ) &&
             strcmp( ar->element->iphase , "-      " )  )
            sprintf ( phase , "%-7.7s" , ar->element->iphase ) ;
        else
            strcpy ( phase , "       " ) ;         /* 7 spaces */

	/* time */
        if ( isValidFloat ( dbl_LIST_ARRIVAL, dbl_ARRIV_TIME, ar->element->time ) ) {
            sprintf ( time , "%-21.21s" , tmListEpochTime ( ar->element->time , 14 ) ) ;
	    time[ 10 ] = ' ' ;
	}
        else
            strcpy ( time , "                     " ) ;       /* 21 spaces */

	/* azim */
        if ( isValidFloat ( dbl_LIST_ARRIVAL, dbl_ARRIV_AZIMUTH , ar->element->azimuth ) )
            sprintf ( azim , "%5.1f" , ar->element->azimuth ) ;
        else
            strcpy ( azim , "     " ) ;         /* 5 spaces */

	/* azres */
        if ( isValidFloat ( dbl_LIST_ARRIVAL, dbl_ARRIV_DELAZ , ar->element->delaz ) )
            sprintf ( azres , "%6.1f" , ar->element->delaz ) ;
        else
            strcpy ( azres , "      " ) ;         /* 6 spaces */

	/* slow */
        if ( isValidFloat ( dbl_LIST_ARRIVAL, dbl_ARRIV_SLOW , ar->element->slow ) )
            sprintf ( slow , "%5.1f" , ar->element->slow ) ;
        else
            strcpy ( slow , "     " ) ;         /* 5 spaces */

	/* sres */
        if ( isValidFloat ( dbl_LIST_ARRIVAL, dbl_ARRIV_DELSLO , ar->element->delslo ) )
            sprintf ( sres , "%5.1f" , ar->element->delslo ) ;
        else
            strcpy ( sres , "     " ) ;         /* 5 spaces */

	/* tdef, adef, & sdef */
	if ( as && as->element->timedef[0] == 'd' )
	    tdef = 'T' ;
	else
	    tdef = ' ' ;
	if ( as && as->element->azdef[0] == 'd' )
	    adef = 'A' ;
	else
	    adef = ' ' ;
	if ( as && as->element->slodef[0] == 'd' )
	    sdef = 'S' ;
	else
	    sdef = ' ' ;

	/* snr */
        if ( isValidFloat ( dbl_LIST_ARRIVAL, dbl_ARRIV_SNR , ar->element->snr ) )
            sprintf ( snr , "%5.1f" , ar->element->snr ) ;
        else
            strcpy ( snr , "     " ) ;         /* 5 spaces */

	/* amp */
        if ( isValidFloat ( dbl_LIST_ARRIVAL, dbl_ARRIV_AMP , ar->element->amp ) )
            sprintf ( amp , "%9.1f" , ar->element->amp ) ;
        else
            strcpy ( amp , "         " ) ;         /* 9 spaces */

	/* per */
        if ( isValidFloat ( dbl_LIST_ARRIVAL, dbl_ARRIV_PER , ar->element->per ) )
            sprintf ( per , "%5.2f" , ar->element->per ) ;
        else
            strcpy ( per , "     " ) ;         /* 5 spaces */

	/* mdef1, mag1, mdef2, & mag2 */
	if ( orig ) {
	    if ( isValidFloat ( dbl_LIST_ORIGIN , dbl_ORIGI_MB , orig->element->mb ) &&
		 isValidFloat ( dbl_LIST_ORIGIN , dbl_ORIGI_MS , orig->element->ms ) ) {
		strcpy ( mdef1 , "mb" ) ;
		sprintf( mag1  , "%4.1f" , orig->element->mb ) ;
		strcpy ( mdef2 , "ms" ) ;
		sprintf( mag2  , "%4.1f" , orig->element->ms ) ;
	    }
	    else if ( isValidFloat ( dbl_LIST_ORIGIN , dbl_ORIGI_MB , orig->element->mb ) &&
		      isValidFloat ( dbl_LIST_ORIGIN , dbl_ORIGI_ML , orig->element->ml ) ) {
		strcpy ( mdef1 , "mb" ) ;
		sprintf( mag1  , "%4.1f" , orig->element->mb ) ;
		strcpy ( mdef2 , "ml" ) ;
		sprintf( mag2  , "%4.1f" , orig->element->ml ) ;
	    }
	    else if ( isValidFloat ( dbl_LIST_ORIGIN , dbl_ORIGI_MS , orig->element->ms ) &&
		      isValidFloat ( dbl_LIST_ORIGIN , dbl_ORIGI_ML , orig->element->ml ) ) {
		strcpy ( mdef1 , "ms" ) ;
		sprintf( mag1  , "%4.1f" , orig->element->ms ) ;
		strcpy ( mdef2 , "ml" ) ;
		sprintf( mag2  , "%4.1f" , orig->element->ml ) ;
	    }
	    else {
		strcpy ( mdef2 , "  " ) ;
		strcpy ( mag2  , "    " ) ;

		if ( isValidFloat ( dbl_LIST_ORIGIN , dbl_ORIGI_MB , orig->element->mb ) ) {
		    strcpy ( mdef1 , "mb" ) ;
		    sprintf( mag1  , "%4.1f" , orig->element->mb ) ;
		}
		else if ( isValidFloat ( dbl_LIST_ORIGIN , dbl_ORIGI_MS , orig->element->ms ) ) {
		    strcpy ( mdef1 , "ms" ) ;
		    sprintf( mag1  , "%4.1f" , orig->element->ms ) ;
		}
		else if ( isValidFloat ( dbl_LIST_ORIGIN , dbl_ORIGI_ML , orig->element->ml ) ) {
		    strcpy ( mdef1 , "ml" ) ;
		    sprintf( mag1  , "%4.1f" , orig->element->ml ) ;
		}
		else {
		    strcpy ( mdef1 , "  " ) ;
		    strcpy ( mag1  , "    " ) ;
		}
	    }
	}
	else {
	    strcpy ( mdef1 , "  " ) ;
	    strcpy ( mag1  , "    " ) ;
	    strcpy ( mdef2 , "  " ) ;
	    strcpy ( mag2  , "    " ) ;
	}

	/* id */
        if ( isValidInt ( dbl_LIST_ARRIVAL, dbl_ARRIV_ARID , ar->element->arid ) )
            sprintf ( id , "%8d" , ar->element->arid ) ;
        else
            strcpy ( id , "        " ) ;         /* 8 spaces */


	fprintf ( ptr , "%s %s %s %c%c%c %s %s %s %s %s %s %s %c%c%c %s %s %s %s%s %s%s %s",
		  sta, dist, evaz, picktype, direction, detchar, phase, time, tres,
		  azim, azres, slow, sres, tdef, adef, sdef, snr, amp, per, 
		  mdef1, mag1, mdef2, mag2, id ) ;

	fprintf(ptr, "\n");
	ar = ar->next;
    }
    fprintf(ptr,"\n");
    printf("\n");

    return 0 ;
} /* end PutArrivalData */


int PutOriginData ( FILE *ptr , DBlist tree )
{
    char HeaderLine1[] = "   Date       Time       Latitude Longitude    Depth    Ndef Nsta "
                         "Gap    Mag1  N    Mag2  N    Mag3  N  Author          ID\n";
    char HeaderLine2[] = "       rms   OT_Error      Smajor Sminor Az        Err   mdist"
                         "  Mdist     Err        Err        Err     Quality\n";

    char time[ 25 ] ,
	fixf = ' ' ,
	lat[ 9 ] ,
	lon[ 10 ] ,
	depth[ 6 ] ,
	ndef[ 5 ] ,
	nsta[ ] = "    " ,
	gap[ ] = "   " ,
	magtype1[ 3 ] ,
	mag1[ 5 ] ,
	magtype2[ 3 ] ,
	mag2[ 5 ] ,
	magtype3[ 3 ] ,
	mag3[ 5 ] ,
	N[] = "  " ,
	author[ 9 ] ,
	id[ 9 ] ,
	line2[] = "\n             +-                                +-                       +-         +-         +-              \n\n" ;


    struct CSStree *Tree = (struct CSStree *) tree ;
    struct originList *orig = Tree->orHead ;

    if(!orig)
	return -1;

    printf("Writing origin data ");

    fprintf(ptr,"DATA_TYPE ORIGIN GSE2.0\n");
    fprintf(ptr, "%s", HeaderLine1);
    fprintf(ptr, "%s\n", HeaderLine2);

    while(orig){
	putchar('.');
	fflush(stdout);

	/* time */
        if ( isValidFloat ( dbl_LIST_ORIGIN, dbl_ORIGI_TIME, orig->element->time ) ) {
            sprintf ( time , "%-21.21s" , tmListEpochTime ( orig->element->time , 14 ) ) ;
	    time[ 10 ] = ' ' ;
	}
        else
            strcpy ( time , "                     " ) ;       /* 21 spaces */

	/* lat */
        if ( isValidFloat ( dbl_LIST_ORIGIN, dbl_ORIGI_LAT, orig->element->lat ) )
            sprintf ( lat , "%8.4f" , orig->element->lat ) ;
        else
            strcpy ( lat , "        " ) ;       /* 8 spaces */

	/* lon */
        if ( isValidFloat ( dbl_LIST_ORIGIN, dbl_ORIGI_LON, orig->element->lon ) )
            sprintf ( lon , "%9.4f" , orig->element->lon ) ;
        else
            strcpy ( lon , "         " ) ;       /* 9 spaces */

	/* depth */
        if ( isValidFloat ( dbl_LIST_ORIGIN, dbl_ORIGI_DEPTH, orig->element->depth ) )
            sprintf ( depth , "%5.1f" , orig->element->depth ) ;
        else
            strcpy ( depth , "     " ) ;       /* 5 spaces */

	/* ndef */
        if ( isValidInt ( dbl_LIST_ORIGIN, dbl_ORIGI_NDEF, orig->element->ndef ) )
            sprintf ( ndef , "%4d" , orig->element->ndef ) ;
        else
            strcpy ( ndef , "    " ) ;       /* 4 spaces */

	/* magnitudes */
	if ( isValidFloat ( dbl_LIST_ORIGIN, dbl_ORIGI_MB, orig->element->mb ) ) {
            sprintf ( mag1 , "%4.1f" , orig->element->mb ) ;
	    strcpy ( magtype1 , "mb" ) ;
	}
        else {
            strcpy ( mag1 , "    " ) ;		/* 4 spaces */
	    strcpy ( magtype1 , "  " ) ;	/* 2 spaces */
	}

        if ( isValidFloat ( dbl_LIST_ORIGIN, dbl_ORIGI_MB, orig->element->ms ) ) {
            sprintf ( mag2 , "%4.1f" , orig->element->ms ) ;
            strcpy ( magtype2 , "ms" ) ;
        }
        else {
            strcpy ( mag2 , "    " ) ;          /* 4 spaces */
            strcpy ( magtype2 , "  " ) ;        /* 2 spaces */
        }

        if ( isValidFloat ( dbl_LIST_ORIGIN, dbl_ORIGI_MB, orig->element->ml ) ) {
            sprintf ( mag3 , "%4.1f" , orig->element->ml ) ;
            strcpy ( magtype3 , "ml" ) ;
        }
        else {
            strcpy ( mag3 , "    " ) ;          /* 4 spaces */
            strcpy ( magtype3 , "  " ) ;        /* 2 spaces */
        }

	/* author */
        if ( isValidString ( dbl_LIST_ORIGIN, dbl_ORIGI_AUTH, orig->element->auth ) &&
             strcmp( orig->element->auth , "-       " )  ) {
	    if ( strlen ( orig->element->auth ) <= 8 )
        	sprintf ( author , "%-8.8s" , orig->element->auth ) ;
	    else {
		strncpy ( author , orig->element->auth , 8 ) ;
		author[ 8 ] = '\0' ;
	    }
	}
        else
            strcpy ( author , "        " ) ;       /* 8 spaces */

	/* id */
	if ( isValidInt ( dbl_LIST_ORIGIN, dbl_ORIGI_ORID, orig->element->orid ) )
            sprintf ( id , "%8d" , orig->element->orid ) ;
        else
            strcpy ( id , "        " ) ;       /* 8 spaces */

	/* out put the strings */
	fprintf ( ptr , "%s %c  %s %s %c  %s %c  %s %s %s  %s%s %s  %s%s %s  %s%s %s  %s  %s%s",
		  time, fixf, lat, lon, fixf, depth, fixf, ndef, nsta, gap, 
		  magtype1, mag1, N, magtype2, mag2, N, magtype3, mag3, N, author, id , line2 ) ;

	orig = orig->next;
    }
    fprintf(ptr,"\n");
    printf("\n");

    return 0 ;

} /* end PutOriginData */


int WriteGSEFile( char *outFile, DBlist tree, char *datasource, int cm6 )
{
    int Ntraces;
    FILE *ptr ;

    ptr = fopen ( outFile , "w" ) ;

    if ( !ptr ) {
	printf ( "Error:  Cannot open output file.\n" ) ;
	return -1 ;
    }

    PutIntro ( ptr , datasource ) ;

    Ntraces = PutWaveformData ( ptr , tree , cm6 ) ;
    if ( Ntraces < 1 ) {
	fclose ( ptr ) ;
	return wfError ;
    }

    PutStationData ( ptr , tree ) ;
    PutChannelData ( ptr , tree ) ;
    PutArrivalData ( ptr , tree ) ;
    PutOriginData ( ptr , tree ) ;

    fclose ( ptr ) ;
    return Ntraces ;
}



/* ------------------------------------------------------------------ */


/* This whole routine was stolen from suds2gse.  I have no idea how it
works, but it is perported to produce distance and azimuth, so I'm using it.
maf */


static void dbDelaz(float *slat, float *slon, float *elat, float *elon,
           float *delt, float *dist, float *azim, float *bazim)
{
    /* Builtin functions */
/*    double tan(), atan(), sin(), cos(), acos(), sqrt(); */

    /* Local variables */
    static double  a, b, c__, z__, celon, elatr, cslon, elonr, selon, a1,
            b1, c1, slatr, slonr, sslon, cd, ceclat, bz, cz, eclatr, seclat,
            csclat, sclatr, ssclat, aaa, cbz;

/* cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */
/*  This subroutine calculates the four quantities that depend on the  c */
/*  relative locations on the globe of epicenter and station.          c */
/*  Output: delt(angular separation id degrees), dist(geodesic distancec */
/*  in kilometers), azim(epicenter-to-station azimuth, in degrees N    c */
/*  thru E), and bazim(station-to-epicenter azimuth, in degrees N thru E */
/*  Input: slat(station latitude), slon(station longitude), elat(epi-  c */
/*  center latitude), and elon(epicenter longitude).  Latitudes(which  c */
/*  are input as geographic latitudes) are to be given from 0 to 90    c */
/*  degrees, +indicating North, and - indicating South.  Longitudes    c */
/*  are to be given from 0 to 180 degrees, + indicating East and -     c */
/*  indicating West, with  0=Greenwich Meridian.                       c */
/* cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc */

/*  converting input angles from degrees to radians */
/*  (suffix r always denotes radian measure) */
    slatr = *slat * (float).01745329;
    slonr = *slon * (float).01745329;
    elatr = *elat * (float).01745329;
    elonr = *elon * (float).01745329;
/*  converting geographic latitudes to geocentric latitudes */
    aaa = tan(slatr) * (float).996647;
    slatr = atan(aaa);
    aaa = tan(elatr) * (float).996647;
    elatr = atan(aaa);
/*  At this point latitudes are converted into colatitudes, which run  c
*/
/*  from 0 at North Pole to 180 at South Pole.  Also, longitudes are   c
*/
/*  re-evaluated so that they run from 0 to 360 from Greenwich Eastward */
/*  sclatr=station colatitude   eclatr=epicenter colatitude  (radians) */
    sclatr = (float)1.570796327 - slatr;
    if (slonr >= 0.) {
        goto L20;
    } else {
        goto L10;
    }
L10:
    slonr += (float)6.283185307;
L20:
    eclatr = (float)1.570796327 - elatr;
    if (elonr >= 0.) {
        goto L40;
    } else {
        goto L30;
    }
L30:
    elonr += (float)6.283185307;
L40:
    seclat = sin(eclatr);
    ceclat = cos(eclatr);
    selon = sin(elonr);
    celon = cos(elonr);
    ssclat = sin(sclatr);
    csclat = cos(sclatr);
    sslon = sin(slonr);
    cslon = cos(slonr);
/*  calculation of direction cosines of station (a1,b1,c1) and of      c
*/
/*  epicenter (a,b,c). For this calculation, x-axis is thru Greenwich  c
*/
/*  Meridian, y-axis at 90E longitude, and z-axis thru North Pole.     c
*/
    a = seclat * celon;
    b = seclat * selon;
    c__ = ceclat;
    a1 = ssclat * cslon;
    b1 = ssclat * sslon;
    c1 = csclat;
/*  Now you have all the info necessary to compute delt, dist, azim, */
/*  and bazim.  For reference:  cosine of delta=cd, cosine of azimuth=cz
*/
/*  azim. in radians=z, cosine of backazimuth=cbz, and back-azimuth */
/*  in radians=bz. */
    cd = a * a1 + b * b1 + c__ * c1;
    *delt = acos(cd) * (float)57.29577951;
    *dist = *delt * (float).01745329252 * (float)6371.;
/*  computation of cz and z.  The following formula is derivable */
/*  from Bullen orig via analytic geometry. */
    cz = (seclat * csclat - ceclat * ssclat * (celon * cslon + selon * sslon))
             / sqrt((float)1. - cd * cd);
    z__ = acos(cz);
/*  The following test determines whether z should be > orig < 180 degrees.
*/
    if (sslon * celon - cslon * selon < (float)0.) {
        z__ = (float)6.283185307 - z__;
    }
    *azim = z__ * (float)57.29577951;
/*  computation of cbz and bz.  This is accomplished by switching the */
/*  roles of station and epicenter in the previous formula. */
    cbz = (ssclat * ceclat - csclat * seclat * (cslon * celon + sslon * selon)
            ) / sqrt((float)1. - cd * cd);
    bz = acos(cbz);
/*  The following test determines whether bz should be > orig < 180 degrees.
 */
    if (selon * cslon - celon * sslon < (float)0.) {
        bz = (float)6.283185307 - bz;
    }
    *bazim = bz * (float)57.29577951;
}
/* --------------------------------------------------------------------- */
