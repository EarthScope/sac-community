
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ssi.h"
#include "dfm.h"
#include "hdr.h"
#include "bool.h"

#include "msg.h"
#include "bot.h"
#include "sacIOdb/sacIOdb.h"

DFM_EXTERN

static char *
strcpy_check(char *dst, char *src) {
    if (dst != src) {
        strcpy(dst, src);
    }
    return dst;
}

void                            /* FUNCTION */
prefPicksToHeader(header, idfl, wf, tree, correction, nerr)
     struct SACheader *header;
     int idfl,                  /* file number of first waveform */
      *nerr;                    /* error number */
     DBlist tree;
     struct wfdisc *wf;
     double correction;         /* to correct epoch time to either B or O */
{
    /* Declare Automatic Variables. */

    int idx, jdx;               /* for loop indeces */
    float *picks[10];
    char *phase[10];
    const int offAUTH = 15;

    /* linked list of arrivals associated with a given waveform */
    struct arrivalList *pArr = NULL,    /* head of stored linked list */
        *storeArr = NULL,       /* some link in stored list */
        *prevArr = NULL,        /* link in front of storeArr */
        *look4Arr = NULL;       /* index list being searched */
    struct arrival *currentArr = NULL;  /* index element of current link */

    /*=====================================================================
     * PURPOSE:  To get picks from SeisMgr into SAC, according to user
     *           preferences.  
     *=====================================================================
     * INPUT ARGUMENTS:
     *    idfl   file number of first file being handled currently
     *    wf:    Struct with waveform information used for comparison.
     *    tree:  Struct with info from a variety of CSS tables including
     *           the arrival table, from which the picks are gleened.
     *    correction: corrects time headers to the desired reference time.
     *=====================================================================
     * OUTPUT ARGUMENTS:
     *    header:  Struct containing SAC header including new picks.
     *    nerr:    Error flag. Set to 0 if no error occurred.
     *=====================================================================
     * MODIFICATION HISTORY:
     *    971202:  Original version.  maf
     *===================================================================== */
    /* PROCEDURE: */
    *nerr = 0;

    /* set pick header variables up for address arithatic. */
    picks[0] = &(header->_t0);
    picks[1] = &(header->_t1);
    picks[2] = &(header->_t2);
    picks[3] = &(header->_t3);
    picks[4] = &(header->_t4);
    picks[5] = &(header->_t5);
    picks[6] = &(header->_t6);
    picks[7] = &(header->_t7);
    picks[8] = &(header->_t8);
    picks[9] = &(header->_t9);
    phase[0] = (header->kt0);
    phase[1] = (header->kt1);
    phase[2] = (header->kt2);
    phase[3] = (header->kt3);
    phase[4] = (header->kt4);
    phase[5] = (header->kt5);
    phase[6] = (header->kt6);
    phase[7] = (header->kt7);
    phase[8] = (header->kt8);
    phase[9] = (header->kt9);

    /* Build a linked list of all the arrivals associated with the given 
       waveform.  Loop between arrivals in the tree */
    do {
        look4Arr = sacFindNextMatchingPick(look4Arr, tree, wf->wfid);

        if (!look4Arr)
            break;
        if (!strcmp(look4Arr->element->iphase, "F"))
            continue;

        /* make author name lower case for case insensitivity */
        modcase(FALSE, look4Arr->element->auth, offAUTH,
                look4Arr->element->auth);

        /* terminate the author and phase */
        terminate(look4Arr->element->auth);
        terminate(look4Arr->element->iphase);

        /* Save pointer to element in pArr */
        if (pArr) {
            storeArr =
                (struct arrivalList *) malloc(sizeof(struct arrivalList));
        } else {
            pArr = (struct arrivalList *) malloc(sizeof(struct arrivalList));
            storeArr = pArr;
        }
        if (storeArr == NULL) {
            /* do some error handling */
            *nerr = 301;
            setmsg("ERROR", *nerr);
            outmsg();
            clrmsg();

            goto L_8888;
        }

        /* link the links together. */
        storeArr->next = NULL;
        storeArr->prev = prevArr;
        if (prevArr != NULL)
            prevArr->next = storeArr;
        storeArr->element = look4Arr->element;

        prevArr = storeArr;

    } while (look4Arr);

    /* Search new linked list for arrivals matching desired author and phase 
       according to the preferences file or instructions from pickauthor 
       and/or pickphase.  Put arrivals into pick header variables t0 - t9.
       Loop between pick header variables. */
    for (idx = 0; idx < 10; idx++) {
        int nA;                 /* number of authors for the given slot. */
        int lA;                 /* TRUE if author list is used */

        /* initialize pick */
        *(picks[idx]) = SAC_FLOAT_UNDEFINED;
        *(phase[idx]) = '\0';

        /* Is author hardwired, or will it cascade through author list? */
        if (strcmp(kmdfm.ktAu[idx], "-") == 0) {
            nA = cmdfm.iauthors;        /* cascade */
            lA = TRUE;
        } else {
            nA = 1;             /* hardwired */
            lA = FALSE;
        }

        /* loop through authors */
        for (jdx = 0; jdx < nA; jdx++) {
            char *thisAuthor;   /* name of current author */

            /* get name of current author */
            if (lA)
                thisAuthor = kmdfm.kauthors[jdx];
            else
                thisAuthor = kmdfm.ktAu[idx];

            /* loop through stored arrival records for one that
               matches the phase and the author. */
            for (storeArr = pArr; storeArr; storeArr = storeArr->next) {
                currentArr = storeArr->element;

                if (strcmp(currentArr->auth, thisAuthor) == 0 &&
                    strcmp(currentArr->iphase, kmdfm.ktPh[idx]) == 0) {

                    /* GET THE PICK */

                    /* get pick in epoch time, correct for trace begin. */
                    *(picks[idx]) = currentArr->time - correction;

                    /* set kt? to the phase name. */
                    strcpy(phase[idx], currentArr->iphase);

                    /* if one wanted to get the author name
                       this is where one would do it. @@@###$$$%%% */

                    /* break out of this loop with 'break',
                       break out of author loop by setting jdx = nA */
                    jdx = nA + 10;
                    break;
                }               /* end if ( strcmp ) */
            }                   /* end for ( storeArr )  */
        }                       /* end for ( jdx ) */

        /* if no match was made for this header variable
           look for alternate authors and alert user. */
        if (jdx < nA + 5) {
            char message[500];  /* message to alert user */

            /* start the message */
            sprintf(message,
                    "For file number %d and phase %s, the following authors have picks:\n",
                    idfl, kmdfm.ktPh[idx]);

            /* add authors to the message as they are found. */
            for (storeArr = pArr; storeArr; storeArr = storeArr->next) {
                currentArr = storeArr->element;
                if (strcmp(currentArr->iphase, kmdfm.ktPh[idx]) == 0) {
                    /*lauthor = TRUE ; *//* author found, message will be printed */
                    sprintf(message, "%s\t%s\n", message, currentArr->auth);
                }
            }                   /* end for ( storeArr ) */

            /* if one or more authors found for the phase, print message */
            /*  if ( lauthor )
               fprintf ( stderr , message ) ; */
        }
        /* end if ( jdx < nA + 5 ) */
    }                           /* end for ( idx ) */

    /* put new picks into the header */
    header->_t0 = *picks[0];
    header->_t1 = *picks[1];
    header->_t2 = *picks[2];
    header->_t3 = *picks[3];
    header->_t4 = *picks[4];
    header->_t5 = *picks[5];
    header->_t6 = *picks[6];
    header->_t7 = *picks[7];
    header->_t8 = *picks[8];
    header->_t9 = *picks[9];

    strcpy_check(header->kt0, phase[0]);
    strcpy_check(header->kt1, phase[1]);
    strcpy_check(header->kt2, phase[2]);
    strcpy_check(header->kt3, phase[3]);
    strcpy_check(header->kt4, phase[4]);
    strcpy_check(header->kt5, phase[5]);
    strcpy_check(header->kt6, phase[6]);
    strcpy_check(header->kt7, phase[7]);
    strcpy_check(header->kt8, phase[8]);
    strcpy_check(header->kt9, phase[9]);

  L_8888:
    /* free up allocated memory */
    while (prevArr && prevArr->prev) {
        prevArr = prevArr->prev;
        free(prevArr->next);
    }
    if (pArr)
        free(pArr);

    return;
}                               /* end prefPicksToHeader */
