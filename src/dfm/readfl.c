/** 
 * @file   readfl.c
 * 
 * @brief  Read a filelist into memory
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#ifndef WIN32
#include <libgen.h>
#endif

#include "config.h"

#include "wild.h"
#include "bot.h"
#include "msg.h"
#include "clf.h"
#include "co.h"
#include "dff.h"
#include "dfm.h"

#include "sac_datetime.h"

#include "fid.h"
#include <libmseed/libmseed.h>
#include <fern/fern.h>

DFM_EXTERN

#ifdef HAVE_LIBRPC
#include <rpc/rpc.h>
#endif /* HAVE_LIBRPC */
#include <fern/array.h>

#include "dfm.h"
#include "bool.h"
#include "amf.h"
#include "hdr.h"

#include "errors.h"
#include "debug.h"

#define PATH_MAX_X 1024

#ifdef MISSING_FUNC_BASENAME


char *
basename(const char *path)
{
	static char bname[PATH_MAX_X];
	size_t len;
	const char *endp, *startp;

	/* Empty or NULL string gets treated as "." */
	if (path == NULL || *path == '\0') {
		bname[0] = '.';
		bname[1] = '\0';
		return (bname);
	}

	/* Strip any trailing slashes */
	endp = path + strlen(path) - 1;
	while (endp > path && (*endp == '/' || *endp == '\\'))
		endp--;

	/* All slashes becomes "/" */
	if (endp == path && (*endp == '/' || *endp == '\\')) {
		bname[0] = '/';
		bname[1] = '\0';
		return (bname);
	}

	/* Find the start of the base */
	startp = endp;
	while (startp > path && (*(startp - 1) != '/' || *(startp-1) != '\\'))
		startp--;

	len = endp - startp + 1;
	if (len >= sizeof(bname)) {
		return (NULL);
	}
	memcpy(bname, startp, len);
	bname[len] = '\0';
	return (bname);
}

#endif /* MISSING_FUNC_BASENAME */

#ifdef MISSING_FUNC_DIRNAME

char *
dirname(const char *path)
{
	static char dname[PATH_MAX_X];
	size_t len;
	const char *endp;

	/* Empty or NULL string gets treated as "." */
	if (path == NULL || *path == '\0') {
		dname[0] = '.';
		dname[1] = '\0';
		return (dname);
	}

	/* Strip any trailing slashes */
	endp = path + strlen(path) - 1;
	while (endp > path && (*endp == '/' || *endp == '\\'))
		endp--;

	/* Find the start of the dir */
	while (endp > path && (*endp != '/' || *endp == '\\'))
		endp--;

	/* Either the dir is "/" or there are no slashes */
	if (endp == path) {
      if(*endp == '/' || *endp == '\\') {
          dname[0] = '/';
      } else {
          dname[0] = '.';
      }
		dname[1] = '\0';
		return (dname);
	} else {
		/* Move forward past the separating slashes */
		do {
			endp--;
		} while (endp > path && (*endp == '/' || *endp == '\\'));
	}

	len = endp - path + 1;
	if (len >= sizeof(dname)) {
		return (NULL);
	}
	memcpy(dname, path, len);
	dname[len] = '\0';
	return (dname);
}
#endif

extern enum filetype {
    sacfi,
    alpha,
    xdr,
        segy,
        miniseed,
} ftype;

static int
read_alpha_file(char *file) {
    int nerr, nlen, ndx1, ndx2;
    int retval = TRUE;
    rdci(0, file, -1, &nlen, &ndx1, &ndx2, &nerr);
    if (nerr) {
        strcpy(kmdfm.kecbdf, "WARNING ");
        typmsg("WARNING");
        outmsg();
        retval = FALSE;
    }
    return retval;

}

static int
read_alpha(string_list * files) {
    int i, retval = 0;
    for (i = 0; i < string_list_length(files); i++) {
        if(!(read_alpha_file(string_list_get(files, i)))) {
            retval = 1;
        }
    }
    return retval;
}

