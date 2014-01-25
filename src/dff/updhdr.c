/** 
 * @file   updhdr.c
 * 
 * @brief  Update the header
 * 
 */

#include <string.h>

#include "dff.h"
#include "hdr.h"
#include "msg.h"
#include "bool.h"

#include "errors.h"

/** 
 * Update an old header to the current version
 * 
 * @param nerr 
 *   Error Return Flag
 *   - 0 on Success
 *
 * @date   821001:  Header version 6 installed.
 * @date   801018:  Header version 5 installed.
 * @date   800809:  Fixed bug with headers of files from Octopus.
 * @date   800510:  Header version 4 installed.
 * @date   800105:  Original Prime version.
 *
 */
void
updhdr(int *nerr) {

	char khdr18[9];
	int icomp, jdx ;
	float horzo;

	*nerr = 0;

	/* - The header is updated one version at a time until
	 *   the current version is reached. 
	 */
	while ( *nvhdr < cmhdr.nvhdrc ){

		/* - Version 2 changed the values in each of the header field types
		 *   used to represent an undefined state.  Previously they were all
		 *   equivalent to the ascii 'UNDEF'.  Now they are:
		 *   - FUNDEF = -12345. for floating point fields.
		 *   - NUNDEF = -12345 for integer fields.
		 *   - IUNDEF = -12345 for selected value fields.
		 *   - KUNDEF = 'UNDEF' for alphanumeric fields.
		 *   - no value for undefined logical fields.
		 * - Version 1 was never supported on the Prime. */

		if( *nvhdr == 1 || *nvhdr == cmhdr.nundef ){
			*nvhdr = 2;
		}

		/* - Version 3:
		 *   - added FINI in FHDR(40).
		 *   - added SB in FHDR(55).
		 *   - added SDELTA in FHDR(56).
		 *   - added FMEAN in FHDR(57).
		 *   - replaced ICOMP in IHDR(4) and HORZO in FHDR(21)
		 *     with CMPAZ in FHDR(58), CMPINC in FHDR(59),
		 *     KCMPNM in KHDR(1,6), and LPSPOL in LHDR(2).
		 *   - possible values of ICOMP were IRADNV=23, ITANNV=24,
		 *     IRADEV=25, ITANEV=26, INORTH=27, IEAST=28,
		 *     IHORZA=29, IDOWN=30, and IUP=31. */

		else if( *nvhdr == 2 ){
			icomp = Ihdr[4];
			Ihdr[4] = cmhdr.iundef;
			horzo = Fhdr[21];
			Fhdr[21] = cmhdr.fundef;
			if( icomp == 23 ){
				if( memcmp(kstnm,"ELKO",4) == 0 ){
					*cmpaz = 10.4699;
				}
				else if( memcmp(kstnm,"KANA",4) == 0 ){
					*cmpaz = 93.01;
				}
				else if( memcmp(kstnm,"LAND",4) == 0 ){
					*cmpaz = 185.25;
				}
				else if( memcmp(kstnm,"MINA",4) == 0 ){
					*cmpaz = 307.71;
				}
				else{
					*cmpaz = cmhdr.fundef;
				}
				*cmpinc = 90.;
			}
			else if( icomp == 24 ){
				if( memcmp(kstnm,"ELKO",4) == 0 ){
					*cmpaz = 280.4699;
				}
				else if( memcmp(kstnm,"KANA",4) == 0 ){
					*cmpaz = 3.01;
				}
				else if( memcmp(kstnm,"LAND",4) == 0 ){
					*cmpaz = 95.25;
				}
				else if( memcmp(kstnm,"MINA",4) == 0 ){
					*cmpaz = 217.71;
				}
				else{
					*cmpaz = cmhdr.fundef;
				}
				*cmpinc = 90.;
			}
			else if( (icomp == 25 || icomp == 26) || icomp == 29 ){
				*cmpaz = horzo;
				*cmpinc = 90.;
				*lpspol = TRUE;
			}
			else if( icomp == 27 ){
				*cmpaz = 0.;
				*cmpinc = 90.;
				*lpspol = TRUE;
			}
			else if( icomp == 28 ){
				*cmpaz = 90.;
				*cmpinc = 90.;
				*lpspol = TRUE;
			}
			else if( icomp == 30 ){
				*cmpaz = 0.;
				*cmpinc = 180.;
				*lpspol = FALSE;
			}
			else if( icomp == 31 ){
				*cmpaz = 0.;
				*cmpinc = 0.;
				*lpspol = TRUE;
			}
			else{
				*cmpaz = cmhdr.fundef;
				*cmpinc = cmhdr.fundef;
				*lpspol = TRUE;
			}
			*nvhdr = 3;

			/* - Version 4:
			 *   - added LOVROK in LHDR(3).
			 *   - moved NZHOUR from NHDR(5) to NHDR(4).
			 *   - moved NZMIN from NHDR(6) to NHDR(5).
			 *   - equivalenced NZDTTM array to NZYEAR in NHDR(2).
			 *   - changed entire KHDR array.  Before each entry was 24 characters
			 *     in length.  Now they may be any multiple of 8 characters in length.
			 *     KHDR was changed to a 1 dimensional array but the length was
			 *     not changed.
			 *
			 *         variable           old_loc   new_loc   new_len
			 *         --------           -------   -------   -------
			 *         KSTNM             1         1         1
			 *         KEVNM             4         2         2
			 *         KHOLE             7         4         3
			 *         KZTIME            10        7         2
			 *         KZDATE            13        9         3
			 *         KCMPNM            16       12         3
			 *         KUSER             NEW       13        3 */

		}
		else if( *nvhdr == 3 ){
			Nhdr[4] = Nhdr[5];
			Nhdr[5] = Nhdr[6];
			Nhdr[6] = cmhdr.nundef;
			Lhdr[3] = TRUE;
			strcpy( kmhdr.khdr[1], kmhdr.khdr[3] );
			strcpy( kmhdr.khdr[2], kmhdr.khdr[4] );
			strcpy( kmhdr.khdr[3], kmhdr.khdr[6] );
			strcpy( kmhdr.khdr[4], kmhdr.khdr[7] );
			strcpy( kmhdr.khdr[5], kmhdr.khdr[8] );
			strcpy( kmhdr.khdr[6], kmhdr.khdr[9] );
			strcpy( kmhdr.khdr[7], kmhdr.khdr[10] );
			strcpy( kmhdr.khdr[8], kmhdr.khdr[12] );
			strcpy( kmhdr.khdr[9], kmhdr.khdr[13] );
			strcpy( kmhdr.khdr[10], kmhdr.khdr[14] );
			strcpy( kmhdr.khdr[11], kmhdr.khdr[15] );
			strcpy( kmhdr.khdr[12], "UNDEF   " );
			for( jdx = 13; jdx < SAC_HEADER_STRINGS; jdx++ ){
				strcpy( kmhdr.khdr[jdx], "        " );
			}
			*nvhdr = 4;

			/* - Version 5:
			 *   - Changed alphanumeric undefined fields from 'UNDEF' to '-12345'.
			 *   - Changed all KHDR fields from dimensioned CHAR*8 variables
			 *     to non dimensioned variables of correct character length.
			 *   - Decreased size of KHOLE from 24 to 8 characters.
			 *   - Changed KUSER of length 24 char. to KUSER0, KUSER1, and KUSER2
			 *     each of length 8 char. Moved them from KHDR(13) to KHDR(18).
			 *   - Deleted KZDATE, KZTIME and KCMPNM.
			 *   - Added 13 new alpha fields: KO, KA, KTn (n=0,9), and KF.
			 *   - Current arrangement of KHDR is therefore:
			 *           variable            old_loc   new_loc   new_len
			 *           --------            -------   -------   -------
			 *           KSTNM              1         1         1
			 *           KEVNM              2         2         2
			 *           KHOLE              4         4         1
			 *           KO                NEW       5         1
			 *           KA                NEW       6         1
			 *           KTn               NEW       7-16      1 each
			 *           KF                NEW       17        1
			 *           KUSER0             13        18        1
			 *           KUSER1             14        19        1
			 *           KUSER2             15        20        1
			 *           KHDR21             EMPTY     21        1
			 *           KHDR22             EMPTY     22        1
			 *           KHDR23             EMPTY     23        1
			 *           KHDR24             EMPTY     24        1
			 *   - Moved NSNPTS from NHDR(10) to NHDR(11).
			 *   - Moved NPTS from NHDR(1) to NHDR(10).
			 *   - Changed size of NZDTTM array from 4 to 6.
			 *   - Moved NZDTTM and NZYEAR from NHDR(2) to NHDR(1).
			 *   - Moved NZJDAY from NHDR(3) to NHDR(2).
			 *   - Moved NZHOUR from NHDR(4) to NHDR(3).
			 *   - Moved NZMIN from NHDR(5) to NHDR(4).
			 *   - Converted ZSECS in FHDR(5) to NZSEC in NHDR(5) and
			 *     NZMSEC in NHDR(6).
			 *   - Moved FINI from FHDR(40) to FHDR(21). */

		}
		else if( *nvhdr == 4 ){
			if( strcmp(kmhdr.khdr[12],"UNDEF   ") != 0 ){
				strcpy( khdr18, kmhdr.khdr[12] );
			}
			else{
				strcpy( khdr18, kmhdr.kundef );
			}
			for( jdx = 4; jdx < 24; jdx++ ){
				strcpy( kmhdr.khdr[jdx], kmhdr.kundef );
			}
			strcpy( kmhdr.khdr[17], khdr18 );
			Nhdr[11] = Nhdr[10];
			Nhdr[10] = Nhdr[1];
			Nhdr[1] = Nhdr[2];
			Nhdr[2] = Nhdr[3];
			Nhdr[3] = Nhdr[4];
			Nhdr[4] = Nhdr[5];
			Nhdr[5] = Fhdr[5];
			Nhdr[6] = (int)( 1000.*(Fhdr[5] - (float)( Nhdr[5] )) + 
			 0.5 );
			Fhdr[5] = cmhdr.fundef;
			Fhdr[21] = Fhdr[40];
			Fhdr[40] = cmhdr.fundef;
			*nvhdr = 5;

			/* - Header version 6:
			 *   (1) Added LCALDA.  If .TRUE. distance/azimuth are always
			 *       recomputed each time file is read.
			 * */
		}
		else if( *nvhdr == 5 ){
			*lcalda = TRUE;
			*nvhdr = 6;

		}

		else {
		    *nerr = ERROR_HEADER_FORMAT ;
		    setmsg ( "ERROR" , *nerr ) ;
		    outmsg () ;
		    break ;
		}
	}

	return;
}

