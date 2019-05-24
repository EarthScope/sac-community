
#include <string.h>

#include "icm.h"
#include "bool.h"

#include "msg.h"
#include "bot.h"

#include "debug.h"

void /*FUNCTION*/
ckinst(fp, lfp, ip, lip, kp, kp_s, lkp, nerr)
     float fp[];
     int lfp[];
     int ip[];
     int lip[];
     char *kp;
     int kp_s;
     int lkp[];
     int *nerr;
{
#define KP(I_,J_)	(kp+(I_)*(kp_s)+(J_))
    char temp[21];

    int *const Lfp = &lfp[0] - 1;
    int *const Lip = &lip[0] - 1;
    int *const Lkp = &lkp[0] - 1;

    UNUSED(ip);
    UNUSED(fp);

    /* MODIFICATIONS:
     *    900409:  Added LNN instrument type.
     * */
    *nerr = 0;
    strcpy(temp, "                    ");

    if (memcmp(KP(0, 0), "ELMAG", 5) == 0 && (!Lfp[1] || !Lfp[2])) {
        error(*nerr = 2101, "");
    } else if (memcmp(KP(0, 0), "EYEOMG", 6) == 0 && (!Lip[1])) {
        error(*nerr = 2102, "");
    } else if (memcmp(KP(0, 0), "GENERAL", 7) == 0 && 
               ((!Lip[1]) || ((!Lfp[2] || (!Lfp[1])) || (!Lfp[3])))) {
        error(*nerr = 2103, "");
    } else if (memcmp(KP(0, 0), "POLEZERO", 8) == 0 &&
               ( !Lkp[2] || lstrip(KP(1,0))[0] == 0) ) {
        // Ignore isolated POLEZERO option
    } else if (memcmp(KP(0, 0), "FAP", 3) == 0 && (!Lkp[2])) {
        error(*nerr = 2104, "FAP");
    } else if (memcmp(KP(0, 0), "LLL", 3) == 0 && (!Lkp[2])) {
        error(*nerr = 2104, "LLL");
    } else if (memcmp(KP(0, 0), "LLL", 3) == 0 && (Lkp[2])) {
        modcase(TRUE, KP(1, 0), 8, temp);
        if (((((temp[0] != 'L' && temp[0] != 'M') && temp[0] != 'K') &&
              temp[0] != 'E') && temp[0] != 'B') ||
            (((temp[1] != 'V' && temp[1] != 'R') && temp[1] != 'T') &&
             temp[1] != 'B')) {
            error(*nerr = 2105, "LLL: %s", KP(1,0));
        } else {
            if (strcmp(temp, "BB                  ") == 0) {
                if (!Lfp[1] || !Lfp[3]) {
                    error(*nerr = 2106, "");
                }
            }
        }

    } else if (memcmp(KP(0, 0), "NORESS", 6) == 0 && (!Lkp[2])) {
        error(*nerr = 2104, "NORESS");
    } else if (memcmp(KP(0, 0), "NORESS", 6) == 0 && (Lkp[2])) {
        modcase(TRUE, KP(1, 0), 8, temp);
        if (((temp[0] != 'L' && temp[0] != 'I') && temp[0] != 'S') ||
            temp[1] != 'P') {
            error(*nerr = 2105, "NORESS: %s", KP(1,0));
        }

    } else if (memcmp(KP(0, 0), "PORTABLE", 8) == 0 && (((!Lfp[1]) || (!Lfp[3]
                                                         )) || (!Lfp[4]))) {
        error(*nerr = 2107, "");
    } else if (memcmp(KP(0, 0), "RSTN", 4) == 0 && (!Lkp[2])) {
        error(*nerr = 2104, "RSTN.");
    } else if (memcmp(KP(0, 0), "RSTN", 4) == 0 && Lkp[2]) {
        modcase(TRUE, KP(1, 0), 8, temp);
        if ((((((((memcmp(temp, "CP", 2) != 0 && memcmp(temp, "NT", 2) != 0) &&
                  memcmp(temp, "NY", 2) != 0) && memcmp(temp, "ON", 2) != 0) &&
                memcmp(temp, "SD", 2) != 0) || 
               (temp[2] != 'K' && temp[2] != '7')) ||
              ((temp[3] != 'L' && temp[3] != 'M') && temp[3] != 'S')) ||
             temp[4] != '.') || ((temp[5] != 'Z' && temp[5] != 'N') &&
                                 temp[5] != 'E')) {
            error(*nerr = 2105, "RSTN: %s", KP(1,0));
        }

    } else if (memcmp(KP(0, 0), "SANDIA", 6) == 0 && (!Lkp[2])) {
        error(*nerr = 2104, "SANDIA.");
    } else if (memcmp(KP(0, 0), "SANDIA", 6) == 0 && Lkp[2]) {
        modcase(TRUE, KP(1, 0), 8, temp);
        if (temp[0] != 'N' && temp[0] != 'O') {
            error(*nerr = 2105, "SANDIA: %s", KP(1,0));
        } else {
            if (((((temp[1] != 'T' && temp[1] != 'L') && temp[1] != 'B') &&
                  temp[1] != 'D') && temp[1] != 'N') && temp[1] != 'E') {
                error(*nerr = 2105, "SANDIA: %s", KP(1,0));
            } else {
                if (temp[1] == 'E' && temp[0] != 'O') {
                    error(*nerr = 2105, "SANDIA: %s\n"
                          " Invalid combination of \"O\" and \"E\".", KP(1,0));
                }
                if (temp[0] == 'N' &&
                    ((temp[2] != 'V' && temp[2] != 'R') && temp[2] != 'T')) {
                    error(*nerr = 2105, "SANDIA: %s\n"
                          "Sub-type \"N\" must be combined "
                          "with \"V\", \"R\", or \"T\".", KP(1,0));
                }
            }
        }

    } else if (memcmp(KP(0, 0), "SRO", 3) == 0 && (!Lkp[2])) {
        error(*nerr = 2104, "SRO.");
    } else if (memcmp(KP(0, 0), "SRO", 3) == 0 && Lkp[2]) {
        modcase(TRUE, KP(1, 0), 8, temp);
        if ((strcmp(temp, "BB") == 0 || strcmp(temp, "SP") == 0) ||
            strcmp(temp, "LPDE") == 0) {
        } else {
            error(*nerr = 2105, "SRO: %s", KP(1,0));
            if (strcmp(temp, "LPST                ") == 0)
                mprint("This sub-type is not currently supported.");
        }

    } else if (memcmp(KP(0, 0), "REFTEK", 6) == 0 && 
               (((!Lfp[1] || !Lfp[3]) || !Lfp[4]) || !Lfp[6])) {
        error(*nerr = 2113, "");
    } else if (memcmp(KP(0, 0), "LNN", 3) == 0 && !Lkp[2]) {
        error(*nerr = 2104, "LNN.");
    } else if (memcmp(KP(0, 0), "LNN", 3) == 0 && Lkp[2]) {
        modcase(TRUE, KP(1, 0), 8, temp);
        if (memcmp(temp, "BB", 2) != 0 && memcmp(temp, "HF", 2) != 0) {
            error(*nerr = 2105, "LNN: %s\n"
                  " Allowed subtypes are BB and HF.", KP(1,0));
        }
    } else {
        
    }

    return;

#undef	KP
}                               /* end of function */
