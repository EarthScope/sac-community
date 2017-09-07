/** 
 * @file   pl.h
 * 
 * @brief  Plotting 
 * 
 */

#ifndef _PL_H_
#define _PL_H_

#include "gem.h"

void centxt(char *ktext, int ktext_s, int ntext, int itextp, double tsize);
void incat(int icurat, int iattab[], int nattab, int *jattab, int *inewat);
void inicol(color *iicol, int *nicol);
void inigem(void);
void inilin(int iilin[], int *nilin);
void iniwidth(void);
void logdta(double array[], int number, int lfloor, double floor, double output[],
            int *nerr);
void logdtaf(float array[], int number, int lfloor, double floor, float output[],
            int *nerr);
void pl2d(float xarray[], float yarray[], int number, int incx, int incy,
          int *nerr);
void plalpha(char *kalpha, int kalpha_s, int malpha, int lprint, int *nerr);
void plblank(double xblank[], double yblank[], double xarray[], double yarray[],
             int number);
void plcalwvtrans(void);
void plclip(double xarray[], double yarray[], int number, int lnewdp);
void pldta(float xarray[], float yarray[], int number, int incx, int incy,
           int *nerr);
void plgrid(int *nerr);
void plhome(void);
void plmap(float xarray[], float yarray[], int number, int incx, int incy,
           int *nerr);
void plnocl(double xarray[], double yarray[], int *number, int lnewdp);
void plplab(void);
void plrest(void);
void plsave(void);
void pltext(char *ktext, double xloc, double yloc);
void xlinax(void);
void xlogax(void);
void ylinax(void);
void ylogax(void);

float vmin(float *array, int n, int dn);
float vmax(float *array, int n, int dn);

#endif /* _PL_H_ */
