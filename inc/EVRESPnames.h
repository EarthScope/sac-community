/** 
 * @file   EVRESPnames.h
 * 
 * @brief  Evalresp names
 * 
 */

#ifndef _EVRESPNAMES_H_
#define _EVRESPNAMES_H_

/** 
 * These are the keywords recognized by the routines in the EVRESPnames module 
 * Transfer from or transfer to  
 */
enum Direction { 
  FROM, 
  TO 
};

enum {
  EV_HOUR = 0,
  EV_MIN,
  EV_SEC,
  EV_MSEC,
};

/** 
 *  The types of evalresp sub-parameters  
 */
enum EVparam { 
  CHANNEL, 
  STATION, 
  NETWORK, 
  LOCID, 
  DATE, 
  TIME, 
  TYPE, 
  FILENAME, 
  NAME_FROM_DB
};


void setStationName(char *, enum Direction);
void setNetworkName(char *, enum Direction);
void setChannelName(char *, enum Direction);
void setLocidName  (char *, enum Direction);
void setFileName(char *name, enum Direction dir);
void setUseDBName(int value, enum Direction dir);

char *getStationName(enum Direction);
char *getNetworkName(enum Direction);
char *getChannelName(enum Direction);
char *getLocidName  (enum Direction);
char *getFileName(enum Direction dir);
int   getUseDBName(enum Direction dir);
int   isSet(enum EVparam, enum Direction);
void  clearEVRESPstrucs(void);

void  setTransferDirection(int);
int   getTransferDirection(void);

void  setDate(char *, enum Direction);
void  setTime(char *, enum Direction);
int   getYear(enum Direction);
int   getJday(enum Direction);
int   getHour(enum Direction);
int   getMinute(enum Direction);
int   getSecond(enum Direction);

void  setType(char *, enum Direction);
char *getType(enum Direction);
int   getTime(enum Direction dir, int type);


#endif /* _EVRESPNAMES_H_ */
