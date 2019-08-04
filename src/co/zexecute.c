/** 
 * @file   zexecute.c
 * 
 * @brief  Execute a "loaded" object
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "mach.h"
#include "amf.h"
#include "co.h"
#include "cpf.h"
#include "bool.h"
#include "dfm.h"
#include "hdr.h"
#include "dff.h"

#include "dload.h"
#include "extfunc.h"

#include "errors.h"


#include "ssi.h"

DFM_EXTERN
EXTCOM_EXTERN

/** 
 * Execute a dynamically loaded external command
 * 
 * @param index 
 *    Index (reference) used to access the command
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_OUT_OF_MEMORY 
 *    
 * @date   900804:  Original version.
 *
 * @bug Only called by top/executecommand()
 *
 */
void
zexecute(int index, int *nerr) {
    int i, jdfl, ireturn;
    sac_files call_data;
    sac_header **call_headers, *this_header;

    char buf[1001];
    char **ext_argv;
    int ext_argc;

    float **ydata, **xdata;

    int update = IGNORE;
    int dummy_init = FALSE;

    sac *s;

    *nerr = 0;

    call_data.nfiles = 0;
    call_data.ext_hdrs = NULL;
    call_data.ext_yvalues = NULL;
    call_data.ext_xvalues = NULL;

    if (index <= cmextcom.nfiles) {

        /* This routine should never be called. This is just here 
         * to cause the external function header access routines 
         * to be loaded */
        if (dummy_init) {
            ext_init();
        }

        /* allocate space for the headers. */
        if ((call_headers =
             malloc(saclen() * sizeof(sac_hdr *))) == NULL) {
            *nerr = ERROR_OUT_OF_MEMORY;
            return;
        }

        /* allocate pointer arrays for data */
        if ((ydata = malloc(saclen() * sizeof(float *))) == NULL) {
            free(call_headers);
            *nerr = ERROR_OUT_OF_MEMORY;
            return;
        }

        if ((xdata = malloc(saclen() * sizeof(float *))) == NULL) {
            *nerr = ERROR_OUT_OF_MEMORY;
            free(call_headers);
            free(ydata);
            return;
        }

        for (i = 0; i < saclen(); i++) {
            call_headers[i] = NULL;
            ydata[i] = NULL;
            xdata[i] = NULL;
        }

        for (jdfl = 1; jdfl <= saclen(); jdfl++) {
            if (!(s = sacget(jdfl - 1, TRUE, nerr))) {
                goto L_8888;
            }

            if ((this_header = malloc(sizeof(sac_hdr))) == NULL) {
                *nerr = ERROR_OUT_OF_MEMORY;
                goto L_8888;
            }

            call_headers[jdfl - 1] = this_header;
            memcpy(this_header, s->h, sizeof(sac_hdr));

            /* allocate memory for the ydata. */
            if ((ydata[jdfl - 1] = malloc(s->h->npts * sizeof(float))) == NULL) {
                *nerr = ERROR_OUT_OF_MEMORY;
                goto L_8888;
            }

            /* copy ydata to call data yarray. */
            memcpy(ydata[jdfl - 1], s->y, (s->h->npts * sizeof(float)));

            /* if there is another data component (xdata), allocate memory */
            /* for it and copy it in.                                      */
            if (s->x) {         /* There is xdata */
                if ((xdata[jdfl - 1] =
                     malloc(s->h->npts * sizeof(float))) == NULL) {
                    *nerr = ERROR_OUT_OF_MEMORY;
                    goto L_8888;
                }
                memcpy(xdata[jdfl - 1], s->x, (s->h->npts * sizeof(float)));
            } else {
                /* if there is no other data component, set the pointer to NULL. */
                xdata[jdfl - 1] = NULL;
            }

        }

        call_data.nfiles = saclen();
        call_data.ext_yvalues = ydata;
        call_data.ext_xvalues = xdata;
        call_data.ext_hdrs = call_headers;

        //strcpy(buf, kmcpf.kinputline);
        tokenize(&ext_argv, &ext_argc, buf, nerr);

        /* Call external routine. */
        if ((ireturn =
             (*(cmextcom.extfuncs[index - 1])) (ext_argc, ext_argv, &call_data,
                                                &update)) != 0) {
            *nerr = ireturn;
            goto L_8888;
        }

        /* Replace, append or ignore the return data, according to update flag. */
        if (update != IGNORE) {
            updatedfl(call_data, update, nerr);

            /* If the user can write his/her own routine, we will trust him/her
               to take care of evids. */
            if (cmdfm.ltrust)
                cmdfm.nreadflag = HIGH;
            else
                cmdfm.nreadflag = LOW;

            cmdfm.nfilesFirst = 0;
            cmdfm.lread = TRUE;
            sacToSeisMgr(update == REPLACE, 0, 1, nerr);
            cmdfm.lread = FALSE;
        }

    } else {
        fprintf(stdout, "%s%2d\n", "ZEXECUTE: Error ", index);
    }

  L_8888:

    /* free memory for call data. */
    call_headers = call_data.ext_hdrs;
    ydata = call_data.ext_yvalues;
    xdata = call_data.ext_xvalues;

    for (i = 0; i < call_data.nfiles; i++) {
        if (ydata[i] != NULL)
            free(ydata[i]);
        /* if not evenly spaced file release x storage. */
        if (!call_headers[i]->ext_lhdr[0])
            free(xdata[i]);
        if (call_headers[i] != NULL)
            free(call_headers[i]);
    }

    free(call_headers);
    free(ydata);
    free(xdata);

    return;

}
