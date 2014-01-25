/** \file
 * Handle Polezero Subtype Files for transfer
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#include <ctype.h>

#include "icm.h"
#include "complex.h"
#include "bool.h"
#include "hdr.h"

#include "string_utils.h"

#include "co.h"
#include "msg.h"
#include "bot.h"
#include "ucf.h"

#include "EVRESPnames.h"

#include "datetime.h"

#define	MPOLES	30
#define	MZEROS	30

#define KEY_CONSTANT  "CONSTANT"
#define KEY_ZEROS     "ZEROS"
#define KEY_POLES     "POLES"
#define KEY_STAR      '*'


typedef struct _pzmeta_t pzmeta_t;
typedef struct _pzcomment_t pzcomment_t;

struct _pzmeta_t {
        char     *net;
        char     *stat;
        char     *chan;
        char     *loc;
        datetime *created;
        datetime *start;
        datetime *end;
        char     *descrip;
        float     lat;
        float     lon;
        float     elev;
        float     depth;
        float     dip;
        float     az;
        float     sample_rate;
        char     *input_unit;
        char     *output_unit;
        char     *instrument_type;
        float     gain;
        char     *comment;
        float     sensitivity;
        float     a0;
};

struct _pzcomment_t {
        char *key;
        void (*parse)(char *p, pzmeta_t *meta, pzcomment_t *c);
        size_t off;
};


static char *
polezero_comment_token(char *p) {
  if(!p) {
    return p;
  }
  p = strchr(p, ':');
  if(p) {
    p = p + 1;
    p = lstrip(p);
    p = rstrip(p);
  }
  return p;
}

void
polezero_comment_string(char *p, pzmeta_t *meta, pzcomment_t *c) {
        char *pp;
        char **s;
        s = (char **) ((char *) meta + c->off);
        if(*s) {
          free(*s);
          *s = NULL;
        }
        pp = polezero_comment_token( p );
        if(!pp || !*pp) {
          *s = NULL;
          return;
        }
        *s = strdup( pp );
        if(!*s) {
          fprintf(stdout, "polezero-comment: Error converting string: '%s'\n", p);
        }
}

void
polezero_comment_datetime(char *p, pzmeta_t *meta, pzcomment_t *c) {
        char *pp;
        datetime **s;
        s = (datetime **) ((char *)meta + c->off);
        *s = NULL;
        pp = polezero_comment_token( p );
        if(!pp || !*pp) {
          datetime_free(*s);
          return;
        }
        *s = datetime_parse( pp, *s);
        if(!*s) {
          fprintf(stdout, "polezero-comment: Error parsing datetime: '%s'\n", p);
        }
}

int
is_numexp(char c) {
    return ( isdigit(c) || 
             c == '.' || 
             c == '+' || c == '-' || 
             c == 'e' || c == 'E' ||
             c == 'd' || c == 'D' 
             );
}

void
polezero_comment_float(char *p, pzmeta_t *meta, pzcomment_t *c) {
        char *pp;
        char *s;
        int nerr;
        float *f;
        nerr = 1;
        f = (float *) ((void *) meta + c->off);
        pp = polezero_comment_token( p );
        if(!pp || !*pp) {
          *f = 0.0;
          return;
        }
        s = strdup(pp);
        pp = s;
        /* Find the end of the number */
        while(pp && is_numexp(*pp)) {
            pp++;
        }
        /* Truncate the string */
        if(pp && *pp) {
            *pp = 0;
        }
        *f = cdouble(s, &nerr);
        free(s);
        s = NULL;
        if(nerr != 0) {
          fprintf(stdout, "polezero-comment: Error converting float: '%s'\n", p);
          *f = 0.0;
        }
}

