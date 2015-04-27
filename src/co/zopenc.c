/** 
 * @file   zopenc.c
 * 
 * @brief  Opne a file
 * 
 */
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "unistdx.h"

#include "co.h"

#include "errors.h"

/** 
 * Open a file or create one
 * 
 * @param pfd 
 *    Fortran file descriptor
 * @param pfname 
 *    File to open
 * @param pnewfl 
 *    Create file is necessary flag
 * @param pro 
 *    Read-only flag
 * @param pnerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_ZOPENC_INSUFFICIENT_ACCESS
 *    - ERROR_ZOPENC_ERROR
 *
 * @param pfnlen 
 *    Length of \p pfname
 *
 * @note  SAC carries around FORTRAN logical unit numbers, but it
 *        doesn't currently have hooks for "C" file descriptors.
 *        Therefore, the "C" descriptors are carried in the same
 *        variable as the logical unit numbers.  The file descriptors
 *        are negated to distinguish them from logical unit numbers.
 *
 * @date 07/29/98  Moved string-length-specifier to the rear of the 
 *                 parameter list, and added stub with an underscore
 *                 (zopenc_) for FORTRAN compatibility.  maf
 * @date 12/16/85  Tested--D. Trimmer
 * @date 05/26/87  Merged MASSCOMP differences in current version--J. Tull
 * @date 07/29/87  Added read-only flag--J. Tull
 * @date 07/31/87  Added check for insufficient access rights--J.Tull
 */
void
zopenc(int *pfd, char *pfname, int *pnewfl, int *pro, int *pnerr, int pfnlen) {

    int i;
    int mode;
    int errno;
    char fname[BUFSIZ];

    *pnerr = 0;
    if (*pro) {
        mode = O_RDONLY;
    } else {
        mode = O_RDWR;
    }

#ifdef WIN32
    mode = mode | O_BINARY;
#endif

    memset(fname, 0, sizeof(fname));

    if ((size_t) pfnlen > strlen(pfname) || pfnlen < 0) {
        pfnlen = strlen(pfname);
    }

    for (i = 0; i < BUFSIZ && i < pfnlen; ++i)
        fname[i] = pfname[i];

    /* get rid of trailing blanks */
    for (--i; i > 0 && ((fname[i] == ' ') || (fname[i] == '\0')); --i);
    fname[i + 1] = '\0';

    if (*pnewfl) {
#ifdef WIN32
        *pfd = open(fname, O_CREAT | O_TRUNC | O_WRONLY | O_BINARY, 0);
#else
        *pfd = creat(fname, 0); /* create file */
#endif
    } else {
        *pfd = open(fname, mode, 0);    /* open file */
    }
    if (*pfd < 0) {
        if (errno == EACCES)
            *pnerr = ERROR_ZOPENC_INSUFFICIENT_ACCESS;
        else
            *pnerr = ERROR_ZOPENC_ERROR;
    }

    if (*pnewfl && *pnerr == 0)
        chmod(fname, 0666);     /* set file permissions */

    *pfd = -(*pfd);             /* see Notes above */
    return;
}

void
zopenc_(int *pfd, char *pfname, int *pnewfl, int *pro, int *pnerr, int pfnlen) {
    zopenc(pfd, pfname, pnewfl, pro, pnerr, pfnlen);
}

void
zopenc__(int *pfd, char *pfname, int *pnewfl, int *pro, int *pnerr, int pfnlen) {
    zopenc(pfd, pfname, pnewfl, pro, pnerr, pfnlen);
}
