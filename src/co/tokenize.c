/** 
 * @file   tokenize.c
 * 
 * @brief  Tokenize a character string
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "co.h"

#include "errors.h"

/** 
 * Tokenize a character string. Tokens are seperated by spaces, 
 *     tabs, or newlines
 * 
 * @param argv 
 *    Output tokenized character array
 * @param argc 
 *    Length of \p argv - Number of tokens
 * @param linein 
 *    Input line to be split
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_OUT_OF_MEMORY
 *
 */
void 
tokenize(char ***argv, 
	 int    *argc, 
	 char   *linein, 
	 int    *nerr) {

  char *temp, *token;
  int i;

  *nerr = 0;
  *argc = 0;


  if((temp = malloc(strlen(linein)+1)) == NULL){
    *nerr = ERROR_OUT_OF_MEMORY;
    return;
  }
  
  strcpy(temp, linein);

  token = strtok(temp, " \t\n\0");
  if( token != NULL ) (*argc)++;

  while( (token = strtok(NULL, " \t\n\0")) != NULL ) (*argc)++;

  if( *argc > 0 ){
    if((*argv = malloc(*argc * sizeof(char *))) == NULL){
      *nerr = ERROR_OUT_OF_MEMORY;
      goto ERROR;
    }else{
      (*argv)[0] = strtok(linein, " \t\n\0");
      for( i=1; i<*argc; i++ ){
        (*argv)[i] = strtok(NULL, " \t\n\0");
      }
    }

  }
  
 ERROR:
  free(temp);

  return;

}
