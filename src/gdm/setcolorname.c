
#include <stdlib.h>
#include "co.h"
#include "mach.h"
#include "gem.h"
#include "gdm.h"
#include "bot.h"
#include "debug.h"
#include "vars/chash.h"

extern dict *color_dict;

/** 
 * Set color attribute by name
 *
 * @param color
 *    Name of the desired color
 *    If not found, set to the foreground color
 * @param color_s
 *    Length of \p color
 * 
 * @date   861020:  Original version.
 *
 */
void
setcolorname(char *kolor, int kolor_s) {
    color *p;
    
    UNUSED(kolor_s);
    if(!(p = dict_get(color_dict, kolor))) {
        setcolor_fg_def();
        return;
    }
    setcolor(*p);
}
