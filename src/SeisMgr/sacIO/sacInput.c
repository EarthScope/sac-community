#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <math.h>
#include <float.h>

#include "hdr.h"
#include "timefuncs.h"
#include "cssListOps/cssStrucs.h"
#include "cssListOps/dblUserData.h"
#include "cssListOps/dblPublicDefs.h"
#include "cssListOps/cssListOps.h"
#include "cssListOps/dblErrors.h"
#include "smMemory/smMemory.h"
#include "sacIOdb/sacIOdb.h"

#include "debug.h"

int CSSfltDefined(float value);
int CSSdblDefined(double value);
int CSSlngDefined(int value);
int CSSchrDefined(char *value);

int originNeeded(struct SACheader *header);
int eventNeeded(struct SACheader *header);
int siteNeeded(struct SACheader *header);
int sitechanNeeded(struct SACheader *header);

//static struct wftagList *FindWftag(DBlist tree, int wfid) ;
static struct wftagList *FindNxtWftag(struct wftagList *wt, DBlist tree,
                                      int wfid);
//static struct wftagList *FindWftagByWfidAndTagname( DBlist tree, int wfid, char* tagname ) ;
static struct eventList *FindEvent(DBlist tree, int evid);
static struct originList *FindOrigin(DBlist tree, int orid);
static struct originList *FindOriginByEvid(DBlist tree, int evid);

#include "smDataIO.h"

#define LONGAGO 1800001
/* ------------------------------------------------------------------ */

char *
CSSstrcpy(char *fresh, char *old) {
    if (!strcmp(old, "-12345"))
        strcpy(fresh, "-");
    else
        strcpy(fresh, old);

    return fresh;
}

/* ------------------------------------------------------------------ */

int
CSSstrcmp(char *s1, char *s2) {
    if (!strcmp(s1, s2))
        return 0;
    if (!strcmp(s1, "-") && !strcmp(s2, "-12345"))
        return 0;
    if (!strcmp(s2, "-") && !strcmp(s1, "-12345"))
        return 0;
    return 1;
}

/* ------------------------------------------------------------------ */

static int
FloatsDiffer0(float V1, float V2) {
    if (V1 == 0.0 && V2 == -12345.0)
        return 0;
    if (V2 == 0.0 && V1 == -12345.0)
        return 0;
    return (fabs(V1 - V2) < FLT_EPSILON) ? 0 : 1;
}


static int
FloatsDiffer9(float V1, float V2) {
    if (V1 == -999.0 && V2 == -12345.0)
        return 0;
    if (V2 == -999.0 && V1 == -12345.0)
        return 0;
    return (fabs(V1 - V2) < FLT_EPSILON) ? 0 : 1;
}

static void
DeBlank(char *str) {
    int j = strlen(str) - 1;
    if (j < 1)
        return;
    while (j && (*(str + j) == ' ')) {
        *(str + j) = '\0';
        j--;
    }
}

/* ------------------------------------------------------------------ */

void
DeBlankHeaderStrings(struct SACheader *header) {

    DeBlank(header->kstnm);
    DeBlank(header->kevnm);
    DeBlank(header->khole);
    DeBlank(header->ko);
    DeBlank(header->ka);
    DeBlank(header->kt0);
    DeBlank(header->kt1);
    DeBlank(header->kt2);
    DeBlank(header->kt3);
    DeBlank(header->kt4);
    DeBlank(header->kt5);
    DeBlank(header->kt6);
    DeBlank(header->kt7);
    DeBlank(header->kt8);
    DeBlank(header->kt9);
    DeBlank(header->kf);
    DeBlank(header->kuser0);
    DeBlank(header->kuser1);
    DeBlank(header->kuser2);
    DeBlank(header->kcmpnm);
    DeBlank(header->knetwk);
    DeBlank(header->kdatrd);
    DeBlank(header->kinst);
}

/* ------------------------------------------------------------------ */

static int
fltDefined(float value) {
    if (value == SAC_FLOAT_UNDEFINED)
        return FALSE;
    return TRUE;
}

/* ------------------------------------------------------------------ */

static int
dblDefined(double value) {
    if (value == SAC_FLOAT_UNDEFINED)
        return FALSE;
    return TRUE;
}

/* ------------------------------------------------------------------ */

int
lngDefined(int value) {
    if (value == SAC_INT_UNDEFINED)
        return FALSE;
    return TRUE;
}

/* ------------------------------------------------------------------ */

static int
StrDefined(char *value) {
    if (!strncmp(value, SAC_CHAR_UNDEFINED, 6))
        return FALSE;
    return TRUE;
}


/* Map event type from CSS to SAC */
static char *
sacSetEtype(int ievtyp) {
    switch (ievtyp) {
        case IUNKN:
            return "Unk";
        case INUCL:
            return "en";
        case IPREN:
            return "PreN";
        case IPOSTN:
            return "PostN";
        case IQUAKE:
        case IEQ:
            return "qt";
        case IPREQ:
            return "ForeShk";
        case IPOSTQ:
            return "AftrShk";
        case ICHEM:
            return "ec";
        case IOTHER:
            return "Other";
        case IQB:
            return "me";
        case IQC:
            return "mc";
        case IME:
            return "ex";
        case IEX:
            return "ex";
        case IEX0:
            return "ep";
        case IOS:
        case IO_:
            return "o";
        case IL:
            return "l";
        case IR:
            return "r";
        case IT:
            return "t";
        case IQB0:
            return "mp";
        case IQB1:
            return "qb+";
        case IQB2:
            return "qb++";
        case IQBX:
            return "qbx";
        case IQMT:
            return "mb";
        case IEQ0:
            return "qp";
        case IEQ1:
            return "eq+";
        case IEQ2:
            return "qf";
        case IEQ3:
            return "qd";
        case INU:
            return "en";
        case INC:
            return "nc";
        case IGEY:
            return "ge";
        case IMET:
            return "xm";
        case ILIT:
            return "xl";
        case IODOR:
            return "xo";
    }
    return "-";
}


static int
MagInfoConsistent(float mag, int imagtyp, int imagsrc, float ml, float mb,
                  float ms, char *auth) {

    UNUSED(auth);
    UNUSED(imagsrc);
    switch (imagtyp) {
        case IMB:
            if (mb == mag)
                break;
            else if (!fltDefined(mag) && !CSSfltDefined(mb))
                break;
            else
                return 0;       /* not consistent. */
        case IMS:
            if (ms == mag)
                break;
            else if (!fltDefined(mag) && !CSSfltDefined(ms))
                break;
            else
                return 0;       /* not consistent. */
        case IML:
            if (ml == mag)
                break;
            else if (!fltDefined(mag) && !CSSfltDefined(ml))
                break;
            else
                return 0;       /* not consistent. */
        default:               /* imagtyp must not be set so see if any css mags are set. */

            /* If imagtype is not set but mb is set and matches mag, then consider that the mag info
               is consistent. This is because the default CSS magtype for SAC mags without imagtyp set
               is mb. */

            if (CSSfltDefined(ms) || CSSfltDefined(ml))
                return 0;       /* not consistent. */
            else if (CSSfltDefined(mb) && mb != mag)
                return 0;
    }

    /* for now, don't check imagsrc against auth */
    return 1;

}

/* ------------------------------------------------------------------ */

int
fltsDiffer(float fltX, float fltY) {
    return fabs(fltX - fltY) > FLT_EPSILON;
}

static int
OriginsMatch(struct originList *orig, struct SACheader *header, double otime) {

    if (!orig)
        return 0;

    if (fltDefined(header->evla) || CSSfltDefined(orig->element->lat))
        if (fltsDiffer(header->evla, orig->element->lat))
            return 0;

    if (fltDefined(header->evlo) || CSSfltDefined(orig->element->lon))
        if (fltsDiffer(header->evlo, orig->element->lon))
            return 0;

    if (fltDefined(header->evdp) || CSSfltDefined(orig->element->depth))
        if (fltsDiffer(header->evdp / 1000.0, orig->element->depth))
            return 0;

    if (dblDefined(otime) || CSSdblDefined(orig->element->time))
        if (fltsDiffer(otime, orig->element->time))
            return 0;

    if (!MagInfoConsistent
        (header->mag, header->imagtyp, header->imagsrc, orig->element->ml,
         orig->element->mb, orig->element->ms, orig->element->auth))
        return 0;

    return 1;

}

/* ------------------------------------------------------------------ */

static struct sitechanList *
FindSiteChan(DBlist tree, char *kstnm, char *kcmpnm, int jdate) {
    struct sitechanList *sc = 0;

    do {
        sc = (struct sitechanList *) dblNextTableInstance(sc, tree,
                                                          dbl_LIST_SITECHAN);
        if (!sc)
            break;
        if (StrDefined(kstnm) && !strcmp(sc->element->sta, kstnm))
            if (StrDefined(kcmpnm) && !strcmp(sc->element->chan, kcmpnm))
                if (sc->element->ondate <= jdate)
                    if (sc->element->offdate >= jdate ||
                        sc->element->offdate < 0)
                        return sc;
    } while (sc);
    return 0;
}

