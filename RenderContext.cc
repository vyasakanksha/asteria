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

#include "altio.h"

#include "RenderContext.h"

using namespace alt;

namespace asteria {

   bool RenderContext::Initialize( RenderState * rs ) {
      if ( currentState == NULL ) {
         currentState = rs;
         rs->EnterState();
      }

      if ( !setupOsWindow( screenWidth, screenHeight ) ) {
         return false;
      }

      // Clear the screen to Black between frames.
      glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

      // Set Depth clear value to 1.0f
      glClearDepth( 1.0f );

      // Specify how to perform depth testing.
      glDepthFunc( GL_LEQUAL );

      return true;

   }

   bool RenderContext::resizeScreen( GLuint width, GLuint height ) {

      // Let OpenGL know we're working with a width by height context. */
      glViewport( 0, 0, (GLsizei)width, (GLsizei)height );

      return true;

   }

   bool RenderContext::setupOsWindow( GLuint width, GLuint height ) {
      screenSurface = SDL_SetVideoMode( width, height, 0, SDL_OPENGL 
                                                        | ( fullScreen
                                                          ? SDL_FULLSCREEN
                                                          : 0 ) );

      if ( !screenSurface ) {
         return false;
      } else {
         return resizeScreen( width, height );
      }
   }

   // FIXME: Need to find a better way to report error messages.
   GLuint RenderContext::MakeShader( const String & name, Reader & src ) {
      GLuint ret;
      const char * ext;
      int read;

      if ( NULL == ( ext = strrchr( name, '.' ) ) ) {
         FPrintf( Stderr, "Shader " + name + "' must have extension '.vtx'"
                          " or '.frg'\n" );
         return -1;
      }

      if ( 0 == strcmp( ext, ".vtx" ) ) {
         ret = glCreateShader( GL_VERTEX_SHADER );
      } else if ( 0 == strcmp( ext, ".frg" ) ) {
         ret = glCreateShader( GL_FRAGMENT_SHADER );
      } else {
         FPrintf( Stderr, "Shader '" + name + "' must have extension '.vtx'"
                          " or '.frg'\n" );
         return -1;
      }

      read = src.Read( (void * )shaderBuffer, shaderBufferSize );
      if ( shaderBufferSize == read ) {
         FPrintf( Stderr, "Shader '" + name + "' must be less than %d bytes"
                          " in size.\n", shaderBufferSize );
      }
      shaderBuffer[read] = '\0';

      glShaderSource( ret, 1, (const GLchar ** )&shaderBuffer, &read );
      glCompileShader( ret );

      glGetShaderiv( ret, GL_COMPILE_STATUS, &read );
      // Commented out the condition here because some of the errors might be
      // warnings.
      //if ( read != GL_TRUE ) {
         glGetShaderInfoLog( ret, shaderBufferSize, &read, shaderBuffer );
         shaderBuffer[read] = '\0';
         FPrintf( Stderr, shaderBuffer );
      //}

      return ret;
   }

   GLuint RenderContext::MakeProgram( GLuint vtx, GLuint frg ) {
      GLuint ret;
      GLint status;

      ret = glCreateProgram();

      glAttachShader( ret, vtx );
      glAttachShader( ret, frg );

      glLinkProgram( ret );

      glGetProgramiv( ret, GL_LINK_STATUS, &status );

      //if ( status != GL_TRUE ) {
         glGetShaderInfoLog( ret, shaderBufferSize, &status, shaderBuffer );
         shaderBuffer[status] = '\0';
         FPrintf( Stderr, "%s", shaderBuffer );
      //}

      return ret;

   }

};
