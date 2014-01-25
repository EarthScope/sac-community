
#include <string.h>

#include "gem.h"
#include "gdm.h"
#include "bool.h"
#include "msg.h"
#include "pl.h"
#include "cpf.h"
#include "bot.h"

int
color_parse(char *key, int *c) {
  int nr;
  if(sscanf(key, "%d%n", c, &nr) == 1 && nr == (int)strlen(key)) {
    return TRUE;
  }
  return convcolorname(key, c);
}

int
color_parse2(char *key, int *p, int *n) {
  int nr;
  char pos[100], neg[100];
  if(sscanf(key, "%[^/]/%[^/]%n", pos, neg, &nr) == 2 && nr == (int)strlen(key)) {
    if(!color_parse(pos, p)) {
      cfmt("UNKNOWN COLOR: ", 15);
    }
    if(!color_parse(neg, n)) {
      cfmt("UNKNOWN COLOR: ", 15);
    }
    return TRUE;
  }
  return FALSE;
}

void /*FUNCTION*/ xline(nerr)
int *nerr;
{
	int lwarning;
	int int_;



	/*=====================================================================
	 * PURPOSE:  To execute the parameter-setting command LINE.
	 *           This command controls the linestyle attributes.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *      nerr:  Error return flag [i]
	 *=====================================================================
	 * MODULE/LEVEL: gem/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    mach:
	 *    gem:     MILINE, ICLINE
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    gem:     lline, icline, liline
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    sac:     lcmore, cfmt, cresp, lclog, lcint, lklog, lckey,
	 *             setmsg, apcmsg, apimsg, aplmsg, outmsg
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    910301:  Changed iline to icline.
	 *    900402:  Added warning message if linestyle list exceeds maximum.
	 *    830114:  Added LIST option.
	 *    820817:  Changed to newest set of parsing and checking functions.
	 *    820817:  Documented subroutine.
	 *    811228:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900402
	 *===================================================================== */
	/* PROCEDURE: */
	*nerr = 0;

	/* - Parse positional tokens first: */

	/* -- ON/OFF turns on/off line drawing. */
	if( lclog( &cmgem.lline ) ){

		/* -- An integer changes the linestyle and turns line drawing on. */
		}
	else if( lcint( &cmgem.icline ) ){
		cmgem.lline = TRUE;

		}

	/* - Parse position-independent tokens: */

L_1000:
	if( lcmore( nerr ) ){

		/* -- INC turns on or off the increment linestyle attribute. */
		if( lklog( "I$",3, &cmgem.liline ) ){
			cmgem.lline = TRUE;
			cmgem.jiline = 0;

			/* -- SOLID changes linestyle to solid. */
			}
		else if( lckey( "S$",3 ) ){
			cmgem.icline = LINE_STYLE_SOLID;
			cmgem.lline = TRUE;

			/* -- DOTTED changes linestyle to dotted. */
			}
		else if( lckey( "D$",3 ) ){
			cmgem.icline = LINE_STYLE_DOTTED;
			cmgem.lline = TRUE;

    }
    /* -- FILL ON|OFF|n/n|LIST n/nChange Fill style */
    else if( lckey("FILL$", 6) ) {
      int nkey;
      char key[101];
      if(lclog(&cmgem.lfill)) {
      } else if(lckey("LIST$", 6)) { /* LIST n/n n/n */
        cmgem.lfill  = TRUE;
        cmgem.lifill = TRUE;
        if(lckey("STANDARD$",10)) {
          inicol(cmgem.iifillp, &cmgem.nifill);
          inicol(cmgem.iifilln, &cmgem.nifill);
        } else {
          int i;
          i = 0;
          cmgem.nifill = 0;
          while(lcchar(100, key, 100, &nkey)) {
            key[100] = 0;
            rstrip(key);
            if(cmgem.nifill >= MICOL) {
            } else if(color_parse2(key, &cmgem.iifillp[i], &cmgem.iifilln[i])) {
              cmgem.nifill++;
              i++;
            } else {
              cfmt("UNKNOWN FILL FORMAT: ", 21);
            }
          }
        }
      } else if(lcchar(100,key,100,&nkey)) {
        cmgem.lfill  = TRUE;
        cmgem.lifill = FALSE;
        key[100] = 0;
        rstrip(key);
        if(!color_parse2(key, &cmgem.ifill[0], &cmgem.ifill[1])) {
          cfmt("UNKNOWN FILL FORMAT: ", 21);
        }
      }
    }
    
			/* -- "LIST STANDARD/n ...":  change list of linestyle numbers to use. */
		else if( lckey( "L$",3 ) ){
			if( lckey( "S$",3 ) ){
				inilin( cmgem.iiline, &cmgem.niline );
				}
			else{
				cmgem.niline = 0;
				lwarning = FALSE;
L_1100:
				if( lcint( &int_ ) ){
					if( cmgem.niline < MILINE ){
						cmgem.niline = cmgem.niline + 1;
						cmgem.iiline[cmgem.niline-1] = int_;
						}
					else if( !lwarning ){
						setmsg( "WARNING", 1 );
						apcmsg( "Maximum length of linestyle list is"
						 ,36 );
						apimsg( MILINE );
						aplmsg( "Will ignore remaining entries in list."
						 ,39 );
						outmsg();
						clrmsg();
						lwarning = TRUE;
						}
					goto L_1100;
					}
				if( cmgem.niline <= 0 )
					inilin( cmgem.iiline, &cmgem.niline );
				cmgem.icline = cmgem.iiline[1-1];
				cmgem.lline = TRUE;
				cmgem.jiline = 0;
				}

			/* -- Bad syntax. */
			}
		else{
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();

			}
		goto L_1000;

		}

       
	return;

} /* end of function */

