
#include "suds/sudsDataConversions.h"


void Convert2(void *in)
{
   char tmp;
   char *In;
   In = (char *) in;
   tmp   = In[1];
   In[1] = In[0];
   In[0] = tmp;
}
/* ------------------------------------------------------- */






void Convert4(void *in)
{
   char tmp;
   char *In;
   char *p1, *p2;
   int j;
   In = (char *) in;
   for(j=0;j<2;j++){
      p1 = In + j;
      p2 = In + 3 - j;
      tmp   = *p1;
      *p1 = *p2;
      *p2 = tmp;
   }
}
/* ------------------------------------------------------- */






void Convert8(void *in)
{
   char tmp;
   char *In;
   char *p1, *p2;
   int j;
   In = (char *) in;
   for(j=0;j<4;j++){
      p1 = In + j;
      p2 = In + 7 - j;
      tmp   = *p1;
      *p1 = *p2;
      *p2 = tmp;
   }
}
/* ------------------------------------------------------- */




void ConvertPZArray(SUDS_CALIBR *PZA, int npts)
{
   int j;

   for(j=0;j<npts;j++){
      Convert4(&(PZA[j].pole.cr) );
      Convert4(&(PZA[j].pole.ci) );
      Convert4(&(PZA[j].zero.cr) );
      Convert4(&(PZA[j].zero.ci) );
   }
}
/* ------------------------------------------------------- */
      



