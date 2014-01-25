/** 
 * @file   qpicks.c
 * 
 * @brief  Report PICKS parameters
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "eam.h"
#include "gam.h"


#include "exm.h"

/** 
 * Report the current values of the PICKS parameters
 * 
 * @date   820921:  Original version (from QDISPL).
 *
 */
void 
qpicks() {

	int j, j_;
        char *cattemp;

	replv( "PICK display$",14, cmgam.ldsppk );
	repav( "Type of each pick display$",27, "        ",9 );
	for( j = 1; j <= MPKNAM; j++ ){
		j_ = j - 1;
                cattemp = malloc(2+2+2);
                strcpy(cattemp,"  ");
                strncat(cattemp,kmgam.kpknam[j_],2);
                strcat(cattemp,"$");
		repav( cattemp, 2+2+1+1, (char*)kmgam.kpktyp[Ipktyp[j] - 1],9 );
                free(cattemp);
		}
	reprv( "WIDTH of pick lines$",21, cmgam.pkwdth );
	reprv( "HEIGHT of pick lines$",22, cmgam.pkhgth );

	return;
}

