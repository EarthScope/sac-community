
/** 
 * @file   rc_options.c
 *
 * @brief  Options Control File
 * 
 */

#include <stdio.h>

#include "complex.h"
#include "proto.h"
#include "hdr.h"

/* List Header */
#include "lhf.h"


/* Resource Control / Options */
#include "rc.h"


#define bool(i) ( (i == TRUE) ? "true" : "false" )
#define IOUT(s,i) {if(rc_verbose()) {fprintf(stderr, "%20s = %d\n", s, i);}}
#define BOUT(s,i) {if(rc_verbose()) {fprintf(stderr, "%20s = %-6s\n", s, bool(i));}}
#define ROUT(s,i) {if(rc_verbose()) {fprintf(stderr, "%20s = %f\n", s, i);}}
#define SOUT(s,i) {if(rc_verbose()) {fprintf(stderr, "%20s = %s\n", s, i);}}


/*Llist Header */
void set_lh_columns        (void *v) { int *i  = (int *)v;   IOUT("lh.columns", *i);       lh_columns_set(*i);    }
void set_lh_inclusive      (void *v) { int *i  = (int *)v;   BOUT("lh.inclusive", *i);     lh_inclusive_set(*i);  }

/* Color */
void set_color_color       (void *v) { int *i  = (int *)v;   BOUT("color.color", *i);      color_switch(*i);                   }
void set_color_foreground  (void *v) { char *c = (char *)v;  SOUT("color.foreground", c);  color_foreground_set_by_name(c); }
void set_color_background  (void *v) { char *c = (char *)v;  SOUT("color.background", c);  color_background_set_by_name(c); }
void set_color_skeleton    (void *v) { char *c = (char *)v;  SOUT("color.skeleton", c);    color_skeleton_set_by_name(c);   }
void set_color_data        (void *v) { char *c = (char *)v;  SOUT("color.data", c);        color_data_set_by_name(c);       }
void set_color_increment   (void *v) { int *i  = (int *)v;   BOUT("color.increment", *i);  color_increment_set(*i);         }

/* History */
void set_history_length    (void *v) { int *i  = (int *)v;  IOUT("history.max", *i);       history_size_set(*i);}

/* Copyright */
void set_display_copyright (void *v) { int *i  = (int *)v;  BOUT("copyright", *i);         display_copyright(*i);}

/* Database */
void set_use_database      (void *v) { int *i  = (int *)v;  BOUT("database", *i);          use_database(*i);}

/* PPK */
void set_ppk_crosshair_size(void *v) { int *i  = (int *)v;  BOUT("ppk.large_crosshairs", *i);  use_large_crosshairs(*i);}

/* Echo */
void set_echo              (void *v) { int *i  = (int *)v;  BOUT("echo", *i);              echo_switch(*i);}

/* QDP */
void set_qdp               (void *v) { int *i  = (int *)v;  BOUT("qdp", *i);               qdp_switch(*i);}
void set_qdp_points        (void *v) { int *i  = (int *)v;  IOUT("qdp.points", *i);        qdp_points(*i);}

/* X Divisions */
void set_xdiv_power        (void *v) { int *i  = (int *)v;  BOUT("xdiv.power", *i);        xdiv_power(*i);}
void set_xdiv_number       (void *v) { int *i  = (int *)v;  IOUT("xdiv.number", *i);       xdiv_number(*i);}
void set_xdiv_nice         (void *v) { int *i  = (int *)v;  BOUT("xdiv.nice", *i);         xdiv_nice(*i);}
void set_xdiv_increment    (void *v) { float *i  = (float *)v;  ROUT("xdiv.increment", *i);    xdiv_increment(*i);}

/* Y Divisions */
void set_ydiv_power        (void *v) { int *i  = (int *)v;  BOUT("ydiv.power", *i);        ydiv_power(*i);}
void set_ydiv_number       (void *v) { int *i  = (int *)v;  IOUT("ydiv.number", *i);       ydiv_number(*i);}
void set_ydiv_nice         (void *v) { int *i  = (int *)v;  BOUT("ydiv.nice", *i);         ydiv_nice(*i);}
void set_ydiv_increment    (void *v) { float *i  = (float *)v;  ROUT("ydiv.increment", *i);    ydiv_increment(*i);}

