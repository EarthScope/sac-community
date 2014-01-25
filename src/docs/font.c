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

#include <string.h>

#include "font.h"

#include "Courier-Bold.h"
#include "Courier-BoldOblique.h"
#include "Courier-Oblique.h"
#include "Courier.h"
#include "Helvetica-Bold.h"
#include "Helvetica-BoldOblique.h"
#include "Helvetica-Oblique.h"
#include "Helvetica.h"
#include "Symbol.h"
#include "Times-Bold.h"
#include "Times-BoldItalic.h"
#include "Times-Italic.h"
#include "Times-Roman.h"
#include "ZapfDingbats.h"


char NAME[] = "Times";
static const int NAME_A = 1;
int NAME_D = 1;
int NAME_W = 1;
FontMetricsWidths NAME_WID[] = { {0,0,"X"} };

FontMetrics Base14Fonts[] = {
  /* Regular Fonts */
  { 
    Helvetica_Name, 
    Helvetica_Ascent, Helvetica_Descent, Helvetica_Average_Width,
    Helvetica_Widths
  },       
  { 
    Courier_Name, 
    Courier_Ascent, Courier_Descent, Courier_Average_Width,
    Courier_Widths
  },       
  { 
    Times_Name, 
    Times_Ascent, Times_Descent, Times_Average_Width,
    Times_Widths
  },       

  /* Bold Fonts */
  { 
    Helvetica_Bold_Name, 
    Helvetica_Bold_Ascent, Helvetica_Bold_Descent, Helvetica_Bold_Average_Width,
    Helvetica_Bold_Widths
  },       
  { 
    Courier_Bold_Name, 
    Courier_Bold_Ascent, Courier_Bold_Descent, Courier_Bold_Average_Width,
    Courier_Bold_Widths
  },       
  { 
    Times_Bold_Name, 
    Times_Bold_Ascent, Times_Bold_Descent, Times_Bold_Average_Width,
    Times_Bold_Widths
  },       

  /* Bold Italic Fonts */ 
  { 
    Helvetica_BoldItalic_Name, 
    Helvetica_BoldItalic_Ascent, Helvetica_BoldItalic_Descent, Helvetica_BoldItalic_Average_Width,
    Helvetica_BoldItalic_Widths
  },       
  { 
    Courier_BoldItalic_Name, 
    Courier_BoldItalic_Ascent, Courier_BoldItalic_Descent, Courier_BoldItalic_Average_Width,
    Courier_BoldItalic_Widths
  },       
  { 
    Times_BoldItalic_Name, 
    Times_BoldItalic_Ascent, Times_BoldItalic_Descent, Times_BoldItalic_Average_Width,
    Times_BoldItalic_Widths
  },       

  /* Symbol and ZapfDingbats */
  { 
    Symbol_Name, 
    Symbol_Ascent, Symbol_Descent, Symbol_Average_Width,
    Symbol_Widths
  },       
  { 
    ZapfDingbats_Name, 
    ZapfDingbats_Ascent, ZapfDingbats_Descent, ZapfDingbats_Average_Width,
    ZapfDingbats_Widths
  },       
};


#define Base14FontLength   14

#define DEFAULT_WIDTH      575
#define DEFAULT_ASCENT     650
#define DEFAULT_DESCENT   -200

/** 
 * Find a font by name
 *
 * @param name
 *    Font name to find
 *
 * @return 
 *    FontMetrics specifying the size of characters
 */
FontMetrics * 
base14font_find(char *name) {
  int i;
  for(i = 0; i < Base14FontLength; i++) {
    if(strcasecmp(name, Base14Fonts[i].name) == 0) {
      //return Base14Fonts[i];
      return &(Base14Fonts[i]);
    }
  }
  
  return NULL;
}


/** 
 * Get ascent of a Font
 *
 * @param font
 *    Font Name
 * @param size
 *    Font Size
 *
 * @return
 *    Ascent of a Font
 */
int
string_ascent(char *font, float size) {
  int ascent;
  FontMetrics *Font = base14font_find(font);
  if(!Font) {
    ascent = DEFAULT_ASCENT;
  } else {
    ascent = Font->ascent;
  }
  return (int)(ascent * size / 1000.0);
}

/** 
 * Get descent of a Font 
 *
 * @param font
 *    Font Name
 * @param size
 *    Font Size
 *
 * @return
 *    Descent of a Font
 */
int
string_descent(char *font, float size) {
  int descent;
  FontMetrics *Font = base14font_find(font);
  if(!Font) {
    descent = DEFAULT_DESCENT;
  } else {
    descent = Font->descent;
  }
  return (int)(-1.0 * descent * size / 1000.0);
}

/** 
 * Get average width of a Font 
 *
 * @param font
 *    Font Name
 * @param size
 *    Font Size
 *
 * @return
 *    Average width of a Font
 */
int
string_average_width(char *font, float size) {
  int width;
  FontMetrics *Font = base14font_find(font);
  if(!Font) {
    width = DEFAULT_WIDTH;
  } else {
    width = Font->average_width;
  }
  return (int)(width * size / 1000.0);
}

/** 
 * Get heigth of a Font 
 *
 * @param font
 *    Font Name
 * @param size
 *    Font Size
 *
 * @return
 *    Height of a Font
 */
int
string_height(char *font, float size) {
  int height;
  FontMetrics *Font = base14font_find(font);
  if(!Font) {
    height = DEFAULT_ASCENT - DEFAULT_DESCENT;
  } else {
    height = Font->ascent - Font->descent;
  }
  
  return (int)(height * size / 1000);
}

/** 
 * Get width of a string using a Font
 *
 * @param font
 *    Font Name
 * @param text
 *    Character String
 * @param size
 *    Font Size
 *
 * @return
 *    Width of a string using a font
 */
int
string_width(char *font, char *text, float size) {
  char c;
  int i;
  int width;
  FontMetrics *Font = base14font_find(font);
  
  width = 0;
  if(!Font) {
    width = strlen(text) * DEFAULT_WIDTH;
  } else {
   for(i = 0; i < (int)strlen(text); i++) {
      c = text[i];
      width += Font->widths[c-32].width;
    }
  }
  width = width * size / 1000;
  return width;
}
