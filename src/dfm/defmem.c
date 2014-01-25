/** 
 * @file   defmem.c
 * 
 * @brief  Define memory requirement for a file
 * 
 */

#include <string.h>

#include "dfm.h"
#include "bool.h"
#include "hdr.h"

#include "errors.h"

#include "msg.h"
#include "clf.h"

/** 
 * Define memory requirement for a given data fiile
 * 
 * @param idfl 
 *    Data file list number
 * @param lcutnow 
 *    - TRUE if the file is being read and needs to be cut
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   850415:  Changes due to restructuring of DFM common block.
 * @date   840228:  Moved cut logic to new subroutine, DEFCUT.
 * @date   820809:  Changed method of storing cut parameters.
 * @date   810903:  Fixed bug in computing stop time when "N" option used.
 *
 */
void 
defmem(int  idfl, 
       int  lcutnow, 
       int  *nerr) {

	int lall;
    char *tmp;
	*nerr = 0;

	/* - Get name of data file. */
    tmp = string_list_get(datafiles, idfl-1);
	/* - Entire file is read if:
	 *   (1) cut option is off.
	 *   (2) file is a spectral file
	 *   (3) file is unevenly spaced */

	if( !cmdfm.lcut || !lcutnow ){
		lall = TRUE;
	}
	else if( *iftype == *irlim || *iftype == *iamph ){
		setmsg( "WARNING", ERROR_CANT_CUT_SPECTRAL_FILE );
        apcmsg2(tmp, strlen(tmp)+1);
		outmsg();
		lall = TRUE;
	}
	else if( !*leven ){
		setmsg( "WARNING", ERROR_CANT_CUT_UNEVENLY_SPACED_FILE );
        apcmsg2(tmp, strlen(tmp)+1);
		outmsg();
		lall = TRUE;
	}
	else{
		lall = FALSE;
	}

	/* - Set parameters if entire file is to be read. */
	if( lall ){
		Nstart[idfl] = 1;
		Nstop[idfl] = *npts;
		Ntotal[idfl] = *npts;
		Nfillb[idfl] = 0;
		Nfille[idfl] = 0;
	}

	/* - Set parameters for partial read. */
	else{
		defcut( kmdfm.kcut , cmdfm.ocut , idfl , nerr );
		if( *nerr != 0 )
			goto L_8888;
	}

	/* - Define number of data components. */
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

	/* - Define length of each data component, including zero fill. */
	Nlndta[idfl] = Nstop[idfl] - Nstart[idfl] + 1;

L_8888:
	return;
}

