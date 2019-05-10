
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "amf.h"
#include "gdm.h"

#include "bool.h"
#include "co.h"
#include "bot.h"

#include "gd2.h"

#ifdef X11_APP
#include "gd3.x11.h"
#endif
#include "string/array.h"
#include "string_utils.h"

#include "debug.h"

#ifdef OSX_APP
void initdevice_osx();
#endif
#ifdef WIN32
void initdevice_win();
#endif

static display_t **gdm_devices;
static int gdm_size;
static int gdm_alloc;

print_device_begin_t print_device_begin;
print_device_end_t print_device_end;

struct t_kmgdm kmgdm;
struct t_cmgdm cmgdm;


short *const Ascstr = &cmgdm.ascstr[0] - 1;
int *const Iflhc = &cmgdm.iflhc[0] - 1;
int *const Igdtyp = &cmgdm.igdtyp[0] - 1;
int *const Lgdon = &cmgdm.lgdon[0] - 1;
short *const Stroke = &cmgdm.stroke[0] - 1;
short *const Stxmax = &cmgdm.stxmax[0] - 1;
short *const Stxmin = &cmgdm.stxmin[0] - 1;
double *const Xvs = &cmgdm.xvs[0] - 1;
float *const Xwindowmax = &cmgdm.xwindowmax[0] - 1;
float *const Xwindowmin = &cmgdm.xwindowmin[0] - 1;
double *const Yvs = &cmgdm.yvs[0] - 1;
float *const Ywindowmax = &cmgdm.ywindowmax[0] - 1;
float *const Ywindowmin = &cmgdm.ywindowmin[0] - 1;

int    window_use_ratio[MWINDOWS+1];
float  window_ratio[MWINDOWS+1]; // ASPECT RATIO

#define BUFLEN 1024

void
string_chomp(char *str) {
    if (str[strlen(str) - 1] == '\n') {
        str[strlen(str) - 1] = 0;
    }
}

void
string_comment(char *str, char *delim) {
    char *p;
    int i;
    for (i = 0; i < (int) strlen(delim); i++) {
        if ((p = strchr(str, delim[i]))) {
            *p = 0;
        }
    }
}

void
char_free(void *p) {
    char *c = (char *) p;
    FREE(c);
}

char *
string_variable_get(char *str, array_t * vars) {
    int n;
    array_t *kv;
    char *key, *val, *k, *v;
    char *p;
    if ((p = strchr(str, '='))) {
        n = p - str;
        key = strcut(str, 1, n);
        val = strcut(str, n + 2, strlen(str));
        k = strdup(lstrip(rstrip(key)));
        v = strdup(lstrip(rstrip(val)));
        kv = array_new();
        kv->free = char_free;
        array_append(kv, k);
        array_append(kv, v);
        array_append(vars, kv);
        FREE(key);
        FREE(val);
        str[0] = 0;
        return str;
    }
    return str;
}

void
string_variable_set(char *str, array_t * vars) {
    int i, n, m;
    char *key, *val, *p;
    for (i = 0; i < array_length(vars); i++) {
        key = array_element(array_element(vars, i), 0);
        val = array_element(array_element(vars, i), 1);
        p = str;
        while ((p = strstr(p, key))) {
            char *e = p + strlen(key);
            if ((!isspace(*e) && *e != 0) || (p > str && !isspace(*(p - 1)))) {
                p++;
                continue;
            }
            n = strlen(str) - strlen(key) + strlen(val);
            m = strlen(p) - strlen(key);
            memmove(p, p + strlen(key), m);
            p[m] = 0;
            m = strlen(p) + strlen(val);
            memmove(p + strlen(val), p, strlen(p));
            p[m] = 0;
            memmove(p, val, strlen(val));
            p[n] = 0;
        }
    }
}

void
array_freep(void *p) {
    array_t *a = (array_t *) p;
    array_free(&a);
}

