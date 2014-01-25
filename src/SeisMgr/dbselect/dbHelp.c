#include <stdlib.h>
#include <string.h>
#include "../stringfun.h"
#include "dbDefaults.h"


char *CheckForAlias(char *token);

void dbPrintToDevice(char *string);
void dbOracleHelpOnSrctype(void);
void dbOracleHelpOnStations(void) ;
void dbOracleHelpOnOneStation(char*) ;
void dbOracleHelpOnAuthors(void) ;
void dbOracleHelpOnPhases(void) ;



static void HelpOnDBname()
{
   char help[] =
   "The CSS 3.0 tables containing the data used by readDB are\n"
   "contained in a schema which you may or may not have access to\n"
   "in your Oracle account. If you do not own the schema or have\n"
   "a set of aliases to the tables in the schema then you must\n"
   "supply the schema name to readDB. This can be done on the\n"
   "command line i.e. \n"
   " readDB ... options ... dbname mena6 ... other options ...\n"
   "or else can be done using a configuration file.\n"
   "As of version 0.59, DBname is no longer stored as such.\n"
   "DBname is now distributed into each table name and is\n"
   "forgotten.  For instance, if dbname mena6 appears on the\n"
   "command line, the tables will be renamed something like:\n"
   "mena6.wfdisc, mena6.wftag, mena6.origin, etc.\n"
   "---------------------------------------------------------------\n";
   dbPrintToDevice("\n");
   dbPrintToDevice(help);
}
/* ------------------------------------------------------------------ */





static void HelpOnHelp()
{
   char help[] =
   "Display the available dbselect options.\n"
   "---------------------------------------------------------------\n";
   dbPrintToDevice("\n");
   dbPrintToDevice(help);
}
/* ------------------------------------------------------------------ */




static void HelpOnSourceType()
{
   char help[] =
   "You can restrict retrieved waveforms to be just those generated\n"
   "by certain types of sources by using the SRCTYPE keyword\n"
   "followed by one or more source specifiers. The currently defined\n"
   "source specifiers are:\n" 
   "--------------------------\n";
   dbPrintToDevice("\n");
   dbPrintToDevice(help);
   dbOracleHelpOnSrctype();
   dbPrintToDevice(
   "---------------------------------------------------------------\n");


}
/* ------------------------------------------------------------------ */







static void HelpOnSourceBox()
{
   char help[] =
   "ReadDB can also restrict seismograms to events with origins\n"
   "inside a user-defined polygonal geographic region. The \n"
   "polygon is defined by a set of lat-lon pairs which are the\n" 
   "vertices of the polygon. The vertices must be listed in\n"
   "counter-clockwise order, and the last vertex is expected to be\n"
   "the same as the starting vertex. The vertices may either be\n"
   "listed on the command line after the SRCBOX keyword or else\n"
   "they may be contained in a file with one vertex per line. To\n"
   "use the file, give its name to readDB after the SRCBOX keyword\n"
   "For example: SRCBOX file=Myfile.polygon\n"
   "---------------------------------------------------------------\n";
   dbPrintToDevice("\n");
   dbPrintToDevice(help);
}
/* ------------------------------------------------------------------ */




static void HelpOnStaBox()
{
   char help[] =
   "ReadDB can also restrict seismograms to stations which are\n"
   "inside a user-defined polygonal geographic region. The \n"
   "polygon is defined by a set of lat-lon pairs which are the\n" 
   "vertices of the polygon. The vertices must be listed in\n"
   "counter-clockwise order, and the last vertex is expected to be\n"
   "the same as the starting vertex. The vertices may either be\n"
   "listed on the command line after the STABOX keyword or else\n"
   "they may be contained in a file with one vertex per line. To\n"
   "use the file, give its name to readDB after the STABOX keyword\n"
   "For example: STABOX file=Myfile.StaPolygon\n"
   "---------------------------------------------------------------\n";
   dbPrintToDevice("\n");
   dbPrintToDevice(help);
}
/* ------------------------------------------------------------------ */




