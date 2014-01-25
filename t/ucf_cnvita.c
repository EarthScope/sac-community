
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "unit.h"
#include "ucf.h"

void
test_cnvita()
{
  /* test cnvita */

  int i;
  int j;
  int inputs[] = { 0,
                   1,
                   -1,
                   2,
                   10,
                   -10,
                   100,
                   1000,
                   100000,
                   1e+03
                   -2e+03
  };
  const int max = sizeof (inputs) / sizeof (char *);

  for (j = 0; j < 10; j++)
    {
      for (i = 0; i < max; i++) /* Loop through all the inputs */
        {
          int var = inputs[i];
          char gold[1000];
          char fmt[25];
          char gold_size[1000];
          int gold_res;
          gold_res = sprintf (gold, "%d", var);
          
          sprintf(fmt, "%%%dd", j-1);
          sprintf(gold_size, fmt, var);
          char check[1000];
          int check_size = j;
          cnvita (var, check, check_size);
          int check_res = 0;
          if (strcmp(check, "BADINPUT") == 0)
            {
              check_res = 1;
            }
          int pass = 1;
          if ((check_res != 0) && (gold_res != 0))
            {
              pass = 0;
            }
          else if ((check_res == 0) && (gold_res == 0))
            {
              pass = 0;
            }
          else if (strcmp (gold, check) != 0)
            {
              pass = 0;
            }
          if(strcmp(gold_size, check) == 0 && pass == 0 && strcmp(gold,check) != 0) {
            not_ok (pass, "cnvita test: %d: check: '%s' (%d) gold: '%s' (%d), cs: %d '%s' %d",
                var, check, check_res, gold, gold_res, check_size, gold_size, 
                strcmp(gold_size, check) == 0 );            
          } else {
            ok (pass, "cnvita test: %d: check: '%s' (%d) gold: '%s' (%d), cs: %d '%s' %d",
                var, check, check_res, gold, gold_res, check_size, gold_size, 
                strcmp(gold_size, check) == 0 );
          }
        }
    }
}


