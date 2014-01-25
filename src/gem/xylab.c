/** 
 * @file   xylab.c
 * 
 * @brief  Control the Y Labeling
 * 
 */

#include <string.h>

#include "co.h"
#include "gem.h"
#include "bool.h"


#include "cpf.h"

void 
ylabel_switch(int flag) {
  cmgem.ylabel.on = flag;  /* X Label Flag */
}

void
ylabel_label(char *c) {
  ylabel_switch(TRUE);
  strncpy(kmgem.kylab, c, MCPTXT); /* kmgem.lxlab - Label for the X Axis */
  cmgem.ylabel.len = min(MCPTXT, strlen(c));
}

void
ylabel_location(char *c) {
  int i;
  for(i = 0; i < SAC_LABEL_LOCATIONS; i++) {
    if(strncasecmp(c, kmgem.ksides[i], min(strlen(c), SAC_KEY_SIZE_LENGTH)) == 0) {
      cmgem.ylabel.pos = i + 1;
      ylabel_switch(TRUE);
    }
  }
}


void
ylabel_size(char *c) {
  int i;
  for(i = 0; i < SAC_FONT_SIZES; i++) {
    if(strncasecmp(c, kmgem.ktxsiz[i], min(strlen(c), SAC_KEY_SIZE_LENGTH)) == 0) {
      cmgem.ylabel.text_size = cmgem.dtxsiz[i];
    }
  }
}

/** 
 *  Parse the command "ylabel" and control the Y Labeling 
 * 
 * @param nerr 
 *   Error Return Flag
 *   - 0 on Success
 *   - Non-Zero on Error
 * 
 * @date   830818:  Changes due to new text size and angle attributes.
 * @date   820924:  Moved LCQUOT to top of parse loop.
 * @date   820614:  Original version.
 *
 */
void 
xylab(int *nerr)
{
	int iylabs;

	*nerr = 0;

	/* - Loop on each token in command: */

L_1000:
	if( lcmore( nerr ) ){

		/* -- Turn y labeling on/off: */
		if( lclog( &cmgem.ylabel.on ) ){

			/* -- Define text of y label: */
			}
		else if( lcquot( MCPTXT, kmgem.kylab,145, &cmgem.ylabel.len ) ){
			cmgem.ylabel.on = TRUE;

			/* -- Set y label size: */
			}
		else if( lklist( "S$",3, (char*)kmgem.ktxsiz,9, MTXSIZ, &iylabs ) ){
			cmgem.ylabel.text_size = cmgem.txsiz[iylabs-1];

			/* -- Set location of y label: */
			}
		else if( lklist( "L$",3, (char*)kmgem.ksides,9, 4, &cmgem.ylabel.pos ) ){
			}
        /* -- Bad syntax. */
		else{
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();

			}
		goto L_1000;
		}

       
	return;
}

