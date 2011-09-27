/****************************************************************************
 *                                                                          *
 * This file is part of the Asteria project.                                *
 * Copyright (C) 2011 Samuel C. Payson, Akanksha Vyas                       *
 *                                                                          *
 * Asteria is free software: you can redistribute it and/or modify it under *
 * the terms of the GNU General Public License as published by the Free     *
 * Software Foundation, either version 3 of the License, or (at your        *
 * option) any later version.                                               *
 *                                                                          *
 * Asteria is distributed in the hope that it will be useful, but WITHOUT   *
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or    *
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License    *
 * for more details.                                                        *
 *                                                                          *
 * You should have received a copy of the GNU General Public License along  *
 * with Asteria. If not, see <http://www.gnu.org/licenses/>.                *
 *                                                                          *
 ****************************************************************************/

#include "libInclude.h"

#define GFX_MAX_STR_LEN 256

// Maps characters to their coordinates on the bitmapped texture.
static const int charLookup[][2] = {
   ['a'] = { 0, 7 }, ['b'] = { 1, 7 }, ['c'] = {  2, 7 }, ['d'] = {  3, 7 },
   ['e'] = { 4, 7 }, ['f'] = { 5, 7 }, ['g'] = {  6, 7 }, ['h'] = {  7, 7 },
   ['i'] = { 8, 7 }, ['j'] = { 9, 7 }, ['k'] = { 10, 7 }, ['l'] = { 11, 7 },
   ['m'] = { 0, 6 }, ['n'] = { 1, 6 }, ['o'] = {  2, 6 }, ['p'] = {  3, 6 },
   ['q'] = { 4, 6 }, ['r'] = { 5, 6 }, ['s'] = {  6, 6 }, ['t'] = {  7, 6 },
   ['u'] = { 8, 6 }, ['v'] = { 9, 6 }, ['w'] = { 10, 6 }, ['x'] = { 11, 6 },
   ['y'] = { 0, 5 }, ['z'] = { 1, 5 }, ['A'] = {  2, 5 }, ['B'] = {  3, 5 },
   ['C'] = { 4, 5 }, ['D'] = { 5, 5 }, ['E'] = {  6, 5 }, ['F'] = {  7, 5 },
   ['G'] = { 8, 5 }, ['H'] = { 9, 5 }, ['I'] = { 10, 5 }, ['J'] = { 11, 5 },
   ['K'] = { 0, 4 }, ['L'] = { 1, 4 }, ['M'] = {  2, 4 }, ['N'] = {  3, 4 },
   ['O'] = { 4, 4 }, ['P'] = { 5, 4 }, ['Q'] = {  6, 4 }, ['R'] = {  7, 4 },
   ['S'] = { 8, 4 }, ['T'] = { 9, 4 }, ['U'] = { 10, 4 }, ['V'] = { 11, 4 },
   ['W'] = { 0, 3 }, ['X'] = { 1, 3 }, ['Y'] = {  2, 3 }, ['Z'] = {  3, 3 },
   ['`'] = { 4, 3 }, ['1'] = { 5, 3 }, ['2'] = {  6, 3 }, ['3'] = {  7, 3 },
   ['4'] = { 8, 3 }, ['5'] = { 9, 3 }, ['6'] = { 10, 3 }, ['7'] = { 11, 3 },
   ['8'] = { 0, 2 }, ['9'] = { 1, 2 }, ['0'] = {  2, 2 }, ['-'] = {  3, 2 },
   ['='] = { 4, 2 }, ['~'] = { 5, 2 }, ['!'] = {  6, 2 }, ['@'] = {  7, 2 },
   ['#'] = { 8, 2 }, ['$'] = { 9, 2 }, ['%'] = { 10, 2 }, ['^'] = { 11, 2 },
   ['&'] = { 0, 1 }, ['*'] = { 1, 1 }, ['('] = {  2, 1 }, [')'] = {  3, 1 },
   ['_'] = { 4, 1 }, ['+'] = { 5, 1 }, ['['] = {  6, 1 }, [']'] = {  7, 1 },
   ['\\']= { 8, 1 }, [';'] = { 9, 1 }, ['\'']= { 10, 1 }, [','] = { 11, 1 },
   ['.'] = { 0, 0 }, ['/'] = { 1, 0 }, ['{'] = {  2, 0 }, ['}'] = {  3, 0 },
   ['|'] = { 4, 0 }, [':'] = { 5, 0 }, ['"'] = {  6, 0 }, ['<'] = {  7, 0 },
   ['>'] = { 8, 0 }, ['?'] = { 9, 0 }, [' '] = { 10, 0 } };

void gfxVDrawDbgTextFmt( float x, float y, float size,
                      const char * fmt, va_list argls ) {
   char out[GFX_MAX_STR_LEN];
   int i;

   vsnprintf( out, GFX_MAX_STR_LEN, fmt, argls );

   for ( i = 0; out[i] != '\0'; ++i ) {
      float topX = charLookup[(int)out[i]][0] * ( 1.0f / 12.0f ),
            topY = charLookup[(int)out[i]][1] * ( 1.0f / 8.0f );
      switch ( out[i] ) {
         // Printable ASCII characters. The engine officially uses UTF-8, but
         // for bitmapped fonts, only ASCII is feasibly supported. I am
         // currently looking into truetype fonts.
         case ' ' ... '~':
            glBegin( GL_TRIANGLE_STRIP );
               glTexCoord2f( topX, topY );
               glVertex2f( x, y );
               glTexCoord2f( topX + ( 1.0f / 12.0f ), topY );
               glVertex2f( x + ( size * 0.6667f ), y );
               glTexCoord2f( topX, topY + ( 1.0f / 8.0f ) );
               glVertex2f( x, y + size );
               glTexCoord2f( topX + ( 1.0f / 12.0f ), topY + ( 1.0f / 8.0f ) );
               glVertex2f( x + ( size * 0.6667f ), y + size );
            glEnd();
         default:
            // We don't print any newline/tab/return characters.
            break;
      }
      x += ( size * 0.6667f );
   }
}

// This code assumes we are in 'overlay mode'. Incidentally, overlay mode
// does not yet exist. Strings are truncated to 256 characters.
void gfxDrawDbgTextFmt( float x, float y, float size, const char * str, ... ) {
   va_list argls;
   va_start( argls, str );
   gfxVDrawDbgTextFmt( x, y, size, str, argls );
   va_end( argls );
}
