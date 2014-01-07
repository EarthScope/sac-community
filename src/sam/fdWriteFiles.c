
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sam.h"
#include "hdr.h"
#include "amf.h"
#include "dfm.h"
#include "bool.h"
#include "SacHeader.h"
#include "ucf.h"
#include "ncpf.h"

#include "co.h"
#include "msg.h"
#include "dff.h"

void /* FUNCTION */ fdWriteFiles ( float *memptr[10] , char * kprefix ,
				   float * userData , int newnpts ,
				   int * nerr )
{

	/* index sacmem for amplitude, phase, group delay,
	   and the impulse response. */
	int fileDescriptor = 0 ,  xbegin = 0 ,
    idx, jdx, nlcmem, nlcdsk, nptwr;


	char kname[ MCPFN ] , ksuffix[ 3 ][ 6 ] ;
  sac *s;
	float *bufout = NULL , *ptr , amph[ 2 ][ 2 * NDATPTS - 2 ] ;	


	*nerr = 0;

	/* handle strings */
	if ( strlen ( kprefix ) > MCPFN - 4 )
	    kprefix[ MCPFN - 4 ] = '\0' ;
	strcpy ( ksuffix[ 0 ] , ".spec" ) ;
	strcpy ( ksuffix[ 1 ] , ".gd" ) ;
	strcpy ( ksuffix[ 2 ] , ".imp" ) ;

	/* Determine the begin of the impulse */
	for ( ptr = memptr[ 9 ] ; *ptr == 0.0 ; ptr++ )
	    xbegin++ ;

	/* fill the amplitude and phase array */
        for ( idx = 0 ; idx < NDATPTS ; idx++ ) {
            amph[ 0 ][ idx ] = memptr[ 6 ][ idx ] ;
            amph[ 1 ][ idx ] = memptr[ 7 ][ idx ] ;
        }

        for (  ; idx < 2 * NDATPTS - 2 ; idx++ ) {
            amph[ 0 ][ idx ] =  memptr[ 6 ][ 2*NDATPTS-idx-2 ] ;
            amph[ 1 ][ idx ] = -memptr[ 7 ][ 2*NDATPTS-idx-2 ] ;
        }
        s = sac_new();

	/* fill some fields. */
	for ( idx = 0 ; idx < 9 ; idx++ )		/* user fields */
    *( &(s->h->user0) + idx ) = userData[ idx ] ;

	switch ( (int) (s->h->user0 + 0.5) ) {
	    case 1:  strcpy ( s->h->kuser0 , "lowpass " ) ;
		     break ;
	    case 2:  strcpy ( s->h->kuser0 , "highpass" ) ;
		     break ;
	    case 3:  strcpy ( s->h->kuser0 , "bandpass" ) ;
		     break ;
	    case 4:  strcpy ( s->h->kuser0 , "bandrej " ) ;
		     break ;
	    default: strcpy ( s->h->kuser0 , "-12345  " ) ;
		     break ;
	}

	switch ( (int) (s->h->user1 + 0.5) ) {
	    case 1:  strcpy ( s->h->kuser1 , "Butter  " ) ;
		     break ;
	    case 2:  strcpy ( s->h->kuser1 , "Bessel  " ) ;
		     break ;
	    case 3:  strcpy ( s->h->kuser1 , "C1      " ) ;
		     break ;
	    case 4:  strcpy ( s->h->kuser1 , "C2      " ) ;
		     break ;
	    default: strcpy ( s->h->kuser1 , "-12345  " ) ;
		     break ;
	}

	fillNZ () ;					/* time fields */

	s->h->b      = 0.0 ;					/* other fields */
	s->h->sb     = 0.0 ;
	s->h->nvhdr  = 6 ;
	s->h->idep   = IUNKN ;
	s->h->iztype = IB ;
	s->h->leven  = TRUE ;
	s->h->lpspol = TRUE ;
	s->h->lovrok = TRUE ;
	s->h->lcalda = FALSE ;

	for ( jdx = 0 ; jdx < 3 ; jdx++ ) {	/* loop between output files. */
 
	    /* fill other header fields specific to the data */
	    switch ( jdx ) {
		case 0:	aphdr( newnpts ) ;
			nlcmem =  6  ;
			break ;
		case 1:	gdhdr( newnpts ) ;
			nlcmem =  8  ;
			break ;
		case 2:	irhdr( newnpts ) ;
			nlcmem =  9  ;
			break ;
		default: goto L_ERROR ;
	    }

	    /* Get file name */
	    sprintf ( kname , "%s%s" , kprefix , ksuffix[ jdx ] ) ;

	    /* Open file */
	    znfile( &fileDescriptor , kname , MCPFN , "DATA" , 5 , nerr );
	    if ( *nerr )
		goto L_ERROR ;

	    /* Get ready to write header to disk */
	    nlcdsk = 0;
	    nptwr = SAC_HEADER_WORDS_FILE;

	    if ( ( bufout = (float *) malloc ( SAC_HEADER_SIZEOF_FILE) ) == NULL ) {
		*nerr = 301;
		goto L_ERROR ;
	    }

	    /* move header into working memory */
	    /* copy ( (int*) cmhdr.fhdr , (int*) cmmem.sacmem[ hdrindex ] , SAC_HEADER_NUMBERS ); */
	    //copy_float( cmhdr.fhdr, cmmem.sacmem[ hdrindex ], SAC_HEADER_NUMBERS );
	    //zputc ( kmhdr.khdr[ 0 ] , 9 , (int *)(cmmem.sacmem[ hdrindex ] + SAC_HEADER_NUMBERS), 
      //		    ( MCPW + 1 ) * SAC_HEADER_STRINGS) ;

	    /* move header into output buffer */
	    //map_hdr_out ( cmmem.sacmem[ hdrindex ] , bufout , FALSE) ;

	    /* write the headers */
	    //zwabs( (int *)&fileDescriptor, (char *)(bufout), nptwr, (int *)&nlcdsk, (int *)nerr );

	    free(bufout);
	    bufout = NULL ;

	    nlcdsk += nptwr;
	    nptwr = NDATPTS ;

	    /* Write data to disk */

	    switch ( jdx ) {
		case 0:	nptwr = 2 * NDATPTS - 2 ;
		  zwabs ( (int *)&fileDescriptor, (char *)(amph[ 0 ]) , nptwr, (int *)&nlcdsk, (int *)nerr ) ;
		  /* nlcmem = memptr[ 7 ] ; */
		  nlcdsk += nptwr;
		  zwabs ( (int *)&fileDescriptor, (char *)(amph[ 1 ]) , nptwr, (int *)&nlcdsk, (int *)nerr ) ;
		  break ;
		  
	    case 1: zwabs( (int *)&fileDescriptor, (char *)(memptr[nlcmem]), nptwr, (int *)&nlcdsk, (int *)nerr );
	      break ;
	      
	    case 2: zwabs( (int *)&fileDescriptor, (char *)(memptr[nlcmem] + xbegin), nptwr, (int *)&nlcdsk, (int *)nerr );
	      break ;
	    }



	    /* Close file */
	    zclose ( &fileDescriptor , nerr ) ;
	    fileDescriptor = 0 ;
	} /* end for */

L_ERROR:

	if ( *nerr ) {
	    setmsg ( "ERROR" , *nerr ) ;
	    outmsg () ;
	    clrmsg () ;
	}

	if ( saclen() > 0 )
    if(!(s = sacget(0, TRUE, nerr))) {
    }
  //getfil ( 1 , TRUE , &unused1 , &unused2 , &unused3 , nerr ) ;

	if ( bufout ) 
	    free ( bufout ) ;
	if ( fileDescriptor ) 
	    zclose ( &fileDescriptor , nerr ) ;

}


