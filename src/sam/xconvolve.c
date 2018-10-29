
#include <string.h>

#include "sam.h"
#include "dfm.h"
#include "amf.h"
#include "hdr.h"
#include "bool.h"

#include "co.h"
#include "ucf.h"
#include "msg.h"
#include "clf.h"
#include "dbh.h"
#include "cpf.h"
#include "dff.h"

#define	MCORLN	4096
#define	MWINLN	2048
SAM_EXTERN

sac *sacread(char *file);
int td_conv(float     *waveform,
            int        n_w,
            float     *pulse,
            int        n_p,
            float     *conv,
            float      delta,
            float      factor,
            float      b_p);

enum pulse {
    TRI     = 1,
    GAUSS   = 4,
    SACFILE = 5,
    SACFILE_IN_MEMORY = 6,
};

float *
norm1d(float *y, int m, double dt) {
    double sum = 0.0;
    double y0 = 0.0;
    // Remove Y Offset
    for(int i = 0; i < m; i++) {   y0 = fmin(y0, y[i]);      }
    for(int i = 0; i < m; i++) {   y[i] = y[i] - y0;         }
    // Normalize
    for(int i = 0; i < m-1; i++) {   sum += 0.5 * dt * (y[i]+y[i+1]);  } // Trapezodial Integration
    for(int i = 0; i < m; i++)   {   y[i] = y[i] / sum; }
    return y;
}

float *
box_pulse(double width, double dt, int *n) {
    if(width/2.0 < dt) {
        error(1002, "box pulse width/2 (%f) < delta t (%f)\n", width/2.0, dt);
        return NULL;
    }
    *n = (int)(floor(width / dt));
    float *y = (float *) calloc(*n, sizeof(float));
    for(int i = 0; i < *n; i++) {
        y[i] = 1.0;
    }
    return norm1d(y, *n, dt);
}

float *
trap_pulse(double w1, double w2, double dt, int *n) {
    int n1,n2;
    float *b1, *b2, *y;
    if(w2 > w1) {
        double w = w1;
        w1 = w2;
        w2 = w;
    }
    b1 = box_pulse(w1, dt, &n1);
    if(b1 == NULL) {
        return NULL;
    }
    b2 = box_pulse(w2, dt, &n2);
    if(b1 == NULL || b2 == NULL) {
        FREE(b1);
        return NULL;
    }
    *n = n1 + n2 - 1;
    y = (float *) calloc(*n, sizeof(float));
    td_conv(b1, n1, b2, n2, y, dt, 1.0, 0.0);
    FREE(b1);
    FREE(b2);
    return norm1d(y, *n, dt);
}



float *
triangle_pulse(double half_width, double dt, int *m) {
    int n = (int)(floor(half_width / dt));
    if(half_width/2.0 < dt || n <= 0) {
        error(1002, "triangle pulse: half-width/2 (%f) < delta t (%f)", half_width/2.0, dt);
        return NULL;
    }
    *m = 2*n + 1;
    float *y = (float *) calloc(*m, sizeof(float));
    for(int i = 0; i < n; i++) {
        y[i] = dt * i;
    }
    for(int i = 0; i < n+1; i++) {
        y[i+n] = dt * (n-i);
    }

    return norm1d(y, *m, dt);
}
float *
mag_pulse(float mag, double dt, float vr, int *n) {
    float a = 5.08;
    float b = 1.16;
    double L = pow(10.0, (mag-a)/b);
    double w = L / vr;
    return triangle_pulse(w, dt, n);
}

sac *
sac_from_data(float *y, int n, double b, double dt, char *filename) {
    sac *s;

    s = sac_new();
    s->m->filename = strdup(filename);
    s->h->npts = n;
    sac_alloc(s);
    s->h->delta = dt;
    s->h->b     = b;
    s->y        = y;
    sac_extrema(s);
    sac_be(s);

    return s;
}

float *
gauss_pulse(double sigma, double dt, int *n) {
    if(sigma / 2.0 < dt) {
        error(1002, "gauss pulse sigma/4 (%f) < dt (%f)", sigma/2.0, dt);
        return NULL;
    }
    double hw = sigma * 5;
    *n = (int) floor(hw*2/dt);
    float *y = (float *) calloc(*n, sizeof(float));
    for(int i = 0; i < *n; i++) {
        double t = (i * dt)-hw;
        double v = pow(t/sigma,2);
        y[i] = exp( -v/2.0  );
    }
    return norm1d(y, *n, dt);
}
sac *
sac_gauss_pulse(double sigma, double dt) {
    int n = 0;
    float *y = gauss_pulse(sigma, dt, &n);
    if(y == NULL) {
        return NULL;
    }
    return sac_from_data(y, n, -(n/2)*dt, dt, "box");
}

