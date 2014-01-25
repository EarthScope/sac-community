
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "unit.h"
#include "ucf.h"

void
test_cnvfta ()
{
  /* test cnvfta */

  int i;
  int digits;
  int sig;
  float inputs[] = { 0,
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

  for (sig = 0; sig < 3; sig++)
    {
      for (digits = 0; digits < 10; digits++)
        {
          const char fmt[100];
          sprintf ((char*)&fmt, "%%%d.%df", digits, sig);
          for (i = 0; i < max; i++)
            {
              float var = inputs[i];
              char gold[1000];
              int gold_res;
              /* gold_res = sprintf (gold, "%f", var); */
              gold_res = sprintf (gold, fmt, var);

              char check[1000];
              int check_size = 0;
              cnvfta (var, digits, sig, check, check_size);
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
              else if ((check_res == 0) && (gold_res < 0))
                {
                  pass = 0;
                }
              else if (strcmp (gold, check) != 0)
                {
                  pass = 0;
                }

              ok (pass, "cnvfta test: %f: check: %s (%d) gold: %s (%d), cs: %d",
                  var, check, check_res, gold, gold_res, check_size);
            }
        }
    }

}
