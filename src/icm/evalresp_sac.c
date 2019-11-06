
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "evalresp/public.h"
#include "evalresp/evresp.h"
#include "icm.h"
#include "libpz.h"
#include "dff.h"
#include <fstr.h>

#define FREE(x) { if(x) { free(x); } }

/**
 *  Compute full displacement response from a evalresp file
 *
 * - id - Instrument Identifer, NET.STA.LOC.CHAN
 * - when - Reference time, YYYY,DDD,HH:MM::SS
 * - resp_file - Evalresp Response File
 * - t - Transfer info containing frequencies, nfft, nfreqs, dt, npts
 *
 */
Response *
evalresp_resp(char *id, char *when, char *resp_file, Transfer *t) {

    int start_stage         = -1;
    int stop_stage          = 0;
    int stdio_flag          = 0;
    char units[]            = "DIS";
    char verbose[]          = "";
    char rtype[]            = "CS"; //
    int iflag               = 0;    //
    int x_for_b62           = 0;    //
    int xml_flag            = 0;    // 
    int useTotalSensitivity = 0;    // 
    char *xresp_file        = NULL;
    double *resp;
    char sta[10],net[10],loc[10],cha[10];
    char when_auto[64];
    
    resp = calloc(t->nfft*2, sizeof(double));

    if(strcmp(when, "*") == 0) {
        sac_reference_time(when_auto);
        when = when_auto;
    }
    sac_station_id_split(id, net, sta, loc, cha);

    if(strcmp(resp_file,"*") != 0) {
        xresp_file = resp_file;
    }

    // Use evresp to generate the instrument respone from RESP file
    iflag = evresp_1(sta, cha, net, loc, when, units, xresp_file,
                     t->f, t->nfreqs, resp, rtype, verbose,
                     start_stage, stop_stage,
                     stdio_flag, useTotalSensitivity,
                     x_for_b62, xml_flag );
    if(iflag != 0) {
        FREE(resp);
        printf("evalresp return: %d\n", iflag);
        if(iflag == -1) {
            printf("More than one response found\n");
        }
        if(iflag == 1) {
            printf("No responses found\n");
        }
        return NULL;
    }
    Response *r = response_from_2vec(resp, t->nfreqs);
    FREE(resp);
    return r;
}

/**
 *  Remove a response using evalresp
 *
 *  - data - Time series data
 *  - n - Length of data
 *  - dt - Sampling interval (seconds)
 *  - limits - Frequency range over which response is removed (Hz)
 *        - limits[0] - Low  Frequency Edge (Response = 0)
 *        - limits[1] - Low  Frequency Edge (Response = 1)
 *        - limits[2] - High Frequency Edge (Response = 1)
 *        - limits[3] - High Frequency Edge (Response = 0)
 *        Example: limits[4] = { 0.002, 0.005, 12.0, 20.0 };
 *  - id - Intrument Identifier, NET.STA.LOC.CHA
 *       - *.*.*.* wildcards are valid, but unexpected results may occur
 *       - * will try to automatically determine the instrument id
 *           using the current sac file
 *  - when - Reference time, YYYY,DDD,HH:MM:SS
 *       - * will try to automatically determine the reference time
 *           using the current sac file
 *  - resp_file - Evalresp Response File
 *
 *  Response is removed in place and original data is overwritten on return
 *
 *  Output time series is in meters
 *
 */
int
remove_evalresp(float *data, int n, float dt, double limits[4], char *id, char *when, char *resp_file) {

    Response *from;

    Transfer *t = transfer_new(n, dt);

    // FROM Response
    if((from = evalresp_resp(id, when, resp_file, t)) == NULL) {
        return 1;
    }
    response_recip(from);

    // TO Response
    Response *to = response_ones(t->nfreqs);

    ztransfer(data, n, dt,
              from->re, from->im,
              to->re, to->im,
              t->nfreqs, t->nfft, t->df, limits);

    for(int i = 0; i < n; i++) {
        data[i] = from->re[i];
    }
    response_free(from);
    response_free(to);
    transfer_free(t);
    return 0;
}


int
remove_evalresp_(float *data, int *n, float *dt, double *limits, char *id, char *when, char *resp_file, int id_s, int when_s, int resp_file_s) {

    char *id_c = fstrdup(id, id_s);
    char *when_c = fstrdup(when, when_s);
    char *resp_file_c = fstrdup(resp_file, resp_file_s);

    int retval = remove_evalresp(data, *n, *dt, limits, id_c, when_c, resp_file_c);

    FREE(id_c);
    FREE(when_c);
    FREE(resp_file_c);

    return retval;
}

int
remove_evalresp__(float *data, int *n, float *dt,  double *limits, char *id, char *when, char *resp_file, int id_s, int when_s, int resp_file_s) {
    return remove_evalresp_(data, n, dt, limits,
                            id, when, resp_file, id_s, when_s, resp_file_s);
}

int
remove_evalresp_simple(float *data, int n, float dt, double limits[4]) {
    return remove_evalresp(data, n, dt, limits, "*", "*", "*");
}
int
remove_evalresp_simple_(float *data, int *n, float *dt, double limits[4]) {
    return remove_evalresp(data, *n, *dt, limits, "*", "*", "*");
}
int
remove_evalresp_simple__(float *data, int *n, float *dt, double limits[4]) {
    return remove_evalresp(data, *n, *dt, limits, "*", "*", "*");
}
