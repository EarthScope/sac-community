/** 
 * @file   xrcor.c
 * 
 * @brief  Read a Correlation function
 * 
 */


#include "spe.h"
#include "amf.h"
#include "bool.h"

#include "errors.h"


#include "cpf.h"
#include "dff.h"

#define error_handling(error, line) if( *(error) != SAC_OK) { goto line; }

/** 
 * Read a Correlation function 
 * 
 * @param nerr 
 */
void 
xrcor(int *nerr) {
  
  int max;
  float zero, delta;

  *nerr = 0;
  
  cmspe.samfrq = 0.0;
  cmspe.lcor   = FALSE;
  cmspe.lspe   = FALSE;

  /* - Loop on each token in command: */
  
  while ( lcmore( nerr ) ){
    /* -- "filename":  define name of file to write. */
    if( lcchar( MCPFN, kmspe.knmcor,MCPFN+1, &cmspe.junk ) )
      { /* do nothing */ }
    
    /* -- Bad syntax. */
    else{
      cfmt( "ILLEGAL OPTION:",17 );
      cresp();
    }
  }

  error_handling(nerr, ERROR);
  
  max = cmspe.firstPowerOf2 * 2;
  rsac1(kmspe.knmcor,                 /* Filename */
	cmmem.sacmem[cmspe.ndxcor],   /* Y Array -- Correlation Function*/
	&cmspe.nlnfft,                /* Length of Array */
	&zero,                        /* Beginning Value */
	&delta,                       /* Delta Value */
	&max,                         /* Maximum Length of Array */
	nerr,                         /* Error Value */
	MCPFN+1                       /* Length of Filename */
	);
  
  error_handling(nerr, ERROR);

  cmspe.samfrq = 1.0 / delta;
  cmspe.lcor   = TRUE;
  cmspe.lspe   = FALSE;
  
 ERROR:
  return;
}