/* ------------------------------------------------------------------ */

static struct sitechanList *
MakeSiteChan(DBlist tree, char *kstnm, char *kcmpnm, float cmpinc, float stdp,
             float cmpaz, int jdate) {
    struct sitechanList *sc;
    UNUSED(jdate);
    sc = (struct sitechanList *) dblCreateTableInstance(tree,
                                                        dbl_LIST_SITECHAN);
    sc->element->chanid = dblNextAvailableChanid(tree);
    strlcpy(sc->element->lddate, tmListEpochTime(tmGetEpochTime(), 18),
            sizeof(sc->element->lddate));
    sc->element->ondate = LONGAGO;

    if (StrDefined(kstnm))
        strlcpy(sc->element->sta, kstnm, sizeof(sc->element->sta));

    if (StrDefined(kcmpnm))
        strlcpy(sc->element->chan, kcmpnm, sizeof(sc->element->chan));

    if (fltDefined(stdp))
        sc->element->edepth = stdp / 1000.0;

    if (fltDefined(cmpaz))
        sc->element->hang = cmpaz;

    if (fltDefined(cmpinc))
        sc->element->vang = cmpinc;

    return sc;
}

/* ------------------------------------------------------------------ */

static struct siteList *
findSite(DBlist tree, char *kstnm, int jdate) {
    struct siteList *si = 0;

    do {
        si = (struct siteList *) dblNextTableInstance(si, tree, dbl_LIST_SITE);
        if (!si)
            break;
        if (!CSSstrcmp(si->element->sta, kstnm))
            if (si->element->ondate <= jdate)
                if (si->element->ondate >= jdate || si->element->offdate < 0)
                    return si;
    } while (si);

    return 0;
}

/* ------------------------------------------------------------------ */

static struct affiliationList *
findAffiliation(DBlist tree, char *sta, char *network) {
    struct affiliationList *af = 0;
    do {
        af = (struct affiliationList *) dblNextTableInstance(af, tree,
                                                             dbl_LIST_AFFILIATION);
        if (!af)
            break;
        if (!CSSstrcmp(af->element->sta, sta))
            if (!CSSstrcmp(af->element->net, network))
                return af;
    } while (af);
    return 0;
}

/* ------------------------------------------------------------------ */

static int
ArrivalJoinsToTrace(DBlist tree, struct arrivalList *ar, int wfid) {
    struct wftagList *wf = 0;
    do {
        wf = (struct wftagList *) dblNextTableInstance(wf, tree,
                                                       dbl_LIST_WFTAG);
        if (!wf)
            break;
        if (strcmp(wf->element->tagname, "arid"))
            continue;
        if (wf->element->tagid != ar->element->arid)
            continue;
        if (wf->element->wfid == wfid)
            return 1;
    } while (wf);
    return 0;
}

/* ------------------------------------------------------------------ */

static void
ResetAssocName(DBlist tree, int arid, char *NewSta) {
    struct assocList *as = 0;
    do {
        as = (struct assocList *) dblNextTableInstance(as, tree,
                                                       dbl_LIST_ASSOC);
        if (!as)
            break;
        if (as->element->arid == arid)
            CSSstrcpy(as->element->sta, NewSta);
    } while (as);

}

/* ------------------------------------------------------------------ */

static struct instrumentList *
FindLinkedSensor(DBlist tree, int inid) {
    struct instrumentList *in = 0;
    do {
        in = (struct instrumentList *) dblNextTableInstance(in, tree,
                                                            dbl_LIST_INSTRUMENT);
        if (!in)
            break;
        if (in->element->inid == inid)
            return in;
    } while (in);
    return 0;
}

/* ------------------------------------------------------------------ */

static int
StaUniqueInWfdiscList(DBlist tree, char *sta) {
    struct wfdiscList *w = 0;
    int Matches = 0;
    do {
        w = (struct wfdiscList *) dblNextTableInstance(w, tree,
                                                       dbl_LIST_WFDISC);
        if (!w)
            break;
        if (!strcmp(w->element->sta, sta))
            Matches++;
    } while (w);
    return (Matches < 2) ? 1 : 0;

}

/* ------------------------------------------------------------------ */

static void
MakeNameUpdateStructCopies(DBlist tree, struct wfdiscList *w, char *NewSta,
                           char *net) {
    /* Make a copy of all structs with a sta key equal to w->element->sta
       and then set the sta key equal to NewSta. */
    struct siteList *si, *si2;
    struct sitechanList *sc, *sc2;
    struct affiliationList *af, *af2;
    struct arrivalList *ar;
    struct sensorList *se, *se2;
    struct instrumentList *in, *in2;

    /* Any arrival that joins to this trace is simply given the new name and its assoc is found
       and its name is reset. */
    ar = 0;
    do {
        ar = (struct arrivalList *) dblNextTableInstance(ar, tree,
                                                         dbl_LIST_ARRIVAL);
        if (!ar)
            break;
        if (ArrivalJoinsToTrace(tree, ar, w->element->wfid)) {
            CSSstrcpy(ar->element->sta, NewSta);
            ResetAssocName(tree, ar->element->arid, NewSta);
        }
    } while (ar);

    /* If this is the only trace with this sta, then only need to reset sta in
       linked table entries. */
    if (StaUniqueInWfdiscList(tree, w->element->sta)) {
        si = findSite(tree, w->element->sta, w->element->jdate);
        if (si)
            CSSstrcpy(si->element->sta, NewSta);

        sc = 0;
        do {
            sc = (struct sitechanList *) dblNextTableInstance(sc, tree,
                                                              dbl_LIST_SITECHAN);
            if (!sc)
                break;
            if (!strcmp(sc->element->sta, w->element->sta)) {
                CSSstrcpy(sc->element->sta, NewSta);
                break;
            }
        } while (sc);

        af = findAffiliation(tree, w->element->sta, net);
        if (af) {
            CSSstrcpy(af->element->sta, NewSta);
        }

        se = 0;
        do {
            se = (struct sensorList *) dblNextTableInstance(se, tree,
                                                            dbl_LIST_SENSOR);
            if (!se)
                break;
            if (!strcmp(se->element->sta, w->element->sta)) {
                CSSstrcpy(se->element->sta, NewSta);
                break;
            }
        } while (se);
        CSSstrcpy(w->element->sta, NewSta);
        return;
    }

    /* Otherwise, find linked entries, make copies, and rename the copies. */
    si = findSite(tree, w->element->sta, w->element->jdate);
    if (si) {
        si2 = (struct siteList *) dblCreateTableInstance(tree, dbl_LIST_SITE);
        *(si2->element) = *(si->element);
        //dblCopyTable(dbl_LIST_SITE, si, si2);
        CSSstrcpy(si2->element->sta, NewSta);
    }

    sc = 0;
    do {
        sc = (struct sitechanList *) dblNextTableInstance(sc, tree,
                                                          dbl_LIST_SITECHAN);
        if (!sc)
            break;
        if (strcmp(sc->element->chan, w->element->chan))
            continue;
        if (strcmp(sc->element->sta, w->element->sta))
            continue;
        if (sc->element->ondate <= w->element->jdate)
            if (sc->element->offdate >= w->element->jdate ||
                sc->element->offdate < 0) {
                sc2 =
                    (struct sitechanList *) dblCreateTableInstance(tree,
                                                                   dbl_LIST_SITECHAN);
                *(sc2->element) = *(sc->element);
                //dblCopyTable(dbl_LIST_SITECHAN, sc, sc2);
                CSSstrcpy(sc2->element->sta, NewSta);
            }
    } while (sc);

    af = findAffiliation(tree, w->element->sta, net);
    if (af) {
        af2 =
            (struct affiliationList *) dblCreateTableInstance(tree,
                                                              dbl_LIST_AFFILIATION);
        *(af2->element) = *(af->element);
        //dblCopyTable(dbl_LIST_AFFILIATION, af, af2);
        CSSstrcpy(af2->element->sta, NewSta);
    }

    /* If a sensor joins to this wfdisc, add a new sensor to list, make a new instrument,
       copy contents of existing sensor and instrument to new elements, create a new inid
       linking the new instrument to new sensor, and rename sensor so it joins to wfdisc
       after renaming. */
    se = 0;
    do {
        se = (struct sensorList *) dblNextTableInstance(se, tree,
                                                        dbl_LIST_SENSOR);
        if (!se)
            break;
        if (strcmp(se->element->chan, w->element->chan))
            continue;
        if (strcmp(se->element->sta, w->element->sta))
            continue;
        if (se->element->time <= w->element->jdate)
            if (se->element->endtime >= w->element->jdate ||
                se->element->endtime < 0) {
                se2 =
                    (struct sensorList *) dblCreateTableInstance(tree,
                                                                 dbl_LIST_SENSOR);
                *(se2->element) = *(se->element);
                //dblCopyTable(dbl_LIST_SENSOR, se, se2);
                CSSstrcpy(se2->element->sta, NewSta);
                in = FindLinkedSensor(tree, se2->element->inid);
                if (in) {
                    in2 =
                        (struct instrumentList *) dblCreateTableInstance(tree,
                                                                         dbl_LIST_INSTRUMENT);
                    *(in2->element) = *(in->element);
                    //dblCopyTable(dbl_LIST_INSTRUMENT, in, in2);
                    in2->element->inid = dblNextAvailableInid(tree);
                    se2->element->inid = in2->element->inid;
                }
            }
    } while (se);

    CSSstrcpy(w->element->sta, NewSta);
}

