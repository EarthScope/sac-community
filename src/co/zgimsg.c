/** 
 * @file   zgimsg.c
 * 
 * @brief  Get 
 * 
 */

#include "co.h"

/** 
 * Concatentate strings together
 * 
 * @param argc 
 *    Length of \p argv
 * @param argv 
 *    String to concatenate
 * @param mess 
 *    Destination string
 * @param messlen 
 *    Length of \p mess
 *
 * @date 08/01/84   Under development--D. Trimmer
 * @date 08/01/84   Tested--D. Trimmer
 *
 * @bug Only used by main/main()
 *
 */

void
zgimsg(int    argc, 
       char **argv,
       char  *mess,
       int    messlen) {

	int i;			/* index for counting parameters */
	int j;			/* index for parameter strings */
	int k;			/* character counter */
	char *pc;		/* character pointer */
 
	k=0;					/* length counter */
	pc = *(++argv);			        /* skip program name */

	for (i=1;i<argc;++i,pc= *(++argv)) {
	  for (j=0;*pc!='\0' && k<messlen;++j,++k)
	    mess[k] = *(pc++);	/* copy parameters */
	  if (k == messlen) {
	    printf ("warning:  command string too long.\n");
	    break;
	  }
	  mess[k++] = ' ';		/* parameter delimiter */
	}
 
	return;
}
 
 
