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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <math.h>

#include "datetime.h"

#define DATETIME_NA        "n/a"
#define DATETIME_NA_VALUE  "2999/365 23:59:59"
#define DATETIME_NA_FORMAT "%{y/d h:m:s}"

/* Internal variable for datetime_parse / datetime_strptime, 
   should be included into datetime structure */

char *strpfmt[] = {
  "y/m/d h:m:s.ms", "%Y/%m/%d %H:%M:%S.%P",
  "y/m/d h:m:s",    "%Y/%m/%d %H:%M:%S",
  "y/m/dTh:m:s.ms", "%Y/%m/%dT%H:%M:%S.%P",
  "y/m/dTh:m:s",    "%Y/%m/%dT%H:%M:%S",

  "y,m,d h:m:s.ms", "%Y,%m,%d %H:%M:%S.%P",
  "y,m,d h:m:s",    "%Y,%m,%d %H:%M:%S",
  "y,m,d,h:m:s.ms", "%Y,%m,%d,%H:%M:%S.%P",
  "y,m,d,h:m:s",    "%Y,%m,%d,%H:%M:%S",
  "y,m,dTh:m:s.ms", "%Y,%m,%dT%H:%M:%S.%P",
  "y,m,dTh:m:s",    "%Y,%m,%dT%H:%M:%S",

  "y-m-d h:m:s.ms", "%Y-%m-%d %H:%M:%S.%P",
  "y-m-d h:m:s",    "%Y-%m-%d %H:%M:%S",
  "y-m-d-h:m:s.ms", "%Y-%m-%d-%H:%M:%S.%P",
  "y-m-d-h:m:s",    "%Y-%m-%d-%H:%M:%S",
  "y-m-dTh:m:s.ms", "%Y-%m-%dT%H:%M:%S.%P",
  "y-m-dTh:m:s",    "%Y-%m-%dT%H:%M:%S",

  "y/d h:m:s.ms",   "%Y/%j %H:%M:%S.%P",
  "y/d h:m:s",      "%Y/%j %H:%M:%S",
  "y/dTh:m:s.ms",   "%Y/%jT%H:%M:%S.%P",
  "y/dTh:m:s",      "%Y/%jT%H:%M:%S",

  "y,d h:m:s.ms",   "%Y,%j %H:%M:%S.%P",
  "y,d h:m:s",      "%Y,%j %H:%M:%S",
  "y,d,h:m:s.ms",   "%Y,%j,%H:%M:%S.%P",
  "y,d,h:m:s",      "%Y,%j,%H:%M:%S",
  "y,dTh:m:s.ms",   "%Y,%jT%H:%M:%S.%P",
  "y,dTh:m:s",      "%Y,%jT%H:%M:%S",

  "y-d h:m:s.ms",   "%Y-%j %H:%M:%S.%P",
  "y-d h:m:s",      "%Y-%j %H:%M:%S",
  "y-d-h:m:s.ms",   "%Y-%j-%H:%M:%S.%P",
  "y-d-h:m:s",      "%Y-%j-%H:%M:%S",
  "y-dTh:m:s.ms",   "%Y-%jT%H:%M:%S.%P",
  "y-dTh:m:s",      "%Y-%jT%H:%M:%S",

  "y/d",            "%Y/%j",
  "y-d",            "%Y-%j",
  "y,d",            "%Y,%j",
  "y/m/d",          "%Y/%m/%d",
  "y-m-d",          "%Y-%m-%d",
  "y,m,d",          "%Y,%m,%d",
  "h:m:s.ms",       "%H:%M:%S.%P",
  "h:m:s",          "%H:%M:%S",
  
  "ydhms",          "%Y%j%H%M%S",
  "ymdhms",         "%Y%m%d%H%M%S",

  "N/A",            "%N",

};

void
year(datetime *t,int x) {
  t->year = x;
  t->set |= DT_YEAR;
  datetime_doy2ymd(t);
}
void
doy(datetime *t, int x) {
  t->doy = x;
  t->set |= DT_DOY;
  datetime_doy2ymd(t);
}

#define month(t,x)  do { t->month = x;      t->set |= DT_MONTH;      } while(0)
#define day(t,x)    do { t->day = x;        t->set |= DT_DAY;        } while(0)
#define hour(t,x)   do { t->hour = x;       t->set |= DT_HOUR;       } while(0)
#define minute(t,x) do { t->minute = x;     t->set |= DT_MINUTE;     } while(0)
#define second(t,x) do { t->second = x;     t->set |= DT_SECOND;     } while(0)
#define ns(t,x)     do { t->nanosecond = x; t->set |= DT_NANOSECOND; } while(0)

