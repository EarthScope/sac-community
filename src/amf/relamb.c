/** 
 * @file   relamb.c
 * 
 * @brief  Release a block of memory
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "complex.h"
#include "proto.h"

#include "debug.h"
/** 
 * A memory releaser
 *
 * Releases a block of memory at index \c index from the array of pointers \c array
 *
 * \todo The index which is being freed should really be check if it 
 *    needs to be freed and an error should be returned 
 * \todo The array of pointer that is passed in should actually be \c memstruct of type t_cmmem to be consistent with iniamb and allamb
 *
 * \param **array
 *    Pointer array being managed
 * \param index
 *     Index to be released
 * \param *nerr
 *     Error return flag
 *     \li  0  on success
 *     \li  no errors
 *
 * \see iniam relamb allamb reaamb
 *
 * \date 070606:  Documented/Reviewed
 *
 */

void 
relamb(float    **array, 
       int        index, 
       int       *nerr)
{

	*nerr = 0;
    DEBUG("free id: %d\n", index);
        free(array[index]);

        array[index] = NULL;

	return;

}

