#ifndef __CSSARCHITECTURE_H__
#define __CSSARCHITECTURE_H__

/* 
   There is a static int defined in cssArchitecture.c
static int big = 1 ;
   One indicates that the code is being run on a bigendin machine.
   Zero indicates a littlendin machine.

   This value is set by SetIndianSize(), and is accessed by IsBigendin().
*/

void SetIndianSize() ;
int  IsBigendin() ;

#endif
