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

#ifndef _ASTERIA_RENDER_CONTEXT_H_
#define _ASTERIA_RENDER_CONTEXT_H_

#include "altio.h"

#include "libInclude.h"
#include "RenderState.h"
#include "Model.h"

namespace asteria {

   class RenderContext {
      public:
         RenderContext( int w, int h, bool fs );

         bool Initialize( void );

         bool SetPerspective( float angle, float near, float far );
         
         bool SetOrthographic( void );

         // Set the render state.
         bool SetState( RenderState * rs );

         // Set the current model.
         bool SetModel( Model * model );

         // Set the Screen Resolution
         bool SetResolution( int w, int h );

         // Decide whether or not we are in fullscreen mode.
         bool SetFullScreen( bool fullscreen );

         int  GetScreenWidth( void )  { return screenWidth; }
         int  GetScreenHeight( void ) { return screenHeight; }
         bool IsFullScreen( void )    { return fullScreen; }

         GLuint MakeShader( const alt::String & name, alt::Reader & rd );
         GLuint MakeProgram( GLuint vtx, GLuint frg );

      private:
         RenderState * currentState;
         Model * currentModel;
         int screenWidth, screenHeight;
         bool fullScreen;

         // We support shaders of up to 64k in size.
         static const size_t shaderBufferSize = 1 << 16;
         char shaderBuffer[shaderBufferSize];

         bool setupOsWindow( GLuint width, GLuint height );
         bool resizeScreen( GLuint width, GLuint height );

         SDL_Surface * screenSurface;

         // The whole point of the following two declarations is to provide
         // no-op instances of the state-transition classes, so that code
         // involving them doesn't need to be sullied with special cases.
         static class dummyModel : public Model {
            public:
               virtual bool StageModel( void )              { return true; }
               virtual bool UnstageModel( void )            { return true; }
               virtual bool DrawFrame( int anim, int time ) { return true; }
               virtual bool DrawStatic( void )              { return true; }
         } dModel;
         static class dummyState : public RenderState {
            public:
               virtual bool EnterState( void ) { return true; }
               virtual bool ExitState( void )  { return true; }
         } dState;

   };

};

#endif // RenderContext.h
