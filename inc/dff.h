
#ifndef _DFF_H_
#define _DFF_H_

#include "config.h"

#ifdef HAVE_LIBRPC
#include <rpc/rpc.h>
#endif /* HAVE_LIBRPC */

void        DEPRECATED       (char *old_func, char *new_func);
char *      fstrtrim         (char *s);
char *      fstrdup          (char *s,  int n);
char *      fstrset          (char *in, char *out, int n);

void        formhv           (char  *kname, 
			      int    kname_s, 
			      int    iform, 
			      char  *kout, 
			      int    kout_s, 
			      int   *nerr);
void        formmarker       (double  time, 
			      char   *type, 
			      int     type_s, 
			      char   *output, 
			      int     output_s, 
			      int    *lok);
void        getfhv           (char  *kname, 
			      float *fvalue, 
			      int   *nerr, 
			      int    kname_s);
void        gethv            (char *kname, 
			      int   kname_s, 
			      char *kvalue, 
			      int   kvalue_s, 
			      int  *nerr);
void        getfil           (int    idfl, 
			      int    ldta, 
			      int   *nlen, 
			      int   *ndx1, 
			      int   *ndx2, 
			      int   *nerr);
void        getihv           (char *kname, 
			      char *kvalue, 
			      int  *nerr, 
			      int   kname_s, 
			      int   kvalue_s);
void        getkhv           (char *kname, 
			      char *kvalue, 
			      int  *nerr, 
			      int   kname_s, 
			      int   kvalue_s);
void        getlhv           (char *kname, 
			      int  *lvalue, 
			      int  *nerr, 
			      int   kname_s);
void        getnfiles        (int *nfiles);
void        getnhv           (char *kname, 
			      int  *nvalue, 
			      int  *nerr, 
			      int   kname_s);
void        hdrfld           (char *kname, 
			      int   kname_s, 
			      int  *icat, 
			      int  *item, 
			      int  *lfound);
void        inihdr           ( );

void        inilhf           ( );
int         lgahdr           (char *kfield, 
			      int   kfield_s, 
			      char *kvalue, 
			      int   kvalue_s);
void        map_chdr_in      (float *memarray,
			      float *buffer);

void        map_chdr_out     (float *memarray,
			      float *buffer);
void        map_hdr_in       (float *memarray,
			      float *buffer,
			      int    lswap);
void        map_hdr_out      (float *memarray,
			      float *buffer, 
			      int    lswap);
void        markhdr          (int    jdflrestore,
			      int    jdfl1, 
			      int    jdfl2, 
			      char  *kvmknm, 
			      double vmk, 
			      char  *kimk);
void        newhdr           ( );
void        putfil           (int  idfl, 
			      int *nerr);
void        rddta            (int   idfl, 
			      int  *nun, 
			      int   lswap, 
			      int  *nerr);
int         rdhdr            (int  idfl, 
                              int  *nun, 
                              char *file,
                              int  *nerr);
void        rdsac            (int    idfl, 
			      char  *kname, 
			      int    kname_s, 
			      int    lname, 
			      int    ldta, 
			      int   *nlen, 
			      int   *ndxh, 
			      int   *ndx1, 
			      int   *ndx2, 
			      int   *nerr);
void        rdsdta           (int   idfl, 
			      int  *nun, 
			      int  *nerr);
void        rdsegy           (int   idfl, 
			      char *kfile,
			      int  *nlen,
			      int  *ndx1, 
			      int  *ndx2, 
			      int  *nerr);
void        rdshdr           (int  idfl, 
			      int *nun, 
			      int *nerr);
void        rdxdrdta         (int   idfl, 
			      char *kname, 
			      int   kname_s, 
			      int  *nerr);
void        sac_data_read    (int    nun, 
			      float *yarray, 
			      int    npts, 
			      int    comp, 
			      int    lswap, 
			      int   *nerr);
void        rsac1            (char      *kname, 
			      float     *yarray, 
			      int       *nlen, 
			      float     *beg, 
			      float     *del, 
			      int       *max_, 
			      int       *nerr, 
			      int        kname_s);
void        rsac2            (char      *kname, 
			      float     *yarray, 
			      int       *nlen, 
			      float     *xarray, 
			      int       *max_, 
			      int       *nerr, 
			      int        kname_s);
void  sacio_initialize_common( );
int  sac_check_header_version(float *hdr, 
			      int   *nerr);
void        sac_header_swap  (float *hdr);
int         sac_header_read  (int nun, int *nerr);
void        sac_header_write (int    nun, 
                              float *hdr, 
                              char  *khdr, 
                              int    swap, 
                              int   *nerr);
void        sac_data_write   (int    nun, 
                              float *y, 
                              float *x, 
                              int    npts, 
                              int    swap, 
                              int   *nerr);
void        sac_data_swap    (float *y, 
                              int    n);
int         sac_byte_order   (int    getset);

void        rsach            (char *kname,
			      int  *nerr,
			      int   kname_s);
void        setfhv           (char  *kname, 
			      float *fvalue, 
			      int   *nerr, 
			      int    kname_s);
void        setihv           (char *kname, 
			      char *kvalue, 
			      int  *nerr, 
			      int   kname_s, 
			      int   kvalue_s);
void        setkhv           (char *kname, 
			      char *kvalue, 
			      int  *nerr, 
			      int   kname_s, 
			      int   kvalue_s);
void        setlhv           (char *kname, 
			      int  *lvalue, 
			      int  *nerr, 
			      int   kname_s);
void        setnfiles        (int nfiles);
void        setnhv           (char *kname, 
			      int  *nvalue, 
			      int  *nerr, 
			      int   kname_s);
void        setrng           ();
void        updhdr           (int *nerr);
void        wrsac            (int   idfl, 
			      char *kname, 
			      int   kname_s, 
			      int   ldta, 
			      int  *nerr);
void        wrsdd            (int   idfl, 
			      char *kname, 
			      int   kname_s, 
			      int   ldta, 
			      int  *nerr);
void        wrsegy           (int   idfl, 
			      char *filename, 
			      int  *nerr );
void        wrxdr            (int   idfl, 
			      char *kname, 
			      int   kname_s, 
			      int   ldta, 
			      int  *nerr);
void        wsac0            (char  *kname, 
			      float *xarray, 
			      float *yarray, 
			      int   *nerr, 
			      int    kname_s);
void        wsac1            (char  *kname, 
			      float *yarray, 
			      int   *nlen, 
			      float *beg, 
			      float *del, 
			      int   *nerr, 
			      int    kname_s);
void        wsac2            (char  *kname, 
			      float *yarray, 
			      int   *nlen, 
			      float *xarray, 
			      int   *nerr, 
			      int    kname_s);
void        wsac3            (char  *kname, 
			      float *xarray, 
			      float *yarray, 
			      int   *nerr, 
			      int    kname_s);
void        update_distaz    ( );

int         CheckByteOrder   ( );

#ifdef HAVE_LIBRPC
void        xdrhdr           (XDR    xdrs, 
			      float *headerbuf, 
			      int   *nerr);
#else 

void librpc_not_available();

#endif /* HAVE_LIBRPC */






#endif /* _DFF_H_ */
