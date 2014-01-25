#include <setjmp.h>

#ifdef DB_ERROR_HANDLER
   jmp_buf env;
   char dbErrorString[500];
#else
   extern jmp_buf env;
   extern char dbErrorString[];
#endif 
