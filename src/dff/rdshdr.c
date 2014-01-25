/** 
 * @file   rdshdr.c
 * 
 * @brief  Read a SDD Header
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "dff.h"
#include "sddhdr.h"
#include "co.h"
#include "hdr.h"
#include "amf.h"
#include "dfm.h"
#include "ucf.h"

/** 
 * Read a SDD Header from a currently open file into memory
 * 
 * @param idfl 
 *    Data file list index number
 * @param nun 
 *    Fortran file unit on which data file is open
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   900904:  Created to read SDD headers.
 *
 */
void 
rdshdr(int  idfl, 
       int *nun, 
       int *nerr) {

	int i, idd, imm, is, iss, ndaerr, nlcdsk, nlcmem, numrd, offset;
        char *strtemp;

        int *Isacmem;

	kschan[12]='\0';
	kschdr[80]='\0';
	ksclas[4]='\0';
	kscom[40]='\0';
	ksevnm[8]='\0';
	ksfrmt[8]='\0';
	ksstnm[8]='\0';

	*nerr = 0;

	/* - Read header into SDD header common. */
	numrd = MWSHDR;
	nlcdsk = 0;
	zrabs( (int *)nun, (char *)(&Ishdr[1]), numrd, (int *)&nlcdsk, (int *)nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* - Initialize SAC header */
	newhdr();

	/* - Convert header from SDD format to SAC format */
	fstrncpy( kevnm, 17, ksevnm, strlen(ksevnm));
    ksstnm[8] = 0;
	strcpy( kstnm, ksstnm );

        strtemp = malloc(9);
        strncpy(strtemp,kschan,8);
        strtemp[8] = '\0';

	subscpy( kcmpnm, 0, 7, 8, strtemp );

	strcpy( kinst, "        " );

        strncpy(strtemp,kschan+8,4);
        strtemp[4] = '\0';

	subscpy( kinst, 0, 3, 8, strtemp );

        free(strtemp);

	*delta = (1.0/(float)( *isdelt ))*100.0;
	*npts = *isnpts;
	*stel = (float)( *issel )/100.0;
	*stdp = (float)( *issdep )/100.0;

	/* - Convert lat/lon from dddmmsshh to ddd.frac */

	iss = *issla%10000;
	is = *issla/10000;
	imm = is%100;
	is = is/100;
	*stla = (float)( is ) + (float)( imm )/60.0 + (float)( iss )/360000.0;

	iss = *isslo%10000;
	is = *isslo/10000;
	imm = is%100;
	is = is/100;
	*stlo = (float)( is ) + (float)( imm )/60.0 + (float)( iss )/360000.0;

	/* - Convert time from yyyymmdd and hhmmssttt to separate fields */

	*nzyear = *isdate/10000;
	imm = *isdate/100 - *nzyear*100;
	idd = *isdate%100;
	kijdat( *nzyear, imm, idd, nzjday, nerr );

	*nzhour = *istime/10000000;
	*nzmin = *istime/100000 - *nzhour*100;
	*nzsec = *istime/1000 - (*nzmin + *nzhour*100)*100;
	*nzmsec = *istime%1000;

	/* - Allocate memory for leftover stuff from the SDD header
	 *   that doesn't go anywhere else */

	numrd = MWESHD;
	allamb( &cmmem, numrd, &Nxsdd[idfl], nerr );
	if( *nerr != 0 )
		goto L_8888;
	nlcmem = Nxsdd[idfl];

        Isacmem = (int *)cmmem.sacmem[nlcmem];

	*Isacmem = *isclas;
	*(Isacmem + 1) = *isfrmt;
	*(Isacmem + 2) = *iscalg;
        offset = 3;


        Isacmem += offset;

	for( i = 1; i <= MSCOM; i++ ){
	   *(Isacmem++) = Iscom[i];
           offset++;
		}

        
	for( i = 1; i <= MSREP; i++ ){
	   *(Isacmem++) = Isrep[i];
           offset++;
		}

	/* - Compute distance, azimuth, etc. if proper header fields are present. */

	if( (((*stla != cmhdr.fundef && *stlo != cmhdr.fundef) && *evla != 
	 cmhdr.fundef) && *evlo != cmhdr.fundef) && *lcalda ){
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


	if( *nzyear >= 0 && *nzyear <= 99 )
		*nzyear = *nzyear + 1900;

	/* - Compute end time if evenly-spaced file. */

	*begin = 0.0;
	if( *leven )
		*ennd = *begin + *delta*(float)( *npts - 1 );

	/* - Copy header back to its location in working memory. */

	putfil( idfl, nerr );
	if( *nerr != 0 )
		goto L_8888;

L_8888:
	return;

}