/* ------------------------------------------------------------------ */

static int
StaChanUniqueInWfdiscList(DBlist tree, char *sta, char *chan) {
    struct wfdiscList *w = 0;
    int Matches = 0;
    do {
        w = (struct wfdiscList *) dblNextTableInstance(w, tree,
                                                       dbl_LIST_WFDISC);
        if (!w)
            break;
        if (!strcmp(w->element->sta, sta) && !strcmp(w->element->chan, chan))
            Matches++;
    } while (w);
    return (Matches < 2) ? 1 : 0;

}

/* ------------------------------------------------------------------ */

static void
MakeChanUpdateStructCopies(DBlist tree, struct wfdiscList *w, char *NewChan) {
    /* Make a copy of all structs with a sta, chan keys equal to w->element->sta, w->element->chan
       and then set the chan key equal to NewChan. */
    struct sitechanList *sc, *sc2;
    struct arrivalList *ar;
    struct sensorList *se, *se2;
    struct instrumentList *in, *in2;

    /* Any arrival that joins to this trace is simply given the new chan. */
    ar = 0;
    do {
        ar = (struct arrivalList *) dblNextTableInstance(ar, tree,
                                                         dbl_LIST_ARRIVAL);
        if (!ar)
            break;
        if (ArrivalJoinsToTrace(tree, ar, w->element->wfid)) {
            CSSstrcpy(ar->element->chan, NewChan);
        }
    } while (ar);

    /* If this is the only trace with this sta-chan, then only need to reset chan in
       linked table entries. */
    if (StaChanUniqueInWfdiscList(tree, w->element->sta, w->element->chan)) {
        sc = 0;
        do {
            sc = (struct sitechanList *) dblNextTableInstance(sc, tree,
                                                              dbl_LIST_SITECHAN);
            if (!sc)
                break;
            if (!strcmp(sc->element->sta, w->element->sta) &&
                !strcmp(sc->element->chan, w->element->chan)) {
                CSSstrcpy(sc->element->chan, NewChan);
                break;
            }
        } while (sc);

        se = 0;
        do {
            se = (struct sensorList *) dblNextTableInstance(se, tree,
                                                            dbl_LIST_SENSOR);
            if (!se)
                break;
            if (!strcmp(se->element->sta, w->element->sta) &&
                !strcmp(se->element->chan, w->element->chan)) {
                CSSstrcpy(se->element->chan, NewChan);
                break;
            }
        } while (se);
        CSSstrcpy(w->element->chan, NewChan);
        return;
    }

    sc = 0;
    do {
        sc = (struct sitechanList *) dblNextTableInstance(sc, tree,
                                                          dbl_LIST_SITECHAN);
        if (!sc)
            break;
        if (strcmp(sc->element->chan, w->element->chan))
            continue;
        if (strcmp(sc->element->sta, w->element->sta))
            continue;
        if (sc->element->ondate <= w->element->jdate)
            if (sc->element->offdate >= w->element->jdate ||
                sc->element->offdate < 0) {
                sc2 =
                    (struct sitechanList *) dblCreateTableInstance(tree,
                                                                   dbl_LIST_SITECHAN);
                *(sc2->element) = *(sc->element);
                //dblCopyTable(dbl_LIST_SITECHAN, sc, sc2);
                CSSstrcpy(sc2->element->chan, NewChan);
            }
    } while (sc);

    /* If a sensor joins to this wfdisc, add a new sensor to list, make a new instrument,
       copy contents of existing sensor and instrument to new elements, create a new inid
       linking the new instrument to new sensor, and rename sensor so it joins to wfdisc
       after renaming. */
    se = 0;
    do {
        se = (struct sensorList *) dblNextTableInstance(se, tree,
                                                        dbl_LIST_SENSOR);
        if (!se)
            break;
        if (strcmp(se->element->chan, w->element->chan))
            continue;
        if (strcmp(se->element->sta, w->element->sta))
            continue;
        if (se->element->time <= w->element->jdate)
            if (se->element->endtime >= w->element->jdate ||
                se->element->endtime < 0) {
                se2 =
                    (struct sensorList *) dblCreateTableInstance(tree,
                                                                 dbl_LIST_SENSOR);
                *(se2->element) = *(se->element);
                //dblCopyTable(dbl_LIST_SENSOR, se, se2);
                CSSstrcpy(se2->element->chan, NewChan);
                in = FindLinkedSensor(tree, se2->element->inid);
                if (in) {
                    in2 =
                        (struct instrumentList *) dblCreateTableInstance(tree,
                                                                         dbl_LIST_INSTRUMENT);
                    *(in2->element) = *(in->element);
                    //dblCopyTable(dbl_LIST_INSTRUMENT, in, in2);
                    in2->element->inid = dblNextAvailableInid(tree);
                    se2->element->inid = in2->element->inid;
                }
            }
    } while (se);

    CSSstrcpy(w->element->chan, NewChan);
}

/* ------------------------------------------------------------------ */

void
sacAddWftagStruct(DBlist tree, struct originList *orig, struct wfdiscList *w) {
    struct wftagList *wt = 0;

    /* Add a wftag struct linking origin to wfdisc... */
    wt = (struct wftagList *) dblCreateTableInstance(tree, dbl_LIST_WFTAG);
    strlcpy(wt->element->tagname, "evid",sizeof(wt->element->tagname));
    wt->element->tagid = orig->element->evid;
    wt->element->wfid = w->element->wfid;
    strlcpy(wt->element->lddate, w->element->lddate, sizeof(wt->element->lddate));
}

/* ------------------------------------------------------------------------- */

struct originList *
sacFindMatchingOriginByHeader(DBlist tree, struct SACheader *header,
                              double otime) {
    struct originList *orig = 0;

    /* Step through all origins looking for one that matches header and otime. */
    do {
        orig =
            (struct originList *) dblNextTableInstance(orig, tree,
                                                       dbl_LIST_ORIGIN);
        if (!orig)
            break;
        if (OriginsMatch(orig, header, otime))
            return orig;
    } while (orig);
    return orig;
}

/* ------------------------------------------------------------------------- */

static struct originList *
OriginByWfTagMatch(int wfid, DBlist tree) {
    int evid = 0, prefor = 0;
    struct wftagList *wt = 0;
    struct eventList *ev = 0;
    struct originList *orig = 0;

    /* First find by finding wftag element with proper wfid... */
    do {
        if (!
            (wt =
             (struct wftagList *) dblNextTableInstance(wt, tree,
                                                       dbl_LIST_WFTAG)))
            break;
        if (!strcmp(wt->element->tagname, "evid")) {
            if (wt->element->wfid == wfid) {
                evid = wt->element->tagid;
                break;
            }
        }
    } while (wt);
    if (!evid)
        return 0;

    /* Now get the eventList element using the evid... */
    do {
        ev = (struct eventList *) dblNextTableInstance((void *) ev, tree,
                                                       dbl_LIST_EVENT);
        if (!ev)
            break;
        if (ev->element->evid == evid) {
            prefor = ev->element->prefor;
            break;
        }
    } while (ev);
    if (!prefor)
        return 0;

    /* Now get the origin element from the prefor */
    do {
        orig =
            (struct originList *) dblNextTableInstance((void *) orig, tree,
                                                       dbl_LIST_ORIGIN);
        if (!orig)
            break;
        if (orig->element->orid == prefor)
            return orig;
    } while (orig);

    /* Nothing found. */
    return 0;
}

/* ---------------------------------------------------------------------------------- */

static struct assocList *
sacAddAssocStruct(DBlist tree, struct SACheader *header,
                  struct originList *orig, struct wfdiscList *w,
                  struct arrivalList *ar) {
    struct assocList *as;
    as = (struct assocList *) dblCreateTableInstance(tree, dbl_LIST_ASSOC);
    as->element->orid = orig->element->orid;
    as->element->arid = ar->element->arid;
    CSSstrcpy(as->element->phase, ar->element->iphase);
    CSSstrcpy(as->element->sta, header->kstnm);
    if (fltDefined(header->gcarc))
        as->element->delta = header->gcarc;
    if (fltDefined(header->baz))
        as->element->seaz = header->baz;
    if (fltDefined(header->az))
        as->element->esaz = header->az;
    CSSstrcpy(as->element->lddate, w->element->lddate);
    as->element->wfid = w->element->wfid;
    return as;
}

/* ------------------------------------------------------------------ */