void
datetime_set_year(datetime *t, int x) {
    t->year = x;
    t->set  |= DT_YEAR;
}
void
datetime_set_doy(datetime *t, int x) {
    t->doy = x;
    t->set  |= DT_DOY;
}
void datetime_set_month      (datetime *t, int x) {  month(t,x);  };
void datetime_set_day        (datetime *t, int x) {  day(t,x);    };
void datetime_set_hour       (datetime *t, int x) {  hour(t,x);   };
void datetime_set_minute     (datetime *t, int x) {  minute(t,x); };
void datetime_set_second     (datetime *t, int x) {  second(t,x); };
void datetime_set_nanosecond (datetime *t, int x) {  ns(t,x);     };


#define REFERENCE_TIME     "0001/01/01 (001) 00:00:00.000"
#define NANOSECONDS        1000000000
#define DAYS_PER_YEAR      365   /* Nominal */
#define DAYS_4_YEARS       1461
#define DAYS_100_YEARS     36524
#define DAYS_400_YEARS     146097
#define SECONDS_PER_DAY    86400
#define SECONDS_PER_HOUR   3600
#define HOURS_PER_DAY      24
#define SECONDS_PER_MINUTE 60
#define MINUTES_PER_HOUR   60

int days_in_month[] = { 
  0, 31, 28, 31, 30,  31,  30,  31,  31,  30,  31,  30,  31 
};
int days_in_year[] = { 
  0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365
};

char *day_name[] = { "Sunday", 
                     "Monday", 
                     "Tuesday", 
                     "Wednesday", 
                     "Thursday", 
                     "Friday", 
                     "Saturday" 
};

int 
day_of_the_week(int y, int m, int d) {
  static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
  y -= m < 3;
  return (y + y/4 - y/100 + y/400 + t[m-1] + d) % 7;
}

int
datetime_day_of_the_week(datetime *t) {
  return day_of_the_week(t->year, t->month, t->day);
}

static long long int
hms2s(int hour, int min, int sec) {
  return hour * SECONDS_PER_HOUR + min * SECONDS_PER_MINUTE + sec;
}

static int
is_leap_year(int year) {
  if(year % 400 == 0) { return 1; } 
  if(year % 100 == 0) { return 0; }
  if(year %   4 == 0) { return 1; }
  return 0;
}

static long long int
days_before_year(int year) {
  long long int y;
  /* Total days in preceeding years
   *   Days without leap years
   *    + leap day every four years
   *    - no leap day every hundred years
   *    + leap day every 400 years
   *
   *  Years start on year 1, not year 0
   */
  y = year - 1;
  return y * DAYS_PER_YEAR + y/4 - y/100 + y/400;
}

static long long int
days_before_month(int year, int month) {
  long long int days;
  days = days_in_year[month];
  if(month > 2 && is_leap_year(year)) {
    days++;
  }
  return days;
}

static long long int
ymd2doy(int year, int month, int day) {
  return days_before_month(year, month) + day;
}

void
datetime_ymd2doy(datetime *t) {
  int d;
  if(! (t->set & DT_YEAR && 
        t->set & DT_MONTH && 
        t->set & DT_DAY)) {
    return;
  }
  d = ymd2doy(t->year, t->month, t->day);
  doy(t, d);
}

static void
doy2ymd(int year, int doy, int *month, int *day) {
  int m;
  long long int dbm;
  m = 1;
  dbm = days_before_month(year, m);
  while(dbm < doy) {
    m++;
    dbm = days_before_month(year, m);
  }
  m--;
  if(m <= 0) {
    m = 1;
  }
  *day = doy - days_before_month(year, m);
  *month = m;
}

void
datetime_doy2ymd(datetime *t) {
  int m, d;
  if(!(t->set & DT_YEAR && t->set & DT_DOY)) {
    return;
  }
  doy2ymd(t->year, t->doy, &m, &d);
  month(t, m);
  day(t, d);
}

static long long int
ymd2ord(int year, int month, int day) {
  return days_before_year(year) + ymd2doy(year, month, day);
}

static long long int 
ymdhms2s(int year, int month, int day, int hour, int min, int sec) {
  return (ymd2ord(year, month, day) - 1) * SECONDS_PER_DAY + hms2s(hour,min,sec);
}

static void
ord2ymd(int ord, int *year, int *month, int *day) {
  int n400, n100, n4, n1, n, i;
  n400 = (ord - 1) / DAYS_400_YEARS;
  n    = (ord - 1) % DAYS_400_YEARS;
  
  n100 = n / DAYS_100_YEARS;
  n    = n % DAYS_100_YEARS;
  
  n4 = n / DAYS_4_YEARS;
  n  = n % DAYS_4_YEARS;
  
  n1 = n / DAYS_PER_YEAR;
  n  = n % DAYS_PER_YEAR;

  *year = (n400 * 400) + (n100 * 100) + (n4 * 4) + n1 + 1;
  if(n1 == 4 || n100 == 4) {
    *year  -= 1;
    *month  = 12;
    *day    = 31;
    return;
  }
  n++;
  i = 1;
  while(n > days_before_month(*year, i+1)) {
    i++;
  }
  *month = i;
  *day = n - days_before_month(*year, *month);
  return;
}

