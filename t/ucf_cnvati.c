#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "unit.h"
#include "ucf.h"

void
test_cnvati()
{
  /* test cnvati */

  int i;
  char *inputs[] = { "0",
                     "1",
                     "2",
                     "3",
                     "4",
                     "5",
                     "6",
                     "7",
                     "8",
                     "9",
                     "10",
                     "10 ",
                     "100",
                     "100 ",
                     "-1",
                     "-1 ",
                     "-10",
                     " 0 ",
                     "5e-03",
                     "5e+04",
                     "a",
                     "a ",
                     "1a",
                     "1a "
                   };
  const int max = sizeof (inputs) / sizeof (char *);

  for (i = 0; i < max; i++)
    {
      char *var = inputs[i];
      errno = 0;
      int gold = strtol (var, 0, 10);
      int has_error = 0;
      if (errno != 0)
        {
          has_error = 1;
        }

      int check;
      int lstrict;
      int nerr;
      nerr = 0;
      lstrict = 0;
      cnvati (var, strlen(var), &check, lstrict, &nerr);
      int pass = 1;
      if ((nerr != 0) && (has_error == 0))
        {
          pass = 0;
        }
      else if ((nerr == 0) && (has_error != 0))
        {
          pass = 0;
        }
      else if (check != gold)
        {
          pass = 0;
        }
      
      if((
          var[strlen(var)] == '\0' && 
          check != gold && 
          has_error == 0) || 
         (strchr(var,'a') != NULL && pass == 0) ) {
        not_ok (pass, "cnvati test: %s: check: %d (nerr: %d) gold: %d (error: %d) pass: %d",
                var, check, nerr, gold, has_error, pass);
      } else {
        ok (pass, "cnvati test: %s: check: %d (nerr: %d) gold: %d (error: %d) pass: %d",
            var, check, nerr, gold, has_error, pass);
      }
    }
}


