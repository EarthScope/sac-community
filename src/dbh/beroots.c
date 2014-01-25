/** 
 * @file   beroots.c
 * 
 * @brief  Bessel Poles for Normalized LowPass (LP) Filter
 */

#include "complex.h"
#include "dbh.h"
#include "co.h"

/** 
 * Compute Bessel Poles For a Normalized Low Pass (LP) Filter
 * 
 * @param p 
 *    Complex Array containing Poles. Contains only one of from each
 *     - Complex Conjugate Pole-Zero Pair
 *     - Complex Conjugate Pole Pair
 *     - Single Real Pole
 * @param rtype 
 *    Character Array indicating 2nd Order Section Types
 *     - 'CPZ' Complex Conjugate Pole-Zero Pair
 *     - 'CP'  Complex Conjugate Pole Pair
 *     - 'SP'  Single Real Pole
 * @param rtype_s 
 *     Length of string \p rtype
 * @param dcvalue 
 *     Magnitude of the filter at Zero Frequency
 * @param nsects 
 *     Number of 2nd Order Sections
 * @param iord 
 *     Desired Filter Order, Must be between 1 and 8
 *
 * @return Nothing
 *
 * @copyright 1990  Regents of the University of California                      
 *
 * \bug Poles are defined explicitly within the routine. 
 *        Pull them out and define and document them.
 *
 * @author  Dave Harris                                                         
 *           Lawrence Livermore National Laboratory                              
 *           L-205                                                               
 *           P.O. Box 808                                                        
 *           Livermore, CA  94550                                                
 *           USA                                                                 
 *           (415) 423-0617                                                      
 *
 *
 * \date 920415   Changed P and RTYPE to adjustable array by 
 *                     using an "*" rather than a "1".     
 *
 */
void 
beroots(complexf *p, 
        char     *rtype, 
        int       rtype_s, 
        float    *dcvalue, 
        int      *nsects, 
        int       iord)
{
#define RTYPE(I_,J_)	(rtype+(I_)*(rtype_s)+(J_))

	complexf *const P = &p[0] - 1;

	if( iord == 1 ){

		P[1] = flttocmplx( -1.0, 0.0 );
		fstrncpy( RTYPE(0,0) , rtype_s - 1 , "SP", 2 );

		}
	else if( iord == 2 ){

		P[1] = flttocmplx( -1.1016013, 0.6360098 );
		fstrncpy( RTYPE(0,0) , rtype_s - 1 , "CP", 2 );

		}
	else if( iord == 3 ){

		P[1] = flttocmplx( -1.0474091, 0.9992645 );
		fstrncpy( RTYPE(0,0) , rtype_s - 1 , "CP", 2 );
		P[2] = flttocmplx( -1.3226758, 0.0 );
		fstrncpy( RTYPE(1,0) , rtype_s - 1 , "SP", 2 );

		}
	else if( iord == 4 ){

		P[1] = flttocmplx( -0.9952088, 1.2571058 );
		fstrncpy( RTYPE(0,0) , rtype_s - 1 , "CP", 2 );
		P[2] = flttocmplx( -1.3700679, 0.4102497 );
		fstrncpy( RTYPE(1,0) , rtype_s - 1 , "CP", 2 );

		}
	else if( iord == 5 ){

		P[1] = flttocmplx( -0.9576766, 1.4711244 );
		fstrncpy( RTYPE(0,0) , rtype_s - 1 , "CP", 2 );
		P[2] = flttocmplx( -1.3808774, 0.7179096 );
		fstrncpy( RTYPE(1,0) , rtype_s - 1 , "CP", 2 );
		P[3] = flttocmplx( -1.5023160, 0.0 );
		fstrncpy( RTYPE(2,0) , rtype_s - 1 , "SP", 2 );

		}
	else if( iord == 6 ){

		P[1] = flttocmplx( -0.9306565, 1.6618633 );
		fstrncpy( RTYPE(0,0) , rtype_s - 1 , "CP", 2 );
		P[2] = flttocmplx( -1.3818581, 0.9714719 );
		fstrncpy( RTYPE(1,0) , rtype_s - 1 , "CP", 2 );
		P[3] = flttocmplx( -1.5714904, 0.3208964 );
		fstrncpy( RTYPE(2,0) , rtype_s - 1 , "CP", 2 );

		}
	else if( iord == 7 ){

		P[1] = flttocmplx( -0.9098678, 1.8364514 );
		fstrncpy( RTYPE(0,0) , rtype_s - 1 , "CP", 2 );
		P[2] = flttocmplx( -1.3789032, 1.1915667 );
		fstrncpy( RTYPE(1,0) , rtype_s - 1 , "CP", 2 );
		P[3] = flttocmplx( -1.6120388, 0.5892445 );
		fstrncpy( RTYPE(2,0) , rtype_s - 1 , "CP", 2 );
		P[4] = flttocmplx( -1.6843682, 0.0 );
		fstrncpy( RTYPE(3,0) , rtype_s - 1 , "SP", 2 );

		}
	else if( iord == 8 ){

		P[1] = flttocmplx( -0.8928710, 1.9983286 );
		fstrncpy( RTYPE(0,0) , rtype_s - 1 , "CP", 2 );
		P[2] = flttocmplx( -1.3738431, 1.3883585 );
		fstrncpy( RTYPE(1,0) , rtype_s - 1 , "CP", 2 );
		P[3] = flttocmplx( -1.6369417, 0.8227968 );
		fstrncpy( RTYPE(2,0) , rtype_s - 1 , "CP", 2 );
		P[4] = flttocmplx( -1.7574108, 0.2728679 );
		fstrncpy( RTYPE(3,0) , rtype_s - 1 , "CP", 2 );

		}

	*nsects = iord - iord/2;

	*dcvalue = 1.0;

	/*  DONE                                                                         
	 * */
	return;
#undef	RTYPE
} 

