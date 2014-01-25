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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

#include "pdf.h"

#include "string_utils.h"

#include "config.h"

#ifdef HAVE_ZLIB
#include <zlib.h>
#endif /* HAVE_ZLIB */

#define PI M_PI

#define PDF_PAGE_LETTER_WIDTH   792 
#define PDF_PAGE_LETTER_HEIGHT  612 

void
pdf_error(char *fmt, ...) {
  va_list args;

  fprintf(stderr, "PDF Error: ");
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);  
  va_end(args);
}

int
pdf_dict_add_dict(pdf_object_t *dict, char *name) {
    pdf_object_t *d, *e;
    if(!dict || !name) 
        return FALSE;
    d = pdf_dict_new();
    if(!d) 
        return FALSE;
    e = pdf_dict_entry_new(name, d);
    if(!e) {
        pdf_dict_free(d);
        return FALSE;
    }
    if(!pdf_dict_add_entry(dict, e)) {
        pdf_dict_entry_free(e);
        return FALSE;
    }
    return TRUE;
}

int
pdf_dict_add_int(pdf_object_t *dict, char *name, int value) {
    pdf_object_t *i, *e;
    if(!dict || !name) 
        return FALSE;
    i = pdf_number_new(PDF_NUMBER_INT, value, 0.0);
    if(!i) 
        return FALSE;
    e = pdf_dict_entry_new(name, i);
    if(!e) {
        pdf_number_free(i);
        return FALSE;
    }
    if(!pdf_dict_add_entry(dict, e)) {
        pdf_dict_entry_free(e);
        return FALSE;
    }
    return TRUE;
}

int
pdf_dict_add_array(pdf_object_t *dict, char *name) {
    pdf_object_t *a, *e;
    if(!dict || !name) 
        return FALSE;
    a = pdf_array_new();
    if(!a) 
        return FALSE;
    e = pdf_dict_entry_new(name, a);
    if(!e) {
        pdf_array_free(a);
        return FALSE;
    }
    if(!pdf_dict_add_entry(dict, e)) {
        pdf_dict_entry_free(e);
        return FALSE;
    }
    return TRUE;
}

int
pdf_dict_add_name(pdf_object_t *dict, char *name, char *value) {
    pdf_object_t *n, *e;
    if(!name || !value || !dict)
        return FALSE;
    n = pdf_name_new(value);
    if(!n) 
        return FALSE;
    e = pdf_dict_entry_new(name, n);
    if(!e) {
        pdf_string_free(n);
        return FALSE;
    }
    if(!pdf_dict_add_entry(dict, e)) {
        pdf_dict_entry_free(e);
        return FALSE;
    }
    return TRUE;
    
}

int
pdf_dict_add_ref(pdf_object_t *dict, char *name) {
    pdf_object_t *r, *e;
    if(!name || !dict)
        return FALSE;
    r = pdf_ref_new();
    if(!r)
        return FALSE;
    e = pdf_dict_entry_new(name, r);
    if(!e) {
        pdf_ref_free(r);
        return FALSE;
    }
    if(!pdf_dict_add_entry(dict, e)) {
        pdf_dict_entry_free(e);
        return FALSE;
    }
    return TRUE;
}

/** 
 * Create and return a new PDF Document Object
 *
 * @return 
 *    PDF Document Object
 */
pdf_t *
pdf_new(void) {
  pdf_t *p;
  pdf_object_t *x, *root;
  int n;

  p = (pdf_t *) malloc(sizeof(pdf_t));
  if(!p) {
    pdf_error("Cannot allocate space for PDF\n");
    return NULL;
  }
  /* Create Storage for new objects */
  p->pages   = NULL;
  p->objs    = pdf_store_new();
  p->offset  = 0;
  p->xoffset = 0;
  p->fp      = NULL;

  /* Catalog Dictonary */
  x = pdf_dict_new();
  n = pdf_object_add(p, x);
  pdf_dict_add_name(x, "Type", "Catalog");
  pdf_dict_add_ref(x, "Pages");
  p->catalog = x;

  /* Trailer */
  p->trailer = pdf_trailer_new();
  root = pdf_dict_find_entry(p->trailer, "Root");
  pdf_ref_set(root, n, 0);

  /* Pages Dictionary */
  x  = pdf_dict_new();
  n = pdf_object_add(p, x);
  pdf_dict_add_name(x, "Type", "Pages");
  pdf_dict_add_int(x, "Count", 0);
  pdf_dict_add_array(x, "Kids");
  p->pages = x;

  /* Add pages reference to Catalog */
  root = pdf_dict_find_entry(p->catalog, "Pages");
  pdf_ref_set(root, n, 0);

  return p;
}

/** 
 * Free a PDF Document Object and its internal objects
 *
 * @param pdf
 *    PDF Document Object to free
 */
void
pdf_free(pdf_t *pdf) {
  pdf_store_free(pdf->objs);
  pdf_object_free( pdf->trailer );
  if(pdf) {
    free(pdf);
    pdf = NULL;
  }
}

/**  
 * Free a PDF Object 
 * 
 * @param obj
 *    PDF Object to free
 *
 */
void
pdf_object_free(pdf_object_t *obj) {
  if(!obj) {
    pdf_error("Attempt to free unallocated pdf object\n");
    return;
  }
  if(!obj->free) {
    pdf_error("Attempt to free with unset free operator\n");
    return;
  }
  string_free(&obj->str);
  obj->free( obj );
}

/** 
 * Write partial contents of a PDF Document to a file
 *
 * @param pdf
 *    PDF Document Object
 * @param fmt
 *    Format of the string to write to a file
 * @param ...
 *    Values for the format \p fmt
 *
 */
void
pdf_write(pdf_t *pdf, char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  pdf->offset += vfprintf(pdf->fp, fmt, ap);
  va_end(ap);
}

/** 
 * Write the PDF Header
 *
 * @param pdf
 *    PDF Document Object
 *
 */
void
pdf_header(pdf_t *pdf) {
  pdf_write(pdf, "%%PDF-1.4\n");
}

/** 
 * Write all PDF Objects in a PDF Document 
 * 
 * @param pdf
 *    PDF Document Object to write
 *
 */
void
pdf_objects_write(pdf_t *pdf) {
  int i;
  pdf_object_t *obj;

  for(i = 0; i < pdf_store_length(pdf->objs); i++) {
    obj         = pdf_store_n(pdf->objs, i);
    obj->offset = pdf->offset;
    obj->pdf    = pdf; /* Reference for Streams, who do their own writing */
    pdf_write(pdf, "%d %d obj\n", i+1, 0);
    pdf_write(pdf, "%s", pdf_store_string_n(pdf->objs, i));
    pdf_write(pdf, "endobj\n");
  }
}

/** 
 * Status of a PDF Object
 *
 * @param obj
 *    PDF Object 
 *
 * @return 
 *   - "n" if the object is "in-use"
 *   - "f" if the object is a "free-entry"
 *  
 */
char * 
pdf_object_status(pdf_object_t *obj) {
  if(obj->status == PDF_OBJECT_NEW) {
    return "n";
  }
  return "f";
}

