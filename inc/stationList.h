#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#ifndef TRUE
#define TRUE   1
#endif
#ifndef FALSE
#define FALSE   0
#endif

struct ChannelData{
   int traceNum;
   char KCMPNM[16];
   int NPTS;
   float B;
   float *data;
   float A;
   float USER0;
   float USER1;
   float T[10];
   int year;
   int jday;
   int hour;
   int min;
   int sec;
   int msec;
   char KA[8];
   char KUSER0[8];
   char KUSER1[8];
   char KT09[80];

};


void AddToStationList(int, int, float *);
void UpdateFromStationList(int );
