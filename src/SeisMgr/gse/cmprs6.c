/*
C******************************************************************************
C                                                                       *
      blame goes to Mike Firpo, LLNL

      void cmprs6( LX, IX, CBUFF )
      int LX, *IX, ;
      char **CBUFF;

      a quick and dirty translation from FORTRAN to C of:

C     SUBROUTINE CMPRS6(LX,IX,LC,CBUFF,IERROR)                          *
C                                                                       *
C     ROUTINE TO COMPRESS INTEGER DATA INTO PRINTABLE ASCII CHARACTERS. *
C     INPUTS ARE INTEGER*4 ARRAY IX OF LENGTH LX.                       *
C     LC SHOULD CONTAIN THE DIMENSION OF THE ARRAY CBUFF.               *
C     OUTPUTS ARE CHARACTER*1 ARRAY CBUFF CONTAINING LC CHARACTERS.     *
C     IF THE ARRAY CBUFF IS NOT LARGE ENOUGH TO CONTAIN ALL OF THE DATA *
C     IERROR IS SET TO -1, OTHERWISE IT IS SET TO ZERO.                 *
C     OUTPUT IS PADDED TO A MULTIPLE OF 80 CHARACTERS USING SPACES.     *
C                                                                       *
C     METHOD OF COMPRESSION IS TO USE THE 6 LEAST SIGNIFICANT SIX BITS  *
C     OF AN EIGHT BIT BYTE SO THAT ALL DATA CAN BE TRANSMITTED AS ASCII *
C     CHARACTERS.                                                       *
C     OF THESE SIX BITS, THE MOST SIGNIFICANT IS USED AS A CONTINUATION *
C     BIT. IF IT IS SET TO ONE THE FOLLOWING BYTE ALSO FORMS PART OF    *
C     THE PRESENT SAMPLE. IF ZERO, THIS IS THE LAST BYTE IN THE SAMPLE. *
C     THE SECOND LEAST SIGNIFICANT BIT IS USED AS A SIGN BIT IN THE     *
C     FIRST BYTE AND FOR DATA IN SUBSEQUENT BYTES (ORIGINAL DATA IS     *
C     FIRST CONVERTED TO SIGN AND MAGNITUDE). ALL OTHER BITS OF FIRST   *
C     AND SUBSEQUENT BYTES FORM THE MAGNITUDE OF THE NUMBER.            *
C                                                                       *
C     TO ENABLE TRANSMISSION OVER AS MANY LINKS AS POSSIBLE DATA IS     *
C     FURTHER TRANSFORMED TO THE CHARACTERS +,-,0 - 9,A - Z, a - z      *
C     USING A LOOKUP TABLE.  These characters are stored in the string  *
C     ACHAR.                                                            *
C                                                                       *
C     DATA MAY BE DECOMPRESSED USING SUBROUTINE DCOMP6.                 *
C                                                                       *
C************************************************************************
*/

#include <stdio.h>
#include <stdlib.h>

#define EIGHTYONE 81
#define ZERO       0

void takeDiff( int *data, int npts)
{
        /* Takes the second difference */
        int     idx;

        for( idx = npts - 1 ; idx > 1 ; idx-- )
           data[ idx ] = data[ idx ] - 2 * data[ idx - 1 ] + data[ idx - 2 ] ;

        data[ 1 ] -= 2 * data[ 0 ] ;

}


static int room( npts, space , theString )
int *npts , *space ;
char **theString ;
{
   int rt = 0 ;

   if( *npts + 1 == *space ) {
      char *temp = NULL ;

      temp = ( char * ) realloc ( *theString , 
             ( *space + EIGHTYONE ) * sizeof ( char ) ) ;
      if ( temp == NULL ) {
         (*theString)[ *npts ] = '\0' ;
         rt = 1 ;
      }
      else{
         *theString = temp ;
         (*theString)[ *npts ] = '\n' ;
         (*npts)++ ;
         (*space) += EIGHTYONE ;
      }
   }

   return rt ;
}






