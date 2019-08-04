/** 
 * @file convcolornam.c
 *
 * @brief Convert a color name
 */
#include <string.h>
#include <stdlib.h>
#include "co.h"
#include "mach.h"
#include "gdm.h"
#include "bot.h"
#include "bool.h"
#include "string_utils.h"

#include <fern/chash.h>
extern color COLOR_WHITE;
extern dict *color_dict;

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
convcolorname(char *name, color *c) {
    int i;
    color *c0;
    char ktest[9];
    char *nofill[] = { "none", "empty", "trans", "transparent" };

    /* - Convert input color name to upper case. */
    upcase(name, min(strlen(name), MCPW), ktest, 9);

    for(i = 0; i < 4; i++) {
        if(strcasecmp(name, nofill[i]) == 0) {
            *c = COLOR_WHITE;
            return TRUE;
        }
    }

    /* - Test name versus list of names in default color table. */
    if(!(c0 = dict_get(color_dict, name))) {
        color_foreground_default(c);
        return FALSE;
    }
    *c = *c0;
    return TRUE;
}
