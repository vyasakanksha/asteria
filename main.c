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
#include <stdio.h>

#include <unistd.h> /* For sleep(). */

#include <math.h>

#include "libInclude.h"
#include "gfxInit.h"
#include "gfxShader.h"

#include "vMath.h"

#include "md5Models.h"

int main( int argc, char * argv[] ) { 

   int i;


   GLuint vtx, frg, prog;


   SDL_Init( SDL_INIT_EVERYTHING );

   SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1 );
   SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 4 );

   gfxSetupOsWindow( 800, 600 );
   gfxInitializeOpenGL();

   if ( GLEW_OK != glewInit() ) {
      fprintf( stderr, "GLEW Failed to initialize.\n" );
      exit( 1 );
   }


   /* Makeshift lighting. */
   GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat mat_shininess[] = { 50.0 };
   GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);

   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);

   vtx  = gfxMakeShader( "shader/test.vtx" );
   frg  = gfxMakeShader( "shader/test.frg" );
   prog = gfxMakeProgram( vtx, frg );
   
   glUseProgram( prog );

   glEnableClientState( GL_VERTEX_ARRAY );
   glEnableClientState( GL_NORMAL_ARRAY );


   for ( i = 0; i < 200; ++i ) {
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

      glLoadIdentity();
      glTranslatef( 0.0f, 0.0f, -3.0f );
      glRotatef( 180.0f, 1.0f, 0.0f, 0.0f );
      glRotatef( 3.0f * i, 0.0f, 1.0f, 0.0f );

      glFinish(); 

      SDL_GL_SwapBuffers();
      SDL_Delay( 25 );
   }

   sleep( 1 );

   SDL_Quit();

   return 0;
}
