/** 
 * @file   crname.c
 * 
 * @brief  Create a pathname
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "bbs.h"
#include "dff.h"
#include "vars.h"
#include "msg.h"
#include "co.h"

#include "errors.h"


#include "bot.h"

/** 
 * Create a pathname piece by piece
 * 
 * @param kname 
 *   Current name to build on
 * @param kname_s 
 *   Length of \p kname
 * @param kdelim 
 *   Delimiter to place before "next" part
 * @param kappnd 
 *   New part of the path name to append to the current name
 * @param kappnd_s 
 *   Length of \p kappnd
 * @param nerr 
 *   Error Return Flag
 *   - 0 on Success
 *   - Non-Zero on Failure
 *   - ERROR_FILE_NAME_TOO_LONG
 *
 * @note Local Variables
 *   - MNAME:   Maximum length of KNAME. [i]
 *   - NNAME:   Current length of KNAME. [i]
 *   - NAPPND:  Length of KAPPND. [i]
 *
 * @date   831110:  Original version.
 *
 */
void 
crname(char *kname, 
       int   kname_s, 
       char  kdelim, 
       char *kappnd, 
       int   kappnd_s, 
       int  *nerr)
{
	int mname, nappnd, nname;
  char *cattemp;

	*nerr = 0;

	/* - Determine length of base and additional name. */

	mname = (kname_s - 1);
	nname = indexb( kname,kname_s );
	nappnd = indexb( kappnd,kappnd_s );

	/* - Concantenate delimiter and new name to base name if they fit. */

	if( mname >= (nname + nappnd + 1) ){
                cattemp = malloc(nappnd+2);
                cattemp[0] = kdelim;
                strncpy(cattemp+1,kappnd,nappnd);
                cattemp[nappnd+1] = '\0';
		subscpy( kname, nname, -1, kname_s - 1, cattemp);
                free(cattemp);
	}
	else{
		*nerr = ERROR_FILE_NAME_TOO_LONG;
    error(*nerr, "%s", kappnd);
	}

	return;
}

