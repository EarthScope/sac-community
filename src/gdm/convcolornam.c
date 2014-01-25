/** 
 * @file convcolornam.c
 *
 * @brief Convert a color name
 */
#include <string.h>

#include "co.h"
#include "mach.h"
#include "gdm.h"
#include "bot.h"
#include "bool.h"

/** 
 * Convert a color name to it's equivalent color number 
 *
 * @param
 *   color name
 * @param
 *   color number on output
 *   - 0 if the color was not found
 *
 * @date   871216:  Subtracted 1 to number -- was wrong because of previous
 *                  modification.
 * @date   871001:  Changed so it would also check the first entry in the
 *                  color table.
 * @date   861020:  Original version.
 */

int
convcolorname(char *name, 
              int   *number) {
  int i;
	char ktest[9];
  char *nofill[] = {"none","empty","trans","transparent"};

	/* - Convert input color name to upper case. */
	upcase( name, min(strlen(name),MCPW), ktest,9 );

	/* - Test name versus list of names in default color table. */
	if( lequal( ktest,9, (char*)kmgdm.ctname[0],9, cmgdm.nctsize+1, number ) ){
    *number = *number - 1;
    return TRUE;
  }
  for(i = 0; i < (int)(sizeof(nofill)/sizeof(char*)); i++) {
    if(strcasecmp(name, nofill[i]) == 0) {
      *number = -1;
      return TRUE;
    }
  }
  /* - If not found, return a -1 */
  *number = -1;
  return FALSE;
}

