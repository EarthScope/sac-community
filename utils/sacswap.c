/*
   *******************************************************************
   * (c) Copyright 2002 The Regents of the University of California. *
   *     All Rights Reserved.                                        *
   *                                                                 *
   *     This work was produced under the sponsorship of the         *
   *     United States Department of Energy.                         *
   *     The Government retains certain rights therein.              *
   *******************************************************************

 author: Peter Goldstein
 date:   august 2002
 where:  LLNL
 Updates: 
     - Brian Savage <savage13@dtm.ciw.edu>
          - Added Error checking for reads and file length
	  - Remove Hard coded numbers for readability
	  - (Kuang He, U. Conn.) Use long swaps for data to avoid signaling
	    NaN bit changes with floating point arithmetic (08/09/01)
    - Brian Savage <savage@uri.edu>
          - Updated to use BSD sacio to handle v6 and v7

 *********************************************************************
*/

#include <stdio.h>
#include <sacio.h>

#define PROGRAM "sacswap"

int
main(int ac, char **av) {

  int i = 0;
  char sacfile[2048] = {0};
  sac *s = NULL;
  int nerr = 0;

  /* Check usage */
  if (ac < 2) {
    fprintf(stderr, "Usage: %s sacfile(s)\n", PROGRAM);
    fprintf(stderr, "\tconvert sacfiles between byte orders\n");
    fprintf(stderr, "\toutput files are appended with .swap\n");
    fprintf(stderr, "\twill read either byte order\n");
    exit(-1);
  }
  /* Loop over files */
  for(i = 1; i < ac; i++) {

      if(!(s = sac_read(av[i], &nerr))) {
          printf("%s: Error Opening file %s\n", PROGRAM, av[i]);
          continue;
      }

      snprintf(sacfile, sizeof sacfile, "%s.swap", av[i]);

      fprintf(stderr, "%s: Writing %s with npts = %d %s\n", PROGRAM,
              sacfile, s->h->npts,
              (s->m->swap) ? "non-native => native" : "native => non->native");
      s->m->swap = ! s->m->swap;

      sac_write(s, sacfile, &nerr);
      sac_free(s);
  }

  return 0;
}

