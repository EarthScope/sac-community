/** 
 * @file   xboec.c
 * 
 * @brief  Execute BOEC
 * 
 */

#include <string.h>

#include "amf.h"
#include "bom.h"
#include "exm.h"
#include "cpf.h"

/** 
 * Execute the parameter setting command "BOEC".  This command controls
 *   certain error conditions that can occur during binary operations.
 * 
 * @param nerr 
 *   Error Return Flag
 *   - 0 on Success
 *
 * @date   820817:  Changed to newest set of parsing and checking functions.
 * @date   810723:  Original version.
 *
 */
void 
xboec(int *nerr) {

	int index;

	*nerr = 0;

L_1000:
	if( lcmore( nerr ) ){

		/* -- "NPTS condition":  
		   set control for unequal number of data points. */
		if( lklist( "N$",3, (char*)kmexm.kectp,9, 
			    cmexm.nectp, &index ) ){
			strcpy( kmbom.kecnpt, kmexm.kectp[index - 1] );

		}
		/* "DELTA condition":  
		   set control for sampling interval mismatch. */
		else if( lklist( "D$",3, (char*)kmexm.kectp, 9, 
				 cmexm.nectp, &index ) ){
			strcpy( kmbom.kecdel, kmexm.kectp[index - 1] );

		}
		else{
			cfmt( "ILLEGAL OPTION:",17 );
			cresp();

		}
		goto L_1000;

	}

	return;
}

static buffer* sac_binary_file_list = NULL;

buffer *buffer_new();
void buffer_set_format(buffer *b, char c);
void buffer_append(buffer *b, void *p, int n);
void *buffer_get(buffer *b, int i);
void buffer_free(buffer *b);

sac *sacread(char *file);

sac *
bflget(string_list *list, int i) {
  sac *s;

  if(!sac_binary_file_list) {
    sac_binary_file_list = buffer_new();
    buffer_set_format(sac_binary_file_list, 'p');
  }

  if(i >= sac_binary_file_list->len) {
    /* Read in File */
    if(i >= string_list_length(list)) {
      return NULL;
    }
    if(!(s = sacread(string_list_get(list, i)))) {
      return NULL;
    }
    buffer_append(sac_binary_file_list, &s, 1);
  }
  s = buffer_get(sac_binary_file_list, i);

  return s;
}


void
bflclear() {

  if(sac_binary_file_list) {
    buffer_free(sac_binary_file_list);
  }
  sac_binary_file_list = NULL;
}
