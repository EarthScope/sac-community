/** 
 * @file   wildfl.c
 * 
 * @brief  Expand a wildcard string
 * 
 */

#include <stdlib.h>
#include <string.h>

#include "amf.h"
#include "mach.h"
#include "wild.h"
#include "dfm.h"
#include "bool.h"

#include "string_utils.h"


#include "co.h"
#include "clf.h"
#include "bot.h"

/** 
 *  To convert a potentially wild-card laden input file list
 *     to an expanded output list of file names.
 *
 * @param kdfdir
 *    Input default directory
 *    Used if input file list has no directory part
 *    Set ot blanks if current directory is to be used
 * @param kdfdir_s
 *    Length of string \p kdfdir
 * @param kdflin
 *    Input file list, possible containing wild cards
 * @param kdflin_s
 *    Length of string \p kdflin
 * @param ndflin
 *    Number of entries in \p kdflin
 * @param kdflou
 *    Output file list after expanding wild cards
 * @param kdflou_s
 *    Length of \p kdflou
 * @param ndflou
 *    Number of entries in kdflou
 * @param lexpnd
 *    Set to .TRUE. if the output list differs from the 
 *    input list because of wild card expansion
 *
 * @date 970108:  Allows wildcards in the directory path now.  Also,
 *                much of the string handling now uses C routines.  maf
 * @date 961031:  Allows access to files in very large directories, and
 *                at higher speeds.  Most of the changes here are in
 *                support of changes in zfiles().  maf
 * @date 910703:  fotran2c interface which adds string length is buggy,
 *                changed to pass string lens explicitly to zfiles_();
 *                changed zfiles call to accept directory name that
 *                include dirname specified in the param dir
 * @date 900821   Changed global variable MODE_FILE_CASE to MODEFILECASE
 * @date 900611:  Changed first argument being passed to zfiles.
 *                Added filename case conversion logic.
 * @date 870619:  Fixed bug in building pathname.
 * @date 860918:  Original version.
 *xo
 * @date 900109:  Documented/Reviewed
 *
 */