void
sac_line_style_read(line_style_function func, void *data) {
    array_t *vars;
    char buf[BUFLEN];
    char *p;
    FILE *fp;
    char file[256];

    memset(file, 0, sizeof(file));
    zbasename(&file[0], 256);
    rstrip(file);
    strcat(file, "/linestyles.txt");

    if (!(fp = fopen(file, "rb"))) {
        fprintf(stderr, "linestyles: Error opening file: %s\n", file);
        return;
    }

    vars = array_new();
    vars->free = array_freep;
    while (fgets(buf, BUFLEN, fp) != NULL) {
        p = buf;
        string_chomp(p);
        string_comment(p, "#;");
        p = lstrip(rstrip(p));
        p = string_variable_get(p, vars);
        if (strlen(buf) > 0) {
            string_variable_set(p, vars);
            func(p, data);
        }
    }
    array_free(&vars);
}

display_t *
gdm_get_device_by_id(int id) {
    int i, n;
    display_t **dev;
    n = gdm_get_ndevices();
    dev = gdm_get_devices();

    for (i = 0; i < n; i++) {
        if (id == dev[i]->id) {
            return dev[i];
        }
    }
    return NULL;
}

display_t *
gdm_get_device_by_name(char *name) {
    int n;
    display_t **dev;
    dev = gdm_get_devices();
    if ((n = gdm_get_device_number_by_name(name)) < 0) {
        return NULL;
    }
    return dev[n];
}

int
gdm_get_device_number_by_name(char *pname) {
    int i, n;
    int nmatch;
    int *match;
    char *name, *p;

    display_t **dev;
    n = gdm_get_ndevices();
    dev = gdm_get_devices();

    name = strdup(pname);
    p = strchr(name, ' ');
    if (p) {
        *p = 0;
    }

    /* Look for exact matches */
    for (i = 0; i < n; i++) {
        if (strcasecmp(dev[i]->name, name) == 0) {
            free(name);
            return i;
        }
    }
    /* Look for partial matches */
    match = (int *) malloc(sizeof(int) * gdm_size);
    nmatch = 0;
    for (i = 0; i < n; i++) {
        if (strncasecmp(name, dev[i]->name, strlen(name)) == 0) {
            match[i] = TRUE;
            nmatch++;
        } else {
            match[i] = FALSE;
        }
    }
    if (nmatch == 1) {
        for (i = 0; i < n; i++) {
            if (match[i]) {
                nmatch = i;
            }
        }
    } else if (nmatch == 0) {
        nmatch = -1;
    } else if (nmatch > 1) {
        fprintf(stdout, "%s is an ambiguous option, possible matches are\n",
                pname);
        for (i = 0; i < n; i++) {
            fprintf(stdout, " %s\n", dev[i]->name);
        }
        nmatch = -1;
    }
    free(match);
    free(name);
    match = NULL;
    return nmatch;
}

void
gdm_init() {
    gdm_size = 0;
    gdm_alloc = 2;
    gdm_devices = malloc(sizeof(display_t *) * gdm_alloc);
    if (!gdm_devices) {
        fprintf(stderr, "Error allocating Graphics Devices, exiting\n");
        exit(-1);
    }
}

void
gdm_register_device(display_t * dev) {
    int i;
    display_t **tmp;
    if (gdm_size + 1 > gdm_alloc) {
        while (gdm_alloc < gdm_size + 1) {
            gdm_alloc *= 2;
        }
        tmp = realloc(gdm_devices, sizeof(display_t *) * gdm_alloc);
        if (!tmp) {
            fprintf(stderr, "Error allocating Graphics Device, exiting\n");
            exit(-1);
        }
        gdm_devices = tmp;
    }
    for (i = gdm_size; i < gdm_alloc; i++) {
        gdm_devices[i] = NULL;
    }
    gdm_devices[gdm_size] = dev;
    gdm_size = gdm_size + 1;
}

int
gdm_get_ndevices() {
    return gdm_size;
}

display_t **
gdm_get_devices() {
    return gdm_devices;
}

void
gdm_free_devices() {
    FREE(gdm_devices);
}

/** 
 * Initialize the Graphics Device Module
 *
 * @param nerr
 *    Error return flag
 *    - 0 on Success
 *    - Non-Zero on error
 *
 * @date   920330:  Block data initialization of lginit moved to initsac.
 * @date   900803:  Added initialization of color table and initial color.
 * @date   870501:  Added initialization of viewspace clipping flag.
 * @date   870416:  Added call to calstatus.
 * @date   861010:  Original version.
 *
 */
