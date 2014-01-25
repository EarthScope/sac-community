#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cssListOps/dblErrors.h"
#include "debug.h"

static char *ErrorMessage=0;
static int NumErrors;
int * registers = (int *)dblJmpBuf;


void
dblErrorFree() {
  FREE(ErrorMessage);
}

void dblClearErrorList(void)
{
  char header[] = "Summary of current errors:\n";
  if(ErrorMessage)free(ErrorMessage);
  ErrorMessage = (char *) malloc(strlen(header) +1);
  strcpy(ErrorMessage,header);
  NumErrors = 0;
}
/*----------------------------------------------------------------------*/






void dblSetError(int Severe, const char * string)
{
  int errmsgLen = 0;
  int j;

  if(string){
    if(ErrorMessage)errmsgLen = strlen(ErrorMessage);
    ErrorMessage = (char *) realloc(ErrorMessage, 
				    (errmsgLen + strlen(string) +2) * sizeof(char) );
    if ( !errmsgLen )
        strcpy(ErrorMessage,string);
    else
        strcat(ErrorMessage,string);
    strcat(ErrorMessage,"\n");
  }
  NumErrors++;
  if(!Severe) return;

/* If any of first 8 registers are non-zero then setjmp was called with dblJmpBuf,  */
/* so it is safe to call longjmp. Otherwise just return. */
  for(j=0;j<8;j++)if(registers[j])longjmp(dblJmpBuf,1);

  return;
}
/*----------------------------------------------------------------------*/






char *dblGetErrorMessage(void)
{
  return ErrorMessage;
}




int dblGetNumErrors(void)
{
  return NumErrors;
}
/*----------------------------------------------------------------------*/

