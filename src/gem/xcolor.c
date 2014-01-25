/** 
 * @file   xcolor.c
 * 
 * @brief  Color Handling 
 * 
 */

#include <stdio.h>

#include "mach.h"
#include "gem.h"
#include "gdm.h"
#include "bool.h"
#include "pl.h"
#include "cpf.h"

#define BAD_COLOR do {                \
  cfmt( "NEED NAME OF A COLOR:",23 ); \
  cresp();                            \
  } while(0) 

int color_on()                 { return cmgem.lcol;   }
int color_foreground()         { return cmgem.icol;   }
int color_background()         { return cmgem.ibacol; }
int color_skeleton()           { return cmgem.iskcol; }
int color_foreground_default() { return 7; }
int color_background_default() { return 0; }

/* Color on or off */
void 
color_switch       (int value) { 
  cmgem.lcol  = value;  
}

void 
color_increment_set(int value) { 
  color_switch(TRUE);
  cmgem.licol = value;  
  cmgem.jicol = 0;
}

/* Background Color */
int 
color_background_set(int color) {
  if(color >= 0) {
    cmgem.ibacol = color;
    color_switch(TRUE);
    return TRUE;
  }
  return FALSE;
}

int 
color_background_set_by_name(char *color) {
  int icolor;
  convcolorname(color, &icolor);
  if(icolor < 0) {
    return FALSE;
  }
  return color_background_set(icolor);
}

/* Data Color */
int
color_data_set(int color) {
  if(color >= 0) {
    cmgem.icol = color;
    color_switch(TRUE);
    return TRUE;
  }
  return FALSE;
}

int 
color_data_set_by_name(char *color) {
  int icolor;
  icolor = 1;
  convcolorname(color, &icolor);
  if(icolor < 0) {
    return FALSE;
  }
  return color_data_set(icolor);
}

/* Skeleton Color */
int
color_skeleton_set(int color) {
  if(color >= 0) {
    cmgem.iskcol = color;
    color_switch(TRUE);
    return TRUE;
  }
  return FALSE;
}
int
color_skeleton_set_by_name(char *color) {
  int icolor;
  convcolorname(color, &icolor);
  if(icolor < 0) {
    return FALSE;
  }
  return color_skeleton_set(icolor);
}

/* Foreground Color (Data + Skeleton) */
int 
color_foreground_set(int color) {
  return color_skeleton_set(color) && color_data_set(color);
}

int 
color_foreground_set_by_name(char* color) {
  int icolor;
  convcolorname(color, &icolor);
  if(icolor < 0) {
    return FALSE;
  }
  return color_foreground_set(icolor);
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
xcolor(int *nerr)
{
	char ktok[10];
	int lnum;
	int inum;
	*nerr = 0;
  

	/* - Parse position-dependent tokens: */

	if( lclog( &cmgem.lcol ) ) {
  } else if(lcint(&lnum)) { 
    if(!color_data_set(lnum)) {
      BAD_COLOR;
    }
  } else if(lcchar(9,ktok,9,&lnum)) {
    if(!color_data_set_by_name(ktok)) {
      BAD_COLOR;
    }
  }
	

	/* - Parse position-independent tokens: */

	while( lcmore( nerr ) ){

    /* -- "SKELETON color/int":  change skeleton color. */
    if( lckey( "SK$",4 ) ){
      if(lcint(&lnum)) {
        if(!color_skeleton_set(lnum)) {
          BAD_COLOR;
        }
      } else if(lcchar(9,ktok,9,&lnum)) {
        if(!color_skeleton_set_by_name(ktok)) {
          BAD_COLOR;
        }
      } else {
        BAD_COLOR;
      }
		}
    /* -- "BACKGROUND color/int":  change the background color. */
    else if( lckey( "BA$",4 ) ){
      if(lcint(&lnum)) {
        if(!color_background_set(lnum)) {
          BAD_COLOR;
        }
      } else if(lcchar(9,ktok,9,&lnum)){
        if(!color_background_set_by_name(ktok)) {
          BAD_COLOR;
        }
      } else {
        BAD_COLOR;
      }
    }
	    /* -- "LIST STANDARD/colorlist":  change the color list. */
    else if( lckey( "L$",3 ) ){
      if( lckey( "S$",3 ) ){
		    inicol( cmgem.iicol, &cmgem.nicol );
      }
      else{
		    cmgem.nicol = 0;
		    while( lcmore(nerr) ) {
          if(lcint(&inum)) {
            if(inum >= 0) {
              if( cmgem.nicol < MICOL )
                cmgem.nicol = cmgem.nicol + 1;
              cmgem.iicol[cmgem.nicol-1] = inum;
            } else {
              BAD_COLOR;
            }
          } else if(lcchar(9,ktok,9,&lnum)){
            convcolorname( ktok, &inum );
            if( inum >= 0 ){
              if( cmgem.nicol < MICOL )
                cmgem.nicol = cmgem.nicol + 1;
              cmgem.iicol[cmgem.nicol-1] = inum;
            } else {
              BAD_COLOR;
            }
          }
		    }
		    if( cmgem.nicol <= 0 )
          inicol( cmgem.iicol, &cmgem.nicol );
		    cmgem.icol = cmgem.iicol[1-1];
        color_switch(TRUE);
		    cmgem.jicol = 0;
      }
    }
    
    /* -- "INCREMENT ON/OFF":  increment color after each file or not */
    else if( lklog( "I$",3, &cmgem.licol ) ){
      color_switch(TRUE);
      cmgem.jicol = 0;
    }
    
    /* -- Bad syntax. */
    else{
      cfmt( "ILLEGAL OPTION:",17 );
      cresp();
    }
	}  /* end while( lcmore( nerr ) ) */


}

