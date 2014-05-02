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

sac * sacread(char *file);

#define error_handling(error, line) if( *(error) != SAC_OK) { goto line; }
extern float *specor;
/** 
 * Read a Correlation function 
 * 
 * @param nerr 
 */
void 
xrcor(int *nerr) {
  
  int max;
  sac *s;
  char *filename;
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

  filename = fstrdup(kmspe.knmcor, MCPFN+1);
  if(!(s = sacread(filename))) {
    *nerr = ERROR_READING_FILE;
    error_handling(nerr, ERROR);
  }

  max = cmspe.firstPowerOf2 * 2;
  if(s->h->npts > max) {
    s->h->npts = max;
  }
  memcpy(specor, s->y, sizeof(float) * s->h->npts);
  cmspe.samfrq = 1.0 / s->h->delta;
  cmspe.nlnfft = s->h->npts;
  cmspe.lcor   = TRUE;
  cmspe.lspe   = FALSE;

 ERROR:
  sac_free(s);
  FREE(filename);
  return;
}

