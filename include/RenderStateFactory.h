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

#ifndef _ASTERIA_RENDER_STATE_FACTORY_H_
#define _ASTERIA_RENDER_STATE_FACTORY_H_

#include "RenderContext.h"
#include "RenderState.h"
#include "Md5RenderState.h"

namespace asteria {

   class RenderStateFactory {
      public:
         RenderStateFactory( RenderContext * rc );

         Md5RenderState * Md5FromShaderNames( const alt::String & vtx,
                                              const alt::String & frg );

      private:
         RenderContext * renderContext;

   };

};

#endif // RenderStateFactory.h
