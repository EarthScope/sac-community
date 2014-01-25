#ifndef SM_DATA_IO_H   
#define SM_DATA_IO_H


#include <stdio.h>

#include "cssListOps/dblPublicDefs.h"

struct WorkSet{
   DBlist tree;
   char *name;
   int index;
   struct WorkSet *next;
};

static const int CreateMode     = 1;
static const int NoData         = 2;
static const int WorkSetNameArg = 3;
static const int NewWorkSet     = 4;
static const int OverWrite      = 5;
static const int Add            = 6;
static const int Directory      = 7;
static const int Filelist       = 8;
static const int Nfiles         = 9;




int smCreateEmptyWorkset(char *name);
void smListAllWorksets(FILE *unit);
int smChangeDefaultWorksetByIndex(int index);
int smChangeDefaultWorksetByName(char *name);
void smDeleteWorksetByIndex(int index);
void smDeleteWorksetByName(char *name);
struct WorkSet *smGetDefaultWorkset(void);
void * smGetDefaultTree(void);
DBlist smMakeDefaultTree(void);
void smDeleteAllWorkSets(void);
void smDeleteDefaultTree(void);
char *smGetDefaultWorksetName(void);
int smChangeWorksetName(char* OldName, char* NewName);
void smClearDefaultTree(void);
int smLoadOracleData(int takeEvid , char *specifier,...);
int smLoadSACData(int takeEvid, char *specifier,...);
int smWriteSACData(char *WorkSet, ... );

#endif