pzcomment_t pzc[] = { 
        { " network ",     polezero_comment_string,   offsetof(pzmeta_t, net) },
        { " station ",     polezero_comment_string,   offsetof(pzmeta_t, stat) },
        { " channel ",     polezero_comment_string,   offsetof(pzmeta_t, chan) },
        { " component ",   polezero_comment_string,   offsetof(pzmeta_t, chan) },
        { " location ",    polezero_comment_string,   offsetof(pzmeta_t, loc) },
        { " description ", polezero_comment_string,   offsetof(pzmeta_t, descrip) },
        { " input_unit ",  polezero_comment_string,   offsetof(pzmeta_t, input_unit) },
        { " output_unit ", polezero_comment_string,   offsetof(pzmeta_t, output_unit) },
        { " insttype ",    polezero_comment_string,   offsetof(pzmeta_t, instrument_type) },
        { " start ",       polezero_comment_datetime, offsetof(pzmeta_t, start)},
        { " end ",         polezero_comment_datetime, offsetof(pzmeta_t, end)},
        { " created ",     polezero_comment_datetime, offsetof(pzmeta_t, created)},
        { " latitude ",    polezero_comment_float,    offsetof(pzmeta_t, lat)},
        { " longitude ",   polezero_comment_float,    offsetof(pzmeta_t, lat)},
        { " elevation ",   polezero_comment_float,    offsetof(pzmeta_t, elev)},
        { " depth ",       polezero_comment_float,    offsetof(pzmeta_t, depth)},
        { " dip ",         polezero_comment_float,    offsetof(pzmeta_t, dip)},
        { " azimuth ",     polezero_comment_float,    offsetof(pzmeta_t, az)},
        { " sample rate ", polezero_comment_float,    offsetof(pzmeta_t, sample_rate)},
        { " instgain ",    polezero_comment_float,    offsetof(pzmeta_t, gain)},
        { " comment ",     polezero_comment_string,   offsetof(pzmeta_t, comment)},
        { " sensitivity ", polezero_comment_float,    offsetof(pzmeta_t, sensitivity)},
        { " a0 ",          polezero_comment_float,    offsetof(pzmeta_t, a0)},
};

void
polezero_meta_init( pzmeta_t *meta) {
        meta->net             = NULL;
        meta->stat            = NULL;
        meta->chan            = NULL;
        meta->loc             = NULL;
        meta->created         = NULL;
        meta->start           = NULL;
        meta->end             = NULL;
        meta->descrip         = NULL;
        meta->lat             = 0.0;
        meta->lon             = 0.0;
        meta->elev            = 0.0;
        meta->depth           = 0.0;
        meta->dip             = 0.0;
        meta->az              = 0.0;
        meta->sample_rate     = 0.0;
        meta->input_unit      = NULL;
        meta->output_unit     = NULL;
        meta->instrument_type = NULL;
        meta->gain            = 0.0;
        meta->sensitivity     = 0.0;
        meta->comment         = NULL;
        meta->a0              = 0.0;
}

pzmeta_t *
polezero_meta_new() {
        pzmeta_t *meta;
        meta = (pzmeta_t *) malloc(sizeof(pzmeta_t));
        if(!meta) {
                fprintf(stderr, "Error allocating space for polezero meta data\n");
                return NULL;
        }
        polezero_meta_init( meta );
        return meta;
}

#define STRINGCOPY(to, from, field) do {                        \
                if(from->field) {                               \
                        to->field = strdup( from->field );      \
                } else {                                        \
                        to->field = strdup(" ");                \
                }                                               \
        } while(0)

pzmeta_t *
polezero_meta_copy(pzmeta_t *m) {
        pzmeta_t *new;
        if(!m) {
                return NULL;
        }
        new = polezero_meta_new();
        if(!new) {
                return NULL;
        }
        STRINGCOPY(new, m, net);
        STRINGCOPY(new, m, stat);
        STRINGCOPY(new, m, chan);
        STRINGCOPY(new, m, loc);
        STRINGCOPY(new, m, input_unit);
        STRINGCOPY(new, m, output_unit);
        STRINGCOPY(new, m, instrument_type);
        STRINGCOPY(new, m, comment);

        new->created         = datetime_copy( m->created );
        new->start           = datetime_copy( m->start );
        new->end             = datetime_copy( m->end );
        new->lat             = m->lat;
        new->lon             = m->lon;
        new->elev            = m->elev;
        new->depth           = m->depth;
        new->dip             = m->dip;
        new->az              = m->az;
        new->sample_rate     = m->sample_rate;
        new->gain            = m->gain;
        new->sensitivity     = m->sensitivity;
        new->a0              = m->a0;        

        return new;
}


