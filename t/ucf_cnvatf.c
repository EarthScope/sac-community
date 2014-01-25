
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "unit.h"
#include "ucf.h"

#ifndef HAVE_FUNC_STRTOF
float
strtof(const char *nptr, char ** endptr) {
  return (float) strtod(nptr, endptr);
}
#endif 

void
test_cnvatf ()
{
  /* test cnvatf */

  int i;
  char *inputs[] = { "0.0",
                     "1.0",
                     "2.0",
                     "3.0",
                     "4",
                     ".5",
                     "6",
                     "0.237",
                     "0.237 ",
                     "8",
                     "9",
                     "10",
                     "10 ",
                     "100",
                     "100 ",
                     "-1",
                     "-1 ",
                     "-1.5",
                     "-1.5 ",
                     "-1.0 ",
                     "-10",
                     "-10.0",
                     "-10 ",
                     " 0 ",
                     "5e-03",
                     "5e+04",
                     "5e+2",
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
      float gold = strtof (var, NULL);  /* Check output against strtof() */
      int has_error = 0;
      if (errno != 0) {
        fprintf(stderr, "errno: %s\n", strerror(errno));
        has_error = 1;
      }

      float check;
      int lstrict;
      int nerr;
      nerr = 0;
      lstrict = 0;
      cnvatf (var, strlen(var), &check, lstrict, &nerr);
      int pass = 1;
      if ((nerr != 0) && (has_error == 0)) {
        pass = 0;
      } else if ((nerr == 0) && (has_error != 0)) {
        pass = 0;
      } else if (check != gold) {
        pass = 0;
      }
      
      if(nerr != 0 ||   /* An Error on conversion in cnvatf() */
         //         strchr(var,'e') != NULL || /* String has an 'e' in it */
         //         strchr(var,'E') != NULL || /* String has an 'E' in it */
         ( var[strlen(var)] == '\0' && has_error == 0 && check != gold ) )
          {
        not_ok (pass, "cnvatf test: %s: check: %f (%d) gold: %f (%d)",
            var, check, nerr, gold, has_error);        
      } else {
        ok (pass, "cnvatf test: %s: check: %f (%d) gold: %f (%d)",
            var, check, nerr, gold, has_error);
      }

    }
}
