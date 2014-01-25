/** 
 * @file   rdhdr.c
 * 
 * @brief  Read header from SAC file
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "dff.h"
#include "hdr.h"
#include "msg.h"
#include "co.h"
#include "ucf.h"
#include "dfm.h"
#include "amf.h"

#include "errors.h"


#include "clf.h"

/** 
 * Read header from currently open SAC file into memory
 * 
 * @param idfl
 *    Data file list index number 
 * @param nun 
 *    Fortran file unit on which data file is open
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_NOT_A_SAC_FILE 
 *    - ERROR_HEADER_OUT_OF_DATE
 *    - ERROR_READING_FILE
 * 
 * @return 
 *
 * @date   870803:  Header update logic changed due to opening files 'RODATA'.
 * @date   860130:  Changed to new message handling package.
 * @date   850415:  Changes due to restructuring of DFM common block.
 * @date   821122:  Added recomputation of ENND every time file is read.
 * @date   821001:  Added check of LCALDA before calculating distance/azimuth.
 * @date   820909:  Mod due to placing KHDR in its own common block.
 * @date   820901:  Fixed bug that was causing NZYEAR to be clobbered.
 * @date   811026:  Documented subroutine.
 * @date   810924:  Included current century in NZYEAR if it was lt 100.
 * @date   810120:  Changed to output message retrieval from disk.
 *
 */