#define FREE( x ) do {                          \
                if( x ) {                       \
                        free( x );              \
                        x = NULL;               \
                }                               \
        } while( 0 )

void
polezero_meta_free( pzmeta_t *meta ) {
        if(!meta) {
                return;
        }
        FREE( meta->net );
        FREE( meta->stat );
        FREE( meta->chan );
        FREE( meta->loc );
        FREE( meta->descrip );
        FREE( meta->input_unit );
        FREE( meta->output_unit );
        FREE( meta->instrument_type );
        datetime_free( meta->start );
        datetime_free( meta->end );
        datetime_free( meta->created );
        FREE( meta );
}

/*
 * Find the first occurrence of find in s, ignore case.
 */
char *
strcasestr_bsd(const char *s, const char *find)
{
  char c, sc;
  size_t len;
  if(!s) {
      return NULL;
  }
  if ((c = *find++) != 0) {
    c = (char)tolower((unsigned char)c);
    len = strlen(find);
    do {
      do {
          if ((sc = *s++) == 0)
              return (NULL);
      } while ((char)tolower((unsigned char)sc) != c);
    } while (strncasecmp(s, find, len) != 0);
    s--;
  }
  return ((char *)s);
}

void
polezero_comment_parse(char *line, pzmeta_t *meta) {
        int i, n;
        char *p;
        n = sizeof(pzc) / sizeof(pzcomment_t);
        for(i = 0; i < n; i++) {
                p = strcasestr_bsd(line, pzc[i].key);
                if(p != NULL && ! strcasestr_bsd(line, "channel flag")) {
                        pzc[i].parse( p, meta, &(pzc[i]) );
                }
        }
        return;
}

datetime *
datetime_get_file_time( datetime *t ) {
        int dir;

        if(!t) {
                t = datetime_new( );
        }

        dir = getTransferDirection();
        if(isSet(TIME, dir)) {
            datetime_set_hour(t,  getTime( dir, EV_HOUR ));
            datetime_set_minute(t, getTime( dir, EV_MIN ));
            datetime_set_second(t, getTime( dir, EV_SEC ));
            datetime_set_nanosecond(t, getTime( dir, EV_MSEC ) * 1000000);
        } else {
            datetime_set_hour(t, *nzhour);
            datetime_set_minute(t, *nzmin);
            datetime_set_second(t, *nzsec);
            datetime_set_nanosecond(t, *nzmsec * 1000000);
        }
        if(isSet(DATE, dir)) {
            datetime_set_year(t, getYear( dir ) - 1900);
            datetime_set_doy(t, getJday( dir ) - 1);
            datetime_doy2ymd( t );
        } else {
            datetime_set_year(t, *nzyear);
            datetime_set_doy(t, *nzjday);
            datetime_doy2ymd( t );
        }
        datetime_normalize( t );
        return t;
}

int
sdef(char *s) {
    return (s && strlen(s) > 0);
}

int
polezero_is_correct_block(pzmeta_t *meta, 
                          datetime *filetime, 
                          char     *stat, 
                          char     *net, 
                          char     *loc, 
                          char     *chan) {
    if((sdef(meta->stat) && sdef(stat) && strcasecmp(stat, meta->stat) != 0) ||
       (sdef(meta->net)  && sdef(net)  && strcasecmp(net,  meta->net)  != 0) ||
       (sdef(meta->loc)  && sdef(loc)  && strcasecmp(loc,  meta->loc)  != 0) ||
       (sdef(meta->chan) && sdef(chan) && strcasecmp(chan, meta->chan) != 0) ) {
        return 0;
    }
    if(filetime &&
       datetime_status(meta->start) == DATETIME_OK &&
       datetime_status(meta->end)   == DATETIME_OK) {
        return datetime_in_span(filetime, meta->start, meta->end);
    }
    return TRUE;
}

