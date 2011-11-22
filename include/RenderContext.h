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

#include "RenderState.h"

namespace asteria {

   class RenderContext {
      public:
         RenderContext( int w, int h, bool fs )
         : currentState( NULL ), screenWidth( w ),
           screenHeight( h ), fullScreen( fs ) {
         }

         

         // Set the render state
         bool SetState( RenderState * rs );

         // Set the Screen Resolution
         bool SetResolution( int w, int h );

         // Decide whether or not we are in fullscreen mode.
         bool SetFullScreen( bool fullscreen );

         int  GetScreenWidth( void )  { return screenWidth; }
         int  GetScreenHeight( void ) { return screenHeight; }
         bool IsFullScreen( void )    { return fullScreen; }

      private:
         RenderState * currentState;
         int screenWidth, screenHeight;
         bool fullScreen;
   };

};

#endif // RenderContext.h
