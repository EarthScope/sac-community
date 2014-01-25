#ifndef FALSE
#       define FALSE 0
#       define TRUE !FALSE
#endif 

#ifndef DBSELECTH
#define DBSELECTH TRUE


#define NUMOPTIONS 32
#define OPTIONWIDTH 20
#define HELPOPTIONWIDTH 70
#define MAXLINELENGTH 81
enum    DATATYPE {REAL, CHAR};
enum    opID{help=0,schema,connect,srclat,srclon,srcdep,srcbox,srcdist,
	     srctype,srcdelta,mbmag,mlmag,msmag,starttime,endtime,staname,
	     maxrows,component,stalat,stalon,stabox,instdepth,showdefaults,
	     authlist,eventid,srccircle,quadrant,phaselist,showsql,cmdfile,
             wfid,sid};



int OwnerHasThisTable(char *Table) ;



#ifdef DEFINE_DB_VARS

char options[NUMOPTIONS][OPTIONWIDTH] = {
                       "HELP\0",
                       "SCHEMA\0",
		       "CONNECT\0",
                       "SRCLAT\0",
		       "SRCLON\0",
		       "SRCDEP\0",
		       "SRCBOX\0",
		       "SRCDIST\0",
		       "SRCTYPE\0",
		       "SRCDELTA\0",
		       "MBMAG\0",
		       "MLMAG\0",
		       "MSMAG\0",
		       "STARTTIME\0",
		       "ENDTIME\0",
		       "STANAME\0",
		       "MAXROWS\0",
		       "COMPONENT\0",
		       "STALAT\0",
		       "STALON\0",
		       "STABOX\0",
		       "INSTDEPTH\0",
		       "SHOWDEFAULTS\0",
		       "AUTHLIST\0",
		       "EVENTID\0",
		       "SRCCIRCLE\0",
		       "QUADRANT\0",
		       "PHASELIST\0",
		       "SHOWSQL\0",
		       "CMDFILE\0",
                       "WFID\0",
                       "SID"};

int MaxParamNum[NUMOPTIONS] = {2,1,1,2,2,2,-1,2,-1,2,2,2,2,6,
			       6,-1,1,-1,2,2,-1,2,0,-1,-1,3,2,-1,1,1,-1,1};

/*   The following variables are allocated as necessary and filled by 
     functions in the dbSelect module. They are used in this module to
     control construction of the SQL string.
*/
/* An array of logicals telling whether an option was specified by user */
int optionSelected[NUMOPTIONS];
  
/* holds the float parameters specified for each option */
float *floatParams[NUMOPTIONS];

/* holds the string parameters specified for each option */ 
char **charParams[NUMOPTIONS];

 /* holds the number of parameters specified for each option */   
int numParams[NUMOPTIONS];


#else

/* An array of logicals telling whether an option was specified by user */
extern int optionSelected[NUMOPTIONS];
  
/* holds the float parameters specified for each option */
extern float *floatParams[NUMOPTIONS];

/* holds the string parameters specified for each option */ 
extern char **charParams[NUMOPTIONS];

 /* holds the number of parameters specified for each option */   
extern int numParams[NUMOPTIONS];

/* An array of strings representing the valid options */       
extern char options[NUMOPTIONS][OPTIONWIDTH];

/* Array holding the maximum number of parameters expected by each option */
extern int MaxParamNum[NUMOPTIONS];

/* descriptive strings for each option */
char optionHelp[NUMOPTIONS][HELPOPTIONWIDTH];

extern void dbFreeTokenList();
/* ------------------------------------------------------------------ */


#endif 




#endif 
   