int
datetime_atoi(char **p, int *pval, int lower, int upper) {
  int val;
  int n;

  if(**p < '0' || **p > '9') {
    return 0;
  }
  n   = 0;
  val = 0;
  do {
    val *= 10;
    val += **p - '0';
    (*p)++;
    n++;
  } while( **p != 0 && **p >= '0' && **p <= '9' && n < upper ) ;
  if(n < lower) {
    return 0;
  }
  *pval = val;
  return 1;
}

int
datetime_atol(char **p, long long int *pval, int lower, int upper) {
  long long int val;
  int n;

  if(**p < '0' || **p > '9') {
    return 0;
  }
  n   = 0;
  val = 0;
  do {
    val *= 10;
    val += **p - '0';
    (*p)++;
    n++;
  } while( **p != 0 && **p >= '0' && **p <= '9' && n < upper ) ;
  if(n < lower) {
    return 0;
  }
  *pval = val * pow(10, 9-n);
  return 1;
}

char *
datetime_strptime(char *buf, char *fmt, datetime *t) {
  int v;
  long long int vl;
  unsigned char c;
  char *b;

  if(!t) {
    return NULL;
  }

  b = buf;
  while ((c = *fmt) != '\0') {
    if(isspace(c)) {
      while(isspace(*b)) {
        b++;
      }
      fmt++;
      continue;
    }
    if((c = *fmt++) != '%') {
      if(c != *b++) {
        return NULL;
      }
      continue;      
    }

    switch(c = *fmt++) {
    case '%':
      if(c != *b++) { return NULL; } break;
    case '{': {
        int i;
        char *p, key[100];
        p = strchr(fmt, '}');
        if(!p) {
          return NULL;
        }
        strncpy(key, fmt, p-fmt);
        key[p-fmt] = 0;
        for(i = 0; i < (int)(sizeof(strpfmt) / sizeof(char *)); i += 2) {
          if(strcasecmp(key, strpfmt[i]) == 0) {
            if(!(b = datetime_strptime(b, strpfmt[i+1], t))) { 
              return NULL; 
            }
            break;
          }
        }
        fmt = p;
        fmt++;
      }
      break;
    case 'D': if(!(b = datetime_strptime(b, "%Y/%m/%d", t)))   { return NULL; } break;
    case 'E': if(!(b = datetime_strptime(b, "%m-%d-%Y", t)))   { return NULL; } break;
    case 'F': if(!(b = datetime_strptime(b, "%Y-%m-%d", t)))   { return NULL; } break;
    case 'J': if(!(b = datetime_strptime(b, "%Y/%j", t)))      { return NULL; } break;
    case 'K': if(!(b = datetime_strptime(b, "%Y,%j", t)))      { return NULL; } break;
    case 'T': if(!(b = datetime_strptime(b, "%H:%M:%S", t)))   { return NULL; } break;
    case 'X': if(!(b = datetime_strptime(b, "%H:%M:%S.%P", t))){ return NULL; } break;

    case 'N':
        if(strncasecmp(b, DATETIME_NA, 3) != 0) {
            return NULL;
        }
        b += strlen(DATETIME_NA);
        datetime_strptime(DATETIME_NA_VALUE, DATETIME_NA_FORMAT, t);
        break;
    case 'H': 
      if(! datetime_atoi(&b, &v, 1, 2)) { 
        return NULL; 
      } 
      hour(t, v);
      break;
    case 'M': 
      if(! datetime_atoi(&b, &v, 1, 2)) { 
        return NULL; 
      } 
      minute(t, v);
      break;
    case 'S': 
      if(! datetime_atoi(&b, &v, 1, 2) ) { 
        return NULL; 
      }
      ns(t, 0L);
      second(t, v);
      break;
    case 'P': 
      if(! datetime_atol(&b, &vl, 1, 9)) { 
        return NULL; 
      } 
      ns(t, vl);
      break;
    case 'd': 
      if(! datetime_atoi(&b, &v, 1, 2) ) {
        return NULL; 
      } 
      day(t, v);
      break;
    case 'j': 
      if(! datetime_atoi(&b, &v, 1, 3)) { 
        return NULL; 
      }
      doy(t, v);
      break;
    case 'm': 
      if(! datetime_atoi(&b, &v, 1, 2) ) { 
        return NULL; 
      } 
      month(t, v);
      break;
    case 'Y': 
      if(! datetime_atoi(&b, &v, 1, 4) ) {
        return NULL; 
      } 
      year(t, v);
      break;
    }
  }
  datetime_normalize( t );
  return b;
}

datetime *
datetime_parse(char *in, datetime *t) {
  char *p;
  int i;
  datetime tmp;
               
  datetime_init(&tmp);
    
  if(!t) {
    t = datetime_new();
    if(!t) {
      return t;
    }
  }
  
  for(i = 1; i <= (int)(sizeof(strpfmt)/sizeof(char*)); i+= 2) {
    datetime_init(&tmp);
    p = datetime_strptime(in, strpfmt[i], &tmp);
    if(p && *p == 0) {
      datetime_merge(t, &tmp);
      datetime_normalize(t);
      return t;
    }
  }
  datetime_free(t);
  return NULL;
}

