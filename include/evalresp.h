/**
 * @file evalresp.h
 *
 * @brief Evalresp Functions
 */

#ifndef __SAC_EVALRESP_H__
#define __SAC_EVALRESP_H__

/**
 *  Interface to Evalresp functionality
 *
 */
int evresp_1(char *sta, char *cha, char *net, char *locid, char *datime,
             char *units, char *file, double *freqs, int nfreqs, double *resp,
             char *rtype, char *verbose, int start_stage, int stop_stage,
             int stdio_flag, int useTotalSensitivityFlag, double x_for_b62,
             int xml_flag) ;

/**
 *  Remove evalresp response from a data series
 *
 */
int remove_evalresp(float *data, int n, float dt, double limits[4],
                    char *id, char *when, char *resp_file);
int remove_evalresp_simple(float *data, int n, float dt, double limits[4]);



#endif /* __SAC_EVALRESP_H__ */
