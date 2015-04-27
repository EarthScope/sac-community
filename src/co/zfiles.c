/** 
 * @file   zfiles.c
 * 
 * @brief  Get a list of files
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

#ifndef WIN32
#include <glob.h>
#else
#include <windows.h>
#endif

#include "clf.h"
#include "co.h"
#include "bool.h"
#include "msg.h"

#include "errors.h"
#include "string_utils.h"
#include "debug.h"

/** 
 * Obtain a list of files in a directory given a regualr expression.
 *    Hidden files and sub-directories are not included.
 * 
 * @param kdirin 
 *    Directory to search
 * @param kpatrn 
 *    Regular Expression to be matched
 * @param files 
 *    Character array of files found. \p files is passed back to the 
 *      calling function. The calling function is responsible for
 *      freeing the memory allocated here.
 * @param nFiles 
 *    Number of files found
 * @param nErr 
 *    Error Return Flag
 *    - 0 on Success
 *    - ERROR_OUT_OF_MEMORY
 * 
 * @return 
 *    - TRUE on Successful completion, \p nFiles may be 0
 *    - FALSE on Error
 *
 * @date   961031:  Added kpatrn, a string (containing wile card characters)
 *                  which indicates the files to be opened. Reworked the way
 *                  that strings are used.  maf
 * @date   961018:  Overhauled to use dynamic memory allocation, and make
 *                  files local to the calling function.  Removed the 
 *                  variables faLen and stringLen.  files is now a pointer
 *                  to a pointer so that the pointer to the allocated space
 *                  is passed by reference.  maf
 * @date   910703:  Changed drLen, faLen to *drLen, *faLen
 *                  because fortran2c passing of string lens seems buggy
 *
 * @note Base for wildfl
 *
 */
string_list *
zfiles(char *kdirin, char *kpatrn, int *nErr) {

    char *command;
    string_list *files;
#ifdef WIN32
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind;
    char *file = NULL;
#else
    int i;
    glob_t g;
#endif

    *nErr = 0;
    files = string_list_init();

    asprintf(&command, "%s%s", kdirin, kpatrn);
    if (!command) {
        *nErr = ERROR_OUT_OF_MEMORY;
        error(*nErr, "Error: Insufficient memory to read in this directory");
        outmsg();
        clrmsg();
        string_list_free(files);
        return NULL;
    }

    *nErr = 0;

#ifdef WIN32
    hFind = FindFirstFile(command, &FindFileData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (strcmp(FindFileData.cFileName, ".") != 0 &&
                strcmp(FindFileData.cFileName, "..") != 0) {
                asprintf(&file, "%s%s", kdirin, FindFileData.cFileName);
                string_list_put(files, file, -1);
                FREE(file);
            }
        } while (FindNextFile(hFind, &FindFileData) != 0);
    }
#else
    glob(command, 0, NULL, &g);
    for (i = 0; i < (int) g.gl_pathc; i++) {
        string_list_put(files, g.gl_pathv[i], -1);
    }
    free(command);
    globfree(&g);
#endif
    return files;
}