static struct arrivalList *
MakeNewArrival(DBlist tree, struct SACheader *header, double refTime,
               struct wfdiscList *w, float pick, char *descrip, char *SACfield,
               struct originList *orig) {
    struct arrivalList *ar;
    struct wftagList *wt;

    ar = (struct arrivalList *) dblCreateTableInstance(tree, dbl_LIST_ARRIVAL);
    ar->element->arid = dblNextAvailableArid(tree);
    ar->element->time = refTime + pick;
    ar->element->jdate = w->element->jdate;
    ar->element->chanid = w->element->chanid;
    CSSstrcpy(ar->element->iphase, descrip);
    CSSstrcpy(ar->element->lddate, w->element->lddate);
    CSSstrcpy(ar->element->SACfield, SACfield);
    CSSstrcpy(ar->element->sta, header->kstnm);
    CSSstrcpy(ar->element->chan, header->kcmpnm);
    if (orig)
        sacAddAssocStruct(tree, header, orig, w, ar);

    /* Add a wftag struct linking arrival to wfdisc... */
    wt = (struct wftagList *) dblCreateTableInstance(tree, dbl_LIST_WFTAG);
    strlcpy(wt->element->tagname, "arid", sizeof(wt->element->tagname));
    wt->element->tagid = ar->element->arid;
    wt->element->wfid = w->element->wfid;
    strlcpy(wt->element->lddate, w->element->lddate, sizeof(wt->element->lddate));
    ar->element->wfid = w->element->wfid;

    return ar;
}


int
AridJoinsWithWfid(DBlist tree, int arid, int wfid) {
    struct wftagList *wt = 0;
    do {
        wt = (struct wftagList *) dblNextTableInstance(wt, tree,
                                                       dbl_LIST_WFTAG);
        if (!wt)
            break;
        if (!strcmp(wt->element->tagname, "arid"))
            if (wt->element->wfid == wfid && wt->element->tagid == arid)
                return 1;
    } while (wt);
    return 0;
}

/* ------------------------------------------------------------------------- */

struct wftagList *
FindConnectingWftag(DBlist tree, int arid, int wfid) {
    struct wftagList *wt = 0;
    do {
        wt = (struct wftagList *) dblNextTableInstance(wt, tree,
                                                       dbl_LIST_WFTAG);
        if (!wt)
            break;
        if (!strcmp(wt->element->tagname, "arid"))
            if (wt->element->wfid == wfid && wt->element->tagid == arid)
                return wt;
    } while (wt);
    return 0;
}

/* ------------------------------------------------------------------------- */

static struct arrivalList *
FindMatchingArrival(DBlist tree, struct SACheader *header, char *SACfield,
                    int wfid) {
    struct arrivalList *ar = 0;
    UNUSED(header);
    /* First try to match up using a wftag join... */
    do {
        ar = (struct arrivalList *) dblNextTableInstance(ar, tree,
                                                         dbl_LIST_ARRIVAL);
        if (!ar)
            break;
        if (!CSSstrcmp(ar->element->SACfield, SACfield))
            if (AridJoinsWithWfid(tree, ar->element->arid, wfid))
                return ar;
    } while (ar);
    return ar;
}

/* ------------------------------------------------------------------------- */

static struct assocList *
findAssoc(DBlist tree, int arid) {
    struct assocList *as = 0;
    do {
        as = (struct assocList *) dblNextTableInstance(as, tree,
                                                       dbl_LIST_ASSOC);
        if (!as)
            break;
        if (as->element->arid == arid)
            break;
    } while (as);

    return as;
}

/* ------------------------------------------------------------------------- */

static void
DeleteExistingArrival(DBlist tree, struct wfdiscList *w, char *SACfield) {
    struct arrivalList *ar = 0;
    struct wftagList *wt;
    struct assocList *as;
    do {
        ar = (struct arrivalList *) dblNextTableInstance(ar, tree,
                                                         dbl_LIST_ARRIVAL);
        if (!ar)
            break;
        if (!CSSstrcmp(ar->element->SACfield, SACfield)) {
            wt = FindConnectingWftag(tree, ar->element->arid, w->element->wfid);
            if (wt) {
                as = findAssoc(tree, ar->element->arid);
                dblDeleteTableInstance(dbl_LIST_ASSOC, tree, as);
                dblDeleteTableInstance(dbl_LIST_WFTAG, tree, wt);
                dblDeleteTableInstance(dbl_LIST_ARRIVAL, tree, ar);
                return;
            }
        }
    } while (ar);
    return;
}

/* ------------------------------------------------------------------------- */

static struct eventList *
FindMatchingEventStruct(DBlist tree, int evid) {
    struct eventList *ev = 0;
    do {
        ev = (struct eventList *) dblNextTableInstance(ev, tree,
                                                       dbl_LIST_EVENT);
        if (!ev)
            break;
        if (ev->element->evid == evid)
            break;
    } while (ev);

    return ev;
}

/* ------------------------------------------------------------------------- */

static struct wftagList *
FindMatchingWftagStruct(DBlist tree, char *tagname, int tagid, int wfid) {
    struct wftagList *wt = 0;
    do {
        wt = (struct wftagList *) dblNextTableInstance(wt, tree,
                                                       dbl_LIST_WFTAG);
        if (!wt)
            break;
        if (wt->element && tagname &&
            !strcmp(wt->element->tagname, tagname) &&
            wt->element->tagid == tagid && wt->element->wfid == wfid)
            break;
    } while (wt);

    return wt;
}

/* ------------------------------------------------------------------------- */

static struct arrivalList *
sacAddArrivalStruct(DBlist tree, struct SACheader *header, double refTime,
                    struct wfdiscList *w, float pick, char *descrip,
                    char *SACfield, struct originList *orig) {

    struct arrivalList *ar;

    /* First see if an arrival exists with a matching SACfield, and wftag join */
    ar = FindMatchingArrival(tree, header, SACfield, w->element->wfid);

    if (ar) {
        CSSstrcpy(ar->element->iphase, descrip);
        ar->element->time = refTime + pick;
    } else if (!ar)
        ar = MakeNewArrival(tree, header, refTime, w, pick, descrip, SACfield,
                            orig);
    return ar;
}

/* ------------------------------------------------------------------ */

static struct wfdiscList *
GetIndexedWfdiscListElement(DBlist tree, int index) {
    struct wfdiscList *w = 0;
    int j = 0;

    do {
        if (!
            (w =
             (struct wfdiscList *) dblNextTableInstance(w, tree,
                                                        dbl_LIST_WFDISC)))
            break;
        if (j++ == index) {
            dblAddComment(tree, dbl_LIST_WFDISC, w, "Updated from SAC");
            return w;
        }
    } while (w);
    return 0;
}

/* ------------------------------------------------------------------ */

static double
GetRefTime(struct SACheader *header) {
    int mon, day;
    float second;
    double time = 0.0;

    if (!lngDefined(header->nzjday) || !lngDefined(header->nzyear) ||
        !lngDefined(header->nzhour) || !lngDefined(header->nzmin) ||
        !lngDefined(header->nzsec) || !lngDefined(header->nzmsec))
        return time;

    mnday(header->nzjday, isleap(header->nzyear), &mon, &day);
    second = header->nzsec + ((float) header->nzmsec) / 1000.0;
    time =
        tmMakeEpochTime(header->nzyear, mon, day, header->nzhour, header->nzmin,
                        second);

    return time;
}

/* ------------------------------------------------------------------- */

