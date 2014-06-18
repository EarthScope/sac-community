/** 
 * @file   select.c
 * 
 * @brief  Control input from different sources
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include <sys/select.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <limits.h>

#include <unistd.h>
#include <errno.h>

#include "config.h"

#ifdef USE_TERMIOS
#include <termios.h>
#endif /* USE_TERMIOS */

#include "string_utils.h"
#include "proto.h"
#include "bool.h"
#include "select.h"
#ifdef USE_X11
#include "gd3.x11.h"
#endif
#include "gdm.h"
#include "comlists.h"
#include "debug.h"

/** 
 * @param SAC_HISTORY_MAX
 *     1024
 *     Maximum size of the internal sac history
 */
#define SAC_HISTORY_MAX 1024

static int sac_history_size = SAC_HISTORY_MAX;

/** 
 * Set the internal history size 
 * 
 * @param value 
 *    New history size 
 *    - < 0 sets the value to its maximum [ SAC_HISTORY_MAX ]
 */
void
history_size_set(int value) {
  if (value <= 0) {
    sac_history_size = INT_MAX;
  } else {
    sac_history_size = value;
  }
  return;
}

/** 
 * Get the current history size
 * 
 * @return 
 *    Current histroy size
 *
 * @see SAC_HISTORY_MAX
 * @see history_size_set()
 *
 */
int
history_size() {
  return sac_history_size;
}

/** 
 * Set and Get a message from the command line
 * 
 * @param p 
 *    Command from the command line
 * @param len 
 *    - > 0 Get the command, length of \p p
 *    - < 0 Set and save the command
 * 
 * @return 
 *   Command from the command line
 *
 */
int
select_loop_message(char *p, int len) {
  static char *str = NULL;

  if(len <= 0) {
    if(str) {
      free(str);
      str = NULL;
    }
    if(!p) {
      return 0;
    }
    str = strdup(p);
    return(strlen(str));
  }
  if(!str) {
      str = strdup("        ");
  }

  len = len - 2; /* 1 for \0 and 1 for a space */
  len = ((int)strlen(str) > len) ? len : (int)strlen(str);

  strncpy(p, str, len);
  p[len++] = ' ';  /* Space */
  p[len++] = '\0'; /* String Teminator */

  return(len);
}

/** 
 * Toggle the select loop on and off
 * 
 * @param w 
 *    - SELECT_ON  - Turn on select loop
 *    - SELECT_OFF - Turn off select loop
 *    - SELECT_QUERY - Get select loop status (do not set)
 * 
 * @return 
 *    Current select loop status
 *
 */