void
datetime_init(datetime *t) {
  t->year       = 1;
  t->month      = 1;
  t->day        = 1;
  t->doy        = 1;
  t->hour       = 0;
  t->minute     = 0;
  t->second     = 0;
  t->nanosecond = 0L;
  t->time       = 0L;
  t->set        = 0;
}

datetime *
datetime_alloc() {
  datetime *t;
  t = (datetime*) malloc(sizeof(datetime));
  return t;
}

datetime * 
datetime_new() {
  datetime *t = datetime_alloc();
  if(t) {
    datetime_init(t);
  } else {
    fprintf(stderr, "Error allocating a datetime object\n");
  }
  return t;
}

void
datetime_merge(datetime *to, datetime *from) {
  if(from->set & DT_YEAR) {
    year(to, from->year);
  }
  if(from->set & DT_DAY) {
    day(to, from->day);
  }
  if(from->set & DT_MONTH) {
    month(to, from->month);
  }
  if(from->set & DT_HOUR) {  
    hour(to, from->hour);
  }
  if(from->set & DT_MINUTE) {
    minute(to, from->minute);
  }
  if(from->set & DT_SECOND) {
    second(to, from->second);
  }
  if(from->set & DT_NANOSECOND) {
    ns(to, from->nanosecond);
  }
  if(from->set & DT_DOY || 
     ( (from->set & DT_MONTH) && (from->set & DT_DAY) && (from->set & DT_YEAR))
     ) {  
    doy(to, from->doy);
  }
}

void
datetime_copy_deep( datetime *new, datetime *t ) {
  new->year       = t->year;
  new->month      = t->month;
  new->day        = t->day;
  new->doy        = t->doy;
  new->hour       = t->hour;
  new->minute     = t->minute;
  new->second     = t->second;
  new->nanosecond = t->nanosecond;
  new->time       = t->time;
  new->set        = t->set;
}

datetime * 
datetime_copy( datetime *t ) {
  datetime *new;
  if(!t) {
      return NULL;
  }
  new = datetime_new();
  if(new) {
    datetime_copy_deep(new, t);
  }
  return new;
}

void
datetime_free(datetime *t) {
  if(t) {
    free(t);
    t = NULL;
  }
}

char *
datetime_string(datetime *t) {
  char *s;
  s = NULL;
  asprintf(&s, "%04d/%02d/%02d (%03d) %02d:%02d:%02d.%03lld",
           t->year, t->month, t->day, t->doy,
           t->hour, t->minute, t->second,
           t->nanosecond / 1000000 );
  return s;
}

void
datetime_print_date(datetime *t) {
        fprintf(stdout, "%04d/%02d/%02d (%03d)", t->year, t->month, t->day, t->doy);
}
void
datetime_printn_date(datetime *t ) {
        datetime_print_date(t);
        fprintf(stdout, "\n");
}

void
datetime_print(datetime *t) {
        char *s;
        s = datetime_string(t);
        fprintf(stdout, "%s", s);
        free(s);
        s = NULL;
}
void
datetime_printn(datetime *t) {
        datetime_print(t);
        fprintf(stdout, "\n");
}


datetime *
datetime_add_hms(datetime *t, int hour, int min, int sec) {
  datetime *new;
  new = datetime_copy( t );
  if(new) {
    new->hour    += hour;
    new->minute  += min;
    new->second  += sec;
    datetime_normalize( new );
  }
  return new;
}

#define LE(a,b) datetime_compare(a,b, dt_LE)
#define GE(a,b) datetime_compare(a,b, dt_GE)
#define EQ(a,b) datetime_compare(a,b, dt_EQ)
#define LT(a,b) datetime_compare(a,b, dt_LT)
#define GT(a,b) datetime_compare(a,b, dt_GT)

int
datetime_status(datetime *t) {
        if(!t) {
                return DATETIME_NOT_INIT;
        }
        if(! (t->set & DT_NORMALIZED) ) {
                return DATETIME_NOT_NORM;
        }
        return DATETIME_OK;
}

static char *datetime_status_messages[] = 
        { 
                "datetime: ok", 
                "datetime: access attempt on uninitailzed object", 
                "datetime: access attempt on unnormalized object"
        };


char * 
datetime_status_message(int status) {
        return datetime_status_messages[ status ];
}