int cmprs6( LX, IX, CBUFF )
int LX, *IX ;
char **CBUFF;
{
      /* Declare variables. */
      char BLANK = ' ',
           ACHAR[ ] = "+-0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz" ;

      int idx        ,   /* Index for loops */
          ierror = 0 ,   /* if non-zero, it's an error. */
          space  = 0 ,   /* Bytes allocated for CBUFF.  */
          ioff   = 0 ,   /* Offset of element in CBUFF. */
          J          ,   /* Offset of element in ACHAR. */
          JN         ,   /* An element from IX array.   */
          NFLAG      ;   /* Used to handle sign bits.   */
      int
           N4    =        16 ,   /* 2 raised to the 4th power */
           N5    =        32 ,   /* 2 raised to the 5th power */
           N5M1  =        31 ,   /* 2 raised to the 5th power minus 1 */
           N9    =       512 ,
           N10   =      1024 ,
           N10M1 =      1023 ,
           N14   =     16384 ,
           N15   =     32768 ,
           N15M1 =     32767 ,
           N19   =    524288 ,
           N20   =   1048576 ,
           N20M1 =   1048575 ,
           N24   =  16777216 ,
           N25   =  33554432 ,
           N25M1 =  33554431 ,
           N28M1 = 134217727 ;

      int MFLAG = N5M1 ;   /* Constant used to determine J (element of ACHAR) */

      *CBUFF = (char *) malloc( EIGHTYONE * sizeof( char ) ) ;
      if( !(*CBUFF) )
         return -1 ;

      space = EIGHTYONE ;

      for( idx = 0 ; idx < LX ; idx++ ){
        JN = IX[idx] ;
/*       SET NFLAG TO 1 TO POINT TO FIRST ELEMENT IN LOOKUP TABLE */
        NFLAG = 1 ;

/*       SEE IF NUMBER IS -VE IF SO CONVERT TO SIGN AND MAGNITUDE. */

        if(JN < 0) {
           NFLAG = NFLAG + N4 ;
           JN = -JN ;
        }
        if(JN < N4) goto L_50 ;
/*        IF HERE, DATA REQUIRES MORE THAN 1 BYTE */
         if(JN < N9) goto L_40 ;
/*         IF HERE, DATA REQUIRES MORE THAN 2 BYTES */
          if(JN < N14) goto L_30 ;
/*          IF HERE, DATA REQUIRES MORE THAN 3 BYTES. */
           if(JN < N19) goto L_20 ;
/*           IF HERE, DATA REQUIRES MORE THAN 4 BYTES. */
            if(JN < N24) goto L_10 ;
/*            IF HERE, DATA REQUIRES MORE THAN 5 BYTES. */
             if(JN > N28M1) JN=N28M1 ;
/*            FILL A BYTE IF HERE NUMBER WILL REQUIRE SIX BYTES */
             J = JN/N25 + NFLAG + MFLAG ;
             if( (ierror = room( &ioff, &space , CBUFF ) )) goto L_80 ;
             (*CBUFF)[ioff] = ACHAR[J] ;
             ioff++ ;
             JN &= N25M1 ;
             NFLAG = 1 ;
L_10:
/*           FIVE CHARACTERS TO GO */
            J = JN/N20 + NFLAG + MFLAG ;
            if(( ierror = room( &ioff, &space , CBUFF ) )) goto L_80 ;
            (*CBUFF)[ioff] = ACHAR[J] ;
            ioff++ ;
            JN &= N20M1 ;
            NFLAG = 1 ;
L_20:
/*          FOUR CHARACTERS TO GO */
           J = JN/N15 + NFLAG + MFLAG ;
           if(( ierror = room( &ioff, &space , CBUFF )) ) goto L_80 ;
           (*CBUFF)[ioff] = ACHAR[J] ;
           ioff++ ;
           JN &= N15M1 ;
           NFLAG = 1 ;
L_30:
/*         THREE CHARACTERS TO GO */
          J = JN/N10 + NFLAG + MFLAG ;
          if(( ierror = room( &ioff, &space , CBUFF ) )) goto L_80 ;
          (*CBUFF)[ioff] = ACHAR[J] ;
          ioff++ ;
          JN &= N10M1 ;
          NFLAG = 1 ;
L_40:
/*        TWO CHARACTERS TO GO */
         J = JN/N5 + NFLAG + MFLAG ;
         if(( ierror = room( &ioff, &space , CBUFF ) )) goto L_80 ;
         (*CBUFF)[ioff] = ACHAR[J] ;
         ioff++ ;
         JN &= N5M1 ;
         NFLAG = 1 ;
L_50:
/*       ONE CHARACTER TO GO */
        J = JN + NFLAG - 1 ;
        if(( ierror = room( &ioff, &space , CBUFF ) )) goto L_80 ;
        (*CBUFF)[ioff] = ACHAR[J] ;
        ioff++ ;

      } /* end for */



/*     NOW MAKE OUTPUT BUFFER UP TO A MULTIPLE OF 80 CHARACTERS */
/*     WITH BLANKS */
      for( idx = ioff ; idx < space - 1 ; idx++ )
         (*CBUFF)[ idx ] = BLANK ;

      (*CBUFF)[ idx ] = '\0' ;

L_80:
      return ierror ;
}

