
#include <string.h>

#include "unit.h"
#include "dff.h"

void test_cnvati ();
void test_inctim ();
void test_cnvita();
void test_cnvatf();
void test_cnvfta();

int
main(int argc, char *argv[])
{
  ok(1 == 1, "initial test");
  sacio_initialize_common();

  // set_show_checks ();

  if (argc == 1)
    {
      test_inctim();
      test_cnvati();
      test_cnvita();
      test_cnvatf();
      test_cnvfta();
    }
  else
    {
      int i;
      for (i = 1; i < argc; i++)
        {
          char *item = argv[i];
          if (strcmp (item, "inctim") == 0)
            {
              test_inctim();
            }
          else if (strcmp (item, "cnvati") == 0)
            {
              test_cnvati();
            }
          else if (strcmp (item, "cnvita") == 0)
            {
              test_cnvita();
            }
          else if (strcmp (item, "cnvatf") == 0)
            {
              test_cnvatf();
            }
          else if (strcmp (item, "cnvfta") == 0)
            {
              test_cnvfta();
            }
        }
    }

  TEST_FINISH;
}

