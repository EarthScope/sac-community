/** 
 * @file   xgmtmap.c
 * 
 * @brief  Create a GMT Map
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "select.h"
#include "fks.h"
#include "co.h"
#include "bool.h"
#include "hdr.h"
#include "dfm.h"
#include "gem.h"

#include "string_utils.h"


#include "msg.h"
#include "bbs.h"
#include "cpf.h"
#include "dff.h"

#define MAXRECORDLENGTH 128 /* max characters in a line of the event file, maf 960620 */
#define MAXSTRING 10000     /* Max characters in a string like psxy etc... */
#define MAXSTANAME 9        /* Max number of characters in a station name */

#define SACPSVIEWER_DEFAULT_NO_TTY "gs -sDEVICE=x11 -q -dNOPROMPT  "
#define SACPSVIEWER_DEFAULT "gs -sDEVICE=x11 -q -dNOPROMPT -dTTYPAUSE "

#define PSVAR  "$psfile"
#define PSFILE "./gmt.ps"
#define __EOF__  "EOF"

#define MERCATOR    1
#define EQUIDISTANT 2
#define AZIM        3
#define ROBINSON    4
#define LAMBERT     5
#define UTM         6

#define MERIDIAN_180_180 1
#define MERIDIAN_0_360   2

#define MAXLON_180_180  (  180.0 )
#define MINLON_180_180  ( -180.0 )
#define MAXLON_0_360    (  360.0 )
#define MINLON_0_360    (    0.0 )
#define MINLAT          (  -89.0 )
#define MAXLAT          (   89.0 )

int
eventfile_count_lines(char *file, int *nerr) {
  FILE *fp;
  char buf[MAXRECORDLENGTH + 1];
  int n;

  *nerr = 0;
  if((fp = fopen(file, "r")) == NULL) {
    *nerr = 100001;
    return 0;
  }
  n = 0;
  while(fgetsp(buf, MAXRECORDLENGTH, fp) != NULL) {
    n++;
  }
  fclose(fp);
  return n;
}

float
lon180(float lon) {
  while(lon < 180) { lon += 360;  }
  while(lon > 180) { lon -= 360;  }
  return lon;
}

float
lon360(float lon) {
  while(lon <   0) { lon += 360; }
  while(lon > 360) { lon -= 360; }
  return lon;
}

void
clamp_lat_lon(int meridian, 
	      float *minlon, 
	      float *maxlon, 
	      float *minlat, 
	      float *maxlat) {

  *minlat = fmax(*minlat, MINLAT);
  *maxlat = fmin(*maxlat, MAXLAT);
  if(meridian == MERIDIAN_180_180) {
    *minlon = lon180(*minlon);
    *maxlon = lon180(*maxlon);
  } else {
    *minlon = lon360(*minlon);
    *maxlon = lon360(*maxlon);	  
  }

}

void
plot_legend_box(FILE *fp, float x, float y, char *output) {
  fprintf(fp, 
	  "psxy -K -O -JX1i/1.5i -R0/1/0/1 -Xa%.1fi -Ya%.1fi "
	  "-L -N -A -B0 <<" __EOF__ " >> %s \n"
	  "0 0\n1 0\n1 1\n0 1\n" __EOF__ "\n\n",
	  x,y,output);
}
void
plot_legend_symbols(FILE *fp, char *symbol, float x, float y,
		    float min, float mean, float max,
		    char *output) {
  fprintf(fp, "psxy -K -O -JX -R -Xa%.1fi -Ya%.1fi %s -N  "
	  " <<" __EOF__ " >> %s \n"
	  "%.3f %.3f %.3f\n"
	  "%.3f %.3f %.3f\n"
	  "%.3f %.3f %.3f\n"
	  __EOF__ "\n\n",
	  x,y, symbol, output,
	  0.25, 0.25, min, 
	  0.25, 0.50, mean, 
	  0.25, 0.75, max);
}
void
plot_legend_text(FILE *fp, float x, float y,
		 float min, float mean, float max,
		 char *output) {
  fprintf(fp, "pstext -K -O -JX -R -N -Xa%.1fi -Ya%.1fi "
	  " <<" __EOF__ " >> %s \n"
	  "%.3f %.3f 14 0 5 5 %.2f \n"
	  "%.3f %.3f 14 0 5 5 %.2f \n"
	  "%.3f %.3f 14 0 5 5 %.2f \n"
	  __EOF__ "\n\n",
	  x, y, output,
	  0.50, 0.25, min,
	  0.50, 0.50, mean,
	  0.50, 0.75, max);
}

/** 
 * To generate a script to input to gmt for production of a
 *    map.
 *
 * @param nerr
 *    Error Return Flag
 *    - 0 on Success
 *    - Non-Zero on Error
 *
 * @date 960111  Original version.
 * @date 991217  Added topography and station labeling options (peterg)
 * @date Mar 12 2008 - Added options
 *                   - AUTOLIMITS
 *                   - MAPSCALE
 *                   - PLOTSTATIONS
 *                   - PLOTEVENTS
 *                   - PLOTLEGEND
 *                   - LEGENDXY
 *                   - FILE
 *
 */
