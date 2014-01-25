/*
Copyright (c) 2011, Brian Savage
All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, 
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the 
      documentation and/or other materials provided with the distribution.
    * The names of the contributors may be used to endorse or promote products 
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
POSSIBILITY OF SUCH DAMAGE.
*/

typedef struct _datetime datetime;

struct _datetime {
  int   year;        /* Year */
  int   month;       /* Month */
  int   day;         /* Day of the month: */
  int   doy;         /* Day of the year: 1 - 366 */
  int   hour;        /* Hour:   0 - 23 */ 
  int   minute;      /* Minute: 0 - 59 */
  int   second;      /* Second: 0 - 59 */
  long long int nanosecond;  /* Nanoseconds */
  long long int time;
  int   set;        
};

#define DT_YEAR       ( 1 << 0 )
#define DT_MONTH      ( 1 << 1 )
#define DT_DAY        ( 1 << 2 )
#define DT_DOY        ( 1 << 3 )
#define DT_HOUR       ( 1 << 4 )
#define DT_MINUTE     ( 1 << 5 )
#define DT_SECOND     ( 1 << 6 )
#define DT_NANOSECOND ( 1 << 7 )
#define DT_NORMALIZED ( 1 << 8 )

typedef enum _datetime_op datetime_op;
enum _datetime_op {  dt_LE, dt_GE, dt_EQ, dt_LT, dt_GT } ;

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

enum datetime_stati {
        DATETIME_OK            = 0,
        DATETIME_NOT_INIT,
        DATETIME_NOT_NORM,

};

datetime * datetime_parse          ( char *in, datetime *t );
void       datetime_init           ( datetime *t );
datetime * datetime_alloc          ( );
datetime * datetime_new            ( );
void       datetime_free           ( datetime *t );
datetime * datetime_copy           ( datetime *t );
void       datetime_copy_deep      ( datetime *new, datetime *t );
void       datetime_merge          ( datetime *to, datetime *from );
char *     datetime_string         ( datetime *t );
datetime * datetime_add_hms        ( datetime *t, int hour, int min, int sec );
int        datetime_compare        ( datetime *a, datetime *b, datetime_op op );
int        datetime_in_span        ( datetime *t, datetime *b, datetime *e );
void       datetime_normalize      ( datetime *t );
void       datetime_doy2ymd        ( datetime *t );
void       datetime_ymd2doy        ( datetime *t );
void       datetime_print          ( datetime *t );
void       datetime_printn         ( datetime *t );
void       datetime_print_date     ( datetime *t );
void       datetime_printn_date    ( datetime *t );
int        datetime_status         ( datetime *t );
char *     datetime_status_message ( int status );

void datetime_set_year       (datetime *t, int x);
void datetime_set_doy        (datetime *t, int x);
void datetime_set_month      (datetime *t, int x);
void datetime_set_day        (datetime *t, int x);
void datetime_set_hour       (datetime *t, int x);
void datetime_set_minute     (datetime *t, int x);
void datetime_set_second     (datetime *t, int x);
void datetime_set_nanosecond (datetime *t, int x);
