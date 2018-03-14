
#include <stdio.h>

#include "sam.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"

#include "msg.h"
#include "icm.h"
#include "ucf.h"
#include "cpf.h"
#include "dff.h"

SAM_EXTERN

void /*FUNCTION*/
xdft(nerr)
     int *nerr;
{
    int jdx, jdfl, jj, nfreq, nlnnew, npts_orig;

    float scalef;
    sac *s;

    double *re, *im;
    float *new, *new2;
        /*=====================================================================
	 * PURPOSE:  To execute the action command DFT.
	 *           This command takes the discrete Fourier transform of data.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *             Potential error numbers:
	 *=====================================================================
	 * MODULE/LEVEL:  SAM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    HDR:     DEPMEN, IRLIM, IAMPH
	 *    SAM:     MFFT
	 *    DFM:     data set storage stuff.
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    DFM:     NDXDTA, NLNDTA, NCOMP
	 *    HDR:     NSNPTS, NPTS, IFTYPE, SB, SDELTA, B, DELTA, E
	 *    SAM:     LWMEAN, LRLIM
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  LCMORE, LCLOG2, CFMT, CRESP, VFLIST, VFEVEN, VFMAXN,
	 *             GETFIL, ALLAMB, COPY, FILL, RELAMB, CPFT, TOAMPH, PUTFIL,
	 *             SETMSG, APFMSG, OUTMSG, NEXT2
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    NPTSMX:  Maximum number of points in files in DFL. [i]
	 *    NLNOLD:  Number of points in current file before transform.. [i]
	 *    NDXOLD:  Index in SACMEM array of time-series before transform. [i]
	 *    NLNNEW:  Number of points in current file after transform. [i]
	 *    NDX1:    Index in SACMEM of first component after transform. [i]
	 *    NDX2:    Index in SACMEM of second component after transform. [i]
	 *    NFREQ:   Number of frequencies in transformed data. [i]
	 *    SCALEF:  Scaling factor applied after transform. [f]
	 *    NDSFILES: Number of files in a given data-set. [i]
	 *    NDSFLNUM: File number (senquental) in the current data set. [i]
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    920422:  Added msg about default change in version 10.6e.
	 *    911015:  Added data-set storage, two sections of code.
	 *    850614:  Major rewrite due to addition of new memory manager.
	 *    821122:  Changed maximum DFT to 65536.
	 *             Deleted double precision DFT option.
	 *    820621:  Changed to newest set of parsing and checking functions.
	 *    810414:  Minor changes relating to new CMSAM.
	 *    810120:  Changed to output message retrieval from disk.
	 *    800202:  Original version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  850617
	 *===================================================================== */
    /* PROCEDURE: */
    *nerr = 0;

    /* PARSING PHASE: */

    /* - Loop on each token in command: */

    while (lcmore(nerr)) {

        /* -- WMEAN/WOMEAN: mean is retained/discarded after transform. */
        if (lclog2("WMEAN$", 7, "WOMEAN$", 8, &cmsam.lwmean)) { /* do nothing */
        }

        /* -- RLIM/AMPH: output will be in real-imaginary/amplitude-phase format. */
        else if (lclog2("RLIM$", 6, "AMPH$", 6, &cmsam.lrlim)) {        /* do nothing */
        }

        /* -- Bad syntax. */
        else {
            cfmt("ILLEGAL OPTION:", 17);
            cresp();
        }

    }                           /* end while */

    /* - The above loop is over when one of two conditions has been met:
     *   (1) An error in parsing has occurred.  In this case NERR is > 0 .
     *   (2) All the tokens in the command have been successfully parsed. */

    if (*nerr != 0)
        goto L_8888;

    /* CHECKING PHASE: */

    /* - Check for null data file list. */

    vflist(nerr);
    if (*nerr != 0)
        goto L_8888;

    /* - Check to make sure all files are evenly spaced time series files. */

    vfeven(nerr);
    if (*nerr != 0)
        goto L_8888;

    /* EXECUTION PHASE: */

    /* - Added temporary msg that fft default changed in 10.6e 
       if( cmsam.lwmean ){
       fprintf( MUNOUT, "(10.6e)FFT default change: not removing the mean\n" );
       }
     */

    /* - For each file in (active working-storage) data file list: */

    for (jdfl = 1; jdfl <= saclen(); jdfl++) {
        if (!(s = sacget(jdfl - 1, TRUE, nerr))) {
            goto L_8888;
        }
        //getfil( jdfl, TRUE, &nlnold, &ndx1, &ntused, nerr );

        /* -- Compute length of data after transform. */
        nlnnew = next2(s->h->npts);
        npts_orig = s->h->npts;
        /* -- If transformed length is greater than current length. */
        if (nlnnew > s->h->npts) {

            /* --- Allocate memory block for first component. */
            //ndxold = ndx1;
            new = (float *) malloc(sizeof(float) * nlnnew);
            /* --- Update DFM entries. */
            //Nlndta[jdfl] = nlnnew;
            //cmdfm.ndxdta[jdfl_][0] = ndx1;
            //s->y = new;
            /* --- Copy time-series data into first block and zero fill. */

            copy_float(s->y, new, s->h->npts);
            fill(new + s->h->npts, nlnnew - s->h->npts, 0.);
            npts_orig = s->h->npts;
            s->h->npts = nlnnew;
            FREE(s->y);
            s->y = new;
        }

        /* end if( nlnnew > nlnold ) */
        /* -- Allocate memory block for second component and zero fill. */
        new2 = (float *) malloc(sizeof(float) * nlnnew);
        s->x = new2;
        //cmdfm.ndxdta[jdfl_][1] = ndx2;
        fill(s->x, nlnnew, 0.);

        /* -- Remove the mean before transform if requested. */
        if (!cmsam.lwmean) {
            for (jdx = 0; jdx < npts_orig; jdx++) {
                s->y[jdx] -= s->h->depmen;
            }
        }

        /* end if( !cmsam.lwmean ) */
        /* allocate memory to store data for call to double precision fft routine */
        re = (double *) malloc(sizeof(double) * nlnnew);
        im = (double *) malloc(sizeof(double) * nlnnew);
        if (!re || !im) {
            printf("error allocating memory-xdft\n");
            goto L_8888;
        }

        DEBUG("re,im: %p %p\n", re, im);
        for (jdx = 0; jdx < nlnnew; jdx++) {
            re[jdx] = (double) s->y[jdx];
            im[jdx] = (double) s->x[jdx];
        }
        DEBUG("re,im: %p %p\n", re, im);

        /* -- Perform FFT. */
        dcpft(re, im, nlnnew, 1, cmsam.ifwd);

        DEBUG("re,im: %p %p\n", re, im);
        DEBUG("re,im: %p %p\n", re, im);
        for (jdx = 0; jdx < nlnnew; jdx++) {
            s->y[jdx] = (float) re[jdx];
            s->x[jdx] = (float) im[jdx];
        }
        DEBUG("re,im: %p %p\n", re, im);
        FREE(re);
        FREE(im);

        nfreq = nlnnew / 2;
        scalef = s->h->delta;
        s->y[0] *= scalef;
        s->y[nfreq] *= scalef;
        for (jdx = 1; jdx <= (nfreq - 1); jdx++) {
            s->y[jdx] *= scalef;
            s->x[jdx] *= scalef;
            jj = nlnnew - jdx;
            s->y[jj] = s->y[jdx];
            s->x[jj] = -s->x[jdx];
        }
        setmsg("OUTPUT", 1607);
        apfmsg(s->y[0]);
        outmsg();

        /* -- Adjust header to reflect new status. */
        s->h->nsnpts = npts_orig;
        s->h->npts = nlnnew;
        s->h->iftype = IRLIM;
        s->h->sb = s->h->b;
        s->h->sdelta = s->h->delta;
        s->h->b = 0.;
        s->h->delta = 1. / (s->h->delta * (float) (s->h->npts));
        s->h->e = s->h->b + (float) (nfreq) * s->h->delta;
        if (!cmsam.lrlim) {
            int i;
            DEBUG("Real/Imag => Amp/Phase %d\n", s->h->iftype);
            toamph(s->y, s->x, s->h->npts, s->y, s->x);
            for (i = 0; i < s->h->npts; i++) {
                DEBUG("%d %f/%f\n", i, s->y[i], s->x[i]);
            }
            s->h->iftype = IAMPH;
            DEBUG("Real/Imag => Amp/Phase %d\n", s->h->iftype);
        }

    }                           /* end for ( jdfl ) */

  L_8888:
    return;

}                               /* end of function */
