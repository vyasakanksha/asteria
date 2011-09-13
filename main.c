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
#include "gfxShader.h"

#include "vMath.h"

#include "md5Models.h"

int main( int argc, char * argv[] ) { 

   int i;
   long ticks;


   GLuint vtx, frg, prog;

   double GL_Version;

   FILE * md5File;
   md5BaseMesh * model;

   SDL_Init( SDL_INIT_EVERYTHING );

   SDL_Rect ** modes = SDL_ListModes( NULL, SDL_OPENGL );

   if ( modes == NULL ) {
      fprintf( stderr, "No video modes available...\n" );
      exit( 1 );
   } else if ( modes == (SDL_Rect **)-1 ) {
      fprintf( stderr, "Video modes unrestricted\n" );
      gfxSetupOsWindow( 640, 480 );
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

   if ( argc == 2 ) {
      if ( NULL == ( md5File = fopen( argv[1], "r" ) ) ) {
         fprintf( stderr, "Error loading '%s': %s\n",
                          argv[1], strerror( errno ) );
         exit( 1 );
      }
   } else {
      char * path;
      fprintf( stderr, "usage: %s [*.md5mesh]\n",
                       ( ( path = strrchr( argv[0], '/' ) )
                       ? path
                       : argv[0] ) );
      exit( 1 );
   }
   fprintf( stderr, "Loading model... " );
   ticks = SDL_GetTicks();
   if ( NULL == ( model = md5LoadMesh( md5File ) ) ) {
      fprintf( stderr, "Failed!\n" );
      SDL_Quit();
      exit( 1 );
   }
   fprintf( stderr, "%ldms elapsed.\n", SDL_GetTicks() - ticks );

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

   vtx  = gfxMakeShader( "shader/md5skel.vtx" );
   frg  = gfxMakeShader( "shader/test.frg" );
   prog = gfxMakeProgram( vtx, frg );
   
   glUseProgram( prog );

   //////////////////// MD5 SKELETON TEST CODE //////////////////////

   for ( i = 0; i < model->numJoints; ++i ) {
      char varName[128];
      GLint loc;
      vec4 tmp4;
      vec3 tmp3;
      snprintf( varName, 128, "jQuat[%d]", i );
      loc = glGetUniformLocation( prog, varName );
      fprintf( stderr, "jQuat[%d]: %d\n", i, loc );
      glUniform4fv( loc, 4, (GLfloat * )&( model->joints[i].orient ) );
      glGetUniformfv( prog, loc, (GLfloat *)&tmp4 );
      fprintf( stderr, "jQuat[%d] = %s, should be %s\n",
                       i,
                       v4Txt( tmp4 ),
                       v4Txt( model->joints[i].orient ) );
      snprintf( varName, 128, "jPos[%d]", i );
      if ( -1 == ( loc = glGetUniformLocation( prog, varName ) ) ) {
         fprintf( stderr, "glGetUniformLocation: %s\n", gluErrorString( glGetError() ) );
      }
      fprintf( stderr, "jPos[%d]: %d\n", i, loc );
      glUniform3fv( loc, 3, (GLfloat * )&( model->joints[i].position ) );
      glGetUniformfv( prog, loc, (GLfloat *)&tmp3 );
      fprintf( stderr, "jPos[%d] = %s, should be %s\n",
                       i,
                       v3Txt( tmp3 ),
                       v3Txt( model->joints[i].position ) );
   }

   GLint wJoints     = glGetAttribLocation( prog, "wJoints" ),
         wBiases     = glGetAttribLocation( prog, "wBiases" ),
         wPositions  = glGetAttribLocation( prog, "wPositions" ),
         wNormals    = glGetAttribLocation( prog, "wNormals" );

   glEnableVertexAttribArray( wJoints );
   glEnableVertexAttribArray( wBiases );
   glEnableVertexAttribArray( wPositions + 0 );
   glEnableVertexAttribArray( wPositions + 1 );
   glEnableVertexAttribArray( wPositions + 2 );
   glEnableVertexAttribArray( wPositions + 3 );
   glEnableVertexAttribArray( wNormals + 0 );
   glEnableVertexAttribArray( wNormals + 1 );
   glEnableVertexAttribArray( wNormals + 2 );
   glEnableVertexAttribArray( wNormals + 3 );

   glVertexAttribPointer( wJoints, 4, GL_FLOAT, GL_FALSE,
                          sizeof( GLfloat ) * 4, model->jIndex );
   glVertexAttribPointer( wBiases, 4, GL_FLOAT, GL_FALSE,
                          sizeof( GLfloat ) * 4, model->biases );
   for ( i = 0; i < 4; ++i ) {
      glVertexAttribPointer( wPositions + i, 4, GL_FLOAT, GL_FALSE,
                             sizeof( vec3 ), model->positions[i] );
      glVertexAttribPointer( wNormals + i, 4, GL_FLOAT, GL_FALSE,
                             sizeof( vec4 ), model->normals[i] );
   }

   fprintf( stderr, "wJoints: %d\n"
                    "wBiases: %d\n"
                    "wPositions: %d\n"
                    "wNormals: %d\n", wJoints, wBiases, wPositions, wNormals );

   for ( i = 0; i < model->numTris; ++i ) {
      int j;
      for ( j = 0; j < 3; ++j ) {
         //int idx = model->indices[i * 3 + j];
         /*
         fprintf( stderr, "jIdx: %s\nBias: %s\nPos[0]: %s\nPos[1]: %s\n"
                          "Pos[2]: %s\nPos[3]: %s\n %d\n\n",
                          v4Txt( *((vec4 *)&model->jIndex[idx] ) ),
                          v4Txt( *((vec4 *)&model->biases[idx] ) ),
                          v3Txt( model->positions[0][idx] ),
                          v3Txt( model->positions[1][idx] ),
                          v3Txt( model->positions[2][idx] ),
                          v3Txt( model->positions[3][idx] ),
                          idx );
         */
      }
   }

   //////////////////// END  MD5 SKELETON TEST ///////////////////////

   for ( i = 0; i < 200; ++i ) {
      glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

      glLoadIdentity();
      glTranslatef( 0.0f, 0.0f, -3.0f );
      glRotatef( 3.0f * i, 0.0f, 1.0f, 0.0f );

      glDrawElements( GL_TRIANGLES, model->numTris * 3,
                      GL_UNSIGNED_INT, model->indices );

      glFinish(); 

      SDL_Delay( 25 );

      SDL_GL_SwapBuffers();
   }

   sleep( 1 );

   SDL_Quit();

   return 0;
}