void 
xgmtmap(int *nerr)
{

        FILE *fp;

        float minlat, maxlat, minlon, maxlon, minlon2, maxlon2; 
        float deltalat, deltalon, deltalon2;
	float deltalatbig = 100.0;
	float deltalonbig = 100.0; 
        float width;
        float lon_annot_spac, lon_tick_spac, lat_annot_spac, lat_tick_spac;
        float latlon_undef = -1000.;    /* undefined value for lat and lon */
        float dlonsave, dlatsave;

        static double user_west  = -1000.;
        static double user_east  = -1000.;
        static double user_north = -1000.;
        static double user_south = -1000.;

        static int luser_west  = FALSE;
        static int luser_east  = FALSE;
        static int luser_north = FALSE;
        static int luser_south = FALSE;

	static float minscale = 0.1; /* minimum size of symbols */
	static float maxscale = 0.5; /* maximum size of symbols */ 
        int lsize = FALSE; /* default is fixed size event symbols */
        int lresid = FALSE; /* default is fixed size event symbols */
        int lrmean_resid = FALSE; /* default is don't subtract mean from resids */
        float size_undef = -1.0e38;    /* undefined value for size variable */
	float minsize, maxsize, midsize, deltasize; 
	float minneg, minpos, maxneg, maxpos; 
	float midscale, deltascale;
	float slope, intercept; 

        float *buf = NULL;
        float *size;
        float *stalat, *stalon, *evlat, *evlon;
	char **staname; 

        int nch, jdfl, jdfl_, nlen, ndxy, ndxx, i, idx, jdx;
        int lrefset = TRUE;
        int irefsta;
 	int nstationlocs = 0;
        int neventlocs = 0;
	int nsize = 0;
	float sumsize = 0.0;
	float meansize = 0.0;
        int npos = 0;
        int nneg = 0;

	int mapJ = MERCATOR;

	static int lstanames = FALSE;
	static int ltopo = FALSE;
 
	char grdcoms[MAXSTRING];
        char pscoast[MAXSTRING];
        char psxy[MAXSTRING],pstext[MAXSTRING];
	char ctitle[MAXSTRING]; 
        char cwidth[10];
        char iwrange[100], ierange[100], isrange[100], inrange[100];
        char carea[100];
        char clengthscale[100];
        char clon_annot_spac[100], clon_tick_spac[100], clat_annot_spac[100];
        char clat_tick_spac[100];
	char cbounds[100];
 	char cprojection[100];
  char cmeridian[10];
  char bazim[10],mean_residual[10];
  char refsta[10];
  char *scriptname = "./gmt.csh";
  char syscommand[100];
  char *viewer;
  
	/* The following variables are used for setting min and max sizes, maf 960702 */
  double	minSizeInput ,	/* defined by user with scale option */
          maxSizeInput ;  /* defined by user with scale option */
        int	lscale = FALSE; /* default is let the code determine min and max */
	int lmapscale     = FALSE;
	int plotstations  = TRUE;
	int plotevents    = TRUE;
	int plotlegend    = TRUE;
	float    legendx       = 1;
	float    legendy       = 1;
	double   lxy[2];
	int auto_limits   = FALSE;
	/* int meridian = MERIDIAN_180_180; */
	char     output[MCPFN + 1];
	int outputlen;
 	/* The following variables are used for handling an event file containing
 	   data on additional events,  maf 960620 */
 	FILE * eventFile;
	int leventfile = FALSE; /* default is no additional data */
 	char eventFileName[MCPFN + 1];	/* name of event file */
 	int eventFileNameLength;   /* length of the event file name. */
 	int eventFileLength = 0;	/* number of lines in the event file */
 	char eventBuffer[MAXRECORDLENGTH + 1];  /* stores a line of info from event file. */
 	char * pEventBuffer ;	/* point to elements in eventBuffer */
 	int number_event_station ;
 
	int sc_len;

	/* PROCEDURE: */
	*nerr = 0;
	staname = NULL;
	strcpy(output, PSFILE);
/* PARSING PHASE: */
L_1000:
        if( lcmore ( nerr ) ){
	  if( lkreal( "W#EST$", 7, &user_west )){
	    luser_west = TRUE;
	  } else if( lkreal( "E#AST$", 7, &user_east )){
	    luser_east = TRUE;
	  } else if( lkreal( "N#ORTH$", 8, &user_north )){
	    luser_north = TRUE;
	  } else if( lkreal( "S#OUTH$", 8, &user_south )){
	    luser_south = TRUE;
	  } 
	  else if( lklog( "AUTO#LIMITS$",    13, &auto_limits)) { 
	    luser_west  = FALSE;
	    luser_east  = FALSE;
	    luser_north = FALSE;
	    luser_south = FALSE;
	  }
/* -- Identify Projection: */
	  else if( lckey( "MER#CATOR$", 11 )){
	    mapJ = MERCATOR;
	  } else if( lckey( "AZ#IM_EQUIDIST$", 16 )){
	    mapJ = AZIM;
	  } else if( lckey( "ROB#INSON$", 11 )){
	    mapJ = ROBINSON;
	  } else if( lckey( "EQ#UIDISTANT$", 14 )){
	    mapJ = EQUIDISTANT;
	  } else if( lckey( "LAM#BERT$", 10 )){
	    mapJ = LAMBERT;
	  } else if( lckey( "UTM$", 5 )){
	    mapJ = UTM;
	  } 

	  /* Check to see if topography was requested */
	  else if( lklog( "TOPO#GRAPHY$", 13, &ltopo ) ) { }
	  
	  /* Check to see if station labeling was requested */
	  else if( lklog( "STAN#AMES$", 11, &lstanames ) ) { }

	  /* eventfile reads named file for additional events; maf 960620 */
	  else if( lkchar("EV#ENTFILE$", 12, MCPFN, 
			  eventFileName, MCPFN, &eventFileNameLength ) ){
	    leventfile = TRUE;
	    eventFileName[eventFileNameLength] = '\0';
	  }
	  
	  /* -- SCALE min max:  
	     define min and max sizes of marks in gmap, maf 960702 */
	  else if( lkrrcp( "SC#ALED$",9, -100.0, 100.0, &minSizeInput, &maxSizeInput ) ){
	    lscale = TRUE ;
	  }

	  /* Initially just use keyword to decide if scaled symbols */ 
	  else if( lklog( "SI#ZE$",            7, &lsize  )) { } 
	  else if( lklog( "MAG#NITUDE$",      12, &lsize  )) { }
	  else if( lklog( "RE#SIDUAL$",       11, &lresid )) { }
	  else if( lckey( "RM#EAN_RESIDUAL$", 17          )) {
	    lresid = TRUE;
	    lrmean_resid = TRUE;
	  } 
	  /* Plotting Options */
	  else if( lklog( "MAPSCALE#$",    11, &lmapscale    )) { }
	  else if( lklog( "PLOTSTATIONS$", 16, &plotstations )) { }
	  else if( lklog( "PLOTEVENTS$",   14, &plotevents   )) { }
	  else if( lklog( "PLOTLEGEND$",   15, &plotlegend   )) { }

	  else if( lkra( "LEGENDXY$", 10, 2, 2, &lxy[0], &i)) {
	    plotlegend = TRUE;
	    legendx = lxy[0];
	    legendy = lxy[1];
	  }
	  else if( lkchar( "FILE#$", 7, MCPFN, output, MCPFN, &outputlen) ) {
	    output[outputlen] = 0;
	  }

	  /* -- Bad syntax. */
	  else {
	    cfmt( "ILLEGAL OPTION:",17 );
	    cresp();
	    goto L_9999;
	  } 
	  
	  goto L_1000;
	}

	/* "sizes" and "resid" are mutually exclusive, but scale requires 
	 * one of them. If lsize and lresid are both true, send an 
	 * error message. if lscale is true and neither lsize nor 
	 * lresid are true, set lsize true.  maf 960702 
	 */
	if ( lsize && lresid ){
	    cfmt( "MUTUALLY EXCLUSIVE OPTIONS: SIZE and RESIDUALS:", 49 ) ;
	    cresp();
	    goto L_9999;
	}
	
	if ( lscale && !lsize && !lresid ) {
	  lsize = TRUE ;
	}

	/* Must plot either the stations and/or events, but not neither */
	if(!plotstations && !plotevents) {
	  setmsg("ERROR", -1);
	  apcmsg("Must specify either events to plot or station to plot", 55);
	  outmsg();
	  goto L_9999;
	}

	/* CHECKING PHASE: */
	/* - Check for null data file list. */
	vflist( nerr );
	if( *nerr != 0  && !leventfile )
	  goto L_9999; 

	/* If an event file was specified, check that it exists 
	 *   and count the number of lines, maf 960620 
	 */

	/* Count the number of events in the event file */
	if( leventfile && plotevents) {
	  eventFileLength = eventfile_count_lines(eventFileName, nerr);
	  if(*nerr != 0) {
	    goto L_9999;
	  }
	}

        /* Allocate a buffer for use as location storage arrays. 
	 * number_event_station is the total number of entries from 
	 * the waveform files plus the a additional entries from 
	 * the event file. maf 960620 
	 */
	number_event_station = cmdfm.ndfl + eventFileLength;

	buf = (float *)malloc(7 * number_event_station * sizeof(float));
	if(buf == NULL){
	  *nerr = 1302;
	  goto L_9999;
	}
    memset(buf, 0, 7 * number_event_station * sizeof(float));
        stalat = buf;
        stalon = buf +     number_event_station;
        evlat  = buf + 2 * number_event_station;
        evlon  = buf + 3 * number_event_station;
        size   = buf + 4 * number_event_station;
        /* pos    = buf + 5 * number_event_station; */
        /* neg    = buf + 6 * number_event_station; */

        minlat  =  1000.0;
        maxlat  = -1000.0;
        minlon  =  1000.0;
        maxlon  = -1000.0;
	minlon2 =  1000.0;
	maxlon2 = -1000.0;

	minsize    =  1.0e38;
	maxsize    = -1.0e38; 
	minneg     =  1.0e38;
	maxneg     = -1.0e38; 
	minpos     =  1.0e38;
	maxpos     = -1.0e38; 

	/* Allocate memory for station names 
	 * eventually will want to change from number_event_station 
	 * to number_stations after we add a station file option 
	 */
	staname = (char ** ) malloc (number_event_station * sizeof (char *));
	if (staname == NULL) {
	  *nerr = 1302;
	  goto L_9999;
	}

	for (idx = 0; idx < number_event_station; idx++ ) {
	  staname[idx] = (char *) malloc(MAXSTANAME * sizeof(char));
	  if ( !staname[idx] ) {
	    for (jdx = idx -1; jdx >= 0; jdx--) {
	      free (staname[jdx]);
	    }
	    free ( staname ) ;
	    *nerr = 1302;
	    goto L_9999;
	  }
	}


	/* DATA INPUT PHASE: */
	/* nch = number of entries from waveform file */
 	nch = cmdfm.ndfl;	

	/* Load up station and event location arrays. */
	for( jdfl = 1; jdfl <= nch; jdfl++ ){
          jdfl_ = jdfl-1;
	  getfil( jdfl, FALSE, &nlen, &ndxy, &ndxx, nerr );
	  if( *nerr != 0 )  {  goto L_9999; }

	  /* Check for a reference station specified in header. */
          if( jdfl == 1) {
            if(strncmp(kuser1, "-12345", 6) == 0){
              lrefset = FALSE;
	    }
            strcpy(refsta,kuser1);
	  }
          if(strcmp(kuser1,refsta) != 0) 
	    lrefset = FALSE;

	  /* Store station location.  These must be set. */
	  if(plotstations) {
	    if( (*stla != cmhdr.fundef) &&  (*stlo != cmhdr.fundef)  ){
	      stalat[jdfl_] = *stla;
	      stalon[jdfl_] = *stlo;
	      strcpy(staname[jdfl_], kstnm);
	      maxlat        = fmax(maxlat,*stla);
	      minlat        = fmin(minlat,*stla);
	      maxlon        = fmax(maxlon, lon180(*stlo));
	      minlon        = fmin(minlon, lon180(*stlo));
	      maxlon2       = fmax(maxlon2,lon360(*stlo));
	      minlon2       = fmin(minlon2,lon360(*stlo));
	      nstationlocs += 1;
	    } else {
	      *nerr = 5301;
	      goto L_9999;
	    }       
	  }
	  /* Check for and store event locations.  These are not required. */
          if( plotevents ) {
	    if( (*evla != cmhdr.fundef) && (*evlo != cmhdr.fundef) ){
	      evlat[jdfl_]  = *evla;
	      evlon[jdfl_]  = *evlo;
	      maxlat        = fmax(maxlat,*evla);
	      minlat        = fmin(minlat,*evla);
	      maxlon        = fmax(maxlon, lon180(*evlo));
	      minlon        = fmin(minlon, lon180(*evlo));
	      maxlon2       = fmax(maxlon2,lon360(*evlo));
	      minlon2       = fmin(minlon2,lon360(*evlo));
	      neventlocs   += 1;
	    } else {
	      evlat[jdfl_]  = latlon_undef;
	    }       
	  }

	  /* Check for and store size/magnitude/variable. 
	   * These are not required. 
	   */
	  size[jdfl_] = size_undef;
	  if (*user0 != cmhdr.fundef) { 
	    /* only use the size data if it is in range, maf 960702 */
	    if ( !lscale || 
		 ( *user0 > minSizeInput && *user0 < maxSizeInput )) {
	      size[jdfl_]  = *user0; 
	      nsize += 1;
	      sumsize += size[jdfl_];
	    } 
	  }
	}


	/* nch is increased by the number of entries from 
	 * the event file. maf 960620 
	 */
        nch = cmdfm.ndfl + eventFileLength ;

	/* If an event file was specified, 
	 * read the data from it. maf 960620 
	 */
	if ( leventfile ) {
	  /* Open the Event File */
	  eventFile = fopen ( eventFileName , "r" ) ; 
	  if ( eventFile == NULL ){ 
	    *nerr = 1000001 ;
	    goto L_9999 ;
	  }

	  for ( ; jdfl <= nch ; jdfl++ ){
	    jdfl_ = jdfl-1;
	    fgetsp(eventBuffer, MAXRECORDLENGTH, eventFile);
	    pEventBuffer = eventBuffer ;
	    
	    /* Store event locations. */
	    evlat[jdfl_]  = atof ( pEventBuffer ) ;
	    while ( *pEventBuffer  == ' ' ) pEventBuffer++ ;
	    while ( *pEventBuffer != ' ' ) pEventBuffer++ ;
	    evlon[jdfl_]  = atof ( pEventBuffer ) ;
	    stalat[jdfl_] = latlon_undef ;
	    stalon[jdfl_] = latlon_undef ;
	    maxlat        = fmax(maxlat, evlat[jdfl_]);
	    minlat        = fmin(minlat, evlat[jdfl_]);
	    maxlon        = fmax(maxlon,  lon180(evlon[jdfl_]));
	    minlon        = fmin(minlon,  lon180(evlon[jdfl_]));
	    maxlon2       = fmax(maxlon2, lon360(evlon[jdfl_]));
	    minlon2       = fmin(minlon2, lon360(evlon[jdfl_]));
	    neventlocs    += 1;

	    while ( *pEventBuffer  == ' ' ) pEventBuffer++ ;
	    while ( *pEventBuffer != ' ' && *pEventBuffer != '\0') 
	      pEventBuffer++ ;
	    while ( *pEventBuffer  == ' ' ) pEventBuffer++ ;

	    /* Check for and store size variables.  These are not required. */
	    size[jdfl_]  = size_undef;
	    if( pEventBuffer != '\0' ){
	      size[jdfl_]  = atof ( pEventBuffer ) ;
	      if ( !lscale || 
		   ( size[jdfl_] > minSizeInput && 
		     size[jdfl_] < maxSizeInput ) ){
		nsize += 1;
		sumsize += size[jdfl_];
	      } else {
		size[jdfl_] = size_undef ;
	      } 
	    }      
	  }
	}
 
	/* Remove mean if requested */
	meansize = sumsize / ( (float) nsize ); 
	if (lrmean_resid) {
	  sprintf(mean_residual, "%1.2f",meansize);
	  setbbv("MEAN_RESIDUAL", mean_residual, nerr, 13, 9);
	  for( i = 0; i < nch; i++ ){
	    if ( size[i] != size_undef )
	      size[i] -= meansize;
	  }
	}

	/* Get max and min size/magnitude/resid variable. 
	 *   Overhauled 960702 maf 
	 */
	if ( lsize || lresid ) {
	  for( i = 0; i < nch; i++ ){
	    if ( lsize ) {
	      maxsize = lscale ? maxSizeInput : fmax(maxsize,size[i]);
	      minsize = lscale ? minSizeInput : fmin(minsize,size[i]);
	    } else if ( lresid ) {
	      maxsize = lscale ? 
		fmax(minSizeInput,maxSizeInput) : 
		fmax(maxsize,fabs(size[i]) );
	      minsize = fmin(minsize,fabs(size[i]) );
	      if (size[i] < 0) {
		nneg += 1;
		minneg = fmin(minneg,fabs(size[i]) );
		maxneg = lscale ? 
		  fabs(minSizeInput) : 
		  fmax(maxneg,fabs(size[i]) );
	      } else {
		npos += 1;
		minpos = fmin(minpos,size[i] );
		maxpos = lscale ? 
		  fabs(maxSizeInput) : 
		  fmax(maxpos,size[i] );
	      }
	    }
	  }
	}

	/* Handle an Isolated Station or Event */
	if(minlat == maxlat && minlon == maxlon) {
	  minlat -= 2.5;
	  maxlat += 2.5;
	  minlon -= 2.5;
	  maxlon += 2.5;
	  minlon2 -= 2.5;
	  maxlon2 += 2.5;
	}

	/* Reset min and max lat and lon if necessary */
	deltalon  = fabs(maxlon-minlon);
	deltalon2 = fabs(maxlon2-minlon2);
        deltalat  = fabs(maxlat-minlat);

        if( luser_west ){
          minlon  = lon180(user_west);
	  minlon2 = lon360(user_west);
	}else{
          minlon  = minlon  - 0.1 * deltalon;
	  minlon2 = minlon2 - 0.1 * deltalon2;
	}

        if( luser_east ){
          maxlon  = lon180(user_east);
	  maxlon2 = lon360(user_east);
	}else{
          maxlon  = maxlon  + 0.1 * deltalon;
	  maxlon2 = maxlon2 + 0.1 * deltalon2;
	}

        if( luser_south) {
          minlat = user_south;
	} else {
          minlat = minlat - .1 * deltalat;
	}

        if( luser_north) {
          maxlat = user_north;
	} else {
          maxlat = maxlat + .1 * deltalat;
	}
	
	/* Recompute deltas if mins and maxs were changed. */
        deltalat  = fabs(maxlat-minlat);
        deltalon  = fabs(maxlon-minlon);
        deltalon2 = fabs(maxlon2-minlon2);


	if( fabs((minlon2 + maxlon2)/2.0 - 180) < (minlon + maxlon)/2.0 ||
	    (deltalon > 360 && deltalon2 <= 360) ) {
	  /* meridian = MERIDIAN_0_360; */
	  minlon   = minlon2;
	  maxlon   = maxlon2;
	  deltalon = deltalon2;
	}
	if(deltalon > 360 && deltalon2 > 360) {
	  minlon = 0;
	  maxlon = 360;
	}
	

	/* Force aspect ratio to be .75 */
        if( !luser_west && !luser_east && !luser_north && !luser_south){
          if( deltalat <  .75*deltalon ){
	    dlatsave = deltalat;
	    if(mapJ == MERCATOR) {
	      /* Gudermannian Formula for Mercator Projection */
	      deltalat = (180/M_PI) * atan(sinh(deltalon*M_PI/180.0));
	      if(deltalat > dlatsave) {
		minlat = minlat - (deltalat - dlatsave) / 2.0;
		maxlat = maxlat + (deltalat - dlatsave) / 2.0;
	      } else {
		deltalat = dlatsave;
	      }
	    } else {
	      deltalat = .75 * deltalon;
	      minlat = minlat - (deltalat - dlatsave ) / 2.0;
	      maxlat = maxlat + (deltalat - dlatsave ) / 2.0;
	      deltalat = maxlat - minlat;
	    }
	  }else if(deltalat > .75*deltalon ){
            dlonsave = deltalon;
            deltalon = 1.3333*deltalat;
            minlon = minlon - ( deltalon - dlonsave )/2.0;
            maxlon = maxlon + ( deltalon - dlonsave )/2.0;
            deltalon = maxlon - minlon;
	  }
        }

	/* Set up spacing for longitudinal labeling. */
        if     ( deltalon > 300. ) lon_annot_spac = 100. ;
        else if( deltalon > 150. ) lon_annot_spac =  50. ;
        else if( deltalon > 90.  ) lon_annot_spac =  30. ;
        else if( deltalon > 60.  ) lon_annot_spac =  20. ;
        else if( deltalon > 30.  ) lon_annot_spac =  10. ;
        else if( deltalon > 15.  ) lon_annot_spac =   5. ;
        else if( deltalon > 9.   ) lon_annot_spac =   3. ;
        else if( deltalon > 6.   ) lon_annot_spac =   2. ;
        else if( deltalon > 3.   ) lon_annot_spac =   1. ;
        else if( deltalon > 1.5  ) lon_annot_spac =    .5;
        else if( deltalon > .9   ) lon_annot_spac =    .3;
        else if( deltalon > .6   ) lon_annot_spac =    .2;
        else if( deltalon > .3   ) lon_annot_spac =    .1;
        else if( deltalon > .15  ) lon_annot_spac =   .05;
        else              lon_annot_spac = deltalon / 3.0;

        lon_tick_spac = lon_annot_spac; 


/* Set up spacing for latitudinal labeling. */
        if     ( deltalat > 300. ) lat_annot_spac = 100. ;
        else if( deltalat > 150. ) lat_annot_spac =  50. ;
        else if( deltalat > 90.  ) lat_annot_spac =  30. ;
        else if( deltalat > 60.  ) lat_annot_spac =  20. ;
        else if( deltalat > 30.  ) lat_annot_spac =  10. ;
        else if( deltalat > 15.  ) lat_annot_spac =   5. ;
        else if( deltalat > 9.   ) lat_annot_spac =   3. ;
        else if( deltalat > 6.   ) lat_annot_spac =   2. ;
        else if( deltalat > 3.   ) lat_annot_spac =   1. ;
        else if( deltalat > 1.5  ) lat_annot_spac =    .5;
        else if( deltalat > .9   ) lat_annot_spac =    .3;
        else if( deltalat > .6   ) lat_annot_spac =    .2;
        else if( deltalat > .3   ) lat_annot_spac =    .1;
        else if( deltalat > .15  ) lat_annot_spac =   .05;
        else              lat_annot_spac = deltalat / 3.0;

        lat_tick_spac = lat_annot_spac; 

        sprintf(clon_annot_spac,"%1.3f",lon_annot_spac);
        sprintf(clon_tick_spac, "%1.3f",lon_tick_spac);
        sprintf(clat_annot_spac,"%1.3f",lat_annot_spac);
        sprintf(clat_tick_spac, "%1.3f",lat_tick_spac);

	width = 6.5;
        sprintf(cwidth, "%.1fi",width);

/* create integer lat and lon range strings for grdraster */
	if ( (float) ((int) minlon) == minlon ) {
	  sprintf(iwrange,"%d",(int)minlon);
	}else{
	  if ( minlon < 0 ) {
	    sprintf(iwrange,"%d",(int)(minlon-1));
	  }else{
	    sprintf(iwrange,"%d",(int)(minlon));
	  }
	}
	if ( (float) ((int) minlat) == minlat ) {
	  sprintf(isrange,"%d",(int)minlat);
	}else{
	  if ( minlat < 0 ) {
	    sprintf(isrange,"%d",(int)(minlat-1));
	  }else{
	    sprintf(isrange,"%d",(int)(minlat));
	  }
	}
	if ( (float) ((int) maxlon) == maxlon ) {
	  sprintf(ierange,"%d",(int)maxlon);
	}else{
	  if ( maxlon < 0 ) {
	    sprintf(ierange,"%d",(int)(maxlon));
	  }else{
	    sprintf(ierange,"%d",(int)(maxlon+1));
	  }
	}
	if ( (float) ((int) maxlat) == maxlat ) {
	  sprintf(inrange,"%d",(int)maxlat);
	}else{
	  if ( maxlat < 0 ) {
	    sprintf(inrange,"%d",(int)(maxlat));
	  }else{
	    sprintf(inrange,"%d",(int)(maxlat+1));
	  }
	}


/* compute area for min resolution in pscoast */
	if( mapJ == MERCATOR || mapJ == EQUIDISTANT || mapJ == LAMBERT || mapJ == UTM ) {
	  sprintf(carea,"%1d",(int)((deltalon*deltalat)+1));
	}else if( mapJ == ROBINSON || mapJ == AZIM ){ 
	  sprintf(carea,"%1d",(int) 10000 );
	}

	/*        sprintf(clengthscale,"%1.3f",(float)((long)(deltalon*30.)));*/
        if((int)(lat_annot_spac*100)<=1) {
	  sprintf(clengthscale,"%1.3f",(float)((lat_annot_spac*100.0)));
	}else{
	  sprintf(clengthscale,"%1.3f",(float)((int)(lat_annot_spac*100.0)));
	}

	/* Adjust symbol size range if mapping a large region */ 
	if ( mapJ == AZIM  || 
	     ( deltalat > deltalatbig && deltalon > deltalonbig ) ) {
	  minscale = .1;
	  maxscale = .3;
	}


	/* Convert size/magnitude/residual vars to linear scale ranging 
	 * from minscale to maxscale inches. 
	 * scale = slope * size + intercept 
	 */ 
	if( lsize || lresid ) { 
	  midscale = (minscale + maxscale) / 2.0; 
	  deltascale = maxscale - minscale;
	  
	  midsize = (minsize + maxsize ) / 2.0;
	  deltasize = maxsize - minsize; 
	  
	  slope = deltascale / deltasize; 
	  intercept = midscale - slope * midsize;
	  for( jdfl = 1; jdfl <= nch; jdfl++ ){
	    jdfl_ = jdfl-1;
	    if(size[jdfl_] != size_undef ){
	      if (lresid && size[jdfl_] < 0.0 ){
		size[jdfl_] = slope * size[jdfl_] - intercept;
	      }else{
		size[jdfl_] = slope * size[jdfl_] + intercept;
	      }
	    }
	  }  
	}


	/* Mercator projection */
        if( mapJ == MERCATOR ){        
	  sprintf(cprojection, "-JM%.1fi", width);
	}
	/* Equidistant cylindrical projection */
        else if( mapJ == EQUIDISTANT  ){
	  /* compute the central meridian */
          if( minlon != maxlon ){
	    sprintf(cmeridian,"%1.3f",(minlon + maxlon)/2.0);
	  }else{
	    sprintf(cmeridian,"%1.3f",minlon);
	  }
	  sprintf(cprojection, "-JQ%s/%.1fi",cmeridian,width);
	}
	/* Robinson cylindrical projection */
        else if( mapJ == ROBINSON ){
	  /* compute the central meridian */
          if( minlon != maxlon ){
	    sprintf(cmeridian,"%1.3f",(minlon + maxlon)/2.0);
	  }else{
	    sprintf(cmeridian,"%1.3f",minlon);
	  }
	  sprintf(cprojection, "-JN%s/%.1fi",cmeridian,width);
	}
	/* Azimuthal Equidistant Projection */
        else if( mapJ == AZIM ){
	  /* Choose the projection center as the first events station */
	  sprintf(cprojection, "-JE%.3f/%.3f/%.1fi",stalat[0],stalon[0],width);
	}
	/* Lambert Projection */
        else if( mapJ == LAMBERT ){
	  /* Choose the projection center as the midpoint in lat and lon */
	  sprintf(cprojection, "-JL%.3f/%.3f/%.3f/%.3f/%.1fi",
		  (minlon + maxlon)/2.0,  (minlat + maxlat)/2.0,
		  (minlat + maxlat)/4.0,  3.0*(minlat + maxlat)/4.0,
		  width);
	}
	/* UTM Projection */
        else if( mapJ == UTM ){
	  /* Determine the utm zone as (long) centerlat/6.0 */
	  sprintf(cprojection, "-JU%d/%.1f",
		  (int) ((minlat + maxlat)/12.0), width);
	}

	/* Map Bounds */
	sprintf(cbounds, "-R%.3f/%.3f/%.3f/%.3f", 
		minlon, maxlon, minlat, maxlat);

	/* Buildup pscoast command */
        memset(pscoast,' ',MAXSTRING);
	sprintf(pscoast, "pscoast -K -O -J -R -B");

	/* Put a legend on the map if title is set */
	if(cmgem.title.on) {
	  for (i=0; i<cmgem.title.len; i++)
	    ctitle[i]=kmgem.ktitl[i];
	  ctitle[cmgem.title.len] = '\0';
	  memset(psxy, ' ', MAXSTRING);
	}
	if( mapJ != AZIM ) {
	  if ( deltalat > deltalatbig && deltalon > deltalonbig ) {
	    strcat(pscoast," -N1 -W -Dc -A");
	  } else { 
	    strcat(pscoast," -N1 -N2 -W -Dl -A");
	  }
	  strcat(pscoast,carea);

	  if ( ! ltopo ) {
	    strcat(pscoast," -G250/250/200 ");
	  }	    
	  if(lmapscale) {
	    sprintf(pscoast, "%s" 
		    " -Lf%.3f/%.3f/%.3f/%s", 
		    pscoast,
		    minlon + deltalon * 0.33, 
		    minlat + deltalat * 0.16,
		    minlat + deltalat / 2.0,
		    clengthscale);
	  }
	} else if( mapJ == AZIM  ){ 
	  sprintf(pscoast, "%s"
		  " -N1 -W -Dc A%s -G250/250/250 -S220/240/250",
		  pscoast, 
		  carea);
	}
	sprintf(pscoast, "%s >> " PSVAR " \n\n", pscoast);

	/* Buildup the optional grd commands for topography */
	if ( ltopo ) {
	  memset(grdcoms,' ',MAXSTRING);
	  sprintf(grdcoms, "# Add Topography \n# \n"
		  "echo \"Creating Topography, please be patient\"\n"
		  "grdraster 1 -Ggmttopo.grd -R%s/%s/%s/%s \n",
		  iwrange,ierange,isrange,inrange);
	  sprintf(grdcoms, 
		  "%s"
		  "grdfilter gmttopo.grd -D1 -Fb35 -N -Ggmttopofilt.grd \n"
		  "grdgradient gmttopofilt.grd -A315 -Nt -Ggmttopograd.grd \n"
		  "grdimage -K -O -J -R "
		  "  gmttopofilt.grd -C${SACAUX}/ctables/gmt.cpt "
		  " -Igmttopograd.grd >> " PSVAR " \n\n",		 
		  grdcoms);
	}

	/* Open the output file */
        if(( fp = fopen(scriptname,"w")) == NULL){
          *nerr = 102;
          goto L_9999;
	}
        fputs("#!/bin/csh -f\n"
	      "#\n",fp);
	fputs("\n"
	      "# Set the output file\n", fp);
	fprintf(fp, "set psfile = \"%s\"\n", output);
	fputs("\n"
	      "# Start the Postscript file \n"
	      "# \n", fp);
	fprintf(fp, "psbasemap %s %s -Ba%.3ff%.3f/a%.3ff%.3fNEWS:.\"%s\": "
		" -X0.75i -Y1.0i -P -K > " PSVAR " \n\n", 
		cprojection, cbounds, 
		lon_annot_spac, lon_tick_spac, 
		lat_annot_spac, lat_tick_spac, 
		ctitle);

	if ( ltopo ) {
	  fputs(grdcoms, fp);
	}
	fputs("# Add the Coastline and National Boundaries\n"
	      "# \n", fp);
	fputs(pscoast, fp);

	/* Add optional station names if requested */
	if (lstanames) {
	  memset(pstext, ' ', MAXSTRING);
	  sprintf(pstext, "pstext -K -O -D0.10i/0.10i -J ");
	  if ( mapJ == AZIM || 
	       (deltalat > deltalatbig && deltalon > deltalonbig )) {
	    sprintf(pstext,"%s -R -S0.1/255 -G0/0/0 <<" __EOF__ " >> " PSVAR " \n", pstext);
	  } else { 
	    sprintf(pstext,"%s -R -S0.25/255 -G0/0/0 <<" __EOF__  " >> " PSVAR " \n", pstext);
	  }	  
	  for( i=0; i < nch; i++ ){
	    if ( stalon[i] != latlon_undef && stalat[i] != latlon_undef ) {
	      if ( i == 0 || 
		   stalon[i] != stalon[i-1] || 
		   stalat[i] != stalat[i-1] ) {
		if (strncmp(staname[i], "-12345", 6)) { 
		  sprintf(pstext, "%s%1.3f %1.3f  10 0 0 BL  %s \n",
			  pstext, stalon[i],stalat[i],staname[i]);
		}
	      }
	    }
	  }
	  strcat(pstext,__EOF__ "\n\n");
	  fputs("# Add station location labels.  \n"
		"# Input are longitude, latitude, and name.\n"
		"#\n",fp);
	  fputs(pstext, fp);
	}

	/* Add station locations to map. */
        memset(psxy, ' ', MAXSTRING);
	sprintf(psxy, "psxy -K -O -J");
	if ( mapJ == AZIM  || 
	     (deltalat > deltalatbig && deltalon > deltalonbig )) {
	  sprintf(psxy,"%s -R -St0.1i -G0/0/0 <<" __EOF__ " >> " PSVAR " \n", psxy);
	} else { 
	  sprintf(psxy,"%s -R -St0.25i -G0/0/0 <<" __EOF__ " >> " PSVAR " \n", psxy);
	}	  
	
        for( i=0; i < nch; i++ ){
	  if ( stalon[i] != latlon_undef && stalat[i] != latlon_undef ) {
	    if ( i == 0 || 
		 stalon[i] != stalon[i-1] || 
		 stalat[i] != stalat[i-1] ) {
	      sprintf(psxy,"%s%1.3f %1.3f \n",
		      psxy, stalon[i],stalat[i]);
	    }
	  }
        }        

        strcat(psxy, __EOF__ "\n\n");
        fputs("# Add station locations.  \n"
	      "# Input are longitude latitude.\n"
	      "#\n",fp);
        fputs(psxy, fp);

	/* Draw a great circle path along the back azimuth if the 
	 * information is available.                              
	 */	
        memset(bazim, 0, sizeof(bazim));
        getbbv("BBFK_BAZIM", bazim, nerr, 10, 9);
        if( *nerr == 0 ){
          if( !lrefset ){
	    /* Use first station in memory as the reference station. */
            getfil(1, FALSE, &nlen, &ndxy, &ndxx, nerr );
            irefsta = 1;
	  } else {
	    /* Get station location of the specified reference station. */
            for( irefsta = 1; irefsta <= nch; irefsta++ ){
              getfil( irefsta, FALSE, &nlen, &ndxy, &ndxx, nerr );
              if( !strcmp(kuser1,kstnm) )
		break;
	    }
	  }
	  memset(psxy, ' ', MAXSTRING);
	  sprintf(psxy, 
		  "psxy -K -O -J -R "
		  "-SV0.025/0.2/0.2 -G125 <<" __EOF__ " >> " PSVAR " \n"
		  "%.3f %.3f %s 1 \n"
		  __EOF__ "\n\n",
		  stalon[irefsta-1], stalat[irefsta-1], bazim);
          fputs("# Draw a great circle path along the back azimuth.\n"
		"# Input are longitude latitude azimuth size in inches.\n"
		"#\n",fp);
          fputs(psxy, fp);

	  /* Draw a line from reference station to event. */
          memset(psxy, ' ', MAXSTRING);
	  sprintf(psxy, 
		  "psxy -K -O -J -R <<" __EOF__ " >> " PSVAR " \n"
		  "%.3f %.3f \n"
		  "%.3f %.3f \n"
		  __EOF__ "\n\n", 
		  stalon[irefsta-1], stalat[irefsta-1],
		  evlon[irefsta-1], evlat[irefsta-1]);
          fputs("# Draw a line from reference station to event.\n",fp);
          fputs(psxy,fp);
	} else {
          *nerr = 0;
	} /* Great Circle along the Back Azimuth */

	/* Add event locations to map if any exist. */
        if( neventlocs > 0 ){

	  /* do event symbols/size/magnitude/positive residuals first */
          memset(psxy, ' ', MAXSTRING);
	  /* put the initial command line out */
	  if( lsize ) {
	    sprintf(psxy, "psxy -O -K -J -R "
		    "-Sc -W10/0/0/0 <<" __EOF__ " >> " PSVAR " \n");
	  } else if( lresid ) {
	    sprintf(psxy, "psxy -O -K -J -R "
		    "-Sx -W10/255/0/0 <<" __EOF__ " >> " PSVAR " \n");
	  } else {
	    sprintf(psxy, "psxy -O -K -J -R -W10/0/0/0 ");
	    if ( mapJ == AZIM || 
		 ( deltalat > deltalatbig && deltalon > deltalonbig ) ) {
	      strcat(psxy," -Sc0.1i ");
	    } else { 
	      strcat(psxy," -Sc0.25i ");
	    }
	    sprintf(psxy, "%s <<" __EOF__ " >> " PSVAR " \n", psxy);
	  }
	  
	  /* put the xy locations and sizes out */
          for( i=0; i<nch; i++) {
	    if ( i == 0 || evlon[i] != evlon[i-1] || evlat[i] != evlat[i-1] ) {
	      if( evlon[i] != latlon_undef ){
		if( lsize ) {
		  sprintf(psxy, "%s" "%1.3f %1.3f %1.3f \n",
			  psxy, evlon[i],evlat[i],size[i]);
		} else if( lresid ) {
		  if (size[i] != size_undef && size[i] >= 0.0 ) {
		    sprintf(psxy, "%s" "%1.3f %1.3f %1.3f \n",
			    psxy, evlon[i],evlat[i],fabs(size[i]) ); 
		  }
		} else {
		  sprintf(psxy, "%s" "%1.3f %1.3f \n",
			  psxy, evlon[i],evlat[i]);
		}
	      }
	    }
	  }
          strcat(psxy, __EOF__ "\n\n");
	  if (lresid) {
	    fputs("# Add event locations with positive residuals.\n"
		  "# Input are longitude latitude.\n"
		  "#\n",fp); 
	  }else{
	    fputs("# Add event locations.\n"
		  "# Input are longitude latitude.\n"
		  "#\n",fp); 
	  }
          fputs(psxy, fp);
	  
	  /* do negative residuals if any exist */
	  if( lresid ){
	    memset(psxy, ' ', MAXSTRING);
	    sprintf(psxy, "psxy -O -K -J -R "
		    "-Sc -W10/0/0/0 <<" __EOF__ " >> " PSVAR " \n");

	    for( i=0; i < nch; i++) {
	      if ( i == 0 || 
		   evlon[i] != evlon[i-1] || evlat[i] != evlat[i-1] ) {
		if( evlon[i] != latlon_undef && 
		    size[i] != size_undef && 
		    size[i] < 0.0 ) {
		  sprintf(psxy, "%s" "%1.3f %1.3f %1.3f \n",
			  psxy, evlon[i],evlat[i],fabs(size[i]) );
		}
	      }
	    }
	    strcat(psxy, __EOF__ "\n\n");
	    fputs("# Add event locations with negative residuals.\n"
		  "# Input are longitude latitude.\n"
		  "#\n",fp); 
	    fputs(psxy, fp);
	  }

	  /* Put a legend on the map if doing size/mag/resid */
	  if(plotlegend) {
	    if (lsize) {
	      fprintf(fp, "# Add Legend for Magnitude \n#\n"); 
	      plot_legend_box(fp,legendx,legendy, PSVAR);
	      plot_legend_symbols(fp, "-Sc -W10/0/0/0 ",  legendx, legendy,
				  minscale, (minscale + maxscale)/2, maxscale,
				  PSVAR);
	      plot_legend_text(fp, legendx, legendy,
			       minsize, (minsize + maxsize)/2, maxsize,
			       PSVAR);
	    } else if (lresid) {
	      /* Negative Residual Legend */
	      if (nneg > 0) {
		fprintf(fp,"# Add Legend for Negative Residuals.\n#\n");
		plot_legend_box(fp, legendx, legendy, PSVAR);
		plot_legend_symbols(fp, "-Sc -W10/0/0/0 ", legendx, legendy,
				    slope * minneg + intercept,
				    slope * ((minneg +maxneg)/2) + intercept,
				    slope * maxneg + intercept,
				    PSVAR);
		plot_legend_text(fp, legendx, legendy,
				 -minneg, (-minneg - maxneg)/2, -maxneg,
				 PSVAR);
	      }
	      /* positive resid legend */ 
	      if (npos > 0) {
		legendx = (nneg > 0) ? legendx+1 : legendx;
		fprintf(fp,"# Add Legend for Positive Residuals.\n#\n");
		plot_legend_box(fp, legendx, legendy, PSVAR);
		plot_legend_symbols(fp, "-Sx -W10/255/0/0 ", legendx, legendy,
				    slope * minpos + intercept,
				    slope * ((minpos + maxpos)/2) + intercept,
				    slope * maxpos + intercept,
				    PSVAR);
		plot_legend_text(fp, legendx, legendy,
				 minpos, (minpos + maxpos)/2, maxpos,
				 PSVAR);
	      }
	    }
	  }
        }
	fprintf(fp, 
		"# End the Postscript File\n"
		"# \n"
		"psxy -R -J -O /dev/null >> " PSVAR " \n\n");

        fclose(fp);

        sprintf(syscommand, "chmod +x %s", scriptname);
        system(syscommand);
        system(scriptname);

	/* Show the map. */
	/* Get the user's postscript viewer preference. */
        if((viewer = getenv("SACPSVIEWER")) != NULL){
          strcpy(syscommand,viewer);
	}else{
	  /* Use the default. */
	  strcpy(syscommand,
		 (use_tty()) ? 
		 SACPSVIEWER_DEFAULT : 
		 SACPSVIEWER_DEFAULT_NO_TTY );
	  fprintf(stderr, 
		  "Using Default Postscript Viewer\n"
		  "\t%s\n"
		  "\tSet an alternative through the SACPSVIEWER "
		  "environment variable\n"
		  "\t%s\n",
		  (use_tty()) ? 
		  SACPSVIEWER_DEFAULT : 
		  SACPSVIEWER_DEFAULT_NO_TTY,
		  (use_tty()) ? 
		  "Press any key to continue" : 
		  "Running SAC within a script, Terminal Mode Disabled");
	}

	sprintf(syscommand,"%s %s  & ", syscommand, output);
	sc_len = strlen(syscommand);
	zsysop(syscommand, 0, &sc_len, nerr);

L_9999:
	/* Clean up memory before finishing */
	if(staname != NULL) {
	  for (idx = 0; idx < number_event_station; idx++ ) {
	    if(staname != NULL) {
	      free (staname[idx]);
	    }
	  }
	  free (staname);
	}
	
        if( buf != NULL) 
	  free(buf);
	return;

} 

