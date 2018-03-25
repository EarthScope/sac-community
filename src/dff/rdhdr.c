/** 
 * @file   rdhdr.c
 * 
 * @brief  Read header from SAC file
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "dff.h"
#include "hdr.h"
#include "msg.h"
#include "co.h"
#include "ucf.h"
#include "dfm.h"
#include "amf.h"

#include "errors.h"

#include "clf.h"

HDR_EXTERN
DFM_EXTERN

/** 
 * Read header from currently open SAC file into memory
 * 
 * @param idfl
 *    Data file list index number 
 * @param nun 
 *    Fortran file unit on which data file is open
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_NOT_A_SAC_FILE 
 *    - ERROR_HEADER_OUT_OF_DATE
 *    - ERROR_READING_FILE
 * 
 * @return 
 *
 * @date   870803:  Header update logic changed due to opening files 'RODATA'.
 * @date   860130:  Changed to new message handling package.
 * @date   850415:  Changes due to restructuring of DFM common block.
 * @date   821122:  Added recomputation of ENND every time file is read.
 * @date   821001:  Added check of LCALDA before calculating distance/azimuth.
 * @date   820909:  Mod due to placing KHDR in its own common block.
 * @date   820901:  Fixed bug that was causing NZYEAR to be clobbered.
 * @date   811026:  Documented subroutine.
 * @date   810924:  Included current century in NZYEAR if it was lt 100.
 * @date   810120:  Changed to output message retrieval from disk.
 *
 */
int
rdhdr(sac * s, int *nun, char *file, int *nerr) {

    int ncerr, nlcdsk, numrd;
    float *buffer;
    int *hdrVer, lswap = 0;
    const int versionLocation = 76;

    *nerr = 0;

    /* - Read header into memory. */
    numrd = SAC_HEADER_WORDS_FILE;

    nlcdsk = 0;

    if ((buffer = (float *) malloc(SAC_FIRST_DATA_POINT_WORD * 4)) == NULL) {
        *nerr = ERROR_READING_FILE;
        error(*nerr, "error allocating input buffer");
        goto L_8888;
    }

    /* read raw header data into buffer */
    zrabs((int *) nun, (char *) buffer, numrd, (int *) &nlcdsk, (int *) nerr);

    /* determine if the data needs to be swapped. */
    hdrVer = (int *) (buffer + versionLocation);
    if (*hdrVer < 1 || *hdrVer > cmhdr.nvhdrc) {
        byteswap((void *) hdrVer, 4);
        if (*hdrVer < 1 || *hdrVer > cmhdr.nvhdrc) {
            *nerr = ERROR_NOT_A_SAC_FILE;
            error(*nerr, "%s not in sac format, nor byteswapped sac format",
                  file);
            goto L_8888;
        } else {
            byteswap((void *) hdrVer, 4);       /* swap back, so it can be */
            lswap = 1;          /* swapped again with the rest. */
        }
    }

    /* move raw data into header location, byteswapping numeric headers
       if appropriate. */
    map_hdr_in((float *) s->h, buffer, lswap);

    free(buffer);

    /* - Update the header if it is in an old format:
     *   (1) Close file and open it for writing.
     *   (2) Write updated header.
     *   (3) Close file again and open it for reading only.
     *   (4) Send a warning message to inform user of all of this. */

    if (s->h->nvhdr > 0 && s->h->nvhdr < cmhdr.nvhdrc) {
        updhdr(nerr);
        if (*nerr == 0)

            if (*nerr != 0) {
                *nerr = ERROR_HEADER_OUT_OF_DATE;
                error(*nerr, "%s File is Bad. Header could not be updated",
                      file);
                goto L_8888;
            }
        warning(ERROR_HEADER_OUT_OF_DATE, "%s", file);
        zclose(nun, &ncerr);

        zopen_sac(nun, file, strlen(file) + 1, "DATA", 5, nerr);

        if (*nerr != 0) {
            *nerr = 0;
            mprint("Insufficient access rights to update disk file.\n"
                   "Header in memory has been updated.");
            goto L_4000;
        }
        nlcdsk = 0;

        if ((buffer = (float *) malloc(SAC_FIRST_DATA_POINT_WORD * 4)) == NULL) {
            *nerr = 115;
            error(*nerr, "allocating output buffer");
            goto L_8888;
        }

        map_hdr_out((float *) s->h, buffer, lswap);

        zwabs((int *) nun, (char *) buffer, SAC_FIRST_DATA_POINT_WORD,
              (int *) &nlcdsk, (int *) nerr);

        free(buffer);

        if (*nerr != 0) {
            *nerr = 0;
            mprint("Could not update header in disk file.\n"
                   "Header in memory has been updated.");
            zclose(nun, &ncerr);
            goto L_4000;
        }
        zclose(nun, &ncerr);
        mprint("Headers in memory and on disk have been updated.");
      L_4000:
        outmsg();
        clrmsg();

        zopen_sac(nun, file, strlen(file) + 1, "RODATA", 7, &ncerr);

    } /* end if( *nvhdr > 0 && *nvhdr < cmhdr.nvhdrc ) */
    else if (s->h->nvhdr <= 0 || s->h->nvhdr > cmhdr.nvhdrc) {
        *nerr = ERROR_HEADER_OUT_OF_DATE;
        error(*nerr, "%s Header version number is incorrect", file);
        goto L_8888;
    }

    /* - Compute distance, azimuth, etc. if proper header fields are present. */
    update_distaz(s);

    /* - Adjust reference year if necessary. */

    if (s->h->nzyear >= 0 && s->h->nzyear <= 99)
        s->h->nzyear = s->h->nzyear + 1900;

    /* - Compute end time */

    sac_be(s);

  L_8888:
    return lswap;
}