/** 
 * Write the PDF Cross Refernce Table 
 *
 * @param pdf
 *    PDF Document Object
 * 
 * Cross Reference includes 
 *   - Starting Object Number
 *   - Number of PDF Objects
 *   - Location (byte-offset) of PDF Objects in the Document
 *   
 * @see "Cross-Reference Table" Section 3.4.3 (PDF-Reference 1.7)
 *
 */
void
pdf_xref(pdf_t *pdf) {
  pdf_object_t *o;
  int i;
  pdf->xoffset = pdf->offset;
  pdf_write(pdf, "xref\n");
  pdf_write(pdf, "%d %d\n", 0, pdf_store_length(pdf->objs)+1);
  pdf_write(pdf, "%0.10d %0.5d %s\r\n", 0, 65535, "f");
  for(i = 0; i < pdf_store_length(pdf->objs); i++) {
    o = pdf_store_n(pdf->objs, i);
    pdf_write(pdf, "%0.10d %0.5d %s\r\n", o->offset, 0, pdf_object_status(o));
  }
}

/** 
 * Write the PDF Footer or trailer
 *
 * @param pdf
 *    PDF Document Object
 * 
 * Footer includes 
 *   - Dictionary with Size of Cross Reference Table
 *   - Reference to the Document Catalog or "Root"
 *   - Cross Reference Table
 *
 */
void
pdf_footer(pdf_t *pdf) {

  pdf_object_t *size;
  pdf_xref(pdf);
  if(!pdf->trailer) {
      return;
  }
  pdf_write(pdf, "trailer\n");
  size = pdf_dict_find_entry(pdf->trailer, "Size");
  pdf_number_set(size,
                 PDF_NUMBER_INT, 
                 pdf_store_length(pdf->objs)+1,
                 0);
  if(pdf->trailer) {
      pdf_string(pdf->trailer);
      if(pdf->trailer->str) {
          pdf_write(pdf, pdf->trailer->str->str);
      }
      pdf_write(pdf, "startxref\n");
      pdf_write(pdf, "%d\n", pdf->xoffset);
      pdf_write(pdf, "%%%%EOF");
  }
}

/** 
 * Stroke a line, "S"
 *
 * @param pdf
 *    PDF Document Object
 */
void
pdf_stroke(pdf_t *pdf) {
  pdf_stream_add(pdf->stream, "S\n");
}

/** 
 * Fill a path, "f"
 *
 * @param pdf
 *    PDF Document Object
 */
void
pdf_fill(pdf_t *pdf) {
    pdf_stream_add(pdf->stream, "f\n");
}

/** 
 * Save a graphics state, "q"
 *
 * @param pdf
 *    PDF Document Object
 */
void
pdf_save(pdf_t *pdf) {
  pdf_stream_add(pdf->stream, "q\n");
}

/** 
 * Restore a graphics state, "Q"
 *
 * @param pdf
 *    PDF Document Object
 */
void
pdf_restore(pdf_t *pdf) {
  pdf_stream_add(pdf->stream, "Q\n");
}

/** 
 * Set drawing color, "RG"
 *
 * @param pdf
 *    PDF Document Object
 * @param c
 *    Color to set 
 *
 */
void
pdf_color_stroke(pdf_t *pdf, pdf_color_t c) {
  pdf_stream_add(pdf->stream, "%f %f %f RG\n", c.r, c.g, c.b);
  pdf_stream_add(pdf->stream, "%f %f %f rg\n", c.r, c.g, c.b);
}

/** 
 * Set fill color, "rg"
 *
 * @param pdf
 *    PDF Document Object
 * @param c
 *    Color to set
 *
 */
void
pdf_color_fill(pdf_t *pdf, pdf_color_t c) {
  pdf_stream_add(pdf->stream, "%f %f %f rg\n", c.r, c.g, c.b);  
}

/** 
 * Draw a absolute line segemnt, "l"
 *
 * @param pdf
 *    PDF Document Object
 * @param x
 *    x position
 * @param y
 *    y position
 */
void
pdf_lineto(pdf_t *pdf, float x, float y) {
  pdf_stream_add(pdf->stream, "%f %f l\n", x, y);
}

/** 
 * Set Line Join Style
 *
 * @param pdf
 *    PDF Document Object
 * @param style
 *    line join style
 *
 */
void
pdf_line_join_style(pdf_t *pdf, int style) {
    pdf_stream_add(pdf->stream, "%d j\n", style);
}

/** 
 * Move to a position, "m"
 *
 * @param pdf
 *    PDF Document Object
 * @param x
 *    x position
 * @param y
 *    y position
 */
void
pdf_moveto(pdf_t *pdf, float x, float y) {
  pdf_stream_add(pdf->stream, "%f %f m\n", x, y);
}

/** 
 * Set a line width, "w"
 *
 * @param pdf
 *    PDF Document Object
 * @param size
 *    Line width
 */
void
pdf_linewidth(pdf_t *pdf, float size) {
  pdf_stream_add(pdf->stream, "%f w\n", size);
}

/** 
 * Set text position and angle, "Tm"
 *
 * @param pdf
 *    PDF Document Object
 * @param x
 *    x position of text
 * @param y
 *    y position of text
 * @param angle
 *    Angle of text
 */
void
pdf_text_matrix(pdf_t *pdf, float x, float y, float angle) { 
 float a = angle * PI/180.0;
  pdf_stream_add(pdf->stream, "%f %f %f %f %f %f Tm\n", 
                 cos(a), sin(a), -sin(a), cos(a), x, y);
}

/** 
 * Translate and scale a graphics state, "cm"
 *
 * @param pdf
 *    PDF Document Object
 * @param x
 *    x position to translate to
 * @param y
 *    y position to translate to 
 * @param xs
 *    x scale value
 * @param ys 
 *    y scale value
 */
void
pdf_translate_scale(pdf_t *pdf, float x, float y, float xs, float ys) {
  pdf_stream_add(pdf->stream, "%f 0.0 0.0 %f %f %f cm\n", xs, ys, x, y);
}

/** 
 * Rotate a graphics state, "cm"
 *
 * @param pdf
 *    PDF Document Object
 * @param angle
 *    Rotation Angle
 */
void
pdf_rotate(pdf_t *pdf, float angle) {
  float x;
  x = angle * PI/180.0;
  pdf_stream_add(pdf->stream,
                 "[ %f %f %f %f 0 0 ] cm\n", 
                 cos(x), sin(x), 
                 -sin(x), cos(x));
}

/** 
 * Set Text Font, "Tf"
 *
 * @param pdf
 *    PDF Document Object
 * @param font
 *    Font name to use
 * @param size
 *    Font size
 */
void
pdf_text_font(pdf_t *pdf, char *font, float size) {
  pdf_stream_add(pdf->stream, "%s %f Tf\n", font, size);
}

/** 
 * Set text position, "Td"
 *
 * @param pdf
 *    PDF Document Object
 * @param x
 *    x position of text
 * @param y
 *    y position of text
 */
