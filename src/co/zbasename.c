/** 
 * @file   zbasename.c
 * 
 * @brief  Get the SAC base directory
 * 
 */
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <sys/stat.h>

#include "config.h"
#include "co.h"
#include "string_utils.h"
#include "debug.h"

#ifdef WIN32
#include <windows.h>
#include <shlobj.h>
#endif

char *aux_tries[] = {
    "/usr/local/sac/aux/messages",
    "/usr/sac/aux/messages",
    "/opt/local/sac/aux/messages",
    "/opt/sac/aux/messages",
    NULL,
};

#define SET_VAR \
    "  To set the SACAUX enviornment variable:\n" \
    "  - For sh like shells:\n" \
    "    export SACAUX=/usr/local/sac/aux\n" \
    "  - For csh like shells:\n" \
    "     setenv SACAUX /usr/local/sac/aux\n" \
    "\n" \
    "  Consult the README file for setting up SAC for more details\n" 

#define AUX_NOT_FOUND \
    "SAC Error: aux directory not Found\n" \
    "  Could not find aux directory from SACAUX environmental variable,\n" \
    "  default location ["SACAUX"] or other probable locations.\n" \
    "\n" \
    SET_VAR 


#define AUX_ALT_FOUND \
    "SAC Warning: difficulty finding aux directory\n" \
    "  Could not find aux directory from SACAUX enviornmental variable or\n" \
    "  default location ["SACAUX"].\n"                 \
    "  Setting SACAUX will disable this warning.\n"\
    "\n" \
    SET_VAR \
    "\n"


char *
sacaux() {
    int i;
    struct stat st;
    static char *aux = NULL;
    char *tmp = NULL;
    if (aux) {
        return aux;
    }
    /* Enviornment Variable */
    if ((aux = getenv("SACAUX"))) {
        asprintf(&tmp, "%s/messages", aux);
        if(stat(tmp, &st) == 0) {
            FREE(tmp);
            return aux;
        }
    }
    /* Compiled Location */
    aux = strdup(SACAUX);
    asprintf(&tmp, "%s/messages", aux);
    if(stat(aux, &st) == 0) {
        FREE(tmp);
        return aux;
    }

    if(stat(aux, &st) == -1) {
        i = 0;
        while(aux_tries[i] && stat(aux_tries[i], &st) == -1) {
            i++;
        }
        if(!aux_tries[i]) {
            fprintf(stderr, AUX_NOT_FOUND);
            exit(-2);
        }
        aux = strdup(dirname(aux_tries[i]));
        fprintf(stderr, AUX_ALT_FOUND);
        fprintf(stderr, "  Found aux directory at: %s\n\n", aux);
    }
    return aux;
}

/** 
 * Get the name of the SAC base directory, will exit if the SACAUX variable
 *    is not defined.  String is padded with spaces and then null-terminated.
 * 
 * @param name 
 *    Expanded base directory on output
 * @param name_len 
 *    Length of \p name
 *
 * @date  04/19/87      Original version based upon zexpnd.
 * @date  04/22/87      Modified to blank fill returned base name.
 * @date  02/02/88      Modified to exit if SACAUX is not defined.
 *
 */
void
zbasename(char *name, int name_len) {

#ifndef WIN32
    char *aux = sacaux();
    if ((int) strlen(aux) > name_len - 1) {
        fprintf(stderr,
                "ERROR: Enviornment variable SACAUX too long: max: %d SACAUX: %d\n",
                name_len - 1, (int) strlen(aux));
        exit(1);
    }
    memset(name, ' ', name_len);
    name[name_len - 1] = 0;
    memcpy(name, aux, strlen(aux));
#else
    TCHAR wintemp[MAX_PATH];
    char *p;

    //GetProcessImageFileName(, wintemp, MAX_PATH);
    //QueryFullProcessImageName(
    GetModuleFileName(NULL, wintemp, MAX_PATH);

    p = wintemp;
    while (p = index(p, '\\')) {
        if (*(p + 1) != '\\') {
            *p = '/';
        }
        p++;
    }

    //getcwd(&wintemp[0], MAX_PATH);
    p = rindex(wintemp, '/');
    *p = 0;
    //p = rindex(wintemp,'/');
    //*p = 0;
    strncat(wintemp, "/winaux", 7);

    strcpy(name, wintemp);
    //name[strlen(wintemp)] = 0;
#endif

    return;
}
