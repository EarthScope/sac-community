#include <stdlib.h>
#include <string.h>

#include "unit.h"
#include "bool.h"
#include "vars.h"
#include "dff.h"

// tests used here
void test_getventry ();

#define LIST_NAME    "listname "
#define LIST_LENGTH  1000


void
test_before_init_vars() {
  ok(cmvars.lvarsinit == FALSE, "cmvars.lvarsinit = %d",cmvars.lvarsinit);
}

void
test_init_vars() {
  int i;

  /* This calls inivars() and inimsg() and iniam() */
  initializevars(); 
  ok(cmvars.lvarsinit == TRUE, "cmvars.lvarsinit = %d",cmvars.lvarsinit);

  /* Only used in convlistname() */
  ok(cmvars.currentnode == 0, "cmvars.currentnode = %d", cmvars.currentnode);
  ok(vfilelist.nallocated == NVFILELIST, "vfilelist.nallocated = %d", vfilelist.nallocated);
  ok(vfilelist.nentries == 0, "vfilelist.nentries = %d", vfilelist.nentries);
  ok(kmvars.vabsflag == '#', "kmvars.vabsflag = %c", kmvars.vabsflag);
  ok(kmvars.vlistdelim == '.', "kmvars.vlistdelim = %c", kmvars.vlistdelim);
  
  for(i = 0; i < MAXVARS;i++) {
    ok(cmvars.varsindex[i] == -1,       "cmvars.varsindex[%d] = %d",i,cmvars.varsindex[i]);
    ok(cmvars.ncvarsname[i] == 0,       "cmvars.ncvarsname[%d] = %d",i,cmvars.ncvarsname[i]);
    ok(cmvars.varsmodified[i] == FALSE, "cmvars.varsmodified[%d] = %d",i,cmvars.varsmodified[i]);
    ok(cmvars.varsindirect[i] == FALSE, "cmvars.indirect[%d] = %d",i,cmvars.varsindirect[i]);
    ok(cmvars.varsnilindex[i] == 0,     "cmvars.nilindex[%d] = %d",i,cmvars.varsnilindex[i]);
  }
  
}

void
test_getventry() {

  int nerr, node, nvalue;
  char *value;

  putvvstring(LIST_NAME, strlen(LIST_NAME),
              "a", 1,
              1, "1", 1, &nerr);
  ok(nerr == 1203, "putvvstring error on return %d, not 1203", nerr);

  /* This list_name length needs to be longer than the actual list_name
     as the last character is chopped off at the end when it is set
     The length is also one less too.
     If the list length is too small, then the functions may fail
     due to insufficient resources.
  */
  createvlist(LIST_NAME, strlen(LIST_NAME), LIST_LENGTH, &node, &nerr);
  ok(nerr == 0, "createvlist error on return");

  /* Reduce the node number by one as the starting index starts at one */
  node -= 1;
  //  ok(existsvlist(LIST_NAME, strlen(LIST_NAME), "MEMORY", &node) == TRUE, 
  //   "Lists %s exists", LIST_NAME);
  
  /* Deletevlist does not reduce the number of lists available 
     Deletevlist does not return an error code if the list does not exist.
     Deletevlist does not seem to know which value to return as a node
  */
  deletevlist(LIST_NAME, strlen(LIST_NAME), "MEMORY", &nerr);
  
  ok(nerr == 0, "deletevlist error on return");
  
  //ok(existsvlist(LIST_NAME, strlen(LIST_NAME), "MEMORY", &node) == FALSE, "Lists %s exists", LIST_NAME);
  
  createvlist("Nothing ", strlen("Nothing "), LIST_LENGTH, &node, &nerr);
  ok(nerr == 0, "createvlist error on return list: Nothing");
  
  deletevlist("Nothing ", strlen("Nothing "), "MEMORY", &nerr);
  ok(nerr == 0, "deletevlist error on return");
  
  createvlist("Nothing1 ", strlen("Nothing "), LIST_LENGTH, &node, &nerr);
  ok(nerr == 0, "createvlist1 error on return");
  
  createvlist("Nothing2 ", strlen("Nothing "), LIST_LENGTH, &node, &nerr);
  ok(nerr == 0, "createvlist2 error on return");
  
  createvlist("Nothing3 ", strlen("Nothing "), LIST_LENGTH, &node, &nerr);    
  ok(nerr == 0, "createvlist3 error on return");
  
  deletevlist("Nothing2 ", strlen("Nothing2 "), "MEMORY", &nerr);
  ok(nerr == 0, "deletevlist2 error on return");
  
  deletevlist("Nothing3 ", strlen("Nothing3 "), "MEMORY", &nerr);
  ok(nerr == 0, "deletevlist3 error on return");
  
  deletevlist("Nothing1 ", strlen("Nothing1 "), "MEMORY", &nerr);
  ok(nerr == 0, "deletevlist1 error on return");
  
  createvlist(LIST_NAME, strlen(LIST_NAME), LIST_LENGTH, &node, &nerr);
  ok(nerr == 0, "createvlist error on return");

  putvvstring(LIST_NAME, strlen(LIST_NAME),
              "a", 1,
              1, "1", 1, &nerr);
  ok(nerr == 0, "putvvstring error on return %d, should be 0", nerr);
  
  value = calloc(100 + 1, sizeof(char));
  getvvstring(LIST_NAME, strlen(LIST_NAME), "a", 1, &nvalue, value, 100, &nerr); 
  ok(nerr == 0, "getvvstring error on return %d, should be 0", nerr);
  ok(strncmp(value, "1", nvalue) == 0, "getvvstring output:[%d] <%s>",nvalue, value);
  
}


int
main(int argc, char *argv[])
{
  ok(1 == 1, "initial test");
  sacio_initialize_common();
  // set_show_checks ();

  test_before_init_vars();
  test_init_vars();

  if (argc == 1) {
    test_getventry();
  } else {
    int i;
    for (i = 1; i < argc; i++) {
      char *item = argv[i];
      if (strcmp (item, "getventry") == 0) {
        test_getventry();
      }
    }
  }
  
  TEST_FINISH;
}