void
pdf_text_position(pdf_t *pdf, float x, float y) {
  pdf_stream_add(pdf->stream, "%f %f Td\n", x, y);
}

/** 
 * Show a text string, "Tj"
 *
 * @param pdf
 *    PDF Document Object
 * @param text
 *    Text string to show
 */
void
pdf_text_show(pdf_t *pdf, char *text) {
  pdf_stream_add(pdf->stream, "(%s) Tj\n", text);
}

/** 
 * Begin Text Object, "BT"
 *
 * @param pdf
 *    PDF Document Object
 */
void
pdf_text_begin(pdf_t *pdf) {
  pdf_stream_add(pdf->stream, "BT\n");
}

/** 
 * End Text Object, "ET"
 *
 * @param pdf
 *    PDF Document Object
 */
void
pdf_text_end(pdf_t *pdf) {
  pdf_stream_add(pdf->stream, "ET\n");
}

/** 
 * Initialize a PDF Object 
 *
 * @param pdf
 *    PDF Document Object
 * @param type
 *    PDF Object Type
 * @param r
 *    function to generate text string 
 * @param f
 *    fucntion to free object data
 *
 */
void
pdf_object_init(pdf_object_t *obj, int type, repr_t r, free_t f) {
  obj->str    = NULL;
  obj->offset = 0;
  obj->id     = 0;
  obj->status = PDF_OBJECT_NEW;
  obj->type   = type;
  obj->repr   = r;
  obj->free   = f;
}

/** 
 * Write an entire PDF Document Object to a file
 * 
 * @param pdf
 *    PDF Document Object
 *
 */
void
pdf_output(pdf_t *pdf) {
  pdf_header(pdf);
  pdf_objects_write(pdf);
  pdf_footer(pdf);
  fclose(pdf->fp);
}

/** 
 * Create PDF Font Object 
 *
 * @param font
 *    Name of font 
 * @param type 
 *    Type of Font
 *
 * @return
 *    PDF Font Object 
 */
pdf_object_t *
pdf_font_new(char *font, char *type) {
  pdf_object_t *f;
  f = pdf_dict_new();
  pdf_dict_add_name(f, "Type",     "Font");
  pdf_dict_add_name(f, "BaseFont",  font);
  pdf_dict_add_name(f, "Subtype",   type);
  pdf_dict_add_name(f, "Encoding", "StandardEncoding");
  return f;
}

/**  
 * Get type of PDF Object 
 *
 * @param obj
 *    PDF Object
 * 
 * @return String of PDF Object Type
 *
 */
char * 
pdf_object_type(pdf_object_t *obj) {
 switch(obj->type) {
 case PDF_DICT:       return "PDF Dictionary"; 
 case PDF_DICT_ENTRY: return "PDF Dictionary Entry"; 
 case PDF_BOOL:       return "PDF Boolean"; 
 case PDF_STREAM:     return "PDF Stream"; 
 case PDF_ARRAY:      return "PDF Array"; 
 case PDF_STRING:     return "PDF String"; 
 case PDF_NUMBER:     return "PDF Number"; 
 case PDF_REF:        return "PDF Reference"; 
 case PDF_NAME:       return "PDF Name"; 
 }
 return "PDF Undefined Type";
}

/** 
 * Generate and Save a string representation of a PDF Object
 * 
 * @param obj
 *    PDF Object 
 *
 * Call the Objects obj->repr() function, saves the string in obj->str
 *
 */
void
pdf_string(pdf_object_t *obj) {
  if(!obj) {
    pdf_error("Object not defined\n");
    return;
  }
  if(obj->repr) {
    obj->repr(obj);
  } else {
    pdf_error("String represntation not defined: %p\n", 
              "\tType: %s\n", obj, pdf_object_type(obj));
  }
}

/** 
 * Generate the string representation of a Dict Entry
 *
 * @param obj
 *    PDF Object 
 *
 * String: "/key value"
 *
 */
void
pdf_dict_entry_string(pdf_object_t *obj)  {
  pdf_dict_entry_t *e = (pdf_dict_entry_t *) obj;
  if(!e) 
      return;
  pdf_string(e->obj);
  if(e->obj && e->obj->str) 
      obj->str = string_printf(obj->str,"/%s %s\n", e->key, e->obj->str->str);
}

/** 
 * Free a Dictionary Entry
 *
 * @param 
 *    PDF Object 
 */
void
pdf_dict_entry_free(pdf_object_t *obj) {
  pdf_dict_entry_t *e = (pdf_dict_entry_t *) obj;
  pdf_object_free(e->obj);
  free(e->key);
  free(e);
  e = NULL;
}

/** 
 * Create a new Dictonary Entry PDF Object 
 * 
 * @param key
 *    Key value 
 * @param value
 *    Value of the entry
 *
 * @return
 *    PDF Object
 */
pdf_object_t *
pdf_dict_entry_new(char *key, pdf_object_t *value) {
  pdf_dict_entry_t *e;
  e = (pdf_dict_entry_t *) malloc(sizeof(pdf_dict_entry_t));
  if(!e) {
    pdf_error("Cannot allocate a PDF Dictionary entry\n");
    return NULL;
  }
  e->key = strdup(key);
  e->obj = value;
  pdf_object_init((pdf_object_t *)e, PDF_DICT_ENTRY, pdf_dict_entry_string, pdf_dict_entry_free);
  return (pdf_object_t *) e;
}

/** 
 * Generate the string representation of a Dictionary
 *
 * @param obj
 *    PDF Object 
 *
 * String: "<<
 *           entries
 *         >>"
 *
 */
void
pdf_dict_string(pdf_object_t *obj) {

  string *s;
  pdf_dict_t *dict = (pdf_dict_t *) obj;
  obj->str = string_new("<<\n");
  s = pdf_store_string(dict->data, "");
  if(s) {
    obj->str = string_append(obj->str, s->str);
    string_free(&s);
  }
  obj->str = string_append(obj->str,">>\n");
}

/** 
 * Free a Dictionary
 *
 * @param 
 *    PDF Object 
 */
void
pdf_dict_free(pdf_object_t *obj) {
  pdf_dict_t *dict = (pdf_dict_t *) obj;
  pdf_store_free(dict->data);
  free(dict);
  dict = NULL;
}

/** 
 * Create a new Dictonary PDF Object
 * 
 * @return
 *    PDF Object
 */
pdf_object_t *
pdf_dict_new() {
  pdf_dict_t *dict;
  dict = (pdf_dict_t *) malloc(sizeof(pdf_dict_t));
  if(!dict) {
    pdf_error("Cannot allocate a PDF dictionary\n");
    return NULL;
  }
  dict->data = pdf_store_new();
  if(!dict->data) {
    free(dict);
    dict = NULL;
  }
  pdf_object_init((pdf_object_t *) dict, PDF_DICT, pdf_dict_string, pdf_dict_free);
  return (pdf_object_t *) dict;
}

/** 
 * Find an entry in a PDF Dictionary, this will not report an error
 *
 * @param obj
 *    PDF Dictionary Object
 * @param key
 *    Key value to seardh for
 *
 * @return
 *    Found PDF Object for associated key or 
 *    NULL if the key was not found
 */
