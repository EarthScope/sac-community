#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../inc/extfunc.h"

#ifndef TRUE
#define TRUE (1)
#endif

#ifndef FALSE
#define FALSE (0)
#endif

sac_header *makehdr( header_in )
sac_header *header_in;

{
  int i, nerr;

  /* Allocate a new header struct and if header_in is not NULL  */
  /* copy its values.  Otherwise (header_in == NULL) initialize */
  /* the new header to default values.                          */
  /* This routine returns a NULL pointer if it can't allocate   */
  /* a new header struct.  It returns a pointer to the new      */
  /* header otherwise.                                          */


  sac_header *new_header;

  if((new_header = malloc( sizeof(sac_header) )) == NULL) 
                                    return ( new_header );
  
  
  if( header_in != NULL ){ /* make a copy of the input header */
    memcpy(new_header, header_in, sizeof(sac_header));
  }else{                   /* initialize to default values    */
    for( i=0; i<MFHDR; i++) new_header->ext_fhdr[i] = FUNDEF;
    for( i=0; i<MNHDR; i++) new_header->ext_nhdr[i] = NUNDEF;
    for( i=0; i<MIHDR; i++) new_header->ext_ihdr[i] = IUNDEF;
    for( i=0; i<MLHDR; i++) new_header->ext_lhdr[i] = FALSE;
    for( i=0; i<MKHDR; i++) strcpy(new_header->ext_khdr[i],AUNDEF);

    /* Now set specific fields to their default values. */
    setnhdr(new_header, "nvhdr",  (int)MVHDRC, &nerr);  /* version       */
    setlhdr(new_header, "leven",  (int)TRUE,   &nerr);  /* evenly spaced */
    setlhdr(new_header, "lovrok", (int)TRUE,   &nerr);  /* overwrite ok  */
    setlhdr(new_header, "lcalda", (int)TRUE,   &nerr);  /* dist, az, baz */
                                                         /* and gcarc are */
                                                         /* calc from station*/
                                                         /* & event coords. */
    /* ninf and nhst were changed to norid and nevid.  maf 961031 */
/*    setnhdr(new_header, "ninf"  , (int)0,      &nerr);
    setnhdr(new_header, "nhst"  , (int)0,      &nerr);		*/
    setehdr(new_header, "iftype", (int)ITIME,  &nerr);
  }

  return ( new_header );

}
