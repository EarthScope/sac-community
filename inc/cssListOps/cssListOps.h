
#ifndef CSS_LIST_OPS_H
#define CSS_LIST_OPS_H


#include <stdio.h>

#include "dblUserData.h"
#include "cssStrucs.h"

DBlist dblNewTree(void);
DBtable dblCreateTableInstance(DBlist list, dblObject StrucType);
void dblDeleteTableInstance(dblObject StrucType, DBlist dblList,  DBtable pntr);
void dblDeleteTree(DBlist Tree);
void dblCopyTable(dblObject StrucType,  DBtable origPntr, DBtable copyPntr);
void dblCopyTableElement(dblObject StrucType, DBelement origPntr, DBtable copyPntr);
DBlist dblCopyTree(DBlist OldTree);
int dblSortWfdiscList(DBlist TreePtr, int *Index, int Nitems);
int dblDeleteWfdiscs(DBlist tree, int *index, int Nitems);
DBlist dblMergeTrees(DBlist tree2, DBlist tree, int takeEvid );
int dblGetNumWaveformsInMemory(DBlist tree);

DBtable dblGetTableInstance(DBobj targetObj, dblObject specifier, int index);
DBobj dblGetTableObject(DBobj targetObj, dblObject specifier);
DBtable dblNextTableInstance(DBtable tableInstance, DBlist dblList, dblObject specifier);
void dblSetTraceData(DBtable tableInstance, DataType dtype, void *data, int npts);
int dblGetTableIndex(dblObject StrucType,  DBtable pntr);
int dblAddComment(DBlist list, dblObject StrucType, DBtable rowPntr, const char* comment);
char** dblGetComments(DBlist list, dblObject StrucType, DBtable rowPntr, int *Nlines);

       /* *****************User Data Access Functions****************** */
void dblSetUserDataComment(DBlist list, const char *comment);
char * dblGetUserDataComment(DBlist list);
void dblSetUserDataMatrixComment(DBlist list, const char *comment);
char * dblGetUserDataMatrixComment(DBlist list);
void dblSetUserDataMatrixData(DBlist list, int Nrows, int Ncols, 
			      ComplexFloat **matrix);
ComplexFloat **dblGetUserDataMatrixData(DBlist list, int *Nrows, int *Ncols);
DBdataComm dblCreateDataComment(DBlist list);
void dblSetUserDataComComment(DBdataComm DCpntr, const char *comment);
void dblDeleteUserDataComStruct(DBdataComm DCpntr, DBlist list);
DBdataComm dblGetNextDataComment(DBdataComm DCpntr, DBlist list);
char *dblGetUserDataComComment(DBdataComm DCpntr);
DBdataComm dblGetNextMatchingDataComment(DBdataComm DCpntr, DBlist list, 
					 char * comment);
DBdataComm dblGetNextDCMatchingIndex(DBdataComm DCpntr, DBlist list, int index); 
void dblSetDCIndex(DBdataComm DCpntr, int index); 
int dblGetDCIndex(DBdataComm DCpntr); 
void dblSetDCType(DBdataComm DCpntr, enum dataType type);
void dblSetSacUserNum(DBdataComm DCpntr, int index);
int dblGetSacUserNum(DBdataComm DCpntr);
DBdataComm dblGetNextDCSacUserMatch(DBlist list, int index, int Num);

 
enum dataType dblGetDCType(DBdataComm DCpntr); 

void dblSetUserDataComData(DBdataComm DCpntr, const ComplexFloat *data, int Npts);
ComplexFloat *dblGetUserDataComData(DBdataComm DCpntr, int *Npts);
int dblGetSeismograms( struct wfdiscList *wfStruc, char*, char*);
int dblWriteUserData(DBlist list, char * filename);
int dblReadUserData(DBlist list, char * filename, int OverWrite);


