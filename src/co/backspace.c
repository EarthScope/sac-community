/** 
 * @file   backspace.c
 * 
 * @brief  Rewind a stream
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** 
 * Rewind a stream by n characters
 * 
 * @param stream 
 *    Stream to rewind
 * @param n 
 *    Lines to rewind by
 *    Newlines consist of 
 *       - \n
 *       - \r\n
 *       - \r
 * 
 * @return 
 *    - 0 on Success
 *    - Non-Zero on Error
 *
 */
int 
backspace(FILE *stream, 
	  int   n) {

   char *buf;
   char *lastchar;

   int  savepos; 
   int nread, readpos, numread;

   /* Get current file position */
   savepos = ftell(stream);
   if(savepos == 0) 
     return(0);

   /* Allocate memory for buffer */
   if((buf = (char *)malloc(savepos+1)) == NULL){
      printf("error allocating memory in backspace\n");
      return (1);
   }
   memset(buf, 0, savepos+1);

   /* Flush current stream */
   fflush(stream);

   readpos = 0;
   numread = savepos;

   /* Go to the beginning of the file */
   fseek(stream, readpos, 0);
   /* Read up to the current file position in the file*/
   nread    = fread(buf, sizeof(char), (size_t)numread, stream);

   lastchar = &buf[nread-1];

   for( ; n>0; n--) {
     /* Order here is important - lastchar is moving backward in the stream
      *   Newlines consist of 
      *     - \n
      *     - \r\n
      *     - \r
      */
      if(*lastchar == '\n') {
        lastchar--;
        savepos--;
      }
      if(*lastchar == '\r') { 
        lastchar--;
        savepos--;
      }
      while (  lastchar >= buf  && 
              *lastchar != '\n' && 
              *lastchar != '\r') {
         lastchar--;
         savepos--;
      }
      if(lastchar <= buf) {
        break;
      }
   }

   /* Free buffer */
   free(buf);
   buf = NULL;

   /* Position place in file at current position - number of lines */
   if(fseek(stream,savepos,0) == 0 ) {
     return(0); /* Success */
   }
   
   return(1); /* Error */
}