int
select_loop_continue(int w) {
  static int flag;
  if(w == SELECT_ON || w == SELECT_OFF) {
    flag = w;
  }
  return(flag);
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


/** 
 * Fix a timeval structure; place extra microseconds into seconds
 * 
 * @param t 
 *    Timeval structure to fix
 *
 */
void
timeval_fix(struct timeval *t) {
  if(t) {
    t->tv_sec  = t->tv_sec  + floor(t->tv_usec /(1000 * 1000));
    t->tv_usec = t->tv_usec - t->tv_sec * (1000 * 1000);
  }
}

#ifndef TERMIOS
/** 
 * Check if input is available from the file descriptor
 * 
 * @param i 
 *    File Descriptor
 * @param fd 
 *    File Descriptor Set (Collection)
 * 
 * @return 
 *    - TRUE if input is available on \p i
 *    - FALSE if input is not available on \p i
 *
 */
static int
input(int i, fd_set *fd) {
  if(i >= 0) {
    return(FD_ISSET(i, fd));
  }
  return(0);
}
#endif /* TERMIOS */

/** 
 * Determine whether to display a prompt or not. Depends on the enviornment
 *    variable SAC_SCRIPT_PROMPT_DISPLAY and if sac has a controlled tty
 *
 * @return 
 *   - TRUE - if the prompt is desired
 *   - FALSE - if the prompt is not desired
 *
 */
int
show_prompt_without_tty(int getset) {
  static int flag = -1;
  char *c;
  if(getset == OPTION_ON || getset == OPTION_OFF) {
    flag = getset;
  }
  if(flag == -1) {
    flag = FALSE;
    if((c = getenv("SAC_SCRIPT_PROMPT_DISPLAY")) != NULL) {
      if(strcmp(c,"1") != 0 && strcmp(c,"0") != 0) {
        fprintf(stderr, 
                "SAC warning: SAC_SCRIPT_PROMPT_DISPLAY must be 0 or 1\n");
        flag = FALSE;
      } else{
        flag = atoi(c);
      }
    }
  }
  return flag;
}

static char *sac_history_filename = NULL;
static int sac_history_loaded = FALSE;

void
sac_history_filename_free() {
  FREE(sac_history_filename);
}

/** 
 * Set the sac_history filename. Free any previous history name.
 *    Copy the name if it is specified, otherwise derive the name
 *    from the user's home directory and the variable SAC_HISTORY_FILE
 * 
 * @param name 
 *    New file name for the sac history file
 *
 * @see SAC_HISTORY_FILE
 *
 */
void
sac_history_file_set(char *name) {
  int len;
  char *sachistory;
  char *home;
  if (sac_history_filename) {
    /* Free previous, if any */
    free(sac_history_filename);
    sac_history_filename = NULL;
  }
  if (name) {
    /* Duplicate for save -- never know whether static area pointed at! */
    sac_history_filename = strdup(name);
  } else {
    /* Null name signifies default */
    home = getenv("HOME");
    if(home) {
      len = strlen(home) + strlen(SAC_HISTORY_FILE) + 2;
      sachistory = (char *)malloc(sizeof(char) * len);
      sprintf(sachistory,"%s/%s", home, SAC_HISTORY_FILE);
      sachistory[len-1] = '\0';
    } else {
      sachistory = NULL;
    }
    sac_history_filename = sachistory;
  }
}

/** 
 * Get the sac history filename
 *
 * @return 
 *    File name for the sac history
 */
char *
sac_history_file() {
  return sac_history_filename;
}

/** 
 * Load the sac history file from a file
 * 
 * @param where 
 *    Filename to load the history from
 *
 */
void
sac_history_load(char *where) {
  stifle_history( history_size() );
  if(where) {
    read_history(where);
  }
  sac_history_loaded = TRUE;
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
      free(line);
      line = NULL;
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

char *
strdup_trim(char *s) {
  int n;
  char *p, *out;
  p = strchr(s, ' ');
  if(!p) {
    n = strlen(s);
  } else {
    n = (p - s) + 1;    
  }
  n = n + 1;
  out = (char *) malloc(sizeof(char) * n);
  strncpy(out, s, n-1);

  out[n] = 0;
  return out;
}

char *
strdup_lower(char *in) {
  char *p;
  char *out = strdup_trim(in);
  p = out;
  while(*p != '\0') {
    *p = tolower(*p);
    p++;
  }
  return out;
}

int
string_case_compare(const void *pa, const void *pb) {
  char **a = (char **) pa;
  char **b = (char **) pb;
  return strcasecmp(*a,*b);
}

char **
uniq(char **s, int *np) {
  int i, j, n;
  char **tmp;
  n = *np;
  if(n <= 0) {
      return s;
  }
  /* Sort Strings */
  qsort(s, n, sizeof(char *), string_case_compare);
  
  /* Count unique strings */
  tmp = (char **) malloc(sizeof(char *) * n);
  j = 1;
  tmp[0] = strdup(s[0]);
  for(i = 1; i < n; i++) {
    if(strcasecmp(s[i], s[i-1]) != 0) {
      tmp[j++] = strdup(s[i]);      
    }
  }
  n = j;
  for(i = 0; i < n; i++) {
    free(s[i]);
    s[i] = NULL;
  }
  free(s);
  s = NULL;
  *np = j;

  return tmp;
}

char ** 
uniq_cmd(int *np) {
  int i,j,n;
  char **cmd;

  n = Ncomlistentries[3] + Icomliststart[3];
  cmd = (char **) malloc(sizeof(char *) * n);
  j = 0;
  for(i = 0; i < n; i++) {
    if(kmcomlists.kcomnames_full[i][0] != ' ' &&
       kmcomlists.kcomnames_full[i][0] != 0 ) {
      cmd[i] = strdup_lower(kmcomlists.kcomnames_full[i]);
      j++;
    }
  }
  n = j;
  cmd = uniq(cmd, &n);
  *np = n;
  return cmd;
}


char *
sac_attempt_complete_command(const char *text, int state) {
  static int i, n, len;
  static int init = TRUE;
  static char **cmd;
  int j;

  if(init) {
    init = FALSE;
    cmd = uniq_cmd(&n);
  }

  /* New Word -- initialize */
  if(!state) {
    i = 0;
    len = strlen(text);
  }
  while(i < n) {
    j = i++;
    if(strncasecmp(cmd[j], text, len) == 0) {
      return strdup(cmd[j]);
    }
  }
  return (char *) NULL;
}

int
is_single_match(char **s) {
  int j;
  if(s == NULL) { /* No matches */
    return FALSE;
  }
  /* Count matches */
  j = 0;
  while(s[j] != NULL) {
    j++;
  }

  /* Matches
   * #1 - Replacement text
   * #2 - 1st Possible completion (Single)
   * #3 - 2nd possible complettion (Not single)
   */
  if(j <= 2) {
    return TRUE;
  }
  return FALSE;
}

int
is_directory(char *s) {
  struct stat stbuf;
  if(stat(s, &stbuf) == 0) {
    return S_ISDIR(stbuf.st_mode);
  }
  return FALSE;
}

#ifndef TERMIOS
char **
sac_attempt_complete(const char *text, int start, int end) {
  char **matches;
  UNUSED(end);
  rl_completion_append_character = '\0';
  if(start == 0) {
    matches = rl_completion_matches(text, sac_attempt_complete_command);
    if(is_single_match(matches)) {
      rl_completion_append_character = ' ';      
    }
  } else {
    matches = rl_completion_matches(text, rl_filename_completion_function);
    if(is_single_match(matches)) {
      if(is_directory(matches[0])) {
        rl_completion_append_character = '/';
      } else {
        rl_completion_append_character = ' ';
      }
    }
  }
  return matches;
}

/** 
 * Select different input from a variety of sources. Primarilly 
 *    the command line (stdin) through readline/editline and the X11
 *    window system.
 * 
 * @param prmt 
 *    Prompt to display on the command line
 * @param prmtlen 
 *    Length of \p prmt
 * @param msg 
 *    Output message from the command line
 * @param msglen 
 *    Length of \p msg
 * @param timeout 
 *    Timeout value if requested
 * @param func 
 *    Function to call when a full command line has been entered
 * 
 * @return 
 */

#ifdef OSX_APP
static int osx_fd[2];
static char value = 'a';
void
osx_gui_command(char *cmd) {
  size_t n;
  n = strlen(cmd);
  write(osx_fd[1], &n, sizeof(size_t));
  write(osx_fd[1], cmd, n * sizeof(char) );
}

#endif

int
select_loop(char *prmt, int prmtlen, 
	    char *msg, int msglen, 
	    struct timeval *timeout, 
	    VCPFunction *func) {

  int i;	/* index for prefilling string w/ NULLs */

  int retval;
  fd_set fd;
  int max_fd, stdin_fd;
#ifdef X11_APP
  int x11_fd;
#endif
  int nerr;
  char kprmt[128];
  char *getline_msg;

  UNUSED(prmtlen);

  if(!sac_history_loaded)
    sac_history_load(sac_history_file());

  /* Show the Prompt */
  i = 0;
  while(prmt[i] != '$') {
    kprmt[i] = prmt[i];
    i++;
  }
  kprmt[i] = '\0';
  
  if(use_tty()) {
    rl_callback_handler_install(kprmt, func);
    rl_completion_append_character = '\0';
    rl_attempted_completion_function = sac_attempt_complete;
  }

  fflush(stdout);
  /* Take care of printing the prompt when there is no tty
   *    This normally happends during script processing 
   */
  if(!use_tty() && show_prompt_without_tty( OPTION_GET )) { 
    fprintf(stdout, "%s", kprmt);
    fflush(stdout);
  }

  stdin_fd = -1;

  /* Loop until we encounter a newline */
  select_loop_continue(SELECT_ON);

  timeval_fix(timeout);

  /* Clear out any pending events */
  handle_event(&nerr);

  while(select_loop_continue(SELECT_QUERY)) {
    max_fd = -1;
    FD_ZERO(&fd);

    /* Add STDIN to the File Descriptor Set (FD_SET) */
    if(!use_tty() && timeout) { /* This is here due to co/xpause.c and co/zsleep.c */
      max_fd = -1;
    } else {
      DEBUG("Adding stdin for select\n");
      stdin_fd =  0;
      FD_SET(stdin_fd, &fd);
      if(stdin_fd > max_fd) {
        max_fd = stdin_fd;
      }
    }
#ifdef X11_APP
    /* Add X11 to the File Descriptor Set (FD_SET) */
    if((x11_fd = get_file_descriptor()) > 0) {
      DEBUG("Adding x11 for select\n");
      FD_SET(x11_fd, &fd);
      if(x11_fd > max_fd) {
        max_fd = x11_fd;
      }
    }
#endif 

#ifdef OSX_APP
    pipe(osx_fd);
    FD_SET(osx_fd[0], &fd);
    if(osx_fd[0] > max_fd) {
      max_fd = osx_fd[0];
    }
#endif

    /* Wait until we get life from one the File Descriptors, then act */
    DEBUG("select wait\n");
    retval = select(max_fd+1, &fd, NULL, NULL, timeout);
    DEBUG("retval: %d\n", retval);
    switch(retval) {
    case -1: /* Error Condition */
      if(errno != EINTR) {
	perror("SAC: Select Error");
	exit(-1);
      }
      break;
    case 0: /* Timeout Expired */
	break;
    default:
      if(input(stdin_fd, &fd)) {
        if(!use_tty()) { 
          if( (getline_msg = getline_stdin()) != NULL ) {
            select_loop_message(getline_msg, -1);
            select_loop_continue(SELECT_OFF);
            free(getline_msg);
            getline_msg = NULL;
          }
        } else {
          rl_callback_read_char();
        }
	if(!use_tty() && select_loop_continue(SELECT_QUERY)) {
	  /* This assumes the the entire line is read in at once and processline is called
	     for each entry into rl_callback_read_char().  This will probably break on
	     some machine, some where, probably when using the GNU readline library. 
	  */
	  fprintf(stderr, "SAC Error: EOF/Quit\n"
              "     SAC executed from a script: quit command missing\n"
              "     Please add a quit to the script to avoid this message\n"
              "     If you think you got this message in error, \n"
              "     please report it to: %s\n", PACKAGE_BUGREPORT);
	  select_loop_continue(SELECT_OFF);
	  select_loop_message("quit", -1);
	}
      }
#ifdef X11_APP      
      if(input(x11_fd, &fd)) {
        handle_event( &nerr );
      }
#endif
#ifdef OSX_APP
      if(input(osx_fd[0], &fd)) {
        size_t n;
        char pmsg[1024];
        memset(pmsg,0,sizeof(pmsg));
        read(osx_fd[0], &n, sizeof(size_t) ); /* Length */
        read(osx_fd[0], &pmsg[0], n );        /* Message */
        select_loop_continue(SELECT_OFF);
        select_loop_message(pmsg, -1);
      }
#endif
    }

    if(timeout) {
      return(0);
    }
  }
  select_loop_message(msg, msglen);
  select_loop_continue(SELECT_OFF);
  return(0);
}

#endif /* __MINGW32__ */
