/** 
 * @file   tokdel.c
 * 
 * @brief  Define token and message delimiters
 * 
 */

#include "ucf.h"
#include "tok.h"
#include "co.h"

/** 
 * Define token and message delimiters for poptok()
 * 
 * @param ktd 
 *    Token delimiters
 * @param ktd_s 
 *    Length of \p ktd
 * @param ntd 
 *    Number of delimiters in \p ktd, max 5
 * @param kmd 
 *    Message delimiters
 * @param kmd_s 
 *    Length of \p kmd
 * @param nmd 
 *    Number of delimiters in \p mkd, max 5
 *
 * @date   820419:  Original version.
 *
 */
void 
tokdel(char *ktd, 
       int   ktd_s, 
       int   ntd, 
       char *kmd, 
       int   kmd_s, 
       int   nmd) {

#define KTD(I_,J_)	(ktd+(I_)*(ktd_s)+(J_))
#define KMD(I_,J_)	(kmd+(I_)*(kmd_s)+(J_))

	int j, j_;

	/* - Store symbol delimiters into common variables. */
	cmtok.ntokdl = min( ntd, MTOKDL );
	for( j = 1; j <= cmtok.ntokdl; j++ ){
		j_ = j - 1;
		kmtok.ktokdl[j_] = KTD(j_,0)[0];
        }
        
	/* - Store message delimiters into common variables. */
	cmtok.nmsgdl = min( nmd, MMSGDL );
	for( j = 1; j <= cmtok.nmsgdl; j++ ){
		j_ = j - 1;
		kmtok.kmsgdl[j_] = KMD(j_,0)[0];
        }
        
	return;

#undef	KMD
#undef	KTD

}

