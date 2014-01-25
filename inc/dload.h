/**
 * @file   dload.h
 * 
 * @brief  Loadable Shared Objects
 * 
 * 
 */

#ifndef _DLOAD_H_
#define _DLOAD_H_

/** 
 * @param MEXTCOMS
 *    Number of Maximum External Commands
 */
#define MEXTCOMS 50

/** 
 * @struct cmextcom 
 *    External Command Storage 
 */
struct t_cmextcom {
  int nfiles;
  int (*extfuncs[MEXTCOMS])();
} cmextcom;


#endif /* _DLOAD_H_ */
