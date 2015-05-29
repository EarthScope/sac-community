/** 
 * @file   xcolor.c
 * 
 * @brief  Color Handling 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mach.h"
#include "gem.h"
#include "gdm.h"
#include "bool.h"
#include "pl.h"
#include "cpf.h"

#define _BLACK_ {0,0,0, "black"};
#define _WHITE_ {255,255,255, "white"};

color COLOR_WHITE = _WHITE_;
color COLOR_RED   = {255,0,0, "red"};
color COLOR_BLUE  = {0,0,255, "blue"};
color COLOR_GREEN = {0,255,0, "green"};
color COLOR_BLACK = _BLACK_;
color COLOR_FG_DEFAULT = _BLACK_;
color COLOR_BG_DEFAULT = _WHITE_;

color COLORS[] = {
    {  0,   0,   0, "black"},
    {255,   0,   0, "red"},
    {  0, 255,   0, "green"},
    {  0,   0, 255, "blue"},
    {255, 255,   0, "yellow"},
    {  0, 255, 255, "cyan"},
    {255,   0, 255, "magenta"},
    {255, 255, 255, "white"},
};


#define BAD_COLOR do {                \
  cfmt( "NEED NAME OF A COLOR:",23 ); \
  cresp();                            \
  } while(0)

int
color_on() {
    return cmgem.lcol;
}

void
color_foreground(color *c) {
    *c = cmgem.icol;
}

void
color_background(color *c) {
    *c = cmgem.ibacol;
}

void
color_skeleton(color *c) {
    *c = cmgem.iskcol;
}

void
color_foreground_default(color *c) {
    *c = COLOR_FG_DEFAULT;
}

void
color_background_default(color *c) {
    *c = COLOR_BG_DEFAULT;
}

/* Color on or off */
void
color_switch(int value) {
    cmgem.lcol = value;
}

void
color_increment_set(int value) {
    color_switch(TRUE);
    cmgem.licol = value;
    cmgem.jicol = 0;
}

/* Background Color */
int
color_background_set(color c) {
    cmgem.ibacol = c;
    color_switch(TRUE);
    return TRUE;
}

int
color_background_set_by_name(char *kolor) {
    color c;
    return convcolorname(kolor, &c) && color_background_set(c);
}

/* Data Color */
int
color_data_set(color c) {
    cmgem.icol = c;
    color_switch(TRUE);
    return TRUE;
}

int
color_data_set_by_name(char *kolor) {
    color c;
    return convcolorname(kolor, &c) && color_data_set(c);
}

/* Skeleton Color */
int
color_skeleton_set(color c) {
    cmgem.iskcol = c;
    color_switch(TRUE);
    return TRUE;
}

int
color_skeleton_set_by_name(char *kolor) {
    color c;
    return convcolorname(kolor, &c) && color_skeleton_set(c);
}

/* Foreground Color (Data + Skeleton) */
int
color_foreground_set(color c) {
    return color_skeleton_set(c) && color_data_set(c);
}

int
color_foreground_set_by_name(char *kolor) {
    color c;
    return convcolorname(kolor, &c) && color_foreground_set(c);
}

color *
color_dup(color c) {
    color *out;
    out = (color *) malloc(sizeof(color));
    *out = c;
    return out;
}

/** 
 * parse the parameter-setting command COLOR.
 *    COLOR controls the color display attributes.
 * 
 * @param nerr 
 *   Error return Flag
 *   - 0 on Success
 *   - Non-Zero on Error
 *
 * @bug Not sending error message if user attempts to create a
 *      too large a color list.  The last color in list is changed.
 *
 * @date   821221:  Added ability to change color list.
 * @date   820809:  Changed to newest set of parsing and checking functions.
 * @date   820305:  Original version.
 *
 */
void
xcolor(int *nerr) {
    char ktok[10];
    int lnum;
    int inum;
    color rgb;
    *nerr = 0;

    /* - Parse position-dependent tokens: */

    if (lclog(&cmgem.lcol)) {
    } else if (lcint(&lnum)) {
        if(!color_index_to_rgb(lnum,&rgb) || !color_data_set(rgb)) {
            BAD_COLOR;
        }
    } else if (lcchar(ktok, sizeof(ktok))) {
        if (!color_data_set_by_name(ktok)) {
            BAD_COLOR;
        }
    }

    /* - Parse position-independent tokens: */

    while (lcmore(nerr)) {

        /* -- "SKELETON color/int":  change skeleton color. */
        if (lckey("SK$", 4)) {
            if (lcint(&lnum)) {
                if (!color_index_to_rgb(lnum,&rgb) || !color_skeleton_set(rgb)) {
                    BAD_COLOR;
                }
            } else if (lcchar(ktok, sizeof(ktok))) {
                if (!color_skeleton_set_by_name(ktok)) {
                    BAD_COLOR;
                }
            } else {
                BAD_COLOR;
            }
        }
        /* -- "BACKGROUND color/int":  change the background color. */
        else if (lckey("BA$", 4)) {
            if (lcint(&lnum)) {
                if (!color_index_to_rgb(lnum,&rgb) || !color_background_set(rgb)) {
                    BAD_COLOR;
                }
            } else if (lcchar(ktok, sizeof(ktok))) {
                if (!color_background_set_by_name(ktok)) {
                    BAD_COLOR;
                }
            } else {
                BAD_COLOR;
            }
        }
        /* -- "LIST STANDARD/colorlist":  change the color list. */
        else if (lckey("L$", 3)) {
            if (lckey("S$", 3)) {
                inicol(cmgem.iicol, &cmgem.nicol);
            } else {
                cmgem.nicol = 0;
                while (lcmore(nerr)) {
                    if (lcint(&inum)) {
                        if (inum >= 0) {
                            if (cmgem.nicol < MICOL)
                                cmgem.nicol = cmgem.nicol + 1;
                            color_index_to_rgb(inum, &cmgem.iicol[cmgem.nicol - 1]);
                        } else {
                            BAD_COLOR;
                        }
                    } else if (lcchar(ktok, sizeof(ktok))) {
                        if(convcolorname(ktok, &rgb)) {
                            cmgem.iicol[cmgem.nicol] = rgb;
                            cmgem.nicol ++;
                        } else {
                            BAD_COLOR;
                        }
                    }
                }
                if (cmgem.nicol <= 0) {
                    inicol(cmgem.iicol, &cmgem.nicol);
                }
                cmgem.icol = cmgem.iicol[0];
                color_switch(TRUE);
                cmgem.jicol = 0;
            }
        }

        /* -- "INCREMENT ON/OFF":  increment color after each file or not */
        else if (lklog("I$", 3, &cmgem.licol)) {
            color_switch(TRUE);
            cmgem.jicol = 0;
        }

        /* -- Bad syntax. */
        else {
            cfmt("ILLEGAL OPTION:", 17);
            cresp();
        }
    }                           /* end while( lcmore( nerr ) ) */

}
