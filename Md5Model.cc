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

#include "Md5Model.h"

namespace asteria {

   Md5Model::Md5Model( unsigned rID, md5AnimData * aD,
                       md5BaseMesh * baseM, md5BufferedMesh * buffM,
                       Md5RenderState * rS )
   : resID( rID ), animData( aD ), baseMesh( baseM ),
   bufferedMesh( buffM ), renderState( rS ) {}

   Md5Model::~Md5Model() {
      // TODO: Properly deallocate Md5Data.
   }

   bool Md5Model::StageModel( void ) {
      return renderState->StageBufferedMesh( bufferedMesh );
   }

   bool Md5Model::UnstageModel( void ) {
      // No resources need to be released at this point, so this method doesn't
      // need to actually do anything.
      return true;
   }

   bool Md5Model::DrawFrame( int anim, int time ) {
      // TODO: Implement this function! Akanksha: This is your thing =].
      return true;
   }

   bool Md5Model::DrawStatic( void ) {
      for ( int i = 0; i < baseMesh->numJoints; ++i ) {
         md5Joint * j = baseMesh->joints + i;
         renderState->SetJoint( i, j->position, j->orient );
      }

      glDrawElements( GL_TRIANGLES, bufferedMesh->nIdx,
                      GL_UNSIGNED_INT, (GLvoid * )0 );
      return true;
   }

};
