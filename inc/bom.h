/** 
 * @file   bom.h
 * 
 * @brief  Binary Operations
 * 
 */

#ifndef _BOM_H_
#define _BOM_H_

#include "mach.h"
#include "clf.h"

#define	MBFL	MDFL

/** 
 * @struct kmbom
 *    Binary Operations Lengths and Indicies
 */ 
struct t_cmbom {
  int nbfl;
  int ibflc;
  int nlenbf;
  int ndxhbf;
  int ndx1bf;
  int ndx2bf;
  int lnewhdr;	/* optionally let the header information come from
		   the new file being added or whatever in
		   addf, subf, mulf, and divf.  */
} cmbom;


/** 
 * @struct kmbom
 *    Binary Operations Characters
 */ 
struct t_kmbom {
  char kbfl[MAXCHARS];
  char kecnpt[9];
  char kecdel[9];
} kmbom;


void getbfl ( string_list *list,
              int ibfl, 
              int ldta, 
              int *nlen, 
              int *ndx1, 
              int *ndx2, 
              int *nerr);
void inibom ( void);
void relbfl ( int *nerr);
void xaddf  ( int *nerr);
void xboec  ( int *nerr);
void xbomc  ( int index, 
              int *nerr);
void xdivf  ( int *nerr);
void xmerge ( int *nerr);
void xmulf  ( int *nerr);
void xsubf  ( int *nerr);

int  vbeven ();
int  isFatal(char *key);
int  isWarning(char *key);
int  delta_equal(float t1, 
                 float t2, 
                 string_list *list1,
                 string_list *list2,
                 int n1,
                 int n2);
int npts_equal(int npts1, 
               int npts2,
               string_list *list1,
               string_list *list2,
               int n1,
               int n2);
int time_equal(int time1[6], 
               int time2[6],
               float b1,
               float b2,
               string_list *list1,
               string_list *list2,
               int n1,
               int n2);
int station_equal(char *name1, 
                  char *name2,
                  string_list *list1,
                  string_list *list2,
                  int n1,
                  int n2);






#endif /* _BOM_H_ */
