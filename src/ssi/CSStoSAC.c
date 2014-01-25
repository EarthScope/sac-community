
#include <stdio.h>
#include <string.h>

#include "ssi.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"


#include "msg.h"
#include "clf.h"
#include "bot.h"
#include "dff.h"

void CSStoSAC ( idfl , header , seis , lname , lcutnow , nerr )
int idfl , * nerr ;
struct trace *seis ;
struct SACheader *header ;
int lname , lcutnow ;
{
    /* Declare Variables. */
    int jcomp ;
    char kfile[ MCPFN+1 ] ;

    /*=====================================================================
     * PURPOSE:  Called by xreaddb() to get CSS formated data from SeisMgr
     *           into SAC.  
     *=====================================================================
     * INPUT ARGUMENTS:
     *    idfl:      file number of first file being handled currently
     *    header:    SeisMgr struct containing a SAC formated header
     *    seis:      seismogram
     *=====================================================================
     * OUTPUT ARGUMENTS:
     *    nerr:    Error flag. Set to 0 if no error occurred.
     *=====================================================================
     * MODIFICATION HISTORY:
     *    971202:  Original version.  maf  much code plagerized form readfl
     *                                     and associated functions.
     *===================================================================== */

    *nerr = 0 ;

    /* -- Allocate block for header. */
    allamb( &cmmem, SAC_HEADER_WORDS , &Ndxhdr[idfl], nerr );
    if( *nerr != 0 )
	goto L_8888;

    Ndsndx[idfl] = 1 ;

    /* -- Get header. */
    DBheaderToSac ( header , TRUE ) ;

    /* Give the file a name internally */
    if ( lname ) {
	char tempSta[9], tempChan[9] ;

	if ( !strcmp ( header->kstnm , SAC_CHAR_UNDEFINED ) )
	    sprintf ( tempSta , "s%03d" , idfl ) ;
	else
	    strcpy ( tempSta , header->kstnm ) ;

	if ( !strcmp ( header->kcmpnm , SAC_CHAR_UNDEFINED ) )
	    sprintf ( tempChan , "c%03d" , idfl ) ;
	else
	    strcpy ( tempChan , header->kcmpnm ) ;

	terminate ( tempSta ) ;
	terminate ( tempChan ) ;

	sprintf ( kfile , "%s.%s.%04d%03d%02d%02d%02d" , tempSta , tempChan ,
		  *nzyear , *nzjday , *nzhour , *nzmin , *nzsec ) ;

    string_list_put(datafiles, kfile, strlen(kfile) +1);
	if ( *nerr )
	    goto L_8888 ;
    }

    /* -- Prepare to get waveform */
    defmem( idfl, lcutnow, nerr );
    if ( *nerr )
	goto L_8888 ;

    /* Allocate space for waveform */
    for ( jcomp = 0 ; jcomp < Ncomp[ idfl ] ; jcomp++ ) {
	allamb( &cmmem, Nlndta[idfl], &cmdfm.ndxdta[idfl - 1][jcomp], nerr );
	if( *nerr != 0 )
	    goto L_8888 ;
    } /* end for ( jcomp ) */

    /* Get waveform */
    DBwfToSac ( idfl , seis , nerr ) ;
    if ( *nerr )
	goto L_8888 ;

    /* Put the new file away. */
    putfil ( idfl , nerr ) ;

L_8888:
    if ( *nerr ) {
	setmsg ( "ERROR" , *nerr ) ;
	outmsg () ;
	clrmsg () ;
    }

    return ;
} /* end CSStoSAC */
