/** 
 * @file   rdsac.c
 * 
 * @brief  Read a SAC file
 * 
 */

#include "dff.h"
#include "amf.h"
#include "hdr.h"
#include "co.h"
#include "bool.h"
#include "clf.h"
#include "dfm.h"
#include "errors.h"
/** 
 * Read a SAC file into memory
 * 
 * @param idfl 
 *    Data file list index number.  Used to store information about 
 *    the location of header and data components after read
 * @param kname 
 *    Name of the data file to read
 * @param kname_s 
 *    Length of \p kname
 * @param lname 
 *    - TRUE if the name is appended to a list of filenames
 *    - FALSE if to not append
 * @param ldta 
 *    - TRUE read the header and data
 *    - FALSE read the header only
 * @param nlen 
 *    Number of data points read
 * @param ndxh 
 *    Index in sacmem array of header
 * @param ndx1 
 *    Index in sacmem array of first data component
 * @param ndx2 
 *    Index in sacmem array of second data component
 *    Set to 0 if second component does not exist
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 * @date   980922:  Added lname to discriminate between filenames which
 *                  should or should not be added to the list.  maf.
 * @date   880913:  Fixed bug in defining data file list.
 * @date   850617:  Major rewrite due to addition of memory manager.
 *                  CHANGED NUMBER AND ORDER OF ARGUMENTS.
 * @date   821004:  Fixed bug which was incorrectly clearing error flag.
 * @date   810120:  Changed to output message retrieval from disk.
 */
void 
rdsac(int    idfl, 
      char  *kname, 
      int    kname_s, 
      int    lname, 
      int    ldta, 
      int   *nlen, 
      int   *ndxh, 
      int   *ndx1, 
      int   *ndx2, 
      int   *nerr) {

	int ncerr, nun, lswap = 0 ;

	*nerr = 0;
  sac *s;

  UNUSED(nlen);
  UNUSED(ndxh);
  UNUSED(ndx1);
  UNUSED(ndx2);

  if(!(s = sacget(idfl-1, ldta, nerr))) {
    goto L_8888;
  }

	/* - Open file. */
	zopen_sac( &nun, kname,kname_s, "RODATA",7, nerr );
	if( *nerr != 0 )
	    goto L_8888;

	/* - Allocate a memory block for header. */
	//allamb( &cmmem, SAC_HEADER_WORDS, ndxh, nerr );
	//if( *nerr != 0 )
  //	    goto L_8888;

	/* - Save some parameters about this data file. */
	if ( lname ) {
    s->m->filename = fstrdup(kname, kname_s);
  }
	if( *nerr != 0 )
	    goto L_8888;

	/* - Read header record. */
	lswap = rdhdr( s, &nun, s->m->filename, nerr );
	if( *nerr != 0 )
	    goto L_8888;

	/* - Read data if requested. */
	if( ldta ){

	    /* -- Determine memory requirements for this file. */
	    defmem( idfl, TRUE, nerr );
	    if( *nerr != 0 )
		goto L_8888;

      sac_alloc(s);

	    /* -- Read data components. */
	    rddta( s, &nun, lswap, nerr );
	    if( *nerr != 0 )
		goto L_8888;

	}

	/* - Close file and return. */
L_8888:
	zclose( &nun, &ncerr );
	return;
}


sac *
sacread(char *file) {
  int nerr, nun, lswap;
  sac *s;

  s = NULL;
  if(!file) {
    return NULL;
  }
  zopen_sac(&nun, file, strlen(file), "RODATA", 7, &nerr);
  if(nerr != 0) {
    return NULL;
  }
  s = sac_new();
  s->m->filename = strdup(file);
  lswap = rdhdr(s, &nun, file, &nerr);
  if(nerr != 0) {
    printf("sacread error: %d header\n", nerr);
    goto ERROR;
  }
  sac_alloc(s);

  s->m->nstart = 1;
  s->m->nstop  = s->h->npts;
  s->m->ntotal = s->h->npts;
  s->m->nfillb = 0;
  s->m->nfille = 0;
  rddta(s, &nun, lswap, &nerr);
  if(nerr != 0) {
    printf("sacread error: %d data\n", nerr);
    goto ERROR;
  }
  zclose(&nun, &nerr);

  return s;
 ERROR:
  printf("sacread error: %d\n", nerr);
  zclose(&nun, &nerr);
  sac_free(s);
  return NULL;
}
