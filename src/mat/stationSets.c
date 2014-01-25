
#include <config.h>

#ifdef HAVE_MATLAB

#include <string.h>
#include <stdio.h>
#include <math.h>

#include "stationSets.h"
#include "dfm.h"
#include "hdr.h"


#define ANGLETOL 90.0
#ifndef PI
#   define PI M_PI
#endif /* PI */

extern int doAuto;        /* if true auto-analyze data */
extern int reference;     /* pick to use for locating auto-proc window A=1, T0=2, ... */

struct StationComp *head=NULLSET;
static struct StationComp *tail=NULLSET;

/* prototypes for private functions... */
struct StationComp *matNewStationComp(int, int, float *);
char *matStripNulls(char* ,int);
void matUpdateStationComp(struct ChannelData );




int matPickSet(void)
{
   switch(reference) {
      case 1:
         if(*a > -12345)
            return TRUE;
         break;
               
      case 2:
         if(*t0 > -12345)
            return TRUE;
         break;
               
      case 3:
         if(*t1 > -12345)
            return TRUE;
         break;
               
      case 4:
         if(*t2 > -12345)
            return TRUE;
         break;
               
      case 5:
         if(*t3 > -12345)
            return TRUE;
         break;
               
      case 6:
         if(*t4 > -12345)
            return TRUE;
         break;
               
      case 7:
         if(*t5 > -12345)
            return TRUE;
         break;
               
      case 8:
         if(*t6 > -12345)
            return TRUE;
         break;
               
      case 9:
         if(*t7 > -12345)
            return TRUE;
         break;
               
      case 10:
         if(*t8 > -12345)
            return TRUE;
         break;
               
      case 11:
         if(*t9 > -12345)
            return TRUE;
         break;
         
     default:
         return FALSE;
         
   }
   return FALSE;

}



char *matStripNulls(char* string,int len)
{
int j;
for(j=0;j<len;j++)
   if(string[j] == '\0')
      string[j] = ' ';

}



void matUpdateStationComp(struct ChannelData ch)
{
    int j;
    *user0=ch.USER0;		/*BAZ*/
    *user1=ch.USER1;		/*INCIDENCE*/
    *user2=ch.USER2;		/*POLARIZATION*/
    *user3=ch.USER3;            /* AZIMUTH FROM ML EXTIMATE */
    *user4=ch.USER4;            /* P-WAVE PROBABILITY AT AZIMUTH */
    *user5=ch.USER5;            /* WIDTH of 10% PROBABILITY DROP */
    *cmpaz=ch.azimuth;		/*UPDATED COMPONENT AZIMUTH */
    for(j=0;j<10;j++)
       *(t0 + j)=ch.T[j];
    strncpy(ka,ch.KA,8);
    strncpy(kuser0,ch.KUSER0,8);
    strncpy(kuser1,ch.KUSER1,8);
    strncpy(kuser2,ch.KUSER2,8);
    strncpy(kt0,ch.KT09,8);
    strncpy(kt1,ch.KT09+8,8);
    strncpy(kt2,ch.KT09+16,8);
    strncpy(kt3,ch.KT09+24,8);
    strncpy(kt4,ch.KT09+32,8);
    strncpy(kt5,ch.KT09+40,8);
    strncpy(kt6,ch.KT09+48,8);
    strncpy(kt7,ch.KT09+56,8);
    strncpy(kt8,ch.KT09+64,8);
    strncpy(kt9,ch.KT09+72,8);
    return;
}       





void matAddChanToStruc(struct ChannelData *ch, int index, int nlen, float *data)
{
    int j;
    strncpy(ch->KCMPNM,kcmpnm,16);   
    ch->traceNum=index;
    ch->azimuth=*cmpaz;
    ch->NPTS=nlen;
    ch->B=*b;
    ch->data=data;
    ch->A=*a;
    ch->USER0=*user0;
    ch->USER1=*user1;
    ch->USER2=*user2;
    ch->USER3=*user3;
    ch->USER4=*user4;
    ch->USER5=*user5;
    for(j=0;j<10;j++)
       ch->T[j]=*(t0 + j);
    ch->year=*nzyear;
    ch->jday=*nzjday;
    ch->hour=*nzhour;
    ch->min=*nzmin;
    ch->sec=*nzsec;
    ch->msec=*nzmsec;
    strncpy(ch->KA,ka,8);
    strncpy(ch->KUSER0,kuser0,8);
    strncpy(ch->KUSER1,kuser1,8);
    strncpy(ch->KUSER2,kuser2,8);
    strncpy(ch->KT09,kt0,8);
    strncpy(ch->KT09+8,kt1,8);
    strncpy(ch->KT09+16,kt2,8);
    strncpy(ch->KT09+24,kt3,8);
    strncpy(ch->KT09+32,kt4,8);
    strncpy(ch->KT09+40,kt5,8);
    strncpy(ch->KT09+48,kt6,8);
    strncpy(ch->KT09+56,kt7,8);
    strncpy(ch->KT09+64,kt8,8);
    strncpy(ch->KT09+72,kt9,8);
       

}





