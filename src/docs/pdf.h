/*
Copyright (c) 2010, Brian Savage
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

#ifndef _PDF_H_
#define _PDF_H_

#include "string_utils.h"

typedef struct _pdf_object_t     pdf_object_t;
typedef struct _pdf_dict_entry_t pdf_dict_entry_t;
typedef struct _pdf_dict_t       pdf_dict_t;
typedef struct _pdf_pages_t      pdf_pages_t;
typedef struct _pdf_page_t       pdf_page_t;
typedef struct _pdf_bool_t       pdf_bool_t;
typedef struct _pdf_number_t     pdf_number_t;
typedef struct _pdf_string_t     pdf_string_t;
typedef struct _pdf_string_t     pdf_name_t;
typedef struct _pdf_array_t      pdf_array_t;
typedef struct _pdf_stream_t     pdf_stream_t;
typedef struct _pdf_image_t      pdf_image_t;
typedef struct _pdf_ref_t        pdf_ref_t;
typedef struct _pdf_t            pdf_t;

typedef struct _pdf_store_t      pdf_store_t;

typedef struct _pdf_color_t      pdf_color_t;

typedef void (*repr_t) (pdf_object_t *obj);
typedef void (*free_t) (pdf_object_t *obj);

struct _pdf_object_t {
  string *str;
  int     offset;
  int     id;  
  int     type;
  int     status;
  repr_t  repr;
  free_t  free;
  pdf_t  *pdf;   /* PDF Document Object, no need to set this */
};

struct _pdf_store_t {
  int n;
  int nalloc;
  pdf_object_t **objs;
};

struct _pdf_bool_t {
  pdf_object_t base;
  int value;
};

struct _pdf_number_t {
  pdf_object_t base;
  float f;
  int   i;
  int type;
};

struct _pdf_string_t {
  pdf_object_t base;
  string *str;
};

struct _pdf_dict_entry_t {
  pdf_object_t base;
  char *key;
  pdf_object_t *obj;
};

struct _pdf_dict_t {
  pdf_object_t base;
  pdf_store_t *data;
};

struct _pdf_array_t {
  pdf_object_t base;
  pdf_store_t *data;
};

struct _pdf_stream_t {
  pdf_object_t base;
  pdf_object_t *dict;
  string *str;
  int encoding;
};

struct _pdf_image_t {
  pdf_object_t base;
  pdf_object_t *dict;
  char *data;
  int len;
  int width;
  int height;
  int type;
  int encoding;
};

struct _pdf_ref_t {
  pdf_object_t base;
  int id;
  int gen;
};

struct _pdf_pages_t {
  pdf_object_t  base;
  pdf_store_t  *pages;
};

struct _pdf_t {
  FILE         *fp;      /* File pointer for output */
  pdf_store_t  *objs;    /* Object Storage */
  pdf_object_t *trailer; /* Trailer */
  int           offset;  /* Current File offset [bytes] */
  int           xoffset; /* xref file offset [bytes] */
  /* Below are references contained in objs storage */
  pdf_object_t *stream;  /* Current stream */
  pdf_object_t *page;    /* Current page */
  pdf_object_t *pages;   /* Pages Dictinary */
  pdf_object_t *catalog; /* Catalog Dictionary */
};

struct _pdf_color_t {
  float r,g,b;
};

pdf_t        * pdf_new(void);
void           pdf_object_free(pdf_object_t *obj);
int            pdf_object_add(pdf_t *pdf, pdf_object_t *obj);
pdf_object_t * pdf_object_font(char *font, int type);
void           pdf_object_store(pdf_t *pdf, pdf_object_t *obj);
void           pdf_output(pdf_t *pdf);
pdf_object_t * pdf_page_new(pdf_t *p);
void           pdf_free(pdf_t *pdf);


pdf_store_t  * pdf_store_new();
string       * pdf_store_string(pdf_store_t *s, char *sep);
void           pdf_store_free(pdf_store_t *s);
int            pdf_store_length(pdf_store_t *s);
int            pdf_store_add(pdf_store_t *s, pdf_object_t *new);


pdf_object_t * pdf_array_new() ;
int            pdf_array_add(pdf_object_t *obj, pdf_object_t *new);
void           pdf_array_free(pdf_object_t *obj);

pdf_object_t * pdf_dict_new();
pdf_object_t * pdf_dict_entry_new(char *key, pdf_object_t *value);
int            pdf_dict_add_entry(pdf_object_t *obj, pdf_object_t *e);
pdf_object_t * pdf_dict_find_entry(pdf_object_t *obj, char *key);
pdf_object_t * pdf_dict_exists_entry(pdf_object_t *obj, char *key);
void           pdf_dict_entry_free(pdf_object_t *obj);
void           pdf_dict_free(pdf_object_t *obj);

pdf_object_t * pdf_name_new(char *name);
pdf_object_t * pdf_number_new(int type, int i, float f);
void           pdf_string_free(pdf_object_t *obj);
void           pdf_number_free(pdf_object_t *obj);

