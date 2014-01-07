
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sam.h"
#include "hdr.h"
#include "amf.h"
#include "SacHeader.h"
#include "bool.h"
#include "dfm.h"
#include "ucf.h"


#include "co.h"
#include "msg.h"
#include "dff.h"

void /* FUNCTION */ fdWhitenWrite( float *resp[4] , char * kprefix ,
				   float * userData , int newnpts ,
				   int nFreq , int * nerr )
{
	/* index sacmem for Amplitude, Phase, 
	   and the impulse response. */

	int fileDescriptor = 0,
    idx, jdx, nlcmem, nlcdsk, nptwr;
	char kname[ MCPFN ] , ksuffix[ 3 ][ 6 ] ;
  sac *s;
	float *bufout = NULL , **amph = NULL ;
    
	*nerr = 0;

	/* allocate room for spectral data */
	amph = (float **) calloc ( 2 , sizeof( float *) ) ;
	if ( amph == NULL ) {
	    *nerr = 301 ;
	    goto L_ERROR ;
	}

	amph[ 0 ] = (float *) malloc ( ( nFreq ) * sizeof( float ) ) ;
	amph[ 1 ] = (float *) malloc ( ( nFreq ) * sizeof( float ) ) ;

	if ( amph[ 0 ] == NULL || amph[ 1 ] == NULL ) {
	    *nerr = 301 ;
	    goto L_ERROR ;
	}

	/* handle strings */
	if ( strlen ( kprefix ) > MCPFN - 4 )
	    kprefix[ MCPFN - 4 ] = '\0' ;
	strcpy ( ksuffix[ 0 ] , ".spec" ) ;
	strcpy ( ksuffix[ 1 ] , ".imp" ) ;
	strcpy ( ksuffix[ 2 ] , ".gd" ) ;

  s = sac_new();
	/* fill the amplitude and phase array */
        for ( idx = 0 ; idx < nFreq ; idx++ ) {
          amph[ 0 ][ idx ] = resp[1][idx];
          amph[ 1 ][ idx ] = resp[2][idx];
        }
        
	/* fill some fields. */
	for ( idx = 0 ; idx < 9 ; idx++ )		/* user fields */
    *( (&s->h->user0) + idx ) = userData[ idx ] ;

	switch ( (int) (s->h->user0 + 0.5) ) {
	    case 1:  strcpy ( s->h->kuser0 , "lowpass " ) ;
		     break ;
	    case 2:  strcpy ( s->h->kuser0 , "highpass" ) ;
		     break ;
	    case 3:  strcpy ( s->h->kuser0 , "bandpass" ) ;
		     break ;
	    case 4:  strcpy ( s->h->kuser0 , "bandrej " ) ;
		     break ;
	    case 5:  strcpy ( s->h->kuser0 , "whiten  " ) ;
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
	    case 5:  strcpy ( s->h->kuser1 , "whiten  " ) ;
		     break ;
	    default: strcpy ( s->h->kuser1 , "-12345  " ) ;
		     break ;
	}

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
		case 0:	aphdrw( newnpts , nFreq ) ;
			nlcmem = 1;
			break ;
		case 1:	irhdrw( newnpts , nFreq ) ;
			nlcmem = 0;
			break ;
		case 2: gdhdrw( newnpts , nFreq ) ;
			nlcmem = 3;
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
	    //copy_float( cmhdr.fhdr, cmmem.sacmem[ hdrindex ] , SAC_HEADER_NUMBERS );
	    //zputc ( kmhdr.khdr[ 0 ] , 9 , (int *)(cmmem.sacmem[ hdrindex ] + SAC_HEADER_NUMBERS), 
      //( MCPW + 1 ) * SAC_HEADER_STRINGS ) ;

	    /* move header into output buffer */
	    map_hdr_out ( (float*)&(s->h->delta), bufout, FALSE ) ;

	    /* write the headers */
	    zwabs( (int *)&fileDescriptor, (char *)bufout, nptwr, (int *)&nlcdsk, (int *)nerr );

	    free(bufout);
	    bufout = NULL ;

	    nlcdsk += nptwr;
	    nptwr = nFreq ;

	    /* Write data to disk */

	    switch ( jdx ) {
	    case 0:	
	      zwabs ( (int *)&fileDescriptor, (char *)(amph[ 0 ]) , nptwr, (int *)&nlcdsk, (int *)nerr ) ;
	      /* nlcmem = memptr[ 2 ] ; */
	      nlcdsk += nptwr;
	      zwabs ( (int *)&fileDescriptor, (char *)(amph[ 1 ]) , nptwr, (int *)&nlcdsk, (int *)nerr ) ;
	      free ( amph[ 0 ] ) ;
	      free ( amph[ 1 ] ) ;
	      free ( amph ) ;
	      amph = NULL ;
	      break ;
	      
	    case 1: zwabs( (int *)&fileDescriptor, (char *)resp[nlcmem], nptwr, (int *)&nlcdsk, (int *)nerr );
	      break ;
	    case 2: zwabs( (int *)&fileDescriptor, (char *)resp[nlcmem], nptwr, (int *)&nlcdsk, (int *)nerr ) ;
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
      return;
    }
    //getfil ( 1 , TRUE , &unused1 , &unused2 , &unused3 , nerr ) ;

	if ( amph ) {
	    if ( amph[ 0 ] )
		free ( amph[ 0 ] ) ;
	    if ( amph[ 1 ] )
		free ( amph[ 1 ] ) ;
	    free ( amph ) ;
	}
	if ( bufout ) 
	    free ( bufout ) ;
	if ( fileDescriptor ) 
	    zclose ( &fileDescriptor , nerr ) ;
}


void aphdrw ( int newnpts , int nFreq )
{
  sac *s;
  s = sacget_current();
  s->h->nsnpts = newnpts ;
        s->h->npts = nFreq ;
        s->h->sdelta = s->h->user6 ;
        s->h->delta = 1. / ( s->h->sdelta * (float) ( nFreq ) ) ;
        s->h->iftype = IAMPH ;

        strcpy ( s->h->kevnm , "FD: AMP/PH" ) ;
}


void gdhdrw ( int newnpts , int nFreq )
{
  sac *s;
  s = sacget_current();
        s->h->nsnpts = newnpts ;
        s->h->npts = nFreq / 2 ;
        s->h->sdelta = s->h->user6 ;
        s->h->delta = 1. / ( s->h->sdelta * nFreq ) ;
        s->h->iftype = ITIME ;

        strcpy ( s->h->kevnm , "FD: GROUP DELAY" ) ;
}

void irhdrw ( int newnpts , int nFreq )
{
  sac *s;
  s = sacget_current();
        s->h->nsnpts = nFreq ;
        s->h->npts = newnpts ;
        s->h->delta = s->h->user6 ;
        s->h->sdelta = 1. / ( s->h->delta * (float) ( s->h->npts ) ) ;
        s->h->iftype = ITIME ;

        strcpy ( s->h->kevnm , "FD: IMPULSE" ) ;
}

