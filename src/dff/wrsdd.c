/** 
 * @file   wrsdd.c
 * 
 * @brief  Write a SDD File
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "dff.h"
#include "sddhdr.h"
#include "hdr.h"
#include "co.h"
#include "dfm.h"
#include "amf.h"

#include "errors.h"


#include "ucf.h"

/** 
 * Write a SDD file from memory to file
 * 
 * @param idfl 
 *    Data file list index number
 * @param kname 
 *    Name of the file to write
 * @param kname_s 
 *    Length of \p kname
 * @param ldta 
 *    - TRUE to write the header and data
 *    - FALSE to write only the header, not data
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   910826:  Changed nint(x) to int(x + .5) to improve portability to
 *             DEC 5000 workstation per Gyu-sang Jang @ UC Davis.
 * @date   870730:  Added logic to check file permissions before writing.
 * @date   850731:  Changes due to new memory manager.
 * @date   840118:  Deleted call to ZTRUNC.
 * @date   800510:  Original version.
 *
 */
void 
wrsdd(int   idfl, 
      char *kname, 
      int   kname_s, 
      int   ldta, 
      int  *nerr) {

        int idx, idd, ideg, ifrac, ihh, ijday, imm, imsec;
        int  iss, itm, jcomp, kundef_len, ncerr, nlcdsk, nlcmem, nptwr;
        int nun;
	float frac;
        char *strtemp;
        int *Isacmem;
        float *Sacmem;

	kschan[12]='\0';
	kschdr[80]='\0';
	ksclas[4]='\0';
	kscom[40]='\0';
	ksevnm[8]='\0';
	ksfrmt[8]='\0';
	ksstnm[8]='\0';

	*nerr = 0;

	/* - If header and data is to be written, a new file is created.
	 *   If header only is to be written, the old file is opened. */
	if( ldta ){
		znfile( &nun, kname,kname_s, "DATA",5, nerr );
		if( *nerr != 0 )
			goto L_8888;
	}
	else{
		zopen_sac( &nun, kname,kname_s, "DATA",5, nerr );
		if( *nerr != 0 )
			goto L_8888;
	}

	/* - Copy header from working memory into header common. */
	nlcmem = Ndxhdr[idfl];
	/* copy( (int*)cmmem.sacmem[nlcmem], (int*)&Fhdr[1], SAC_HEADER_NUMBERS ); */
	copy_float( cmmem.sacmem[nlcmem], &(Fhdr[1]), SAC_HEADER_NUMBERS );
	zgetc( (int *)cmmem.sacmem[nlcmem] + SAC_HEADER_NUMBERS, kmhdr.khdr[0], (MCPW+1)* SAC_HEADER_STRINGS );

	/* - Initialize fields in SDD header */
	fstrncpy( kschdr, 80, " ", 1);

	for( idx = 21; idx <= MWSHDR; idx++ ){
		Ishdr[idx] = 0;
	}

	/* - Convert header from SAC format to SDD format */
        kundef_len = strlen(kmhdr.kundef);

	if( memcmp(kevnm,kmhdr.kundef,kundef_len) != 0 )
		strscpy( ksevnm, kevnm, 8 );
	if( memcmp(kstnm,kmhdr.kundef,kundef_len) != 0 )
		strcpy( ksstnm, kstnm );
	if( memcmp(kcmpnm,kmhdr.kundef,kundef_len) != 0 ) {
                strtemp = malloc(9);
                strncpy(strtemp,kcmpnm,8);
                strtemp[8] = '\0';
		subscpy( kschan, 0, 7, 12, strtemp );
                free(strtemp);
	}
	if( memcmp(kinst,kmhdr.kundef,kundef_len) != 0 ) {
                strtemp = malloc(5);
                strncpy(strtemp,kinst,4);      
                strtemp[4] = '\0';
		subscpy( kschan, 8, 11, 12, strtemp );
                free(strtemp);
	}
	if( *delta != cmhdr.fundef )
		*isdelt = (int)( (1.0/ *delta)*100.0 + .5 );
	*isnpts = *npts;
	if( *stel != cmhdr.fundef )
		*issel = (int)( *stel*100.0 + .5 );
	if( *stdp != cmhdr.fundef )
		*issdep = (int)( *stdp*100.0 + .5 );

	/* - Pack date and time into one word */

	ijday = *nzjday;
	itm = *nzmsec + (((*nzhour*60 + *nzmin)*60) + *nzsec)*1000;
	if( *begin != cmhdr.fundef && *begin != 0.0 ){
		itm = itm + (int)( *begin*1000.0 + .5 );
		if( itm < 0 ){
			ijday = ijday - 1;
			itm = itm + 8640000;
			if( ijday < 0 ){
				*nerr = 1377;
				goto L_8888;
			}
		}
		imsec = itm%1000;
		itm = itm/1000;
		iss = itm%60;
		itm = itm/60;
		imm = itm%60;
		ihh = itm/60;
		*istime = imsec + ((ihh*100 + imm)*100 + iss)*1000;
	}
	else{
		*istime = *nzmsec + ((*nzhour*100 + *nzmin)*100 + *nzsec)*
		 1000;
	}
	kidate( *nzyear, ijday, &imm, &idd, nerr );
	*isdate = idd + (*nzyear*100 + imm)*100;

	/* - Convert lat/lon back from fraction to minutes/seconds */
	if( *stla != cmhdr.fundef ){
		ideg = *stla;
		frac = *stla - (float)( ideg );
		imm = frac*60.0;
		frac = frac*60.0 - (float)( imm );
		iss = frac*60.0;
		frac = frac*60.0 - (float)( iss );
		ifrac = (int)( frac*100.0 + .5 );
		*issla = ifrac + ((ideg*100 + imm)*100 + iss)*100;
	}

	if( *stlo != cmhdr.fundef ){
		ideg = *stlo;
		frac = *stlo - (float)( ideg );
		imm = frac*60.0;
		frac = frac*60.0 - (float)( imm );
		iss = frac*60.0;
		frac = frac*60.0 - (float)( iss );
		ifrac = (int)( frac*100.0 + .5 );
		*isslo = ifrac + ((ideg*100 + imm)*100 + iss)*100;
	}

	/* - Copy extra SDD information, if exists, to header */

	nlcmem = Nxsdd[idfl];
	if( nlcmem != 0 ){
                Isacmem = (int *)cmmem.sacmem[nlcmem];
		*isclas = *(Isacmem++);
		*isfrmt = *(Isacmem++);
		*iscalg = *(Isacmem++);

		for( idx = 1; idx <= MSCOM; idx++ ){
			Iscom[idx] = *(Isacmem++);
		}

		for( idx = 1; idx <= MSREP; idx++ ){
			Isrep[idx] = *(Isacmem++);
		}
	}


	/* - Write the header to disk. */

	nlcdsk = 0;
	nptwr = MWSHDR;
	zwabs( (int *)&nun, (char *)(&Ishdr[1]), nptwr, (int *)&nlcdsk, (int *)nerr );

	/* - Write each data component, if requested. */

	if( ldta ){
		for( jcomp = 0; jcomp < Ncomp[idfl]; jcomp++ ){
			nlcdsk = nlcdsk + nptwr;
			nlcmem = cmdfm.ndxdta[idfl - 1][jcomp];
			nptwr = Nlndta[idfl];
                        Isacmem = (int *)cmmem.sacmem[nlcmem];
                        Sacmem = cmmem.sacmem[nlcmem];

			/*         Convert data to integers before writing */

			for( idx = 0; idx <= (nptwr - 1); idx++ ){
				*(Isacmem++) = *(Sacmem++)*100.0;
			}

			zwabs( (int *)&nun, (char *)(cmmem.sacmem[nlcmem]), nptwr, (int *)&nlcdsk, (int *)nerr );
		}
	}

	/* - Close disk file. */

L_8888:
	zclose( &nun, &ncerr );

	return;

} /* end of function */

