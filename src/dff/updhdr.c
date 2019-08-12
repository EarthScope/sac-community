/** 
 * @file   updhdr.c
 * 
 * @brief  Update the header
 * 
 */

#include <string.h>

#include "amf.h"
#include "dff.h"
#include "hdr.h"
#include "msg.h"
#include "bool.h"

#include "errors.h"

HDR_EXTERN

/** 
 * Update an old header to the current version
 * 
 * @param nerr 
 *   Error Return Flag
 *   - 0 on Success
 *
 * @date   821001:  Header version 6 installed.
 * @date   801018:  Header version 5 installed.
 * @date   800809:  Fixed bug with headers of files from Octopus.
 * @date   800510:  Header version 4 installed.
 * @date   800105:  Original Prime version.
 *
 */
void
updhdr(int *nerr) {

    char khdr18[9];
    int icomp, jdx;
    float horzo;
    sac *s;

    *nerr = 0;
    s = sacget_current();
    /* - The header is updated one version at a time until
     *   the current version is reached. 
     */
    while (s->h->nvhdr < cmhdr.nvhdrc) {

        /* - Version 2 changed the values in each of the header field types
         *   used to represent an undefined state.  Previously they were all
         *   equivalent to the ascii 'UNDEF'.  Now they are:
         *   - FUNDEF = -12345. for floating point fields.
         *   - NUNDEF = -12345 for integer fields.
         *   - IUNDEF = -12345 for selected value fields.
         *   - KUNDEF = 'UNDEF' for alphanumeric fields.
         *   - no value for undefined logical fields.
         * - Version 1 was never supported on the Prime. */

        if (s->h->nvhdr == 1 || s->h->nvhdr == SAC_INT_UNDEFINED) {
            s->h->nvhdr = 2;
        }

        /* - Version 3:
         *   - added FINI in FHDR(40).
         *   - added SB in FHDR(55).
         *   - added SDELTA in FHDR(56).
         *   - added FMEAN in FHDR(57).
         *   - replaced ICOMP in IHDR(4) and HORZO in FHDR(21)
         *     with CMPAZ in FHDR(58), CMPINC in FHDR(59),
         *     KCMPNM in KHDR(1,6), and LPSPOL in LHDR(2).
         *   - possible values of ICOMP were IRADNV=23, ITANNV=24,
         *     IRADEV=25, ITANEV=26, INORTH=27, IEAST=28,
         *     IHORZA=29, IDOWN=30, and IUP=31. */

        else if (s->h->nvhdr == 2) {
            icomp = IHDR(s)[4];
            IHDR(s)[4] = SAC_ENUM_UNDEFINED;
            horzo = VALUE(xfhdr(s, 21));
            VALUE(xfhdr(s, 21)) = SAC_FLOAT_UNDEFINED;
            if (icomp == 23) {
                if (memcmp(s->h->kstnm, "ELKO", 4) == 0) {
                    s->h->cmpaz = 10.4699;
                } else if (memcmp(s->h->kstnm, "KANA", 4) == 0) {
                    s->h->cmpaz = 93.01;
                } else if (memcmp(s->h->kstnm, "LAND", 4) == 0) {
                    s->h->cmpaz = 185.25;
                } else if (memcmp(s->h->kstnm, "MINA", 4) == 0) {
                    s->h->cmpaz = 307.71;
                } else {
                    s->h->cmpaz = SAC_FLOAT_UNDEFINED;
                }
                s->h->cmpinc = 90.;
            } else if (icomp == 24) {
                if (memcmp(s->h->kstnm, "ELKO", 4) == 0) {
                    s->h->cmpaz = 280.4699;
                } else if (memcmp(s->h->kstnm, "KANA", 4) == 0) {
                    s->h->cmpaz = 3.01;
                } else if (memcmp(s->h->kstnm, "LAND", 4) == 0) {
                    s->h->cmpaz = 95.25;
                } else if (memcmp(s->h->kstnm, "MINA", 4) == 0) {
                    s->h->cmpaz = 217.71;
                } else {
                    s->h->cmpaz = SAC_FLOAT_UNDEFINED;
                }
                s->h->cmpinc = 90.;
            } else if ((icomp == 25 || icomp == 26) || icomp == 29) {
                s->h->cmpaz = horzo;
                s->h->cmpinc = 90.;
                s->h->lpspol = TRUE;
            } else if (icomp == 27) {
                s->h->cmpaz = 0.;
                s->h->cmpinc = 90.;
                s->h->lpspol = TRUE;
            } else if (icomp == 28) {
                s->h->cmpaz = 90.;
                s->h->cmpinc = 90.;
                s->h->lpspol = TRUE;
            } else if (icomp == 30) {
                s->h->cmpaz = 0.;
                s->h->cmpinc = 180.;
                s->h->lpspol = FALSE;
            } else if (icomp == 31) {
                s->h->cmpaz = 0.;
                s->h->cmpinc = 0.;
                s->h->lpspol = TRUE;
            } else {
                s->h->cmpaz = SAC_FLOAT_UNDEFINED;
                s->h->cmpinc = SAC_FLOAT_UNDEFINED;
                s->h->lpspol = TRUE;
            }
            s->h->nvhdr = 3;

            /* - Version 4:
             *   - added LOVROK in LHDR(3).
             *   - moved NZHOUR from NHDR(5) to NHDR(4).
             *   - moved NZMIN from NHDR(6) to NHDR(5).
             *   - equivalenced NZDTTM array to NZYEAR in NHDR(2).
             *   - changed entire KHDR array.  Before each entry was 24 characters
             *     in length.  Now they may be any multiple of 8 characters in length.
             *     KHDR was changed to a 1 dimensional array but the length was
             *     not changed.
             *
             *         variable           old_loc   new_loc   new_len
             *         --------           -------   -------   -------
             *         KSTNM             1         1         1
             *         KEVNM             4         2         2
             *         KHOLE             7         4         3
             *         KZTIME            10        7         2
             *         KZDATE            13        9         3
             *         KCMPNM            16       12         3
             *         KUSER             NEW       13        3 */

        } else if (s->h->nvhdr == 3) {
            NHDR(s)[4] = NHDR(s)[5];
            NHDR(s)[5] = NHDR(s)[6];
            NHDR(s)[6] = SAC_INT_UNDEFINED;
            LHDR(s)[3] = TRUE;
            strcpy(khdr(s, 1), khdr(s, 3));
            strcpy(khdr(s, 2), khdr(s, 4));
            strcpy(khdr(s, 3), khdr(s, 6));
            strcpy(khdr(s, 4), khdr(s, 7));
            strcpy(khdr(s, 5), khdr(s, 8));
            strcpy(khdr(s, 6), khdr(s, 9));
            strcpy(khdr(s, 7), khdr(s, 10));
            strcpy(khdr(s, 8), khdr(s, 12));
            strcpy(khdr(s, 9), khdr(s, 13));
            strcpy(khdr(s, 10), khdr(s, 14));
            strcpy(khdr(s, 11), khdr(s, 15));
            strcpy(khdr(s, 12), "UNDEF   ");
            for (jdx = 13; jdx < SAC_HEADER_STRINGS; jdx++) {
                strcpy(khdr(s, jdx), "        ");
            }
            s->h->nvhdr = 4;

            /* - Version 5:
             *   - Changed alphanumeric undefined fields from 'UNDEF' to '-12345'.
             *   - Changed all KHDR fields from dimensioned CHAR*8 variables
             *     to non dimensioned variables of correct character length.
             *   - Decreased size of KHOLE from 24 to 8 characters.
             *   - Changed KUSER of length 24 char. to KUSER0, KUSER1, and KUSER2
             *     each of length 8 char. Moved them from KHDR(13) to KHDR(18).
             *   - Deleted KZDATE, KZTIME and KCMPNM.
             *   - Added 13 new alpha fields: KO, KA, KTn (n=0,9), and KF.
             *   - Current arrangement of KHDR is therefore:
             *           variable            old_loc   new_loc   new_len
             *           --------            -------   -------   -------
             *           KSTNM              1         1         1
             *           KEVNM              2         2         2
             *           KHOLE              4         4         1
             *           KO                NEW       5         1
             *           KA                NEW       6         1
             *           KTn               NEW       7-16      1 each
             *           KF                NEW       17        1
             *           KUSER0             13        18        1
             *           KUSER1             14        19        1
             *           KUSER2             15        20        1
             *           KHDR21             EMPTY     21        1
             *           KHDR22             EMPTY     22        1
             *           KHDR23             EMPTY     23        1
             *           KHDR24             EMPTY     24        1
             *   - Moved NSNPTS from NHDR(10) to NHDR(11).
             *   - Moved NPTS from NHDR(1) to NHDR(10).
             *   - Changed size of NZDTTM array from 4 to 6.
             *   - Moved NZDTTM and NZYEAR from NHDR(2) to NHDR(1).
             *   - Moved NZJDAY from NHDR(3) to NHDR(2).
             *   - Moved NZHOUR from NHDR(4) to NHDR(3).
             *   - Moved NZMIN from NHDR(5) to NHDR(4).
             *   - Converted ZSECS in FHDR(5) to NZSEC in NHDR(5) and
             *     NZMSEC in NHDR(6).
             *   - Moved FINI from FHDR(40) to FHDR(21). */

        } else if (s->h->nvhdr == 4) {
            if (strcmp(khdr(s, 12), "UNDEF   ") != 0) {
                strcpy(khdr18, khdr(s, 12));
            } else {
                strcpy(khdr18, SAC_CHAR_UNDEFINED);
            }
            for (jdx = 4; jdx < 24; jdx++) {
                strcpy(khdr(s, jdx), SAC_CHAR_UNDEFINED);
            }
            strcpy(khdr(s, 17), khdr18);
            VALUE(nhdr(s, 11)) = VALUE(nhdr(s, 10));
            VALUE(nhdr(s, 10)) = VALUE(nhdr(s, 1));
            VALUE(nhdr(s, 1)) = VALUE(nhdr(s, 2));
            VALUE(nhdr(s, 2)) = VALUE(nhdr(s, 3));
            VALUE(nhdr(s, 3)) = VALUE(nhdr(s, 4));
            VALUE(nhdr(s, 4)) = VALUE(nhdr(s, 5));
            VALUE(nhdr(s, 5)) = VALUE(xfhdr(s, 5));
            VALUE(nhdr(s, 6)) =
                (int) (1000. *
                       (VALUE(xfhdr(s, 5)) - (float) (VALUE(nhdr(s, 5)))) + 0.5);
            VALUE(xfhdr(s, 5)) = SAC_FLOAT_UNDEFINED;
            VALUE(xfhdr(s, 21)) = VALUE(xfhdr(s, 40));
            VALUE(xfhdr(s, 40)) = SAC_FLOAT_UNDEFINED;
            s->h->nvhdr = 5;

            /* - Header version 6:
             *   (1) Added LCALDA.  If .TRUE. distance/azimuth are always
             *       recomputed each time file is read.
             * */
        } else if (s->h->nvhdr == 5) {
            s->h->lcalda = TRUE;
            s->h->nvhdr = 6;

        }

        else {
            *nerr = ERROR_HEADER_FORMAT;
            setmsg("ERROR", *nerr);
            outmsg();
            break;
        }
    }

    return;
}
