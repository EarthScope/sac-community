/** 
 * @file xsetdevice.c
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gdm.h"
#include "gam.h"

#include "co.h"
#include "cpf.h"

#define TOKEN_LENGTH 256

/** 
 * Command SETDEVICE to set the default graphics device
 *
 * @param nerr
 *   Error Return Flag
 *    - 0 on Success
 *    - Non-Zero on Error
 *
 * @date   870416:  Original version.
 *
 */

void 
xsetdevice(int *nerr) {

        char token[TOKEN_LENGTH];
        int is_number;
        display_t *dev;
        char *p;

	*nerr = 0;

	/* - Loop on each token in command: */

	while ( lcmore( nerr ) ){

		/* -- "text":  the name of the default graphics device. */
    if(lcchar(TOKEN_LENGTH, &token[0], TOKEN_LENGTH, &is_number)) {
      p = strchr(&token[0], ' ');
      *p = 0;
      if((dev = gdm_get_device_by_name( &token[0]))) {
        strscpy(kmgam.kgddef, dev->name, strlen(dev->name));
      } else {
        cfmt( "ILLEGAL OPTION:",17 );
        cresp();
      }
    }
	}

	return;

}

