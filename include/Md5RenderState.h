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

#include "vMath.h"

namespace asteria {

   // This class knows how to enter and exit the render state for rendering Md5
   // Models.
   class Md5RenderState : public RenderState {
      public:
         // Not sure this is how I want to do this, but it looks okay
         // for the moment.
         Md5RenderState( GLuint vJ, GLuint vB, GLuint vP, GLuint vN,
                         GLuint * uP, GLuint * uR,
                         GLuint fS, GLuint vS, GLuint sP );

         virtual ~Md5RenderState();

         // Make these NOPs for now, just stand-ins.
         virtual bool EnterState( void );
         virtual bool ExitState( void );

         // Set one of the joints to a given position and rotation.
         void SetJoint( int idx, vec3 pos, vec4 rot );

         // The maximum number of joints we are going to allow a single Md5
         // mesh to have. This limit exists because we have a maximum number of
         // allowed uniform variables. The OpenGL standard guaruntees us at
         // least enough space for 64, though.
         static const int MaxJoints = 64;

      private:
         // The GLSL 'varying variables', i.e. our vertex attributes.
         GLuint varJoints, varBiases, varPositions, varNormals;

         GLuint vtxShader, frgShader, shaderProg;

         // Uniform variables
         GLuint uniPos[MaxJoints], uniRot[MaxJoints];
   };

};

#endif // Md5RenderState.h