int
datetime_compare(datetime *a, datetime *b, datetime_op op) { 
        int stat;
        if((stat = datetime_status(a)) != DATETIME_OK) {
                fprintf(stderr, "%s\n", datetime_status_message( stat ));
                return 0;
        }
        if((stat = datetime_status(b)) != DATETIME_OK) {
                fprintf(stderr, "%s\n", datetime_status_message( stat ));
                return 0;
        }
  if(a->time == b->time) {
    switch (op) {
    case dt_LE: return a->nanosecond <= b->nanosecond; break;
    case dt_GE: return a->nanosecond >= b->nanosecond; break;
    case dt_EQ: return a->nanosecond == b->nanosecond; break;
    case dt_LT: return a->nanosecond <  b->nanosecond; break;
    case dt_GT: return a->nanosecond >  b->nanosecond; break;
    default: break;
    }
  }
  switch (op) {
  case dt_LE: return a->time <= b->time; break;
  case dt_GE: return a->time >= b->time; break;
  case dt_EQ: return a->time == b->time; break;
  case dt_LT: return a->time <  b->time; break;
  case dt_GT: return a->time >  b->time; break;
  default: break;
  }
  return 0;
}

int
datetime_in_span(datetime *t, datetime *b, datetime *e) {
  if( GT(b,e) ) {
    return 0;
  }
  if( GE(t,b) && LE(t,e) ) {
    return 1;
  }
  return 0;
}


void
datetime_normalize_ymd(datetime *t) {
  int y, m, ord;
  m = t->month - 1;
  if(m >= 12 || m <= 0) {
    y = floor(m / 12); /* Integer years */
    m = m - y * 12;    /* Remaining months */
    t->year += y;
    t->month = m;
  }
  t->month = m + 1;
  
  ord = ymd2ord(t->year, t->month, 1) + t->day - 1;
  ord2ymd(ord, &t->year, &t->month, &t->day);
}

void
norm(int *big, int *small, int size) {
  int new_big, new_small;
  new_big = floor(*small / size);
  if(*small < 0) {
    new_big -= 1;
  }
  new_small = *small - new_big * size;
  *big += new_big;
  *small = new_small;
  if(*small == size) { /* ???? */
    norm(big, small, size);
  }
}

void
datetime_normalize(datetime *t) {
  //norm(&t->second, &t->nanosecond, NANOSECONDS);
  norm(&t->minute, &t->second,     SECONDS_PER_MINUTE);
  norm(&t->hour,   &t->minute,     MINUTES_PER_HOUR);
  norm(&t->day,    &t->hour,       HOURS_PER_DAY);
  datetime_normalize_ymd(t);
  t->doy    = ymd2doy(t->year, t->month, t->day);
  t->time   = ymdhms2s(t->year, t->month, t->day, t->hour, t->minute, t->second);
  t->set |= DT_NORMALIZED;
}


#ifdef __UNIT_TESTING__

#include <stdarg.h>

void
ok(int pass, char *fmt, ...) {
  va_list ap;
  if(! pass) {
    fprintf(stderr, "Fail: ");
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fprintf(stderr, "\n");
  }
}
#define CLEAR "clear", "" 

