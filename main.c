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
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h> /* For sleep(). */

#include <math.h>

#include "libInclude.h"
#include "gfxInit.h"
#include "gfxTexture.h"
#include "gfxModes.h"
#include "gfxText.h"
#include "gfxConfig.h"

int main( int argc, char * argv[] ) { 

   int i;

   double GL_Version;

   float offset;

   gfxLoadConfig();

   SDL_Init( SDL_INIT_EVERYTHING );

   SDL_Rect ** modes = SDL_ListModes( NULL, SDL_OPENGL );

   if ( modes == NULL ) {
      fprintf( stderr, "No video modes available...\n" );
      exit( 1 );
   } else if ( modes == (SDL_Rect **)-1 ) {
      fprintf( stderr, "Video modes unrestricted\n" );
      gfxSetupOsWindow( gfxConfig.xRes, gfxConfig.yRes );
   } else {
      gfxSetupOsWindow( (*modes)->w, (*modes)->h );
      for ( ; *modes; ++modes ) {
         fprintf( stderr, "width: %d, height: %d\n", (*modes)->w, (*modes)->h );
      }
   }

   gfxInitializeOpenGL();

   GL_Version = strtod( (char *)glGetString( GL_VERSION ), NULL );
   if ( GL_Version < 2.1f ) {
      fprintf( stderr, 
      "OpenGL Version must be at least 2.1, but you appear to be running\n"
      "OpenGL %2.1f. Be sure that your hardware supports OpenGL 2.1, and\n"
      "that you have the most up-to-date drivers for your graphics hardware.\n",
      GL_Version );
      SDL_Quit();
      exit( 1 );
   } else {
      printf( "Detected OpenGL Version %2.1f, Have Fun!\n", GL_Version );
   }

   if ( GLEW_OK != glewInit() ) {
      fprintf( stderr, "GLEW Failed to initialize.\n" );
      SDL_Quit();
      exit( 1 );
   }

   glEnable( GL_TEXTURE_2D );
   glEnable( GL_BLEND );
   glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

   glBindTexture( GL_TEXTURE_2D, gfxTextureFromTiff( "res/FreeMono.tiff" ) );
   glLoadIdentity();

   for ( i = 0; i < 51; ++i ) {

      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

      gfxEnter3DMode();

      gfxEnterOverlayMode();

      glColor3f( .8, .8, .8 );
      offset = gfxDrawDbgTextFmt( 0.01, 0.02, 0.02, "GL Version: " );
      glColor3f( .2, 1.0, .2 );
      gfxDrawDbgTextFmt( offset, 0.02, 0.02, "%2.1f", GL_Version );
      glColor3f( .8, .8, .8 );
      offset = gfxDrawDbgTextFmt( 0.01, 0.0, 0.02,  "GL Error:   " );

      if ( glGetError() != GL_NO_ERROR ) {
         glColor3f( 1, 0, 0 );
      } else {
         glColor3f( .6, .6, 1 );
      }

      gfxDrawDbgTextFmt( offset, 0.0, 0.02, "%s", gluErrorString( glGetError() ) );

      glFinish(); 

      SDL_Delay( 50 );

      SDL_GL_SwapBuffers();

   }

   sleep( 1 );

   SDL_Quit();

   return 0;
}
