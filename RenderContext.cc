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

#include "RenderContext.h"

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

};
