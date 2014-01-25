/** 
 * @file xprint.c
 *
 * @ brief Print the last SGF file 
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "gam.h"
#include "gd2.h"
#include "exm.h"
#include "bool.h"
#include "string_utils.h"

#include "cpf.h"

#include "select.h"

int
sac_print_disabled(int getset) {
    static int virgin = TRUE;
    static int print_disabled = FALSE;
    if(getset == OPTION_ON || getset == OPTION_OFF) {
        print_disabled = getset;
        virgin = FALSE;
    }
    if(virgin) {
        virgin = FALSE;
        print_disabled = env_bool("SAC_PRINT_DISABLED", print_disabled);
    }
    return print_disabled;
}

int
sac_print_verbose(int getset) {
    static int virgin = TRUE;
    static int print_verbose = FALSE;
    if(getset == OPTION_ON || getset == OPTION_OFF) {
        print_verbose = getset;
        virgin = FALSE;
    }
    if(virgin) {
        virgin = FALSE;
        print_verbose = env_bool("SAC_PRINT_VERBOSE", print_verbose);
    }
    return print_verbose;
}

char *
tmpfile_create(char *template, int xs) {
  int fd;
  char *file = strdup(template);
  if (!file)
    return NULL;
  fd = mkstemps(file, xs);
  if(fd < 0) {
    free(file);
    file = NULL;
  }
  close(fd);
  return file;
}

int
sgf_to_ps(char *sgf, char *ps) {
  int retval;
  char *command = NULL;
  if(!sgf || !*sgf) {
    fprintf(stderr, "sgf_to_ps: No SGF file for conversion from\n");
    return -1;
  }
  if(!ps || !*ps) {
    fprintf(stderr, "sgf_to_ps: No PS file for conversion to\n");
    return -1;
  }
  asprintf(&command, "sgftops %s %s ", sgf, ps);
  if(!command) {
    fprintf(stderr, "sgf_to_ps: Error allocating memory for command\n");
    return -1;
  }
  if(sac_print_verbose(OPTION_GET)) {
    fprintf(stderr, "sgf:   %s\n", sgf);
    fprintf(stderr, "ps:    %s\n", ps);
  }
  retval = system(command);
  free(command);
  command = NULL;
  return retval;
}

int
ps_print(char *ps, char *printer) {
  int retval = 0;
  char *command = NULL;
  if(!ps || !*ps) {
    fprintf(stderr, "ps_print: No PS file to print\n");
    return -1;
  }
  if(printer && printer[0] != 0) {
    asprintf(&command, "lpr -P %s %s", printer, ps);
  } else {
    asprintf(&command, "lpr %s", ps);
  }
  if (!command) {
    fprintf(stderr, "ps_print: Error allocating memory for command\n");
    return -1;
  }
  if(sac_print_verbose(OPTION_GET)) {
      fprintf(stderr, "print: %s\n", command);
  }
  if(!sac_print_disabled(OPTION_GET)) {
      retval = system(command);
  }
  if(sac_print_verbose(OPTION_GET)) {
      fprintf(stderr, "%% ls /tmp/sac*\n");
      system("ls /tmp/sac*");
  }
  free(command);
  command = NULL;
  return retval;
}

int
sgf_print(char *sgf, char *printer) {
  int retval;
  char *ps;
  if(!sgf || !*sgf) {
    fprintf(stderr, "sgf_print: No SGF file to print\n");
    return -1;
  }
  ps = tmpfile_create("/tmp/sac_temp_XXXXXX.ps", 3);
  if(!ps) {
    fprintf(stderr, "Error creating tmp PS file\n");
    return -1;
  }
  if(sgf_to_ps(sgf, ps)) {
    retval = -1;
    goto ERROR;
  }
  retval = ps_print(ps, printer);

 ERROR:
  unlink(ps);
  free(ps);
  return retval;
}

/*
 * To execute the action command PRINT. This command makes a hardcopy of the 
 *     last SGF file produced
 *
 * @param nerr
 *    Error return flag.  Set to 0 if no error occurred.
 *
 * @see kmgd2.kfilename
 *
 * @see lcchar
 *
 * MODIFICATION HISTORY:
 *      130521: took out 2.5 for default line width
 *	060531: Erased /tmp/sactemp.ps after printing jas/vt
 *	999422:	Original version.  
 *
 */
void 
xprint ( int *nerr ) 
{
	char printerName[ 80 ];
	int unused ;

        memset(printerName, 0, 80);

        *nerr = 0;

        /* PARSING PHASE: */

        /* - There will be one or fewer tokens:  printer name */

    if ( lcchar ( 79 , printerName , 80 , &unused ) ) { }
	    
	/* EXECUTION PHASE */

	if ( kmgd2.kfilename[ 0 ] ) {	/* if there is an SGF file */
          sgf_print(kmgd2.kfilename, printerName);
	}
	else 		/* if no SGF files have been produced */
	    *nerr = 2405 ;
}