int 
rdhdr(int  idfl, 
      int  *nun, 
      char *file, 
      int  *nerr) {

	int ncerr, ndaerr, nlcdsk, nlcmem, numrd;
        float *buffer;
        int *hdrVer, lswap = 0 ;
        const int versionLocation = 76 ;

	*nerr = 0;

	/* - Read header into memory. */
	nlcmem = Ndxhdr[idfl];
        numrd = SAC_HEADER_WORDS_FILE;

	nlcdsk = 0;

        if((buffer=(float *)malloc(SAC_FIRST_DATA_POINT_WORD * 4)) == NULL){
            *nerr = ERROR_READING_FILE;
            error(*nerr, "error allocating input buffer");
            goto L_8888;
	}

        /* read raw header data into buffer */
	zrabs((int *) nun, (char *)buffer, numrd, (int *)&nlcdsk, (int *)nerr );

        /* determine if the data needs to be swapped. */
        hdrVer = (int *)( buffer + versionLocation ) ;
        if( *hdrVer < 1 || *hdrVer > cmhdr.nvhdrc ){
            byteswap( (void *)hdrVer, 4 ) ;
            if( *hdrVer < 1 || *hdrVer > cmhdr.nvhdrc ){
                *nerr = ERROR_NOT_A_SAC_FILE ;
                error(*nerr, "%s not in sac format, nor byteswapped sac format", 
                      file);
                goto L_8888 ;
            }
            else{
                byteswap( (void *)hdrVer, 4 ) ; /* swap back, so it can be */
                lswap = 1 ;                     /* swapped again with the rest. */
            }
        }

        /* move raw data into header location, byteswapping numeric headers
           if appropriate. */
        map_hdr_in(cmmem.sacmem[nlcmem],buffer, lswap );

        free(buffer);

	if( *nerr != 0 )
	    goto L_8888;

	/* - Copy header from working memory into header common. */

	/* copy( (int*)cmmem.sacmem[nlcmem], (int*)&Fhdr[1], SAC_HEADER_NUMBERS ); */
	copy_float( cmmem.sacmem[nlcmem], &(Fhdr[1]), SAC_HEADER_NUMBERS );
	zgetc( (int *)cmmem.sacmem[nlcmem] + SAC_HEADER_NUMBERS, kmhdr.khdr[0], (MCPW+1)* SAC_HEADER_STRINGS );

	/* - Update the header if it is in an old format:
	 *   (1) Close file and open it for writing.
	 *   (2) Write updated header.
	 *   (3) Close file again and open it for reading only.
	 *   (4) Send a warning message to inform user of all of this. */

	if( *nvhdr > 0 && *nvhdr < cmhdr.nvhdrc ){
	    updhdr( nerr );
	    if( *nerr == 0 )
		putfil( idfl, nerr );
	    if( *nerr != 0 ){
            *nerr = ERROR_HEADER_OUT_OF_DATE;
            error(*nerr, "%s File is Bad. Header could not be updated", file);
            goto L_8888;
	    }
        warning(ERROR_HEADER_OUT_OF_DATE, "%s", file);
	    zclose( nun, &ncerr );

        zopen_sac( nun, file, strlen(file)+1, "DATA",5, nerr );

	    if( *nerr != 0 ){
            *nerr = 0;
            aplmsg( "Insufficient access rights to update disk file." ,48 );
            aplmsg( "Header in memory has been updated.",35 );
            goto L_4000;
	    }
	    nlcdsk = 0;

        if((buffer=(float *)malloc(SAC_FIRST_DATA_POINT_WORD * 4)) == NULL){
            *nerr = 115;
            error(*nerr, "allocating output buffer");
            goto L_8888;
        }
        
        map_hdr_out(cmmem.sacmem[nlcmem],buffer, lswap);
        
	    zwabs((int *) nun, (char *)buffer, SAC_FIRST_DATA_POINT_WORD, 
              (int *)&nlcdsk, (int *)nerr );

        free(buffer);

	    if( *nerr != 0 ){
            *nerr = 0;
            aplmsg( "Could not update header in disk file.",38 );
            aplmsg( "Header in memory has been updated.",35 );
            zclose( nun, &ncerr );
            goto L_4000;
	    }
	    zclose( nun, &ncerr );
	    aplmsg( "Headers in memory and on disk have been updated." ,49 );
L_4000:
	    outmsg();
	    clrmsg();

        zopen_sac( nun, file, strlen(file)+1, "RODATA",7, &ncerr );

	} /* end if( *nvhdr > 0 && *nvhdr < cmhdr.nvhdrc ) */
	else if( *nvhdr <= 0 || *nvhdr > cmhdr.nvhdrc ){
	    *nerr = ERROR_HEADER_OUT_OF_DATE;
        error(*nerr, "%s Header version number is incorrect", file);
	    goto L_8888;
	} 

	/* - Compute distance, azimuth, etc. if proper header fields are present. */

	if( (((*stla != cmhdr.fundef && *stlo != cmhdr.fundef) && *evla != 
	 cmhdr.fundef) && *evlo != cmhdr.fundef) && *lcalda ){
            if ((fabs(*stla - *evla) < RNDOFF ) && (fabs(*stlo - *evlo) < RNDOFF)) {
		*dist = cmhdr.fundef;
		*az = cmhdr.fundef;
		*baz = cmhdr.fundef;
		*gcarc = cmhdr.fundef;
	    }else {
		*az = 0.;
		*baz = 0.;
		*gcarc = 0.;
		*dist = 0.;
		distaz( *evla, *evlo, (float*)stla, (float*)stlo, 1, (float*)dist, 
		 (float*)az, (float*)baz, (float*)gcarc, &ndaerr );
		if( ndaerr != 0 ){
		    *dist = cmhdr.fundef;
		    *az = cmhdr.fundef;
		    *baz = cmhdr.fundef;
		    *gcarc = cmhdr.fundef;
		}
	    }
	} /* end if( (((*stla != cmhdr.fundef ... ) */

	/* - Adjust reference year if necessary. */

	if( *nzyear >= 0 && *nzyear <= 99 )
	    *nzyear = *nzyear + 1900;

	/* - Compute end time if evenly-spaced file. */

	if( *leven )
	    *ennd = *begin + *delta*(float)( *npts - 1 );

	/* - Copy header back to its location in working memory. */

	putfil( idfl, nerr );
	if( *nerr != 0 )
	    goto L_8888;

L_8888:
	return lswap ;
}

