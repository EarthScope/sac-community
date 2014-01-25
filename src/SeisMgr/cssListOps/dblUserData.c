#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <math.h>

#include "bool.h"

#include "cssListOps/dblErrors.h"
#include "smMemory/smMemory.h"
#include "cssListOps/cssListStrucs.h"
#include "cssListOps/dblPublicDefs.h"

#define UD_LIST (userData *)
#define NULL_UD_LIST UD_LIST NULL


/* +++++++++++++++ PRIVATE FUNCTION PROTOTYPES FOR THIS FILE ++++++++++++++ */
userData *dblCreateUserDataStruc(DBlist list);
void dblDestroyUserDataStruc(DBlist list);
static void dblFreeUserDataComment(DBlist list);
static void dblFreeMatrix(ComplexFloat **matrix, int Nrows);
static Matrix *dblAddUserDataMatrixStruc(DBlist list);
static void dblDestroyUserDataMatrixData(DBlist list);
static void dblDestroyUserDataDataCommentList(DBlist list);



/* ++++++++++++++ END FUNCTION PROTOTYPES FOR THIS FILE ++++++++++++ */





/* +++++++++++++++++ USERDATA ACCESS FUNCTIONS +++++++++++++++++++ */

/* The UserData struc contains 3 components. The first component is a
   string which is expected to be a description of the contents and
   comments about the data stored in the UserData struct.

   The second main component is a Matrix struct. This struct holds 
   the number of rows and columns, a pointer array of type ComplexFloat,
   and a comment string.

   The last component is a DataComment struct. This is a linked list where
   each element of the list is a struct containing a pointer to a ComplexFloat,
   the length of the ComplexFloat array, an associated comment, a SacUserNum,
   and a pointer to the next element in the list.
*/



/* ................ UserData struct manipulation functions .............. */

/* This function is not called directly by users. Instead, it is called by
   the access functions that add to the UserData struct if they determine
   that there is no UserData struct in the current tree. A userData struct
   is not automatically created when a tree is created so this function is
   not called by NewTree.
*/
userData *dblCreateUserDataStruc(DBlist list)
{
   userData               *newudPntr;
   struct CSStree *tree = (struct CSStree *) list;

   newudPntr = UD_LIST smMalloc(sizeof(userData));
   if(newudPntr == NULL_UD_LIST){
      dblSetError(1, "ERROR: Could not allocate User Data struct in  dblCreateUserDataStruc.\n");
      return (userData *) NULL;
   }

   newudPntr->dataComment = (DataComment *) NULL;
   newudPntr->matrix = (Matrix *) NULL;
   newudPntr->comment = (char *) NULL;
   tree->UserData = newudPntr;
   return /* (DBtable) */ newudPntr;
}
/* --------------------------------------------------------------- */





/* UserData structures must be destroyed when their tree is destroyed.
   This function accomplishes that task. It is not available to users.
*/
void dblDestroyUserDataStruc(DBlist list)
{
   struct CSStree *tree = (struct CSStree *) list;

   if(!tree)return;
   if(!tree->UserData)return;
   if(tree->UserData->comment)dblFreeUserDataComment(list);
   if(tree->UserData->matrix)dblDestroyUserDataMatrixData(list);
   if(tree->UserData->dataComment)dblDestroyUserDataDataCommentList(list);
   smFree(tree->UserData);
   tree->UserData = (userData *)NULL;
   return;
}
/* --------------------------------------------------------------- */
/* .............. End UserData struct manipulation functions ............ */





/* ................ UserData Comment access functions .............. */
  /* There are 2 functions to access UserData Comment strings; one to set
     the string, and one to get a copy of its contents. These follow.
  */
