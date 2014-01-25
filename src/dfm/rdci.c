/** 
 * @file   rdci.c
 * 
 * @brief  Read a SAC Card Image data file
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

#include "dfm.h"
#include "hdr.h"
#include "bool.h"
#include "amf.h"
#include "co.h"

#include "errors.h"

#include "string_utils.h"


#include "msg.h"
#include "clf.h"
#include "dff.h"

/** 
 * Read a SAC Card Image data file into memory
 * 
 * @param idfl 
 *    Data file list index number
 * @param kname 
 *    Name of the file to read
 * @param kname_s 
 *    Length of \p kname
 * @param nlen 
 *    Number of data points read
 * @param ndx1 
 *    Index in sacmem array of first data component
 * @param ndx2 
 *    Index of sacmem array of second data component
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_READING_CARD_IMAGE_HEADER
 *
 * @date   130105:  kevnm bug fix from Kasahara
 * @date   850730:  Changes due to new memory manager.
 *                  CHANGED NUMBER AND ORDER OF ARGUMENTS.
 * @date   830607:  Fixed bug causing extra data point to be read.
 * @date   810728:  Added argument specifying data format..
 * @date   810120:  Changed to output message retrieval from disk.
 * @date   800109:  Original version.
 *
 */
void 
rdci(int   idfl, 
     char *kname, 
     int   kname_s, 
     int  *nlen, 
     int  *ndx1, 
     int  *ndx2, 
     int  *nerr) {

	int ilhdr[SAC_HEADER_LOGICALS], jdx, jj, jjj, ncards, nlcmem, nremdr;
        FILE *nun;
        char kiline[MCMSG+1];
        char *kiptr;

        float *Sacmem;

	int *const Ilhdr = &ilhdr[0] - 1;

	*nerr = 0;

	/* - Open file. */
	zopens( &nun, kname,kname_s, "ROTEXT",7, nerr );
	if( *nerr != 0 )
	    goto L_8888;

	/* - Read header. */
	jdx = 1;
	for( jj = 1; jj <= (SAC_HEADER_FLOATS/5); jj++ ){
	    if(fgetsp(kiline,MCMSG,nun)==NULL){
		*nerr = ERROR_READING_CARD_IMAGE_HEADER;
		goto L_8888;
	    }
	    for( jjj = jdx; jjj <= (jdx + 4); jjj++ ){
		kiptr = strtok ( jjj == jdx ? kiline : NULL , " " ) ;
		if ( !kiptr )
		    break ;
		Fhdr[jjj] = atof(kiptr);
	    }
	    jdx = jdx + 5;
	}

	jdx = 1;

	for( jj = 1; jj <= (SAC_HEADER_INTEGERS/5); jj++ ){
	    if(fgetsp(kiline,MCMSG,nun)==NULL){
		*nerr = ERROR_READING_CARD_IMAGE_HEADER;
		goto L_8888;
	    }
	    for( jjj = jdx; jjj <= (jdx + 4); jjj++ ){
		kiptr = strtok ( jjj == jdx ? kiline : NULL , " " ) ;
		if ( !kiptr )
		    break ;
		Nhdr[jjj] = atol(kiptr);
	    }
	    jdx = jdx + 5;
	}

	jdx = 1;
	for( jj = 1; jj <= (SAC_HEADER_ENUMS/5); jj++ ){
	    if(fgetsp(kiline,MCMSG,nun)==NULL){
		*nerr = ERROR_READING_CARD_IMAGE_HEADER;
		goto L_8888;
	    }
	    for( jjj = jdx; jjj <= (jdx + 4); jjj++ ){
		kiptr = strtok ( jjj == jdx ? kiline : NULL , " " ) ;
		if ( !kiptr )
		    break ;
		Ihdr[jjj] = atol(kiptr);
	    }
	    jdx = jdx + 5;
	}

	jdx = 1;
	for( jj = 1; jj <= (SAC_HEADER_LOGICALS/5); jj++ ){
	    if(fgetsp(kiline,MCMSG,nun)==NULL){
		*nerr = ERROR_READING_CARD_IMAGE_HEADER;
		goto L_8888;
	    }

	    for( jjj = 1; jjj <= 5; jjj++ ){
		kiptr = strtok ( jjj == jdx ? kiline : NULL , " " ) ;
		if ( !kiptr )
		    break ;
		Ilhdr[jjj] = atol(kiptr);
	    }

	    for( jjj = 1; jjj <= 5; jjj++ ){
		if( Ilhdr[jjj] == 1 ){
		    Lhdr[jdx] = TRUE;
		}
		else{
		    Lhdr[jdx] = FALSE;
		}
		jdx = jdx + 1;
	    }
	}

	/* read the character header values */
        if(fgetsp(kiline,MCMSG,nun)==NULL){
	    *nerr = ERROR_READING_CARD_IMAGE_HEADER;
	    goto L_8888;
        }

        strncpy(kmhdr.khdr[0],kiline,8);
        kmhdr.khdr[0][8] = '\0';
        strncpy(kmhdr.khdr[1],kiline+8,16);
        kmhdr.khdr[2][7] = '\0';

	for( jj = 4; jj <= SAC_HEADER_STRINGS; jj += 3 ){
	    if(fgetsp(kiline,MCMSG,nun)==NULL){
		*nerr = ERROR_READING_CARD_IMAGE_HEADER;
		goto L_8888;
	    }
	    for( jjj = jj; jjj <= (jj + 2); jjj++ ){
		strncpy(kmhdr.khdr[jjj-1],kiline+((jjj-jj)*8),8);
		kmhdr.khdr[jjj-1][8] = '\0';
	    }
	}

	/* - Update the header if necessary. */

	if( *nvhdr < cmhdr.nvhdrc ){
	    updhdr( nerr );
	    if( *nerr != 0 )
		goto L_8888;
	}

	/* - Make sure the most important header values are defined. */

	if( *npts == cmhdr.nundef || *begin == cmhdr.fundef ){
	    *nerr = ERROR_READING_CARD_IMAGE_HEADER;
	    setmsg( "ERROR", *nerr );
	    goto L_8888;
	}

	/* - Move header to SACMEM array. */

	allamb( &cmmem, SAC_HEADER_WORDS, &Ndxhdr[idfl], nerr );
	if( *nerr != 0 )
	    goto L_8888;
	putfil( idfl, nerr );

	/* - Set up data space. */

    string_list_put(datafiles, kname, kname_s);
	if( *nerr != 0 )
	    goto L_8888;
	Nlndta[idfl] = *npts;
	*nlen = *npts;
	allamb( &cmmem, *npts, &cmdfm.ndxdta[idfl - 1][0], nerr );
	if( *nerr != 0 )
	    goto L_8888;


	if( (*iftype == *itime || *iftype == *ixy) || *iftype == *iunkn ){
	    if( *leven ){
		Ncomp[idfl] = 1;
	    }
	    else{
		Ncomp[idfl] = 2;
	    }
	}
	else if( *iftype == *ixyz ){
	    Ncomp[idfl] = 1;
	}
	else{
	    Ncomp[idfl] = 2;
	}

	if ( Ncomp[ idfl ] == 2 ) {
	    allamb( &cmmem, *npts, &cmdfm.ndxdta[idfl - 1][1], nerr );
	    if( *nerr != 0 )
		goto L_8888;
	}

	/* - Read first data component. */

	ncards = *npts/5;
	nremdr = *npts - 5*ncards;
	nlcmem = cmdfm.ndxdta[idfl - 1][0];
	*ndx1 = nlcmem;
	for( jj = 1; jj <= ncards; jj++ ){
	    if(fgetsp(kiline,MCMSG,nun)==NULL){
		*nerr = ERROR_READING_CARD_IMAGE_HEADER;
		goto L_8888;
	    }
	    Sacmem = cmmem.sacmem[*ndx1]+(nlcmem-*ndx1);
	    for( jjj = nlcmem; jjj <= (nlcmem + 4); jjj++ ){
		kiptr = strtok ( jjj == nlcmem ? kiline : NULL , " " ) ;
		if ( !kiptr )
		    break ;
		*Sacmem++ = atof(kiptr);
	    }
	    nlcmem = nlcmem + 5;
	}
	if( nremdr > 0 ){
	    if(fgetsp(kiline,MCMSG,nun)==NULL){
		*nerr = ERROR_READING_CARD_IMAGE_HEADER;
		goto L_8888;
	    }
	    Sacmem = cmmem.sacmem[*ndx1]+(nlcmem-*ndx1);
	    for( jjj = nlcmem; jjj <= (nlcmem + nremdr - 1); jjj++ ){
		kiptr = strtok ( jjj == nlcmem ? kiline : NULL , " " ) ;
		if ( !kiptr )
		    break ;
		*Sacmem++ = atof(kiptr);
	    }
	}

	/* - Read second data component if present. */

	if( Ncomp[idfl] == 2 ){
	    nlcmem = cmdfm.ndxdta[idfl - 1][1];
	    *ndx2 = nlcmem;
	    for( jj = 1; jj <= ncards; jj++ ){
		if(fgetsp(kiline,MCMSG,nun)==NULL){
		    *nerr = ERROR_READING_CARD_IMAGE_HEADER;
		    goto L_8888;
		}
		Sacmem = cmmem.sacmem[*ndx2]+(nlcmem-*ndx2);
		for( jjj = nlcmem; jjj <= (nlcmem + 4); jjj++ ){
		    kiptr = strtok ( jjj == nlcmem ? kiline : NULL , " " ) ;
		    if ( !kiptr )
			break ;
		    *Sacmem++ = atof(kiptr);
		}
		nlcmem = nlcmem + 5;
	    }
	    if( nremdr > 0 ){
		if(fgetsp(kiline,MCMSG,nun)==NULL){
		    *nerr = ERROR_READING_CARD_IMAGE_HEADER;
		    goto L_8888;
		}
		Sacmem = cmmem.sacmem[*ndx2]+(nlcmem-*ndx2);
		for( jjj = nlcmem; jjj <= (nlcmem + nremdr - 1); jjj++ ){
		    kiptr = strtok ( jjj == nlcmem ? kiline : NULL , " " ) ;
		    if ( !kiptr )
			break ;
		    *Sacmem++ = atof(kiptr);
		}
	    }
	}
	else{
	    *ndx2 = 0;
	}

	/* - Close file and return. */
	zcloses( &nun, nerr );

L_8888:
	return;
}

