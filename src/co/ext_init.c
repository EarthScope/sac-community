/** 
 * @file   ext_init.c
 * 
 * @brief  Cause external header access routines to be loaded
 * 
 */

#include "co.h"
#include "extfunc.h"

int setup_args(int argc, char **argv, char **fargs, int *lenarg);
int setup_data(sac_files **call_data, float **fyinput, float **fxinput, int *numfiles, int *nptsmax);
int retrieve_data(sac_files **call_data, float *fyinput, float *fxinput, int numfiles, int nptsmax);

/** 
 * To cause external header access routines to be loaded.
 *          This routine should never actually be called.
 * 
 */
void 
ext_init() {
        int error, value, index;
        sac_header *this_header;
 
        this_header = makehdr( NULL );

        getehdr( *this_header, " ", &error );
        setehdr( *this_header, " ", 1, &error );
        getfhdr( *this_header, " ", &error );
        setfhdr( *this_header, " ", 1.0, &error );
        getnhdr( *this_header, " ", &error );
        setnhdr( *this_header, " ", 1, &error );
        getlhdr( *this_header, " ", &error );
        setlhdr( *this_header, " ", 1, &error);
        getahdr( *this_header, " ", &error );
        setahdr( *this_header, " ", " ", &error );

        index = 1;

        fgetahdr_(&index, " ", (char *)&value, &error, 1, 4);
        fgetehdr_(&index, " ", &value, &error, 1);
        fgetfhdr_(&index, " ", (float *)&value, &error, 1); 
        fgetlhdr_(&index, " ", &value, &error, 1);
        fgetnhdr_(&index, " ", &value, &error, 1);
        fsetahdr_(&index, " ", " ", &error, 1, 1);
        fsetehdr_(&index, " ", &value, &error, 1);
        fsetfhdr_(&index, " ", (float *)&value, &error, 1);
        fsetlhdr_(&index, " ", &value, &error, 1);
        fsetnhdr_(&index, " ", &value, &error, 1);

        setup_data( NULL, NULL, NULL, NULL, NULL);
        setup_args(0, NULL, NULL, NULL);
        retrieve_data(NULL, NULL, NULL, 0, 0);
 
	return;

}

