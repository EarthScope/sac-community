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
   float azimuth;
   char KCMPNM[16];
   int NPTS;
   float B;
   float *data;
   float A;
   float USER0;
   float USER1;
   float USER2;
   float USER3;
   float USER4;
   float USER5;
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
   char KUSER2[8];
   char KT09[80];
   
};

struct StationComp{
   char *name;
   float delta;
   struct StationComp *next;
   int Vfound;
   struct ChannelData V;
   int H1found;   
   struct ChannelData H1;
   int H2found;   
   struct ChannelData H2;
   int flipHoriz;		/* true if axis1-axis2 angle is negative */
   float Axis1Angle;		/* angle of axis 1 from East */
};

#define NULLSET (struct StationComp *) 0

/*  Component identifiers  */
enum Component { VERTICAL, HORIZ1, HORIZ2 };


void matAddToChanSet(int, int, float *);
void matTrimSets(void); /* Delete incomplete or incorrect station sets*/
int matGetCompIndex(struct StationComp *, enum Component);
int matGetNumStationSets(void);
void matFreeChanSetList(void);
void matListStationSets(void);  /* print list of station sets */
int matGetMaxDataLen(void);
float *matGetSeisPntr(enum Component , int , int *);
void matCopyToHeader(enum Component , int , double *);
void matCopyStrings(enum Component , int , char *);
void matCopyHeaderValues(enum Component , int , double *);
void matCopyStringsToHeader(enum Component , int , char *);
void matUpdateFromChanSet(int );