pdf_object_t * pdf_ref_new();
void           pdf_ref_set(pdf_object_t *obj, int id, int gen);
void           pdf_ref_free(pdf_object_t *obj);

pdf_object_t * pdf_store_n(pdf_store_t *s, int i);
char         * pdf_store_string_n(pdf_store_t *s, int i);

pdf_object_t * pdf_trailer_new();

void           pdf_string(pdf_object_t *obj);

void           pdf_number_set(pdf_object_t *obj, int type, int i, float f);
float          pdf_number_get_float(pdf_object_t *obj);
int            pdf_number_get_int(pdf_object_t *obj);

void           pdf_color_fill(pdf_t *pdf, pdf_color_t c);
void           pdf_color_stroke(pdf_t *pdf, pdf_color_t c);

pdf_object_t * pdf_stream_new();
void           pdf_stream_add(pdf_object_t *obj, char *fmt, ...);
void           pdf_stream_backup(pdf_t *pdf);
void           pdf_stream_set_encoding(pdf_object_t *obj, int encoding);

void           pdf_text_font(pdf_t *pdf, char *font, float size);
void           pdf_text_position(pdf_t *obj, float x, float y);
void           pdf_text_show(pdf_t *pdf, char *text);
void           pdf_text_begin(pdf_t *pdf);
void           pdf_text_end(pdf_t *pdf);

void           pdf_text_matrix(pdf_t *pdf, float x, float y, float angle);
void           pdf_translate_scale(pdf_t *pdf, float x, float y, float xs, float ys);

pdf_object_t * pdf_image_new();
void           pdf_image_set_encoding(pdf_object_t *obj, int encoding);
void           pdf_image_show(pdf_t *pdf, char *name);
void           pdf_image_color_add_resources(pdf_t *pdf);
void           pdf_image_add_xobject(pdf_t *pdf, pdf_object_t *image, char *name);
void           pdf_image_set(pdf_object_t *obj, char *data, int w, int h, int type);

void           pdf_rectangle(pdf_t *pdf, float x, float y, pdf_color_t c);

/* Stream Encoding */
char * ASCIIHexEncode(unsigned char *p, int *np);
char * RunLengthEncode(unsigned char *p, int *np);
char * FlateEncode(unsigned char *p, int *np);


#define NAME(k,v)  pdf_dict_entry_new(k, pdf_name_new(v))
#define REF(k)     pdf_dict_entry_new(k, pdf_ref_new())
#define DICT(k)    pdf_dict_entry_new(k, pdf_dict_new())
#define ARRAY(k)   pdf_dict_entry_new(k, pdf_array_new())
#define INT(k,v)   pdf_dict_entry_new(k, pdf_number_new(PDF_NUMBER_INT, v, 0.0))
#define FLOAT(k,v) pdf_dict_entry_new(k, pdf_number_new(PDF_NUMBER_FLOAT, 0, v))

enum {
  PDF_DICT = 1,
  PDF_DICT_ENTRY,
  PDF_BOOL,
  PDF_STREAM,
  PDF_ARRAY,
  PDF_STRING,
  PDF_NUMBER,
  PDF_REF,
  PDF_NAME,
  PDF_IMAGE,
};

enum {
  PDF_IMAGE_GRAY,
  PDF_IMAGE_RGB,
  PDF_IMAGE_CMYK,
};

enum {
  PDF_ENCODE_NONE,
  PDF_ENCODE_RLE,
  PDF_ENCODE_HEX,
  PDF_ENCODE_FLATE,
};

enum {
  PDF_NUMBER_INT,
  PDF_NUMBER_FLOAT,
};

enum {
  PDF_OBJECT_NEW = 1,
  PDF_OBJECT_FREE
};

enum {
  PDF_FONT_TYPE_0,
  PDF_FONT_TYPE_1,
  PDF_FONT_TYPE_TRUE,
  PDF_FONT_TYPE_0_CID,
  PDF_FONT_TYPE_2_CID,
  PDF_FONT_TYPE_3,
};

enum {
    PDF_LINE_JOIN_STYLE_MITER = 0,
    PDF_LINE_JOIN_STYLE_ROUND = 1,
    PDF_LINE_JOIN_STYLE_BEVEL = 2,
} PDF_LINE_JOIN_STYLE;

void pdf_header(pdf_t *pdf);
void pdf_footer(pdf_t *pdf);
void pdf_stroke(pdf_t *pdf);
void pdf_fill(pdf_t *pdf);
void pdf_save(pdf_t *pdf);
void pdf_restore(pdf_t *pdf);
void pdf_color(pdf_color_t c);
void pdf_lineto(pdf_t *pdf, float x, float y);
void pdf_moveto(pdf_t *pdf, float x, float y);
void pdf_linewidth(pdf_t *pdf, float size);
void pdf_rotate(pdf_t *pdf, float angle);
void pdf_textsize(float size);
void pdf_unscale();
void pdf_textshow(char *text);
void pdf_textjustify(char *text, int h, int v);
void pdf_line_join_style(pdf_t *pdf, int style);


void pdf_write(pdf_t *pdf, char *fmt, ...);

#endif /* _PDF_H_ */
