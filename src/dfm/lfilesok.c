/** 
 * @file   lfilesok.c
 * 
 * @brief  Test for the existance of data files 
 * 
 */

#include <stdio.h>
#include <string.h>

#include "config.h"

#include "co.h"
#include "clf.h"
#include "dff.h"
#include "ncpf.h"

#ifdef HAVE_LIBRPC 
#include <rpc/rpc.h>
#endif /* HAVE_LIBRPC */

#include "dfm.h"
#include "hdr.h"
#include "bool.h"
#include "amf.h"

#include "debug.h"

/** 
 * Test for the existence of data files to be read
 * 
 * @param kfiles 
 *    List of files to be read
 * @param kfiles_s 
 *    Length of \p kfiles
 * @param nfiles 
 *    Number of files in \p kfiles
 * @param kdir 
 *    Default directory to read from
 * @param kdir_s 
 *    Length of \p kdir
 * @param lmore 
 *    - TRUE if the files are to be appended to the current list
 *    - FALSE if the list is to be truncated first
 * @param lheader 
 *    - TRUE if reading the header
 *    - FALSE of just opening the file
 * @param lxdr 
 *    - TRUE - If the list of files are xdr format
 *    - FALSE - If the list of files are not xdr format
 * @param nerr 
 *    - Error Return Flag
 *    - 0 on Success
 *    - 
 * 
 * @return 
 *    - TRUE if a file in the list could be opened and the header read. 
 *    - FALSE if no files could be opened for reading.
 *
 * @bug This calls zopen_sac(), which opens the file. Does it need to do this
 *       All it needs to do is check for the files existence.
 *
 * @date   981103:  Removed call to mkdsflsactive() by removing cmdfm.ldschg
 *                  which is never TRUE since removing the TO option from
 *                  the READ command.  maf
 * @date   920618:  Changed kfile to be substr of kfiles rather than kdflrq.
 *                  Problem came up with READ DIR bug reported by Owens.
 * @date   920427:  Original version.
 *
 */
int
lfilesok(string_list *files,
         char *kdir, 
         int   kdir_s, 
         int   lmore, 
         int   lheader, 
         int   lxdr, 
         int  *nerr) {

    int i;
	char *file, *kopen;
	int lfilesok_v ;
	int ihdrndx, idx, nsdxhdr, nun;
        
#ifdef HAVE_LIBRPC
    XDR xdrs;
#else
    if(lxdr) {
        return FALSE;
    }
#endif /* HAVE_LIBRPC */
    UNUSED(kdir_s);
    kopen = NULL;
    file  = NULL;

	*nerr = 0;

	/* - Assume the worst. */
	lfilesok_v = FALSE;

	/* -- Save contents of ndxhdr(1) */
	nsdxhdr = Ndxhdr[1];

	/* -- Allocate block for header. Set ndxhdr for this file. */
	allamb( &cmmem, SAC_HEADER_WORDS, &ihdrndx, nerr );
	if( *nerr != 0 )
	    goto L_8888;
	Ndxhdr[1] = ihdrndx;
    
	for( idx = 0 ; idx < SAC_HEADER_WORDS ; idx++ ) {
	    cmmem.sacmem[ihdrndx][idx] = 0 ;
	}

	/* - For each file in the list. */
    for(i = 0; i < string_list_length(files); i++) {
	    /* -- Try to open and [optionally] read the header of each file in 
	     *    the list. If successfull then it's ok to continue read 
	     *    process. */

	    /* -- Get a file name. */
        file = string_list_get(files, i);
	    /* -- Try to open the file. */
        
        if(kdir) {
            asprintf(&kopen, "%s/%s", kdir, file);
        } else {
            kopen = strdup(file);
        }
        DEBUG("file: %s\n", kopen);
        if( lxdr ) {
#ifdef HAVE_LIBRPC 
            znfiles(&fileun, kopen, MCPFN+1, "TEXT", 5, nerr);
            if( *nerr == 0 ){
                xdrstdio_create(&xdrs, fileun, XDR_DECODE);
            }
            else{
                continue ;
            }
#endif /* HAVE_LIBRPC */
	    }
	    else{
            zopen_sac( &nun, kopen, strlen(kopen), "RODATA",7, nerr );
            if( *nerr != 0 ){
                continue ;
            }
	    }
	    /* --- Try to read the header. */
	    if( lheader ){
            if( lxdr ){
#ifdef HAVE_LIBRPC
                xdrhdr( xdrs, cmmem.sacmem[ihdrndx], nerr );
#endif /* HAVE_LIBRPC */
            }
            else{
                rdhdr( 1, &nun, file, nerr );
            }
	    }
	    else{
            lfilesok_v = TRUE;
            if( lxdr ) {
#ifdef HAVE_LIBRPC 
                xdr_destroy( &xdrs );
                zcloses( &fileun, nerr);
#endif /* HAVE_LIBPRC */                    
            }
            else{
                zclose( &nun, nerr );
            }
            break;
	    }
        
	    /* -- Test for successfull header read with this file. */
	    if( *nerr != 0 ){
            if( lxdr ) {
#ifdef HAVE_LIBRPC 
                xdr_destroy( &xdrs );
                zcloses( &fileun, nerr);
#endif /* HAVE_LIBPRC */
            }
            else{
                zclose( &nun, nerr );
            }
            continue ;
	    }
	    else{
            lfilesok_v = TRUE;
            if( lxdr ) {
#ifdef HAVE_LIBRPC 
                xdr_destroy( &xdrs );
                zcloses( &fileun, nerr);
#endif /* HAVE_LIBPRC */
            }
            else{
                zclose( &nun, nerr );
            }
            break;
	    }
	} /* end for */
    
    
	/* - Restore ndxhdr */
	Ndxhdr[1] = nsdxhdr;

	/* - A file was found to be a valid sac file, or we ran out of files */
	if( lfilesok_v ){
	    if( !lmore ){
	      /* -- When not using READ MORE: 
	       * --- Clear working-storage pointers, data-set storage pointers
	       *     and deallocate memory for the current data set. 
	       */
	      cleardfl( nerr );
	    }
	}

L_8888:
	relamb( cmmem.sacmem, ihdrndx, nerr );
    if(kopen) { 
        free(kopen);
    }

	return( lfilesok_v );
} 










