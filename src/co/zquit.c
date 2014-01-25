/** 
 * @file   zquit.c
 * 
 * @brief  Terminate the SAC program
 * 
 */

#include <stdio.h>
#include <stdlib.h>

#include "co.h"
#include "bot.h"
#include "select.h"

#include "config.h"


#include "gdm.h"
#include "xyz.h"

#include "token.h"
#include "cpf.h"
#include "amf.h"

void sac_vars_free();
void saccommands_cleanup(eval *e);
void sacmem_free(struct t_cmmem *mem);
void vfilelist_free();
void sac_history_filename_free();
void dblErrorFree();
void dfm_free();
void gdm_free_devices();
void lexer_free();

/** 
 * Terminate the SAC program
 * 
 * @bug The call to co/zdestf() tries to delete files which are never created
 *       Should be removed.
 *
 * @note There is no longer any system dependent coding
 *       in this subroutine.  There was in previous versions.  It is 
 *       kept in this subdirectory for both historical and slothful reasons.
 *
 * @date   Sept 02, 2008 Added sac_history_set_file(NULL)
 *                       B. Savage < savage _AT_ uri.edu >
 * @date   900310:  Added call to utahcleanup.
 * @date   891002:  Deleted calls to zsysop and zintof that were
 *                 no longer doing anything.
 * @date   880520:  Fixed bug in call to engraphics.
 * @date   860930:  Deleted call to zendfr.
 * @date   840822:  Retyped on B4.2 UNIX VAX--D. Trimmer
 * @date   830818:  Cleanup and documented.
 * @date   820823:  Delete LSAVE input argument.
 * @date   811112:  Added a call to ZENDFR to end current graphics frame.
 * @date   891002:  Documented/Reviewed
 *
 */
void 
zquit() {
  char *sachistory;
  int nerr;

	/* - Close and dispose of any output files. */
	/* -- Plot files. */
	endgraphics( &nerr );
	if( nerr != 0 )
		goto L_8888;

	/* -- Pick files. */
	capf( &nerr );
	if( nerr != 0 )
		goto L_8888;
	chpf( &nerr );
	if( nerr != 0 )
		goto L_8888;

	/* - Destroy any scratch files created during execution. */

	zdestf( "ZDFL ",6, &nerr );
	if( nerr != 0 )
		goto L_8888;

	xyzcleanup();
  sac_vars_free();



#ifdef READLINE
        /* Cleanup Command Line Editing Tools */
	{
	  sachistory = sac_history_file();
	  if(sachistory) {
	    write_history(sachistory);
        clear_history();
	  }
	}
  if(use_tty()) {
    rl_callback_handler_remove();
    /* rl_cleanup(); */
  }
#endif /* READLINE */
  arg_reset();
  saccommands_cleanup(NULL);
  sacmem_free( &cmmem );
  vfilelist_free();
  sac_history_filename_free();
  dblErrorFree();
  select_loop_message(NULL, -1);
  dfm_free();
  gdm_free_devices();
  lexer_free();
	/* - Terminate program. */

	exit(0);

L_8888:
	return;

}

