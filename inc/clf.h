/** 
 * @file   clf.h
 *
 * @brief  Character List Functions
 * 
 */

#ifndef _CLF_H_
#define _CLF_H_

int fndelcl ( char *kcl, 
              int kcl_s, 
              int fileNumber);
int ldelcl ( char *kcl, 
             int kcl_s, 
             char *kentry, 
             int kentry_s);
int lnumcl ( char *kcl, 
             int kcl_s, 
             int num, 
             int *index1, 
             int *index2);
int lnxtcl ( char *kcl, 
             int kcl_s, 
             int *index1, 
             int *index2);
int lnxtcl ( char *kcl, 
             int kcl_s, 
             int *index1, 
             int *index2);
int nfndcl ( char *kcl, 
             int kcl_s, 
             char *kentry, 
             int kentry_s, 
             int *index1, 
             int *index2);
void putcl ( char *kcl, 
             int kcl_s, 
             char *kentry, 
             int kentry_s, 
             int *nerr);

struct _string_list {
    int    n;
    int    alloc;
    char **strings;
};
typedef struct _string_list string_list;

string_list * string_list_new      ();
string_list * string_list_init     ();
int           string_list_grow     (string_list *list, int n);
int           string_list_put      (string_list *list, char *str, int len);
char *        string_list_get      (string_list *list, int n);
int           string_list_find     (string_list *list, char *str, int len);
int           string_list_delete   (string_list *list, int n);
void          string_list_print    (string_list *list);
int           string_list_length   (string_list *list);
void          string_list_extend   (string_list *list, string_list *add);
string_list * string_list_from_cfl (char *cfl, int len);
char *        string_list_to_cfl   (string_list *list);
void          string_list_free     (string_list *list);
char *        string_list_pop      (string_list *list);
void          string_list_clear    (string_list *list);


#endif /* _CLF_H_ */
