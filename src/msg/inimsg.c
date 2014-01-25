/** 
 * @file   inimsg.c
 * 
 * @brief  Initialize the Message Subsystem
 * 
 */

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "string_utils.h"

#include "msg.h"
#include "bool.h"
#include "dff.h"

/** 
 * Initialize the Message Subsystem
 *
 * \return Nothing
 *
 * \see sacmsg outmsg sendmesg
 * \see t_kmmsg.ktpmsg
 * \see t_cmmsg.nummsg
 * \see t_cmmsg.itpmsg
 * \see t_cmmsg.autoout
 * \see t_cmmsg.nfmsg
 * \see t_cmmsg.nunits
 * \see t_cmmsg.iunits
 *
 * \date   881230:  Added activation of standard output for messages.
 * \date   860203:  Added options for message storage in common.
 * \date   841005:  Original version.
 * \date   881230:  Documented/Reviewed
 */
void 
inimsg()
{
	int send_[MTPMSG];
	int nerr;
        int i;
	int *const Send_ = &send_[0] - 1;

	strcpy( kmmsg.ktpmsg[MERRORS - 1], "ERRORS  " );
	strcpy( kmmsg.ktpmsg[MWARNINGS - 1], "WARNINGS" );
	strcpy( kmmsg.ktpmsg[MOUTPUT - 1], "OUTPUT  " );
	strcpy( kmmsg.ktpmsg[MCOMMANDS - 1], "COMMANDS" );
	strcpy( kmmsg.ktpmsg[MMACROS - 1], "MACROS  " );
	strcpy( kmmsg.ktpmsg[MPROCESSED - 1], "PROCESSE" );

	cmmsg.nummsg = 0;
	cmmsg.itpmsg = 0;
	cmmsg.autoout = FALSE;

	cmmsg.nfmsg = 0;
	sacmsg( &nerr );
	if( nerr != 0 )
		outmsg();

	cmmsg.nunits = 0;

        for (i=0; i<MUNITS; i++){
          cmmsg.iunits[i] = NULL;
	}

        /* Errors, Warnings and Output sent to STDOUT */
	Send_[MERRORS]    = TRUE;
	Send_[MWARNINGS]  = TRUE;
	Send_[MOUTPUT]    = TRUE;
	Send_[MCOMMANDS]  = FALSE;
	Send_[MMACROS]    = FALSE;
	Send_[MPROCESSED] = FALSE;
	sendmesg( MUNOUT, TRUE, send_ );

        /* Nothing sent to STDERR */
	Send_[MERRORS]    = FALSE;
	Send_[MWARNINGS]  = FALSE;
	Send_[MOUTPUT]    = FALSE;
	Send_[MCOMMANDS]  = FALSE;
	Send_[MMACROS]    = FALSE;
	Send_[MPROCESSED] = FALSE;
	sendmesg( stderr, TRUE, send_ );

	return;
}

void
check_message_subsystem() {
  if(cmmsg.nunits < 2) {
    inimsg();
  }
}

void
sac_message_control(int type, FILE *stream, int value) {
  int i;
  
  check_message_subsystem();

  /* Loop over all output streams */
  for(i = 0; i < cmmsg.nunits; i++) {
    if(stream == cmmsg.iunits[i]) {
      cmmsg.lsend[i][type-1] = value;
      return;
    }
  }
  
}

/* Warnings */
void 
sac_warning_control(FILE *stream, int value) {
  sac_message_control(MWARNINGS, stream, value);
}
void sac_warning_stdout()  { 
  sac_message_control(MWARNINGS, stdout, TRUE); 
  sac_message_control(MWARNINGS, stderr, FALSE); 
}
void sac_warning_stderr()  { 
  sac_message_control(MWARNINGS, stdout, FALSE); 
  sac_message_control(MWARNINGS, stderr, TRUE); 
}
void sac_warning_off() { 
  sac_message_control(MWARNINGS, stdout, FALSE); 
  sac_message_control(MWARNINGS, stderr, FALSE); 
}

void sac_warning_stdout_()  { sac_warning_stdout(); }
void sac_warning_stdout__() { sac_warning_stdout(); }
void sac_warning_stderr_()  { sac_warning_stderr(); }
void sac_warning_stderr__() { sac_warning_stderr(); }
void sac_warning_off_()     { sac_warning_off(); }
void sac_warning_off__()    { sac_warning_off(); }

