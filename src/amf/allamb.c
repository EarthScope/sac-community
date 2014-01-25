/** 
 * @file   allamb.c
 * 
 * @brief  Allocate a Block of Memory
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "amf.h"
#include "msg.h"
#include "debug.h"

/**  \def MEMINC
 *    How much to increase the memory upon request
 */
#define MEMINC 50

/** 
 * A memory allocator
 *
 * Allocates a block of memory in \c memstruct of a specific size \c nsize 
 *    and returns the index \c index of the new block
 * 
 * Find the smallest index which is unallocated
 *
 * If all available entries are taken, 
 *    expand the pointer array in place using realloc()
 *    and expand by the value of MEMINC
 *
 * Initialize the smallest available memory pointer with the size requested
 * 
 * Return the index in the pointer array of the newly allocated memory, 
 *    else return 0
 *
 * \param *memstruct
 *     Memory Structure being managed
 * \param nsize
 *     Size of newly request memory block
 * \param *index
 *     Index of memory block just allocated
 *     \li  0   on error
 * \param *nerr
 *     Error return flag
 *     \li 0   on success
 *     \li 301 on error 
 *
 * \see iniam relamb allamb reaamb
 *
 * \date 070606:  Documented/Reviewed
 * \date 960709:  Changed malloc to calloc to initialize to zero, maf
 * \date 940127:  Original version.
 *
 */

void 
allamb(struct t_cmmem *memstruct, 
       int             nsize, 
       int            *index, 
       int            *nerr)
{

        float **tempptr;
        int i, j, ierr;

	*nerr = 0;
        ierr = 0;

	/* find an available pointer to use */
	/* SAC HACK: start at sacmem[1] rather than sacmem[0] to avoid */
	/* handing out index of zero, because this has historical      */
	/* significance throughout the code */

        for(i=1; (i<memstruct->nallocated && memstruct->sacmem[i] != NULL); i++)
	{ /* do nothing */ }

        if(i == memstruct->nallocated){
	/* there were no available entries, expand pointer array in place */

          if((tempptr = (float **)realloc(memstruct->sacmem,
                      (memstruct->nallocated+MEMINC)*sizeof(float *))) == NULL)
            ierr = 1;
          else {
            memstruct->sacmem = tempptr;
            memstruct->nallocated += MEMINC;
            for (j=i; j<memstruct->nallocated; j++)memstruct->sacmem[j] = NULL;
	  }
	}

	/* Changed malloc to calloc to initialize to zero.  maf 960709 */
        if((memstruct->sacmem[i] = (float*)calloc(nsize,sizeof(float))) == NULL)
          ierr = 1;
        else
          *index = i;

        DEBUG("id: %d size: %d\n", *index, nsize);
        if( ierr != 0 ) {

	  *index = 0;
	  *nerr = 301;
	  setmsg( "ERROR", *nerr );
	  apimsg( nsize );
        }

}

