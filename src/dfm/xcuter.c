/** 
 * @file   xcuter.c
 * 
 * @brief  Setup behavior during CUT errors
 * 
 */

#include "dfm.h"


#include "cpf.h"

/** 
 * Execute the command CUTERR which defines the behavior when certain
 *    cut errors are encounted during reads
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   820809:  Changed to newest set of parsing and checking functions.
 *
 */
void 
xcuter(int *nerr) {

  *nerr = 0;

  while( lcmore( nerr ) ){
    
    /* -- "FATAL/USEBE/FILLZ":  select cut error control technique. */
    if( lclist( (char*)kmdfm.kcuter,9, MCUTER, &cmdfm.icuter ) ) {  

    } else {
      /* -- Bad syntax. */
      cfmt( "ILLEGAL OPTION:",17 );
      cresp();
    }
  }
  
  return;
}
