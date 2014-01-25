/** 
 * @file   dfm.h
 * 
 * @brief  Data File Management
 * 
 */

#ifndef _DFM_H_
#define _DFM_H_

#include "clf.h"
#include "mach.h"

#define MAXSORTFIELDS	5
#define	MCOMP	        2
#define	MCUTER	        3
#define	MDGSUB          3
#define	MHDRCO	        20
#define	MPICK	        18
#define	MRWFMT          5
#define COMMIT	        0
#define RECALL          1
#define ROLLBACK        2
#define BINARY          0
#define ASCII           1

typedef enum {
  Any, 
  MsMag, 
  MlMag, 
  MbMag
} MagType;

typedef enum {
  Ascending, 
  Descending
} SortOrder;

/* TWODAYS in seconds is 48 hours times 3600 seconds per hour. */
#define TWODAYS ( 48 * 3600 )

enum readFlag { 
  RDB, 
  HIGH, 
  LOW 
};

struct t_cmdfr {
  int nstart[MDFL];
  int nfillb[MDFL];
  int nstop[MDFL];
  int nfille[MDFL];
  int ntotal[MDFL];
  int nxsdd[MDFL];
} cmdfr;

struct t_cmdfm {
  int ndfl;          /** Number of files in the Data File List */
  int idflc;         /** Number of files in the Data File Write List */
  int ndsndx[MDFL];  /** Data Set Storage, not sure if this is really used */
  int ndsflcnt ;     /** Number of files in the Data Set Storage */
  int ncdir;         /** Only used in wildfl () */
  int lechof;        /** Echo filenames for wildcards */
  int ndxhdr[MDFL];  /** */
  int ndxdta[MDFL][MCOMP]; /** */
  int nlndta[MDFL];  /** */
  int ncomp[MDFL];   /** */
  int lovrrq;       /** Ok to overwrite requested files */
  int lcut;         /** If the cut parameter is on */
  double ocut[2];
  int icuter;       /** Current Cut Error Behavior */
  int nrwfmt;       /** Current Convert File Type Format */
  int iwfmt;        /** Curret Write File Format  */
  int icfmt[2];     /** Current Convert File In/Out Formats */
  int ipckn;      /** ID for Pick "N" */
  int ipckz;      /** ID for Pick "Z" */
  int ipckg;      /** ID for Pick "G" */
  int ipckb;      /** ID for Pick "B" */
  int ipcke;      /** ID for Pick "E" */
  int ipcko;      /** ID for Pick "O" */
  int ipcka;      /** ID for Pick "A" */
  int ipckt0;     /** ID for Pick "T0" */
  int ipckt1;     /** ID for Pick "T1" */
  int ipckt2;     /** ID for Pick "T2" */
  int ipckt3;     /** ID for Pick "T3" */
  int ipckt4;     /** ID for Pick "T4" */
  int ipckt5;     /** ID for Pick "T5" */
  int ipckt6;     /** ID for Pick "T6" */
  int ipckt7;     /** ID for Pick "T7" */
  int ipckt8;     /** ID for Pick "T8" */
  int ipckt9;     /** ID for Pick "T9" */
  int ipckf;      /** ID for Pick "F" */
  int ipckhd[MPICK];  /** Index of picks into the SAC header */
  int lround;         /** Current Synch Rounding Behavior */
  int icatco[MHDRCO];
  int itemco[MHDRCO];
  int ldfree;        /** Reading Free Format or not */
  int idgsub;        /** Current Data Generate Type */
  int ndgfil;        /** Current Number of files in the Data Generage List */
  int lshift;        /** If rcss should perform a time shift */
  int lscale;        /** If rcss should calibrate data */
  MagType nMagSpec;  /** specify which magnitude to read in rcss. */
  int  iauthors;     /** Number of authors to search for in .arrival cssfile. */
  int nSortOrder;    /** Sort Order */
  int larray;	     /** If ARRAY option for READCSS is on */
  int lrascii;	     /** If RCSS reads ascii flat files, else reads CSSB */
  int lwascii;       /** If WCSS writes ascii flat files, else writes CSSB */
  int ldata;	     /** If waveform committed in COMMIT command. */
  int lpref;	     /** If CSS picks should be read through the preferences */

  /* COMMIT / ROLLBACK options. 0: commit, 1: recall trace, 2: rollback */
  int icomORroll ;

  /* ASCEND / DESCEND for SORT */
  SortOrder idirection[ MAXSORTFIELDS ] ;

  /* COMMIT option on deletechannel. */
  int lcommit ;

  /* rdsegy */
  int iztype ;

  /* writetable */
  int liftype ;
  int lheader ;

  /* Fix data transfer from SAC buffers to CSS buffers. */
  int ltrust ;
  enum readFlag nreadflag ;
  int lread ;
  int  nfilesFirst ;

  int lcm6 ;  /* GSE CM6 compressed format instead of integer format */
} cmdfm;