struct StationComp *matNewStationComp(int index, int nlen, float *data)
{
   struct StationComp *new;

      new=(struct StationComp *) malloc(sizeof(struct StationComp));
      if(new == NULLSET){ /* As long as it is V, or Horiz add it.*/
         fprintf(stderr,"ERROR allocating memory for StationComp struct! \n");
         exit(1);
      }
      new->Vfound=FALSE;
      new->H1found=FALSE;
      new->H2found=FALSE;
      if(*cmpinc == 0.0){
         new->Vfound=TRUE;
         matAddChanToStruc(&(new->V), index,nlen,data);

      }
      else if(*cmpinc == 90.0){
         new->H1found = TRUE;
         matAddChanToStruc(&(new->H1), index,nlen,data);
      }
      else {
         free(new);
         return(NULLSET);
      }
      new->name=malloc(strlen(kstnm)+1);
      strcpy(new->name,kstnm);
      new->name[strlen(kstnm)]='\0';
      
      new->delta=*delta;
      new->next=NULLSET;

      return(new);
}




void matAddToChanSet(int index, int nlen, float *data)
{
   struct StationComp *cur, *new;

   if(doAuto){
      if(!matPickSet()){
         printf("Pick Not Set \n");
         return;
      }
   }


   
   if(head == NULLSET){
      new=matNewStationComp(index,nlen,data);
      if(new == NULLSET)
         return;
      head=new;
      tail=new;
      return;   
   }
   else{ /* See if the new channel belongs in an existing structure element */
      cur=head;
      while(cur != NULLSET){
         
         if(!strcmp(kstnm , cur->name) && *delta == cur->delta){
            if(*cmpinc == 0.0 && cur->Vfound == FALSE){
               matAddChanToStruc(&(cur->V), index,nlen,data);
               cur->Vfound=TRUE;
               return;
            }
            else if((*cmpinc == 90.0) && !cur->H1found){
               matAddChanToStruc(&(cur->H1), index,nlen,data);
               cur->H1found=TRUE;
               return;
            }
            else if((*cmpinc == 90.0) && (cur->H1found) && (!cur->H2found)){
               matAddChanToStruc(&(cur->H2), index,nlen,data);
               cur->H2found=TRUE;
               return;
            }
         
         }
         cur=cur->next;
      }
   }
   /* Need to add one to the list */
   new=matNewStationComp(index,nlen,data);
      if(new == NULLSET)
         return;
   tail->next=new;
   tail=new;
   
   return;

}



void matUpdateFromChanSet(int index)
/* Update SAC header variables from chanset (index) Called after return
   from Matlab processing. */
{
   struct StationComp *cur;
   if(head == NULLSET){
      return;
   }
   
   else{  /* find the correct structure element */
      cur=head;
      while(cur != NULLSET){
         if(index == cur->V.traceNum){
            matUpdateStationComp(cur->V);
            return;
         }
         if(index == cur->H1.traceNum){
            matUpdateStationComp(cur->H1);
            return;
         }
         if(index == cur->H2.traceNum){
            matUpdateStationComp(cur->H2);
            return;
         }
         
         cur=cur->next;
      }
   }
   return;

}


int matCheckComponentOrder(float az1, float az2)
{
   float ang1,ang2;
   float result;
   
   ang1=(90-az1)*PI/180.0;
   ang2=(90-az2)*PI/180.0;
   result = cos(ang1)*sin(ang2) - sin(ang1)*cos(ang2); /* cross product */
   if(result > 0)
      return FALSE;
   else
      return TRUE;
}



int matValidChanSet(struct StationComp *cur)
{
   float axisAngle;
/* Make sure chanset has 3 components and the horizontals are within
   ANGLETOL of being 90 degrees apart. */
   if( !(cur->Vfound && cur->H1found && cur->H2found) ){
       printf("Removing chanset %s: ",cur->name);
       printf("   Not enough components. \n"); 
       return FALSE;
   }
   cur->flipHoriz=matCheckComponentOrder(cur->H1.azimuth,cur->H2.azimuth);
   if(cur->flipHoriz){
      axisAngle = cur->H2.azimuth - cur->H1.azimuth;
      cur->Axis1Angle = 90.0 - cur->H2.azimuth;
      printf("Reverse Orientation ... Rotation angle = %f \n",-cur->Axis1Angle);
   }
   else{
      axisAngle = cur->H1.azimuth - cur->H2.azimuth;
      cur->Axis1Angle = 90.0 - cur->H1.azimuth;
      printf("Normal Orientation  ... Rotation angle = %f \n",-cur->Axis1Angle);
   }
      
     
   if( abs(axisAngle - 90) <= ANGLETOL )
      return TRUE;
      
   printf("Removing chanset %s: \n",cur->name);
   printf("      Horizontal components are not at 90 degrees. \n"); 
   return FALSE;   
      
}