char *tests[] = {
  "1994/01/13 12:13:45.123", "1994/01/13 (013) 12:13:45.123",
  "1/01/01 00:00:00.000",    "0001/01/01 (001) 00:00:00.000",
  "1813/13 00:00:00.000",    "1813/01/13 (013) 00:00:00.000",
  CLEAR,
  "01:02:03",                "0001/01/01 (001) 01:02:03.000",
  "01:02:03.124",            "0001/01/01 (001) 01:02:03.124",
  "01:02:03.1",              "0001/01/01 (001) 01:02:03.100",
  "01:02:03.12",             "0001/01/01 (001) 01:02:03.120",
  "01:02:03.123",            "0001/01/01 (001) 01:02:03.123",
  "01:02:03.1234",           "0001/01/01 (001) 01:02:03.123",
  "01:02:03.12345",          "0001/01/01 (001) 01:02:03.123",
  "01:02:03.0",              "0001/01/01 (001) 01:02:03.000",
  "01:02:03.123",            "0001/01/01 (001) 01:02:03.123",
  "1812/12",                 "1812/01/12 (012) 01:02:03.123",
  "1812/12/14",              "1812/12/14 (349) 01:02:03.123",
  "1811/12/14",              "1811/12/14 (348) 01:02:03.123",
  "1812/2/28",               "1812/02/28 (059) 01:02:03.123",
  "1811/2/28",               "1811/02/28 (059) 01:02:03.123",
  "1812/2/29",               "1812/02/29 (060) 01:02:03.123",
  "1811/2/29",               "1811/03/01 (060) 01:02:03.123",
  "1812/3/01",               "1812/03/01 (061) 01:02:03.123",
  "1811/3/01",               "1811/03/01 (060) 01:02:03.123",
  "1812/3/02",               "1812/03/02 (062) 01:02:03.123",
  "1811/3/02",               "1811/03/02 (061) 01:02:03.123",
  "01:60:03",                "1811/03/02 (061) 02:00:03.000",
  "01:61:03",                "1811/03/02 (061) 02:01:03.000",
  "19940113121345",          "1994/01/13 (013) 12:13:45.000",
  "1994013121345",           "1994/01/13 (013) 12:13:45.000",

  "1994/04/15 01:02:03.356", "1994/04/15 (105) 01:02:03.356", CLEAR,
  "1994/04/15 01:02:03",     "1994/04/15 (105) 01:02:03.000", CLEAR,
  "1994/04/15T01:02:03.356", "1994/04/15 (105) 01:02:03.356", CLEAR,
  "1994/04/15T01:02:03",     "1994/04/15 (105) 01:02:03.000", CLEAR,

  "1994,04,15 01:02:03.356", "1994/04/15 (105) 01:02:03.356", CLEAR,
  "1994,04,15 01:02:03",     "1994/04/15 (105) 01:02:03.000", CLEAR,
  "1994,04,15,01:02:03.356", "1994/04/15 (105) 01:02:03.356", CLEAR,
  "1994,04,15,01:02:03",     "1994/04/15 (105) 01:02:03.000", CLEAR,
  "1994,04,15T01:02:03.356", "1994/04/15 (105) 01:02:03.356", CLEAR,
  "1994,04,15T01:02:03",     "1994/04/15 (105) 01:02:03.000", CLEAR,

  "1994-04-15 01:02:03.356", "1994/04/15 (105) 01:02:03.356", CLEAR,
  "1994-04-15 01:02:03",     "1994/04/15 (105) 01:02:03.000", CLEAR,
  "1994-04-15-01:02:03.356", "1994/04/15 (105) 01:02:03.356", CLEAR,
  "1994-04-15-01:02:03",     "1994/04/15 (105) 01:02:03.000", CLEAR,
  "1994-04-15T01:02:03.356", "1994/04/15 (105) 01:02:03.356", CLEAR,
  "1994-04-15T01:02:03",     "1994/04/15 (105) 01:02:03.000", CLEAR,

  "1994/105 01:02:03.356", "1994/04/15 (105) 01:02:03.356", CLEAR,
  "1994/105 01:02:03",     "1994/04/15 (105) 01:02:03.000", CLEAR,
  "1994/105T01:02:03.356", "1994/04/15 (105) 01:02:03.356", CLEAR,
  "1994/105T01:02:03",     "1994/04/15 (105) 01:02:03.000", CLEAR,

  "1994,105 01:02:03.356", "1994/04/15 (105) 01:02:03.356", CLEAR,
  "1994,105 01:02:03",     "1994/04/15 (105) 01:02:03.000", CLEAR,
  "1994,105,01:02:03.356", "1994/04/15 (105) 01:02:03.356", CLEAR,
  "1994,105,01:02:03",     "1994/04/15 (105) 01:02:03.000", CLEAR,
  "1994,105T01:02:03.356", "1994/04/15 (105) 01:02:03.356", CLEAR,
  "1994,105T01:02:03",     "1994/04/15 (105) 01:02:03.000", CLEAR,

  "1994-105 01:02:03.356", "1994/04/15 (105) 01:02:03.356", CLEAR,
  "1994-105 01:02:03",     "1994/04/15 (105) 01:02:03.000", CLEAR,
  "1994-105-01:02:03.356", "1994/04/15 (105) 01:02:03.356", CLEAR,
  "1994-105-01:02:03",     "1994/04/15 (105) 01:02:03.000", CLEAR,
  "1994-105T01:02:03.356", "1994/04/15 (105) 01:02:03.356", CLEAR,
  "1994-105T01:02:03",     "1994/04/15 (105) 01:02:03.000", CLEAR,

  "1994/105",              "1994/04/15 (105) 00:00:00.000", CLEAR,
  "1994-105",              "1994/04/15 (105) 00:00:00.000", CLEAR,
  "1994,105",              "1994/04/15 (105) 00:00:00.000", CLEAR,

  "1994/04/15",            "1994/04/15 (105) 00:00:00.000", CLEAR,
  "1994-04-15",            "1994/04/15 (105) 00:00:00.000", CLEAR,
  "1994,04,15",            "1994/04/15 (105) 00:00:00.000", CLEAR,

  "01:02:03.356",          "0001/01/01 (001) 01:02:03.356", CLEAR,
  "01:02:03",              "0001/01/01 (001) 01:02:03.000", CLEAR,

  "1994105010203",         "1994/04/15 (105) 01:02:03.000", CLEAR,
  "19940415010203",        "1994/04/15 (105) 01:02:03.000", CLEAR,
  "N/A",                   "2999/12/31 (365) 23:59:59.000", CLEAR,
  "n/a",                   "2999/12/31 (365) 23:59:59.000", CLEAR,
};

void
check_parse() {
  int i;
  char *s;
  datetime *t;
  s = NULL;
  t = datetime_new();
  for(i = 0; i < (int)(sizeof(tests)/sizeof(char *)); i+= 2) {
    if(strcmp(tests[i], "clear") == 0) {
      datetime_init( t );
    } else {
      datetime_parse( tests[i], t);
      s = datetime_string(t);
      ok(strcmp(tests[i+1], s) == 0, "%s\n      %s [%s]", s,tests[i+1], tests[i]);
      if(s) {
          free(s);
      }
      s = NULL;
    }
  }
  datetime_free(t);
}

