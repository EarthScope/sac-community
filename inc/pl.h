/** 
 * @file   pl.h
 * 
 * @brief  Plotting 
 * 
 */

#ifndef _PL_H_
#define _PL_H_

void centxt ( char *ktext, 
              int ktext_s, 
              int ntext, 
              int itextp, 
              double tsize);
void incat ( int icurat, 
             int iattab[], 
             int nattab, 
             int *jattab, 
             int *inewat);
void inicol ( int iicol[], 
              int *nicol);
void inigem (void);
void inilin ( int iilin[], 
              int *nilin);
void iniwidth (void);
void logdta ( float array[], 
              int number, 
              int lfloor, 
              double floor, 
              float output[], 
              int *nerr);
void pl2d ( float xarray[], 
            float yarray[], 
            int number, 
            int incx, 
            int incy, 
            int *nerr);
void plalpha ( char *kalpha, 
               int kalpha_s, 
               int malpha, 
               int lprint, 
               int *nerr);
void plblank ( float xblank[], 
               float yblank[], 
               float xarray[], 
               float yarray[], 
               int number);
void plcalwvtrans (void);
void plclip ( float xarray[], 
              float yarray[], 
              int number, 
              int lnewdp);
void pldta ( float xarray[], 
             float yarray[], 
             int number, 
             int incx, 
             int incy, 
             int *nerr);
void plgrid ( int *nerr);
void plhome (void);
void plmap ( float xarray[], 
             float yarray[], 
             int number, 
             int incx, 
             int incy, 
             int *nerr);
void plnocl ( float xarray[], 
              float yarray[], 
              int *number, 
              int lnewdp);
void plplab (void);
void plrest (void);
void plsave (void);
void pltext ( char *ktext, 
              int ktext_s, 
              double xloc, 
              double yloc);
void xlinax (void);
void xlogax (void);
void ylinax (void);
void ylogax (void);

#endif /* _PL_H_ */