void matTrimSets(void) /* Find which StationComps are valid and delete the rest*/
{
   struct StationComp *cur, *prev;
   cur=head;
   prev=NULLSET;
   while(cur != NULLSET){
      if(matValidChanSet(cur)){
         prev=cur;
         cur=cur->next;
      }
      else{
         if(prev != NULLSET){
            prev->next=cur->next;
            free(cur->name);
            free(cur);
            cur=prev->next;
         }
         else{
            prev=cur;
            cur=cur->next;
            head=cur;
            free(prev->name);
            free(prev);
            prev=NULLSET;
         }
      }
   }
   return;

}



void matFreeChanSetList(void)   /* Release memory associated with station sets */
{
   struct StationComp *cur;
   
   cur=head;
   while(cur != NULLSET){
      head=cur->next;
      free(cur->name);
      free(cur);
      cur=head;
   }
   return;
}


int matGetCompIndex(struct StationComp *station, enum Component comp)
{
   if(comp == VERTICAL)
      return (station->V).traceNum;
   else if(comp == HORIZ1)
      return (station->H1).traceNum;
   else if(comp == HORIZ2)
      return (station->H2).traceNum;
   else
      return -1;

}


int matGetNumStationSets(void)  /* Return the number of complete station sets */
{
   struct StationComp *cur;
   int NumSets=0;
   
   cur=head;
   while(cur != NULLSET){
      NumSets++;
      cur=cur->next;
   }
   return(NumSets);
}


void matListStationSets(void)  /* print names of complete station sets */
{
   struct StationComp *cur;
   cur=head;
   while(cur != NULLSET){
      printf("%s \n",cur->name);
      cur=cur->next;
   }
   return;
}



int matGetMaxDataLen(void)
{
   int MaxLen=0;
   struct StationComp *cur;
   
   
   cur=head;
   while(cur != NULLSET){
      if(MaxLen < (cur->V).NPTS)
         MaxLen=(cur->V).NPTS;
      if(MaxLen < (cur->H1).NPTS)
         MaxLen=(cur->H1).NPTS;
      if(MaxLen < (cur->H2).NPTS)
         MaxLen=(cur->H2).NPTS;
         
      cur=cur->next;
   }
   return(MaxLen);


}

float *matGetSeisPntr(enum Component comp, int index, int *NPTS)
{
   int j=0;
   struct StationComp *cur;
   struct ChannelData *ch;   
   
   cur=head;
   while(cur != NULLSET){
      j++;
      if(j == index){
         switch(comp) {
            case HORIZ1:
               if(cur->flipHoriz)
                  ch=&(cur->H2);
               else
                  ch=&(cur->H1);
               break;
               
            case HORIZ2:
               if(cur->flipHoriz)
                  ch=&(cur->H1);
               else
                  ch=&(cur->H2);
               break;
               
            case VERTICAL:
               ch=&(cur->V);
               break;
               
            default:
               return;
         
         }
         *NPTS=ch->NPTS;
         return(ch->data);
      }
         
      cur=cur->next;
   }
   return( (float *) NULL);

}



void matCopyHeaderValues(enum Component comp, int index, double *HeaderData)
/* copy values from 3-comp struc into a double array which will be passed to Matlab */
{
   int j=0;
   struct StationComp *cur;
   struct ChannelData *ch;   
   
   cur=head;
   while(cur != NULLSET){
      j++;
      if(j == index){
         switch(comp) {
            case HORIZ2:
               if(cur->flipHoriz)
                  ch=&(cur->H1);
               else
                  ch=&(cur->H2);
               break;
               
            case HORIZ1:
               if(cur->flipHoriz)
                  ch=&(cur->H2);
               else
                  ch=&(cur->H1);
               break;
               
            case VERTICAL:
               ch=&(cur->V);
               break;
               
            default:
               return;
         
         }
         HeaderData[0]=cur->delta;
         HeaderData[1]=ch->NPTS;
         HeaderData[2]=ch->B;
         HeaderData[3]=ch->A;
         HeaderData[4]=ch->USER0;
         HeaderData[5]=ch->USER1;
         for(j=0;j<10;j++)
            HeaderData[6+j]=ch->T[j];
         HeaderData[16]=ch->year;
         HeaderData[17]=ch->jday;
         HeaderData[18]=ch->hour;
         HeaderData[19]=ch->min;
         HeaderData[20]=ch->sec;
         HeaderData[21]=ch->msec;
         HeaderData[22]=ch->azimuth;
         HeaderData[23]=cur->Axis1Angle;
         HeaderData[24]=ch->USER2;
         HeaderData[25]=ch->USER3;
         HeaderData[26]=ch->USER4;
         HeaderData[27]=ch->USER5;
         
         return;
      }
         
      cur=cur->next;
   }


}