int dblWfidInUse(DBlist tree, int wfid);
void dblReplaceWfid(DBlist tree, int OldWfid, int NewWfid);
int dblNextAvailableWfid(DBlist tree);
struct originList *dblOridInUse(DBlist tree, int orid);
int dblNextAvailableOrid(DBlist tree);
void dblReplaceOrid(DBlist tree, int OldOrid, int NewOrid);
struct eventList *dblEvidInUse(DBlist tree, int evid);
int dblNextAvailableEvid(DBlist tree);
void dblReplaceEvid(DBlist tree, int OldEvid, int NewEvid);
int dblAridInUse(DBlist tree, int arid);
int dblNextAvailableArid(DBlist tree);
void dblReplaceArid(DBlist tree, int OldArid, int NewArid);
int dblChanidInUse(DBlist tree, int chanid);
int dblNextAvailableChanid(DBlist tree);
void dblReplaceChanid(DBlist tree, int OldChanid, int NewChanid);
int dblInidInUse(DBlist tree, int inid);
int dblNextAvailableInid(DBlist tree);
void dblReplaceInid(DBlist tree, int OldInid, int NewInid);


char *MakeUniqueSiteName(DBlist tree, char *NewName);
char *MakeUniqueChanName(DBlist tree, char *sta, char *NewChan);
char *CSSstrcpy(char *fresh, char *old);
int CSSstrcmp(char *s1, char *s2);



/*             ****** Functions check for NULL valued CSS header fields. ******     */
int isValidFloat ( int table , int field , double value );
int isValidInt ( int table , int field , int value );
int isValidString ( int table , int field , char *value );


/*             ****** Functions to print formatted data from tree ******            */
void dblPrintUserDataInstance(DBdataComm DCpntr, FILE *unit);
void dblPrintUserDataList(DBlist list, FILE *unit);

void dblDumpTable(DBlist list, dblObject StrucType, FILE *unit,...);
void dblPrintTableInstance(DBtable tableInstance,  dblObject specifier, FILE *unit);
void dblTableOfContents(DBlist dblList, FILE *unit);

userData *dblCreateUserDataStruc(DBlist list);
void dblDestroyUserDataStruc(DBlist list);

struct trace * dblGetData ( int index ) ;

#define AF_LIST (struct affiliationList *)
#define AR_LIST (struct arrivalList *)
#define OR_LIST (struct originList *)
#define RE_LIST (struct remarkList *)
#define WF_LIST (struct wfdiscList *)
#define WT_LIST (struct wftagList *)
#define AS_LIST (struct assocList *)
#define EV_LIST (struct eventList *)
#define GR_LIST (struct gregionList *)
#define IN_LIST (struct instrumentList *)
#define OE_LIST (struct origerrList *)
#define SE_LIST (struct sensorList *)
#define SL_LIST (struct siteList *)
#define SC_LIST (struct sitechanList *)
#define UD_LIST (userData *)
#define SA_LIST (struct stassocList *)
#define SD_LIST (struct sacdataList *)



#define NULL_AF_LIST AF_LIST NULL  
#define NULL_AR_LIST AR_LIST NULL
#define NULL_OR_LIST OR_LIST NULL
#define NULL_RE_LIST RE_LIST NULL
#define NULL_WF_LIST WF_LIST NULL
#define NULL_WT_LIST WT_LIST NULL
#define NULL_AS_LIST AS_LIST NULL
#define NULL_EV_LIST EV_LIST NULL
#define NULL_GR_LIST GR_LIST NULL
#define NULL_IN_LIST IN_LIST NULL
#define NULL_OE_LIST OE_LIST NULL
#define NULL_SE_LIST SE_LIST NULL
#define NULL_SL_LIST SL_LIST NULL
#define NULL_SC_LIST SC_LIST NULL
#define NULL_UD_LIST UD_LIST NULL
#define NULL_SA_LIST SA_LIST NULL
#define NULL_SD_LIST SD_LIST NULL



#endif