/* Errors */
void 
sac_error_control(FILE *stream, int value) {
  sac_message_control(MERRORS, stream, value);
}
void sac_error_stdout()  { 
  sac_message_control(MERRORS, stdout, TRUE); 
  sac_message_control(MERRORS, stderr, FALSE); 
}
void sac_error_stderr()  { 
  sac_message_control(MERRORS, stdout, FALSE); 
  sac_message_control(MERRORS, stderr, TRUE); 
}
void sac_error_off() { 
  sac_message_control(MERRORS, stdout, FALSE); 
  sac_message_control(MERRORS, stderr, FALSE); 
}

void sac_error_stdout_()  { sac_error_stdout(); }
void sac_error_stdout__() { sac_error_stdout(); }
void sac_error_stderr_()  { sac_error_stderr(); }
void sac_error_stderr__() { sac_error_stderr(); }
void sac_error_off_()     { sac_error_off(); }
void sac_error_off__()    { sac_error_off(); }

/* Output */
void sac_output_stdout() {
  sac_message_control(MOUTPUT, stdout, TRUE);
  sac_message_control(MOUTPUT, stderr, FALSE);
}
void sac_output_stderr() {
  sac_message_control(MOUTPUT, stdout, FALSE);
  sac_message_control(MOUTPUT, stderr, TRUE);
}
void sac_output_off() {
  sac_message_control(MOUTPUT, stdout, FALSE);
  sac_message_control(MOUTPUT, stderr, FALSE);
}

void sac_output_stdout_()  { sac_output_stdout(); }
void sac_output_stdout__() { sac_output_stdout(); }
void sac_output_stderr_()  { sac_output_stderr(); }
void sac_output_stderr__() { sac_output_stderr(); }
void sac_output_off_()     { sac_output_off(); }
void sac_output_off__()    { sac_output_off(); }

#define MESSAGE_PREFIX "Number"

char *message_prefix[] = { "", 
			   "ERROR", 
			   "WARNING", 
			   "OUTPUT", 
			   "COMMANDS", 
			   "MACROS",
			   "==>" 
};

char *
getsmsg2(int number) {
  int j;
  for(j = 1; j <= cmmsg.nfmsg; j++) {
    if(number == cmmsg.ifmsg[j]) {
      return fstrdup(kmmsg.kfmsg[j], MCMSG+1);
    }
  }
  return NULL;
}

char *
encode_error(int number) {
  char *msg;
  int len;
  len = strlen(MESSAGE_PREFIX) + 5 + 1;
  msg = (char *)malloc(sizeof(char) * len);
  snprintf(msg, len, "%s%5d", MESSAGE_PREFIX, number);
  return msg;
}

#define ADD_MESSAGE_NUMBERS FALSE

int
msg(int error, int type, char *message, va_list args) {
  char *msg;
  string *str = NULL;

  if(error >= 100) {
    msg = getsmsg2(error);
  } else {
    msg = encode_error(error);
  }
  cmmsg.nummsg = error;
  cmmsg.itpmsg = type;

  str = string_new("");
  if( ADD_MESSAGE_NUMBERS ) {
    if(type == MERRORS) {
      string_printf_append(str, " %s %4d: %s ", 
                           message_prefix[type], error, msg);
    }else if(type == MWARNINGS) {
      string_printf_append(str, " %s: %s ", message_prefix[type], msg);
    }else if(type == MPROCESSED) {
      string_printf_append(str, " %s ", message_prefix[type]);
    } else {
      string_append(str, " ");
    }
  } else {
    if(type == MERRORS) {
      string_printf_append(str, "%s ", msg);
    }else if(type == MWARNINGS) {
      string_printf_append(str, "%s ", msg);
    }else if(type == MPROCESSED) {
      string_printf_append(str, " ");//message_prefix[type]);
    } else {
      string_append(str, " ");
    }
    
  }
  string_printf_append_internal(str, message, args);
  apcmsg(string_string(str), string_length(str) + 1);
  string_free(&str);
  return TRUE;
}

void
error(int error, char *message, ...) {
  va_list args;
  va_start(args, message);
  msg(error, MERRORS, message, args);
  va_end(args);
}

void
warning(int warning, char *message, ...) {
  va_list args;
  va_start(args, message);
  msg(warning, MWARNINGS, message, args);
  va_end(args);
}

void
processed(int val, char *message, ...) {
  va_list args;
  va_start(args, message);
  msg(val, MPROCESSED, message, args);
  va_end(args);
  outmsg();
  clrmsg();
}

void
out(char *fmt, ...) {
  char *str;
  va_list ap;
  va_start(ap, fmt);
  vasprintf(&str, fmt, ap);
  va_end(ap);
  apcmsg(str, strlen(str) + 1);
  free(str);
  str = NULL;
}

void
message(int type, int num, char *message, ...) {
    va_list args;
    va_start(args, message);
    msg(num, type, message, args);
    va_end(args);
}