static void HelpOnConnect()
{
   char help[] =
   "A login name and password are required to connect to the Oracle\n"
   "database. These are concatenated by readDB into a connect string\n"
   "of the form   loginName/Password  . ReadDB constructs a default\n"
   "connect string from your unix login name which is simply\n"
   "login/login. If this matches your Oracle name and password you\n"
   "do not need to do anything else to connect to Oracle using\n"
   "readDB. Otherwise, specify the connect string yourself either on\n"
   "the command line or in a configuration file. \n"
   "---------------------------------------------------------------\n";
   dbPrintToDevice("\n");
   dbPrintToDevice(help);
}
/* ------------------------------------------------------------------ */





static void HelpOnSourceRestrictions()
{
   char help[] =
   "The SRCLAT, SRCLON, and SRCDEP options can be used either singly\n"
   "or together to restrict seismograms read by readDB to sources\n"
   "contained within a certain geographical range. Each of these\n"
   "options requires both a minimum and maximum value. In the case\n"
   "of longitudes, first argument values greater than 180 have 360\n"
   "subtracted so that you may make selections like: SRCLON 355 20\n"
   "and get seismograms from sources between 5 degrees west\n"
   "longitude to 20 degrees east longitude rather than from the\n"
   "complementary range of longitudes.\n"
   "---------------------------------------------------------------\n";
   dbPrintToDevice("\n");
   dbPrintToDevice(help);
}
/* ------------------------------------------------------------------ */






static void HelpOnStationRestrictions()
{
   char help[] =
   "The STALAT, STALON, and INSTDEPTH options can be used singly\n"
   "or together to restrict seismograms read by readDB to stations\n"
   "contained within a certain geographical range. Each of these\n"
   "options requires both a minimum and maximum value. In the case\n"
   "of longitudes, first argument values greater than 180 have 360\n"
   "subtracted so that you may make selections like: STALON 355 20\n"
   "and get seismograms from sources between 5 degrees west\n"
   "longitude to 20 degrees east longitude rather than from the\n"
   "complementary range of longitudes.\n"
   "---------------------------------------------------------------\n";
   dbPrintToDevice("\n");
   dbPrintToDevice(help);
}
/* ------------------------------------------------------------------ */







static void HelpOnTime()
{
   char help[] =
   "The STARTTIME and ENDTIME options are used to select waveforms by\n"
   "time. If a StartTime is specified then only waveforms with begin\n"
   "times >= the specified time will be selected. If the EndTime is \n"
   "specified then only waveforms with endtimes <= the specified\n"
   "time will be used. \n\n"
   "There are two basic formats for specifying time, one using \n"
   "month/day format, and the other using day-of-year format.\n\n"
   "      YYYY/MM/DD/HH:MM:SS.SSS\n"
   "      YYYY/DDD/HH:MM:SS.SSS\n\n"
   "legal separators between tokens are '/ - : ' as well as the space\n"
   "and tab characters.\n\n"
   "Subsets of the formats are also supported as long as they are not\n"
   "ambiguous. For instance YYYY DDD HH will be interpreted as\n"
   "YYYY/DDD/HH:00:00.000. Note that if day-of year format is used and\n"
   "if the day-of year is given with only 2 digits, the string will be\n"
   "ambiguous unless the remainder of the string is complete to the\n"
   "second and the second has a non-zero number of milliseconds. I.E.\n"
   "1997 12 22 13 will be ambiguous, but 1997 12 22 13.45 will not be.\n"
   "-----------------------------------------------------------------\n";
   dbPrintToDevice("\n");
   dbPrintToDevice(help);
}
/* ------------------------------------------------------------------ */







static void HelpOnMagnitude()
{
   char help[] =
   "The MBMAG, MLMAG, and MSMAG options are used to restrict\n"
   "waveforms by source magnitude. Each of these options must be\n"
   "followed by a minimum value and a maximum value. To restrict by\n"
   "Mb use MBMAG, to restrict by Ml use MLMAG, and to restrict by Ms\n"
   "use MSMAG. Note that if the source has not been assigned a\n"
   "magnitude of the type specified, then it will be excluded.\n"
   "---------------------------------------------------------------\n";
   dbPrintToDevice("\n");
   dbPrintToDevice(help);
}
/* ------------------------------------------------------------------ */