void dblSetUserDataComment(DBlist list, const char *comment)
{
   struct CSStree *tree = (struct CSStree *) list;

   if(!tree){
      dblSetError(1, "ERROR: Null Tree struc in dblSetUserDataComment.\n");
      return;
   }
   
   if(!comment) return;

   if(!tree->UserData) {
       tree->UserData = dblCreateUserDataStruc(list);   
   }
   if(tree->UserData) {
       tree->UserData->comment = (char *) smRealloc(tree->UserData->comment,
                                                    strlen(comment) + 1);
       strcpy(tree->UserData->comment,comment);
   }
   return;
}
/* --------------------------------------------------------------- */







/* Note that this function returns a copy of the comment in UserData
   Thus it is the user's responsibility to free memory when no longer
   required.
*/
char * dblGetUserDataComment(DBlist list)
{
   struct CSStree *tree = (struct CSStree *) list;
   char *string;

   if(!tree)return (char *) NULL;
   if(!tree->UserData)return (char *) NULL;
   if(!tree->UserData->comment)return (char *) NULL;

   string = (char *) smMalloc(strlen(tree->UserData->comment) + 1);
   strcpy(string, tree->UserData->comment);
   return string;
   
}
/* --------------------------------------------------------------- */



/* For purposes of list management a routine is required to delete a
   comment string. This function is not avaliable to users.
*/
static void dblFreeUserDataComment(DBlist list)
{
   struct CSStree *tree = (struct CSStree *) list;

   if(!tree)return;
   if(!tree->UserData)return;
   if(!tree->UserData->comment)return;
   smFree(tree->UserData->comment);

}
/* --------------------------------------------------------------- */
/* .............. End UserData Comment access functions ........... */






/* .............  UserData Matrix functions    ............... */

/*Existing Matrix structures must be deleted when a new one is being
  created in the same UserData struct or when the UserDataStruct is
  being destroyed. This function is used for that purpose. It is not
  available to users. 
*/
static void dblFreeMatrix(ComplexFloat **matrix, int Nrows)
{
   int j;

   if(!matrix){
      dblSetError(1, "ERROR: Null matrix struc in dblFreeMatrix.\n");
      return;
   }

   if(Nrows < 1) return;
   for ( j=0;j<Nrows;j++)
      if(matrix[j]) smFree(matrix[j]);
   
   smFree(matrix);
   return;

}
/* --------------------------------------------------------------- */






/* This function adds an empty UserData matrix structure to UserData.
   It is called when a user attempts to set a matrix, but is not directly
   available to users.
*/
static Matrix *dblAddUserDataMatrixStruc(DBlist list)
{
   struct CSStree *tree = (struct CSStree *) list;

   if(!tree){
      dblSetError(1, "ERROR: Null Tree struc in dblAddUserDataMatrixStruc.\n");
      return (Matrix *)NULL;
   }
   

   if(!tree->UserData) tree->UserData = dblCreateUserDataStruc(list);   
   tree->UserData->matrix = (Matrix *) smCalloc(1, sizeof(Matrix) );
   tree->UserData->matrix->matrix = (ComplexFloat **) NULL;
   tree->UserData->matrix->comment = (char *) NULL;

   return tree->UserData->matrix;
}
/* --------------------------------------------------------------- */




/* This function is called when the UserData struct is being destroyed.
   It is not directly available to users.
*/
static void dblDestroyUserDataMatrixData(DBlist list)
{
   struct CSStree *tree = (struct CSStree *) list;

   if(!tree)return;
   if(!tree->UserData) return;
   if(!tree->UserData->matrix) return;
   if(tree->UserData->matrix->matrix){
      if(tree->UserData->matrix->nrows > 0)
         dblFreeMatrix(tree->UserData->matrix->matrix, 
		       tree->UserData->matrix->nrows);

   }
   if(tree->UserData->matrix->comment)smFree(tree->UserData->matrix->comment);
   smFree(tree->UserData->matrix);
   tree->UserData->matrix = (Matrix *)NULL;
   return;
}
/* --------------------------------------------------------------- */
   





