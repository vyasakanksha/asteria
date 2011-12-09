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
      int lastFrame, nextFrame;
      float t = 0;

      // Allocate a temporary frame, use stack space to store joints
      vec3 positions[Md5RenderState::MaxJoints];
      vec4 rotations[Md5RenderState::MaxJoints];
      float msPerFrame;

      // FIXME: This assumes we are always drawing the same animation, etc, etc
      time = time % ( ( animData->numFrames * 1000 ) / animData->frameRate );

      lastFrame = ( animData->frameRate * time ) / 1000;
      nextFrame = lastFrame + 1;

      msPerFrame = 1000.0f / float( animData->frameRate );
      // 't' is in the range [0,1) and tells us how far we are between
      // lastFrame and nextFrame. Note that the variable 'lastFrame' is an
      // integral type, and hence the above division implies 'floor()'.
      t = ( float( time ) - msPerFrame * float( lastFrame ) ) / msPerFrame;

      for ( int i = 0; i < animData->numJoints; ++i ) {
         vec3 pos;
         vec4 rot;
         int parent;

         parent = animData->baseFrame[i].parent;
         if ( t != 0.0f && ( nextFrame < animData->numFrames ) ) {

            vec3 lastPos, nextPos;
            vec3 lastRot, nextRot;

            // Get the positions of the frames we are between
            lastPos = animData->frames[lastFrame].joints[i].position;
            nextPos = animData->frames[nextFrame].joints[i].position;

            // Interpolate between the positions with LERP
            pos = v3LERP( lastPos, nextPos, t );

            // Now get their rotations
            lastRot = animData->frames[lastFrame].joints[i].orient;
            nextRot = animData->frames[nextFrame].joints[i].orient;

            // Interpolate between the rotations with NLERP
            rot = qtNLERP( lastRot, nextRot, t );

         } else {

            // We're on the final frame, so we just grab position and
            // orientation. There's nothing to interpolate.
            pos = animData->frames[lastFrame].joints[i].position;
            rot = animData->frames[lastFrame].joints[i].orient;

         }

         if ( parent != -1 ) {
            // Rotate our position by the parent's orientation
            pos = qtRotate( rotations[parent], pos );

            // Add positions
            pos += positions[parent];

            // Concatenate Rotations
            rot = qtMul( rot, rotations[parent] );

         }

         positions[i] = pos;
         rotations[i] = v4Normalize( rot );

         renderState->SetJoint( i, pos, rot );

      }


      glDrawElements( GL_TRIANGLES, bufferedMesh->nIdx,
                      GL_UNSIGNED_INT, (GLvoid * )0 );

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