/* X Label */
void set_xlabel            (void *v) { int *i  = (int *)v;  BOUT("xlabel", *i);            xlabel_switch(*i);}
void set_xlabel_label      (void *v) { char *c = (char *)v; SOUT("xlabel.label", c);       xlabel_label(c);}
void set_xlabel_location   (void *v) { char *c = (char *)v; SOUT("xlabel.location", c);    xlabel_location(c);}
void set_xlabel_size       (void *v) { char *c = (char *)v; SOUT("xlabel.size", c);        xlabel_size(c);}

/* Y Label */
void set_ylabel            (void *v) { int *i  = (int *)v;  BOUT("ylabel", *i);            ylabel_switch(*i);}
void set_ylabel_label      (void *v) { char *c = (char *)v; SOUT("ylabel.label", c);       ylabel_label(c);}
void set_ylabel_location   (void *v) { char *c = (char *)v; SOUT("ylabel.location", c);    ylabel_location(c);}
void set_ylabel_size       (void *v) { char *c = (char *)v; SOUT("ylabel.size", c);        ylabel_size(c);}

struct rc_options Options[] = {

  {"listhdr.columns",      RC_INTEGER, set_lh_columns},
  {"lh.columns",           RC_INTEGER, set_lh_columns},
  {"listhdr.inclusive",    RC_BOOL,    set_lh_inclusive},
  {"lh.inclusive",         RC_BOOL,    set_lh_inclusive},

  {"color.color",          RC_BOOL,    set_color_color},
  {"color.foreground",     RC_STRING,  set_color_foreground},
  {"color.background",     RC_STRING,  set_color_background},
  {"color.increment",      RC_BOOL,    set_color_increment},
  {"color.skeleton",       RC_STRING,  set_color_skeleton},
  {"color.data",           RC_STRING,  set_color_data},

  {"copyright",            RC_BOOL,    set_display_copyright},

  {"database",             RC_BOOL,    set_use_database},

  {"ppk.large_crosshairs", RC_BOOL,    set_ppk_crosshair_size},

  {"echo",                 RC_BOOL,    set_echo},

  {"qdp",                  RC_BOOL,    set_qdp},
  {"quick_and_dirty_plot", RC_BOOL,    set_qdp},
  {"qdp.points",           RC_INTEGER, set_qdp_points},

  {"xdiv.power",           RC_BOOL,    set_xdiv_power},
  {"xdiv.number",          RC_INTEGER, set_xdiv_number},
  {"xdiv.nice",            RC_BOOL,    set_xdiv_nice},
  {"xdiv.increment",       RC_REAL,    set_xdiv_increment},

  {"ydiv.power",           RC_BOOL,    set_ydiv_power},
  {"ydiv.number",          RC_INTEGER, set_ydiv_number},
  {"ydiv.nice",            RC_BOOL,    set_ydiv_nice},
  {"ydiv.increment",       RC_REAL,    set_ydiv_increment},

  {"xlabel.label",         RC_STRING,  set_xlabel_label},
  {"xlabel",               RC_BOOL,    set_xlabel},
  {"xlabel.location",      RC_STRING,  set_xlabel_location},
  {"xlabel.size",          RC_STRING,  set_xlabel_size},

  {"ylabel.label",         RC_STRING,  set_ylabel_label},
  {"ylabel",               RC_BOOL,    set_ylabel},
  {"ylabel.location",      RC_STRING,  set_ylabel_location},
  {"ylabel.size",          RC_STRING,  set_ylabel_size},
  
  {"history.max",          RC_INTEGER, set_history_length},
  {NULL, RC_NULL, NULL},
};
/*   {"window.width",         RC_INTEGER, set_window_width}, */
/*   {"window.height",        RC_INTEGER, set_window_height}, */
/*
void set_window_width(void *v) {  
  int *i = (int *)v; 
  IOUT("window.width", *i);
}
void set_window_height(void *v) {  
  int *i = (int *)v; 
  IOUT("window.height", *i);
}
*/