static int
read_segy_file(char *file) {
    sac *s = NULL;
    int nerr;
    int nlen, ndx1, ndx2, retval;
    retval = TRUE;
    nerr = 0;
    if (!(s = sac_new())) {
        return ERROR_ILLEGAL_DATA_FILE_LIST_NUMBER;
    }
    s->m->filename = strdup( file );
    sacput(s);
    rdsegy(saclen(), s->m->filename, &nlen, &ndx1, &ndx2, &nerr);
    if (nerr) {
        strcpy(kmdfm.kecbdf, "WARNING ");
        typmsg("WARNING");
        outmsg();
        sacpop();
        retval = FALSE;
    }
    return retval;

}

static int
read_segy(string_list * files) {
    int i, retval = 0;
    for (i = 0; i < string_list_length(files); i++) {
        if(!(read_segy_file(string_list_get(files, i)))) {
            retval = 1;
        }
    }
    return retval;
}

static int
read_xdr_file(char *file) {
    int nerr = 0;
    rdxdrdta(0, file, -1, &nerr);
    if (nerr) {
        strcpy(kmdfm.kecbdf, "WARNING ");
        typmsg("WARNING");
        outmsg();
        return 0;
    }
    return 1;
}

static int
read_xdr(string_list * files) {
    int i, retval;
    retval = 0;
    for (i = 0; i < string_list_length(files); i++) {
        if(!(read_xdr_file(string_list_get(files,i)))) {
            retval = 1;
        }
    }
    return retval;
}

static
int read_sdd_file(char *file, int ldata) {
    int i, nerr, nun, retval;
    sac *s;
    s = NULL;
    retval = TRUE;
    i = 0;

    nerr = 0;

    if (!(s = sac_new())) {
        return FALSE;
    }
    s->m->filename = strdup(file);
    sacput(s);

    zopen_sac(&nun, file, -1, "RODATA", 7, &nerr);
    if (nerr) {
        goto error;
    }

    rdshdr(saclen(), &nun, &nerr);
    if (nerr) {
        goto error;
    }

    s->m->data_read = ldata;
    if (s->h->nevid == -12345 || s->h->norid == -12345) {
        cmdfm.nreadflag = LOW;
    }

    if (ldata) {
        defmem(saclen(), TRUE, &nerr);
        if (nerr) {
            goto error;
        }

        sac_alloc(s);

        rdsdta(saclen(), &nun, &nerr);
        if (nerr) {
            goto error;
        }

        zclose(&nun, &nerr);
    }
 error:
    if (nerr) {
        retval = FALSE;
        strcpy(kmdfm.kecbdf, "WARNING ");
        typmsg("WARNING");
        outmsg();
    }
    return retval;

}

static int
read_sdd(string_list * files, int ldata) {
    int i, retval;
    retval = FALSE;
    i = 0;
    while (i < string_list_length(files)) {
        if(!(read_sdd_file(string_list_get(files,i), ldata))) {
            string_list_delete(files, i);
            retval = TRUE;
        } else {
            i++;
        }
    }
    return retval;
}

double
time_tolerance_func(MS3Record *r) {
    UNUSED(r);
    return 0.0;
}
double
samprate_tolerance_func(MS3Record *r) {
    UNUSED(r);
    return 0.0;
}

datetime
sac_datetime(sac *s) {
    datetime start = {.year = s->h->nzyear,
                      .doy  = s->h->nzjday,
                      .hour = s->h->nzhour,
                      .minute = s->h->nzmin,
                      .second = s->h->nzsec,
                      .nanosecond = s->h->nzmsec * 1000000,
                      .set = DT_YEAR | DT_DOY,
    };
    return start;
}

int
read_sac_file(char *file, int ldata) {
    int nerr, retval;
    sac *s = NULL;
    retval = TRUE;
    s = NULL;
    nerr = 0;

    if(cmdfm.lcut) {
        s = sac_read_with_cut(file,
                              kmdfm.kcut[0],
                              cmdfm.ocut[0],
                              kmdfm.kcut[1],
                              cmdfm.ocut[1],
                              cmdfm.icuter, &nerr);
    } else {
        s = sac_read(file, &nerr);
    }
    if(!s) {
        error(nerr, "%s", file);
        return FALSE;
    }
    sacput(s);
    s->m->data_read = ldata;
    return TRUE;
}