pdf_object_t *
pdf_dict_exists_entry(pdf_object_t *obj, char *key) {
  int i;
  pdf_dict_entry_t *e;
  pdf_dict_t *dict = (pdf_dict_t *) obj;
  if(!dict || !dict->data) {
      return NULL;
  }
  for(i = 0; i < pdf_store_length(dict->data); i++) {
    e = (pdf_dict_entry_t *) pdf_store_n(dict->data, i);
    if(e && strcmp(key, e->key) == 0) {
      return e->obj;
    }
  }
  return NULL;
}

/** 
 * Find an entry in a PDF Dictionary, this will report an error
 *
 * @param obj
 *    PDF Dictionary Object
 * @param key
 *    Key value to seardh for
 *
 * @return
 *    Found PDF Object for associated key or 
 *    NULL if the key was not found
 */
pdf_object_t *
pdf_dict_find_entry(pdf_object_t *obj, char *key) {
  pdf_object_t *p;
  p = pdf_dict_exists_entry(obj, key);
  if(!p) {
    pdf_error("Cannot Find PDF dictionary entry: %s\n", key);
    return NULL;
  }
  return p;
}

/** 
 * Add an Entry to a PDF Dictionary
 * 
 * @param obj
 *    PDF Dictionary Object
 * @param e
 *    PDF Entry Object to add to PDF Dictionary
 *
 */
int
pdf_dict_add_entry(pdf_object_t *obj, pdf_object_t *e) {
  pdf_dict_t *dict = (pdf_dict_t *) obj;
  if(!dict || !e) {
      return FALSE;
  }
  return pdf_store_add(dict->data, e);
}

/** 
 * Free a Boolean
 *
 * @param 
 *    PDF Object 
 */
void
pdf_bool_free(pdf_object_t *obj) {
  pdf_bool_t *bool = (pdf_bool_t *) obj;
  free(bool);
  bool = NULL;
}

/** 
 * Generate the string representation of a Boolean
 *
 * @param obj
 *    PDF Object 
 *
 * String: true | false
 */
void
pdf_bool_string(pdf_object_t *obj) {
  pdf_bool_t *bool = (pdf_bool_t *) obj;
  if(bool->value) {
    obj->str = string_new("true");
  } else {
    obj->str = string_new("false");
  }
}

/** 
 * Create a new Boolean PDF Object 
 * 
 * @param value
 *    Value of the boolean
 *
 * @return
 *    PDF Object
 */
pdf_bool_t *
pdf_bool_new(int value) {
  pdf_bool_t *b;
  b = (pdf_bool_t *) malloc(sizeof(pdf_bool_t));
  b->value = value;
  pdf_object_init((pdf_object_t *) b, PDF_BOOL, pdf_bool_string, pdf_bool_free);
  return b;
}

static int stream_pos;

/** 
 * Reverse a stream one data commit 
 *
 * @param pdf
 *    PDF Doument Object
 */
void
pdf_stream_backup(pdf_t *pdf) {
  pdf_stream_t *s = (pdf_stream_t *) pdf->stream;
  s->str = string_remove(s->str, stream_pos, -1);
}

/** 
 * Add a data to a PDF Stream Object
 *
 * @param obj
 *    PDF Stream Object
 * @param fmt
 *    Format of data to add
 * @param ...
 *    Data to add
 */
void
pdf_stream_add(pdf_object_t *obj, char *fmt, ...) {
  va_list ap;
  pdf_stream_t *s = (pdf_stream_t *) obj;
  if(s->str) {
    stream_pos = string_length(s->str);
  } else {
    stream_pos = 0;
  }
  va_start(ap, fmt);
  s->str = string_printf_append_internal(s->str, fmt, ap);
  va_end(ap);
}

/** 
 * Encode or compress a stream 
 *
 * @param dict
 *    Dictionary of the PDF Stream Object
 * @param data
 *    Raw Stream Data 
 * @param n
 *    Length of raw stream data in \p data
 * @param encoding 
 *    Type of Encoding 
 */
void
pdf_stream_encode(pdf_object_t *obj, pdf_object_t *dict, char *data, int n, int encoding) {
  pdf_object_t *r;
  char *xdata;
  if(!dict){
      return;
  }
#ifndef HAVE_ZLIB
  if(encoding == PDF_ENCODE_FLATE) {
    fprintf(stderr, "sac: Writing to PDF files with Zlib/Flate Compreesion\n"
            "    not supported in the vertion\n"
            "    This functionality can be compiled into SAC\n"
            "    Using Ascii Hex Encoding\n" );
    encoding = PDF_ENCODE_HEX;
  }
#endif
  xdata = NULL;
  if(encoding == PDF_ENCODE_FLATE) {
    pdf_dict_add_name(dict, "Filter", "FlateDecode");
    xdata = FlateEncode((unsigned char *)data, &n);
  } else if(encoding == PDF_ENCODE_HEX) {
    pdf_dict_add_name(dict, "Filter", "ASCIIHexDecode");
    xdata = ASCIIHexEncode((unsigned char *)data, &n);
  } else if(encoding == PDF_ENCODE_RLE) {
    pdf_dict_add_name(dict, "Filter", "RunLengthDecode");
    xdata = RunLengthEncode((unsigned char *)data, &n);
  } 
  r = pdf_dict_find_entry(dict, "Length");
  pdf_number_set(r, PDF_NUMBER_INT, n, 0);
  pdf_string(dict);
  
  if(dict && dict->str) {
      pdf_write(obj->pdf, "%s", dict->str->str);
  }
  if(obj->pdf) {
      pdf_write(obj->pdf, "stream\n");
      fwrite(xdata, n, 1, obj->pdf->fp);
      obj->pdf->offset += n;
      pdf_write(obj->pdf, "endstream\n");
  }
  free(xdata);
  return;
}

/** 
 * Generate the string representation of a Stream
 *
 * @param obj
 *    PDF Object 
 *
 * String: "Stream Dictionary
 *          stream
 *            ... contents ...
 *          endstream"
 */
void
pdf_stream_string(pdf_object_t *obj) {
  pdf_object_t *r;
  pdf_stream_t *s = (pdf_stream_t *) obj;
  if(!s) {
      return;
  }
  if(s->encoding != PDF_ENCODE_NONE) {
    pdf_stream_encode(obj, s->dict, s->str->str, string_length(s->str), s->encoding);
    return;
  }
  if(!s->dict) {
      return;
  }
  r = pdf_dict_find_entry(s->dict, "Length");
  pdf_number_set(r, PDF_NUMBER_INT, string_length(s->str), 0);
  pdf_string(s->dict);
  if(!s->dict->str) {
      return;
  }
  obj->str = string_append(obj->str, s->dict->str->str);
  obj->str = string_append(obj->str, "stream\n");
  obj->str = string_printf_append(obj->str, "%s", s->str->str);
  obj->str = string_append(obj->str, "endstream\n");
  
}

/** 
 * Set the encoding or compression type for a stream
 *
 * @param obj
 *    PDF Stream Object
 * @param encoding
 *    Encoding Type
 */