/* This is a user function that allows the user to set a comment string into
   the UserData Matrix struct.
*/
void dblSetUserDataMatrixComment(DBlist list, const char *comment)
{
   struct CSStree *tree = (struct CSStree *) list;

   if(!tree){
      dblSetError(1, "ERROR: Null Tree struc in dblSetUserDataMatrixComment.\n");
      return;
   }
   
   if(!comment) return;

   if(!tree->UserData) tree->UserData = dblCreateUserDataStruc(list);
   if(tree->UserData) {
       if(!tree->UserData->matrix) tree->UserData->matrix = 
                                       dblAddUserDataMatrixStruc(list);
       if(tree->UserData->matrix) {
           if(tree->UserData->matrix->comment) smFree(tree->UserData->matrix->comment);
           tree->UserData->matrix->comment = (char *) smMalloc( strlen(comment) + 1);
           strcpy(tree->UserData->matrix->comment,comment);
       }

   }

   return;
}
/* --------------------------------------------------------------- */






/* This is a user function that allows the user to get a copy of the comment 
   string from the UserData Matrix struct. The user must free them memory
   when no longer required.
*/
char * dblGetUserDataMatrixComment(DBlist list)
{
   struct CSStree *tree = (struct CSStree *) list;
   char *string;

   if(!tree)return (char *) NULL;
   if(!tree->UserData)return (char *) NULL;
   if(!tree->UserData->matrix)return (char *) NULL;
   if(!tree->UserData->matrix->comment)return (char *) NULL;

   string = (char *) smMalloc(strlen(tree->UserData->matrix->comment) + 1);
   strcpy(string, tree->UserData->matrix->comment);
   return string;
   
}
/* --------------------------------------------------------------- */






/* This user function copies the matrix into a UserData Matrix struct
   creating the struct if necessary. If the struct exists and has data,
   the old data is overwritten. The contents of the UserData comment struct
   are not altered by this function.
*/
void dblSetUserDataMatrixData(DBlist list, int Nrows, int Ncols, 
			      ComplexFloat **matrix)
{
   struct CSStree *tree = (struct CSStree *) list;
   int j, k;

   if(!tree){
      dblSetError(1, "ERROR: Null Tree struc in dblSetUserDataMatrixData.\n");
      return;
   }
   
   if(!matrix) return;
   if(Nrows < 1 ) return;
   if(Ncols < 1 ) return;

   if(!tree->UserData) tree->UserData = dblCreateUserDataStruc(list);
   if(tree->UserData) {
       if(!tree->UserData->matrix) tree->UserData->matrix = 
                                       dblAddUserDataMatrixStruc(list);
       
       if(tree->UserData->matrix->matrix){
           dblFreeMatrix(tree->UserData->matrix->matrix, Nrows);
       }
       tree->UserData->matrix->matrix = (ComplexFloat **) 
                smCalloc(1,Nrows * sizeof(ComplexFloat *) );
       if(tree->UserData->matrix) {
           for(j=0;j<Nrows;j++){
               tree->UserData->matrix->matrix[j] = (ComplexFloat *) 
                   smMalloc(Ncols * sizeof(ComplexFloat) );
               for(k=0;k<Ncols;k++)
                   tree->UserData->matrix->matrix[j][k] = matrix[j][k];
           }
           tree->UserData->matrix->nrows = Nrows;
           tree->UserData->matrix->ncols = Ncols;
       }
   }
   return;
}
/* --------------------------------------------------------------- */