static struct wfdiscList *
sacAddWfdiscStruct(DBlist tree, struct SACheader *header, sacSACdata * data,
                   int SkipData, int index) {
    struct wfdiscList *w = 0;
    struct wftagList *wt = 0;
    double RefTime;

    if (index >= 0)             /* This is supposed to be an update so find existing w. */
        w = GetIndexedWfdiscListElement(tree, index);

    if (!w) {                   /* Either its not an update or index was out of range. */
        w = (struct wfdiscList *) dblCreateTableInstance(tree, dbl_LIST_WFDISC);
        if (header->nwfid > 0) {
            if (dblWfidInUse(tree, header->nwfid))
                dblReplaceWfid(tree, header->nwfid, dblNextAvailableWfid(tree));
            w->element->wfid = header->nwfid;
        }

        else
            w->element->wfid = dblNextAvailableWfid(tree);

        w->element->chanid = dblNextAvailableChanid(tree);

        if (StrDefined(header->kstnm))
            strlcpy(w->element->sta, header->kstnm, sizeof(w->element->sta));
        else
            strlcpy(w->element->sta, "-",sizeof(w->element->sta));

        if (StrDefined(header->kcmpnm))
            strlcpy(w->element->chan, header->kcmpnm, sizeof(w->element->chan));
        else
            strlcpy(w->element->chan, "-", sizeof(w->element->chan));

        strlcpy(w->element->lddate, tmListEpochTime(tmGetEpochTime(), 18), sizeof(w->element->lddate));
        strlcpy(w->element->dattype, "t4", sizeof(w->element->dattype));
        dblAddComment(tree, dbl_LIST_WFDISC, w, "Converted from SAC format");
    }

    w->element->nsamp = header->npts;
    if (header->delta > 0.0)
        w->element->samprate = 1.0 / header->delta;
    else
        w->element->samprate = 1.0;

    RefTime = GetRefTime(header);
    if (RefTime) {
        int myear, mmonth, mday, mhour, mmin;
        float msecond;

        w->element->time = RefTime + header->b; /* start time of file */

        /* jdate */
        tmDecodeEpochTime(w->element->time, &myear, &mmonth, &mday, &mhour,
                          &mmin, &msecond);
        w->element->jdate = myear * 1000 + yrday(mmonth, mday, isleap(myear));
    } else {
        w->element->jdate = 1970001;
        w->element->time = header->b;
    }

    if (header->nevid > 0) {
        wt = 0;
        do {
            wt = FindNxtWftag(wt, tree, w->element->wfid);
        } while (wt && strcmp(wt->element->tagname, "evid"));
        if (wt) {
            dblReplaceEvid(tree, wt->element->tagid, header->nevid);
        } else {
            wt = 0;
            wt = (struct wftagList *) dblCreateTableInstance(tree,
                                                             dbl_LIST_WFTAG);
            strlcpy(wt->element->tagname, "evid", sizeof(wt->element->tagname));
            wt->element->tagid = header->nevid;
            wt->element->wfid = w->element->wfid;
            strlcpy(wt->element->lddate, w->element->lddate, sizeof(wt->element->lddate));
        }
    }

    w->element->endtime =
        w->element->time + (w->element->nsamp) * header->delta;

    w->element->calib = CALIB_UNDEF;

    if (StrDefined(header->kinst))
        strlcpy(w->element->instype, header->kinst,sizeof(w->element->instype));

    if (!SkipData) {
        w->seis->Cmplx = 0;
        if (w->seis->r) {       /* This may be an update so w->seis->r may be allocated but wrong size. */
            smFree(w->seis->r);
            w->seis->r = 0;
        }
        if (w->seis->i) {       /* This may be an update so w->seis->i may be allocated but wrong size. */
            smFree(w->seis->i);
            w->seis->i = 0;
        }
        if (data->xarray) {
            w->seis->r = (float *) smMalloc(w->element->nsamp * sizeof(float));
            if (!w->seis->r) {
                printf("Error allocating float array in sacAddWfdiscStruct!\n");
                return w;
            }
            memcpy(w->seis->r, data->xarray, w->element->nsamp * sizeof(float));
        }
        if (data->yarray) {
            w->seis->i = (float *) smMalloc(w->element->nsamp * sizeof(float));
            if (!w->seis->i) {
                printf("Error allocating float array in sacAddWfdiscStruct!\n");
                return w;
            }
            memcpy(w->seis->i, data->yarray, w->element->nsamp * sizeof(float));
        }
        if (data->xarray && data->yarray)
            w->seis->Cmplx = 1;
    }

    return w;
}

/* ------------------------------------------------------------------ */

static int
SitesMatch(struct siteList *si, struct SACheader *header) {
    if (!si)
        return 0;
    if (FloatsDiffer9(si->element->lat, header->stla))
        return 0;
    if (FloatsDiffer9(si->element->lon, header->stlo))
        return 0;
    if (fltDefined(header->stel)) {
        if (FloatsDiffer9(si->element->elev, header->stel / 1000))
            return 0;
        else
            return 1;
    } else if (FloatsDiffer9(si->element->elev, header->stel))
        return 0;

    return 1;
}

/* ------------------------------------------------------------------ */

static int
SiteChansMatch(struct sitechanList *sc, struct SACheader *header) {
    if (!sc)
        return 0;
    if (FloatsDiffer0(sc->element->hang, header->cmpaz))
        return 0;
    if (FloatsDiffer0(sc->element->vang, header->cmpinc))
        return 0;
    if (fltDefined(header->stdp)) {
        if (FloatsDiffer0(sc->element->edepth, header->stdp / 1000))
            return 0;
        else
            return 1;
    } else if (FloatsDiffer0(sc->element->edepth, header->stdp))
        return 0;

    return 1;
}

/* ------------------------------------------------------------------ */

static struct siteList *
sacAddSiteStruct(DBlist tree, char *name, struct SACheader *header, int jdate) {
    struct siteList *si;
    UNUSED(jdate);
    si = (struct siteList *) dblCreateTableInstance(tree, dbl_LIST_SITE);
    CSSstrcpy(si->element->sta, name);
    if (fltDefined(header->stla))
        si->element->lat = header->stla;
    if (fltDefined(header->stlo))
        si->element->lon = header->stlo;
    if (fltDefined(header->stel))
        si->element->elev = header->stel / 1000.0;
    si->element->ondate = LONGAGO;      /* make site active from pre-seismography  */
    strlcpy(si->element->lddate, tmListEpochTime(tmGetEpochTime(), 18),sizeof(si->element->lddate));

    return si;
}

/* ------------------------------------------------------------------ */

static void
UpdateSiteStruct(DBlist tree, char *name, struct SACheader *header) {

    struct siteList *si = 0;
    do {
        si = (struct siteList *) dblNextTableInstance(si, tree, dbl_LIST_SITE);
        if (!si)
            break;
        if (!CSSstrcmp(si->element->sta, name)) {
            if (fltDefined(header->stla))
                si->element->lat = header->stla;
            if (fltDefined(header->stlo))
                si->element->lon = header->stlo;
            if (fltDefined(header->stel))
                si->element->elev = header->stel / 1000.0;
        }
    } while (si);
}

/* ------------------------------------------------------------------ */

static void
UpdateSitechanStruct(DBlist tree, struct wfdiscList *w,
                     struct SACheader *header) {

    struct sitechanList *sc = 0;
    do {
        sc = (struct sitechanList *) dblNextTableInstance(sc, tree,
                                                          dbl_LIST_SITECHAN);
        if (!sc)
            break;
        if (!CSSstrcmp(sc->element->sta, w->element->sta))
            if (!CSSstrcmp(sc->element->chan, w->element->chan)) {
                if (fltDefined(header->cmpinc))
                    sc->element->vang = header->cmpinc;
                if (fltDefined(header->cmpaz))
                    sc->element->hang = header->cmpaz;
                if (fltDefined(header->stdp))
                    sc->element->edepth = header->stdp / 1000.0;
            }
    } while (sc);
}

/* ------------------------------------------------------------------ */

static struct affiliationList *
AddAffilStruct(DBlist tree, struct wfdiscList *w, char *net) {
    struct affiliationList *af;

    af = (struct affiliationList *) dblCreateTableInstance(tree,
                                                           dbl_LIST_AFFILIATION);
    CSSstrcpy(af->element->net, net);
    CSSstrcpy(af->element->sta, w->element->sta);
    strlcpy(af->element->lddate, w->element->lddate,sizeof(af->element->lddate));
    return af;
}

/* ------------------------------------------------------------------ */

struct sacdataList *
findSacData(DBlist tree, int wfid) {
    struct sacdataList *sd = 0;
    do {
        sd = (struct sacdataList *) dblNextTableInstance(sd, tree,
                                                         dbl_LIST_SACDATA);
        if (!sd)
            break;
        if (sd->element->wfid == wfid)
            return sd;
    } while (sd);
    return 0;
}

/* ------------------------------------------------------------------ */

static void
sacAddSacdataStruct(DBlist tree, struct SACheader *header, int wfid) {
    struct sacdataList *sd = findSacData(tree, wfid);
    if (!sd) {
        sd = (struct sacdataList *) dblCreateTableInstance(tree,
                                                           dbl_LIST_SACDATA);
        sd->element->wfid = wfid;
    }
    strlcpy(sd->element->userdata.label[0], header->kuser0, sizeof(sd->element->userdata.label[0]));
    strlcpy(sd->element->userdata.label[1], header->kuser1, sizeof(sd->element->userdata.label[1]));
    strlcpy(sd->element->userdata.label[2], header->kuser2, sizeof(sd->element->userdata.label[2]));

    sd->element->userdata.value[0] = header->user0;
    sd->element->userdata.value[1] = header->user1;
    sd->element->userdata.value[2] = header->user2;
    sd->element->userdata.value[3] = header->user3;
    sd->element->userdata.value[4] = header->user4;
    sd->element->userdata.value[5] = header->user5;
    sd->element->userdata.value[6] = header->user6;
    sd->element->userdata.value[7] = header->user7;
    sd->element->userdata.value[8] = header->user8;
    sd->element->userdata.value[9] = header->user9;

    sd->element->synthflag = header->isynth;
    sd->element->lpspol = header->lpspol;
    sd->element->iztype = header->iztype;
    sd->element->idep = header->idep;
    sd->element->iftype = header->iftype;
    sd->element->nsnpts = header->nsnpts;
    sd->element->nxsize = header->nxsize;
    sd->element->nysize = header->nysize;
    sd->element->leven = header->leven;
    sd->element->fmt = header->fmt;
    sd->element->sb = header->sb;
    sd->element->sdelta = header->sdelta;
    sd->element->xminimum = header->xminimum;
    sd->element->xmaximum = header->xmaximum;
    sd->element->yminimum = header->yminimum;
    sd->element->ymaximum = header->ymaximum;
    sd->element->odelta = header->odelta;
    sd->element->resp[0] = header->resp0;
    sd->element->resp[1] = header->resp1;
    sd->element->resp[2] = header->resp2;
    sd->element->resp[3] = header->resp3;
    sd->element->resp[4] = header->resp4;
    sd->element->resp[5] = header->resp5;
    sd->element->resp[6] = header->resp6;
    sd->element->resp[7] = header->resp7;
    sd->element->resp[8] = header->resp8;
    sd->element->resp[9] = header->resp9;
    sd->element->evel = header->evel;
    sd->element->az = header->az;
    sd->element->baz = header->baz;
    sd->element->gcarc = header->gcarc;
    sd->element->dist = header->dist;
    sd->element->iinst = header->iinst;
    if (lngDefined(header->istreg))
        sd->element->istreg = header->istreg - REGCONV;
    sd->element->iqual = header->iqual;
    sd->element->lovrok = header->lovrok;
    sd->element->lcalda = header->lcalda;
    strlcpy(sd->element->khole, header->khole, sizeof(sd->element->khole));
    strlcpy(sd->element->ko, header->ko, sizeof(sd->element->ko));
    strlcpy(sd->element->kdatrd, header->kdatrd,sizeof(sd->element->kdatrd));

}

