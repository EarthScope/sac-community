/** 
 * @file   formhv.c
 * 
 * @brief  Format a header variable
 * 
 */

#include <stdio.h>
#include <string.h>

#include "amf.h"
#include "dff.h"
#include "msg.h"
#include "bool.h"
#include "bot.h"
#include "hdr.h"
#include "co.h"
#include "lhf.h"
#include "ucf.h"

HDR_EXTERN
LHF_EXTERN

/*
  Convert an ibody header type to a description
 */
char *
body_des(int ibody) {
    switch(ibody) {
    case ISUN:     return "Sun";                    break;
    case IMERCURY: return "Mercury IAU 2000";       break;
    case IVENUS:   return "Venus IAU2000";          break;
    case IEARTH:   return "Earth WGS 84";           break;
    case IMOON:    return "Moon IAU2000";           break;
    case IMARS:    return "Mars IAU2000";           break;
    default:       return "Earth (Sac Historical)"; break;
    }
}

/** 
 * Format a header variable into a text string
 * 
 * @param kname 
 *    Header variable name
 * @param kname_s 
 *    Length of \p kname
 * @param iform 
 *    Type of ouput format
 *    - 1 gives name followed by " = " followed by value.
 *    - 2 gives name followed by ": " followed by value.
 *    - 3 gives value only.
 * @param kout 
 *    Output string
 * @param kout_s 
 *    Length of \p kout
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - -1 if \p kname is undefined in the current header
 *            then \p kout is still formatted in this case
 *    - -2 if \p iform has a bad value
 *
 * @date   970128:  Now allows floats 7 digits. maf
 * @date   961212:  Modified to add INCLUSIVE option to the lh command.  maf
 * @date   841025:  Original version.
 *
 */
void
formhv(char *kname, int kname_s, int iform, char *kout, int kout_s, int *nerr) {

    char kvalue[41];
    int lok, lok2 = FALSE, linc;
    int icat, item, nc, ip;
    double fp;
    char *p;
    sac *s;
    *nerr = 0;
    s = sacget_current();
    memset(&(kvalue[0]), ' ', 40);
    kvalue[40] = '\0';

    /* if cmhdr.linc and .llh are both TRUE, so is linc. maf 961212 */
    linc = cmhdr.linc && cmhdr.llh;

    /* - Determine type and location of header field. */
    hdrfld(kname, kname_s, &icat, &item, &lok);

    if (lok) {
        /* lok was true coming out of hdrfld().  maf 961212 */
        lok2 = TRUE;
        switch (icat) {
            case FLOAT_TYPE:
                sac_get_float(s, item, &fp);
                lok = (fp != SAC_FLOAT_UNDEFINED);
                if (lok || linc) {
                    sprintf(kvalue, float_format_with_default("%#16.6e"), fp);
                    ljust(kvalue, 41);
                }
                break;
            case INT_TYPE:
                ip = VALUE(nhdr(s, item));
                lok = ip != SAC_INT_UNDEFINED;
                if (lok || linc) {
                    sprintf(kvalue, "%10d", ip);
                    ljust(kvalue, 41);
                }
                break;
            case ENUM_TYPE:
                ip = VALUE(ihdr(s, item));
                lok = ip != SAC_ENUM_UNDEFINED;
                if (lok) {
                    /*
                      Why is this if statement here for BODY TYPE ?
                      The event and station regions start at a value of 100
                      and the proposed IBODY values extended into this region
                      Eventually we will need to shift the event and station
                      regions to a higher integer value
                      Really, all of this should be in the sacio library.
                     */
                    if(item == SAC_BODY_TYPE_ENUM) {
                        char *b = body_des(ip);
                        fstrncpy(kvalue, 40, b, strlen(b));
                    } else {
                        fstrncpy(kvalue, 40, kmlhf.kdiv[ip - 1],
                                 strlen(kmlhf.kdiv[ip - 1]));
                    }
                }
                else if (linc)
                    strcpy(kvalue, "UNDEFINED                               ");
                break;
            case LOGICAL_TYPE:
                ip = VALUE(lhdr(s, item));
                lok = TRUE;
                if (ip) {
                    strcpy(kvalue, "TRUE                                    ");
                } else {
                    strcpy(kvalue, "FALSE                                   ");
                }
                break;
            case STRING_TYPE:
                p = khdr(s, item);
                lok = ! is_kundefn(p, item);
                if (lok || linc) {
                    memset(kvalue, ' ', sizeof(kvalue));
                    strncpy(kvalue, p, strlen(p));
                    kvalue[strlen(p)] = 0;
                }
                break;
            case AUX_TYPE:
                lok = lgahdr(kname, kname_s, kvalue, 41);
                break;
        }
    }

    if (!lok2 || (!lok && !linc)) {
        strcpy(kvalue, "Undefined                               ");
        *nerr = -1;
    }

    if (iform == 1) {
        nc = indexb(kname, kname_s);
        fstrncpy(kout, kout_s - 1, kname, nc);
        fstrncpy(kout + nc, kout_s - 1 - nc, " = ", 3);
        fstrncpy(kout + nc + 3, kout_s - 1 - nc - 3, kvalue, strlen(kvalue));
    } else if (iform == 2) {
        nc = indexb(kname, kname_s);
        fstrncpy(kout, kout_s - 1, kname, nc);
        fstrncpy(kout + nc, kout_s - 1 - nc, ": ", 2);
        fstrncpy(kout + nc + 2, kout_s - 1 - nc - 2, kvalue, strlen(kvalue));
    } else if (iform == 3) {
        fstrncpy(kout, kout_s - 1, kvalue, strlen(kvalue));
    } else {
        *nerr = -2;
        strcpy(kvalue, "Bad format number                       ");
    }

    return;
}
