/** 
 * @file   ljust.c
 * 
 * @brief  Left justify a character string
 * 
 */

#include "ucf.h"

/** 
 * Left justify a character string
 * 
 * @param ksym 
 *    Character string to justify and left justified on output
 * @param ksym_s 
 *    Length of\p ksym
 *
 * @date   900322:  Fixed problem when ncout gt ncsym. (VAX/VMS bug fix.)
 * @date   830811:  Original version (from ZLJUST).
 * @date   871022:  Documented/Reviewed
 *
 */
void 
ljust(char *ksym, 
      int   ksym_s)
{
	int i, count, ncsym, ncsave, nchar;
        char *temp;

	/* - Determine number of characters in symbol. */
	ncsym = (ksym_s - 1);
        temp = ksym;
        count = 0;
        while ( *temp != '\0' && (count <= ncsym) ){
          temp++;
          count++;
	}
        ncsym = count < ncsym ? count : ncsym;

        ncsave = ncsym;

	/* - Do loop excludes leading blanks. */
        temp = ksym;
        while( ( *temp == ' ' ) && (ncsym > 0) ){
          temp++;
          ncsym--;
	}

        if( (ncsym > 0) && (temp != ksym) ) {
          nchar = ncsave - (temp - ksym);
          for(i=0; i<nchar; i++){
            ksym[i] = temp[i];
	  }
          for(i=nchar; i< ncsave; i++) ksym[i] = ' ';
	}

	return;
}