char *at[] = {
  "01:02:03", "0","0","0",   "0001/01/01 (001) 01:02:03.000",
                             "0001/01/01 (001) 01:02:03.000",
  "01:02:03", "0","0","1",   "0001/01/01 (001) 01:02:03.000",
                             "0001/01/01 (001) 01:02:04.000",
  "01:02:03", "0","1","0",   "0001/01/01 (001) 01:02:03.000",
                             "0001/01/01 (001) 01:03:03.000",
  "01:02:03", "1","0","0",   "0001/01/01 (001) 01:02:03.000",
                             "0001/01/01 (001) 02:02:03.000",
  "01:02:03", "0","0","60",  "0001/01/01 (001) 01:02:03.000",
                             "0001/01/01 (001) 01:03:03.000",
  "01:02:03", "0","0","61",  "0001/01/01 (001) 01:02:03.000",
                             "0001/01/01 (001) 01:03:04.000",
  "01:02:03", "24","0","60", "0001/01/01 (001) 01:02:03.000",
                             "0001/01/02 (002) 01:03:03.000",
  "01:02:03", "720","0","0", "0001/01/01 (001) 01:02:03.000",
                             "0001/01/31 (031) 01:02:03.000",
  "1812/13", "0","0","60",   "1812/01/13 (013) 01:02:03.000",
                             "1812/01/13 (013) 01:03:03.000",
  "01:02:03", "720","0","0", "1812/01/13 (013) 01:02:03.000",
                             "1812/02/12 (043) 01:02:03.000",

};

void
check_add() {
  int i;
  char *s;
  datetime *t, *n;

  t = datetime_new();
  for(i = 0; i < (int)(sizeof(at)/sizeof(char *)); i+= 6) {
    if(strcmp(at[i], "clear") == 0) {
      datetime_init( t );
    } else {
      datetime_parse( at[i], t);
      s = datetime_string(t);
      ok(strcmp(at[i+4], s) == 0, "%s\n      %s", s,at[i+4]);
      free(s);
      s = NULL;

      n = datetime_add_hms(t, atoi(at[i+1]), atoi(at[i+2]), atoi(at[i+3]));
      s = datetime_string(n);
      ok(strcmp(at[i+5], s) == 0, "%s (add)\n      %s", s,at[i+5]);
      datetime_free(n);
      free(s);
      s = NULL;
    }
  }
  datetime_free(t);
  
}

void
check_hms2s() {
  int i,j,k;
  for(i = 0; i < 100; i++) {
    for(j = 0; j < 100; j++) {
      for(k = 0; k < 100; k++) {
        ok(hms2s(i,j,k) == i * 3600 + j * 60 + k, "h:m:s %d %d %d", i,j,k);
      }
    }
  }
}

void 
check_leap() {
  int i;
  int leap;
  for(i = 0; i <= 10000; i++) {
    leap = 0;
    if(i%4   == 0) { leap = 1; }
    if(i%100 == 0) { leap = 0; }
    if(i%400 == 0) { leap = 1; }
    ok(is_leap_year(i) == leap, "leap: %d %d %d", i, leap == is_leap_year(i), leap);
  }
}

void
check_dbm() {
  int y;
  int d;
  for(y = 3; y <= 4; y++) {
    d = 0;
    ok(days_before_month(y,  0) == d, "days before month: %d", 0);
    ok(days_before_month(y,  1) == d, "days before month: %d", 1);
    d += 31;
    ok(days_before_month(y,  2) == d, "days before month: %d", 2);
    d += 28;
    if(y == 4) {
      d ++;
    }
    ok(days_before_month(y,  3) == d, "days before month: %d", 3);
    d += 31;
    ok(days_before_month(y,  4) == d, "days before month: %d", 4);
    d += 30;
    ok(days_before_month(y,  5) == d, "days before month: %d", 5);
    d += 31;
    ok(days_before_month(y,  6) == d, "days before month: %d", 6);
    d += 30;
    ok(days_before_month(y,  7) == d, "days before month: %d", 7);
    d += 31;
    ok(days_before_month(y,  8) == d, "days before month: %d", 8);
    d += 31;
    ok(days_before_month(y,  9) == d, "days before month: %d", 9);
    d += 30;
    ok(days_before_month(y, 10) == d, "days before month: %d", 10);
    d += 31;
    ok(days_before_month(y, 11) == d, "days before month: %d", 11);
    d += 30;
    ok(days_before_month(y, 12) == d, "days before month: %d", 12);
    d += 31;
  }
}

void
check_ymd2doy() {
  int doy, y, m, d, leap, mon, day;
  for(y = 3; y <= 4; y++) {
    doy = 0;
    for(m = 1; m <= 12; m++) {
      leap = 0;
      if(is_leap_year( y ) && m == 2) {
        leap = 1;
      }
      for(d = 1; d <= days_in_month[m]+leap; d++) {
        doy++;
        ok(ymd2doy(y,m,d) == doy, "ymd2doy %d/%d/%d <=> %d", y,m,d,doy);
        doy2ymd(y, doy, &mon, &day);
        ok(m == mon, "doy2ymd mon (*)%d <=> %d %d", m, mon, doy);
        ok(d == day, "doy2ymd day (*)%d <=> %d", d, day);
      }
    }
  }
}

