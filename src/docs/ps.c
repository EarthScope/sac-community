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

#include "ps.h"
#include "debug.h"

ps_t *
ps_new() {
  ps_t *ps;
  ps = (ps_t *) malloc(sizeof(ps_t));
  if(!ps) {
    fprintf(stderr, "Cannot allocate Postscript Object\n");
    return NULL;
  }
  ps->fp = NULL;
  return ps;
}

int
ps_file_open(ps_t *ps, char *file) {
  ps->fp = fopen(file, "w");
  if(!ps->fp) {
    return 0;
  }
  return 1;
}

ps_t * 
ps_current(ps_t *ps) {
  static ps_t *current;
  if(ps != NULL) {
    current = ps;
  }
  return current;
}

void
ps_close(ps_t *ps) {
  fclose(ps->fp);
}

void
ps_header(ps_t *ps) {
    fprintf(ps->fp, "%%!PS-Adobe-3.0 EPSF-3.0\n");
    fprintf(ps->fp, "%%%%BoundingBox: 0 0 612 472\n");
    fprintf(ps->fp, "%%%%Creator: sgftops (20090313)\n");
    fprintf(ps->fp, "%%%%Pages: 1\n");
    fprintf(ps->fp, "%%%%Orientation: Portrait\n");
    fprintf(ps->fp, "%%%%EndComments\n");
    fprintf(ps->fp, "%%%%BeginProlog\n");
    fprintf(ps->fp, "/Helvetica findfont 13 scalefont setfont\n");
    fprintf(ps->fp, "\n"
            "/ils1 {[]} def\n"
            "/ils2 {[1.1363 1.7045] } def\n"
            "/ils3 {[6.8181 4.4545] } def\n"
            "/ils4 {[13.6363 6.8181] } def\n"
            "/ils5 {[13.6363 4.5454 1.7045 4.5454] } def\n"
            "/ils6 {[13.6363 4.5454 6.8181 4.5454] } def\n"
            "/ils7 {[13.6363 4.5454 1.7045 4.5454 1.7045 4.5454] } def\n"
            "/ils8 {[13.6363 4.5454 6.8181 4.5454 6.8181 6.8181] } def\n"
            "/ils9 {[13.6363 4.5454 1.7045 4.5454 1.7045 4.5454 1.7045 4.5454] } def\n"
            "/ils10 {[13.6363 4.5454 6.8181 4.5454 6.8181 4.5454 6.8181 4.5454] } def\n"
            "/xls   { {1 mul} forall count array astore } def\n"
            "/xls1  { [] 0 setdash} def\n"
            "/xls2  { ils2 xls 0 setdash} def\n"
            "/xls3  { ils3 xls 0 setdash} def\n"
            "/xls4  { ils4 xls 0 setdash} def\n"
            "/xls5  { ils5 xls 0 setdash} def\n"
            "/xls6  { ils6 xls 0 setdash} def\n"
            "/xls7  { ils7 xls 0 setdash} def\n"
            "/xls8  { ils8 xls 0 setdash} def\n"
            "/xls9  { ils9 xls 0 setdash} def\n"
            "/xls10 { ils10 xls 0 setdash} def\n");
    fprintf(ps->fp, "/c {closepath} def\n");
    fprintf(ps->fp, "/L {lineto} def\n");
    fprintf(ps->fp, "/R {rlineto} def\n");
    fprintf(ps->fp, "/m {moveto} def\n");
    fprintf(ps->fp, "/g {setgray} def\n");
    fprintf(ps->fp, "/s {stroke} def\n");
    fprintf(ps->fp, "/N {newpath} def\n");
    fprintf(ps->fp, "/stringsize { dup stringwidth pop /width exch def gsave newpath 0 0 moveto true charpath flattenpath pathbbox /height exch def pop pop pop grestore } def\n");
    fprintf(ps->fp, "/GetFontHeight { gsave true charpath pathbbox 3 -1 roll sub 3 1 roll pop pop /FontHeight exch def grestore } def\n");
    fprintf(ps->fp, "/T { /Helvetica findfont exch scalefont setfont } def\n");
    /* define procedure to make filled rectangles */
    fprintf(ps->fp, "/F {\n");
    fprintf(ps->fp, "   /blue exch def\n");
    fprintf(ps->fp, "   /green exch def\n");
    fprintf(ps->fp, "   /red exch def\n");
    fprintf(ps->fp, "   /dy exch def\n");
    fprintf(ps->fp, "   /dx exch def\n");
    fprintf(ps->fp, "   /y exch def\n");
    fprintf(ps->fp, "   /x exch def\n");
    fprintf(ps->fp, "   s\n");
    fprintf(ps->fp, "   red green blue setrgbcolor\n");
    fprintf(ps->fp, "   x y m\n");
    fprintf(ps->fp, "   0 dy rlineto\n");
    fprintf(ps->fp, "   dx 0 rlineto\n");
    fprintf(ps->fp, "   0 dy neg rlineto\n");
    fprintf(ps->fp, "   closepath\n");
    fprintf(ps->fp, "   fill\n");
    fprintf(ps->fp, "} def\n");

    ps_newline(ps);
    fprintf(ps->fp, "%%%%EndProlog\n");
    fprintf(ps->fp, "%%%%Page: 1 1\n");
    
    ps_save(ps);
    fprintf(ps->fp, "8.5 11 div 8.5 11 div scale \n");
    ps_linewidth(ps, 1 );
}