void
pdf_stream_set_encoding(pdf_object_t *obj, int encoding) {
  pdf_stream_t *s = (pdf_stream_t *) obj;
  s->encoding = encoding;
}

/** 
 * Free a Stream
 *
 * @param 
 *    PDF Object 
 */
void
pdf_stream_free(pdf_object_t *obj) {
  pdf_stream_t *s = (pdf_stream_t *) obj;
  pdf_object_free(s->dict);
  string_free(&s->str);
  free(s);
  s = NULL;
}

/** 
 * Create a new Stream PDF Object 
 *
 * @return
 *    PDF Object
 */
pdf_object_t *
pdf_stream_new() {
  pdf_stream_t *s;
  s = (pdf_stream_t *) malloc(sizeof(pdf_stream_t));
  pdf_object_init((pdf_object_t *) s, PDF_STREAM, pdf_stream_string, pdf_stream_free);
  s->dict = pdf_dict_new();
  pdf_dict_add_int(s->dict, "Length", 0);
  s->str = string_new("");
  s->encoding = PDF_ENCODE_NONE;
  return (pdf_object_t *) s;
}

/** 
 * Generate the string representation of a Image
 *
 * @param obj
 *    PDF Object 
 *
 * String: "Image Dictionary
 *          Image stream"
 *
 */
void
pdf_image_string(pdf_object_t *obj) {
  int n;

  pdf_image_t *im = (pdf_image_t *) obj;
  pdf_dict_add_int(im->dict, "Width", im->width);
  pdf_dict_add_int(im->dict, "Height", im->height);
  pdf_dict_add_int(im->dict, "BitsPerComponent", 8);
  if(im->type == PDF_IMAGE_RGB) {
    pdf_dict_add_name(im->dict, "ColorSpace", "DeviceRGB");
    im->len = 3 * im->width * im->height;
  } else if(im->type == PDF_IMAGE_GRAY) {
    pdf_dict_add_name(im->dict, "ColorSpace", "DeviceGray");
    im->len = im->width * im->height;
  } else if(im->type == PDF_IMAGE_CMYK) {
    pdf_dict_add_name(im->dict, "ColorSpace", "DeviceCMYK");
    im->len = 4 * im->width * im->height;
  }
  n = im->len;

  pdf_stream_encode(obj, im->dict, im->data, n, im->encoding);

}

/** 
 * Set the encoding or compression for a PDF Image Object
 *
 * @param obj
 *    PDF Image Object
 * @param encoding
 *    Type of encoding to set
 */
void
pdf_image_set_encoding(pdf_object_t *obj, int encoding) {
  pdf_image_t *im = (pdf_image_t *) obj;
  im->encoding = encoding;
}

/** 
 * Set the data for a PDF Image Object
 * 
 * @param obj
 *    PDF Image Object
 * @param data
 *    Raw Image Data
 * @param w
 *    Width of Image
 * @param h
 *    Height of Image 
 * @param type
 *    PDF Image Type
 *    - PDF_IMAGE_RGB
 *    - PDF_IMAGE_GRAY
 *    - PDF_IMAGE_CMYK
 */
void
pdf_image_set(pdf_object_t *obj, char *data, int w, int h, int type) {
  pdf_image_t *im = (pdf_image_t *) obj;
  im->width  = w;
  im->height = h;
  im->type   = type;
  if(im->type == PDF_IMAGE_RGB) {
    im->len = 3 * im->width * im->height;
  } else if(im->type == PDF_IMAGE_GRAY) {
    im->len = im->width * im->height;
  } else if(im->type == PDF_IMAGE_CMYK) {
    im->len = 4 * im->width * im->height;
  }
  im->data = (char *) malloc(sizeof(char) * im->len);
  memcpy(im->data, data, im->len);
}

/** 
 * Free an Image
 *
 * @param 
 *    PDF Object 
 */
void
pdf_image_free(pdf_object_t *obj) {
  pdf_image_t *im = (pdf_image_t *) obj;
  pdf_object_free(im->dict);
  free(im->data);
  free(im);
  im = NULL;
}

/** 
 * Create a new Image PDF Object 
 *
 * @return
 *    PDF Object
 */
pdf_object_t *
pdf_image_new() {
  pdf_image_t *im;
  im = (pdf_image_t *) malloc(sizeof(pdf_image_t));
  pdf_object_init((pdf_object_t *)im, PDF_IMAGE, pdf_image_string, pdf_image_free);
  im->dict = pdf_dict_new();
  pdf_dict_add_int(im->dict, "Length", 0);
  pdf_dict_add_name(im->dict, "Subtype", "Image");  
  im->width = 0;
  im->height = 0;
  im->encoding = PDF_ENCODE_RLE;
  im->type = PDF_IMAGE_RGB;
  im->data = NULL;
  im->len = 0;
  return (pdf_object_t *) im;
}

/** 
 * Generate the string representation of an Array
 *
 * @param obj
 *    PDF Object 
 *
 * String: "[ ... contents ... ] "
 */
void
pdf_array_string(pdf_object_t *obj) {

  string *s;
  pdf_array_t *a = (pdf_array_t *) obj;
  obj->str = string_new("[ ");
  s = pdf_store_string(a->data, " ");
  if(s) {
    obj->str = string_append(obj->str, s->str);
    string_free(&s);
  }
  obj->str = string_append(obj->str, " ]");
}

/** 
 * Add a value to a PDF Array Object
 * 
 * @param obj
 *    PDF Array Object
 * @param new
 *    PDF Object to add to array
 */
int
pdf_array_add(pdf_object_t *obj, pdf_object_t *new) {
  pdf_array_t *a = (pdf_array_t *) obj;
  if(!obj || !new) {
      return 0;
  }
  return pdf_store_add(a->data, new);
}

/** 
 * Free an Array
 *
 * @param 
 *    PDF Object 
 */
void
pdf_array_free(pdf_object_t *obj) {
  pdf_array_t *a = (pdf_array_t *) obj;
  if(a) {
      if(a->data) {
          pdf_store_free(a->data);
      }
      free(a);
  }
  a = NULL;
}

/** 
 * Create a new Array PDF Object 
 *
 * @return
 *    PDF Object
 */
pdf_object_t *
pdf_array_new() {
  pdf_array_t *a;
  a = (pdf_array_t *)malloc(sizeof(pdf_array_t));
  a->data = pdf_store_new();
  if(!a->data) {
      free(a);
      a = NULL;
      return NULL;
  }
  pdf_object_init((pdf_object_t *) a, PDF_ARRAY, pdf_array_string, pdf_array_free);
  return (pdf_object_t *) a;
}

/** 
 * Add a PDF Object to a PDF Document
 * 
 * @param pdf
 *    PDF Document Object
 * @param obj
 *    Object to add to PDF Document Object
 *
 * Uses pdf_store 
 */
int 
pdf_object_add(pdf_t *pdf, pdf_object_t *obj) {
  int n;
  if(!obj) {
      return -1;
  }
  n = pdf_store_add(pdf->objs, obj);
  obj->id = n;
  return n;
}