static void HelpOnMaxrows()
{
   char help[] =
   "Some queries can return very large numbers of seismograms. If\n"
   "the memory required to load the seismograms approaches or\n"
   "exceeds physical memory, performance is likely to be degraded.\n"
   "Therefore readDB restricts all queries to return a maximum\n"
   "number of rows. This number can be set in a configuration file\n"
   "or it can be specified on the command line using the MAXROWS\n"
   "option. If you use the MAXROWS option you must follow it with\n"
   "a single positive number.\n"
   "---------------------------------------------------------------\n";
   dbPrintToDevice("\n");
   dbPrintToDevice(help);
}
/* ------------------------------------------------------------------ */





static void HelpOnStations(char *subOption)
{
   char *tmp;
   char help[] =
   "The STANAME and COMPONENT options may be used either singly or\n"
   "in combination to restrict the waveforms by station and/or\n"
   "component. Stations to select from can be listed on the \n"
   "command line in a space- or comma-delimited list, or the station\n"
   "names can be put in a file (one station name per line) and the\n"
   "file name can be specified on the command line using the syntax\n"
   "file=filename. Components may only be specified on the command\n"
   "line. Stations and components currently available are:\n";
   if(subOption){
      if(!strcmp(dbGetCaseSensitivity(), "OFF"))
         tmp = Upstring(subOption);
      else
         tmp = subOption;
      dbOracleHelpOnOneStation(tmp);
      return;
   }


   dbPrintToDevice("\n");
   dbPrintToDevice(help);
   dbOracleHelpOnStations();
   dbPrintToDevice(
   "---------------------------------------------------------------\n");

}
/* ------------------------------------------------------------------ */





static void HelpOnDefaults()
{
   char help[] =
   "ReadDB maintains an internal list of defaults used in queries\n"
   "to the Oracle database. You can see the current defaults by\n"
   "typing SHOWDEFAULTS on the readDB command line. This will\n"
   "produce a listing similar to:\n\n"
   "Max rows to return:    50\n"
   "Oracle login name:     dodge\n"
   "Oracle login password: dodge\n"
   "Restrict phases to:  \n"  
   "Restrict authors to:  \n\n"
   "All of these defaults can be set on the readDB command line.\n"
   "However, you may find it easier to set them in a configuration\n"
   "file. The configuration consists of a series of keyword=value\n"
   "pairs, one per line. The following is an example of the\n"
   "contents of a configuration file:\n\n"  
   "MaxRowsInQuery = 50;\n"
   "OracleLoginName = dodge\n"
   "! This is a comment\n"
   "# In fact all text preceded by !#/; are comments\n"
   "OraclePassword = dodge  !This is a trailing comment\n"
   "Phaselist = PcP, PcS, Pdiff,  Pg, Pn, Pp\n"
   "Authlist = tom, dick, harry\n\n"
   "The configuration file can be located anywhere in the file\n"
   "system where you have read access, and can have any legal name.\n"
   "However, you must make it known to readDB by setting the\n"
   "environment variable DBSELECT_CONFIG_FILE to the fully-qualified\n"
   "filename, e.g.\n"
   "setenv DBSELECT_CONFIG_FILE=/us/dodge/SeisMgr/dbselect.ini\n"
   "---------------------------------------------------------------\n";
   dbPrintToDevice("\n");
   dbPrintToDevice(help);
}
/* ------------------------------------------------------------------ */