void matCopyStrings(enum Component comp, int index, char *StringData)
{
   int j=0;
   struct StationComp *cur;
   struct ChannelData *ch;   
   
   cur=head;
   while(cur != NULLSET){
      j++;
      if(j == index){
         switch(comp) {
            case HORIZ2:
               if(cur->flipHoriz)
                  ch=&(cur->H1);
               else
                  ch=&(cur->H2);
               break;
               
            case HORIZ1:
               if(cur->flipHoriz)
                  ch=&(cur->H2);
               else
                  ch=&(cur->H1);
               break;
               
            case VERTICAL:
               ch=&(cur->V);
               break;
               
            default:
               return;
         
         }
         strncpy(StringData,matStripNulls(cur->name,8),8);
         strncpy(StringData+8,matStripNulls(ch->KCMPNM,16),16);
         strncpy(StringData+24,matStripNulls(ch->KA,8),8);
         strncpy(StringData+32,matStripNulls(ch->KUSER0,8),8);
         strncpy(StringData+40,matStripNulls(ch->KUSER1,8),8);
         for(j=0;j<10;j++)
            strncpy(StringData+48+j*8,matStripNulls(ch->KT09+j*8,8),8);
         strncpy(StringData+128,matStripNulls(ch->KUSER2,8),8);
         return;
      }
         
      cur=cur->next;
   }


}



void matCopyToHeader(enum Component comp, int index, double *HeaderData)
{
   int j=0;
   struct StationComp *cur;
   struct ChannelData *ch;   
   
   cur=head;
   while(cur != NULLSET){
      j++;
      if(j == index){
         switch(comp) {
            case HORIZ2:
               if(cur->flipHoriz)
                  ch=&(cur->H1);
               else
                  ch=&(cur->H2);
               break;
               
            case HORIZ1:
               if(cur->flipHoriz)
                  ch=&(cur->H2);
               else
                  ch=&(cur->H1);
               break;
               
            case VERTICAL:
               ch=&(cur->V);
               break;
               
            default:
               return;
         
         }
         cur->delta=HeaderData[0];
         ch->NPTS=HeaderData[1];
         ch->B=HeaderData[2];
         ch->A=HeaderData[3];
         ch->USER0=HeaderData[4];
         ch->USER1=HeaderData[5];
         ch->USER2=HeaderData[24];
         ch->USER3=HeaderData[25];
         ch->USER4=HeaderData[26];
         ch->USER5=HeaderData[27];
         for(j=0;j<10;j++)
            ch->T[j]=HeaderData[6+j];
         ch->year=HeaderData[16];
         ch->jday=HeaderData[17];
         ch->hour=HeaderData[18];
         ch->min=HeaderData[19];
         ch->sec=HeaderData[20];
         ch->msec=HeaderData[21];
         ch->azimuth=HeaderData[22];
         cur->Axis1Angle=HeaderData[23];
         
         return;
      }
         
      cur=cur->next;
   }


}



void matCopyStringsToHeader(enum Component comp, int index, char *StringData)
{
   int j=0;
   struct StationComp *cur;
   struct ChannelData *ch;   
   
   cur=head;
   while(cur != NULLSET){
      j++;
      if(j == index){
         switch(comp) {
            case HORIZ2:
               if(cur->flipHoriz)
                  ch=&(cur->H1);
               else
                  ch=&(cur->H2);
               break;
               
            case HORIZ1:
               if(cur->flipHoriz)
                  ch=&(cur->H2);
               else
                  ch=&(cur->H1);
               break;
               
            case VERTICAL:
               ch=&(cur->V);
               break;
               
            default:
               return;
         
         }
         strncpy(cur->name,StringData,8);
         strncpy(ch->KCMPNM,StringData+8,16);
         strncpy(ch->KA,StringData+24,8);
         strncpy(ch->KUSER0,StringData+32,8);
         strncpy(ch->KUSER1,StringData+40,8);
         strncpy(ch->KUSER2,StringData+128,8);
         for(j=0;j<10;j++)
            strncpy(ch->KT09+j*8,StringData+48+j*8,8);
         return;
      }
         
      cur=cur->next;
   }


}


#endif /* HAVE_MATLAB */

#ifndef HAVE_MATLAB

void __stationSets_undef_symbol() { }

#endif 
