/** 
 * @file   contouring.h
 * 
 * @brief  Contouring 
 * 
 */

#ifndef _CONTOURING_H_
#define _CONTOURING_H_

#include "mach.h"

#define	MACTIONDRAW	2
#define	MACTIONLABEL	10
#define	MACTIONMOVE	1
#define	MACTIONTICK	3
#define	MLABELCANDIDATE	1
#define	MLABELREJECTED	2
#define	MLABELSELECTED	3
#define	MLINKCLOSED	(-2)
#define	MLINKOPEN	(-1)
#define	MSEGLABELCOMPLE	3
#define	MSEGLABELELIMIN	2
#define	MSEGLABELINCOMP	1
#define	MZLEVELS	40



struct t_cmcontouring {
  int    maxsegments;
  int    numsegments;
  int    indexlevels;
  int    indexstarts;
  int    indexstops;
  int    maxpoints;
  int    numpoints;
  int    indexpoints;
  int    indexlinks;
  int    indexrlinks;
  int    indexaction;
  int    ixdatastart;
  int    ixdatastop;
  int    iydatastart;
  int    iydatastop;
  double  zlevels[MZLEVELS];
  int    nzlevels;
  double  zlevellist[MZLEVELS];
  int    nzlevellist;
  double  zlevelminimum;
  double  zlevelmaximum;
  double  zlevelincrement;
  int    nznumlevels;
  int    lines[MZLEVELS];
  int    nlines;
  int    linelist[MZLEVELS];
  int    nlinelist;
  double  zregionlist[MZLEVELS];
  int    nzregionlist;
  int    ncolorlist;
  int    iticklist[MZLEVELS];
  int    nticklist;
  double  ticklength;
  double  tickspacing;
  int    lticksdown;
  int    indexseglabelst;
  int    indexseglabelnu;
  int    indexseglabelfi;
  int    maxlabels;
  int    numlabels;
  int    indexlabelpoint;
  int    indexlabeltype;
  int    indexlabelangle;
  int    indexlabeltext;
  int    nlabellist;
  double  minlabelspacing;
  double  maxlabelspacing;
  double  deslabelspacing;
  double  desiredangle;
  double  widthlabels;
  double  heightlabels;
} cmcontouring;

struct t_kmcontouring {
  char   klistname[MCPFN+1];
  char   kdatamode[9];
  char   klevelmode[9];
  char   klinemode[9];
  char   klinetype[9];
  char   kcolormode[9];
  char   kcolorlist[MZLEVELS][9];
  char   ktickmode[9];
  char   klabelmode[9];
  char   klabellist[MZLEVELS][17];
  char   klabel[MZLEVELS][17];
  char   kdecimal;
} kmcontouring;



#ifdef DOINITS

int   *const Iticklist   = &cmcontouring.iticklist[0] - 1;
int   *const Linelist    = &cmcontouring.linelist[0] - 1;
int   *const Lines       = &cmcontouring.lines[0] - 1;
double *const Zlevellist  = &cmcontouring.zlevellist[0] - 1;
double *const Zlevels     = &cmcontouring.zlevels[0] - 1;
double *const Zregionlist = &cmcontouring.zregionlist[0] - 1;

#else

extern int  *const Iticklist;
extern int  *const Linelist;
extern int  *const Lines;
extern double *const Zlevellist;
extern double *const Zlevels;
extern double *const Zregionlist;

#endif

void alloclabels ( int maxsegments, 
                   int maxlabels, 
                   int *indexseglabelst, 
                   int *indexseglabelnu, 
                   int *indexseglabelfi, 
                   int *indexlabelpoint, 
                   int *indexlabeltype, 
                   int *indexlabelangle, 
                   int *indexlabeltext, 
                   int *nerr);
void allocpoints ( int maxpoints, 
                   int *indexpoints, 
                   int *indexlinks, 
                   int *indexrlinks, 
                   int *indexaction, 
                   int *nerr);
void allocsegments ( int maxsegments, 
                     int *indexlevels, 
                     int *indexstarts, 
                     int *indexstops, 
                     int *nerr);
void calccontlabel1 (void);
void calccontlabel4 (void);
void calccontlabels (void);
void calccontrlinks (void);
void calccontsegs ( float array[], 
                    int nxsize, 
                    int nysize, 
                    int *nerr);
void calccontticks (void);
void calclastpoint ( int jstop, 
                     double skiplength, 
                     int *jlast);
void calcsegangle ( float point1[], 
                    float point2[], 
                    float *angle);
void fastcontdata ( float array[], 
                    int nxsize, 
                    int nysize, 
                    int *nerr);
void getcontlabel ( int number, 
                    int *jpoint, 
                    int *jtype, 
                    float *angle, 
                    int *jtext);
void getcontpoint ( int number, 
                    float point[], 
                    int *link, 
                    int *action);
void getcontrlink ( int number, 
                    int *rlink);
void getcontseg ( int number, 
                  int *level, 
                  int *start, 
                  int *stop);
void getcontseglabel ( int number, 
                       int *status, 
                       int *numlocs, 
                       int *firstloc);
void initcontattr (void);
void linkcontsegs ( int level, 
                    float point1[], 
                    float point2[]);
void listcontsegs (void);
void markcontlabel ( int index, 
                     int ilabel, 
                     double width, 
                     float *angle);
void mergecontsegs (void);
void newcontlabel ( int jpoint, 
                    int jtype, 
                    double angle, 
                    int jtext, 
                    int *number);
void newcontpoint ( float point[], 
                    int link, 
                    int action, 
                    int *number);
void newcontseg ( int level, 
                  int start, 
                  int stop, 
                  int *number);
int nextcontseg ( int *number, 
                  int *level, 
                  int *start, 
                  int *stop);
void plotcontdata ( float zdata[], 
                    int nxsize, 
                    int nysize, 
                    int *nerr);
void plotcontsegs (void);
int pointsequal ( float point1[], 
                  float point2[]);
void putcontlabel ( int number, 
                    int jpoint, 
                    int jtype, 
                    double angle, 
                    int jtext);
void putcontpoint ( int number, 
                    float point[], 
                    int link, 
                    int action);
void putcontrlink ( int number, 
                    int rlink);
void putcontseg ( int number, 
                  int level, 
                  int start, 
                  int stop);
void putcontseglabel ( int number, 
                       int status, 
                       int numlocs, 
                       int firstloc);
void releaselabels (void);
void releasepoints (void);
void releasesegments (void);
void setcontdatalim ( int ixstart, 
                      int ixstop, 
                      int iystart, 
                      int iystop);

#endif /* _CONTOURING_H_ */