static void HelpOnAuthors()
{
   char help[] =
   "By default readDB will find all picks associated with each\n"
   "seismogram it retrieves. If you wish to only get picks by\n"
   "certain authors you can specify your preferred authors, either\n"
   "in the configuration file or on the readDB command line. To\n"
   "specify one or more authors on the command line, use the\n"
   "AUTHLIST option followed by a space- or comma-delimited list\n"
   "of authors. Authors currently registered in the database are:\n"
   "---------------------------------------------------------------\n";
   dbPrintToDevice("\n");
   dbPrintToDevice(help);
   dbOracleHelpOnAuthors();
   dbPrintToDevice(
   "---------------------------------------------------------------\n");

}
/* ------------------------------------------------------------------ */






static void HelpOnPhases()
{
   char help[] =
   "By default readDB will find all picks associated with each\n"
   "seismogram it retrieves. If you wish to only get picks of\n"
   "certain phases you can specify your desired phases, either\n"
   "in the configuration file or on the readDB command line. To\n"
   "specify one or more phases on the command line, use the\n"
   "PHASELIST option followed by a space- or comma-delimited list\n"
   "of phases. Phases currently registered in the database are:\n"
   "---------------------------------------------------------------\n";
   dbPrintToDevice("\n");
   dbPrintToDevice(help);
   dbOracleHelpOnPhases();
   dbPrintToDevice(
   "---------------------------------------------------------------\n");

}
/* ------------------------------------------------------------------ */





static void HelpOnEventID()
{
   char help[] =
   "You can restrict the seismograms selected by readDB to be\n"
   "associated with a specific set of events by providing a list of\n"
   "evids to readDB. After the EVENTID option you can supply a \n"
   "comma- or space-delimited list of evids. Alternatively, give \n"
   "the name of a file containing the evid list (one evid per line)\n"
   "---------------------------------------------------------------\n";
   dbPrintToDevice("\n");
   dbPrintToDevice(help);
}
/* ------------------------------------------------------------------ */





static void HelpOnSrcDist()
{
   char help[] =
   "The SRCDIST and SRCDELT options both are used to restrict\n"
   "retrieved seismograms to be from events with epicenters within\n"
   "a distance range from a selected station. SRCDIST expects that\n"
   "the distance will be specified in kilometers and SRCDELT expects\n"
   "the distance to be in degrees. Both options require that one\n"
   "station has been selected using the STANAME option. If just one\n"
   "distance is specified, then both commands treat that distance as\n"
   "a maximum. If two distances are specified, then the selected\n"
   "events will be within the distance range bounded by the two\n"
   "values.\n"
   "---------------------------------------------------------------\n";
   dbPrintToDevice("\n");
   dbPrintToDevice(help);
}
/* ------------------------------------------------------------------ */




static void HelpOnSrcCircle()
{
   char help[] =
   "The SRCCIRCLE option is used to select seismograms from events\n"
   "with epicenters within a specified distance from some point on\n"
   "the earth's surface. You must specify the latitude and longitude\n"
   "of the circle and its radius in kilometers.\n"
   "---------------------------------------------------------------\n";
   dbPrintToDevice("\n");
   dbPrintToDevice(help);
}
/* ------------------------------------------------------------------ */




static void HelpOnQuadrant()
{
   char help[] =
   "The QUADRANT option is used to select seismograms from events\n"
   "with epicenters within a specified back-azimuth range from a\n"
   "selected station. You must specify one station using the\n"
   "STANAME option and you must specify a minimum back-azimuth\n"
   "and a maximum back-azimuth.\n"
   "---------------------------------------------------------------\n";
   dbPrintToDevice("\n");
   dbPrintToDevice(help);
}
/* ------------------------------------------------------------------ */




static void HelpOnCmdFile()
{
   char help[] =
   "If you frequently use the same options with, perhaps, minor\n"
   "variations from one use to the next, you may find it convenient\n"
   "to save your command string in a file and then have readDB read\n"
   "the commands from the file. The command string may be on one\n"
   "line just as you would type it at the readDB prompt, or may be\n"
   "spread over multiple lines. To process the command file use the\n"
   "CMDFILE option followed by the file name of command file. When\n"
   "you use the CMDFILE option, any other options on the readDB\n"
   "command line are ignored.\n"
   "---------------------------------------------------------------\n";
   dbPrintToDevice("\n");
   dbPrintToDevice(help);
}
/* ------------------------------------------------------------------ */




