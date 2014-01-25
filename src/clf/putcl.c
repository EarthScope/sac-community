/** 
 * @file   putcl.c
 * 
 * @brief  Put an entry into a character list
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "clf.h"
#include "bot.h"
#include "bool.h"
#include "msg.h"
#include "co.h"
#include "dff.h"

#include "errors.h"
#include "string_utils.h"
#include "debug.h"

/** 
 * Put an entry into a character list
 * 
 * @param kcl 
 *    Character list
 * @param kcl_s 
 *    Length of \p kcl
 * @param kentry 
 *    Entry to put into \p kcl
 *    Trailing blanks are trimmed before putting into list.
 * @param kentry_s 
 *    Length of \p kentry
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_CHARACER_LIST_EXCEED_BOUNDS
 *    - ERROR_DELIMITER_FOUND_IN_ENTRY
 *
 * @date   870722:  Added descriptive error messages.
 * @date   860128:  Original version.
 *
 */
void 
putcl(char *kcl, 
      int   kcl_s, 
      char *kentry, 
      int   kentry_s, 
      int  *nerr) {

	char kdel;
	int ibeg, idel, iend_, ncl, nentry;
        char *strtemp;

	*nerr = 0;

	/* - Determine length of character list and  delimiter. */
	ncl = (kcl_s - 1);
	kdel = kcl[0];

	/* - Determine length of entry without trailing blanks. */
	nentry = indexb( kentry,kentry_s );

        strtemp = malloc(nentry+1);
        strncpy(strtemp,kentry,nentry);
        strtemp[nentry] = '\0';

	/* - Make sure delimiter is not present in entry. */
	idel = indexa( strtemp, nentry + 1, kdel, TRUE, TRUE );

	if( idel > 0 ){
		*nerr = ERROR_DELIMITER_FOUND_IN_ENTRY;
		setmsg( "ERROR", *nerr );
		apcmsg2(kentry,nentry);
		goto L_8888;
        }

	/* - Determine end of last entry in character list.
	 *   (This is the first nonoccurance of delimiter searching backwards.) */
	iend_ = indexa( kcl,kcl_s, kdel, FALSE, FALSE );

	/* - Make sure there is room in character list for entry and delimiter. */
	ibeg = iend_ + 2;
	if( (ibeg + nentry + 1) > ncl ){
               *nerr = ERROR_CHARACER_LIST_EXCEED_BOUNDS;
		setmsg( "ERROR", *nerr );
                apcmsg2(kentry,nentry);
		goto L_8888;
		}

	/* - Copy entry to character list. */
	subscpy( kcl, ibeg - 1, ibeg + nentry - 2, kcl_s - 1, strtemp);

L_8888:
        free(strtemp);
	return;
}

string_list *
string_list_new() {
    string_list *list;
    list = (string_list *) malloc(sizeof(string_list));
    return list;
}

string_list *
string_list_init() {
    string_list *list;
    list = string_list_new();
    if(list) {
        list->n       = 0;
        list->alloc   = 0;
        list->strings = NULL;
    }
    return list;
}

int
string_list_grow(string_list *list, int n) {
    int i;
    char **tmp;
    if(n < list->alloc) {
        return TRUE;
    }
    list->alloc = 1;
    while(list->alloc <= n) {
        list->alloc *= 2;
    }
    tmp = (char **) realloc(list->strings, sizeof(char *) * list->alloc);
    if(!tmp) {
        fprintf(stderr, "Error growing list for string\n");
        return FALSE;
    }
    list->strings = tmp;
    for(i = list->n; i < list->alloc; i++) {
        list->strings[i] = NULL;
    }
    return TRUE;
}

int
string_list_put(string_list *list, 
                char        *str,
                int          len) {
    if(!list) {
        return FALSE;
    }
    DEBUG("add '%s' n: %d alloc: %d\n", str, list->n, list->alloc);
    if(list->n + 1 >= list->alloc) {
        if(!string_list_grow(list, list->n+1)) {
            return FALSE;
        }
    }
    list->strings[list->n] = fstrdup(str, len);
    list->n++;
    return TRUE;
}
    
int
string_list_find(string_list *list,
                 char        *str,
                 int          len) {
    int i, k;
    char *tmp;

    tmp = fstrdup(str, len);
    k = -1;
    for(i = 0; i < list->n; i++) {
        if(strcmp(list->strings[i], tmp) == 0) {
            k = i;
            break;
        }
    }
    free(tmp);
    tmp = NULL;
    return k;
}

int
string_list_delete(string_list *list,
                   int          n) {
    if(n < 0 || n >= list->n) {
        return FALSE;
    }
    free(list->strings[n]);
    list->strings[n] = NULL;
    if(n < list->n - 1) {
        memmove(list->strings+n, 
                list->strings+n+1, 
                sizeof(char *) * (list->n - n - 1));
    }
    list->n--;
    return TRUE;
}

char * 
string_list_get(string_list *list,
                int          n) {
    if(n < 0) {
        n = n + list->n;
    }
    if(n >= 0 && n < list->n) {
        return list->strings[n];
    }
    return NULL;
}

void
string_list_clear(string_list *list) {
    int i;
    if(list && list->strings) {
        for(i = 0; i < list->n; i++) {
            if(list->strings[i]) {
                free(list->strings[i]);
                list->strings[i] = NULL;
            }
        }
        free(list->strings);
        list->strings = NULL;
        list->n       = 0;
        list->alloc   = 0;
    }
}

void
string_list_print(string_list *list) {
    int i;
    if(list) {
        for(i = 0; i < list->n; i++) {
            DEBUG("%d/%d: '%s'\n", i, list->n, list->strings[i]);
        }
    }
}

int
string_list_length(string_list *list) {
    if(list) {
        return list->n;
    }
    return -1;
}

void
string_list_extend(string_list *list, 
                   string_list *add) {
    int i;
    char *tmp;
    for(i = 0; i < add->n; i++) {
        tmp = string_list_get(add, i);
        string_list_put(list, tmp, strlen(tmp));
    }
}

char * 
string_list_pop(string_list *list) {
    char *str;
    str = NULL;
    if(list) {
        if(list->n > 0) {
            str = strdup(list->strings[list->n-1]);
            free(list->strings[list->n-1]);
            list->strings[list->n-1] = NULL;
            list->n--;
        }
    }
    return str;
}

string_list *
string_list_from_cfl(char *cfl, int len) {
    int i1, i2;
    string_list *list;
    list = string_list_init();
    i1 = 0;
    i2 = 0;
    while( lnxtcl(cfl, len, &i1, &i2) ) {
        string_list_put(list, cfl+i1-1, i2-i1+1);
    }
    return list;
}

char * 
string_list_to_cfl(string_list *list) {
    int i;
    string *str;
    char *tmp;
    str = string_new(" ");
    for(i = 0; i < list->n; i++) {
        str = string_append(str, string_list_get(list, i));
        str = string_append(str, " ");
    }
    tmp = strdup(string_string(str));
    string_free(&str);
    return tmp;
}

void
string_list_free(string_list *list) {
    string_list_clear(list);
    if(list) {
        if(list->strings) {
            free(list->strings);
            list->strings = NULL;
        }
        free(list);
        list = NULL;
    }
}
