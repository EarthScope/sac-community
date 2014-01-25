#ifndef SACIO_H
#define SACIO_H

#include <stdio.h>

#include "SacHeader.h"

#include "bool.h"
#include "dfm.h"

//FILE *sacfile;
//static char sacErrorStrg[300];

#define PREPICK  1
#define POSTPICK 2
#define CUTPOINT 3

int  AridJoinsWithWfid(DBlist tree, int arid, int wfid);





extern int sacLoadDataFromFiles(char *specifier, int SkipData,
                                char* WorkSetName , int takeEvid );

extern sacSACdata *sacInput(char *filename, struct SACheader *header, ...);
extern void sacHeaderFromCSS(DBlist tree, struct SACheader *header, struct wfdiscList *w,
                             int RefTimeType, double *RefTime, MagType Mtype);
extern int sacWriteSacFile(DBlist tree, struct wfdiscList *w, char 
		                                       *dir, char *fname);
int sacLoadFromHeaderAndData(struct SACheader *header, sacSACdata *data,
                             char *WorkSetName, int Replace, int index,
                             int UpdateData , int takeEvid );

struct arrivalList *sacFindNextMatchingPick(struct arrivalList *ar,
                                            DBlist tree, int wfid);

#endif

