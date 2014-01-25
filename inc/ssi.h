/** 
 * @file   ssi.h
 * 
 * @brief  ?
 * 
 */

#ifndef _SSI_H_
#define _SSI_H_

#include "mach.h"
#include "cssListOps/dblPublicDefs.h"
#include "cssListOps/cssStrucs.h"
#include "SacHeader.h"

struct SACheader globalSacHeader[ MDFL ] ;

void CSStoSAC ( int idfl, 
                struct SACheader *header, 
                struct trace *seis, 
                int lname, 
                int lcutnow, 
                int *nerr);

void DBheaderToSac ( struct SACheader *header, 
                     int lall);
void DBwfToSac ( int idfl, 
                 struct trace *seis, 
                 int *nerr);
int OnOrOff ( char *inString);
void SacHeaderToDB ( struct SACheader *header, 
                     int whichHeaders, 
                     int idfl);
int SeisMgrCode ( char *kcommand, 
	 int *nerr);
void echoAndShave ( char *kcommand, 
                    char **kcommandPtr, 
                    int commandLength);
void SeisMgrToSac ( DBlist tree, 
                    int lname, 
                    int *nerr, 
                    int Verbose, 
                    int lcutnow, 
                    int takeEvid);
void alignFiles ( int *nerr);
int deleteAllSacFiles ( int *nerr, 
                        int lname);
void inissi (void);
void prefPicksToHeader ( struct SACheader *header, 
                         
int idfl, 
                         struct wfdisc *wf, 
                         DBlist tree, 
                         double correction, 
                         int *nerr);
void rollback ( int whichHeaders, 
                int *nerr);
int Unique ( int *array, 
             int size, 
             int wfid);
int next_wfid ( int *array, 
                int size);
void sacToSeisMgr ( int lnew, 
                    int lupdate, 
                    int ldata, 
                    int *nerr);
int uniqueStaAndChan (void);
void xrecall ( int *nerr);
void xcommit ( int *nerr);
void xcutim ( int *nerr);
void xreaddb ( char *kinput, 
               int *nerr);
void xrecall ( int *nerr);
void xrollback ( int *nerr);
void xtablname ( char *kinput, 
                 int *nerr);


int set_default_station_name(int getset);

#endif /* _SSI_H_ */