void
ps_footer(ps_t *ps) {
  fprintf(ps->fp, "showpage\n");
  ps_restore(ps);
  ps_newline(ps);
  fprintf(ps->fp, "%%%%Trailer\n");
}

void
ps_stroke(ps_t *ps) {
  fprintf(ps->fp, "s\n");
}

void
ps_fill(ps_t *ps) {
  fprintf(ps->fp, "fill\n");
}

void
ps_line_join_style(ps_t *ps, int style) {
    fprintf(ps->fp, "%d setlinejoin\n", style);
}

void
ps_save(ps_t *ps) {
  fprintf(ps->fp, "gsave\n");
}
void
ps_restore(ps_t *ps) {
  fprintf(ps->fp, "grestore\n");
}

void
ps_color(ps_t *ps, ps_color_t c) {
  fprintf(ps->fp, "%f %f %f setrgbcolor\n", c.r, c.g, c.b);
}

void
ps_lineto(ps_t *ps, float x, float y) {
  fprintf(ps->fp, "%f %f L\n", x, y);
}

void
ps_moveto(ps_t *ps, float x, float y) {
  fprintf(ps->fp, "%f %f m\n", x, y);
}

void
ps_newline(ps_t *ps) {
  fprintf(ps->fp, "\n");
}

void
ps_linewidth(ps_t *ps, int size) {
  fprintf(ps->fp, "%d setlinewidth\n", size);
}

void
ps_rotate(ps_t *ps, float angle) {
  fprintf(ps->fp,"%f rotate\n", angle);
}

void
ps_textsize(ps_t *ps, float size) {
  fprintf(ps->fp, "%f T\n", size);
}

void
ps_textshow(ps_t *ps, char *text) {
  fprintf(ps->fp, "(%s) show\n", text);
}

void
ps_textjustify(ps_t *ps, char *text, int h, int v, float size) {
  float off[4] = { 0.0, 0.0, -0.5, -1.0 };
  UNUSED(size);
  fprintf(ps->fp, "(%s) stringsize\n", text);
  fprintf(ps->fp, "(%s) GetFontHeight\n", text);
  fprintf(ps->fp, "width %f mul FontHeight %f mul rmoveto\n", off[h], off[v]);
}

void
ps_rectangle(ps_t *ps, float x, float y) {
  fprintf(ps->fp, "N 0 0 m %f 0 R 0 %f R %f 0 R c fill\n", x, y, -x);  
}

void
ps_image(ps_t *ps,
         char *data,
         float x,
         float y,
         unsigned int w,
         unsigned int h) {
  int i;
  unsigned short hex;
  unsigned char *ptr;
  
  ptr = (unsigned char *) data;
  ps_newline(ps);
  ps_save(ps);

  fprintf(ps->fp, "/picstr %d string def\n", 3 * w);
  fprintf(ps->fp, "%f %f translate\n", x, y);
  fprintf(ps->fp, "%d %d scale\n", w, h);
  fprintf(ps->fp, "%d %d 8\n", w, h);
  fprintf(ps->fp, "[ %d 0 0 %d 0 %d ] \n", w, -h, h);
  fprintf(ps->fp, "{ currentfile \n picstr readhexstring pop }\n");
  fprintf(ps->fp, "false 3\n");
  fprintf(ps->fp, "colorimage\n\n");
  for(i = 0; i < (int)(3 * w * h); i++) {
    hex = (unsigned short)ptr[i];
    fprintf(ps->fp, "%2.2hx", hex);
    if(i%24 == 1 ) {
      ps_newline(ps);
    }
  }
  ps_newline(ps);
  ps_restore(ps);
}

void
ps_raw(ps_t *ps, char *fmt, ...) {
  va_list args;

  va_start(args, fmt);
  vfprintf(ps->fp, fmt, args);
  va_end(args);
  
}
