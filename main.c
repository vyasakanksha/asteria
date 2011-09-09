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

/* main.c */
#include <stdio.h>

#include <unistd.h> /* For sleep(). */

#include <math.h>

#include "libInclude.h"
#include "gfxInit.h"

#include "vMath.h"

#include "md5Models.h"

int main( int argc, char * argv[] ) { 

   int i;

   md5SimpleBindPose * model;


   model = md5GetSimpleBindPose( "robot" );

   SDL_Init( SDL_INIT_EVERYTHING );

   gfxSetupOsWindow( 1600, 900 );
   gfxInitializeOpenGL();

   if ( GLEW_OK != glewInit() ) {
      fprintf( stderr, "GLEW Failed to initialize.\n" );
      exit( 1 );
   }

   glTranslatef( 0.0f, 0.0f, -5.0f );

   glEnableClientState( GL_VERTEX_ARRAY );

   glVertexPointer( 3, GL_FLOAT, sizeof( vec4 ), model->verts );

   for ( i = 0; i < 200; ++i ) {
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

      glRotatef( 3.0f, 0.0f, 1.0f, 0.0f );
      glDrawElements( GL_TRIANGLES, model->numIdx, GL_UNSIGNED_INT, model->idxs );

      glFinish(); 

      SDL_GL_SwapBuffers();
      SDL_Delay( 25 );
   }

   sleep( 1 );

   SDL_Quit();

   return 0;
}