struct t_kmdfm {
    //  char kdfl[MAXCHARS];     /** Data File List Filenames */
  char krddir[MCPFN+1];    /** Current directory to read from */
  char kwrdir[MCPFN+1];    /** Current directory to write to */
  char krdcssdir[MCPFN+1]; /** Current directory to read CSS files from */
  int lstation;            /** CSS read station filter */
  int lchannel;            /** CSS read channel filter */
  char kstation[7];        /** CSS station filter value */
  char kchannel[9];        /** CSS channel filter value */
  int  lbandw;             /** CSS read bandwidth filter */
  char kbandw[9];          /** CSS bandwidth filter value */
  int  lorient;            /** CSS read orientation filter */
  char korient[9];         /** CSS orientation filter valye */
  char kdirnm[MCPFN+1];    /** Never Used */
  char ksuffx[MDFL][3];    /** Files name suffix, waste of space */
  char kcut[2][9];         /** Picks to use during cut*/
  char kcuter[MCUTER][9];  /** Defined Cut Errors */
  char kecbdf[9];          /** Warning or Error while reading file @bug */
  char kecmem[9];          /** To keep or destroy files in memory @bug */
  char krwfmt[MRWFMT][9];  /** Defined Convert File Formats */
  char kcfile[2][MCPFN+1]; /** Current Convert In/Out Filename */
  char kcfmt[2][9];        /** Current Convert Data Card Format */
  char kpick[MPICK][9];    /** Defined Pick Names */
  char kdcont[MCMSG+1];    /** Reading Formatted Type: Content Type  */
  char kdform[MCMSG+1];    /** Reading Formatred Type: Format Statement */
  char kdgsub[MDGSUB][9];  /** Defined Data Generate Types @bug */
  char kdgfil[MCMSG+1];    /** Data Generate File List @bug */

  /* The following were added to facilitate rcss reading  
   * picks from .arrival file. 
   */
  char kprefsFileName[ MCPFN ];  /** CSS Picks File Name */ 
  char **kauthors ;              /** CSS Authors File Name */
  char ktPh[10][9];              /** Phases */
  char ktAu[10][16] ;            /** Authors */

  /* BINARY or ASCII options */
  char kbinORasc[2][9] ;         /** Defined CSS Binary or Ascii file type */

  /* Added for the sort command. */
  char ksort[ MAXSORTFIELDS ][ 9 ] ; /** Fields to use during sorting */
} kmdfm;


#ifdef DOINITS
int   *const Icatco      = &cmdfm.icatco[0] - 1;
int   *const Icfmt       = &cmdfm.icfmt[0] - 1;
int   *const Ipckhd      = &cmdfm.ipckhd[0] - 1;
int   *const Itemco      = &cmdfm.itemco[0] - 1;
int   *const Ncomp       = &cmdfm.ncomp[0] - 1;
int   *const Ndsndx      = &cmdfm.ndsndx[0] - 1;
int   *const Ndxhdr      = &cmdfm.ndxhdr[0] - 1;
int   *const Nfillb      = &cmdfr.nfillb[0] - 1;
int   *const Nfille      = &cmdfr.nfille[0] - 1;
int   *const Nlndta      = &cmdfm.nlndta[0] - 1;
int   *const Nstart      = &cmdfr.nstart[0] - 1;
int   *const Nstop       = &cmdfr.nstop[0] - 1;
int   *const Ntotal      = &cmdfr.ntotal[0] - 1;
int   *const Nxsdd       = &cmdfr.nxsdd[0] - 1;
double *const Ocut        = &cmdfm.ocut[0] - 1;
int    const wfHeader    = -1 ;
int    const allHeader   = 0 ;
int    const eventHeader = 1 ;
float  MaxMem      = 0.3 ;

string_list *datafiles = NULL;

#else
extern int   *const Icatco;
extern int   *const Icfmt;
extern int   *const Ipckhd;
extern int   *const Itemco;
extern int   *const Ncomp;
extern int   *const Ndsndx;
extern int   *const Ndxhdr;
extern int   *const Nfillb;
extern int   *const Nfille;
extern int   *const Nlndta;
extern int   *const Nstart;
extern int   *const Nstop;
extern int   *const Ntotal;
extern int   *const Nxsdd;
extern double *const Ocut;
extern int    const wfHeader ;
extern int    const allHeader ;
extern int    const eventHeader ;
extern float        MaxMem ;
extern string_list *datafiles;
#endif


void        cleardfl    (int *nerr);
void        clear_file  (int i, int *nerr);
void        cnvfmt      (char  *kcard, 
			 int    kcard_s, 
			 char  *kfmt, 
			 int    kfmt_s, 
			 int    nentry, 
			 float *fentry, 
			 int   *nerr);
void        cnvfre      (char  *kcard, 
			 int    kcard_s, 
			 int    mentry, 
			 int   *nentry, 
			 float *fentry, 
			 int   *ientry, 
			 char  *kalpha, 
			 int    kalpha_s, 
			 int    lstrict, 
			 int   *nerr);
void        crsac       (int  idfl, 
			 int  ncmp, 
			 int  nlen, 
			 int *ndxh, 
			 int *ndx1, 
			 int *ndx2, 
			 int *nerr);