/** 
 * Get the length or number of items of the PDF Storage
 *
 * @param s
 *    PDF Storage 
 */
int
pdf_store_length(pdf_store_t *s) {
  return s->n;
}

/** 
 * Get the i-th object from the PDF Storage 
 *
 * @param s
 *    PDF Storage
 * @param i
 *    Number of element to get [ 0 .. n-1 ] where n is the number of items
 */
pdf_object_t *
pdf_store_n(pdf_store_t *s, int i) {
  return s->objs[i];
}

/** 
 * Get a string representation of the i-th PDF Object in a PDF Storage
 * 
 * @param s
 *    PDF Storage
 * @param i
 *    Number of element to get [ 0 .. n-1 ] where n is the number of items
 *
 * Calls pdf_string()
 */
char * 
pdf_store_string_n(pdf_store_t *s, int i) {
  if(!s) {
    return NULL;
  }
  if(i < 0 && i >= s->n) {
    pdf_error("Attempt to access object outside of storage scope\n"
              "  [%d %d]: %d\n", 0, s->n, i);
    return NULL;
  }
  pdf_string(s->objs[i]);
  if(s->objs && s->objs[i] && s->objs[i]->str) {
    return s->objs[i]->str->str;
  }
  return "";
}

/** 
 * Generate the string representation of a Store
 *
 * @param obj
 *    PDF Object 
 *
 * String: " ... contents ... "
 */
string * 
pdf_store_string(pdf_store_t *s, char *sep) {
  int i;
  string *str;
  
  str = NULL;
  for(i = 0; i < s->n; i++) {
    str = string_append(str, pdf_store_string_n(s, i));
    if(i < s->n - 1) {
      str = string_append(str, sep);
    }
  }
  return str;
}

/** 
 * Free a Store
 *
 * @param 
 *    PDF Object 
 */
void
pdf_store_free(pdf_store_t *s) {
  int i;
  pdf_object_t *obj;
  for(i = 0; i < s->n; i++) {
    obj = pdf_store_n(s, i);
    pdf_object_free( obj );
  }
  free(s->objs);
  s->objs = NULL;
  free(s);
  s = NULL;
}

/** 
 * Create a new PDF Store 
 *
 * @return
 *    PDF Store
 */
pdf_store_t *
pdf_store_new() {
  pdf_store_t *s;
  s = (pdf_store_t *)malloc(sizeof(pdf_store_t));
  s->n = 0;
  s->nalloc = 4;
  s->objs = (pdf_object_t **) malloc(sizeof(pdf_object_t *) * s->nalloc);
  return s;
}

/** 
 * Add an object to the end of a PDF Storage 
 *
 * @param s
 *    PDF Storage 
 * @param new
 *    PDF Object to append to the PDF Storage
 */
int
pdf_store_add(pdf_store_t *s, pdf_object_t *new) {
  pdf_object_t **tmp;
  if(!s || !new) {
      return 0;
  }
  if(s->n + 1 >= s->nalloc) {
    s->nalloc = s->nalloc * 2;
    tmp = (pdf_object_t **) realloc(s->objs,sizeof(pdf_object_t *) * s->nalloc);
    if(!tmp) {
      pdf_error("Cannot allocate PDF storage\n");
      return 0;
    }
    s->objs = tmp;
  }
  s->objs[s->n] = new;
  s->n = s->n + 1;
  return s->n;
}

/** 
 * Generate the string representation of a String
 *
 * @param obj
 *    PDF Object 
 *
 * String: "string"
 */
void
pdf_string_string(pdf_object_t *obj) {
  pdf_string_t *s = (pdf_string_t *)obj;
  obj->str = string_printf(obj->str, "%s", s->str->str);
}

/** 
 * Generate the string representation of a Name
 *
 * @param obj
 *    PDF Object 
 *
 * String: "/name"
 */
void
pdf_name_string(pdf_object_t *obj) {
  pdf_name_t *s = (pdf_name_t *) obj;
  obj->str = string_printf(obj->str, "/%s", s->str->str);
}

/** 
 * Set the name of a PDF Name Object 
 *
 * @param obj
 *    PDF Name Object
 * @param name
 *    New name of PDF Object
 */
void
pdf_name_set(pdf_object_t *obj, char *name) {
  pdf_name_t *s = (pdf_name_t *) obj;
  if(s->str) {
    string_free(& s->str);
    s->str = NULL;
  }
  s->str = string_new(name);
}

/** 
 * Free a String
 *
 * @param 
 *    PDF Object 
 */
void
pdf_string_free(pdf_object_t *obj) {
  pdf_name_t *s = (pdf_name_t *) obj;
  string_free(&s->str);
  free(s);
  s = NULL;
}

/** 
 * Create a new Name PDF Object 
 *
 * @return
 *    PDF Object
 */
pdf_object_t * 
pdf_name_new(char *name) {
  pdf_name_t *s;
  s = (pdf_name_t *)malloc(sizeof(pdf_name_t));
  s->str = NULL;
  pdf_name_set((pdf_object_t *) s, name);
  pdf_object_init((pdf_object_t *) s, PDF_NAME, pdf_name_string, pdf_string_free);
  return (pdf_object_t *) s;  
};

/** 
 * Create a new String PDF Object 
 *
 * @return
 *    PDF Object
 */
pdf_object_t *
pdf_string_new() {
  pdf_string_t *s;
  s = (pdf_string_t *)malloc(sizeof(pdf_string_t));
  s->str = NULL;
  pdf_object_init((pdf_object_t *) s, PDF_STRING, pdf_string_string, pdf_string_free);
  return (pdf_object_t *) s;
}

/** 
 * Set the value of a PDF Number Object
 *
 * @param obj
 *    PDF Number Object
 * @param type
 *    Type of Number
 *    - PDF_NUMBER_INT
 *    - PDF_NUMBER_FLOAT
 * @param i
 *    Integer value to set
 * @param f
 *    Floating point value to set
 */
void
pdf_number_set(pdf_object_t *obj, int type, int i, float f) {
  pdf_number_t *n = (pdf_number_t *) obj;  
  if(!n) {
      return;
  }
  n->type = type;
  if(type == PDF_NUMBER_INT) {
    n->i = i;
  } else {
    n->f = f;
  }
}

/** 
 * Get the floating point value for a PDF Number Object 
 *
 * @param obj
 *    PDF Number Object
 * 
 * @return
 *    Floating point number 
 *    If not a floating point number, return 0
 */
float
pdf_number_get_float(pdf_object_t *obj) {
  pdf_number_t *n = (pdf_number_t *)obj;
  if(n->type == PDF_NUMBER_FLOAT) {
    return n->f;
  }
  pdf_error("PDF Number, accessing incorrect number type\n");
  return 0;
}

/** 
 * Get the integervalue for a PDF Number Object 
 *
 * @param obj
 *    PDF Number Object
 * 
 * @return
 *    Integer number 
 *    If not a floating point number, return 0
 */
