/** 
 * @file   xwtab.c
 * 
 * @brief  Write a Table Ascii file
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dfm.h"
#include "bool.h"
#include "hdr.h"
#include "amf.h"

#include "SacHeader.h"

#include "extfunc.h"


#include "wild.h"
#include "bot.h"
#include "ssi.h"
#include "msg.h"
#include "clf.h"
#include "cpf.h"
#include "co.h"
#include "dff.h"
#include "ncpf.h"

#define MALPHA  100
#define MAXCH   40
#define MBLKSZ  500
#define MENTRY  40
/** 
 * Execute the command WRITETABLE which reads in columns of data
 * 
 * @param nerr 
 *    Error Return Flag
 *    - 0 on Success
 *
 *    000419:  Original version, plagerized from xrtab.c.
 *
 */
void 
xwtab(int *nerr) {

    int i;
    char *tmp;
        char slash[2], kchange[ MCPFN+1 ], 
	     kfile[ MCPFN+1 ], kcdir[ MCPFN+1 ], 
	     kstring[ MCPFN+1 ], kdirpart[ MCPFN+1 ],
	     *cattemp, *strtemp1, *strtemp2, *strtemp3, junk[ 7 ] ;
	int nstr, nchg, nchange, nwrdir, nlen, ndx1, ndx2, nderr ;
        int lexpnd, liftype, lheader, lwrdir ;
        int idx, ic1, ic2, jdfl, nchar, nstring;
        FILE *nun;

        char *file;
        string_list *list;

        nun = 0;
        
        lwrdir = FALSE;

        *nerr = 0;
        lexpnd = TRUE;
        /* PARSING PHASE: */
        /* - Loop on each token in command: */

        list = string_list_init();

        while ( lcmore( nerr ) ){

            /* -- "DIR CURRENT|name":  set name of the default subdirectory. */
            if( lkchar( "DIR#$",6, MCPFN, kmdfm.kwrdir,MCPFN+1, &nchar ) ){
                lwrdir = TRUE;
                if(memcmp(kmdfm.kwrdir,"CURRENT",7) == 0 || 
                   memcmp(kmdfm.kwrdir,"current",7) == 0 ){
                    fstrncpy( kmdfm.kwrdir, MCPFN, " ", 1);
                }
                else if( kmdfm.kwrdir[nchar - 1] != KDIRDL ){
                    slash[0] = KDIRDL;
                    slash[1] = '\0';
                    subscpy( kmdfm.kwrdir, nchar, -1, MCPFN, slash );
                }
            }

            /* -- "IFTYPE": provide the data type, default is off. */
            else if( lklog( "&IFTYPE$",9, &liftype ) ) {
                cmdfm.liftype = liftype ;
            }

            /* -- "HEADER": prints begin and delta on a line at the top. */
            else if( lklog( "&HEADER$",9 , &lheader ) ) {
                cmdfm.lheader = lheader ;
            }


            /* -- "COMMIT|RECALLTRACE|ROLLBACK":
                  how to treat existing data */
            else if ( lckeyExact ( "COMMIT" , 7 ) )
                cmdfm.icomORroll = COMMIT ;
            else if (lckeyExact ( "RECALLTRACE" , 12 ) )
                cmdfm.icomORroll = RECALL ;
            else if ( lckeyExact ( "RECALL" , 7 ) )
                cmdfm.icomORroll = RECALL ;
            else if ( lckeyExact ( "ROLLBACK" , 9 ) )
                cmdfm.icomORroll = ROLLBACK ;

            /* generate names from the KSTCMP header field */
            else if( lckeyExact( "KSTCMP#$",9 ) ){
                lexpnd = FALSE;
                gennames("KSTCMP ",7, list,string_list_length(datafiles),nerr);
                if(*nerr != 0)
                    goto L_8888;
            }

            /* -- "APPEND string": append string to filenames */
            else if( lkcharExact( "APPEND#$",9, MCPFN, kstring,MCPFN+1,
                     &nstring ) ){
                ic1 = 0;
                ic2 = 0;
                for(i = 1; i <= cmdfm.ndfl; i++) {
                    tmp = string_list_get(datafiles, i-1);
                    appendstring(kstring, MCPFN+1, tmp, strlen(tmp)+1, kfile, MCPFN+1);
                    string_list_put(list, kfile, MCPFN+1);
                    if( *nerr != 0 )
                        goto L_8888;
                }
                cmdfm.lovrrq = FALSE;
                lexpnd = TRUE;
            }

            /* -- "PREPEND string": prepend string to filenames */
            else if( lkcharExact( "PREPEND#$",10, MCPFN, kstring,
                     MCPFN+1, &nstring ) ){
                ic1 = 0;
                ic2 = 0;
                for(i = 1; i <= cmdfm.ndfl; i++) {
                    tmp = string_list_get(datafiles, i-1);
                    strtemp1 = malloc(nstring+1);
                    strncpy(strtemp1,kstring,nstring);
                    strtemp1[nstring] = '\0';
                    prependstring( strtemp1, nstring+1, tmp,
                                   ic2-ic1+2, kfile,MCPFN+1 );

                    free(strtemp1);

                    string_list_put(list, kfile, MCPFN+1);
                    if( *nerr != 0 )
                        goto L_8888;
                }
                cmdfm.lovrrq = FALSE;
                lexpnd = TRUE;
            }

            /* -- "DELETE string": delete string from filenames */
            else if( lkcharExact( "DELETE#$",9, MCPFN, kstring,MCPFN+1,
                     &nstring ) ){
                ic1 = 0;
                ic2 = 0;
                for(i = 1; i <= cmdfm.ndfl; i++) {
                    strtemp1 = malloc(nstring+1);
                    strncpy(strtemp1,kstring,nstring);
                    strtemp1[nstring] = '\0';
                    strtemp2 = string_list_get(datafiles, i-1);
                    deletestring( strtemp1, nstring+1, strtemp2,
                                  ic2-ic1+2, kfile,MCPFN+1 );

                    free(strtemp1);
                    string_list_put(list, kfile, MCPFN+1);
                    if( *nerr != 0 )
                        goto L_8888;
                }
                cmdfm.lovrrq = FALSE;
                lexpnd = TRUE;
            }

            /* -- "CHANGE string1 string2": change string1 to string2 in filenames */
            else if( lkcharExact( "CHANGE#$",9, MCPFN, kstring,MCPFN+1,
                     &nstring ) ){
                lcchar( MCPFN, kchange,MCPFN+1, &nchange );
                ic1 = 0;
                ic2 = 0;
                for(i = 1; i <= cmdfm.ndfl; i++) {
                    nstr = indexb( kstring,MCPFN+1 );
                    nchg = indexb( kchange,MCPFN+1 );

                    strtemp1 = malloc(nstr+1);
                    strtemp2 = malloc(nchg+1);
                    strncpy(strtemp1,kstring,nstr);
                    strncpy(strtemp2,kchange,nchg);
                    strtemp1[nstr] = '\0';
                    strtemp2[nchg] = '\0';
                    strtemp3 = string_list_get(datafiles, i-1);
                    changestring( strtemp1, nstr+1, strtemp2, nchg+1,
                                  strtemp3, ic2-ic1+2, kfile,MCPFN+1 );

                    free(strtemp1);
                    free(strtemp2);
                    string_list_put(list, kfile, MCPFN+1);
                    if( *nerr != 0 )
                        goto L_8888;
                }
                cmdfm.lovrrq = FALSE;
                lexpnd = TRUE;
            }


            /* -- "filelist":  define a new filelist (or add to old filelist) */
            else if( ( list = lcdfl() ) ) 
            { /* do nothing */ }

            /* -- Bad syntax. */
            else{
                cfmt( "ILLEGAL OPTION:",17 );
                cresp();
            }
        }
        if( *nerr != 0 )
            goto L_8888;

        /* CHECKING PHASE: */
        /* - Check for null write filelist. */
        if( string_list_length(list) <= 0 ){
            *nerr = 1311;
            setmsg( "ERROR", *nerr );
            goto L_8888;
        }

        /* - Make sure the write filelist has as many entries as read filelist*/
        if( string_list_length(list) != cmdfm.ndfl ){
            *nerr = 1312;
            setmsg( "ERROR", *nerr );
            apimsg( string_list_length(list) );
            apimsg( cmdfm.ndfl );
            goto L_8888;
        }


        /* EXECUTION PHASE: */
        /* - Commit or rollback data according to cmdfm.icomORroll */
        alignFiles( nerr ) ;
        if( *nerr )
            return ;

        if( cmdfm.lechof && lexpnd ){
            setmsg( "OUTPUT", 0 );
            ic1 = 0;

            for(i = 0; i < string_list_length(list); i++) {
                file = string_list_get(list, i);
                getdir(file, strlen(file)+1, kcdir,MCPFN , kfile,MCPFN+1 );

                /* -- Echo the filename part if there is no directory part. */
                if( strcmp(kcdir,"        ") == 0 )
                        apcmsg( kfile,MCPFN+1 );

                /* -- Prepend the filename part with some special characters if
                 *    directory part is same as that of the previous file. */
                else if( kcdir[ 0 ] == '\0' ) {
                    cattemp = malloc(3+strlen(kfile)+1);
                    strcpy(cattemp, "...");
                    strcat(cattemp,kfile);
                    apcmsg( cattemp, 3+strlen(kfile)+1 );
                    free(cattemp);
                }
                /* -- Echo complete pathname if directory part is different. */
                else{
                    apcmsg2(file, strlen(file)+1);
                }
            }
            wrtmsg( MUNOUT );
        }


        /* - Write each file in memory to disk. */

        nwrdir = indexb( kmdfm.kwrdir,MCPFN+1 );
        for( jdfl = 1; jdfl <= cmdfm.ndfl; jdfl++ ){
            /* -- Get file from memory manager. */
            getfil( jdfl, TRUE, &nlen, &ndx1, &ndx2, nerr );
            if( *nerr != 0 )
                goto L_8888;

            /* isolate file name */
            file = string_list_get(list, jdfl-1);
            /* -- Check overwrite-protect flag in header record. */
            if( cmdfm.lovrrq && !*lovrok ){
                *nerr = 1303;
                setmsg( "ERROR", *nerr );
                apcmsg2(file, strlen(file)+1);
                outmsg () ;
                clrmsg () ;
                goto L_8888;
            }

            /* -- Prepare output file name:
             * --- If directory option is ON (lwrdir=.TRUE. and nwrdir>0),
             *     concatenate directory name with file name part of write file
             *     list.
             * --- If directory option is CURRENT (lwrdir=.TRUE. and nwrdir=0),
             *     use file name part of write file list.
             * --- If directory option is OFF, use write file list. */
            if( lwrdir ){
                if( nwrdir > 0 ){
                    fstrncpy( kfile, MCPFN, kmdfm.kwrdir,min(nwrdir,MCPFN));

                    strtemp2 = malloc(130-(nwrdir+1));
                    strncpy(strtemp2,kfile+nwrdir,MCPFN+1-(nwrdir + 1));
                    strtemp2[MCPFN+1-(nwrdir+1)] = '\0';

                    getdir(file, strlen(file)+1, 
                           kdirpart, MCPFN+1,
                           strtemp2,-(nwrdir+1)+130);
                    subscpy(kfile,nwrdir,-1,MCPFN,strtemp2);

                    free(strtemp2);
                }
                else{
                    fstrncpy( kfile, MCPFN, " ", 1);
                    getdir(file, strlen(file)+1, 
                           kdirpart,MCPFN+1, kfile,MCPFN+1);
                }
            }
            else
                fstrncpy( kfile, MCPFN, file, strlen(file)+1);

            /* - Create file. */

            zdest( kfile, strlen( kfile ) , &nderr );
            znfiles( &nun, kfile, strlen( kfile ) , "TEXT",5, nerr );
            if( *nerr != 0 )
                goto L_8888;

            if( cmdfm.liftype ) {
                /* Write the data type. */
                switch( *iftype ) {
                  case ITIME:  fprintf( nun, "Time Series\n" ) ;
                               break ;
    
                  case IRLIM:  fprintf( nun, "Spectral:  real/imaginary\n" ) ;
                               break ;
    
                  case IAMPH:  fprintf( nun, "Spectral:  amplitude/phase\n" ) ;
                               break ;
    
                  case IXY:    fprintf( nun, "XY file\n" ) ;
                               break ;
    
                  case IXYZ:   fprintf( nun, "XYZ file\n" ) ;
                               break ;
    
                  default:     if( ndx2 <= 1 )
                                   fprintf( nun, "No datatype specified:  treated as Time Series\n" ) ;
                               else
                                   fprintf( nun, "No datatype specified: treated as XY data\n" ) ;
                               break ;
                } /* end switch */
            } /* end if( cmdfm.liftype ) */
            
            if( cmdfm.lheader ) {
                /* if leven, write begin and delta */
                if( *leven ) {
                    fprintf( nun, "Begin time: %f ;  Delta time: %f\n" ,
                             *begin , *delta ) ;
                }
                else
                    fprintf( nun, "Unevenly spaced data.\n" ) ;

                if( *iftype == ITIME && *leven == TRUE ) {
                    for( idx = 0 ; idx < *npts ; idx++ ) {
                        fprintf( nun , "%0 13.6e\n" , cmmem.sacmem[ndx1][idx] );
                    }
		    fflush( nun ) ;
                }
                else {
                    for( idx = 0 ; idx < *npts ; idx++ ) {
                        fprintf( nun , "%0 13.6e\t%0 13.6e\n" ,
                                 cmmem.sacmem[ndx1][idx],
                                 cmmem.sacmem[ndx2][idx] );
                    }
		    fflush( nun ) ;
                }
            } /* end if( cmdfm.lheader ) */

            else if( *leven ){
               switch( *iftype ) {
                 case ITIME : /* one calculated column, one read column */
                   if( *begin == cmhdr.fundef || *delta == cmhdr.fundef ) {
                       /* error */
                       *nerr = 1393 ;
                       setmsg( "WARNING" , *nerr ) ;
                       sprintf( junk , " %d" , jdfl ) ;
                       apcmsg( junk , strlen( junk ) + 1 ) ;
                       outmsg() ;
                       clrmsg() ;
                       *nerr = 0 ;
                   } 
                   for( idx = 0 ; idx < *npts ; idx++ ) {
                       fprintf( nun , "%0 13.6e\t%0 13.6e\n" , 
                                *begin + ( *delta * (float) idx ) ,
                                cmmem.sacmem[ndx1][idx] ) ;
                   }
                   fflush( nun ) ;
                   break ;

                 case IRLIM : case IAMPH : case IXYZ : /* one calc, two read */
                   if( *begin == cmhdr.fundef || *delta == cmhdr.fundef ) {
                       /* error */
                       *nerr = 1393 ;
                       setmsg( "WARNING" , *nerr ) ;
                       sprintf( junk , " %d" , jdfl ) ;
                       apcmsg( junk , strlen( junk ) + 1 ) ;
                       outmsg() ;
                       clrmsg() ;
                       *nerr = 0 ;
                   }
                   for( idx = 0 ; idx < *npts ; idx++ ) {
                       fprintf( nun , "%0 13.6e\t%0 13.6e\t%0 13.6e\n" , 
                                *begin + ( *delta * (float) idx ) ,
                                cmmem.sacmem[ndx1][idx] ,
                                cmmem.sacmem[ndx2][idx] ) ;
                   }
                   fflush( nun ) ;
                   break ;

                 case IXY :  /* two read columns */
                   for( idx = 0 ; idx < *npts ; idx++ ) {
                       fprintf( nun , "%0 13.6e\t%0 13.6e\n" , 
                                cmmem.sacmem[ndx1][idx] ,
                                cmmem.sacmem[ndx2][idx] ) ;
                   }
                   fflush( nun ) ;
                   break ;

                 default : /* error */
                           *nerr = 1393 ;
                           setmsg( "WARNING" , *nerr ) ;
                           sprintf( junk , " %d" , jdfl ) ;
                           apcmsg( junk , strlen( junk ) + 1 ) ;
                           outmsg() ;
                           clrmsg() ;
                           *nerr = 0 ;
                           break ;
               } /* end switch */
            } /* end elseif( *leven ) */

            else {  /* lheader if false, *leven is false */
                switch( *iftype ) {
                  case ITIME : case IXY :  /* write 2 read columns */
                   for( idx = 0 ; idx < *npts ; idx++ ) {
                       fprintf( nun , "%0 13.6e\t%0 13.6e\n" ,
                                cmmem.sacmem[ndx1][idx] ,
                                cmmem.sacmem[ndx2][idx] ) ;
                   }
                   fflush( nun ) ;
                   break ;

                  case IRLIM: case IAMPH: case IXYZ: /* pretend it's leven */
                    if( *begin == cmhdr.fundef || *delta == cmhdr.fundef ) {
                        /* error */
                       *nerr = 1393 ;
                       setmsg( "WARNING" , *nerr ) ;
                       sprintf( junk , " %d" , jdfl ) ;
                       apcmsg( junk , strlen( junk ) + 1 ) ;
                       outmsg() ;
                       clrmsg() ;
                       *nerr = 0 ;
                    }
                    for( idx = 0 ; idx < *npts ; idx++ ) {
                       fprintf( nun , "%0 13.6e\t%0 13.6e\t%0 13.6e\n" , 
                                *begin + ( *delta * (float) idx ) ,
                                cmmem.sacmem[ndx1][idx] ,
                                cmmem.sacmem[ndx2][idx] ) ;
                    }
                    fflush( nun ) ;
                    break ;

                  default: /* error */
                           *nerr = 1393 ;
                           setmsg( "WARNING" , *nerr ) ;
                           sprintf( junk , " %d" , jdfl ) ;
                           apcmsg( junk , strlen( junk ) + 1 ) ;
                           outmsg() ;
                           clrmsg() ;
                           *nerr = 0 ;
                           break ;
                } /* end switch */
            }
        } /* end for ( jdfl ) */

L_8888:

        if(nun != 0) {
            zcloses( &nun, &nderr );
        }
        return;

}

