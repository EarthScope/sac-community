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

#ifndef _PS_H_
#define _PS_H_

#include <stdio.h>
#include <stdarg.h>

#include "string_utils.h"

enum {
    PS_LINE_JOIN_STYLE_MITER = 0,
    PS_LINE_JOIN_STYLE_ROUND = 1,
    PS_LINE_JOIN_STYLE_BEVEL = 2,
} PS_LINE_JOIN_STYLE;

typedef struct _ps_t ps_t;
struct _ps_t {
  FILE *fp;
};

typedef struct _ps_color_t ps_color_t;
struct _ps_color_t {
  float r,g,b;
};
ps_t * ps_new();
int    ps_file_open(ps_t *ps, char *file);
ps_t * ps_current(ps_t *ps);
void   ps_close(ps_t *ps);

void ps_header(ps_t *ps);
void ps_footer(ps_t *ps);
void ps_stroke(ps_t *ps);
void ps_fill(ps_t *ps);
void ps_save(ps_t *ps);
void ps_restore(ps_t *ps);
void ps_color(ps_t *ps, ps_color_t c);
void ps_lineto(ps_t *ps, float x, float y);
void ps_moveto(ps_t *ps, float x, float y);
void ps_newline(ps_t *ps);
void ps_linewidth(ps_t *ps, int size);
void ps_rotate(ps_t *ps, float angle);
void ps_textsize(ps_t *ps, float size);
void ps_textshow(ps_t *ps, char *text);
void ps_textjustify(ps_t *ps, char *text, int h, int v, float size);
void ps_rectangle(ps_t *ps, float x, float y);
void ps_image(ps_t *ps, char *data, float x, float y, unsigned int w, unsigned int h);
void ps_line_join_style(ps_t *ps, int style);

void ps_raw(ps_t *ps, char *fmt, ...);
#endif /* _PS_H_ */