void
check_sec() {
  int y,m,d,h,mi,s,leap;
  long long int k, n;
  datetime t;
  k = (60 * 60 * 24 * 365 * (3-1)) ;
  k = k - 1;
  for(y = 3; y <= 4; y++) {
    for(m = 1; m <= 12; m++) {
      leap = 0;
      if(is_leap_year(y) && m == 2) {
        leap = 1;
      }
      for(d = 1; d <=days_in_month[m]+leap; d++) {
        for(h = 0; h <= 23; h++) {
          for(mi = 0; mi <= 59; mi++) {
            for(s = 0; s <= 59; s++) {
              k++;
              n = ymdhms2s(y,m,d,h,mi,s);
              ok(n == k, "%d/%d/%d %d:%d:%d %lld <=> %lld", y,m,d,h,mi,s,k,n);
              datetime_init(&t);
              t.second = n;
              datetime_normalize( &t );
              ok(t.year == y, "year: %d <=> %d", t.year, y);
              ok(t.month == m, "month: %d <=> %d", t.month, m);
              ok(t.day == d, "day: %d <=> %d", t.day, d);
              ok(t.hour == h, "hour: %d <=> %d", t.hour, h);
              ok(t.minute == mi, "minute  %d <=> %d", t.minute, mi);
              ok(t.second == s, "second: %d <=> %d", t.second, s);
            }
          }
        }
      }
    }
  }
}

char *strp[] = {
  "2",          "%Y",       "0002/01/01 (001) 00:00:00.000",
  "20",         "%Y",       "0020/01/01 (001) 00:00:00.000",
  "200",        "%Y",       "0200/01/01 (001) 00:00:00.000",
  "2000",       "%Y",       "2000/01/01 (001) 00:00:00.000",
  "2",          "%m",       "0001/02/01 (032) 00:00:00.000",
  "12",         "%m",       "0001/12/01 (335) 00:00:00.000",
  "2",          "%d",       "0001/01/02 (002) 00:00:00.000",
  "20",         "%d",       "0001/01/20 (020) 00:00:00.000",
  "45",         "%d",       "0001/02/14 (045) 00:00:00.000",
  "14",         "%d",       "0001/01/14 (014) 00:00:00.000",
  "3/2/2",      "%Y/%m/%d", "0003/02/02 (033) 00:00:00.000",
  "3/2/2",      "%D",       "0003/02/02 (033) 00:00:00.000",
  "1:2:4",      "%T",       "0001/01/01 (001) 01:02:04.000",
  "1:2:40",     "%T",       "0001/01/01 (001) 01:02:40.000",
  "1994/123",   "%{y/d}",   "1994/05/03 (123) 00:00:00.000",
  "20:2:40",    "%T",       "0001/01/01 (001) 20:02:40.000",
  "1",          "%P",       "0001/01/01 (001) 00:00:00.100",
  "12",         "%P",       "0001/01/01 (001) 00:00:00.120",
  "123",        "%P",       "0001/01/01 (001) 00:00:00.123",
  "1234",       "%P",       "0001/01/01 (001) 00:00:00.123",
  "12.1234",    "%S.%P",    "0001/01/01 (001) 00:00:12.123",
  "34:12.1234", "%M:%S.%P", "0001/01/01 (001) 00:34:12.123",
  "23:34:12.1234", "%X",    "0001/01/01 (001) 23:34:12.123",
  "23:34:12.1234", "%H:%M:%S.%P", "0001/01/01 (001) 23:34:12.123",
  "23:34:12.1234", "%{h:m:s.ms}", "0001/01/01 (001) 23:34:12.123",
  "2002/3/4 23:34:12.1234", "%{y/m/d h:m:s.ms}", "2002/03/04 (063) 23:34:12.123",

};

void
check_strptime() {
  int i;
  char *s, *p;
  datetime *t;
  t = datetime_new();

  for(i = 0; i < (int)(sizeof(strp) / sizeof(char *)); i+= 3) {
    datetime_init( t );
    p = datetime_strptime(strp[i], strp[i+1], t);
    if(!p) {
      ok(p != NULL, "parse error: %s [%s]\n", strp[i], strp[i+1]);
      continue;
    }
    ok(*p == '\0', "leftovers: '%s'\n", p);
    s = datetime_string(t);
    ok(strcmp(strp[i+2], s) == 0, "%s\n      %s",s,strp[i+2]);
    free(s);
    s = NULL;
  }
  
}

int
main() {
  check_parse();
  check_add();
  check_hms2s();
  check_leap();
  check_dbm();
  check_ymd2doy();
  //check_sec();

  check_strptime();
  return 0;
}

#endif /* __UNIT_TESTING__ */
