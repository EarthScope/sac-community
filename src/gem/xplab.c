
/** 
 * @file xplab.c
 *
 * @brief Plabel Command Interface
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "gem.h"
#include "bool.h"

#include "string/array.h"

#include "msg.h"
#include "cpf.h"

static array_t *store = NULL;

label * 
label_new() {
  label *p;
  p = (label *) malloc(sizeof(label));
  if(!p) {
    return NULL;
  }
  p->plot     = FALSE;
  p->relative = FALSE;
  p->x        = 0.50;
  p->y        = 0.20;
  p->size     = cmgem.tsdef;
  p->angle    = TEXT_HORIZONTAL;
  p->text     = NULL;
  return p;
}

void
label_free_text(label *p) {
  if(!p) { 
    return;
  }
  if(p->text) {
    free(p->text);
    p->text = NULL;
  }
}

void
label_text(label *p, char *text) {
  if(!p) {
    return;
  }
  label_free_text(p);
  p->text = strdup(text);
}

void
label_free(label *p) {
  if(!p) {
    return;
  }
  label_free_text(p);
  free(p);
  p = NULL;
}
void
label_obj_free(void *obj) {
  label *p = (label *) obj;
  label_free(p);
  p = NULL;
}

void
label_obj_print(void *obj) {
  label *p = (label *) obj;
  fprintf(stderr, 
          "label: '%s' on: %d relative: %d "
          "position: (%f,%f) size: %f angle: %f\n", 
          p->text, p->plot, p->relative, p->x, p->y, p->size, p->angle);
}

int
label_obj_compare(void *pa, void *pb) {
  label *a = (label *) pa;
  label *b = (label *) pb;
  return strcmp(a->text, b->text);
}

void
label_store_init() {
  if(store) {
    return;
  }
  store = array_new();
  store->print   = label_obj_print;
  store->compare = label_obj_compare;
  store->free    = label_obj_free;
}

int
label_store_length() {
  if(!store) { 
    label_store_init();
  }
  return array_length(store);
}


label * 
label_store_get(int n) {
  label *p;
  n--;
  if(!store) {
    label_store_init();
  }
  if(n > array_length(store)) {
    fprintf(stderr, "LABEL: attempted access outside of array bounds: %d bounds: [0,%d]\n", n, array_length(store));
    return NULL;
  }
  if(n < 0 || n == array_length(store)) {
    p = label_new();
    array_push(store, p);
  } else {
    p = (label *) array_element(store, n);
  }
  return p;
}


/** 
 * Parse the parameer setting command: plabel
 *
 * @param *nerr
 *    Error return flag
 *    - 0 on Success
 *    - Non-Zero on Failure
 *
 * @date   830818:  Changes due to new text size and angle attributes.
 * @date   820924:  Moved LCQUOT to top of parse loop.
 * @date   820614:  Original version.
 *
 */
void
xplab(int *nerr) {
  int iplabs, notusd, ntemp;
  double temp[3];
  char str[2048];
  label *p, *prev;

  double *const Temp = &temp[0] - 1;
  memset(str, 0, sizeof(str));

	*nerr = 0;
  prev  = NULL;
  p     = NULL;      
	/* - Parse positional tokens first. */
	/* -- "n":  the number of the label to modify. */
        cmgem.nplab = -1;
	if( lcirc(1, label_store_length()+1, &cmgem.nplab ) ){
           if(cmgem.nplab == -1) {
               setmsg("ERROR", 3501);
               apimsg( label_store_length() + 1);
               outmsg();
               clrmsg();
               return;
             }
    p = label_store_get( cmgem.nplab );
  } else { /* No Number Given -- increment the label number. */
    p = label_store_get( -1 );
    cmgem.nplab = label_store_length();
  }
  /* Find previous label if label number is bigger than 1 */
  if(cmgem.nplab > 1) {
    prev = label_store_get( cmgem.nplab );
  }
	/* - If this is the first reference to this label number,
	 *   assume it is to be placed "below" the last one,
	 *   and using the same size characters. */
	if( prev ) {
    if(p && p->text == NULL ) {
      p->relative = TRUE;
      p->size     = prev->size;
    }
  }
	/* - Loop on each token in command: */        
L_1000:
	if( lcmore( nerr ) ){

          if( lclog( &(p->plot) ) ){ 
            /* -- Turn plot labeling on/off: */
          } else if( lcquot( 2048, str, 2048, &notusd)) {
            /* -- Define text of plot label: */
            str[notusd+1] = 0;
            p->text = strdup(str);
            p->plot = TRUE;
          } else if( lklist( "S$",3, (char*)kmgem.ktxsiz,9, MTXSIZ, &iplabs)) {
            /* -- Set plot label size: */
            p->size = cmgem.txsiz[iplabs-1];
          } else if( lkra( "P$",3, 2, 3, temp, &ntemp ) ) {
            /* -- Set location of plot label: */
            p->relative = FALSE;
            p->x = Temp[1];
            p->y = Temp[2];
            if( ntemp == 3 ){
              p->angle = Temp[3];
            } else {
              p->angle = 0.0;
            }
            
          } else if( lckey( "B$",3 ) ){
            /* -- Set location of plot label to be below previous label. */
            if(prev) {
              p->relative = TRUE;
            } else {
              p->relative = FALSE;
            }
          } else {
            /* -- Bad syntax. */
            cfmt( "ILLEGAL OPTION:",17 );
            cresp();
            
          }
          goto L_1000;
        }
	return;
        
}