/* This user function returns a pointer to a copy of the Matrix stored in
   the UserData struct. The input arguments Nrows and Ncols are updated as well.
   It is the user's responsibility to destroy the copy when it is no longer
   needed. 
*/
ComplexFloat **dblGetUserDataMatrixData(DBlist list, int *Nrows, int *Ncols)
{
   struct CSStree *tree = (struct CSStree *) list;
   int j, k;
   ComplexFloat ** matrix;

   if(!tree){
      dblSetError(1, "ERROR: Null Tree struc in dblGetUserDataMatrixData.\n");
      return (ComplexFloat **)NULL;
   }
   
   if(!tree->UserData) return (ComplexFloat ** ) NULL;
   if(!tree->UserData->matrix) return (ComplexFloat ** ) NULL;
   if(!tree->UserData->matrix->matrix) return (ComplexFloat ** ) NULL;
   if(tree->UserData->matrix->nrows < 1 ) return (ComplexFloat ** ) NULL;
   if(tree->UserData->matrix->ncols < 1 ) return (ComplexFloat ** ) NULL;

   *Nrows = tree->UserData->matrix->nrows;
   *Ncols = tree->UserData->matrix->ncols;

   matrix = (ComplexFloat **)smCalloc(1,(*Nrows) * sizeof(ComplexFloat *) );
   
   for(j=0;j<*Nrows;j++){
      matrix[j] = (ComplexFloat *) smMalloc((*Ncols) * sizeof(ComplexFloat) );
      for(k=0;k<*Ncols;k++)
         matrix[j][k] = tree->UserData->matrix->matrix[j][k];
   }

   return matrix;
}
/* --------------------------------------------------------------- */
/* .............  End UserData Matrix functions    ............... */





/* .............  UserData DataComment functions    ............... */
/* UserData DataComment is a linked list of structures, each containing
   a pointer to ComplexFloat, a long for the length of the array of
   ComplexFloats, and an associated comment. The functions in this section
   provide means of setting an array of ComplexFloats, setting a comment,
   replacing either the numeric or string data given a pointer to the
   structure, deleting a structure, getting the next pointer to a structure,
   getting a pointer to the structure whose comment matches an input
   string, and deleting the entire list of DataComment structures.
*/



/* This function creates a new DataComment structure and adds it to the
   end of the list attached to the current tree (DBlist list). It also
   returns a void * pointer to the DataComment structure.
*/
DBdataComm dblCreateDataComment(DBlist list)
{
   DataComment *newDCpntr, *curDCpntr;
   struct CSStree *tree = (struct CSStree *) list;

   if(!tree){
      dblSetError(1, "ERROR: Null Tree struc in dblCreateDataComment.\n");
      return (void *)NULL;
   }
   if(!tree->UserData) tree->UserData = dblCreateUserDataStruc(list);

   newDCpntr = (DataComment *) smMalloc( sizeof(DataComment) );
   newDCpntr->data = (ComplexFloat *)NULL;
   newDCpntr->comment = (char *)NULL;
   newDCpntr->SacUserNum = -1;
   newDCpntr->reference = 0;

   newDCpntr->prev = (DataComment *)NULL;
   newDCpntr->next = (DataComment *)NULL;
   if(tree->UserData) {
       if(!tree->UserData->dataComment){ /* this is the first in the list */
           tree->UserData->dataComment = newDCpntr;
       }
       else{
           curDCpntr = tree->UserData->dataComment;
           while(curDCpntr->next)curDCpntr = curDCpntr->next;
           curDCpntr->next = newDCpntr;
           newDCpntr->prev = curDCpntr;
       }
   }
   return (void *) newDCpntr;
}
/* --------------------------------------------------------------- */




/* This function deletes the DataComment structure pointed to by DCpntr
   from the tree pointed to by (DBlist list).
*/
void dblDeleteUserDataComStruct(DBdataComm DCpntr, DBlist list)
{
   DataComment *pntr = (DataComment *) DCpntr;
   struct CSStree *tree = (struct CSStree *) list;

   if(!tree){
      dblSetError(1, "ERROR: Null Tree struc in dblDeleteUserDataComStruct.\n");
      return;
   }
   if(!pntr)return;
   if(!tree->UserData->dataComment)return;

   if(!pntr->prev){     /* head of list */
      tree->UserData->dataComment = pntr->next;
      if(pntr->next){
	 pntr->next->prev = (DataComment *)NULL;
      }
      else
	 tree->UserData->dataComment = /*(void *)*/NULL;
   }
   else{
      pntr->prev->next = pntr->next;
   }

   if(pntr->data)smFree(pntr->data);
   if(pntr->comment)smFree(pntr->comment);
   smFree(pntr);
   return;
}
/* --------------------------------------------------------------- */





