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
#include "gfxText.h"

static long frameTime[50] = { [0 ... 49] = 0 };
static unsigned int frameNbr = 0;

void gfxRegisterFrame( void ) {

   frameTime[frameNbr++] = SDL_GetTicks();
   frameNbr %= 50;
}

float gfxGetFrameRate( void ) {
   return  1000.0f / ( ( SDL_GetTicks() - frameTime[frameNbr] ) / 50.0f );
}

void gfxDrawDbgHUD( void ) {

   GLint err = glGetError();

   float offset;

   offset = gfxDrawDbgTextFmt( 0.01, 0.0, 0.02,  "GL Error:   " );

   if ( err != GL_NO_ERROR ) {
      glColor3f( 1, 0, 0 );
   } else {
      glColor3f( .6, .6, 1 );
   }

   gfxDrawDbgTextFmt( offset, 0.0, 0.02, "%s",
                      gluErrorString( err ) );

   glColor3f( .8, .8, .8 );
   gfxDrawDbgTextFmt( 0.01, 0.02, 0.02, "%2.2f fps", gfxGetFrameRate() );

}
