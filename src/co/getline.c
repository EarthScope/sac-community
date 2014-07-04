/** 
 * @file   getline.c
 * 
 * @brief  Get a line from a file descriptor
 * 
 */
#include "config.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef USE_TERMIOS
#include <termios.h>
#endif /* USE_TERMIOS */

#include "select.h"
#include "debug.h"
#include "co.h"

/** 
 * Read a line from a file descriptor and null terminate
 * 
 * @param pfd 
 *    File descriptor
 * @param pch 
 *    Character string
 * @param maxlen 
 *    Length of \p pch
 * 
 * @return Number of characters read
 *
 * @bug Should be replaced by fgets() or equivalent and looks very similar 
 *      to getfline() already in sac
 *      Only used by zgtmsg() when readline is on
 *      and zfiles() (which should be replaced too with a fnmatch or glob)
 *
 */ 
int
getline_sac(FILE *pfd,
	char *pch,
	int   maxlen) {

	char *pchsave;	/* save string pointer */
	int   i;	/* index and number of characters read */
	short ichar;	/* used to read characters--is integer so EOF (usually
			   a -1 can be read) */
 
	pchsave = pch;
    ichar = '\0';
	for (i=0;i<(maxlen-1) && (ichar=getc(pfd))!='\0' && ichar!='\n' &&
		ichar!=EOF;++i)
			*(pchsave++) = (char) ichar;
    if(ichar == EOF) {
        i = EOF;
    }

	*(pchsave) = '\0';
 
	return(i);
}
 

char * 
getline_stdin() {
  char *line;
  size_t lenmax, len;
  int c;

  lenmax = 2;
  line = (char *) malloc(lenmax);
  line[0] = 0;
  len = 0;
  if(line == NULL) {
    return NULL;
  }
  while(1) {
    c = fgetc(stdin);
    if(c == EOF) {
      FREE(line);
      return NULL;
    }
    if(len + 1 >= lenmax) { 
      lenmax *= 2;
      char *linep = (char *) realloc(line, lenmax);
      if(linep == NULL) {
        free(line);
        line = NULL;
        return NULL;
      }
      line = linep;
    }
    if(c == '\n') { /* Return on a newline */
      break;
    }
    line[len]   = c;
    line[len+1] = 0; /* String Terminator */
    len++;
  }
  return line;
}


int
tty_force(int getset) {
  static int use = -1;
  if(getset != OPTION_GET) {
    use = getset;
  }
  return use;
}

#ifdef USE_TERMIOS
/**  
 * Determine if the tty (terminal) is in use.  The terminal is 
 *   normally disabled during scripts/
 * 
 * @return 
 *    - TRUE - Terminal is active
 *    - FALSE - Terminal is not active
 */
int
use_tty() {
  static int use = -1;

  if(use == -1 && tty_force(OPTION_GET) != -1) {
    use = tty_force(OPTION_GET);
  }

  if(use == -1) {
    struct termios t;
    FILE *rl_instream = stdin;
    if(tcgetattr(fileno(rl_instream), &t) == -1) {
      /*      perror("tcgetattr warning:");*/
      use = 0;
    } else {
      use = TRUE;
    }
  }
  return(use);
}
#else
int
use_tty() {
	return FALSE;
}
#endif /* TERMIOS */

