#include <stdarg.h>
#include <stdio.h>
#include "debug.h"

int smLoadOracleData(int takeEvid, char *specifier,...)
{
  UNUSED(takeEvid);
  UNUSED(specifier);
   printf("This version of SAC does not support database queries.\n");
   return 0;
  
}
/*----------------------------------------------------------------------*/



char *dbGetCropFromPath(void){return 0;}


char *dbGetPrependToPath(void){return 0;}


void dbSetQueryDefaults(void){}

char *dbGetQueryLogin(void) {return 0;}

char *dbGetQueryPasswd(void) {return 0;}

