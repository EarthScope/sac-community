/** 
 * @file   reaamb.c
 * 
 * @brief  Reallocate a block of Memory
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "complex.h"
#include "proto.h"
#include "debug.h"

/** 
 * A memory reallocator
 *
 * Reallocates a block of memory at index \c index and size \c nosize from the array of pointers \c array to a new size \c nsize and returns the index of the reallocated block in \c newndx
 * 
 *
 * \todo The array of pointer that is passed in should actually be \c memstruct 
 *       of type t_cmmem to be consistent with iniamb and allamb
 *
 * \param **array
 *    Pointer array being managed
 * \param nsize
 *    Size of the newly requested block
 * \param nosize
 *    Size of the old memory block, this is currently not used 
 * \param index
 *     Index of the old block
 * \param *newndx
 *     Index of the newly requested block
 *     \li 0 on error
 * \param *nerr
 *     Error return flag
 *     \li  0  on success
 *     \li  301 on error
 *
 * \see iniam relamb allamb reaamb
 *
 * \date 920723:  Original version. (Amanda Goldner)
 * \date 070606:  Documented/Reviewed
 *
 */

void 
reaamb(float **array, 
       int     nsize, 
       int     nosize, 
       int     index, 
       int    *newndx, 
       int    *nerr)
{

  float *tempptr;
  UNUSED(nosize);
	*nerr = 0;

	/* - Allocate a new block */

        if((tempptr = (float *)realloc(array[index],nsize*sizeof(float))) == NULL){
          *nerr = 0301;
          *newndx = -1;
	}
        else {
          *newndx = index;
          array[index] = tempptr;
        }              

       	return;
}

