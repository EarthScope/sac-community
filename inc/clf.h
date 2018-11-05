/** 
 * @file   clf.h
 *
 * @brief  Character List Functions
 * 
 */

#ifndef _CLF_H_
#define _CLF_H_

int fndelcl(char *kcl, int kcl_s, int fileNumber);
int ldelcl(char *kcl, int kcl_s, char *kentry, int kentry_s);
int lnumcl(char *kcl, int kcl_s, int num, int *index1, int *index2);
int lnxtcl(char *kcl, int kcl_s, int *index1, int *index2);
int lnxtcl(char *kcl, int kcl_s, int *index1, int *index2);
int nfndcl(char *kcl, int kcl_s, char *kentry, int kentry_s, int *index1,
           int *index2);
void putcl(char *kcl, int kcl_s, char *kentry, int kentry_s, int *nerr);

#endif /* _CLF_H_ */
