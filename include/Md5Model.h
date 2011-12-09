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

#ifndef _ASTERIA_MD5_MODEL_H_
#define _ASTERIA_MD5_MODEL_H_

#include "Model.h"
#include "Md5RenderState.h"
#include "md5Structures.h"

namespace asteria {

   class Md5Model : public Model {
      public:
         Md5Model( unsigned rID,
                   md5AnimData * aD,
                   md5BaseMesh * baseM,
                   md5BufferedMesh * buffM,
                   Md5RenderState * rS
                 );

         virtual ~Md5Model();

         // Prepare this model to be rendered.
         virtual bool StageModel( void );

         // Clean up after the staging process
         virtual bool UnstageModel( void );

         // Draw animation 'anim' at 'time' milliseconds.
         virtual bool DrawFrame( int anim, float time );

         // Draws the base mesh.
         virtual bool DrawStatic( void );

      private:
         // Every resource ( audio, texture, model, etc... ) gets a 'resID'
         // tag. This is a unique identifier assigned to it by the resource
         // manager.
         unsigned resID;

         // Per-frame skeletal animation data.
         md5AnimData * animData;

         // For now, we're not getting rid of the base mesh right away, in the
         // future we might decide to only hold on to the buffered version.
         md5BaseMesh * baseMesh;

         // This is the version of the mesh that has it's data in GPU memory
         // ( or wherever else OpenGL decides to put its buffer objects ).
         md5BufferedMesh * bufferedMesh;

         Md5RenderState * renderState;
   };

};

#endif // Md5Model.h
