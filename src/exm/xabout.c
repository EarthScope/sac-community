/** 
 * @file   xabout.c
 * 
 * @brief  About SAC
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "msg.h"
#include "select.h"
#include "bool.h"
#include "co.h"

#include "proto.h"

#include "config.h"

char *env_on[]  = {"on",  "true",  "yes", "1" };
char *env_off[] = {"off", "false", "no",  "0" };

/** 
 * Get an environment variable, logical value
 * 
 * @param env 
 *    Environment varaible name
 * @param def 
 *    Default value, if variable is not found
 * 
 * @return 
 *    Value of the environment variable
 *
 */
int
env_bool(char *env, int def) {
  int i;
  int n;
  char *env_string = getenv(env);
  if(env_string != NULL) {
    n = strlen(env_string);
    for(i = 0; i < (int)(sizeof(env_on)/sizeof(char *)); i++) {
      if(strncasecmp(env_string, env_on[i], min(n, strlen(env_on[i]))) == 0) {
        return TRUE;
      }
    }
    for(i = 0; i < (int)(sizeof(env_off)/sizeof(char *)); i++) {
      if(strncasecmp(env_string, env_off[i], min(n, strlen(env_off[i]))) == 0) {
        return FALSE;
      }
    }
  }
  return def;
}


/** 
 * Display the Copyright
 * 
 * @param getset 
 *    - OPTION_ON  - Show Copyright (Set the value)
 *    - OPTION_OFF - Do not show Copyright (Set the value)
 *    - OPTION_GET - Get the copyright value (Do not set)
 *
 * @return 
 *    - TRUE - Show the copyright
 *    - FALSE - Do not show the copyright
 *    
 */
int
display_copyright(int getset) {
  static int virgin = TRUE;
  static int show_copyright = TRUE;
  if(virgin) {
    virgin = FALSE;
    show_copyright = env_bool(SAC_DISPLAY_COPYRIGHT, show_copyright);
  }
  if(getset == OPTION_ON || getset == OPTION_OFF) {
    show_copyright = getset;
    virgin = FALSE;
  }
  return show_copyright;
}

/** 
 * Use the SeisMgr Database
 * 
 * @param getset 
 *    - OPTION_ON  - Use Database (Set the value)
 *    - OPTION_OFF - Do not use the database (Set the value)
 *    - OPTION_GET - Get the use the database value (Do not set)
 *
 * @return 
 *    - TRUE - Use the database
 *    - FALSE - Do not use the database
 *    
 */
int
use_database(int getset) {
  static int virgin = TRUE;
  static int use_db = TRUE;  /* Database is ON by Default: SAC_USE_DATABASE */
  if(getset == OPTION_ON || getset == OPTION_OFF) {
    use_db = getset;
    virgin = FALSE;
  }
  if(virgin) {
    virgin = FALSE;
    use_db = env_bool(SAC_USE_DATABASE, use_db);
  }
  return use_db;
}

/** 
 * Display the about message if requested
 * 
 */
void 
xabout ( ) {
    char kvdate[200];
    /* char fmt[] = "SEISMIC ANALYSIS CODE [%s (Version 00.59.49)]"; */
    char fmt[] = "SEISMIC ANALYSIS CODE [%s (Version %s)]";
    char kcopyr[] = "Copyright 1995 Regents of the University of California\n" ;

    if(! display_copyright(OPTION_GET)) {
      return;
    }
    sprintf( kvdate, fmt, BUILD_DATE, PACKAGE_VERSION );
    setmsg( "OUTPUT", 99 );
    apcmsg( kvdate, strlen ( kvdate ) + 1 );
    aplmsg( kcopyr, strlen ( kcopyr ) + 1 );
    outmsg();
    clrmsg();
}

