/** 
 * @file   tok.h
 * 
 * @brief  Tokens
 * 
 */

#ifndef _TOK_H_
#define _TOK_H_

#define	MMSGDL	5
#define	MTOKDL	5


/** 
 * @struct kmtok
 *    Token Characters 
 */
struct t_kmtok {
  char ktokdl[MTOKDL];
  char kmsgdl[MMSGDL];
} kmtok;


/** 
 * @struct kmtok
 *    Token Lengths
 */
struct t_cmtok {
  int ntokdl;
  int nmsgdl;
} cmtok;


#endif /* _TOK_H_ */
