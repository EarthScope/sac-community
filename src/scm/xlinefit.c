
#include <stdio.h>
#include <string.h>

#include "scm.h"
#include "hdr.h"
#include "dfm.h"
#include "amf.h"
#include "bool.h"

#include "msg.h"
#include "bbs.h"
#include "ucf.h"
#include "dff.h"

void /*FUNCTION*/
xlinefit(nerr)
     int *nerr;
{
    int jdfl;
    float corrcf, sddta, sdslp, sdyint, slp, yint;
    sac *s;
    char seqnum[5], vnslope[10], vnyint[9], vnsdslope[12], vnsdyint[11],
        vnsddata[11], vncorrcoef[13];

        /*=====================================================================
	 * PURPOSE:  To execute the action command LINEFIT.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *      NERR:  Error return flag.
	 *=====================================================================
	 * MODULE/LEVEL:  SCM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    DFM:     DEPMIN, DEPMAX, DEPMEN
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *    SACLIB:  VFLIST, VFTIME, GETFIL, LIFITE, LIFITU,
	 *             WRTXTT, EXTRMA, PUTFIL
	 *=====================================================================
	 * LOCAL VARIABLES:
	 *    SLP:     Slope of linear trend.
	 *    YINT:    Intercept of linear trend.
	 *===================================================================== */
    /* PROCEDURE: */
    *nerr = 0;

    strcpy(seqnum, "    ");

    /* CHECKING PHASE: */

    /* - Check for null data file list. */

    vflist(nerr);
    if (*nerr != 0)
        goto L_8888;

    /* - Check to make sure all files are time series files. */

    vftime(nerr);
    if (*nerr != 0)
        goto L_8888;

    /* EXECUTION PHASE: */

    /* - Perform the requested function on each file in DFL. */

    for (jdfl = 1; jdfl <= saclen(); jdfl++) {

        /* -- Get the next file in DFL, moving header to CMHDR. */
        if (!(s = sacget(jdfl - 1, TRUE, nerr))) {
            goto L_8888;
        }
        //getfil( jdfl, TRUE, &nlen, &ndx1, &ndx2, nerr );

        /* -- Two versions of linear fit: one for evenly spaced
         *    data and one for unevenly spaced data. */

        if (s->h->leven) {

            lifite(s->h->b, s->h->delta, s->y, s->h->npts, &slp, &yint, &sdslp,
                   &sdyint, &sddta, &corrcf);

        } else {
            lifitu(s->x, s->y, s->h->npts, &slp, &yint, &sdslp, &sdyint, &sddta,
                   &corrcf);

        }

        /* -- Write results oflinear fit. */

        setmsg("OUTPUT", 0);
            out("Slope and standard deviation are: %.5g %.5g\n"
                " Intercept and standard deviation are: %.5g %.5g\n"
                " Data standard deviation is: %.5g\n"
                " Data correlation coefficient is: %.5g",
                slp, sdslp, yint, sdyint, sddta, corrcf);
        outmsg();

        /* store results to blackboard */
        if (saclen() > 1) {
            sprintf(seqnum, "%4d", jdfl);
            ljust(seqnum, 5);
        }
        strcpy(vnslope, "slope");
        strcat(vnslope, seqnum);
        //sprintf(cslp,"%12.8f",slp);
        //setbbv(vnslope,cslp,nerr,strlen(vnslope),strlen(cslp));
        setbb(vnslope, VAR_VALUE, slp);
        //if(*nerr != 0) goto L_8888;

        strcpy(vnyint, "yint");
        strcat(vnyint, seqnum);
        //sprintf(cyint,"%12.8f",yint);
        //setbbv(vnyint,cyint,nerr,strlen(vnyint),strlen(cyint));
        setbb(vnyint, VAR_VALUE, yint);
        if (*nerr != 0)
            goto L_8888;

        strcpy(vnsdslope, "sdslope");
        strcat(vnsdslope, seqnum);
        //sprintf(csdslp,"%12.8f",sdslp);    
        //setbbv(vnsdslope,csdslp,nerr,strlen(vnsdslope),strlen(csdslp));
        setbb(vnsdslope, VAR_VALUE, sdslp);
        if (*nerr != 0)
            goto L_8888;

        strcpy(vnsdyint, "sdyint");
        strcat(vnsdyint, seqnum);
        //sprintf(csdyint,"%12.8f",sdyint);
        //setbbv(vnsdyint,csdyint,nerr,strlen(vnsdyint),strlen(csdyint));
        setbb(vnsdyint, VAR_VALUE, sdyint);
        if (*nerr != 0)
            goto L_8888;

        strcpy(vnsddata, "sddata");
        strcat(vnsddata, seqnum);
        //sprintf(csddta,"%12.8f",sddta);
        //setbbv(vnsddata,csddta,nerr,strlen(vnsddata),strlen(csddta));
        setbb(vnsddata, VAR_VALUE, sddta);
        if (*nerr != 0)
            goto L_8888;

        strcpy(vncorrcoef, "corrcoef");
        strcat(vncorrcoef, seqnum);
        //sprintf(ccorrcf,"%12.8f",corrcf);
        //setbbv(vncorrcoef,ccorrcf,nerr,strlen(vncorrcoef),strlen(ccorrcf));
        setbb(vncorrcoef, VAR_VALUE, corrcf);
        if (*nerr != 0)
            goto L_8888;

    }

  L_8888:
    return;

        /*=====================================================================
	 * MODIFICATION HISTORY:
	 *    950912:  Original version.
	 *===================================================================== */

}                               /* end of function */
