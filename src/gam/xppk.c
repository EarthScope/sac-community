/** 
 * @file   xppk.c
 * 
 * @brief  
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <fern/array.h>

#include "dfm.h"
#include "hdr.h"
#include "amf.h"
#include "gem.h"
#include "gdm.h"
#include "eam.h"
#include "gam.h"
#include "bool.h"

#include "gtm.h"
#include "pl.h"
#include "bot.h"
#include "ucf.h"
#include "msg.h"
#include "bbs.h"
#include "cpf.h"
#include "co.h"
#include "dff.h"
#include "debug.h"

#define ACK_CHAR 6

GAM_EXTERN
GEM_EXTERN
EAM_EXTERN

#define	MWIN	5

int bellON = TRUE;

void
xppk(int *nerr) {

    char _c0[2], kmsg[MCMSG + 1], kptext[MCMSG + 1], kundrt[9],
        kxloc[17], kyloc[17];
    int lany, lempty, *lhlwrt, ltitls, lwfok, *lzdttm;
    int xlabelsave, ylabelsave;
    char kchar;
    int iwf[5], jdx, jdfl, jdfl1, jdfl2, jdfls, jfr, jhdr1, jhdr2, jhour, jjday,
        jmark, jmark1, jmark2, jmin, jmsec, jofset, jsec, jwin, jyear, ncerr,
        ndxpk, nexday, nfr, nlncda, nperfr, npmark, npmsec, npsec,
        nsavelast = 0, nst, unused;
    float amplmn, amplmx, facc, fsecsi, prl, psecsi, seccur, secinc = 0.0, ssecsi,
        time, tminew = 0.0, tref1, twin[MWIN][2],
        xloc, xloc1, xloc2, xtpos, *yimnzs, *yimxzs, yloc,
        ypdel, ypdelv, ypmns, ypmnv, ypmxs, ypmxus, ypmxv, ytpos;
    double tmp, tmin, tmax;
    double *toff = NULL;
    double xlocs1, xlocs2;
    sac *s;
    int j;
    static char kndate[25] = "                        ";
    static char kntime[17] = "                ";
    static int lint = FALSE;
    static int lnewxw = FALSE;
    static char kdir = 'U';
    static char ktype = 'I';
    static char kqual = '0';
    static int lhltrm = FALSE;
    static int lhlhyp = FALSE;
    static int nsavelocs = 0;
    static int lppkab = TRUE;

    int *const Iwf = &iwf[0] - 1;
    int bellJUNK;

    s = NULL;
        /*=====================================================================
	 * PURPOSE:  To parse and execute the action command PPK.
	 *           This command plots data for purposes of picking
	 *           arrival times, coda, etc. using the cursor.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *=====================================================================
	 * MODULE/LEVEL:  GAM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:  
	 *    GAM:     KGDDEF
	 *    GEM:     LWIDTH, IWIDTH, ITHIN
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    GAM:     LRTWXL, KRTWXL, ORTWXL
	 *=====================================================================
	 * GLOBAL COUPLING:
	 *=====================================================================
	 * GLOBAL SYSTEM INPUT:
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *=====================================================================
	 * ASSUMPTIONS:
	 *=====================================================================
	 * LIMITATIONS:
	 *=====================================================================
	 * KNOWN ERRORS:
	 *=====================================================================
	 * MODIFICATION HISTORY:
	 *    970924:  Changed the way lppkab is handled.  maf
	 *    970908:  Changed response to ddttm().  maf
         *    970130:  Added arguments to dispid() to plot file number. maf
         *    970129:  Add parameter (0) to cnvati.  0 means that if a string
         *             of digits is too long, let it slide by.  maf 
	 *    920602:  Added line-width kludge.
	 *    910608:  Added call to zgetgd when no graphics device specified.
	 *             Changed call to begindevice to begindevices. (wct)
	 *    890710:  Added saving of location output to blackboard.
	 *    870306:  Added MARKALL option and fixed several small bugs.
	 *    850528:  Deleted 'UNDEFINED REFERENCE TIME' in file id.
	 *    841218:  Changed quality and kill cursor responses.
	 *    830929:  Improved calculation of time offsets.
	 *             Added ability to go back more than one time window.
	 *             Added ability to go back one frame.
	 *    830114:  Changed calls to MOVE to PLHOME.
	 *    821122:  Added check for bad date fields.
	 *    820810:  Changed names for HPF and APF variables.
	 *    820721:  Changed to newest set of parsing and checking functions.
	 *    820309:  Fixed bug in title positioning.
	 *             Added conversion of returned character to upper case.
	 *    820120:  Fixed bug in processing cursor response "K".
	 *    811228:  Deleted call to ZCLIP.
	 *    811028:  Added call to ENDFRAME(NERR) when a new plot is requested.
	 *             Added calls to MOVE before each ENDFRAME(NERR) to home cursor.
	 *    811003:  Added calls to ZPASOF/ZPASON to turn off/on passive dev.
	 *    810521:  Added option to save new plot window as x axis limits.
	 *    810414:  Minor changes relating to common block reorganization.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:
	 *===================================================================== */
    /* PROCEDURE: */

    lempty = TRUE;
    *nerr = 0;

    lhlwrt = xarray_new_with_len('i', saclen() + 1);
    lzdttm = xarray_new_with_len('i', saclen() + 1);
    toff = xarray_new_with_len('d', saclen() + 1);
    yimnzs = xarray_new_with_len('f', saclen() + 1);
    yimxzs = xarray_new_with_len('f', saclen() + 1);


    xlabelsave = cmgem.xlabel.on;
    ylabelsave = cmgem.ylabel.on;

    /* PARSING PHASE: */

    /* - Loop on each token in command: */
    while (lcmore(nerr)) {

        /* -- "PERPLOT ON/OFF/n":  changes the number of plots on each frame. */
        if (lklogi("PERPLOT$", 9, &cmgam.lppkpp, &cmgam.nppkpp)) {      /* do nothing */
        } else if (lklogi("PP$", 4, &cmgam.lppkpp, &cmgam.nppkpp)) {    /* do nothing */
        }

        /* -- "BELL ON/OFF/n":  Turns the bell on and off for X11. */
        else if (lklogi("BELL$", 6, &bellON, &bellJUNK)) {      /* do nothing */
        }

        /* -- "ABSOLUTE/RELATIVE":  changes the way time is plotted. */
        else if (lclog2("ABSOLUTE$", 10, "RELATIVE$", 10, &lppkab)) {   /* do nothing */
        }

        /* -- "GMT/ZERO":  changes the way time is displayed. */
        else if (lclog2("GMT$", 5, "ZERO$", 6, &cmgam.lppkut)) {        /* do nothing */
        }

        /* -- "REFERENCE ON/OFF/v":  changes the reference line plotting option. */
        else if (lklogr("REF#ERENCE$", 12, &cmgam.lppkrl, &tmp)) {
            cmgam.vppkrl = (float) tmp;
        }

        /* -- "MARKALL ON|OFF":  marks all of files in current plot or only one. */
        else if (lklog("MARKALL$", 9, &cmgam.lmkall)) { /* do nothing */
        }

        /* -- "SAVELOCS ON|OFF": save picked locations in blackboard or not. */
        else if (lklog("SAVELOCS$", 10, &cmgam.lsavelocs)) {    /* do nothing */
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

    /* - Check to make sure all files are time series files. */

    vftime(nerr);
    if (*nerr != 0)
        goto L_8888;

    /* - If no graphics device is open, try to open the default device. */
    getstatus("ANY", &lany);
    if (!lany) {
        zgetgd(kmgam.kgddef, 9);
        begindevices(kmgam.kgddef, 9, 1, nerr);
        if (*nerr != 0) {
            goto L_8888;
        }
    }

    /* EXECUTION PHASE: */

    /* - Save plot environment. */

    plsave();

    /* - Temporarily turn on cursor graphics device only. */

    cursoron();
    /* - Set up specific options for this plot. */

    cmgem.axis[LEFT].annotate = TRUE;
    cmgem.axis[RIGHT].annotate = FALSE;
    cmgem.axis[TOP].annotate = FALSE;
    cmgem.axis[BOTTOM].annotate = FALSE;
    cmgem.axis[LEFT].ticks = TRUE;
    cmgem.axis[RIGHT].ticks = TRUE;
    cmgem.axis[TOP].ticks = TRUE;
    cmgem.axis[BOTTOM].ticks = FALSE;
    cmgem.lframe = FALSE;
    ltitls = cmgem.title.on;
    cmgem.title.on = FALSE;
    cmgem.plot.ymax = 0.80;
    cmgem.plot.ymin = 0.10;
    cmgem.xdiv_number_on = TRUE;
    cmgem.xdiv_number = 7;
    cmgem.ydiv_number_on = TRUE;
    cmgem.ydiv_number = 5;
    cmgem.lxfudg = FALSE;
    cmgem.xlabel.on = FALSE;
    cmgem.ylabel.on = FALSE;
    kchar = 'U';

    psecsi = 0;
    ssecsi = 0;
    fsecsi = 0;

    /* - Set up y window for each subplot. */

    if (cmgam.lppkpp) {
        nfr = (saclen() - 1) / cmgam.nppkpp + 1;
        nperfr = cmgam.nppkpp;
    } else {
        nfr = 1;
        nperfr = saclen();
    }
    ypdel = (cmgem.plot.ymax - cmgem.plot.ymin) / (float) (nperfr);


    /* - Initialize number of saved locations to blackboard. */

    nsavelast = nsavelocs;
    nsavelocs = 0;

    /* - Loop on number of frames. */

    ypmns = cmgem.plot.ymin;
    ypmxs = cmgem.plot.ymax;
    ypmxus = cmgem.uplot.ymax;
    jfr = 1;

  L_1900:
    if (jfr > nfr)
        goto L_7777;

    jdfl1 = 1 + (jfr - 1) * nperfr;
    jdfl2 = min(saclen(), jdfl1 + nperfr - 1);

    /* - Calculate offsets used to align files in time if in absolute mode.
     *   Calculate maximum duration of files if in relative mode. */

    /* -- Determine time limits for x axis of this frame.
     *    (Correct for any differences in GMT reference time.) */

    calc_time_offsets(!lppkab, toff, jdfl1, jdfl2, &tmin, &tmax);

    jwin = 1;
    twin[jwin - 1][0] = tmin;
    twin[jwin - 1][1] = tmax;

    /* - Check range of time limits to avoid errors that could occur
     *   later during plotting. */

    if (fabs(tmax - tmin) > (float) (MLARGE)) {
        *nerr = 1504;
        setmsg("ERROR", *nerr);
        goto L_8888;
    }


    /* -- Begin new frame and set up some parameters. */
  L_2000:
    DEBUG("time: %f %f\n", tmin, tmax);

    cmgem.lframe = FALSE;
    beginframe(FALSE, nerr);

    if (*nerr != 0)
        goto L_8888;
    getvspace(&cmgem.view.xmin, &cmgem.view.xmax, &cmgem.view.ymin,
              &cmgem.view.ymax);
    ypmxv = ypmxs * cmgem.view.ymax;
    ypmnv = ypmns * cmgem.view.ymax;
    ypdelv = ypdel * cmgem.view.ymax;
    cmgem.chht = cmgem.tsdef;
    cmgem.chwid = cmgem.txrat * cmgem.chht;
    settextsize(cmgem.chwid, cmgem.chht);
    xtpos = cmgem.plot.xmin;
    ytpos = cmgem.view.ymax - 1.1 * cmgem.chht;
    cmgem.axis[TOP].ticks = TRUE;
    cmgem.lxlim = TRUE;
    cmgem.ximn = tmin;
    cmgem.ximx = tmax;
    cmgem.axis[BOTTOM].ticks = FALSE;
    cmgem.axis[BOTTOM].annotate = FALSE;
    cmgem.plot.ymax = ypmxs;

    /* These lines allow dispid to adjust the text size.  maf 970130 */
    cmgem.tsdef =
        fmin(cmgem.tsdef,
             (cmgem.view.ymax - cmgem.view.ymin) / (8.0 * (float) (nperfr)));
    cmgam.tsfid = cmgem.tsdef;
    cmgam.tspk = cmgem.tsdef;
    cmgem.tsaxis = cmgem.tsdef;

    j = 0;
    /* -- Loop on each file in this frame. */
    for (jdfl = jdfl1; jdfl <= jdfl2; jdfl++) {
        j += 1;
        if (jdfl == jdfl2)
            cmgem.axis[BOTTOM].ticks = TRUE;
        cmgem.plot.ymin = cmgem.plot.ymax - ypdel;
        if (!(s = sacget(jdfl - 1, TRUE, nerr))) {
            goto L_7777;
        }
        //getfil( jdfl, TRUE, &nlen, &nlcy, &nlcx, nerr );
        /* --- Plot this file. */

        getylm(&cmgem.lylim, &cmgem.yimn, &cmgem.yimx);
        if (s->h->leven) {
            cmgem.xgen.on = TRUE;
            cmgem.xgen.first = B(s) + toff[j];
            cmgem.xgen.delta = DT(s);
        } else {
            cmgem.xgen.on = FALSE;
        }
        pl2d(s->x, s->y, s->h->npts, 1, 1, nerr);
        if (*nerr != 0)
            goto L_7777;
        dispid(cmgam.lfinorq, jdfl, 0, NULL);
        DEBUG("%d toff: %f\n", j, toff[j]);
        lzdttm[jdfl] = ldttm(&s->h->nzyear);
        disppk(toff[j]);
        yimnzs[jdfl] = cmgem.zdata.ymin;
        yimxzs[jdfl] = cmgem.zdata.ymax;
        cmeam.lpphas = (cmeam.lhpfop && A(s) != SAC_FLOAT_UNDEFINED) &&
            s->h->ka[0] == 'P';
        cmeam.lpphas = cmeam.lpphas && lzdttm[jdfl];
        cmeam.lsphas = T0(s) != SAC_FLOAT_UNDEFINED && s->h->kt0[0] == 'S';
        cmeam.lfini = F(s) != SAC_FLOAT_UNDEFINED;
        if (cmeam.lpphas) {
            psecsi = A(s);
            fstrncpy(kmeam.kpwave, 8, s->h->ka, 4);
            if (cmeam.lsphas) {
                ssecsi = T0(s);
                fstrncpy(kmeam.kswave, 8, s->h->kt0, 4);
            }
            if (cmeam.lfini)
                fsecsi = F(s);
            inctim(s->h->nzhour, s->h->nzmin, s->h->nzsec, s->h->nzmsec, psecsi,
                   &cmeam.nphour, &cmeam.npmin, &npsec, &npmsec, &nexday);
            cmeam.psecs = tosecs(npsec, npmsec);
            incdat(s->h->nzyear, s->h->nzjday, nexday, &cmeam.npyear,
                   &cmeam.npjday);
            kidate(cmeam.npyear, cmeam.npjday, &cmeam.npmon, &cmeam.npday,
                   &ncerr);
            strcpy(kmeam.kstid, s->h->kstnm);
            if (cmeam.lsphas)
                cmeam.ssecs = cmeam.psecs - psecsi + ssecsi;
            if (cmeam.lfini)
                cmeam.fmp = fsecsi - psecsi;
            settextangle(TEXT_HORIZONTAL);
            whpf1(kmsg, MCMSG + 1);
            if (lhlwrt[jdfl]) {
                pltext("*", xtpos - cmgem.chwid, ytpos);
            }
            rstrip(kmsg);
            pltext(kmsg, xtpos, ytpos);
            ytpos = ytpos - cmgem.chht;
            lhltrm = FALSE;
        }
        cmgem.plot.ymax = cmgem.plot.ymin;
    }                           /* end for( jdfl = jdfl1; jdfl <= jdfl2; jdfl++ ) */
    jdfls = 0;
    jdfl = 0;

    /* -- If MARKALL option is on, set header and plot marker limits. */

    if (cmgam.lmkall) {
        jhdr1 = jdfl1;
        jhdr2 = jdfl2;
        jmark1 = 1;
        jmark2 = jdfl2 - jdfl1 + 1;
    } else {
        jhdr1 = jdfl;
        jhdr2 = jdfl;
        jmark1 = jdfl - jdfl1 + 1;
        jmark2 = jmark1;
    }
    npmark = 0;

    /* -- Put time axes at bottom of plot. */

    cmgem.chht = cmgem.tsaxis;
    cmgem.chwid = cmgem.txrat * cmgem.chht;
    settextsize(cmgem.chwid, cmgem.chht);
    cmgem.axis[TOP].ticks = FALSE;
    cmgem.axis[BOTTOM].annotate = TRUE;
    if (cmgem.ixint == AXIS_LINEAR)
        xlinax();
    if (ltitls) {
        /*  This puts the title at the bottom of the plot */
        cmgem.uplot.ymax = ypmxus;
        centxt(kmgem.ktitl, 145, cmgem.title.len, cmgem.title.pos, cmgem.title.text_size);
    }
    settextjust(LEFT, BOTTOM);

    if (xlabelsave) {
        centxt(kmgem.kxlab, 145, cmgem.xlabel.len, cmgem.xlabel.pos, cmgem.xlabel.text_size);
    }
    if (ylabelsave) {
        centxt(kmgem.kylab, 145, cmgem.ylabel.len, cmgem.ylabel.pos, cmgem.ylabel.text_size);
    }

    /* -- Perform graphics input function. */

    xloc = cmgem.plot.xmin + 0.05 * (cmgem.plot.xmax - cmgem.plot.xmin);
    yloc = ypmxv - 0.5 * ypdelv;
    cmgem.chht = cmgem.tsdef;
    cmgem.chwid = cmgem.txrat * cmgem.chht;
    settextsize(cmgem.chwid, cmgem.chht);
    settextangle(TEXT_HORIZONTAL);

  L_4000:
    flushbuffer(nerr);
    cursor0(&xloc, &yloc, &kchar);

    if (kchar == (char) ACK_CHAR) {
        error(*nerr=901, ": Window resized during PPK\n"
              "\t\t Exiting from PPK because picks are inaccurate after resize.\n"
              "\t\t Window resizing must be done before entering PPK.");
        goto L_7777;
    }

    upcase(&kchar, 1, &kchar, 1);

    /* - If the last character was a T, then this character must be
     *   integer.  In this case, the new cursor position is ignored. */

    if (lint) {
        ncerr = 0;
        cnvati(&kchar, 1, &unused, 0, &ncerr);  /* add 0, maf 970129 */
        if (ncerr > 0) {
            setmsg("WARNING", 1905);
            pltmsg(&xtpos, &ytpos);
            outmsg();
            ytpos = ytpos - cmgem.chht;
        } else {
            fstrncpy(kmeam.kpkid, 8, "T", 1);
            fstrncpy(kmeam.kpkid + 1, 8 - 1, (char *) &kchar, 1);
            markhdr(jdfl, jhdr1, jhdr2, kmeam.kpkid, secinc,
                    SAC_CHAR_UNDEFINED);
            markvert(jmark1, jmark2, &xloc, ypmxv, ypdelv, kmeam.kpkid, 9, 0);
            lint = FALSE;
        }
        goto L_5000;
    }

    /* - Act upon other cursor responses that do NOT need cursor position. */

    /* -- Go back to last x window. */
    else if (kchar == 'O') {
        tmin = twin[jwin - 1][0];
        tmax = twin[jwin - 1][1];
        jwin = max(1, jwin - 1);
        plhome();
        endframe(FALSE, nerr);
        /* npmark = 0; */
        goto L_2000;
    }

    /* -- Kill PPK; return immediately to command level. */
    else if (kchar == 'Q' || kchar == 'K') {
        plhome();
        endframe(FALSE, nerr);
        goto L_7777;
    }

    /* -- Go to next subplot. */
    else if (kchar == 'N') {
        plhome();
        endframe(FALSE, nerr);
        /* npmark = 0; */
        jfr = jfr + 1;
        goto L_1900;
    }

    /* -- Go back to last subplot. */
    else if (kchar == 'B') {
        plhome();
        endframe(FALSE, nerr);
        /* npmark = 0; */
        jfr = max(1, jfr - 1);
        goto L_1900;
    }

    /* - Rest of cursor responses need a valid cursor position. */

    if (((xloc < cmgem.plot.xmin || xloc > cmgem.plot.xmax) || yloc < ypmnv) || yloc > ypmxv) {
        setmsg("OUTPUT", 1502);
        apfmsg(xloc);
        apfmsg(yloc);
        pltmsg(&xtpos, &ytpos);
        ytpos = ytpos - cmgem.chht;
        clrmsg();
        goto L_4000;
    }

    /* - Determine which file. */
    jofset = (ypmxv - yloc) / ypdelv;
    jdfl = jdfl1 + jofset;
    if (!cmgam.lmkall) {
        jhdr1 = jdfl;
        jhdr2 = jdfl;
        jmark1 = jdfl - jdfl1 + 1;
        jmark2 = jmark1;
    }
    j = jofset + 1;
    
    /* - Determine time at cursor location.
     *   Time is relative to start of file
     *   (Correct for any differences between the zero times.) */
    if (cmgem.ixint == AXIS_LINEAR) {
        secinc = (xloc - cmgem.xmpip2) / cmgem.xmpip1 - toff[j];
    } else {
        secinc = pow(10., (xloc - cmgem.xmpip2) / cmgem.xmpip1);
    }
    DEBUG("secinc: %f [%f,%f] %d %f\n", secinc, xloc,yloc,jofset, (xloc - cmgem.xmpip2) / cmgem.xmpip1);
    /* - If a different file from last time, exchange headers. */
    if (jdfl != jdfls) {
        jdfls = jdfl;
        if (!(s = sacget(jdfl - 1, TRUE, nerr))) {
            goto L_7777;
        }
        //getfil( jdfl, TRUE, &nlen, &nlcy, &nlcx, nerr );

        cmeam.lpphas = A(s) != SAC_FLOAT_UNDEFINED && s->h->ka[0] == 'P';
        cmeam.lpphas = cmeam.lpphas && lzdttm[jdfl];
        cmeam.lsphas = T0(s) != SAC_FLOAT_UNDEFINED && s->h->kt0[0] == 'S';
        cmeam.lfini = F(s) != SAC_FLOAT_UNDEFINED;
        if (cmeam.lpphas) {
            psecsi = A(s);
            fstrncpy(kmeam.kpwave, 8, s->h->ka, 4);
            ktype = kmeam.kpwave[0];
            kdir = kmeam.kpwave[2];
            kqual = kmeam.kpwave[3];
            lempty = FALSE;
            if (cmeam.lsphas) {
                ssecsi = T0(s);
                fstrncpy(kmeam.kswave, 8, s->h->kt0, 4);
            }
            if (cmeam.lfini)
                fsecsi = F(s);
        } else if (cmeam.lsphas) {
            ssecsi = T0(s);
            fstrncpy(kmeam.kswave, 8, s->h->kt0, 4);
            ktype = kmeam.kswave[0];
            kdir = kmeam.kswave[2];
            kqual = kmeam.kswave[3];
            lempty = FALSE;
            if (cmeam.lfini)
                fsecsi = F(s);
        }
    }

    /* - Determine amplitude corresponding to cursor position. */
    amplmn = yimnzs[jdfl];
    amplmx = yimxzs[jdfl];
    cmeam.pkampl =
        amplmx - (ypmxv - jofset * ypdelv - yloc) * (amplmx - amplmn) / ypdelv;

    /* - Initialize hypo pick file values. */
    if (lempty) {
        cmeam.lpphas = FALSE;
        cmeam.lsphas = FALSE;
        cmeam.lfini = FALSE;
        cmeam.lampx = FALSE;
    }

    /* - Perform action corresponding to returned non-integer character. */

    /* -- Define end of new x window. */
    if (lnewxw) {
        jwin = min(MWIN, jwin + 1);
        twin[jwin - 1][0] = tmin;
        twin[jwin - 1][1] = tmax;
        /* Find the Smallest and Largest of the Two Time Points */
        tmin = fmin(tminew, secinc) + toff[j];
        tmax = fmax(tminew, secinc) + toff[j];
        if (kchar == 'S') {
            cmgam.lrtwxl = TRUE;
            strcpy(kmgam.krtwxl[0], "Z       ");
            strcpy(kmgam.krtwxl[1], "Z       ");
            cmgam.ortwxl[0] = tmin;
            cmgam.ortwxl[1] = tmax;
        }
        lnewxw = FALSE;
        plhome();
        endframe(FALSE, nerr);
        goto L_2000;
    }
    /* -- Define start of new x window. */
    else if (kchar == 'X') {
        lnewxw = TRUE;
        tminew = secinc;
        markvert(jmark1, jmark2, &xloc, ypmxv, ypdelv, "X", 2, 0);
    }

    /* -- Determine time and amplitude of cursor location. */
    else if (kchar == 'L') {
        sprintf(kyloc, "%15.5e", cmeam.pkampl);
        if (cmgam.lsavelocs) {
            char bbvar[32];
            nsavelocs = nsavelocs + 1;
            snprintf(bbvar, sizeof(bbvar), "yloc%d", nsavelocs);
            setbb("nlocs", VAR_INTEGER, nsavelocs);
            setbb(bbvar,   VAR_VALUE, cmeam.pkampl);
        }
        if (lzdttm[jdfl] && cmgam.lppkut) {
            inctim(s->h->nzhour, s->h->nzmin, s->h->nzsec, s->h->nzmsec, secinc,
                   &jhour, &jmin, &jsec, &jmsec, &nexday);
            incdat(s->h->nzyear, s->h->nzjday, nexday, &jyear, &jjday);
            kadate(jyear, jjday, 24, kndate, 25, &ncerr);
            katime(jhour, jmin, jsec, jmsec, 16, kntime, 17, &ncerr);

            sprintf(kptext, "%s %s %s", kndate, kntime, kyloc);
            if (cmgam.lsavelocs) {
                char bbvar[32], bbval[64];
                snprintf(bbvar, sizeof(bbvar), "xloc%d", nsavelocs);
                snprintf(bbval, sizeof(bbval), "%s %s", kndate, kntime);
                setbb(bbvar, VAR_STRING, bbval);
                snprintf(bbvar, sizeof(bbvar), "tloc%d", nsavelocs);
                setbb(bbvar, VAR_VALUE, secinc);
            }
        } else {
            sprintf(kxloc, "%15.5e", secinc);
            sprintf(kptext, "%s %s", kxloc, kyloc);
            if (cmgam.lsavelocs) {
                char bbvar[32];
                snprintf(bbvar, sizeof(bbvar), "xloc%d", nsavelocs);
                setbb(bbvar, VAR_VALUE, secinc);
                snprintf(bbvar, sizeof(bbvar), "tloc%d", nsavelocs);
                setbb(bbvar, VAR_VALUE, secinc);
            }
        }
        pltext(kptext, xtpos, ytpos);
        flushbuffer(nerr);
        ytpos = ytpos - cmgem.chht;
        strcpy(kmeam.kpkid, "LOC     ");
    }

    /* end else if( kchar == 'L' ) */
    /* -- Define first arrival time. */
    else if (kchar == 'A') {
        markhdr(jdfl, jhdr1, jhdr2, "A", secinc, kmeam.kpkid);
        markvert(jmark1, jmark2, &xloc, ypmxv, ypdelv, kmeam.kpkid, 9, npmark);
        npmark = npmark + 1;
    }

    /* -- First character of Tn time pick. */
    else if (kchar == 'T') {
        lint = TRUE;
    }

    /* -- Set direction to down. */
    else if (kchar == 'D')
        kdir = 'D';

    /* -- Set direction to up. */
    else if (kchar == 'U')
        kdir = 'U';

    /* -- Set direction to unknown. */
    else if (kchar == ' ')
        kdir = ' ';

    /* -- Set direction to slightly up. */
    else if (kchar == '+')
        kdir = '+';

    /* -- Set Direction to slightly down. */
    else if (kchar == '-')
        kdir = '-';

    /* -- Set phase onset to impulsive. */
    else if (kchar == 'I')
        ktype = 'I';

    /* -- Set phase onset to emergent. */
    else if (kchar == 'E')
        ktype = 'E';

    /* -- Set phase quality. */
    else if (kchar == '0')
        kqual = kchar;

    else if (kchar == '1')
        kqual = kchar;

    else if (kchar == '2')
        kqual = kchar;

    else if (kchar == '3')
        kqual = kchar;

    else if (kchar == '4')
        kqual = kchar;

    /* -- Calculate P wave arrival time. */
    else if (kchar == 'P') {
        fstrncpy(kmeam.kpkid, 8, (char *) &ktype, 1);
        fstrncpy(kmeam.kpkid + 1, 8 - 1, "P", 1);
        *(kmeam.kpkid + 2) = kdir;
        *(kmeam.kpkid + 3) = kqual;

        markhdr(jdfl, jhdr1, jhdr2, "A", secinc, kmeam.kpkid);
        markvert(jmark1, jmark2, &xloc, ypmxv, ypdelv, kmeam.kpkid, 9, npmark);
        npmark = npmark + 1;
        if (cmeam.lhpfop && lzdttm[jdfl]) {
            strcpy(kmeam.kpwave, kmeam.kpkid);
            psecsi = secinc;
            lempty = FALSE;
            cmeam.lpphas = TRUE;
            lhlwrt[jdfl] = FALSE;
            lhltrm = TRUE;
        }
    }

    /* -- Characterize first arrival. */
    else if (kchar == 'C') {
        ndxpk = 1 + (int) ((secinc - B(s)) / DT(s) + 0.9);
        pkchar(s->y, s->h->npts, DT(s), ndxpk, &ktype, &kdir, &kqual);

        fstrncpy(kmeam.kpkid, 8, (char *) &ktype, 1);
        fstrncpy(kmeam.kpkid + 1, 8 - 1, "P", 1);
        *(kmeam.kpkid + 2) = kdir;
        *(kmeam.kpkid + 3) = kqual;

        markhdr(jdfl, jhdr1, jhdr2, "A", secinc, kmeam.kpkid);
        markvert(jmark1, jmark2, &xloc, ypmxv, ypdelv, kmeam.kpkid, 9, npmark);
        npmark = npmark + 1;
        if (cmeam.lhpfop && lzdttm[jdfl]) {
            strcpy(kmeam.kpwave, kmeam.kpkid);
            psecsi = secinc;
            lempty = FALSE;
            cmeam.lpphas = TRUE;
            lhlwrt[jdfl] = FALSE;
            lhltrm = TRUE;
        }
        pkeval(s->y, s->h->npts, DT(s), ndxpk, &nlncda);
        if (nlncda > 0) {
            time = A(s) + DT(s) * (float) (nlncda);
            xloc =
                cmgem.plot.xmin + (time + toff[j] -
                                   tmin) * (cmgem.plot.xmax -
                                            cmgem.plot.xmin) / (tmax - tmin);
            markhdr(jdfl, jhdr1, jhdr2, "F", time, SAC_CHAR_UNDEFINED);
            if (xloc <= cmgem.plot.xmax) {
                markvert(jmark1, jmark2, &xloc, ypmxv, ypdelv, "F", 2, 0);
            }
            if (cmeam.lhpfop && lzdttm[jdfl]) {
                fsecsi = F(s);
                lempty = FALSE;
                cmeam.lfini = TRUE;
                lhltrm = TRUE;
            }
        }
    }

    /* end else if( kchar == 'C' ) */
    /* -- Define S wave arrival time. */
    else if (kchar == 'S') {
        fstrncpy(kmeam.kpkid, 8, (char *) &ktype, 1);
        fstrncpy(kmeam.kpkid + 1, 8 - 1, "S", 1);
        *(kmeam.kpkid + 2) = kdir;
        *(kmeam.kpkid + 3) = kqual;

        markhdr(jdfl, jhdr1, jhdr2, "T0", secinc, kmeam.kpkid);
        markvert(jmark1, jmark2, &xloc, ypmxv, ypdelv, kmeam.kpkid, 9, 0);
        if (cmeam.lhpfop && lzdttm[jdfl]) {
            fstrncpy(kmeam.kswave, 8, kmeam.kpkid, 2);
            *(kmeam.kswave + 2) = 'N';
            *(kmeam.kswave + 3) = kmeam.kpkid[3];

            ssecsi = secinc;
            lempty = FALSE;
            cmeam.lsphas = TRUE;
            lhltrm = TRUE;
        }
    }

    /* -- Define coda length (fini). */
    else if (kchar == 'F') {
        strcpy(kmeam.kpkid, "FINI    ");
        markhdr(jdfl, jhdr1, jhdr2, kmeam.kpkid, secinc, SAC_CHAR_UNDEFINED);
        markvert(jmark1, jmark2, &xloc, ypmxv, ypdelv, kmeam.kpkid, 9, 0);
        if (cmeam.lhpfop && lzdttm[jdfl]) {
            fsecsi = secinc;
            lempty = FALSE;
            cmeam.lfini = TRUE;
            lhltrm = TRUE;
        }
    }

    /* - Write HYPO line to HPF or terminal. */
    else if (kchar == 'G' || kchar == 'H') {
        if (cmeam.lhpfop) {
            if (lhlwrt[jdfl]) {
                setmsg("OUTPUT", 1907);
                pltmsg(&xtpos, &ytpos);
                ytpos = ytpos - cmgem.chht;
            } else if (!lzdttm[jdfl]) {
                pltext(kundrt, xtpos, ytpos);
                ytpos = ytpos - cmgem.chht;
            } else {
                if (kchar == 'G') {
                    lhltrm = TRUE;
                } else {
                    lhlhyp = TRUE;
                    lempty = TRUE;
                }
            }
        } else {
            setmsg("OUTPUT", 1908);
            pltmsg(&xtpos, &ytpos);
            ytpos = ytpos - cmgem.chht;
        }
    }

    /* end else if( kchar == 'G' || kchar == 'H' ) */
    /* -- Compute waveform. */
    else if ((kchar == 'W' || kchar == 'M') || kchar == 'V') {
        if (!(s = sacget(jdfl - 1, TRUE, nerr))) {
            goto L_7777;
        }
        //getfil( jdfl, TRUE, &nln, &nlcy, &nlcx, nerr );

        nst = (int) ((secinc - B(s)) / DT(s)) + 2;
        wavfrm(s->y, nst, s->h->npts, cmeam.pkampl, 5, iwf, &lwfok);
        // nst - Data Sample to start search from
        // iwf[0] - First value crossing backwards
        // iwf[1] - Next extrema
        // iwf[2] - Next value Crossing
        // iwf[3] - Next extrema
        // iwf[4] - Next value crossing
        if (lwfok) {
            tref1 = DT(s) * (s->y[Iwf[1] - 1] -
                               cmeam.pkampl) / (s->y[Iwf[1] - 1] -
                                                s->y[Iwf[1]]);
            Dtwf[1] = 0.;
            Awf[1] = cmeam.pkampl;
            Dtwf[2] = DT(s) * (float) (Iwf[2] - Iwf[1]) - tref1;
            Awf[2] = s->y[Iwf[2] - 1];

            Dtwf[3] =
                DT(s) * (s->y[Iwf[3] - 1] -
                               cmeam.pkampl) / (s->y[Iwf[3] - 1] -
                                                s->y[Iwf[3]]) - tref1 +
                DT(s) * (float) (Iwf[3] - Iwf[1]);

            Awf[3] = cmeam.pkampl;
            Dtwf[4] = DT(s) * (Iwf[4] - Iwf[1]) - tref1;
            Awf[4] = s->y[Iwf[4] - 1];

            Dtwf[5] =
                DT(s) * (s->y[Iwf[5] - 1] -
                               cmeam.pkampl) / (s->y[Iwf[5] - 1] -
                                                s->y[Iwf[5]]) - tref1 +
                DT(s) * (float) (Iwf[5] - Iwf[1]);

            Awf[5] = cmeam.pkampl;
            secinc = B(s) + DT(s) * (float) (Iwf[1] - 1) + tref1;
            facc = (cmgem.plot.xmax - cmgem.plot.xmin) / (tmax - tmin);
            xlocs1 = (secinc + toff[j] - tmin) * facc + cmgem.plot.xmin;
            seccur = secinc + Dtwf[5];
            xlocs2 = (seccur + toff[j] - tmin) * facc + cmgem.plot.xmin;
            jmark = jdfl - jdfl1 + 1;

            _c0[0] = kchar;
            _c0[1] = '\0';
            markwf(jmark, jmark, &xlocs1, &xlocs2, ypmxv, ypdelv, _c0, 2);

            if (kchar == 'W') {
                strcpy(kmeam.kpkid, "WF      ");
            } else if (kchar == 'V') {
                strcpy(kmeam.kpkid, "WAWF    ");
            } else {
                strcpy(kmeam.kpkid, "MWF     ");
                if (cmeam.lhpfop) {
                    cmeam.ampx = fabs(Awf[2] - Awf[4]);
                    cmeam.prx = Dtwf[5];
                    cmeam.lampx = TRUE;
                    lhltrm = TRUE;
                }
            }
        } else {
            setmsg("OUTPUT", 1909);
            pltmsg(&xtpos, &ytpos);
            ytpos = ytpos - cmgem.chht;
        }
    }

    /* end else if( (kchar == 'W' || kchar == 'M') || kchar == 'V' ) */
    /* -- Define a noise level. */
    else if (kchar == 'J') {
        strcpy(kmeam.kpkid, "NL      ");
        xloc1 = fmax(xloc - 0.05, cmgem.plot.xmin);
        xloc2 = fmin(xloc + 0.05, cmgem.plot.xmax);
        setlinewidth(cmgem.iwidth);
        line(xloc1, yloc, xloc2, yloc);
        setlinewidth(LINE_WIDTH_THIN);
        move(xloc2 + 0.005, yloc);
        settextjust(LEFT, CENTER);
        text(kmeam.kpkid, 9, 2);
        settextjust(LEFT, BOTTOM);
    }

    /* -- Define a reference or zero level. */
    else if (kchar == 'Z') {
        strcpy(kmeam.kpkid, "ZERO    ");
        setlinewidth(cmgem.iwidth);
        line(cmgem.plot.xmin, yloc, cmgem.plot.xmax, yloc);
        setlinewidth(LINE_WIDTH_THIN);
        move(cmgem.plot.xmax + 0.005, yloc);
        settextjust(LEFT, CENTER);
        text(kmeam.kpkid, 9, 4);
        if (cmgam.lppkrl) {
            prl = cmgam.vppkrl * ypdelv / (amplmx - amplmn);
            setlinewidth(cmgem.iwidth);
            line(cmgem.plot.xmin, yloc + prl, cmgem.plot.xmax, yloc + prl);
            setlinewidth(LINE_WIDTH_THIN);
            move(cmgem.plot.xmax + 0.005, yloc + prl);
            text("REF", 4, 3);
            setlinewidth(cmgem.iwidth);
            line(cmgem.plot.xmin, yloc - prl, cmgem.plot.xmax, yloc - prl);
            setlinewidth(LINE_WIDTH_THIN);
            move(cmgem.plot.xmax + 0.005, yloc - prl);
            text("REF", 4, 3);
        }
        settextjust(LEFT, BOTTOM);
    }

    else if (kchar == 0) {

    }
    /* -- Bad cursor response handled here. */
    else {
        setmsg("OUTPUT", 1503);
        apcmsg(&kchar, 1);
        pltmsg(&xtpos, &ytpos);
        ytpos = ytpos - cmgem.chht;
        clrmsg();
    }

    /* -- Write to alphanumeric pick file. */

  L_5000:
    if (strcmp(kmeam.kpkid, "        ") != 0 && cmeam.lapfop) {
        if (lzdttm[jdfl] && cmgam.lppkut) {
            cmeam.lpfgmt = TRUE;
        } else {
            cmeam.lpfgmt = FALSE;
        }
        cmeam.pkseci = secinc;
        strcpy(kmeam.kpksrc, "M       ");
        strcpy(kmeam.kpkrid, "        ");
        wapf();
        strcpy(kmeam.kpkid, "        ");
    }

    /* - Write to hypo line to file or terminal. */

    if (lhltrm || lhlhyp) {
        inctim(s->h->nzhour, s->h->nzmin, s->h->nzsec, s->h->nzmsec, psecsi,
               &cmeam.nphour, &cmeam.npmin, &npsec, &npmsec, &nexday);
        cmeam.psecs = tosecs(npsec, npmsec);
        incdat(s->h->nzyear, s->h->nzjday, nexday, &cmeam.npyear,
               &cmeam.npjday);
        kidate(cmeam.npyear, cmeam.npjday, &cmeam.npmon, &cmeam.npday, &ncerr);
        strcpy(kmeam.kstid, s->h->kstnm);
        if (cmeam.lsphas)
            cmeam.ssecs = cmeam.psecs - psecsi + ssecsi;
        if (cmeam.lfini)
            cmeam.fmp = fsecsi - psecsi;
        if (lhlhyp) {
            whpf1(kmsg, MCMSG + 1);
            fprintf(cmeam.nhpfun, "%s\n", kmsg);
            lhlwrt[jdfl] = TRUE;
            pltext("*", xtpos - cmgem.chwid, ytpos);
            rstrip(kmsg);
            pltext(kmsg, xtpos, ytpos);
            ytpos = ytpos - cmgem.chht;
            lhlhyp = FALSE;
        } else if (lhltrm) {
            whpf1(kmsg, MCMSG + 1);
            rstrip(kmsg);
            pltext(kmsg, xtpos, ytpos);
            ytpos = ytpos - cmgem.chht;
            lhltrm = FALSE;
        }
    }

    /* -- Loop back for another cursor response. */

    goto L_4000;

    /* - Restore plot environment. */

  L_7777:
    plrest();

    /* - Return to normal graphics device mode. */

    cursoroff();

    /* - Delete any extra locations from blackboard if necessary. */

    if (cmgam.lsavelocs) {
        for (jdx = nsavelocs + 1; jdx <= nsavelast; jdx++) {
            char bbvar[32];

            snprintf(bbvar, sizeof(bbvar), "xloc%d", jdx);
            unsetbbv(bbvar, nerr, -1);

            snprintf(bbvar, sizeof(bbvar), "yloc%d", jdx);
            unsetbbv(bbvar, nerr, -1);

        }
    }

  L_8888:
    cmgem.xlabel.on = xlabelsave;
    cmgem.ylabel.on = ylabelsave;
    xarray_free(lhlwrt);
    xarray_free(lzdttm);
    xarray_free(toff);
    xarray_free(yimnzs);
    xarray_free(yimxzs);
    return;

}                               /* end of function */