void
display_file_list(string_list * files) {
    char *file, *dir, *base, *pdir;
    pdir = NULL;
    int i;
    for (i = 0; i < string_list_length(files); i++) {
        file = strdup(string_list_get(files, i));
        dir = strdup(dirname(file));
        FREE(file);
        if (strcmp(dir, ".") == 0) {    /* No directory */
            out(string_list_get(files, i));
        } else if (pdir && strcmp(dir, pdir) == 0) {    /* Same as previous directory */
            file = strdup(string_list_get(files, i));
            base = strdup(basename(file));
            out("...%s", base);
            FREE(file);
            FREE(base);
        } else {                /* With directory part */
            out(string_list_get(files, i));
            FREE(pdir);
            pdir = strdup(dir);
        }
        FREE(dir);
    }
    FREE(pdir);
}

/** 
 * Read a data filelist into memory
 * 
 * @param ldata 
 *    - TRUE read in the header and data
 *    - FALSE read in only the header
 * @param lmore 
 *    - TRUE files read in are to be appended to current list
 *    - FALSE files read in are to replace the current list
 * @param lsdd 
 *    - TRUE files are in SDD format
 *    - FALSE files are not in SDD format
 * @param kdirin 
 *    Default input directory if the filenames do not contain a directory part
 * @param kdirin_s 
 *    Length of \p kdirin
 * @param kdflin 
 *    Input file list
 * @param kdflin_s 
 *    Length of \o kdflin
 * @param ndflin 
 *    Number of entries in \p kdflin
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_NO_DATA_FILES_READ_IN
 *    - ERROR_UNABLE_TO_READ_SOME_FILES
 *
 * @date   920619:  Changed lfilesok args to use kdflrq, includes directory,
 *             and use " " for source directory. Bug introduced while
 *             fixing READ DIR option.
 * @date   920618:  Commented redundent appends that were probably added when
 *             readcss was implemented. Did not cause a problem unless
 *             "DIR" option was used in the read. Fixes bug reported 
 *             6/15/92 by Owens and others.
 * @date   920501:  Added memory delete on NOFILES and MEMORY DELETE READERR
 *             contitions.
 * @date   920418:  Added call to lfilesok, and moved memory setup stuff 
 *             into logical function, lfilesok. This avoids memory 
 *             shuffle when no files match the requested filelist.
 * @date   910115:  Added file count for current data-set.
 * @date   910826:  Changed kopen = ""  ---> kopen=' ' at 2 places to improve
 *             portability to the DEC 2000 workstations,
 *             per Gyu-sang Jang at UC Davis.
 * @date   910813:  Added multiple data-set stuff; changing many of the data
 *             structure names used for storing information about 
 *             pointers, component count, file size, etc.
 *
 * @note       Some of this may seem a little backwards in that some
 *             subroutines load info into working-storage, and the info
 *             is then copied to permanent data-set storage; But many 
 *             subroutines use the working storage and so this apporach 
 *             avoids making changes in many other subroutines.
 *
 * @bug This function is too long.  Many parts could be moved into separate 
 *      routines.
 *
 * @date   910610:  Fixed bug in opening file not referenced from pwd
 * @date   910222:  Better handling of bombed allamb -- back out to ndflsv.
 * @date   890417:  Fixed bug in processing of errors in read list.
 * @date   880308:  Added LDATA subroutine argument.
 * @date   870915:  Temporarily softened failure mode when memory requirement
 *             exceeds available room.  Still need to add buffered mode.
 * @date   870908:  Fixed bug in do loops limits after file deletion.
 * @date   870626:  Extracted from xr subroutine.
 * @date   860918:  Changed to character lists for data file list storage.
 * @date   860130:  Changed to new message handling package.
 * @date   850409:  Major rewrite due to addition of memory manager.
 * @date   850321:  Fixed bug involving MORE option and null DFL.
 * @date   830930:  Moved range calculation into it's own subroutine.
 * @date   811230:  Added MORE option.
 * @date   810708:  Added logic to compute range of dependent variable.
 * @date   810120:  Changed to output message retrieval from disk. 
 * @date   800512:  Reworked error messages.
 *
 */