int
pdf_number_get_int(pdf_object_t *obj) {
  pdf_number_t *n = (pdf_number_t *)obj;
  if(!n) {
      return 0;
  }
  if(n->type == PDF_NUMBER_INT) {
    return n->i;
  }
  pdf_error("PDF Number, accessing incorrect number type\n");
  return 0;
}

/** 
 * Generate the string representation of a Number
 *
 * @param obj
 *    PDF Object 
 *
 * String: "number"
 */
void
pdf_number_string(pdf_object_t *obj) {
  pdf_number_t *n = (pdf_number_t *) obj;
  if(n->type == PDF_NUMBER_INT) {
    obj->str = string_printf(obj->str, "%d", n->i);
  } else {
    obj->str = string_printf(obj->str, "%f", n->f);
  }
}

/** 
 * Free a Number
 *
 * @param 
 *    PDF Object 
 */
void
pdf_number_free(pdf_object_t *obj) {
  pdf_number_t *n = (pdf_number_t *) obj;
  free(n);
  n = NULL;
}

/** 
 * Create a new PDF Number Object 
 *
 * @return
 *    PDF Object
 */
pdf_object_t *
pdf_number_new(int type, int i, float f) {
  pdf_number_t *n;
  n = (pdf_number_t *) malloc(sizeof(pdf_number_t));
  pdf_object_init((pdf_object_t *) n, PDF_NUMBER, pdf_number_string, pdf_number_free);
  pdf_number_set((pdf_object_t *)n, type, i, f);
  return (pdf_object_t *) n;
}

/** 
 * Create a new PDF Page Object 
 *
 * @return
 *    PDF Object
 */
pdf_object_t * 
pdf_page_new(pdf_t *p) { 
  int n;
  pdf_object_t *new;
  pdf_object_t *ref, *x, *f, *a, *r;
  pdf_object_t *kids;

  new = pdf_dict_new();
  pdf_dict_add_name(new, "Type", "Page");
  pdf_dict_add_ref(new, "Parent");
  pdf_dict_add_dict(new, "Resources");
  pdf_dict_add_array(new, "MediaBox");
  pdf_dict_add_array(new, "Contents");

  /* Define Resources */
  ref = pdf_dict_find_entry(new, "Resources");
  pdf_dict_add_array(ref, "ProcSet");  
  pdf_dict_add_dict(ref, "Font");

  /* Add ProcSet */
  x = pdf_dict_find_entry(ref, "ProcSet");
  pdf_array_add(x, pdf_name_new("PDF"));
  pdf_array_add(x, pdf_name_new("Text"));

  /* Add Font */
  x = pdf_dict_find_entry(ref, "Font");
  pdf_dict_add_ref(x, "F1");
  
  f = pdf_font_new("Helvetica", "Type1");
  n = pdf_object_add(p, f);
  x = pdf_dict_find_entry(ref, "Font");
  x = pdf_dict_find_entry(x, "F1");
  pdf_ref_set(x, n, 0);

  /* Define Media Box */
  ref = pdf_dict_find_entry(new, "MediaBox");
  pdf_array_add(ref, pdf_number_new(PDF_NUMBER_INT, 0, 0));
  pdf_array_add(ref, pdf_number_new(PDF_NUMBER_INT, 0, 0));
  pdf_array_add(ref, pdf_number_new(PDF_NUMBER_INT, PDF_PAGE_LETTER_WIDTH, 0));
  pdf_array_add(ref, pdf_number_new(PDF_NUMBER_INT, PDF_PAGE_LETTER_HEIGHT, 0));

  /* Add Object */
  n = pdf_object_add(p, new);
  
  /* Tell the Kid Page about its Pages Parent */
  ref = pdf_dict_find_entry(new, "Parent");
  pdf_ref_set(ref, p->pages->id, 0);

  /* Tell the Pages Parent about the new Kid Page */
  ref = pdf_ref_new();
  if(ref) {
      pdf_ref_set(ref, n, 0);
      kids = pdf_dict_find_entry(p->pages, "Kids");
      if(!pdf_array_add(kids, ref)) {
          pdf_ref_free(ref);
      }
  }

  ref = pdf_dict_find_entry(p->pages, "Count");
  pdf_number_set(ref, PDF_NUMBER_INT, pdf_number_get_int(ref) + 1, 0);

  /* Open a new stream for content */
  p->stream = pdf_stream_new();
#ifdef HAVE_ZLIB
  pdf_stream_set_encoding(p->stream, PDF_ENCODE_NONE);
#else 
  pdf_stream_set_encoding(p->stream, PDF_ENCODE_RLE);
#endif
  n = pdf_object_add(p, p->stream);
  a = pdf_dict_find_entry(new, "Contents");
  r = pdf_ref_new();
  if(r) {
      pdf_ref_set(r, n, 0);
      if(!pdf_array_add(a, r)) {
          pdf_ref_free(r);
      }
  }

  p->page = new;
  return new;
}

/** 
 * Generate the string representation of a Reference
 *
 * @param obj
 *    PDF Object 
 *
 * String: "id gen R"
 */
void
pdf_ref_string(pdf_object_t *obj) {
  pdf_ref_t *r = (pdf_ref_t *) obj;
  obj->str = string_printf(obj->str, "%d %d R", r->id, r->gen);
}

/** 
 * Set the reference for a PDF Object 
 *
 * @param obj
 *    PDF Object
 * @param id
 *    ID of the object
 * @param gen
 *    Generation Number, normally 0
 */
void
pdf_ref_set(pdf_object_t *obj, int id, int gen) {
  pdf_ref_t *r = (pdf_ref_t *) obj;
  if(r) {
      r->id  = id;
      r->gen = gen;
  }
}

/** 
 * Free a PDF Reference
 *
 * @param 
 *    PDF Object 
 */
void
pdf_ref_free(pdf_object_t *obj) {
  pdf_ref_t *r = (pdf_ref_t *) obj;
  free(r);
  r = NULL;
}

/** 
 * Create a new PDF Reference Object
 * 
 * @return
 *    PDF Object
 */
pdf_object_t *
pdf_ref_new() {
  pdf_ref_t *r;
  r = (pdf_ref_t *) malloc(sizeof(pdf_ref_t));
  r->id = -1;
  r->gen = 0;
  pdf_object_init((pdf_object_t *) r, PDF_REF, pdf_ref_string, pdf_ref_free);
  return (pdf_object_t *) r;
}

/** 
 * Create a new PDF Trailer Object
 * 
 * @return
 *    PDF Object
 */
pdf_object_t *
pdf_trailer_new() {
  pdf_object_t *t;
  t = pdf_dict_new();
  pdf_dict_add_int(t, "Size", 0);
  pdf_dict_add_ref(t, "Root");
  return t;
}

/** 
 * Draw and fill a rectangle for entire page
 *    
 * @param pdf
 *    PDF Document Object
 * @param x
 *    Width of rectangle
 * @param y
 *    Height of rectangle
 * @param c
 *    Color of rectangle fill
 *
 * Rectangle is added to the current stream
 */