/** 
 * Handle a Pole Zero file \p subtyp for the transfer command.  Parse and return
 *   the appropriate variables.
 *
 *   Generic transfer function - user supplies poles and zeros
 *        Search for the polezero file.  Search order is:
 *         - (1) current directory.
 *         - (2) global polezero directory. 
 *
 *   Polezero understands Four different Key Words
 *      - CONSTANT real-number
 *          Scaling Value
 *      - ZEROS npoles
 *          Number of Zeros
 *          Location of Complex Zeros follow -->  Real Imaginary
 *      - POLES npoles
 *          Number of Poles
 *          Location of Complex Poles follow -->  Real Imaginary
 *      - * 
 *          Comment, rest of the line is ignored
 *
 * \param nfreq
 *     Input Number of frequencies.  Defined from the maximum number of points in the
 *     entire data file list: npts_max.  The next larger power of 2 is taken
 *     from npts_max to find the the number of points for the FFT: nfft.  Then
 *     nfft is divided by 2 and given one more point. 
 *     \see xtransfer
 * \param delfrq
 *     Input Frequency spacing, defined as 1 / ( nfft * dt ). 
 *     \see transfer
 * \param xre
 *     Output Real Part of the transfer function in the frequency domain.
 *     Length \p nfreq
 * \param xim
 *     Output Imaginary Part of the transer function in the frequency domain.
 *     Length \p nfreq
 * \param subtyp
 *     Input Name of file containing the Pole Zero Response
 * \param subtyp_s
 *     Input Length of string \p subtyp
 * \param nerr
 *     Error return flag
 *          - 0 on Success
 *          - Non-Zero on Error
 *
 * \see xtransfer
 *
 *
 * \date   970129:  Add parameter (0) to cnvatf and cnvati.  0 means that if
 *                  a string of digits is too long, let it slide by.  maf 
 * \date   071003   Documented/Reviewed
 * \date   071003   Fixed a bug with the length of a string key.  Was defined as
 *                  length 9, now automatically determined.
 */