void
inigdm(int *nerr) {
    int j, jgd, nentry;
    float offset;

    *nerr = 0;

    /* - If library is already initialized, terminate it gracefully. */

    if (cmgdm.lginit) {
        endgraphics(nerr);
        if (*nerr != 0)
            goto L_8888;
    }

    /* - Initialize common block. */

    /* -- Device flags. */
    for (jgd = 1; jgd <= MGD; jgd++) {
        Lgdon[jgd] = FALSE;
        Iflhc[jgd] = 0;
    }

    cmgdm.igdcur = 3;
    cmgdm.igdhc = 2;
    cmgdm.igdtxt = 3;

    /* -- Begin frame flag. */
    cmgdm.lbegf = FALSE;

    /* -- Window attributes. */
    for (j = 1; j <= MWINDOWS; j++) {
        offset = 0.02 * (float) (j - 1);
        Xwindowmin[j] = 0.05 + offset;
        Xwindowmax[j] = 0.65 + offset;
        Ywindowmin[j] = 0.45 - offset;
        Ywindowmax[j] = 0.95 - offset;
        window_use_ratio[j] = FALSE;
        window_ratio[j] = 11 / 8.5;
    }
    cmgdm.iwindow = 1;

    /* -- Viewspace attributes. */
    cmgdm.lvsful = TRUE;
    cmgdm.vsrat = 0.75;
    cmgdm.lvsclip = TRUE;

    /* -- Text attributes. */
    cmgdm.ltsoft = TRUE;
    cmgdm.thgt = 0.02;
    cmgdm.twidth = 0.667 * cmgdm.thgt;
    cmgdm.tangle = 0.0;

    /* -- Software font attributes. */
    strcpy(kmgdm.kgtfn[0], "simplx                                  ");
    strcpy(kmgdm.kgtfn[1], "duplex                                  ");
    strcpy(kmgdm.kgtfn[2], "complx                                  ");
    strcpy(kmgdm.kgtfn[3], "triplx                                  ");
    cmgdm.nfont = -1;
    settextfont(1);

    /* -- Line drawing attributes. */
    cmgdm.iline = 1;

    /* - Default color table and initial color value. */

    initctable("default", 8, &nentry, nerr);
    if (*nerr != 0)
        goto L_8888;
    /*      call setcolor(nentry-1) */

    loadctable("color.tbl1.sac", NULL, &cmgdm.npscimage, nerr);
    if (*nerr != 0) {
        /* color table not found--use the data loaded one */
        *nerr = 0;
        cmgdm.npscimage = NPSCIMAGE;
    }
    /* - Initialize each device's common blocks. */

    gdm_init();

    initdevice2();
#ifdef X11_APP
    initdevice3();
#endif
#ifdef WIN32
    initdevice_win();
#endif

#ifdef OSX_APP
    initdevice_osx();
#endif /* OSX_APP */

    initdevice_postscript();
    initdevice_pdf();
    initdevice_record();
    initdevice_text();
    initdevice_xpm();
    initdevice_png();

    init_print_device_SGF(&print_device_begin, &print_device_end);

    /* - Get the names of graphics devices. */
    /*
       getdeviceinfo2( (char*)kmgdm.kgdnam[1],13, &Igdtyp[2] );
       getdeviceinfo3( kmgdm.kgdnam[2],13, &Igdtyp[3] );
     */
    /* - Blank fill end of graphics device names. */
    /*
       for( j = 1; j <= MGD; j++ ){
       j_ = j - 1;
       nc = indexb( (char*)kmgdm.kgdnam[j_],13 );
       subscpy( kmgdm.kgdnam[j_], nc, -1, 12, "            " );
       }
     */
    /* - Set the "library initialized" flag. */

    cmgdm.lginit = TRUE;

    /* - Calculate graphics status variables. */
    DEBUG("\n");
    calstatus();
    DEBUG("\n");

  L_8888:
    return;

}                               /* end of function */