/* ------------------------------------------------------------------ */

static void
UpdateMagInfoInOrigin(struct originList *orig, struct SACheader *header) {
    orig->element->mb = -999.0;
    orig->element->ms = -999.0;
    orig->element->ml = -999.0;
    strlcpy(orig->element->auth, "-", sizeof(orig->element->auth));

    if (fltDefined(header->mag)) {
        if (header->imagtyp) {
            switch (header->imagtyp) {
                case IMB:
                    orig->element->mb = header->mag;
                    break;
                case IMS:
                    orig->element->ms = header->mag;
                    break;
                case IML:
                    orig->element->ml = header->mag;
                    break;
                default:
                    orig->element->mb = header->mag;
            }
        } else
            orig->element->mb = header->mag;
    }

    if (lngDefined(header->imagsrc)) {
        switch (header->imagsrc) {
            case INEIC:
                strlcpy(orig->element->auth, "NEIC",sizeof(orig->element->auth));
                break;
            case IPDE:
                strlcpy(orig->element->auth, "PDE",sizeof(orig->element->auth));
                break;
            case IPDEQ:
                strlcpy(orig->element->auth, "PDE-Q",sizeof(orig->element->auth));
                break;
            case IPDEW:
                strlcpy(orig->element->auth, "PDE-W",sizeof(orig->element->auth));
                break;
            case IISC:
                strlcpy(orig->element->auth, "ISC",sizeof(orig->element->auth));
                break;
            case IREB:
                strlcpy(orig->element->auth, "REB",sizeof(orig->element->auth));
                break;
            case IUSGS:
                strlcpy(orig->element->auth, "USGS",sizeof(orig->element->auth));
                break;
            case IBRK:
                strlcpy(orig->element->auth, "BRK",sizeof(orig->element->auth));
                break;
            case ICALTECH:
                strlcpy(orig->element->auth, "CALTECH",sizeof(orig->element->auth));
                break;
            case ILLNL:
                strlcpy(orig->element->auth, "LLNL",sizeof(orig->element->auth));
                break;
            case IEVLOC:
                strlcpy(orig->element->auth, "EVLOC",sizeof(orig->element->auth));
                break;
            case IJSOP:
                strlcpy(orig->element->auth, "JSOP",sizeof(orig->element->auth));
                break;
            case IUSER:
                strlcpy(orig->element->auth, "USER",sizeof(orig->element->auth));
                break;
            case IUNKNOWN:
                strlcpy(orig->element->auth, "UNKNOWN",sizeof(orig->element->auth));
                break;
        }
    }
}

/* ------------------------------------------------------------------ */

static void
CreateUpdateOrigin(DBlist tree, struct originList *OldOr,
                   struct SACheader *header, struct wfdiscList *w,
                   double otime) {
    struct originList *orig;
    struct wftagList *wt = 0;
    struct eventList *ev, *ev2;
    int wftagFound = 0;
    struct assocList *as = 0;
    int OldOrid = OldOr->element->orid;

    if (!fltDefined(header->evla) && !fltDefined(header->evlo) &&
        !fltDefined(header->evdp) && !fltDefined(header->mag) &&
        !dblDefined(otime))
        return;

    orig = (struct originList *) dblCreateTableInstance(tree, dbl_LIST_ORIGIN);
    *(orig->element) = *(OldOr->element);
    //dblCopyTable(dbl_LIST_ORIGIN, OldOr, orig);
    if (fltDefined(header->evla))
        orig->element->lat = header->evla;
    if (fltDefined(header->evlo))
        orig->element->lon = header->evlo;
    if (fltDefined(header->evdp))
        orig->element->depth = header->evdp / 1000.0;
    if (dblDefined(otime))
        orig->element->time = otime;
    UpdateMagInfoInOrigin(orig, header);

    ev = FindMatchingEventStruct(tree, orig->element->evid);
    orig->element->evid = dblNextAvailableEvid(tree);
    orig->element->orid = dblNextAvailableOrid(tree);
    orig->element->wfid = w->element->wfid;

    ev2 = (struct eventList *) dblCreateTableInstance(tree, dbl_LIST_EVENT);
    if (ev) {
        *(ev2->element) = *(ev->element);
        //dblCopyTable(dbl_LIST_EVENT, ev, ev2);
    } else {
        strlcpy(ev2->element->auth, orig->element->auth, sizeof(ev2->element->auth));
        strlcpy(ev2->element->lddate, orig->element->lddate, sizeof(ev2->element->lddate));
        ev2->element->commid = orig->element->commid;
    }
    ev2->element->evid = orig->element->evid;
    ev2->element->prefor = orig->element->orid;
    if (strcmp(header->kevnm, SAC_CHAR_UNDEFINED)) {
        strlcpy(ev2->element->evname, header->kevnm, sizeof(ev2->element->evname));
    }
    ev2->element->wfid = w->element->wfid;

    /* Now find wftag element with proper wfid and reset its evid... */
    do {
        if (!
            (wt =
             (struct wftagList *) dblNextTableInstance(wt, tree,
                                                       dbl_LIST_WFTAG)))
            break;
        if (!strcmp(wt->element->tagname, "evid"))
            if (wt->element->wfid == w->element->wfid) {
                dblReplaceEvid(tree, wt->element->tagid, orig->element->evid);
                wftagFound = 1;
                break;
            }
    } while (wt);

    if (!wftagFound) {

        /* Make a new wftag struct and associate new origin with the wfdisc. */
        wt = (struct wftagList *) dblCreateTableInstance(tree, dbl_LIST_WFTAG);
        strlcpy(wt->element->tagname, "evid", sizeof(wt->element->tagname));
        wt->element->tagid = orig->element->evid;

        wt->element->wfid = w->element->wfid;
        strlcpy(wt->element->lddate, w->element->lddate,sizeof(wt->element->lddate));
    }

    do {
        as = (struct assocList *) dblNextTableInstance(as, tree,
                                                       dbl_LIST_ASSOC);
        if (!as)
            break;
        if (as->element->orid == OldOrid)
            as->element->orid = orig->element->orid;
    } while (as);

}

/* ------------------------------------------------------------------ */

static void
CreateUpdateEvent(DBlist tree, struct originList *orig,
                  struct SACheader *header) {
    struct eventList *ev = 0;

    ev = FindMatchingEventStruct(tree, orig->element->evid);

    if (ev)
        if (strcmp(header->kevnm, SAC_CHAR_UNDEFINED)) {
            strlcpy(ev->element->evname, header->kevnm, sizeof(ev->element->evname));
        }
}