void 
polezero(int   nfreq, 
	 double     delfrq, 
	 double     xre[], 
	 double     xim[], 
	 char      *subtyp, 
	 int        subtyp_s, 
	 int  *nerr)
{
	char kfile[MCPFN+1], kiline[MCMSG+1];
	char *key;
        char *kline;
	int lexist, lopen, lpoles, lzeros;
	int i, idx, ic, ic1, ic2, ipoles, itype, izeros, nc, ncerr, 
	 npoles, nzeros, numsave;
        FILE *nun;
	float temp1, temp2;
	double const_ ;
	complexf poles[MPOLES], zeros[MZEROS];
    char *s1;
        
    pzmeta_t *meta, *meta_used;
    datetime *filetime;

    char *stat, *net, *loc, *chan;
    char *pstat, *pnet, *ploc, *pchan;


	complexf *const Poles = &poles[0] - 1;
	complexf *const Zeros = &zeros[0] - 1;

    memset(kfile, 0, sizeof(kfile));
    memset(kiline, 0, sizeof(kiline));

        meta = NULL;
        meta_used = NULL;

        for( idx = 0 ; idx < MCPFN ; idx++ )
            kfile[ idx ] = ' ' ;
        kfile[ MCPFN ] = '\0' ;

        filetime = datetime_get_file_time( NULL );
        meta = polezero_meta_new( );
        pstat = strdup(kstnm);
        pnet  = strdup(knetwk);
        ploc  = strdup(khole);
        pchan = strdup(kcmpnm);
        stat = rstrip(lstrip(pstat));
        net  = rstrip(lstrip(pnet));
        loc  = rstrip(lstrip(ploc));
        chan = rstrip(lstrip(pchan));
        if(! SAC_CHAR_DEFINED(stat) ) { stat[0] = 0; }
        if(! SAC_CHAR_DEFINED(net) )  { net[0]  = 0; }
        if(! SAC_CHAR_DEFINED(loc) )  { loc[0]  = 0; }
        if(! SAC_CHAR_DEFINED(chan) ) { chan[0] = 0; }


	/*     generic transfer function - user supplies poles and zeros */
	/* - Search for the polezero file.  Search order is:
	 *   (1) current directory.
	 *   (2) global polezero directory. */
	lopen = FALSE;
	fstrncpy( kfile, MCPFN, subtyp, strlen(subtyp));
	zinquire( kfile, &lexist );
	if( lexist )
		goto L_5000;

	/* Look in global polezero directory 
	   ${SACAUX}/polezeros
	   This code will not work as kmcreq is never set.
	   The idea behind this code is interesing
	   but possibly dangerous through the use of an 
	   uninitilized value.  -BKS
	 */
	/*
	zbasename( kfile,MCPFN+1 );
	crname( kfile,MCPFN+1, KSUBDL, "polezeros",10, nerr );
	if( *nerr != 0 )
		goto L_8888;
	crname( kfile,MCPFN+1, KDIRDL, kmcreq,9, nerr );
	if( *nerr != 0 )
		goto L_8888;
	zinquire( kfile, &lexist );
	if( lexist )
		goto L_5000;
	*/

	/* - Raise error condition if macro file does not exist. */

	*nerr = 108;
	setmsg( "ERROR", *nerr );
	apcmsg( subtyp,subtyp_s );
	goto L_8888;


	/* - Set default values for constant, poles, and zeros. */

L_5000:
	const_ = 1.0;
        npoles = 0;
        nzeros = 0;
	for( i = 1; i <= MZEROS; i++ ){
		Zeros[i] = flttocmplx( 0.0, 0.0 );
		}

	for( i = 1; i <= MPOLES; i++ ){
		Poles[i] = flttocmplx( 0.0, 0.0 );
		}

	/* - Open file. */

	zopens( &nun, kfile,MCPFN+1, "ROTEXT",7, nerr );
	if( *nerr != 0 )
		goto L_8888;
	lopen = TRUE;

	/* - Read and decode lines in file. */

	lpoles = FALSE;
	lzeros = FALSE;
L_6000:
        if(fgetsp(kiline,MCMSG+1,nun)==NULL) {
          if(feof(nun)) goto L_7000;
          goto L_9000;
	}

        /* remove leading blanks. */
        kline = kiline;
        while( (*kline == ' ') || (*kline == '\n') || (*kline == '\t') ) {
                kline++;
        }

        /* Continue if empty line */
        if ( (numsave = strlen(kline)) == 0 ) {
                goto L_6000;
        }
        /* Convert newline into space */
        if( kline[numsave-1] == '\n' ) {
                kline[numsave-1] = ' ';
        }
 
        /* eliminate tabs in the input line */
        for ( i = 0; i < numsave; i++) {
                if(kline[i] == '\t') {
                        kline[i] = ' ';
                }
        }
        nc = indexb( kline,strlen(kline)+1 );
	ic = 0;
	poptok( kline, nc, &ic, &ic1, &ic2, &itype );

	/* Make space for the possible KeyValue and truncate the string */
	key = strdup(kline);
        memset(key, 0, strlen(key));

	modcase( TRUE, kline+ic1 - 1, ic2 - ic1 + 1, key );

	if( key[0] == KEY_STAR) { /* Comment Line */
        polezero_comment_parse( kiline, meta );
    } else if(!polezero_is_correct_block(meta, filetime, stat, net, loc, chan) ) {

    } else if( strncmp(key, KEY_CONSTANT, strlen(KEY_CONSTANT) ) == 0 ){
		poptok( kline, nc, &ic, &ic1, &ic2, &itype );
        strncpy((s1=malloc(ic2-ic1+2)),kline+ic1 - 1,ic2-ic1+1);
        s1[ic2-ic1+1] = '\0';
        
        const_ = atof( s1 ) ;
                if( const_ == 0 || const_ == HUGE_VAL || 
                    const_ == -HUGE_VAL || isnan( const_ ) ){
                        *nerr = 2118 ;
                        setmsg( "ERROR", *nerr ) ;
                        apcmsg( "Unrecognized Constant: ", 24 ) ;
                        apcmsg( s1 , strlen( s1 ) + 1 ) ;
			free(s1) ;
                        goto L_8888 ;
                }
		free(s1) ;
                meta_used = polezero_meta_copy( meta );
		
	}
	else if( strncmp(key,KEY_POLES, strlen(KEY_POLES)) == 0 ){
		poptok( kline, nc, &ic, &ic1, &ic2, &itype );
                strncpy((s1=malloc(ic2-ic1+2)),kline+ic1 - 1,ic2-ic1+1);
                s1[ic2-ic1+1] = '\0';
		cnvati( s1, ic2-ic1 + 2, &npoles, 0, nerr ); /* add 0 before nerr. maf 970129 */
		free(s1);
		if( *nerr != 0 )
			goto L_8888;
		if( npoles > MPOLES ){
			*nerr = 2109;
			setmsg( "ERROR", *nerr );
			apcmsg( subtyp,subtyp_s );
			apimsg( MPOLES );
			goto L_8888;
			}
		lpoles = TRUE;
		lzeros = FALSE;
		ipoles = 0;
		}
	else if( strncmp(key,KEY_ZEROS, strlen(KEY_ZEROS)) == 0 ){
		poptok( kline, nc, &ic, &ic1, &ic2, &itype );
                strncpy((s1=malloc(ic2-ic1+2)),kline+ic1 - 1,ic2-ic1+1);
                s1[ic2-ic1+1] = '\0';
		cnvati( s1, ic2-ic1 + 2, &nzeros, 0, nerr ); /* add 0 before nerr. maf 970129 */
		free(s1);
		if( *nerr != 0 )
			goto L_8888;
		if( nzeros > MZEROS ){
			*nerr = 2109;
			setmsg( "ERROR", *nerr );
			apcmsg( subtyp,subtyp_s );
			apimsg( MZEROS );
			goto L_8888;
			}
		lpoles = FALSE;
		lzeros = TRUE;
		izeros = 0;
		}
	else if( lpoles ){
		if( ipoles < MPOLES ){
			ipoles = ipoles + 1;
                        strncpy((s1=malloc(ic2-ic1+2)),kline+ic1 - 1,ic2-ic1+1);
                        s1[ic2-ic1+1] = '\0';
			cnvatf( s1, ic2- ic1 + 2, &temp1, 0, nerr ); /* add 0 before nerr. maf 970129 */
                        if( *nerr != 0 ){
                            *nerr = 2126 ;
                            setmsg( "ERROR", *nerr ) ;
                            apcmsg( s1 , strlen( s1 ) + 1 ) ;
                            free(s1) ;
                            goto L_8888 ;
                        }
                        free(s1) ;
			poptok( kline, nc, &ic, &ic1, &ic2, &itype );
                        strncpy((s1=malloc(ic2-ic1+2)),kline+ic1 - 1,ic2-ic1+1);
                        s1[ic2-ic1+1] = '\0';
			cnvatf( s1, ic2- ic1 + 2, &temp2, 0, nerr ); /* add 0 before nerr. maf 970129 */
                        if( *nerr != 0 ){
                            *nerr = 2126 ;
                            setmsg( "ERROR", *nerr ) ;
                            apcmsg( s1 , strlen( s1 ) + 1 ) ;
                            free(s1) ;
                            goto L_8888 ;
                        }
                        free(s1) ;
			Poles[ipoles] = flttocmplx( temp1, temp2 );
			}
		else{
			*nerr = 2108;
			setmsg( "ERROR", *nerr );
			apcmsg( subtyp,subtyp_s );
			apimsg( MPOLES );
			goto L_8888;
			}
		}
	else if( lzeros ){
		if( izeros < MZEROS ){
			izeros = izeros + 1;
                        strncpy((s1=malloc(ic2-ic1+2)),kline+ic1 - 1,ic2-ic1+1);
                        s1[ic2-ic1+1] = '\0';
			cnvatf( s1, ic2- ic1 + 2, &temp1, 0, nerr ); /* add 0 before nerr. maf 970129 */
                        if( *nerr != 0 ){
                            *nerr = 2127 ;
                            setmsg( "ERROR", *nerr ) ;
                            apcmsg( s1 , strlen( s1 ) + 1 ) ;
                            free(s1) ;
                            goto L_8888 ;
                        }
                        free(s1) ;
			poptok( kline, nc, &ic, &ic1, &ic2, &itype );
                        strncpy((s1=malloc(ic2-ic1+2)),kline+ic1 - 1,ic2-ic1+1);
                        s1[ic2-ic1+1] = '\0';
			cnvatf( s1,ic2 - ic1 + 2, &temp2, 0, nerr ); /* add 0 before nerr. maf 970129 */
                        if( *nerr != 0 ){
                            *nerr = 2127 ;
                            setmsg( "ERROR", *nerr ) ;
                            apcmsg( s1 , strlen( s1 ) + 1 ) ;
                            free(s1) ;
                            goto L_8888 ;
                        }
                        free(s1) ;
			Zeros[izeros] = flttocmplx( temp1, temp2 );
			}
		else{
			*nerr = 2109;
			setmsg( "ERROR", *nerr );
			apcmsg( subtyp,subtyp_s );
			apimsg( MZEROS );
			goto L_8888;
			}
		}
	else{
		*nerr = 2110;
		setmsg( "ERROR", *nerr );
		apcmsg( subtyp,subtyp_s );
		apcmsg( key,9 );
		goto L_8888;
		}
	free(key);
	goto L_6000;

	/* - Compute transfer function. */

L_7000:
    if(const_ == 1.0 && npoles == 0 && nzeros == 0) {
        *nerr = 2114;
        error(*nerr, "\n Station: %s.%s.%s.%s", net, stat, chan, loc);
        if(meta && filetime &&
           datetime_status(meta->start) == DATETIME_OK &&
           datetime_status(meta->end)   == DATETIME_OK ) {
            printf(" Time of data not found in file\n Date Time: "); 
            datetime_printn(filetime);
            printf("\n");
        }
    } else {
        printf(" Using polezero response for %s, %s, %s, %s...\n", stat, chan, net, loc);
        if(FALSE) {
            printf("\n");
            if(meta_used &&
               datetime_status(meta_used->start) == DATETIME_OK &&
               datetime_status(meta_used->end)   == DATETIME_OK ) {
                printf("  Station:  %s.%s.%s.%s <> %s.%s.%s.%s \n", 
                       meta_used->net, meta_used->stat, meta_used->chan, meta_used->loc, 
                       net, stat, chan, loc);
                printf("  On:       "); datetime_printn(meta_used->start);
                printf("  Off:      "); datetime_printn(meta_used->end);
                printf("  File:     "); datetime_printn(filetime);
            } else {
                printf("  Station:  %s.%s.%s.%s \n", net, stat, chan, loc);
            }
            printf("  Polezero Response\n");
            printf("\tconstant: %e\n", const_);
            printf("\tzeros:    %d\n", nzeros);
            for(i = 0; i < nzeros; i++) {
                printf("\t    %e  %e\n", zeros[i].re, zeros[i].im);
            }
            printf("\tpoles:    %d\n", npoles);
            for(i = 0; i < npoles; i++) {
                printf("\t    %e  %e\n", poles[i].re, poles[i].im);
            }
        }
    }
        
	getran( nfreq, delfrq, const_, nzeros, zeros, npoles, poles, xre, xim );
        datetime_free( filetime );
        polezero_meta_free( meta );
        polezero_meta_free( meta_used );
        
L_8888:
        if(pstat) { free(pstat); }
        if(pchan) { free(pchan); }
        if(pnet)  { free(pnet);  }
        if(ploc)  { free(ploc);  }
        pstat = pchan = pnet = ploc = NULL;

	if( lopen )
		zcloses( &nun, &ncerr );
	return;

L_9000:
	*nerr = 114;
	setmsg( "ERROR", *nerr );
	apcmsg( kfile,MCPFN+1 );
	goto L_8888;

}

