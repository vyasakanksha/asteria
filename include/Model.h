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

#ifndef _ASTERIA_MODEL_H_
#define _ASTERIA_MODEL_H_

#include "altio.h"
#include "libInclude.h"

namespace asteria {

   class Model {
      public:
         virtual ~Model() {}

         // These functions set up and break-down OpenGL state required for the
         // rendering of this model. This is to prevent repeatedly calling code
         // when the same model is rendered multiple times.
         virtual bool StageModel( void ) = 0;
         virtual bool UnstageModel( void ) = 0;


         // The code that calls these functions is responsible for performing
         // any transformations that are necessary for the model to render
         // correctly.

         // 'anim' is the number of the animation sequence being rendered.
         // 'time' is the number of milliseconds since the animation started.
         virtual bool DrawFrame( int anim, int time ) = 0;

         virtual bool DrawStatic( void ) = 0;

      private:

   };

};

#endif // Model.h
