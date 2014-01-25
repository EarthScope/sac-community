/** 
 * @file   byteswap.c
 * 
 * @brief  Byte swap numbers
 * 
 */

#include "ucf.h"

/** 
 * Byteswap data. This routine will reverse the byte order of 2, 4, 
 *    or 8 byte numeric types.  
 * 
 * @param swappee 
 *    Data to byteswap
 * @param Nbytes 
 *    Size in bytes of data to byteswap
 *
 * @return If data was swapped
 *    - 0 if data was swapped
 *    - 1 if data was not swapped
 *
 * @date    030205:  Original Version maf (2/5/03)
 *
 */
int 
byteswap(void *swappee, 
	 int   Nbytes )
{
    char temp, *ptr = swappee ;

    if( Nbytes == 2 ){
        temp   = ptr[0] ;
        ptr[0] = ptr[1] ;
        ptr[1] = temp ;
        return 0 ;
    }

    if( Nbytes == 4 ){
        temp   = ptr[0] ;
        ptr[0] = ptr[3] ;
        ptr[3] = temp ;

        temp   = ptr[1] ;
        ptr[1] = ptr[2] ;
        ptr[2] = temp ;
        return 0 ;
    }

     if( Nbytes == 8 ){
        temp   = ptr[0] ;
        ptr[0] = ptr[7] ;
        ptr[7] = temp ;

        temp   = ptr[1] ;
        ptr[1] = ptr[6] ;
        ptr[6] = temp ;

        temp   = ptr[2] ;
        ptr[2] = ptr[5] ;
        ptr[5] = temp ;

        temp   = ptr[3] ;
        ptr[3] = ptr[4] ;
        ptr[4] = temp ;
        return 0 ;
    }

    return -1 ;
}