static struct originList *
sacAddOriginStruct(DBlist tree, struct SACheader *header, struct wfdiscList *w,
                   double otime, int takeEvid) {
    struct originList *orig = 0;
    struct wftagList *wt = 0;
    struct eventList *ev = 0;
    int haveEvent = FALSE;

    if (!fltDefined(header->evla) && !fltDefined(header->evlo) &&
        !fltDefined(header->evdp) && !fltDefined(header->mag) &&
        !dblDefined(otime))
        return 0;

    if (header->nevid > 0 && takeEvid && (ev = FindEvent(tree, header->nevid))) {
        haveEvent = TRUE;
    } else {
        haveEvent = FALSE;
    }

    if (header->norid > 0 && takeEvid &&
        (orig = FindOrigin(tree, header->norid))) {

        if (!haveEvent) {
            /* do event */
            ev = (struct eventList *) dblCreateTableInstance(tree,
                                                             dbl_LIST_EVENT);
            ev->element->evid = orig->element->evid;
            ev->element->prefor = orig->element->orid;
            if (StrDefined(header->kevnm)) {
                strlcpy(ev->element->evname, header->kevnm, sizeof(ev->element->evname));
            }
            strlcpy(ev->element->lddate, orig->element->lddate, sizeof(ev->element->lddate));
            strlcpy(ev->element->auth, orig->element->auth, sizeof(ev->element->auth));
        }
    } else {

        if (haveEvent) {
            /* do origin */
            int myear, mmonth, mday, mhour, mmin;
            float msecond;

            orig =
                (struct originList *) dblCreateTableInstance(tree,
                                                             dbl_LIST_ORIGIN);
            if (fltDefined(header->evla))
                orig->element->lat = header->evla;
            if (fltDefined(header->evlo))
                orig->element->lon = header->evlo;
            if (fltDefined(header->evdp))
                orig->element->depth = header->evdp / 1000.0;
            if (dblDefined(otime))
                orig->element->time = otime;
            UpdateMagInfoInOrigin(orig, header);

            orig->element->evid = ev->element->evid;
            orig->element->orid = ev->element->prefor;
            orig->element->wfid = w->element->wfid;

            /* jdate */
            tmDecodeEpochTime(otime, &myear, &mmonth, &mday, &mhour, &mmin,
                              &msecond);
            orig->element->jdate =
                myear * 1000 + yrday(mmonth, mday, isleap(myear));

            if (lngDefined(header->ievreg))
                orig->element->grn = header->ievreg - REGCONV;
            if (lngDefined(header->ievtyp)) {
                strlcpy(orig->element->etype, sacSetEtype(header->ievtyp), sizeof(orig->element->etype));
            }
            strlcpy(orig->element->lddate, w->element->lddate, sizeof(orig->element->lddate));
        } else {
            int evid, orid;
            /* get next evid and orid */
            evid = dblNextAvailableEvid(tree);
            orid = dblNextAvailableOrid(tree);

            /* do event and origin */
            ev = (struct eventList *) dblCreateTableInstance(tree,
                                                             dbl_LIST_EVENT);
            ev->element->evid = evid;
            ev->element->prefor = orid;
            if (StrDefined(header->kevnm)) {
                strlcpy(ev->element->evname, header->kevnm, sizeof(ev->element->evname));
            }
            strlcpy(ev->element->lddate, w->element->lddate, sizeof(ev->element->lddate));

            orig =
                (struct originList *) dblCreateTableInstance(tree,
                                                             dbl_LIST_ORIGIN);
            if (fltDefined(header->evla))
                orig->element->lat = header->evla;
            if (fltDefined(header->evlo))
                orig->element->lon = header->evlo;
            if (fltDefined(header->evdp))
                orig->element->depth = header->evdp / 1000.0;
            if (dblDefined(otime))
                orig->element->time = otime;
            UpdateMagInfoInOrigin(orig, header);

            orig->element->evid = evid;
            orig->element->orid = orid;
            orig->element->wfid = w->element->wfid;

            orig->element->jdate = w->element->jdate;
            if (lngDefined(header->ievreg))
                orig->element->grn = header->ievreg - REGCONV;
            if (lngDefined(header->ievtyp)) {
                strlcpy(orig->element->etype, sacSetEtype(header->ievtyp), sizeof(orig->element->etype));
            }
            strlcpy(orig->element->lddate, w->element->lddate, sizeof(orig->element->lddate));
        }
    }

    /* Check wftag structs linking event to wfdisc... */
    if (!
        (wt =
         FindMatchingWftagStruct(tree, "evid", ev->element->evid,
                                 w->element->wfid))) {
        wt = (struct wftagList *) dblCreateTableInstance(tree, dbl_LIST_WFTAG);
        strlcpy(wt->element->tagname, "evid", sizeof(wt->element->tagname));
        wt->element->tagid = ev->element->evid;
        wt->element->wfid = w->element->wfid;
        strlcpy(wt->element->lddate, w->element->lddate, sizeof(wt->element->lddate));
    }

    return orig;
}

/* ------------------------------------------------------------------ */

static struct eventList *
FindEvent(DBlist tree, int evid) {
    struct eventList *ev = 0;
    do {
        ev = (struct eventList *) dblNextTableInstance(ev, tree,
                                                       dbl_LIST_EVENT);
        if (!ev)
            break;
        if (ev->element->evid == evid)
            return ev;
    } while (ev);
    return 0;
}

/* ------------------------------------------------------------------ */

static struct originList *
FindOrigin(DBlist tree, int orid) {
    struct originList *orig = 0;
    do {
        orig =
            (struct originList *) dblNextTableInstance(orig, tree,
                                                       dbl_LIST_ORIGIN);
        if (!orig)
            break;
        if (orig->element->orid == orid)
            return orig;
    } while (orig);
    return 0;
}

/* ------------------------------------------------------------------ */

static struct originList *
FindOriginByEvid(DBlist tree, int evid) {
    struct originList *orig = 0;
    do {
        orig =
            (struct originList *) dblNextTableInstance(orig, tree,
                                                       dbl_LIST_ORIGIN);
        if (!orig)
            break;
        if (orig->element->evid == evid)
            return orig;
    } while (orig);
    return 0;
}


static struct wftagList *
FindNxtWftag(struct wftagList *wt, DBlist tree, int wfid) {
    /* struct wftagList *wt = 0; */
    do {
        wt = (struct wftagList *) dblNextTableInstance(wt, tree,
                                                       dbl_LIST_WFTAG);
        if (!wt)
            break;
        if (wt->element->wfid == wfid)
            return wt;
    } while (wt);
    return 0;
}

static struct eventList *
sacAddEventStruct(DBlist tree, struct SACheader *header, struct wfdiscList *w,
                  int evid) {
    struct eventList *ev = 0;
    struct wftagList *wt = 0;

    do {
        wt = dblNextTableInstance(wt, tree, dbl_LIST_WFTAG);
        if (!wt)
            break;

        if (wt->element->wfid == w->element->wfid &&
            !strcmp(wt->element->tagname, "evid"))
            break;
    } while (wt);

    if (wt)
        ev = FindEvent(tree, wt->element->tagid);

    if (ev)
        return ev;

    ev = (struct eventList *) dblCreateTableInstance(tree, dbl_LIST_EVENT);
    if (evid < 0)
        ev->element->evid = dblNextAvailableEvid(tree);
    else
        ev->element->evid = evid;
    if (strcmp(header->kevnm, SAC_CHAR_UNDEFINED)) {
        strncpy(ev->element->evname, header->kevnm, 15);
    }
    strlcpy(ev->element->lddate, w->element->lddate, sizeof(ev->element->lddate));

    if (!wt) {
        wt = (struct wftagList *) dblCreateTableInstance(tree, dbl_LIST_WFTAG);
        wt->element->wfid = w->element->wfid;
        strlcpy(wt->element->tagname, "evid", sizeof(wt->element->tagname));
        wt->element->tagid = ev->element->evid;
        strlcpy(wt->element->lddate, w->element->lddate, sizeof(wt->element->lddate));
    }
    return ev;
}


static void
DeBlankSacHeaderStrings(struct SACheader *header) {

    DeBlank(header->kstnm);
    DeBlank(header->kevnm);
    DeBlank(header->khole);
    DeBlank(header->ko);
    DeBlank(header->ka);
    DeBlank(header->kt0);
    DeBlank(header->kt1);
    DeBlank(header->kt2);
    DeBlank(header->kt3);
    DeBlank(header->kstnm);
    DeBlank(header->kt4);
    DeBlank(header->kt5);
    DeBlank(header->kt6);
    DeBlank(header->kt7);
    DeBlank(header->kt8);
    DeBlank(header->kt9);
    DeBlank(header->kf);
    DeBlank(header->kuser0);
    DeBlank(header->kuser1);
    DeBlank(header->kuser2);
    DeBlank(header->kcmpnm);
    DeBlank(header->knetwk);
    DeBlank(header->kdatrd);
    DeBlank(header->kinst);
}

/* ------------------------------------------------------------------ */

/*If index >= 0 then the function will find the wfdiscList struct whose place in the list
  is given by index, and update that struct and its connected CSS structs of other types.
  Otherwise, the function will create new structs and add them to the list.
*/

