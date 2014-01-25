#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>

#define DEFINE_STRING_FUN
#include "stringfun.h"
#undef DEFINE_STRING_FUN

/* Concatenate a varying list of input strings, creating new string as result. */
char *stringcat(char *str1, ...)
{
   int i, j;
   char *str;
   char *outstr = (char *)NULL;
   va_list pvar;
   int len;

   if(!str1)return(outstr);

   va_start(pvar, str1);
   len = strlen(str1);
   for (i=1; ;i++) {
      str = (char *) va_arg(pvar, char *);
      if (!str)break;
      len += strlen(str);
   }
   va_end(pvar);

   outstr = (char *) calloc(len +1, sizeof(char));
   va_start(pvar, str1);
   strcpy(outstr,str1);
   for (j=1; j<i;j++)      {
      str = (char *) va_arg(pvar, char *);
      strcat(outstr,str);
   }
   va_end(pvar);

   return(outstr);
}
/* --------------------------------------------------------------------------- */




char *Upstring(char *str)
{
   int i;
   if(!str)return 0;
   for(i=0;i<(int)strlen(str);i++)
      str[i] = toupper(str[i]);
   return str;
}
/* ------------------------------------------------------------------ */





char *CpUpstring(const char *str)
{
   int j;
   static char tmp[100];
   if(!str)return 0;
   strcpy(tmp,str);
   for(j=0;j<(int)strlen(tmp);j++) *(tmp+j) = toupper( *(tmp +j));
   return tmp;
}
/* ------------------------------------------------------------------ */










/* concatenate string2 to string1 after allocating necessary space */
char *strAllocCat(char * string1, char *string2)
{
   int len1, len2;
   if(!string2)return string1;
   len2 = strlen(string2);

   if(!string1){
      string1 = (char *) malloc(len2 + 1);
      strcpy(string1,string2);
      return string1;
   }
   len1 = strlen(string1);
   string1 = (char *) realloc(string1, len1 + len2 + 1);
   strcat(string1, string2);
   return string1;
}
/* ------------------------------------------------------------------ */




