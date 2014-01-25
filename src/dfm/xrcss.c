/** 
 * @file   xrcss.c
 * 
 * @brief  Read a CSS File
 * 
 */

#include <string.h>

#include "dfm.h"
#include "bool.h"
#include "com.h"


#include "co.h"
#include "ssi.h"
#include "cpf.h"

/** 
 * Execute the command READCSS to read a CSS file 
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   970403:  New option to conveniently select the channel.
 * @date   970206:  New option to specify which magnitude to read.  maf
 * @date   961216:  Now filtering on station rather than gain.  maf
 * @date   920420:  Changed null string, "", to ' ' - porting to IBM. 
 * @date   910703:  add # to parameter keyword GAIN; 
 *                  add * as a legal value for params GAIN, BAND, ORIENT
 * @date   910402:  New code.
 *
 */
void 
xrcss(int *nerr) {

    char _c0[2];
    char file[MCMSG+1];
    int len;
	int lmore, lshift, lscale, larray ;
	int nchar;
    static int Verbose = 0;
    static int ibinORasc;
    static string_list *last_list = NULL;
    double memory_max;

    memory_max = 0.30;

	char kmag[4] ;	/* magnitude type: mb, ms, or ml. maf 970206 */
    string_list *list;

    if(!last_list) {
        last_list = string_list_init();
    }

	*nerr = 0;
    list = string_list_init();

	/* PARSING PHASE: */
	/* - Parse position-dependent tokens: */
	lmore = FALSE;
  
	while ( lcmore( nerr ) ){
    
    /* -- "MORE":  signifies addition of more files to current read 
     *  filelist rather than replacement of current list with new one */
    if( lckey( "MORE#$",7 ) && cmdfm.ndfl > 0 ){
      lmore = TRUE;
      continue ;
    }
    
    /* -- "VERBOSE ON|OFF":  turn Verbose mode on or off. */
    else if( lklog( "VER$BOSE",9, &Verbose ) ){
      continue ;
    }
    /* -- "SHIFT ON|OFF":  turn calibration on or off. */
    else if( lklog( "SHIFT$",7, &lshift ) ){
      cmdfm.lshift = lshift ;
      continue ;
    }

	    /* -- "SCALE ON|OFF":  turn scaling on or off. */
    else if( lklog( "SCALE$",7, &lscale ) ){
      cmdfm.lscale = lscale ;
      continue ;
    }
    
    /* -- "ARRAY ON|OFF":  turn lienient array behavior on or off. */
    else if ( lklog ( "ARRAY$" , 7 , &larray ) ) {
      cmdfm.larray = larray ;
      continue ;
    }
    
    /* -- "MAXMEM v": change maximum fractional memory used by SeisMgr*/
    else if( lkreal( "MAX#MEM$",9, &memory_max ) )
	    { 
        MaxMem = memory_max;
      }
    
    /* -- "DIR CURRENT|name": set the name of the default subdirectory*/
    else if(lkchar("DIR#$",6,MCPFN,kmdfm.krdcssdir,MCPFN+1,&nchar)){
      if( memcmp(kmdfm.krdcssdir,"CURRENT",7) == 0 ||
          memcmp(kmdfm.krdcssdir ,"current",7) == 0 ){
		    cfmt( "ILLEGAL PARAM VALUE: current",30 );
		    cresp();
		    return ;
      }
      else if( kmdfm.krdcssdir[nchar - 1] != KDIRDL ){
		    _c0[0] = KDIRDL;
		    _c0[1] = '\0';
		    subscpy( kmdfm.krdcssdir, nchar, -1, MCPFN, _c0 );
      }
      continue;
    }
    
    /* -- "MAGNITUDE|mb|ms|ml|def":  specify a field for magnitude, or
       if def is found, use the algorithm to determine which
       magnitude to read.  maf 970206. */
    else if ( lkchar ( "MAG#NITUDE$", 12 , 4 , kmag , 4 , &nchar ) ) {
      if ( kmag [ 0 ] == 'm' || kmag[ 0 ] == 'M' ) {
		    if ( kmag [ 1 ] == 'b' || kmag [ 1 ] == 'B' ) 
          cmdfm.nMagSpec = MbMag ;
		    else if ( kmag [ 1 ] == 's' || kmag [ 1 ] == 'S' )
          cmdfm.nMagSpec = MsMag ;
		    else if ( kmag [ 1 ] == 'l' || kmag [ 1 ] == 'L' )
          cmdfm.nMagSpec = MlMag ;
		    else {
          cfmt( "ILLEGAL PARAM VALUE:",22 );
          cresp();
          return ;
		    }
      } /* end if ( kmag [ 0 ] == 'm' ... ) */
      else if ( strncmp ( kmag , "def" , 3 ) == 0 || 
                strncmp ( kmag , "DEF" , 3 ) == 0 )
		    cmdfm.nMagSpec = Any ;
      else {
		    cfmt( "ILLEGAL PARAM VALUE:",22 );
		    cresp();
		    return ;
      }
    } /* end if ( lkchar ( "MAG#NITUDE$", ... ) */


	    /* -- "STATION": whether wfdisc record fld 'sta' matches given */
	    /*		string.   maf 961216 */
    else if( lklogc("STA#TION$",10,&kmdfm.lstation,kmdfm.kstation,7 ) ){
      char *ptr ; 
      
      ptr = strchr ( kmdfm.kstation , ' ' ) ;
      if ( ptr != NULL )
		    *ptr = '\0' ;
      continue;
    }

    /* -- "CHANNEL": whether wfdisc record fld 'chan' matches the 
     *              given string.  maf 970403 */
    else if( lklogc("CHAN#NEL$",10,&kmdfm.lchannel,kmdfm.kchannel,9 ) ){
      char *ptr ;
      
      ptr = strchr ( kmdfm.kchannel , ' ' ) ;
      if ( ptr != NULL )
		    *ptr = '\0' ;
      //if( jcdflbeg > 0 && jcdflend == 0 )
      //  jcdflend = jcparmbeg - 1;
      continue;
    }


    /* -- "BANDWIDTH": whether wfdisc record fld 'chan' has a leading 
     *     letter which is S(short), M(medium), or L(long) */
    else if( lklogc( "BAND#WIDTH$",12, &kmdfm.lbandw,kmdfm.kbandw,9 ) ){
      //if( jcdflbeg > 0 && jcdflend == 0 )
      //jcdflend = jcparmbeg - 1;
      continue;
    } /* end else if( lklogc( "BAND#WIDTH$" ... */

	    /* -- "ORIENTATION: whether wfdisc record fld 'chan' has a second 
	     *     letter which is N(north), E(east), or Z(vertical) */
    else if(lklogc("ORIENT#ATION$",14,&kmdfm.lorient,kmdfm.korient,9)){
      //if( jcdflbeg > 0 && jcdflend == 0 )
      //jcdflend = jcparmbeg - 1;
      continue;
    } /* end else if( lklogc( "ORIENT#ATION$" ... */
    
    /* -- TRUST:  whether or not to trust matching evids while moving
       data from SAC buffers to CSS buffers. */
    else if( lklog( "TRUST#$",8, &cmdfm.ltrust ) )
      { /* do nothing */ }
    
    /* -- "COMMIT|RECALLTRACE|ROLLBACK":
       how to treat existing data */
    else if ( lckeyExact ( "COMMIT" , 7 ) )
      cmdfm.icomORroll = COMMIT ;
    else if (lckeyExact ( "RECALLTRACE" , 12 ) )
      cmdfm.icomORroll = RECALL ;
    else if ( lckeyExact ( "RECALL" , 7 ) )
      cmdfm.icomORroll = RECALL ;
    else if ( lckeyExact ( "ROLLBACK" , 9 ) )
      cmdfm.icomORroll = ROLLBACK ;
    
    
    /* -- "BINARY|ASCII": CSSB versus flat files */
    else if( lclist( (char*)kmdfm.kbinORasc,9, 2, &ibinORasc ) ) {
      cmdfm.lrascii = ibinORasc - 1 ;
    }
    
	    /* -- Else assume it to be beginning/continuing dfl, if not 
	     *    recognized as above 
	     *    Use kdflin as dummy buffer to skip a string  */
	    else if(lcchar(MCMSG, file, MCMSG+1, &len)) {
            string_list_put(list, file, len);
	    } 
	} 

	/* - The above loop is over when one of two conditions has been met:
	 *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
	 *   (2) All the tokens in the command have been successfully parsed. */

	if( *nerr != 0 )
	    return ;

    if(string_list_length(list) <= 0) {
        string_list_extend(list, last_list);
    }
	/* EXECUTION PHASE: */

        /* - Commit or rollback data according to lmore and cmdfm.icomORroll */
        if ( lmore ) {
            alignFiles ( nerr ) ;
	    if ( *nerr )
		return ;
	    cmdfm.nfilesFirst = cmdfm.ndfl ;
        } /* end if */
	else {
	    cmdfm.nreadflag = HIGH ;
	    cmdfm.nfilesFirst = 0 ;
	}

        /* Copy Current List to Last_List */
        string_list_clear(last_list);
        string_list_extend(last_list, list);
        /* - Expand the filelist and read the files into memory.
         * -- Parameter kstation, kband, korient are picked up in readcfl from
         *    ../../inc/dfm */
        readcfl( lmore, kmdfm.krdcssdir,MCPFN+1, list, 
                 Verbose, cmdfm.lrascii, MaxMem, nerr );

        string_list_free(list);
        list = NULL;
	return;

}