/* This function returns a void * pointer to the next DataComment structure
   after DCpntr. DCpntr must point to an existing DataComment struct, and should
   have been obtained by a call to dblGetNextDataComment, dblCreateDataComment
   or dblGetMatchingDataComment. If DCpntr is NULL, then the first 
   DataComment structure in (DBlist list) is returned. If DCpntr points to
   the last DataComment structure, then NULL is returned. If there is no
   list or no UserData, then NULL is returned.
*/
DBdataComm dblGetNextDataComment(DBdataComm DCpntr, DBlist list)
{
   DataComment *pntr = (DataComment *) DCpntr;
   struct CSStree *tree = (struct CSStree *) list;

   if(!tree){
      dblSetError(1, "ERROR: Null Tree struc in dblGetNextDataComment.\n");
      return (void *)NULL;
   }
   if(!tree->UserData) return (void *)NULL;
   if(!tree->UserData->dataComment) return (void *)NULL;

   if(!pntr) return (void *) tree->UserData->dataComment;
   return (void *) pntr->next;
}
/* --------------------------------------------------------------- */





/* This function returns a void * pointer to a UserData DataComment structure */
/* which has a comment string that matches comment. It starts the search at */
/* the beginning of the DataComment list if DCpntr is NULL or else at the */
/* DataComment structure immediately following DCpntr. If it does not find a */
/* match, it returns a NULL pointer. If DCpntr is NULL on input and the  */
/* (DBlist list) pointer is NULL it is a fatal error. */
DBdataComm dblGetNextMatchingDataComment(DBdataComm DCpntr, DBlist list, 
					 char * comment)
{
   DataComment *pntr = (DataComment *) DCpntr;
   struct CSStree *tree = (struct CSStree *) list;

   
   if(!tree){
      dblSetError(1, "ERROR: Null Tree struc in dblGetNextMatchingDataComment.\n");
      return (void *)NULL;
   }
   if(!tree->UserData) return (void *)NULL;
   if(!tree->UserData->dataComment) return (void *)NULL;
   if(!comment) return (void *)NULL; 
   if(!pntr) pntr = tree->UserData->dataComment;
   else pntr = pntr->next;
   while(pntr){
      if(pntr->comment && !strcmp(pntr->comment, comment) ) return (void *)pntr;
      pntr = pntr->next;
   }
   return (void *)NULL;
}
/* --------------------------------------------------------------- */





/* This function returns a pointer to the next DataComment struct that has an index */
/* matching index. If none is found it returns a NULL pointer.  */
DBdataComm dblGetNextDCMatchingIndex(DBdataComm DCpntr, DBlist list, int index) 
{
   DataComment *pntr = (DataComment *) DCpntr;
   struct CSStree *tree = (struct CSStree *) list;

   
   if(!tree){
      dblSetError(1, "ERROR: Null Tree struc in dblGetNextDCMatchingIndex.\n");
      return (void *)NULL;
   }
   if(!tree->UserData) return (void *)NULL;
   if(!tree->UserData->dataComment) return (void *)NULL;
   if(!pntr) pntr = tree->UserData->dataComment;
   else pntr = pntr->next;
   while(pntr){
      if(pntr->reference == index) return (void *) pntr;
      pntr = pntr->next;
   }
   return (void *) NULL;
}
/* --------------------------------------------------------------- */




/* Set the reference of a DataComment struct to index. */
void dblSetDCIndex(DBdataComm DCpntr, int index) 
{
   DataComment *pntr = (DataComment *) DCpntr;

   if(!pntr)return;
   pntr->reference = index;
   return;
}
/* --------------------------------------------------------------- */




/* Set the SacUserNum of a DataComment struct to index. */
void dblSetSacUserNum(DBdataComm DCpntr, int index) 
{
   DataComment *pntr = (DataComment *) DCpntr;

   if(!pntr)return;
   pntr->SacUserNum = index;
   return;
}
/* --------------------------------------------------------------- */