void aphdr ( int newnpts )
{
  sac *s = sacget_current();
	s->h->nsnpts = newnpts ;
	s->h->npts = 2*NDATPTS - 2 ;
	s->h->sdelta = s->h->user6 ;
	s->h->delta = 1. / ( s->h->sdelta * (float) ( s->h->npts ) ) ;
	s->h->iftype = IAMPH ;

	strcpy ( s->h->kevnm , "FD: AMP/PH" ) ;
}


void gdhdr ( int newnpts )
{
  sac *s = sacget_current();
        s->h->nsnpts = newnpts ;
        s->h->npts = NDATPTS ;
        s->h->sdelta = s->h->user6 ;
        s->h->delta = 1. / ( s->h->sdelta * (float) ( s->h->npts ) ) ;
        s->h->iftype = ITIME ;

        strcpy ( s->h->kevnm , "FD: GROUP DELAY" ) ;
}

void irhdr ( int newnpts )
{
  sac *s = sacget_current();
        s->h->nsnpts = NDATPTS ;
        s->h->npts = newnpts ;
        s->h->delta = s->h->user6 ;
        s->h->sdelta = 1. / ( s->h->delta * (float) ( s->h->npts ) ) ;
        s->h->iftype = ITIME ;

        strcpy ( s->h->kevnm , "FD: IMPULSE" ) ;
}



int MDtoDoy () ;
int isLeapYear () ;


void fillNZ ()
{
	double time = tmGetEpochTime () ;
	int year , month , day , hour , minute ;
	float second ;
  sac *s;
  s = sacget_current();
	tmDecodeEpochTime ( time , &year , &month , &day ,
			    &hour , &minute , &second ) ;
  
	s->h->nzyear = year ;
	s->h->nzjday = MDtoDoy ( month , day , isLeapYear ( year ) ) ;
	s->h->nzhour = hour ;
	s->h->nzmin  = minute ;
	s->h->nzsec  = (int) second ;
	s->h->nzmsec = ( second - s->h->nzsec ) * 1000 ;
}


