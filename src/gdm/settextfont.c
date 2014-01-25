
#include <stdio.h>
#include <string.h>

#include "gdm.h"
#include "mach.h"
#include "co.h"
#include "msg.h"
#include "bot.h"

#include "string_utils.h"

/** 
 * Set the Graphics Text Font
 *
 * @param ifont
 *    Number of the font to set
 *    Odd numbered fonts are italics
 *    Fonts are Hershey fonts
 *    Numbers greater than 8 are translated to numbers between 1 and 8
 *     - 1 -- Simplex Block.
 *     - 2 -- Simplex Italics.
 *     - 3 -- Duplex Block.
 *     - 4 -- Duplex Italics.
 *     - 5 -- Complex Block.
 *     - 6 -- Complex Italics.
 *     - 7 -- Triplex Block.
 *     - 8 -- Triplex Italics. 
 *
 * @date   870923:  Deleted ".saf" from aux file names.
 * @date   850205:  Original version.
 *
 */
void 
settextfont(int ifont)
{
	char kfile[MCPFN+1], kiline[MCMSG+1], fontbuf[11];
	int i, igtfn, j, jfont, ncerr, nerr;
        FILE *nun;

	for( i = 0 ; i < MCPFN ; i++ )
	    kfile[ i ] = ' ' ;
	kfile[ MCPFN ] = '\0' ;

	/* - This version assumes the existence of 4 files
	 *   containing the even numbered (block) Hershey fonts.
     *
     * - Ignore if the font is already loaded. */
	jfont = ((ifont - 1)%8) + 1;
	if( jfont == cmgdm.nfont || ((jfont - 1) == cmgdm.nfont && (jfont%
	 2) == 0) )
		goto L_8888;

	/* - If new font, create the file name,  open the file, and
	 *   read the data into the font tables. */

	zbasename( kfile,MCPFN+1 );
	crname( kfile,MCPFN+1, KSUBDL, "fonts",6, &nerr );
	if( nerr != 0 )
		goto L_9000;
	igtfn = (ifont + 1)/2;
	crname( kfile,MCPFN+1, KDIRDL, (char*)kmgdm.kgtfn[igtfn - 1]
	 ,41, &nerr );
	if( nerr != 0 )
		goto L_9000;
	zopens( &nun, kfile,MCPFN+1, "ROTEXT",7, &nerr );
	if( nerr != 0 )
		goto L_9000;

	/* - Read in base parameters. */

        if(fgetsp(kiline,MCMSG,nun)==NULL)goto L_9100;
        if(sscanf(kiline,"%3d%3d%3d%3d%5d",  &cmgdm.iofset, 
	 &cmgdm.ispace, &cmgdm.iyoff, &cmgdm.iyhigh, &cmgdm.maxstr) != 5){
          printf("error reading base parameters-settextfont\n");
          goto L_9100;
	}

	/* - Read in the ASCII lookup table and x-bounds. */

        fontbuf[10] = '\0';

	for( i = 1; i <= 121; i += 8 ){
                if(fgetsp(kiline,MCMSG,nun)==NULL)goto L_9100;
		for( j = i; j <= (i + 7); j++ ){
                   strncpy(fontbuf,kiline+((j-i)*10),10);
                   if(sscanf(fontbuf,"%4hd%3hd%3hd", &Ascstr[j], &Stxmin[j], 
			 &Stxmax[j] ) != 3){
                      printf("error reading ASCII lookup table-settextfont\n");
                      goto L_9100;
	           }
		}
	}

	/* - Read in the stroke table for the font. */

	for( i = 1; i <= (cmgdm.maxstr - 11); i += 12 ){
                if(fgetsp(kiline,MCMSG,nun)==NULL)goto L_9100;
		for( j = i; j <= (i + 11); j++ ){
                   if(sscanf(kiline+6+((j-i)*6),"%6hd",&Stroke[j] ) != 1){
                      printf("error reading stroke table-settextfont\n");
                      goto L_9100;
	           }
		}
	}

	/* - Close the font  file. */

	zcloses( &nun, &ncerr );

	/* - Save the font number and return. */

L_8888:
	cmgdm.nfont = ifont;
	return;

	/* - Process any errors here. */

L_9000:
	nerr = 2204;
	setmsg( "ERROR", nerr );
	apimsg( ifont );
	apcmsg( (char*)kmgdm.kgtfn[igtfn - 1],41 );
	goto L_8888;

L_9100:
	nerr = 2205;
	setmsg( "ERROR", nerr );
	apimsg( ifont );
	apcmsg( (char*)kmgdm.kgtfn[igtfn - 1],41 );
	goto L_8888;

} 

