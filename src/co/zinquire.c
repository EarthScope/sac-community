/** 
 * @file   zinquire.c
 * 
 * @brief  Check for a file's existance
 * 
 */
#include <stdlib.h>
#include <string.h>
#include "unistdx.h"

#include "co.h"
#include "bot.h"
#include "bool.h"

char * rstrip(char *str);

/** 
 * Inquire about the existance of a disk file
 * 
 * @param kname 
 *    Name of the file
 * @param lexist 
 *    - TRUE if the file exists
 *    - FALSE if the file does not exist
 *
 */
void 
zinquire(char *kname, 
         int  *lexist) {

    char *tok;

    *lexist = FALSE;
    if(!kname || strlen(kname) == 0) {
        *lexist = FALSE;
        return;
    }
	tok = strdup(kname);
    tok = rstrip(tok);
    if(tok) {
        *lexist = (access(tok, F_OK ) == 0) ? TRUE : FALSE;
        free(tok);
        tok = NULL;
    } 
	return;
}

