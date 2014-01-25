
#include <config.h>

#ifdef HAVE_MATLAB

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <memory.h>
#include <ctype.h>

#include "dfm.h"
#include "hdr.h"
#include "com.h"
#include "bool.h"

#include "exm.h"     /* used by the getBlackboardVars function */
#include "bbs.h"     /* used by the getBlackboardVars function */
#include "matStructDefs.h"

struct BlackBoardVars *bbListHead = (struct BlackBoardVars *) NULL;
struct BlackBoardVars *bbListTail = (struct BlackBoardVars *) NULL;








void  matAddToBlackboardList(char *nameBuffer, char *valBuffer)
{
   struct BlackBoardVars *new;
   char alpha[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
   
   new = (struct BlackBoardVars *) calloc(1,sizeof(struct BlackBoardVars));
   new->next = (struct BlackBoardVars *) NULL;
   new->stringval = NULL;
   if(bbListHead == (struct BlackBoardVars *) NULL){
      bbListHead = new;
      bbListTail = new;
   }
   else{
      bbListTail->next = new;
      bbListTail = new;
   }

   new->name = (char *) calloc(strlen(nameBuffer) + 1, sizeof(char));
   strcpy(new->name,nameBuffer);

   if(strpbrk(valBuffer,alpha) != (char *) NULL){
      new->stringval = (char *) calloc(strlen(valBuffer) + 1, sizeof(char));
      strcpy(new->stringval,valBuffer);
      new->type = STRING_BBVAL;
   }
   else if (strpbrk(valBuffer, ".") != (char *) NULL){
      sscanf(valBuffer,"%f",&(new->floatval));
      new->type = FLOAT_BBVAL;
   }
   else{
      sscanf(valBuffer,"%d",&(new->intval));
      new->type = INT_BBVAL;
   }

}
/* ---------------------------------------------------------------------------*/








void  matDestroyBlackboardList(void)
{
   struct BlackBoardVars *cur;

   cur = bbListHead;
   while(cur){
      bbListHead = cur->next;
      if(cur->stringval)free(cur->stringval);
      free(cur->name);
      free(cur);
      cur = bbListHead;
   }
   bbListTail = NULL;

}
/* ---------------------------------------------------------------------------*/







void matGetBlackboardVars() /* copy blackboard variables from old SAC internal storage into
			    a linked list where each struc holds the name, type, and value */
{
    char kbbname[MCMSG+1], kbbvalue[MCMSG+1];
    int index;
    int nerr;
    char nameBuffer[100], valBuffer[100];
    char **keys;
    var *v;
    int i;

    cmexm.lbball=TRUE;
    nerr=0;
    
    char **keys = sac_vars_keys(kmbbs.knmbbs);
    i = 0;
    while(keys[i]) {
      v = sac_vars_get_var(kmbbs.knmbbs, keys[i++]);
      if(!strcmp(v->name,"SACERROR"))continue;
      if(!strcmp(v->name,"NUMERROR"))continue;
      if(!strcmp(v->name,"SACNFILES"))continue;
      switch(v->type) {
      case VAR_VALUE:
        sprintf(valBuffer, "%lf", v->value);
        matAddToBlackboardList(v->name, valBuffer);
        break;
      case VAR_STRING:
        matAddToBlackboardList(v->name, v->str);
        break;
      case VAR_INTEGER:
        sprintf(valBuffer, "%d", v->ival);
        matAddToBlackboardList(v->name, valBuffer);
        break;
      }
    }
    return;
}
/* ---------------------------------------------------------------------------*/







void matSetBlackboardVars() /* copy blackboard variables to old SAC internal storage from
			    a linked list where each struc holds the name, type, and value */
{
    struct BlackBoardVars *cur;
    int index;
    int nerr = 0;
    char nameBuffer[100], valBuffer[100];
    int j;

    for(j=0;j<100;j++){
       nameBuffer[j]=' ';
       valBuffer[j]=' ';
    }

    cur=bbListHead;
    while (cur != (struct BlackBoardVars *) NULL ){
       strcpy(nameBuffer,cur->name);
       switch(cur->type){

       case INT_BBVAL:
	   sprintf(valBuffer,"%d",cur->intval);
           break;

       case FLOAT_BBVAL:
	   sprintf(valBuffer,"%f",cur->floatval);
           break;

       case STRING_BBVAL:
	  strcpy(valBuffer,cur->stringval);

       }
       setbbv(nameBuffer, valBuffer, &nerr, 99, 99);
       cur = cur->next;
    }
}
/* ---------------------------------------------------------------------------*/







int matNumBlackBoardVars(void)
{
    struct BlackBoardVars *cur;
    int index;
    cur=bbListHead;
    index = 0;
    while (cur != (struct BlackBoardVars *) NULL ){
       index ++;
       cur = cur->next;
    }

    return index;
}
/* ---------------------------------------------------------------------------*/






struct BlackBoardVars *matGetBBPtr(int index)
{
    struct BlackBoardVars *cur;
    int j;
    cur=bbListHead;
    j = 0;
    while (cur != (struct BlackBoardVars *) NULL ){
       if( j == index)return cur;
       j ++;
       cur = cur->next;
    }

    return (struct BlackBoardVars *) NULL;
}
/* ---------------------------------------------------------------------------*/

#endif /* HAVE_MATLAB */

#ifndef HAVE_MATLAB

void __matBlackBoardVars_undef_symbol() { }

#endif 
