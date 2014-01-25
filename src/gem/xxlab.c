/** 
 * @file   xxlab.c
 * 
 * @brief  Control the X Labeling
 * 
 */

#include <string.h>

#include "co.h"
#include "gem.h"
#include "bool.h"


#include "cpf.h"

void 
xlabel_switch(int flag) {
  cmgem.xlabel.on = flag;  /* X Label Flag */
}

void
xlabel_label(char *c) {
  xlabel_switch(TRUE);
  strncpy(kmgem.kxlab, c, MCPTXT); /* kmgem.lxlab - Label for the X Axis */
  cmgem.xlabel.len = min(MCPTXT, strlen(c));
}

void
xlabel_location(char *c) {
  int i;
  for(i = 0; i < SAC_LABEL_LOCATIONS; i++) {
    if(strncasecmp(c, kmgem.ksides[i], min(strlen(c), SAC_KEY_SIZE_LENGTH)) == 0) {
      cmgem.xlabel.pos = i + 1;
      xlabel_switch(TRUE);
    }
  }
}


void
xlabel_size(char *c) {
  int i;
  for(i = 0; i < SAC_FONT_SIZES; i++) {
    if(strncasecmp(c, kmgem.ktxsiz[i], min(strlen(c), SAC_KEY_SIZE_LENGTH)) == 0) {
      cmgem.xlabel.text_size = cmgem.dtxsiz[i];
    }
  }
}

/** 
 * Parse the command "xlabel" and control the X Labeling
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
xxlab(int *nerr) {
	int ixlabs;

	*nerr = 0;

	/* - Loop on each token in command: */

L_1000:
	if( lcmore( nerr ) ){

		/* -- Turn titling on/off: */
		if( lclog( &cmgem.xlabel.on ) ){

			/* -- Define text of x label: */
			}
		else if( lcquot( MCPTXT, kmgem.kxlab,145, &cmgem.xlabel.len ) ){
			cmgem.xlabel.on = TRUE;

			/* -- Set x label size: */
			}
		else if( lklist( "S$",3, (char*)kmgem.ktxsiz,9, MTXSIZ, &ixlabs ) ){
			cmgem.xlabel.text_size = cmgem.txsiz[ixlabs-1];

			/* -- Set location of x label: */
			}
		else if( lklist( "L$",3, (char*)kmgem.ksides,9, 4, &cmgem.xlabel.pos ) ){
        }
        /* -- Bad syntax. */
		else{
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();

			}
		goto L_1000;
		}

	/* - The above loop is over when one of two conditions has been met:
	 *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
	 *   (2) All the tokens in the command have been successfully parsed. */

       
	return;

}