/* Get the reference of a DataComment struct. */
int dblGetDCIndex(DBdataComm DCpntr) 
{
   DataComment *pntr = (DataComment *) DCpntr;

   if(!pntr)return 0;
   return pntr->reference;
}
/* --------------------------------------------------------------- */




/* Get the SacUserNum of a DataComment struct. */
int dblGetSacUserNum(DBdataComm DCpntr) 
{
   DataComment *pntr = (DataComment *) DCpntr;

   if(!pntr)return 0;
   return pntr->SacUserNum;
}
/* --------------------------------------------------------------- */



/* Set the DataType of a DataComment struct. */
void dblSetDCType(DBdataComm DCpntr, enum dataType type) 
{
   DataComment *pntr = (DataComment *) DCpntr;

   if(!pntr)return;
   pntr->type = type;
   return;
}
/* --------------------------------------------------------------- */




/* Get the datatype of a DataComment struct. */
enum dataType dblGetDCType(DBdataComm DCpntr) 
{
   DataComment *pntr = (DataComment *) DCpntr;

   if(!pntr)return dblREAL;
   return pntr->type;
}
/* --------------------------------------------------------------- */




/* 
This function returns a pointer to the first DataComment struct that has an index 
matching index and a SacUserNum matching Num. If none is found it returns a NULL 
pointer.  
*/
DBdataComm dblGetNextDCSacUserMatch(DBlist list, int index, int Num) 
{
   DataComment *pntr;
   struct CSStree *tree = (struct CSStree *) list;

   
   if(!tree) return 0;
   if(!tree->UserData) return 0;
   if(!tree->UserData->dataComment) return 0;
   
   pntr = tree->UserData->dataComment;
   while(pntr){
      if(pntr->reference == index && pntr->SacUserNum == Num) return (void *) pntr;
      pntr = pntr->next;
   }
   return 0;
}
/* --------------------------------------------------------------- */






      

/* This function adds a comment to the DataComment struct pointed to by
   DCpntr. DCpntr must point to an existing DataComment struct, and should
   have been obtained by a call to dblGetNextDataComment, dblCreateDataComment
   or dblGetMatchingDataComment. 
*/
void dblSetUserDataComComment(DBdataComm DCpntr, const char *comment)
{
   DataComment *pntr = (DataComment *) DCpntr;

   if(!pntr)return;
   if(!comment) return;
   pntr->comment = (char *) smRealloc(pntr->comment, strlen(comment) +1);
   strcpy(pntr->comment,comment);
   return;
}
/* --------------------------------------------------------------- */





/* This function returns a copy of the comment in the UserData DataComment */
/* structure pointed-to by DCpntr. If there is no comment or if the pointer */
/* is NULL it returns a (char *)NULL. */
char *dblGetUserDataComComment(DBdataComm DCpntr)
{
   DataComment *pntr = (DataComment *) DCpntr;
   char * comment;
   if(!pntr)return (char *)NULL;
   if(!pntr->comment) return (char *)NULL;
   comment = (char *) smMalloc( strlen(pntr->comment) +1);
   strcpy(comment, pntr->comment);
   return comment;
}
/* --------------------------------------------------------------- */






/* This function adds data to the DataComment struct pointed to by
   DCpntr. DCpntr must point to an existing DataComment struct, and should
   have been obtained by a call to dblGetNextDataComment, dblCreateDataComment
   or dblGetMatchingDataComment. Npts must be > 0.
*/
void dblSetUserDataComData(DBdataComm DCpntr, const ComplexFloat *data, int Npts)
{
   int j;
   DataComment *pntr = (DataComment *) DCpntr;

   if(!pntr)return;
   if(!data) return;
   if(Npts < 1)return;
   pntr->data = (ComplexFloat *) smRealloc(pntr->data, Npts * sizeof(ComplexFloat) );
   pntr->dataLen = Npts;
   for(j=0;j<Npts;j++) pntr->data[j] =  data[j];
   return;
}
/* --------------------------------------------------------------- */