string_list *
wildfl(char *kdfdir, 
       int   kdfdir_s, 
       string_list *list,
       int  *lexpnd) {

	char kdirin[MCPFN+1], kfile[MCPFN+1], kpatrn[MCPFN+1];
	int nc1, nc2, ncfdir, nerr = 0 ;
    char *s1;
    int i;

    string_list *files, *wild_files;

    files = string_list_init();
    wild_files = string_list_init();

	/* PROCECURE: */
	/* - Initialize the output file list. */
	*lexpnd = FALSE;

    memset(kdirin, ' ', MCPFN);
    memset(kfile, ' ', MCPFN);
    memset(kpatrn, ' ', MCPFN);

	kdirin[MCPFN] = '\0' ;
	kfile[MCPFN] = '\0' ;
	kpatrn[MCPFN] = '\0' ;

	/* - Loop on each entry in input file list. */
	  /* kdflin is a space delimited list of filenames possibly including paths
	     and wild cards, ic1 points is the index of the beginning of the current
	     filename and ic2 is the index of the end of the current filename. */
    for(i = 0; i < string_list_length(list); i++) {
        s1 = string_list_get(list, i);
		/* prepare s1 to see if entry contains any wild-cards */
		if( lwildc( s1 , strlen(s1)+1 ) ){	/* if there is/are wildcard(s) */
			/* --- Break entry into directory part and pattern part. */
			getdir( s1, strlen(s1)+1, kdirin,MCPFN+1, kpatrn,MCPFN+1 );
			/* --- If no directory name was typed, use the default one. */
			if ( strncmp ( kdirin , "        " , 8 ) == 0 ) 
			    strncpy ( kdirin , kdfdir , MCPFN+1 ) ;
            /*     Else if an absolute directory was typed, just use it. */
			else if ( kdirin[ 0 ] == '/' )
			    { /* do nothing */ }
			/*     Else if a relative directory was typed, append it to kdfdir. */
			else if ( strncmp ( kdfdir , "        " , 8 ) != 0 )
			{
			    char kTemp[ MCPFN ] , * pTemp = NULL ;

			    pTemp = strrchr ( kdfdir , '/' ) ;

			    if ( pTemp != NULL ) {
				int iTemp ;

				nc1 = indexb ( kdirin , MCPFN + 1 ) ;  /* return beginning of padding */
				iTemp = nc1 < MCPFN - ( pTemp - kdfdir ) ?
					nc1 : MCPFN - ( pTemp - kdfdir ) ;
				strncpy ( kTemp , kdfdir , pTemp - kdfdir + 1 ) ;
				strncat ( kTemp + ( pTemp - kdfdir + 1 ) , kdirin , iTemp ) ;
				strncpy ( kdirin , kTemp , MCPFN + 1 ) ;
			    }
			} 

			    
			/* --- Perform case conversion of directory and pattern if necessary. */
			nc1 = indexb ( kdirin , MCPFN + 1 ) ;
			nc2 = indexb ( kpatrn , MCPFN + 1 ) ;

			/* Here's a trick to make these strings behave like C strings. 
			   After zfiles is run, set these back to FORTRANesc strings. maf 961031*/
			kdirin[ nc1 < MCPFN+1 ? nc1 : MCPFN+1 ] = '\0' ;
			kpatrn[ nc2 < MCPFN+1 ? nc2 : MCPFN+1 ] = '\0' ;

      /* --- Get the list of files in the directory that
       *     match the regular expression. */
			wild_files = zfiles( kdirin, kpatrn, &nerr );
			if( nerr != 0 || ! wild_files)
				goto L_8888;

			/* set these strings back to their FORTRANish mode for future use. maf 961031 */
            kdirin[ nc1 < MCPFN+1 ? nc1 : MCPFN+1 ] = ' ' ;
            kpatrn[ nc2 < MCPFN+1 ? nc2 : MCPFN+1 ] = ' ' ;

			/* get the length of kdirin */
			cmdfm.ncdir = indexb( kdirin,MCPFN+1 );
			
            *lexpnd = TRUE;
            string_list_extend(files, wild_files);
            string_list_free(wild_files);
            wild_files = NULL;

		} else {
            /* -- Otherwise, entry is a simple file name. Add to output. */
			/* --- Break entry into directory part and name part. */
                      
			getdir( s1 , strlen(s1)+1, kdirin,MCPFN+1, kpatrn,MCPFN+1 );

			/* --- If no directory name was typed, use the default one.
			 *          if(kdirin.eq.' ')kdirin=kdfdir */
			cmdfm.ncdir = indexb( kdirin,MCPFN+1 );
			ncfdir = indexb( kdfdir,kdfdir_s );
			nc2 = indexb( kpatrn,MCPFN+1 );

			/* make arrays C-like for the following code. maf 970108 */
			kdirin[ cmdfm.ncdir < MCPFN+1 ? cmdfm.ncdir : MCPFN+1 ] = '\0' ;
			kdfdir[ ncfdir < MCPFN+1 ? ncfdir : MCPFN+1 ] = '\0' ;
			kpatrn[ nc2 < MCPFN+1 ? nc2 : MCPFN+1 ] = '\0' ;

			/* --- Recreate filename from directory and name parts. */
			if( ncfdir > 0 ){                    /* Default Directory Exists  */
				if( cmdfm.ncdir > 0 ){           /* Directory in Filename     */
				    if ( kdirin[ 0 ] == '/' ) {  /* Absolute Directory + File */
					strcpy ( kfile , kdirin ) ;
					strcat ( kfile , kpatrn ) ;
				    } else {                     /* Default Directory + Directory + File */
					strcpy ( kfile , kdfdir ) ;
					strcat ( kfile , kdirin ) ;
					strcat ( kfile , kpatrn ) ;
				    }
				} else {                         /* Default Directory + File*/
					strcpy ( kfile , kdfdir ) ;
					strcat ( kfile , kpatrn ) ;
				}
			}
			else{
				if( cmdfm.ncdir > 0 ){           /* Directory + File*/
					strcpy ( kfile , kdirin ) ;
					strcat ( kfile , kpatrn ) ;
				}
				else{                            /* File Only */
					strcpy ( kfile , kpatrn ) ;
				}
			}
            string_list_put(files, kfile, strlen(kfile));

			/* return strings to their FORTRANish state.  maf 970108 */
			kdirin[ cmdfm.ncdir < MCPFN+1 ? cmdfm.ncdir : MCPFN+1 ] = ' ' ;
			kdfdir[ ncfdir < MCPFN+1 ? ncfdir : MCPFN+1 ] = ' ' ;
                        kpatrn[ nc2 < MCPFN+1 ? nc2 : MCPFN+1 ] = ' ' ;

		} 

		if( nerr != 0 )
			break ;

	} /* end while */

L_8888:

    return files;
} 

