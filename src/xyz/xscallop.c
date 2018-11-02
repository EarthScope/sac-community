
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mach.h"
#include "xyz.h"
#include "spe.h"
#include "gem.h"
#include "gdm.h"
#include "amf.h"
#include "bool.h"

#include "bot.h"
#include "ucf.h"
#include "dfm.h"
#include "msg.h"
#include "cpf.h"
#include "dff.h"
#include "array.h"


GDM_EXTERN
GEM_EXTERN
SPE_EXTERN

void /*FUNCTION*/
xscallop(nerr)
     int *nerr;
{
    int jdfl, *nptslist, numfiles, speclength, specsize, specwidth, nchar;
    int lprint = FALSE, ltry = FALSE;
    float begin, *deltalist, xmaximum, xminimum, ymaximum, yminimum;
    float *sdata, *scdata;
    static double window = 2.0;
    static double slice = 1.0;
    float ymin = VLARGE;
    float ymax = -1.0;
    static float fmin = 2.0;
    static float fmax = 6.0;
    static int lmean = TRUE;
    static char type[4] = "mem";
    static char imagetype[6] = "color";
    static int order = 200;
    static int morder = 100;
    static int lorder = FALSE;
    static int lbinary = FALSE;
    static int lcbar = TRUE;
    static int sfft = 1024;

    static int cnumber = 1;     /* number of correlation windows */
    static int lcnumber = FALSE;        /* was cnumber set by user? */

    static float cwinlength = 0.0;      /* length of correlation window (in seconds)
                                           should default to the window size of the image (WINDOW) */
    static int lcwinlength = FALSE;     /* was cwinlength set? if not default to value of WINDOW */

    static char cwintype[9] = "HAMMING ";       /* correlation window type */
    int cwindex;

    static char scale[11] = "STOCHASTIC";       /* type of scaling */

    sac *s;
    double tmp;
    float *spec;
        /*=====================================================================
	 * PURPOSE:  To execute the action command SCALLOP
	 *           This command computes a spectrogram of data in memory.
	 *           The spectrogram is held in memory.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *      NERR:  0 - no error, .ne. 0 - error
	 *=====================================================================
	 * MODULE/LEVEL:  IMAGING/2
	 *=====================================================================
	 * GLOBAL INPUT: 
	 *  inc/mem:      sacmem
	 *=====================================================================
	 * GLOBAL COUPLING: none
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    sac:  cfmt, cresp, vflist, vfeven, 
	 *          getnfiles, getfil, gethfv, spectrogram, flipdata,
	 *          setnfv, setihv, setfhv
	 *=====================================================================
	 * LOCAL VARIABLES: see below
	 *=====================================================================
	 * ASSUMPTIONS: none
	 *=====================================================================
	 * LIMITATIONS: Although this routine is set up to receive correlation
	 *              function as an input, only the 'pds' function is 
	 *              implemented.
	 *=====================================================================
	 * KNOWN ERRORS: none
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    900308:  Added coding to replace data in memory with spectrogram.
	 *             Retained WINDOW and SLICE values between executions.
	 *    900129:  Original version by Terri Quinn.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  900308
	 *===================================================================== */
    /* LOCAL VARIABLES */
    /* EXTERNALS:  */
    /* PROCEDURE: */
    *nerr = 0;
    begin = 0.0;
    nptslist = xarray_new_with_len('i', saclen());
    deltalist = xarray_new_with_len('f', saclen());
    memset(deltalist, 0, saclen() * sizeof(float));
    /* - Loop on each token in command: */
    while (lcmore(nerr)) {

        /* -- WINDOW v: define window size of image. */
        if (lkreal("WINDOW$", 8, &window)) {
        }

        else if (lklog("CBAR$", 6, &lcbar)) {   /* do nothing */
        }

        /* -- SLICE v:  define slice size for image. */
        else if (lkreal("SLICE$", 7, &slice)) {
        } else if (lkreal("YMAX#$", 7, &tmp)) {
            ymax = (float) tmp;
        } else if (lkreal("YMIN#$", 7, &tmp)) {
            ymin = (float) tmp;
        }

        /* -- CORRELATION v: correlation function */
        else if (lkchar("COR#RELATION$", 14, 3, type, 4, &nchar)) {     /* do nothing */
        }

        /* -- METHOD v: correlation function */
        else if (lkchar("M#ETHOD$", 9, 3, type, 4, &nchar)) {   /* do nothing */
        }

        else if (lkreal("FMIN$", 6, &tmp)) {
            fmin = (float) tmp;
        } else if (lkreal("FMAX$", 6, &tmp)) {
            fmax = (float) tmp;
        }

        else if (lclog2("MEAN$", 6, "MEDIAN$", 8, &lmean)) {    /* do nothing */
        }

        else if (lclog2("BINARY$", 6, "FULL$", 8, &lbinary)) {  /* do nothing */
        }

        /* -- ORDER v: order of correlation function */
        else if (lkirc("ORDER$", 7, 10, 400, &order)) {
            lorder = TRUE;      /* user specified order */
        }

        /* number of points in the spectral estimate */
        else if (lkirc("NUMBER$", 8, 512, 2048, &sfft)) {
            sfft = next2(sfft);
        }

        /* -- Color image    */
        else if (lckey("C#OLOR$", 8)) {
            strcpy(imagetype, "color");
        }

        /* -- Greyscale image */
        else if (lckey("G#REY$", 7)) {
            strcpy(imagetype, "grey");
        } else if (lckey("G#RAY$", 7)) {
            strcpy(imagetype, "grey");
        }

        else if (lkreal("CWL#ENGTH$", 11, &tmp)) {
            lcwinlength = TRUE;
            cwinlength = (float) tmp;
        }

        else if (lklogi("CWNUM#BER$", 11, &cnumber, &lcnumber)) {       /* do nothing */
        }

        else if (lklist
                 ("CWT#YPE&", 9, (char *) kmspe.kwintp, 9, MWINTP, &cwindex)) {
            strcpy(cwintype, (char *) kmspe.kwintp[cwindex]);
        }

        /* if prewhitening is requested prewhiten all the input traces and replace the
           data in memory with the prewhitened data first, then call the spectrogram
           routine with these.  Prewhitening will alter the number of points in the
           input, so have to be sure to get that right. */
/*              else if("PREW#HITEN&",12,&lprew,&nprew)
		{ * do nothing * }  */

        else if (lckey("STOCH#ASTIC$", 13)) {
            strcpy(scale, "STOCHASTIC");
        }

        else if (lckey("TRANS#IENT$", 12)) {
            strcpy(scale, "TRANSIENT");
        }

        /* if PRINT option is tried, get printer name */
        else if (ltry) {
            lcchar(kmgem.kptrName, sizeof(kmgem.kptrName));
            if (!lprint)
                kmgem.kptrName[0] = '\0';

            ltry = FALSE;
        }

        /* -- "PRINT":  print the final product */
        else if (lckey("PRINT#$", 8)) {
            ltry = TRUE;
            if (cmgdm.lbegf) {
                setmsg("WARNING", 2403);
                outmsg();
                clrmsg();
            } else if (Lgdon[3] || !Lgdon[2]) {
                setmsg("WARNING", 2404);
                outmsg();
                clrmsg();
            } else {
                lprint = TRUE;
            }
        }

        else {
            cfmt("ILLEGAL OPTION:", 17);
            cresp();
        }
    }

    /* - The above loop is over when one of two conditions has been met:
     *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
     *   (2) All the tokens in the command have been successfully parsed. */

    if (!lorder && (!strncmp(type, "MEM", 3) || !strncmp(type, "MLM", 3)))
        order = morder;

    if (*nerr != 0) {
        goto L_8888;
    } else {
        if ((window <= 0.0) || (slice <= 0.0)) {
            fprintf(stdout,
                    "Error: WINDOW and SLICE must be positive (xspectrogram).\n");
            goto L_8888;
        } else if (slice > window) {
            fprintf(stdout,
                    "Error: SLICE can not be greater than WINDOW(xspectrogram).\n");
            goto L_8888;
        } else {
            /* cwinlength defaults to size of window */
            if (!lcwinlength)
                cwinlength = window;

            /* - Get number of files in data file list. */
            getnfiles(&numfiles);

            /* CHECKING PHASE:
             * - Check for null data file list. */
            vflist(nerr);
            if (*nerr != 0) {
                goto L_8888;
            } else {
                /* - Check to make sure all files are evenly spaced time series files. */
                vfeven(nerr);
                if (*nerr != 0) {
                    goto L_8888;
                } else {
                    /* - Perform the requested function on each file in DFL. */
                    for (jdfl = 1; jdfl <= numfiles; jdfl++) {

                        if (!(s = sacget(jdfl - 1, TRUE, nerr))) {
                            goto L_8888;
                        }
                        /* -- Get the next file and their lengths in DFL, moving header to CMHDR. */
                        nptslist[jdfl - 1] = s->h->npts;
                        //getfil( jdfl, TRUE, &Nptslist[jdfl], &idum, &idum, nerr );

                        deltalist[jdfl - 1] = s->h->delta;
                        /* -- Get begin value if first file. */
                        if (jdfl == 1) {
                            begin = s->h->b;
                        }

                    }           /* end for */

                    /* -- Check if all files have same delta. */
                    for (jdfl = 1; jdfl <= numfiles; jdfl++) {
                        if (deltalist[0] != deltalist[jdfl - 1])
                            *nerr = 1;
                    }

                    /* EXECUTION PHASE: */
                    if (*nerr != 0) {
                        /* 'Sampling intervals of files not equal.' */
                        goto L_8888;
                    } else {
                        if (fmax > (1.0 / (2.0 * deltalist[0]))) {
                            /* fmax should be <= nyquist */
                            *nerr = 1;
                            printf("Error:  Fmax > Nyquist\n");
                            goto L_8888;
                        }

                        if (spectrogram
                            (window, slice, type, &order, numfiles, nptslist,
                             (double) deltalist[0], &spec, &specwidth,
                             &speclength, sfft, cwinlength, lcnumber, cnumber,
                             cwintype, scale) != 0) {
                            goto L_8888;
                        }
                    }
                }
            }
        }
    }

    /* - Do the spectrogram graphical output. */

    /* Flip the x and y storage */
    if ((sdata =
         (float *) malloc(specwidth * speclength * sizeof(float))) == NULL) {
        printf("error allocating sdata-xspectrogram\n");
        *nerr = 301;
        goto L_8888;
    }

    flipdata(spec, specwidth, speclength, sdata);
    FREE(spec);

    if ((scdata =
         (float *) malloc(specwidth * speclength * sizeof(float))) == NULL) {
        printf("error allocating scdata-xspectrogram\n");
        *nerr = 301;
        goto L_8888;
    }

    xminimum = begin + 0.5 * window;
    xmaximum = xminimum + (float) (speclength - 1) * slice;
    yminimum = 0.0;
    ymaximum = 0.5 / deltalist[0];

    scallop(sdata, specwidth, speclength, ymaximum, fmin, fmax, lmean, scdata,
            nerr);
    if (*nerr != 0)
        goto L_8888;

    /* nxsize = speclength, nysize = specwidth */
    if (ymax <= 0.0)
        ymax = ymaximum;
    if (ymin > 1.e30)
        ymin = yminimum;
    specplot(scdata, speclength, specwidth, xminimum, xmaximum, yminimum,
             ymaximum, ymin, ymax, imagetype, lbinary, lcbar, lprint, nerr);
    if (*nerr != 0)
        goto L_8888;

    /* - Replace data in memory with spectrogram. */

    /* -- Clear current data file list. */
    sacclear();

    /* -- Create space for a single data file. */

    specsize = specwidth * speclength;

    s = sac_new();
    s->m->filename = strdup("scallop");
    sacput(s);
    //getfil( 1, TRUE, &itemp1, &itemp2, &itemp3, nerr );

    /* Store the spectrogram data in sacmem */
    s->y = scdata;

    free(sdata);

    /* -- Store header values. */
    s->h->npts = specsize;
    s->h->delta = 1.0;
    s->h->b = 0.0;
    s->h->e = specsize - 1.0;

    s->h->iftype = IXYZ;
    s->h->nxsize = speclength;
    s->h->nysize = specwidth;
    s->h->xminimum = xminimum;
    s->h->xmaximum = xmaximum;
    s->h->yminimum = yminimum;
    s->h->ymaximum = ymaximum;

    extrma(s->y, 1, s->h->npts, &s->h->depmin, &s->h->depmax, &s->h->depmen);

  L_8888:

    return;

}                               /* end of function */