void        decont      (char *kcont, 
			 int   kcont_s, 
			 int   maxch, 
			 int  *numch, 
			 int  *numxch, 
			 int  *numych, 
			 int  *iopch, 
			 int  *ltoend, 
			 int  *nerr);
void        defcut      (char   kcut[2][9], 
                         double ocut[2],
                         int    idfl, 
                         int   *nerr);
void        defmem      (int  idfl, 
			 int  lcutnow, 
			 int  *nerr);
void        detnum      (char *kfmt, 
			 int   kfmt_s, 
			 int  *nentry) ;
void        gennames    (char *headerfield, 
                         int   lenheaderfield, 
                         string_list *list,
                         int   nfiles, 
                         int  *nerr);
void        getatw      (char  *krtw, 
                         int    krtw_s, 
                         double *ortw, 
                         double *tmin, 
                         double *tmax, 
                         int   *nofmin, 
                         int   *nlnwin, 
                         int   *nerr);
void        getprefs    (int lauth, 
                         int lphase);
string_list * getwfdiscs  (string_list *list,
                           int         *nerr);
void        inidfm      ( );
void       iztypeMessage(const int item, 
			 int iztype);
int         lfilesok    (string_list *files,
                         char *kdir, 
                         int   kdir_s, 
                         int   lmore, 
                         int   lheader, 
                         int   lxdr, 
                         int  *nerr);
void        makeuniq    (char *filelist,
			 int   lenfilelist,
			 int   nfiles,
			 int  *nerr);
void        rdci        (int   idfl, 
			 char *kname, 
			 int   kname_s, 
			 int  *nlen, 
			 int  *ndx1, 
			 int  *ndx2, 
			 int  *nerr);
void        readcfl     (int    lmore, 
                         char  *kdirin, 
                         int    kdirin_s, 
                         string_list *list,
                         int    Verbose, 
                         int    isASCII, 
                         float  MaxMem, 
                         int   *nerr);
void        readfl      (int   ldata,
			 int   lmore, 
			 int   lsdd, 
			 char *kdirin, 
			 int   kdirin_s,
                         string_list *list,
			 int  *nerr);
void        readgse     (int   lmore, 
                         char *kdirin, 
                         int   kdirin_s, 
                         string_list *list,
                         int   Verbose, 
                         int   isASCII, 
                         float MaxMem, 
                         int  *nerr);

void        readsuds    (int lmore, 
                         char *kdirin, 
                         int   kdirin_s, 
                         string_list *list,
                         int   Verbose, 
                         int   isASCII, 
                         float MaxMem, 
                         int  *nerr);
void        Index       (char *a, 
			 int   N, 
			 int   size, 
			 int   (*fp1)(void *a1, void *a2), 
			 int   *index);
void        synch       (int   ndttmi[][6], 
			 float  *offsti, 
			 int     num, 
			 int   ndttmo[][6], 
			 float  *offsto, 
			 int     lbegin);
void        vblist      (int *nerr);
void        vfeven      (int *nerr);
void        vflist      (int *nerr);
void        vfmax       (int *maxf, 
			 int *nerr);
void        vfmaxn      (int  maxn, 
			 int *maxf, 
			 int *nerr);
void        vfrng       (double  rngmin, 
			 double  rngmax, 
			 int    *nerr);
void        vfspec      (int *nerr);
void        vftime      (int *nerr);
void        wrci        (int   idfl, 
			 char *kname, 
			 int   kname_s, 
			 char *kfmt, 
			 int  *nerr);
void       xHeaderWindow(int   *nerr, 
			 int    argc, 
			 char **argv);
void        xch         (int *nerr);
void        xconv       (int *nerr);
void        xcopyhdr    (int *nerr);
void        xcuter      (int *nerr);
void        xdatagen    (int *nerr);
void      xdeletechannel(int *nerr);
void        xdfmc       (int  index, 
			 int *nerr);
void        xlh         (int *nerr);
void        xpickauthor (int *nerr);
void        xpickphase  (int *nerr);
void        xpickprefs  (int *nerr);
void        xr          (int *nerr);
void        xra         (int  lplot, 
			 int *nerr);
void        xrcss       (int *nerr);
void        xrerr       (int *nerr);
void        xrgse       (int *nerr);
void        xrh         (int *nerr);
void        xrsdd       (int *nerr);
void        xrsuds      (int *nerr);
void        xrtab       (int  lplot, 
			 int *nerr);
void        xsort       (int *nerr);
void        xsynch      (int *nerr);
void        xw          (int  lsdd, 
			 int *nerr);
void        xwcss       (int *nerr);
void        xwgse       (int *nerr);
void        xwh         (int *nerr);
void        xwild       (int *nerr);
void        xwtab       (int *nerr);


int    LessThanStrg     ( void *a1, 
                          void *a2);
int    GreaterThanStrg  ( void *a1, 
                          void *a2);
int    isNaN            ( float a1);
int    LessThanFloat    ( void *a1, 
                          void *a2);
int    GreaterThanFloat ( void *a1, 
                          void *a2);
int    LessThanLong     ( void *a1, 
                          void *a2);
int    GreaterThanLong  ( void *a1, 
                          void *a2);

#endif /* _DFM_H_ */
