/** 
 * @file   non_num_com.c
 * 
 * @brief  Check if a command should not not numeric arguments
 * 
 */

#include <string.h>

#include "cpf.h"
#include "bool.h"
#include "debug.h"

/** 
 * Check the current command against a list of commands which
 *    do not have numeric arguments.  They should not convert
 *    numeric strings to to numbers
 * 
 * @param command 
 *    Command to check
 * @param command_s 
 *    Length of \p command
 * 
 * @return 
 *    - TRUE if a command matches a command with no numeric arguments
 *    - FALSE if a command does not match a command with no numeric arguments
 *
 */
int 
non_num_com(char *command, 
	    int   command_s) {
  UNUSED(command_s);

  if(     memcmp(command,"r "      ,2) == 0) return TRUE;
  else if(memcmp(command,"read "   ,5) == 0) return TRUE;
  else if(memcmp(command,"rh "     ,3) == 0) return TRUE;
  else if(memcmp(command,"readhdr ",8) == 0) return TRUE;
  else if(memcmp(command,"readcss ",8) == 0) return TRUE;
  else if(memcmp(command,"rcss "   ,5) == 0) return TRUE;
  else if(memcmp(command,"merge "  ,6) == 0) return TRUE;
  else if(memcmp(command,"setbb "  ,6) == 0) return TRUE;
  else if(memcmp(command,"message" ,7) == 0) return TRUE;

  return FALSE;  

}

