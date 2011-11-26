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
#include "gfxTexture.h"
#include "gfxModes.h"
#include "gfxText.h"
#include "gfxConfig.h"
#include "gfxDebug.h"

#include "RenderContext.h"
#include "Md5RenderState.h"
#include "RenderStateFactory.h"

#include "md5Models.h"
#include "md5Anim.h"

#define DTR( N ) fprintf( stderr, "trace-point %d.\n", (N) )

using namespace asteria;

int md5animparse( md5AnimData * );

int main( int argc, char * argv[] ) { 

   int i;

   double GL_Version;

   FILE * meshFile;

   gfxLoadConfig();
   SDL_Init( SDL_INIT_EVERYTHING );

   RenderContext * renderContext;

   SDL_Rect ** modes = SDL_ListModes( NULL, SDL_OPENGL );

   DTR( 0 );

   if ( argc < 2 ) { 
      char * n;
      fprintf( stderr, "usage: %s [md5MeshFile]\n",
                       ( ( n = strrchr( argv[0], '/' ) )
                       ? n
                       : argv[0] ) );
      exit( 1 );
   } else if ( ( meshFile = fopen( argv[1], "r" ) ) == NULL ) {
      fprintf( stderr, "Could not open '%s': %s\n",
                       argv[1], strerror( errno ) );
      exit( 1 );
   }

   DTR( 1 );

   if ( modes == NULL ) {
      fprintf( stderr, "No video modes available...\n" );
      exit( 1 );
   } else if ( modes == (SDL_Rect **)-1 ) {
      fprintf( stderr, "Video modes unrestricted\n" );
      renderContext = new RenderContext( gfxConfig.xRes, gfxConfig.yRes, false );
   } else {
      renderContext = new RenderContext( (*modes)->w, (*modes)->h, false );
      for ( ; *modes; ++modes ) {
         fprintf( stderr, "width: %d, height: %d\n", (*modes)->w, (*modes)->h );
      }
   }

   DTR( 2 );

   renderContext->Initialize();

   DTR( 3 );

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


   DTR( 4 );

   if ( GLEW_OK != glewInit() ) {
      fprintf( stderr, "GLEW Failed to initialize.\n" );
      SDL_Quit();
      exit( 1 );
   }

   DTR( 5 );

   RenderStateFactory * rsFactory = new RenderStateFactory( renderContext );
   Md5RenderState * md5State = rsFactory->Md5FromShaderNames( "shader/md5skel.vtx",
                                                              "shader/test.frg" );


   renderContext->SetState( md5State );

   gfxInitBitMapFont();


   //md5BaseMesh     * base = md5LoadMesh( meshFile );
   //md5BufferedMesh * mesh = md5BufferMesh( base );

   SDL_Event event;

   int keepGoing = 1;

   for ( i = 0; keepGoing; ++i ) {
      //int j;

      gfxRegisterFrame();

      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
      glLoadIdentity();

      renderContext->SetPerspective( 45.0f, 0.01f, 100.0f );

#if 0
      // md5 Drawing Code.
      md5LoadState();

      for ( j = 0; j < base->numJoints; ++j ) {
         md5SetJoint( j, base->joints[j].position, base->joints[j].orient );
      }

      md5PrepareMesh( mesh );

      glTranslatef( 0.0f, 0.0f, -4.0f );

      glRotatef( 1.0f * i, 0.0f, 1.0f, 0.0f );

      md5DrawMesh();

      md5ExitState();

#endif
      // Overlay drawing code.
      renderContext->SetOrthographic();

      gfxDrawDbgHUD();

      glFinish(); 

      SDL_Delay( 20 );

      SDL_GL_SwapBuffers();

      while ( SDL_PollEvent( &event ) ) {
         switch( event.type ) {
            case SDL_KEYDOWN:
               if ( event.key.keysym.sym == SDLK_q ) {
                  keepGoing = 0;
               }
            default:
               break;
         }
      }

   }

   SDL_Quit();

   return 0;
}
