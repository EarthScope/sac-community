/** 
 * @file   dfir.h
 * 
 * @brief  Finite Impulse Response Filter
 * 
 */

#ifndef _DFIR_H_
#define _DFIR_H_

#define	MBANDS	 10
#define	MFILT	 255
#define	MSIZE1	 (MFILT/2 + 2)
#define	MSIZE2	 (16*MSIZE1)
#define	MSIZE3	 (2*MBANDS)
#define	MXNSECTS 30

/** 
 * @struct cmdfir
 *    Fir Filter type ???
 *
 */
struct t_cmdfir {
    double pi2;
    double ad[MSIZE1];
    double dev;
    double x[MSIZE1];
    double y[MSIZE1];
    float grid[MSIZE2];
    float des[MSIZE2];
    float wt[MSIZE2];
    float alpha[MSIZE1];
    int iext[MSIZE1];
    int nfcns;
    int ngrid;
    float h[MSIZE1];
    int infilt;
    int nfilt;
    int jtype;
    int nbands;
    int ngrden;
    float edgehz[MSIZE3];
    float edge[MSIZE3];
    float fx[MBANDS];
    float wtx[MBANDS];
    float deviat[MBANDS];
    float delta;
} ;

/** 
 * @struct cmfir2 
 *    Fir Filter type 2 ???
 *
 */
struct t_cmfir2 {
    char kfilnm[9];
    int nlnwr1;
    char ktype[9];
    int nid;
    char kid[81];
    float dt;
    int ncoeff;
} ;

/** 
 * @struct cmfir3 
 *    Fir Filter type 3 ???
 *
 */
struct t_cmfir3 {
    int nsects;
    float sn[3 * MXNSECTS];
    float sd[3 * MXNSECTS];
    char filttype[3];
} ;

#define DFIR_EXTERN                             \
    extern struct t_cmfir3 cmfir3;              

#endif /* _DFIR_H_ */