void
readfl(int ldata, int lmore, int lsdd, char *kdirin, int kdirin_s,
       string_list * list, int *nerr) {

    int lexpnd, lrdrem;
    string_list *files;

    /* PROCEDURE: */
    *nerr = 0;

    /* - Convert KDFLIN which may contain wild-cards, predefined file sets,
     *   etc. into an expanded file list. */
    files = wildfl(kdirin, kdirin_s, list, &lexpnd);

    /* - Echo expanded filelist if requested. */
    if ((cmdfm.lechof && lexpnd) && string_list_length(files) > 0) {
        setmsg("OUTPUT", 0);
        display_file_list(files);
        wrtmsg(MUNOUT);
    }

    /* -- Test to see that at least one file exists. */
    if (!lfilesok(files, NULL, 0, lmore, FALSE, ftype == xdr, nerr)) {
        DEBUG("files not ok\n");
        /* --- If destroying files in memory, */
        if (strcmp(kmdfm.kecmem, "DELETE  ") == 0) {
            sacclear();
        }
        *nerr = ERROR_NO_DATA_FILES_READ_IN;
        goto L_8888;
    }

    if (!lmore) {
        sacclear();
    }

    lrdrem = FALSE;

    if (ftype == alpha) {
        cmdfm.nreadflag = LOW;
        lrdrem = read_alpha(files);
    } else if (ftype == segy) {
        cmdfm.nreadflag = LOW;
        lrdrem = read_segy(files);
    } else if (ftype == xdr) {
        lrdrem = read_xdr(files);
    } else if (lsdd) {
        lrdrem = read_sdd(files, ldata);
    } else {
        MS3TraceList *mst3k = NULL;

        // Auto-detect and read files
        int t = 0;
        for(int i = 0; i < string_list_length(files); i++) {
            char *file = string_list_get(files, i);
            int ret = 0;
            t = fid( file );
            if(t & FID_SAC) {
                ret = read_sac_file(file, ldata);
            } else if(t & FID_MSEED) {
                if(!mst3k) {
                    mst3k = mstl3_init(NULL);
                }
                ret = read_miniseed_file(mst3k, file);
            } else if(t & FID_SEGY) {
                cmdfm.nreadflag = LOW;
                ret = read_segy_file(file);
            } else if(t & FID_SACA) {
                cmdfm.nreadflag = LOW;
                ret = read_alpha_file(file);
            } else {
                // https://stackoverflow.com/a/650913 (access)
                // https://stackoverflow.com/a/3828537 (stat)
                struct stat sb;
                if(stat(file, &sb) == -1) {
                    printf(" WARNING: File does not exist: %s\n", file);
                } else if(! S_ISREG(sb.st_mode) ) {
                    printf(" WARNING: Not a regualr file: %s\n", file);
                } else {
                    printf(" WARNING: Unknown file type: %s [%d]\n", file, t);
                }
                lrdrem = 1;
            }
            if(!ret) {
                lrdrem = 1;
            }
        }
        if(mst3k) {
            sac **out = miniseed_trace_list_to_sac(mst3k);
            for(size_t i = 0; i < xarray_length(out); i++) {
                sacput(out[i]);
            }
            xarray_free(out);
            out = NULL;
            mstl3_free(&mst3k, 1);
            mst3k = NULL;
        }
    }

    /* - Check again for a non-null DFL. */
    if (saclen() <= 0) {
        *nerr = ERROR_NO_DATA_FILES_READ_IN;
        setmsg("ERROR", *nerr);
    }

    /* - Write a warning message if some of the files could not be read. */
    else if (lrdrem) {
        setmsg("WARNING", ERROR_UNABLE_TO_READ_SOME_FILES);
        apcmsg(" reading the rest of the files.", 32);
        outmsg();
        clrmsg();
    }

    /* - Calculate range of dependent variable for use during plotting. */
    setrng();

  L_8888:
    if(files) {
        string_list_free(files);
        files = NULL;
    }
    return;
}