void
pdf_rectangle(pdf_t *pdf, float x, float y, pdf_color_t c) {
  pdf_save(pdf);
  pdf_color_fill(pdf, c);
  pdf_stream_add(pdf->stream, 
                 "0 0 m "
                 "%d 0 l "
                 "%d %d l "
                 "0 %d l "
                 "h f\n", x, x, y, y);

  pdf_restore(pdf);
}

/** 
 * Show an image, "Do"
 * 
 * @param pdf
 *    PDF Document Object
 * @param name
 *    Name of image to Show
 */
void
pdf_image_show(pdf_t *pdf, char *name) {
  pdf_stream_add(pdf->stream, "/%s Do\n", name);
}

/** 
 * Add Image Color Resources to a PDF Document Object
 *
 * @param pdf
 *    PDF Document Object
 *
 * ImageC is set in Resources->ProcSet
 */
void
pdf_image_color_add_resources(pdf_t *pdf) {
    pdf_object_t *r, *p, *n;
  r = pdf_dict_find_entry(pdf->page, "Resources");
  p = pdf_dict_find_entry(r, "ProcSet");
  n = pdf_name_new("ImageC");
  if(!pdf_array_add(p, n)) {
      pdf_string_free(n);
  }
}

/** 
 * Add an XObject to a PDF Document Object
 *
 * @param pdf
 *    PDF Document Object
 * @param image
 *    PDF Image Object added to XObject
 * @param name
 *    Name to add to XObject
 */
void
pdf_image_add_xobject(pdf_t *pdf, pdf_object_t *image, char *name) {
    pdf_object_t *r, *x, *z, *e;
  r = pdf_dict_find_entry(pdf->page, "Resources");
  if(!r) {
      return;
  }
  if(!pdf_dict_exists_entry(r, "XObject")) {
    pdf_dict_add_dict(r, "XObject");
  }
  if(!(x = pdf_dict_exists_entry(r, "XObject"))) {
    return;
  }
  if(!(z = pdf_ref_new())) {
    return;
  }
  pdf_ref_set(z, image->id, 0);
  if(!(e = pdf_dict_entry_new(name, z))) {
    pdf_ref_free(z);
    return;
  }
  if(!pdf_dict_add_entry(x, e)) {
    pdf_dict_entry_free(e);
    return;
  }
}

/** 
 * Encode a character stream as ASCII Hex
 *
 * @param p
 *    Raw character stream
 * @param np
 *    Number of points on input and output
 * 
 * @return
 *    Encoded character stream
 *
 * Individual Characters are encoded as Hex Values
 */
char *
ASCIIHexEncode(unsigned char *p, int *np) {
  int i;
  char *in;
  int n;
  unsigned short hex;
  n = *np;
  in = (char *) malloc(sizeof(char) * ((n*2) + 4));
  for(i = 0; i < n; i++) {
    hex = (unsigned short) p[i];
    sprintf(&in[i*2], "%2.2hx", hex);
  }
  in[n*2  ] = '>';
  in[n*2+1] = '\n';
  in[n*2+2] = '0';
  *np = n*2+2;
  return in;
}

/** 
 * Encode a character stream with Run Length Encoding
 *
 * @param p
 *    Raw character stream
 * @param np
 *    Number of points on input and output
 * 
 * @return
 *    Encoded character stream
 *
 * Individual Characters are encoded as binary and repeated characters 
 *    are compressed into smaller blocks
 */
char * 
RunLengthEncode(unsigned char *p, int *n) {
  char *in;
  int i, k, j, x,y;
  int len;
  char t[129];

  len = 128;

  in = (char *) malloc(sizeof(char) * ((*n * 2)+1));
  
  j = 0;
  k = 0;
  y = 0;
  t[k++] = p[0];

  for(i = 1; i < *n; i++) { 
    if(k >= len) { /* Write out full uncompressed block */
      in[j++] = k-1;  for(x = 0; x < k; x++) { in[j++] = t[x]; }
      k = 0;
    }

    if(k > 0) { /* If there are previous records */
      if(t[k-1] != p[i]) { /* No Match */
        if(y > 0) { /* Handle Previous Matches, if we already found a match */
          in[j++] = (unsigned int) 255 - y + 1; /* 257 => 2, 256 => 3, (257 - length) */
          in[j++] = t[0]; /* Byte to be copied */
          y = 0;
          k = 0;
        }
        t[k++] = p[i]; 
      } else { /* Match found */
        if(k > 1) { /* Write out previously found bytes */
          in[j++] = k-2;  for(x = 0; x < k-1; x++) { in[j++] = t[x]; }
        }
        k = 0;
        t[k++] = p[i]; /* Save Current Byte to Record Start */
        y++;
      }
    } else {
      t[k++] = p[i]; /* Nothing, then just copy */
    }
  }
  /* Final Characters */
  if(y == 0) {
    in[j++] = k-1; for(x = 0; x < k; x++) { in[j++] = t[x]; }
  } else {
    in[j++] = (unsigned int) 255 - y + 1;
    in[j++] = t[0];
  }
  in[j++] = 128; /* End of Encoding */
  in[j++] = '\n';
  *n = j;
  return in;
}

#ifdef HAVE_ZLIB

#define BUFSIZE 2048

/** 
 * Encode a character stream with Flate Encoding (libz)
 *
 * @param p
 *    Raw character stream
 * @param np
 *    Number of points on input and output
 * 
 * @return
 *    Encoded character stream
 *
 * Compression is accomplished using the routines in libz
 *   
 */
char * 
FlateEncode(unsigned char *data, int *n) {
  int ret, flush;
  unsigned int have;
  char *out, *tmp;
  int i, j,  len;
  z_stream strm;
  
  strm.zalloc = NULL;
  strm.zfree  = NULL;
  strm.opaque = NULL;
  
  len = BUFSIZE ;
  tmp = (char *) malloc(sizeof(char) * len);
  out = (char *) malloc(sizeof(char) * len);

  i = 0;
  j = 0;

  ret = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
  if(ret != Z_OK) {
    pdf_error("Cannot initialize zlib stream\n");
    return NULL;
  }
  do {
    if(i + BUFSIZE > *n) {
      strm.avail_in = *n - i;
      flush = Z_FINISH;
    } else {
      strm.avail_in = BUFSIZE;
      flush = Z_NO_FLUSH;
    }
    strm.next_in = &data[i];
    i = i + BUFSIZE;
    do {
      strm.avail_out = BUFSIZE;
      strm.next_out = (unsigned char *)tmp;
      deflate(&strm, flush);
      have = BUFSIZE - strm.avail_out;
      if((int)(j + have) > len) {
        len = len * 2;
        out = realloc(out, sizeof(char) * len);
      }
      memcpy(&out[j], tmp, have);
      j = j + have;
    } while(strm.avail_out == 0);
  } while(flush != Z_FINISH);
  (void)deflateEnd(&strm);

  *n = j;
  free(tmp);
  return out;
}

#else 

char * 
FlateEncode(unsigned char *data, int *n) {
  fprintf(stderr, "sac: Writing to PDF files with Zlib/Flate Compreesion\n"
          "    Not supported in the vertion\n"
          "    This functionality can be compiled into SAC\n");
  return NULL;
}

#endif

