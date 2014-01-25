
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

void /* FUNCTION */ fdWriteFiles ( int * memptr , char * kprefix ,
				   float * userData , int newnpts ,
				   int * nerr )
{

	/* index sacmem for amplitude, phase, group delay,
	   and the impulse response. */
	int fileDescriptor = 0 , hdrindex , xbegin = 0 ,
		 idx, jdx, nlcmem, nlcdsk, nptwr, 
		 unused1 , unused2 , unused3 ;

	char kname[ MCPFN ] , ksuffix[ 3 ][ 6 ] ;

	float *bufout = NULL , *ptr , amph[ 2 ][ 2 * NDATPTS - 2 ] ;	
	void zwabs() ;

	*nerr = 0;

	/* handle strings */
	if ( strlen ( kprefix ) > MCPFN - 4 )
	    kprefix[ MCPFN - 4 ] = '\0' ;
	strcpy ( ksuffix[ 0 ] , ".spec" ) ;
	strcpy ( ksuffix[ 1 ] , ".gd" ) ;
	strcpy ( ksuffix[ 2 ] , ".imp" ) ;

	/* Determine the begin of the impulse */
	for ( ptr = cmmem.sacmem[memptr[ 9 ]] ; *ptr == 0.0 ; ptr++ )
	    xbegin++ ;

	/* fill the amplitude and phase array */
        for ( idx = 0 ; idx < NDATPTS ; idx++ ) {
            amph[ 0 ][ idx ] = cmmem.sacmem[ memptr[ 6 ] ][ idx ] ;
            amph[ 1 ][ idx ] = cmmem.sacmem[ memptr[ 7 ] ][ idx ] ;
        }

        for (  ; idx < 2 * NDATPTS - 2 ; idx++ ) {
            amph[ 0 ][ idx ] =  cmmem.sacmem[ memptr[ 6 ] ][ 2*NDATPTS-idx-2 ] ;
            amph[ 1 ][ idx ] = -cmmem.sacmem[ memptr[ 7 ] ][ 2*NDATPTS-idx-2 ] ;
        }

	/* Allocate block for headers. */
	allamb ( &cmmem, SAC_HEADER_WORDS, &hdrindex , nerr ) ;
	if ( *nerr != 0 )
	    goto L_ERROR ;

	/* null the header */
	for ( idx = 0 ; idx < SAC_HEADER_FLOATS ; idx++ )
	    cmhdr.fhdr[ idx ] = SAC_FLOAT_UNDEFINED ;
	for ( idx = 0 ; idx < SAC_HEADER_INTEGERS ; idx++ )
	    cmhdr.nhdr[ idx ] = SAC_INT_UNDEFINED ;
	for ( idx = 0 ; idx < SAC_HEADER_ENUMS ; idx++ )
	    cmhdr.ihdr[ idx ] = SAC_INT_UNDEFINED ;
	for ( idx = 0 ; idx < SAC_HEADER_STRINGS ; idx++ )
	    strcpy ( kmhdr.khdr[ idx ] , SAC_CHAR_UNDEFINED ) ;

	/* fill some fields. */
	for ( idx = 0 ; idx < 9 ; idx++ )		/* user fields */
	    *( user0 + idx ) = userData[ idx ] ;

	switch ( (int) (*user0 + 0.5) ) {
	    case 1:  strcpy ( kuser0 , "lowpass " ) ;
		     break ;
	    case 2:  strcpy ( kuser0 , "highpass" ) ;
		     break ;
	    case 3:  strcpy ( kuser0 , "bandpass" ) ;
		     break ;
	    case 4:  strcpy ( kuser0 , "bandrej " ) ;
		     break ;
	    default: strcpy ( kuser0 , "-12345  " ) ;
		     break ;
	}

	switch ( (int) (*user1 + 0.5) ) {
	    case 1:  strcpy ( kuser1 , "Butter  " ) ;
		     break ;
	    case 2:  strcpy ( kuser1 , "Bessel  " ) ;
		     break ;
	    case 3:  strcpy ( kuser1 , "C1      " ) ;
		     break ;
	    case 4:  strcpy ( kuser1 , "C2      " ) ;
		     break ;
	    default: strcpy ( kuser1 , "-12345  " ) ;
		     break ;
	}

	fillNZ () ;					/* time fields */

	*begin  = 0.0 ;					/* other fields */
	*sb     = 0.0 ;
	*nvhdr  = 6 ;
	*idep   = IUNKN ;
	*iztype = IB ;
	*leven  = TRUE ;
	*lpspol = TRUE ;
	*lovrok = TRUE ;
	*lcalda = FALSE ;

	for ( jdx = 0 ; jdx < 3 ; jdx++ ) {	/* loop between output files. */
 
	    /* fill other header fields specific to the data */
	    switch ( jdx ) {
		case 0:	aphdr( newnpts ) ;
			nlcmem = memptr[ 6 ] ;
			break ;
		case 1:	gdhdr( newnpts ) ;
			nlcmem = memptr[ 8 ] ;
			break ;
		case 2:	irhdr( newnpts ) ;
			nlcmem = memptr[ 9 ] ;
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
	    copy_float( cmhdr.fhdr, cmmem.sacmem[ hdrindex ], SAC_HEADER_NUMBERS );
	    zputc ( kmhdr.khdr[ 0 ] , 9 , (int *)(cmmem.sacmem[ hdrindex ] + SAC_HEADER_NUMBERS), 
		    ( MCPW + 1 ) * SAC_HEADER_STRINGS) ;

	    /* move header into output buffer */
	    map_hdr_out ( cmmem.sacmem[ hdrindex ] , bufout , FALSE) ;

	    /* write the headers */
	    zwabs( (int *)&fileDescriptor, (char *)(bufout), nptwr, (int *)&nlcdsk, (int *)nerr );

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
		  
	    case 1: zwabs( (int *)&fileDescriptor, (char *)(cmmem.sacmem[nlcmem]), nptwr, (int *)&nlcdsk, (int *)nerr );
	      break ;
	      
	    case 2: zwabs( (int *)&fileDescriptor, (char *)(cmmem.sacmem[nlcmem] + xbegin), nptwr, (int *)&nlcdsk, (int *)nerr );
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

	if ( cmdfm.ndfl > 0 )
	    getfil ( 1 , TRUE , &unused1 , &unused2 , &unused3 , nerr ) ;

	if ( bufout ) 
	    free ( bufout ) ;
	if ( fileDescriptor ) 
	    zclose ( &fileDescriptor , nerr ) ;
	relamb ( cmmem.sacmem , hdrindex , nerr );
}


void aphdr ( int newnpts )
{
	*nsnpts = newnpts ;
	*npts = 2*NDATPTS - 2 ;
	*sdelta = *user6 ;
	*delta = 1. / ( *sdelta * (float) ( *npts ) ) ;
	*iftype = IAMPH ;

	strcpy ( kevnm , "FD: AMP/PH" ) ;
}


void gdhdr ( int newnpts )
{
        *nsnpts = newnpts ;
        *npts = NDATPTS ;
        *sdelta = *user6 ;
        *delta = 1. / ( *sdelta * (float) ( *npts ) ) ;
        *iftype = ITIME ;

        strcpy ( kevnm , "FD: GROUP DELAY" ) ;
}

void irhdr ( int newnpts )
{
        *nsnpts = NDATPTS ;
        *npts = newnpts ;
        *delta = *user6 ;
        *sdelta = 1. / ( *delta * (float) ( *npts ) ) ;
        *iftype = ITIME ;

        strcpy ( kevnm , "FD: IMPULSE" ) ;
}



int MDtoDoy () ;
int isLeapYear () ;


void fillNZ ()
{
	double time = tmGetEpochTime () ;
	int year , month , day , hour , minute ;
	float second ;

	tmDecodeEpochTime ( time , &year , &month , &day ,
			    &hour , &minute , &second ) ;

	*nzyear = year ;
	*nzjday = MDtoDoy ( month , day , isLeapYear ( year ) ) ;
	*nzhour = hour ;
	*nzmin  = minute ;
	*nzsec  = (int) second ;
	*nzmsec = ( second - *nzsec ) * 1000 ;
}


