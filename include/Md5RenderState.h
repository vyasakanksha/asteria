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

#ifndef _ASTERIA_MD5_RENDER_STATE_H_
#define _ASTERIA_MD5_RENDER_STATE_H_

#include "RenderState.h"

namespace asteria {

   // This class knows how to enter and exit the render state for rendering Md5
   // Models.
   class Md5RenderState : public RenderState {
      public:
         virtual ~Md5RenderState() {}

         // Make these NOPs for now, just stand-ins.
         virtual bool EnterState( void ) { return true; }
         virtual bool ExitState( void ) { return true; }
   };

};

#endif // Md5RenderState.h

