#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dbselect/dbselect.h" 

#include "dbselect/dbDefaults.h"
#include "dbselect/dbBuildSQLstring.h"

#define DB_ERROR_HANDLER
#include "dbselect/dbErrorHandler.h"
#undef DB_ERROR_HANDLER

#include "cssListOps/cssStrucs.h"
#include "cssListOps/dblUserData.h"
#include "cssListOps/dblPublicDefs.h"
#include "cssListOps/cssListOps.h"
#include "cssListOps/dblErrors.h"
#include "dbselect/dbDefaults.h"
#include "smDataIO.h"
#include "sacIO/sacIO.h"



static struct WorkSet *WShead = 0;
static struct WorkSet *WStail = 0;
static struct WorkSet *WScurrent = 0;
static char TmpString[12];

void smDeleteAllWorkSets(void)
{
   struct WorkSet *cur;
   while(WShead){
      cur = WShead;
      WShead = cur->next;
      if(cur->tree)dblDeleteTree(cur->tree);
      free(cur->name);
      free(cur);
   }
   WShead = 0;
   WStail = 0;
   WScurrent = 0;
}
/* -------------------------------------------------------------------- */






struct WorkSet *smGetDefaultWorkset(void)
{
     return WScurrent;
}
/*----------------------------------------------------------------------*/








DBlist smGetDefaultTree(void)
{
  if(!WScurrent)return 0;

  return (void *) WScurrent->tree;
}
/*----------------------------------------------------------------------*/




DBlist smMakeDefaultTree(void)
{
  if(!WScurrent){
     smCreateEmptyWorkset(0);
  }
  if(!WScurrent->tree) WScurrent->tree = dblNewTree(); 
  return WScurrent->tree;
}
/*----------------------------------------------------------------------*/





void smDeleteDefaultTree(void)
{
   if(!WScurrent) return;
   if(!WScurrent->tree) return;
   dblDeleteTree(WScurrent->tree);
   WScurrent->tree = 0;
}
/*----------------------------------------------------------------------*/





void smClearDefaultTree(void)
{
  if(!WScurrent){
     smCreateEmptyWorkset(0);
     return;
  }
  if(!WScurrent->tree){
     WScurrent->tree = dblNewTree();
     return;
  } 
   dblDeleteTree(WScurrent->tree);
   WScurrent->tree = dblNewTree();
}
/*----------------------------------------------------------------------*/



   



int smCreateEmptyWorkset(char *name)
{
   struct WorkSet *fresh;
   fresh = (struct WorkSet *) malloc(sizeof(struct WorkSet) );
   WScurrent = fresh;
   WScurrent->tree = dblNewTree();
   
   if(WStail)
      fresh->index = WStail->index + 1;
   else
      fresh->index = 0;
   if(name){
     fresh->name = (char *) malloc( strlen(name) + 1);
     strcpy(fresh->name,name);
   }
   else{
     fresh->name = (char *) malloc( 12 *sizeof(char) );
     sprintf(fresh->name,"Workset%0d",fresh->index);
   }

   fresh->next = 0;
   if(!WShead){
      WShead = fresh;
      WStail = fresh;
   }
   else{
      WStail->next = fresh;
      WStail = fresh;
   }
   return fresh->index;
}
/*----------------------------------------------------------------------*/








void smListAllWorksets(FILE *unit)
{
   struct WorkSet *cur;
   char data[20];
   cur = WShead;


   while(cur){
      if(cur->tree)
	 strcpy(data,"(Tree Allocated)");
      else
	 strcpy(data,"(No Tree)");

      if(WScurrent == cur)
	 fprintf(unit,"index = %d, name = %s, %s <-- current\n",
		 cur->index,cur->name, data);
      else
	 fprintf(unit,"index = %d, name = %s, %s\n",cur->index,cur->name,data);
      cur = cur->next;
   }
   fprintf(unit,"\n");
}
/*----------------------------------------------------------------------*/




	



int smChangeDefaultWorksetByIndex(int index)
{
   struct WorkSet *cur;
   cur = WShead;
   while(cur){
      if(index == cur->index){
	 WScurrent = cur;
	 return TRUE;
      }
      cur = cur->next;
   }
   return FALSE;
}
/*----------------------------------------------------------------------*/







int smChangeDefaultWorksetByName(char *name)
{
   struct WorkSet *cur;
   cur = WShead;
   while(cur){
      if( !strcmp(name,cur->name) ){
	 WScurrent = cur;
	 return TRUE;
      }
      cur = cur->next;
   }
   return FALSE;
}
/*----------------------------------------------------------------------*/







void smDeleteWorksetByIndex(int index)
{
   struct WorkSet *cur, *prev;
   prev = cur = WShead;

   if(!cur)return;
   if(index == cur->index){  /* matches list head index */
      if(WScurrent == cur)WScurrent = cur->next;
      WShead = cur->next;
      if( cur == WStail)WStail = cur->next;
      if(cur->tree)dblDeleteTree(cur->tree);
      free(cur->name);
      free(cur);
      return;
   }

   while(cur){
      if(index == cur->index){
         if(WScurrent == cur)WScurrent = prev;
         if( cur == WStail)WStail = prev;
	 prev->next = cur->next;
         if(cur->tree)dblDeleteTree(cur->tree);
         free(cur->name);
         free(cur);
         return;
      }
      prev = cur;
      cur = cur->next;
   }
}
/*----------------------------------------------------------------------*/







void smDeleteWorksetByName(char *name)
{
   struct WorkSet *cur, *prev;
   prev = cur = WShead;

   if(!cur)return;
   if(!name)return;
   if(!strcmp(name, cur->name) ){  /* matches list head name */
      if(WScurrent == cur)WScurrent = cur->next;
      WShead = cur->next;
      if( cur == WStail)WStail = cur->next;
      if(cur->tree)dblDeleteTree(cur->tree);
      free(cur->name);
      free(cur);
      return;
   }

   while(cur){
      if(!strcmp(name, cur->name)){
         if(WScurrent == cur)WScurrent = prev;
         if( cur == WStail)WStail = prev;
	 prev->next = cur->next;
         if(cur->tree)dblDeleteTree(cur->tree);
         free(cur->name);
         free(cur);
         return;
      }
      prev = cur;
      cur = cur->next;
   }
}
/*----------------------------------------------------------------------*/





char *smGetDefaultWorksetName(void)
{
   if(!WShead || !WScurrent)return 0;
   strcpy(TmpString, WScurrent->name);
   return TmpString;
}
/*----------------------------------------------------------------------*/

   



int smChangeWorksetName(char* OldName, char* NewName)
{
   struct WorkSet *cur;
   
   if(!OldName || !strlen(OldName) )return 0;
   if(!NewName || !strlen(NewName) )return 0;
   if(!WShead)return 0;
   
   cur = WShead;
   while(cur){
      if(!strcmp(cur->name, NewName)){
         printf("Can't change name: %s already in use!\n",NewName);
         return 0;
      }
      cur = cur->next;
   }
   cur = WShead;
   while(cur){
      if(!strcmp(cur->name, OldName)){
         free(cur->name);
         cur->name = (char*) malloc(strlen(NewName) + 1);
         strcpy(cur->name, NewName);
         return 1;
      }
      cur = cur->next;
   }
   return 0;
}
/* --------------------------------------------------------------------- */   
   
