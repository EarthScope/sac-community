
#include <assert.h>
#include "cpf.h"
#include "unit.h"
void *tokenize_line(char *in);

int
main() {
    int rv;

    //printf("ADDING HELLO\n");
    tokenize_line("HELLO");
    //printf("CHECKING ARGUMENT\n");
    
    rv = lckey("THERE", 6);
    ok(rv == 0, "THERE != HELLO %d", rv);

    rv = lckey("HELLO", 0);
    ok(rv == 1, "HELLO == HELLO %d",rv);

    tokenize_line("HELLO");
    rv = lckey("HELLO$", 0);
    ok(rv == 1, "HELLO$ == HELLO %d",rv);

    tokenize_line("HELLO");
    rv = lckey("H#ELLO$", 0);
    ok(rv == 1, "H#ELLO$ == HELLO %d",rv);

    tokenize_line("HELLO");
    rv = lckey("HE#LLO$", 0);
    ok(rv == 1, "HE#LLO$ == HELLO %d",rv);

    tokenize_line("HELLO");
    rv = lckey("HE#LLO$", 0);
    ok(rv == 1, "HE#LLO$ == HELLO %d",rv);

    tokenize_line("HELLO");
    rv = lckey("HEL#LO$", 0);
    ok(rv == 1, "HEL#LO$ == HELLO %d", rv);

    tokenize_line("HELLO");
    rv = lckey("HELL#O$", 0);
    ok(rv == 1, "HELL#O$ == HELLO %d", rv);

    tokenize_line("HELLO");
    rv = lckey("HELLO#$", 0);
    ok(rv == 1, "HELLO#$ == HELLO %d", rv);

    tokenize_line("HELLNO");
    rv = lckey("HEL#LO$", 0);
    ok(rv == 0, "HEL#LO$ != HELLNO %d", rv);

    tokenize_line("triangle-pulse.sac");
    rv = lckey("TRI#ANGLE$", 0);
    ok(rv == 0, "TRI#ANGLE$ ==  triangle-pulse.sac", rv);

    TEST_FINISH;

}
