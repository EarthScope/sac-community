/** 
 * @file   sort.c
 * 
 * @brief  Sort data files
 * 
 */

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "dfm.h"
#include "dff.h"

/** 
 * String \p a2 is less than string \p a1
 * 
 * @param a1 
 *    String 
 * @param a2 
 *    String
 * 
 * @return 
 *    - TRUE if \p a2 is less than \p a1
 *    - FALSE if \p a2 is not less than \p a1
 */
int 
LessThanStrg(void *a1, void *a2)
{
   char **a1i = (char**)a1;
   char **a2i = (char**)a2;
   if( strcmp(a1i[0], a2i[0]) < 0 ) return 1;
   return 0;
}

/** 
 * String \p a2 is greater than string \p a1
 * 
 * @param a1 
 *    String 
 * @param a2 
 *    String
 * 
 * @return 
 *    - TRUE if \p a2 is greater than \p a1
 *    - FALSE if \p a2 is not greater than \p a1
 */
int
GreaterThanStrg(void *a1, void *a2)
{
    char **a1i = (char**)a1;
    char **a2i = (char**)a2;
    if( strcmp(a1i[0], a2i[0]) > 0 ) return 1;
    return 0 ;
}


/** 
 * Check to see if a number is NaN.  This function is deprecated.
 * 
 * @param a1 
 *    Number to check
 * 
 * @return 
 *    - TRUE if \p a1 is NaN
 *    - FALSE if \p a1 is not NaN
 */
int 
isNaN( float a1 )
{
    DEPRECATED("isNaN()", "isnan()");
    if( !( a1 < 1.0 ) && !( a1 > 1.0 ) && !( a1 == 1.0 ) )
	return 1 ;
    return 0 ;
}

/** 
 * Number \p a2 is less than number \p a1
 * 
 * @param a1 
 *    Number
 * @param a2 
 *    Number
 * 
 * @return 
 *    - TRUE if \p a2 is less than \p a1
 *    - FALSE if \p a2 is not less than \p a1
 */
int 
LessThanFloat(void *a1, void *a2)
{
   float a1i = *( (float*)a1);
   float a2i = *( (float*)a2);
   if( a1i < a2i ) return 1;
   if( isnan( a1i ) ) {
      if( !isnan( a2i ) )
         return 1 ;
   }
   return 0;
}

/** 
 * Number \p a2 is greater than number \p a1
 * 
 * @param a1 
 *    Number
 * @param a2 
 *    Number
 * 
 * @return 
 *    - TRUE if \p a2 is greater than \p a1
 *    - FALSE if \p a2 is not greater than \p a1
 */
int 
GreaterThanFloat(void *a1, void *a2)
{
   float a1i = *( (float*)a1);
   float a2i = *( (float*)a2);
   if( a1i > a2i ) return 1;
   if( isnan( a2i ) ) {
      if( !isnan( a1i ) )
         return 1 ;
   }
   return 0;
} 


/** 
 * Number \p a2 is less than number \p a1
 * 
 * @param a1 
 *    Number
 * @param a2 
 *    Number
 * 
 * @return 
 *    - TRUE if \p a2 is less than \p a1
 *    - FALSE if \p a2 is not less than \p a1
 */
int 
LessThanLong(void *a1, void *a2)
{
   int a1i = *( (int*)a1);
   int a2i = *( (int*)a2);
   if( a1i < a2i ) return 1;
   return 0;
}

/** 
 * Number \p a2 is greater than number \p a1
 * 
 * @param a1 
 *    Number
 * @param a2 
 *    Number
 * 
 * @return 
 *    - TRUE if \p a2 is greater than \p a1
 *    - FALSE if \p a2 is not greater than \p a1
 */
int 
GreaterThanLong(void *a1, void *a2)
{
   int a1i = *( (int*)a1);
   int a2i = *( (int*)a2);
   if( a1i > a2i ) return 1;
   return 0;
}



/** 
 * Sorting function that produces a permutation index that will put
 *    input array (a) into sorted order, either ascending or descending.
 *    The sort is stable, meaning that the relative order of equal keys
 *    is not changed. The sorting method is straight selection
 *    (N-squared order).
 *
 * @param a 
 *    The array to be sorted is left unchanged and should be cast to void*.
 * @param N 
 *    Number of elements in the array \p a.
 * @param 
 *    size is the size of one element.
 * @param fp1
 *    fp1 is a pointer to a user-supplied function with a prototype like:
 *       int fp1(<Type T> *a1, <Type T> *a2);
 *    for  ascending order, fp1 should return true if a1 is less than a2.
 *    for descending order, fp1 should return true if a1 is greater than a2.
 * @param 
 *    integer array which on output contains the permutation index.
 *
 *
 */
void 
Index(char *a, 
      int   N, 
      int   size, 
      int   (*fp1)(void *a1, void *a2), 
      int   *index) {

   int i, j;
   char *v;
   
   for(i=0; i< N;i++)
      index[i] = i;
   
   for(i=1;i<N; i++){
      v = a + index[i] * size;
      j = i;
      while(j > 0 && fp1(v, a +  index[j-1] * size ) ){
         index[j] = index[j-1];
         j--;
      }
      index[j] = i;
   }
}

