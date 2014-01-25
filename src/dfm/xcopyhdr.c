/** 
 * @file   xcopyhdr.c
 * 
 * @brief  Copy header variables
 * 
 */

#include <string.h>

#include "dfm.h"
#include "bool.h"
#include "hdr.h"
#include "lhf.h"


#include "clf.h"
#include "cpf.h"
#include "dff.h"

/** 
 * Execute the command COPYHDR which copies header varibles from one
 *    file to the remaining file in the data file list
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   880413:  Was not reinitializing header list correctly.
 * @date   870209:  Converted to an internal command.
 * @date   840906:  Original XSC version.
 *
 */
void 
xcopyhdr(int *nerr) {

	char ktemp1[MCPFN+1], ktemp2[9], ktemp3[2][9];
	int lfirst, lfound, ltemp;
	int icatcox, idflco, itemcox, itemp, j;
	int j_, jdfl, jhdrco, nhdrco, notusd, ntemp;
	float ftemp;


	*nerr = 0;
    memset(ktemp1, 0, sizeof(ktemp1));
    memset(ktemp2, 0, sizeof(ktemp2));
	/* - Loop on each token in command: */
	lfirst = TRUE;
    idflco = 1;
    nhdrco = 0;
L_1000:
	if( lcmore( nerr ) ){

		/* -- "FROM name|n":  determine which file to copy from. */
		if( lckey( "FROM#$",7 ) ){
			if( lcirc( 1, cmdfm.ndfl, &idflco ) ){
				}
			else if( lcchar( MCPFN, ktemp1,MCPFN+1, &notusd ) ){
        idflco =string_list_find(datafiles, ktemp1, MCPFN+1);
        if(idflco < 0) {
          arg_prev();
          cfmt( "BAD FILE NAME:",16 );
          cresp();
				}
        idflco += 1;
      }
      else{
        cfmt( "NEED A FILE NAME OR A NUMBER:",31 );
        cresp();
      }
    }
    /* -- "hdrvar":  name of a header variable to copy. */
    else if( lcchar( MCPW, ktemp2,9, &notusd ) ){
			hdrfld( ktemp2,9, &icatcox, &itemcox, &lfound );
			if( lfound ){
				if( lfirst ){
					nhdrco = 0;
					lfirst = FALSE;
					}
				if( nhdrco < MHDRCO ){
					nhdrco = nhdrco + 1;
					Icatco[nhdrco] = icatcox;
					Itemco[nhdrco] = itemcox;
					}
				else{
					cfmt( "TOO MANY HEADER VARIABLES:",28 );
					cresp();
					}
				}
			else{
				cfmt( "ILLEGAL HEADER VARIABLE:",26 );
				cresp();
				}

			/* -- Bad syntax. */
			}
		else{
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();

			}
		goto L_1000;

		}

	if( *nerr != 0 )
		goto L_8888;

	/* CHECKING PHASE: */
	/* - Test for a non-null data file list. */

	vflist( nerr );
	if( *nerr != 0 )
		goto L_8888;

	/* EXECUTION PHASE: */
	/* - Loop on each header itemco in list. */

	for( jhdrco = 1; jhdrco <= nhdrco; jhdrco++ ){
        ftemp = SAC_FLOAT_UNDEFINED;
        itemp = SAC_INT_UNDEFINED;
        ntemp = SAC_ENUM_UNDEFINED;
        ltemp = SAC_LOGICAL_UNDEFINED;
        strncpy(ktemp3[0], SAC_CHAR_UNDEFINED, 8);
        strncpy(ktemp3[1], SAC_CHAR_UNDEFINED, 8);
		/* -- Get master file's header from memory manager. */
		getfil( idflco, FALSE, &notusd, &notusd, &notusd, nerr );
		if( *nerr != 0 )
			goto L_8888;

		/* -- Get header variable from master file. */
		if( Icatco[jhdrco] == cmlhf.icatf ){
			ftemp = Fhdr[Itemco[jhdrco]];
			}
		else if( Icatco[jhdrco] == cmlhf.icati ){
			itemp = Ihdr[Itemco[jhdrco]];
			}
		else if( Icatco[jhdrco] == cmlhf.icatn ){
			ntemp = Nhdr[Itemco[jhdrco]];
			}
		else if( Icatco[jhdrco] == cmlhf.icatl ){
			ltemp = Lhdr[Itemco[jhdrco]];
			}
		else if( Icatco[jhdrco] == cmlhf.icatk ){
			for( j = 1; j <= Nkhdr[Itemco[jhdrco]]; j++ ){
				j_ = j - 1;
				strcpy( ktemp3[j_], kmhdr.khdr[Itemco[jhdrco] + j_ - 
				 1] );
				}
			}

		/* -- Copy this variable to all (other) files in DFL. */
		for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){
			getfil( jdfl, FALSE, &notusd, &notusd, &notusd, nerr );
			if( *nerr != 0 )
				goto L_8888;
			if( Icatco[jhdrco] == cmlhf.icatf ){
				Fhdr[Itemco[jhdrco]] = ftemp;
				}
			else if( Icatco[jhdrco] == cmlhf.icati ){
				Ihdr[Itemco[jhdrco]] = itemp;
				}
			else if( Icatco[jhdrco] == cmlhf.icatn ){
				Nhdr[Itemco[jhdrco]] = ntemp;
				}
			else if( Icatco[jhdrco] == cmlhf.icatl ){
				Lhdr[Itemco[jhdrco]] = ltemp;
				}
			else if( Icatco[jhdrco] == cmlhf.icatk ){
				for( j = 1; j <= Nkhdr[Itemco[jhdrco]]; j++ ){
					j_ = j - 1;
					strcpy( kmhdr.khdr[Itemco[jhdrco] + j_ - 1], ktemp3[j_]
					  );
					}
				}

			/* -- Return file to memory manager. */
			putfil( jdfl, nerr );
			if( *nerr != 0 )
				goto L_8888;
			}

		}

L_8888:
	return;
}

