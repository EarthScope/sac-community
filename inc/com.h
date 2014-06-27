/** 
 * @file   com.h
 * 
 * @brief  Command Parsing Functions
 * 
 */

#ifndef _COM_H_
#define _COM_H_

/** 
 * @param MCOM 
 *    Maximum Number of commands on the command stack
 */
#define	MCOM	200
/** 
 * @param MKARGS 
 *    Maximum Length of arguments to commands
 */
#define MKARGS  100

struct t_cmcom {
  int   ncerr;  /* Command Error number */
} cmcom;


#endif /* _COM_H_ */