/* This function returns a copy of the data associated with the UserData
   DataComment structure pointed to by DCpntr. If there is no data, the
   function returns (ComplexFloat *)NULL.
*/
ComplexFloat *dblGetUserDataComData(DBdataComm DCpntr, int *Npts)
{
   int j;
   DataComment *pntr = (DataComment *) DCpntr;
   ComplexFloat *data;
   if(!pntr)return (ComplexFloat *)NULL;
   if(!pntr->data) return (ComplexFloat *)NULL;
   if(pntr->dataLen < 1) return (ComplexFloat *)NULL;
   data = (ComplexFloat *) smMalloc(pntr->dataLen * sizeof(DataComment) );
   *Npts = pntr->dataLen;
   for(j=0;j<*Npts;j++) data[j] = pntr->data[j];
   return (ComplexFloat *)data;
}
/* --------------------------------------------------------------- */






/* This function formats and prints the comment and data associated with
   the UserData DataComment structure pointed to by DCpntr. If either of
   the data or comment are NULL it prints [NULL data] or [NULL comment]
   as required. If there is 1 data item, it is printed on the same line
   as the comment. Otherwise, each data item is printed on its own line.
   printing is to the unit specified by the argument unit.
*/
void dblPrintUserDataInstance(DBdataComm DCpntr, FILE *unit)
{
   int j;
   DataComment *pntr = (DataComment *) DCpntr;
   int result = 0;
   char fmt[30];


   if(!pntr)return;

   
   result = fprintf(unit,"UserData DC contents: ");
   if(result != EOF){
      if(pntr->comment)
         result = fprintf(unit,"%s ",pntr->comment);
      else
	 result = fprintf(unit,"[NULL comment] ");
      if(result != EOF){
	if(!pntr->data){
	   result = fprintf(unit,"[NULL data] \n");
	   if(result != EOF)return;
	}
	else{
	  if(pntr->dataLen == 1){
	     strcpy(fmt,"(%f + %fi)");
	     result = fprintf(unit,fmt,pntr->data[0].real,pntr->data[0].imag);
	     if(result != EOF){fprintf(unit,"\n"); return;}
	  }
	  else{
	     strcpy(fmt,"\n(%f + %fi)");
	     j=0;
	     while(result != EOF && j < pntr->dataLen){
	        result = fprintf(unit,fmt,pntr->data[j].real,pntr->data[j].imag);
		j++;
	     }
	     if(result != EOF){fprintf(unit,"\n\n"); return;}
	  }
	}
      }
   }
	 
   dblSetError(1, "ERROR: Writing file instance in dblPrintUserDataInstance.\n");
   return;
}
/* --------------------------------------------------------------- */






/* This function prints the entire contents of the UserData DataComment
   list to the unit specified by the unit argument. Printing is done by
   repeated use of the dblPrintUserDataInstance function.
*/
void dblPrintUserDataList(DBlist list, FILE *unit)
{
   DataComment *DCpntr;
   struct CSStree *tree = (struct CSStree *) list;

   if(!tree)return;
   if(!tree->UserData) return;

   DCpntr = ( DataComment * )dblGetNextDataComment(0,tree);
   while(DCpntr){
      dblPrintUserDataInstance(DCpntr, unit);
      DCpntr = ( DataComment * )dblGetNextDataComment(DCpntr, tree);
   }
}
/* --------------------------------------------------------------- */





 


/* Given a pointer to a tree, Delete the entire UserData DataComment list */
static void dblDestroyUserDataDataCommentList(DBlist list)
{
   struct CSStree *tree = (struct CSStree *) list;

   if(!tree)return;
   if(!tree->UserData)return;
   if(!tree->UserData->dataComment)return;

   while(tree->UserData->dataComment){
      dblDeleteUserDataComStruct(tree->UserData->dataComment, list);
   }
}
/* --------------------------------------------------------------- */

/* .............  End UserData DataComment functions    ............. */

/* +++++++++++++++++ END USERDATA ACCESS FUNCTIONS +++++++++++++++++++ */