sac *
sac_box_pulse(double width, double dt) {
    int n;
    float *y = box_pulse(width, dt, &n);
    if(y == NULL) {
        return NULL;
    }
    return sac_from_data(y, n, -(n/2)*dt, dt, "box");
}
sac *
sac_tri_pulse(double width, double dt) {
    int n;
    float *y = triangle_pulse(width, dt, &n);
    if(y == NULL) {
        return NULL;
    }
    return sac_from_data(y, n, -(n/2)*dt, dt, "triangle");
}
sac *
sac_mag_pulse(float mag, double dt, float vr) {
    int n;
    float *y = mag_pulse(mag, dt, vr, &n);
    if(y == NULL) {
        return NULL;
    }
    return sac_from_data(y, n, -(n/2)*dt, dt, "triangle");
}
sac *
sac_trap_pulse(double w1, double w2, double dt) {
    int n;
    float *y = trap_pulse(w1, w2, dt, &n);
    if(y == NULL) {
        return NULL;
    }
    return sac_from_data(y, n, -(n/2)*dt, dt, "trapezoi");
}

int
sac_td_conv(sac *s, sac *p) {
    int m;
    float *z;
    m = s->h->npts + p->h->npts - 1;
    z = (float *) calloc(m, sizeof(float));
    if(! td_conv(s->y, s->h->npts,
                 p->y, p->h->npts,
                 z, s->h->delta, 1.0, p->h->b))  {
        error(1002, "waveform npts (%d) < pulse npts (%d)", s->h->npts, p->h->npts);
        return 0;
    }

    FREE(s->y);
    s->y = z;
    s->h->npts = m;
    sac_be(s);
    sac_extrema(s);
    return 1;
}



int
td_conv(float     *waveform,
        int        n_w,
        float     *pulse,
        int        n_p,
        float     *conv,
        float      delta,
        float      factor,
        float      b_p) {
    int i, j, j_1;
    float temp;

    if (n_p > n_w) {
        return 0;
    }

    j_1 = -lrint(b_p/delta);

    for(i=0; i < n_w+n_p-1; i++){
        temp = 0.0;
        for(j=0; j < n_w; j++) {
            if (i >= (j-j_1) && n_p > (i-j+j_1)) {
                temp = temp + waveform[j]*pulse[i-j+j_1];
            }
        }
        conv[i] = factor*temp;
    }
    return 1;
}

#define DT_CHECK(a,b) do {                          \
    if(fabs(a->h->delta - b->h->delta) >= 1e-7) {   \
        nerr = ERROR_UNEQUAL_SAMPLE_RATES;          \
        goto L_8888;                                \
    }                                               \
} while(0);

static int
file_exists(char *filename) {
    int exists = FALSE;
    zinquire(filename, &exists);
    return exists;
}

int
convolve_with_all(sac *p) {
    int i;
    int nerr = SAC_OK;
    sac *s = NULL;
    for(i = 0; i < saclen(); i++) {
        if (!(s = sacget(i, TRUE, &nerr))) { goto L_8888; }
        DT_CHECK(s, p); // This may exit if the sample rates are not equal
    }
    for(i = 0; i < saclen(); i++) {
        if (!(s = sacget(i, TRUE, &nerr))) { goto L_8888; }
        if(!sac_td_conv(s, p)) {
            nerr = error_status();
            goto L_8888;
        }
    }
 L_8888:
    return nerr;
}