static void HelpOnShowSQL()
{
   char help[] =
   "ReadDB translates the options you specify into a series of SQL\n"
   "commands that are used to extract the data from the Oracle data\n"
   "base. To view the generated SQL use the SHOWSQL option. SHOWSQL\n"
   "can be used either with or without parameters. If used without\n"
   "parameters it acts as a toggle. That is, if the current state is\n"
   "to not show generated SQL, then issuing a command that includes\n"
   "the SHOWSQL option will change the state to ON. Otherwise, the\n"
   "state is changed to OFF. You can also pass the parameters ON\n"
   "and OFF to SHOWSQL to explicitly set the state.\n"
   "---------------------------------------------------------------\n";
   dbPrintToDevice("\n");
   dbPrintToDevice(help);
}
/* ------------------------------------------------------------------ */





void DisplaySpecificHelp(char *option, char *subOption)
{
   char *token;

   token = (char *) malloc(strlen(option) + 1 );
   strcpy(token,option);
   token = CheckForAlias(Upstring(token));

   if( !strcmp(token , "HELP"))
      HelpOnHelp();
   else if( !strcmp(token , "CONNECT"))
      HelpOnConnect();
   else if( !strcmp(token , "SRCLAT"))
      HelpOnSourceRestrictions();
   else if( !strcmp(token , "SRCLON"))
      HelpOnSourceRestrictions();
   else if( !strcmp(token , "SRCDEP"))
      HelpOnSourceRestrictions();
   else if( !strcmp(token , "SRCBOX"))
      HelpOnSourceBox();
   else if( !strcmp(token , "SRCDIST"))
      HelpOnSrcDist();
   else if( !strcmp(token , "SRCTYPE"))
      HelpOnSourceType();
   else if( !strcmp(token , "SRCDELTA"))
      HelpOnSrcDist();
   else if( !strcmp(token , "MBMAG"))
      HelpOnMagnitude();
   else if( !strcmp(token , "MLMAG"))
      HelpOnMagnitude();
   else if( !strcmp(token , "MSMAG"))
      HelpOnMagnitude();
   else if( !strcmp(token , "STARTTIME"))
      HelpOnTime();
   else if( !strcmp(token , "ENDTIME"))
      HelpOnTime();
   else if( !strcmp(token , "STANAME"))
      HelpOnStations(subOption);
   else if( !strcmp(token , "MAXROWS"))
      HelpOnMaxrows();
   else if( !strcmp(token , "COMPONENT"))
      HelpOnStations(subOption);
   else if( !strcmp(token , "STALAT"))
      HelpOnStationRestrictions();
   else if( !strcmp(token , "STALON"))
      HelpOnStationRestrictions();
   else if( !strcmp(token , "STABOX"))
      HelpOnStaBox();
   else if( !strcmp(token , "INSTDEPTH"))
      HelpOnStationRestrictions();
   else if( !strcmp(token , "SHOWDEFAULTS"))
      HelpOnDefaults();
   else if( !strcmp(token , "AUTHLIST"))
      HelpOnAuthors();
   else if( !strcmp(token , "EVENTID"))
      HelpOnEventID();
   else if( !strcmp(token , "SRCCIRCLE"))
      HelpOnSrcCircle();
   else if( !strcmp(token , "QUADRANT"))
      HelpOnQuadrant();
   else if( !strcmp(token , "PHASELIST"))
      HelpOnPhases();
   else if( !strcmp(token , "CMDFILE"))
      HelpOnCmdFile();
   else if( !strcmp(token , "SHOWSQL"))
      HelpOnShowSQL();
   else{
      dbPrintToDevice("No help available for (");
      dbPrintToDevice(token);
      dbPrintToDevice("). Did you mistype the option name?\n");
   }
   free(token);
}
/* --------------------------------------------------------------------------- */