int
sacLoadFromHeaderAndData(struct SACheader *header, sacSACdata * data,
                         char *WorkSetName, int Replace, int index,
                         int UpdateData, int takeEvid) {
    DBlist tree;
    struct wfdiscList *w;
    struct wftagList *wt = 0;
    struct siteList *si = 0;
    struct eventList *ev = 0;
    struct originList *orig = 0;
    struct affiliationList *af = 0;
    struct sitechanList *sc = 0;

    double otime;
    double RefTime;
    int j;
    float pick;
    char *descrip;
    int SkipData = 0;
    char SACfield[3];
    char TmpStaName[9];
    char TmpChanName[9];
    char sacErrorStrg[300];

    memset(TmpStaName, 0, 9);
    memset(TmpChanName, 0, 9);

    if (!WorkSetName || !strlen(WorkSetName)) {
        printf("Invalid worksetname! Cannot add SAC data to workset.\n");
        return 0;
    }

    if (Replace) {
        smDeleteWorksetByName(WorkSetName);
        smCreateEmptyWorkset(WorkSetName);      /* This workset is now the default. */
    } else if (!smChangeDefaultWorksetByName(WorkSetName))
        smCreateEmptyWorkset(WorkSetName);      /* This workset is now the default. */

    tree = smGetDefaultTree();
    if (!tree)
        tree = smMakeDefaultTree();
    if (!data || !UpdateData)
        SkipData = 1;
    else {
        if (data) {
            if (data->dataType != ITIME && !data->xarray) {
                /* error */
                strlcpy(sacErrorStrg, "Missing data array for complex data\n",sizeof(sacErrorStrg));
                dblSetError(0, sacErrorStrg);
                return 0;
            } else if (data->dataType == ITIME && data->xarray) {
                /* warning */
                strlcpy(sacErrorStrg, "Unexpected data array found. \n",sizeof(sacErrorStrg));
                dblSetError(0, sacErrorStrg);
            }
        }
    }

    DeBlankSacHeaderStrings(header);
    if (!strcmp(header->kstnm, "-12345")) {
        strlcpy(header->kstnm, MakeUniqueSiteName(tree, "STA"), sizeof(header->kstnm));
    }
    if (!strcmp(header->kcmpnm, "-12345")) {
        strlcpy(header->kcmpnm, MakeUniqueSiteName(tree, "CHAN"), sizeof(header->kcmpnm));
    }
    CSSstrcpy(TmpStaName, header->kstnm);
    /* Create or update a wfdisc from header and data. */
    w = sacAddWfdiscStruct(tree, header, data, SkipData, index);        /* index is position in list. */
    if (CSSstrcmp(w->element->sta, header->kstnm)) {    /* There's been a name change. */
        strlcpy(TmpStaName, MakeUniqueSiteName(tree, header->kstnm),sizeof(TmpStaName));
        MakeNameUpdateStructCopies(tree, w, TmpStaName, header->knetwk);
        /* everything linked by name must get new row. which is copy of old but with new name */
    }
    if (CSSstrcmp(w->element->chan, header->kcmpnm)) {
        strlcpy(TmpChanName,
                MakeUniqueChanName(tree, w->element->sta, header->kcmpnm),
                sizeof(TmpChanName));
        MakeChanUpdateStructCopies(tree, w, TmpChanName);
        /* everything linked by name and chan must get new row. which is copy of old but with new chan */
    }
    sacAddSacdataStruct(tree, header, w->element->wfid);        /* Stuff that doesn't fit in CSS. */

    /* Now see if there is a site struct matching data. If not, create and populate. */
    if (siteNeeded(header)) {
        si = findSite(tree, w->element->sta, w->element->jdate);
        if (!si)
            si = sacAddSiteStruct(tree, w->element->sta, header,
                                  w->element->jdate);
        else {
            if (!SitesMatch(si, header)) {      /* check non-key elements */
                strlcpy(TmpStaName, MakeUniqueSiteName(tree, si->element->sta), sizeof(TmpStaName));
                MakeNameUpdateStructCopies(tree, w, TmpStaName, header->knetwk);
                UpdateSiteStruct(tree, w->element->sta, header);
            }
        }
    }

    /* Now see if there is a sitechan matching data. If not create and populate. */
    if (sitechanNeeded(header)) {
        sc = FindSiteChan(tree, w->element->sta, w->element->chan,
                          w->element->jdate);
        if (!sc)
            sc = MakeSiteChan(tree, w->element->sta, w->element->chan,
                              header->cmpinc, header->stdp, header->cmpaz,
                              w->element->jdate);
        else {
            if (!SiteChansMatch(sc, header)) {  /* check non-key elements */
                strlcpy(TmpChanName,
                       MakeUniqueChanName(tree, sc->element->sta,
                                          sc->element->chan),
                        sizeof(TmpChanName));
                MakeChanUpdateStructCopies(tree, w, TmpChanName);
                UpdateSitechanStruct(tree, w, header);
            }
        }
    }

/* Now create an affiliation structList element if KNETWK is defined. */

    af = findAffiliation(tree, w->element->sta, header->knetwk);
    if (!af && strcmp(header->knetwk, SAC_CHAR_UNDEFINED)) {
        af = AddAffilStruct(tree, w, header->knetwk);
    }

/* Now see if there is an origin struct matching data. If not create and populate. */
    RefTime = GetRefTime(header);

    if (fltDefined(header->o))  /* Can this ever be undefined? */
        otime = RefTime + header->o;
    else
        otime = RefTime;        /* Not really right, must be accompanied by warning from chnhdr. */

    if (originNeeded(header)) {
        orig = OriginByWfTagMatch(w->element->wfid, tree);
        if (!orig) {
            orig = sacFindMatchingOriginByHeader(tree, header, otime);
            if (orig) {
                sacAddWftagStruct(tree, orig, w);
            } else {
                orig = sacAddOriginStruct(tree, header, w, otime, takeEvid);
                ev = FindEvent(tree, orig->element->evid);
                if (!ev)
                    ev = sacAddEventStruct(tree, header, w,
                                           orig->element->evid);
                if (ev->element->prefor < 1)
                    ev->element->prefor = orig->element->orid;
            }
        } else {
            /*UpDateEventStruct(tree, header, w ); */
            if (!OriginsMatch(orig, header, otime))
                /*Create new origin retaining unchanged CSS data, and reset wftag pointer. */
                CreateUpdateOrigin(tree, orig, header, w, otime);
            else {
                CreateUpdateEvent(tree, orig, header);
                sacAddWftagStruct(tree, orig, w);
            }
        }
    } else {
        /* disconnect linked origins. */
        do {
            if (!
                (wt =
                 (struct wftagList *) dblNextTableInstance(wt, tree,
                                                           dbl_LIST_WFTAG)))
                break;
            if (wt->element->wfid == w->element->wfid) {
                if (!strcmp(wt->element->tagname, "evid")) {
                    do {
                        if (!
                            (ev =
                             (struct eventList *) dblNextTableInstance(ev, tree,
                                                                       dbl_LIST_EVENT)))
                            break;
                        orig = FindOriginByEvid(tree, ev->element->evid);
                        if (!orig)
                            break;
                        if (wt->element->tagid == ev->element->evid &&
                            ev->element->prefor == orig->element->orid) {
                            if (eventNeeded(header)) {
                                ev->element->prefor = -1;
                                orig->element->evid = -1;
                            } else {
                                dblDeleteTableInstance(dbl_LIST_WFTAG, tree,
                                                       wt);
                            }
                            break;
                        }
                    } while (ev);
                }
            }
        } while (wt);

        if (eventNeeded(header) && !ev) {
            ev = sacAddEventStruct(tree, header, w, -1);
        }
    }

    /* kluge to get prefor set without actually fixing the w->element->evid 
       thing above (wfdiscs aren't supposed to have evids anymore).  */
    if (orig && ev && ev->element->prefor < 0)
        ev->element->prefor = orig->element->orid;

/* Now add arrival structs for any of A, T0 - T9 which are defined and  */
/* which have a description as well in KA KT0 - KT9 */
    if (fltDefined(header->a)) {
        sacAddArrivalStruct(tree, header, RefTime, w, header->a, header->ka,
                            "A", orig);
    } else
        DeleteExistingArrival(tree, w, "A");

    for (j = 0; j < 10; j++) {
        pick = *(&(header->t0) + j);
        descrip = (header->kt0 + 9 * j);
        sprintf(SACfield, "T%d", j);
        SACfield[2] = '\0';
        if (fltDefined(pick))
            sacAddArrivalStruct(tree, header, RefTime, w, pick, descrip,
                                SACfield, orig);
        else
            DeleteExistingArrival(tree, w, SACfield);
    }

    if (fltDefined(header->f)) {
        if (strcmp(header->kf, SAC_CHAR_UNDEFINED))
            sacAddArrivalStruct(tree, header, RefTime, w, header->f, header->kf,
                                "F", orig);
        else
            sacAddArrivalStruct(tree, header, RefTime, w, header->f, "F", "F",
                                orig);
    } else
        DeleteExistingArrival(tree, w, "F");

    return 1;

}

/* ------------------------------------------------------------------ */

int
originNeeded(struct SACheader *header) {
    if (fltDefined(header->evla) || fltDefined(header->evlo) ||
        fltDefined(header->evdp) || fltDefined(header->o) ||
        header->iztype == IO || lngDefined(header->ievreg) ||
        lngDefined(header->ievtyp) || fltDefined(header->mag) ||
        lngDefined(header->imagsrc))
        return TRUE;

    return FALSE;
}

int
eventNeeded(struct SACheader *header) {
    if (originNeeded(header))
        return TRUE;

    if (header->kevnm[0] == '\0')
        return FALSE;

    if (StrDefined(header->kevnm))
        return TRUE;

    return FALSE;
}

int
siteNeeded(struct SACheader *header) {
    if (fltDefined(header->stla) || fltDefined(header->stlo) ||
        fltDefined(header->stel))
        return TRUE;

    return FALSE;
}

int
sitechanNeeded(struct SACheader *header) {
    if (fltDefined(header->stdp) || fltDefined(header->cmpaz) ||
        fltDefined(header->cmpinc))
        return TRUE;

    return FALSE;
}