void /*FUNCTION*/
xconvolve(nerr)
     int *nerr;
{
    sac *s, *p;
    char tmp[512];
    char pulse_file[512];
    int i;
    int centered = FALSE;
    static int nvals;
    static double val[2];
    static int master = 1;
    static enum pulse pulse_kind = SACFILE_IN_MEMORY;
    s = NULL;
    p = NULL;
    /*=====================================================================
	 * PURPOSE: To parse and execute the action command CONVOLVE.
	 *          This command computes convolutions.
	 *=====================================================================
	 * OUTPUT ARGUMENTS:
	 *    NERR:    Error flag. Set to 0 if no error occurred.
	 *=====================================================================
	 * MODULE/LEVEL:  SAM/2
	 *=====================================================================
	 * GLOBAL INPUT:
	 *    MACH:
	 *    DFM:     NDFL
	 *=====================================================================
	 * GLOBAL OUTPUT:
	 *    HDR:     DEPMIN, DEPMAX, DEPMEN
	 *    MEM:     SACMEM
	 *=====================================================================
	 * SUBROUTINES CALLED:
	 *=====================================================================
	 * LOCAL VARIABLES:
   *
	 *=====================================================================
	 * MODIFICATION HISTORY:
   *    180731:  Retooled to focus on adding a source or instrument shape
	 *    961204:  Modified to return full range of values instead of just
	 *             the central half.  Timing changed to leave begin times
	 *             of signals unchanged from the input signals.  
	 *    920110:  Added DATA-SET update logic.
	 *    870925:  Fixed bug when signals were of different length.
	 *             Now output signals are all equal in length to the
	 *               maximum length of the input signals.
	 *    870312:  Added ability to choose master file by name.
	 *    870209:  Converted to an internal command.
	 *    830000:  Original XSC version.
	 *=====================================================================
	 * DOCUMENTED/REVIEWED:  870925
	 *===================================================================== */
    /* PROCEDURE: */
    *nerr = 0;

    while (lcmore(nerr)) {
        if (lklog("CENTERED", 5, &centered)) {
        } else if (lkreal("TRI", 4, &val[0])) {
            pulse_kind = TRI;
        } else if (lkreal("GAU", 4, &val[0])) {
            pulse_kind = GAUSS;
        } else if(lkint("PULSE", 6, &master)) {
            pulse_kind = SACFILE_IN_MEMORY;
        } else if(lcchar(tmp, sizeof(tmp))) {
            if(!file_exists(tmp)) {
                // Back up parser and complain about unknown file as option
                arg_prev();
                cfmt("ILLEGAL OPTION:", 17);
                cresp();
            } else {
                pulse_kind = SACFILE;
                strcpy(pulse_file, tmp);
            }
        } else {
            cfmt("ILLEGAL OPTION:", 17);
            cresp();
        }
    }

    if (*nerr != 0) {
        goto L_8888;
    }
    /* - Test for a non-null data file list. */
    vflist(nerr);
    if (*nerr != 0) {
        goto L_8888;
    }

    /* - Make sure each file is an evenly spaced time series file. */
    vfeven(nerr);
    if (*nerr != 0) {
        goto L_8888;
    }

    if(pulse_kind == TRI || pulse_kind == GAUSS) {
        for(i = 0; i < saclen(); i++) {
            if (!(s = sacget(i, TRUE, nerr))) { goto L_8888; }
            switch (pulse_kind) {
              case SACFILE: break;
              case SACFILE_IN_MEMORY: break;
              case GAUSS: p = sac_gauss_pulse(val[0], s->h->delta); break;
              case TRI:   p = sac_tri_pulse(val[0], s->h->delta); break;
            }
            if(!p) {
                *nerr = error_status();
                goto L_8888;
            }
            /* Set initial time to zero per an option */
            if(!centered) {
                p->h->b = 0.0;
                sac_be(p);
            }
            if(!sac_td_conv(s, p)) {
                *nerr = error_status();
                goto L_8888;
            }
            sac_free(p);
            p = NULL;
        }
    } else if (pulse_kind == SACFILE) {
        if(!(p = sacread(pulse_file))) {
            clrmsg();
            error(*nerr = ERROR_READING_FILE, ": %s", pulse_file);
            goto L_8888;
        }
        *nerr = convolve_with_all(p);
    } else if (pulse_kind == SACFILE_IN_MEMORY) {
        if (!(p = sacget(master-1, TRUE, nerr))) {
            error(*nerr = 1310, " pulse: %d", master);
            goto L_8888;
        }
        // Create a copy of the data and remove it from global memory
        p = sac_copy(p);
        sacdel(master-1);
        *nerr = convolve_with_all(p);
    }

    setrng();
  L_8888:
    if(p) {
        sac_free(p);
        p = NULL;
    }
    return;

}                               /* end of function */
