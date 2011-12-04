#include "Md5RenderState.h"

namespace asteria {

   Md5RenderState::Md5RenderState( GLuint vJ, GLuint vB, GLuint vP, GLuint vN,
         GLuint * uP, GLuint * uR,
         GLuint fS, GLuint vS, GLuint sP )
      : varJoints( vJ ), varBiases( vB ), varPositions( vP ), varNormals( vN ),
      vtxShader( vS ), frgShader( fS ), shaderProg( sP ) {

         // Copy the contents of the input arrays over into the local arrays.
         for ( int i = 0; i < MaxJoints; ++i ) {
            uniPos[i] = uP[i];
            uniRot[i] = uR[i];
         }

      }

   Md5RenderState::~Md5RenderState() {
      glDeleteShader( vtxShader );
      glDeleteShader( frgShader );
      glDeleteProgram( shaderProg );
   }

   bool Md5RenderState::EnterState( void ) {
      glUseProgram( shaderProg );

      glEnableVertexAttribArray( varJoints );
      glEnableVertexAttribArray( varBiases );
      glEnableVertexAttribArray( varPositions );
      glEnableVertexAttribArray( varNormals );

      return true;
   }

   bool Md5RenderState::ExitState( void ) {
      glUseProgram( 0 ); // Switch back to the fixed functionality.

      glDisableVertexAttribArray( varJoints );
      glDisableVertexAttribArray( varBiases );
      glDisableVertexAttribArray( varPositions );
      glDisableVertexAttribArray( varNormals );

      return true;
   }

   void Md5RenderState::SetJoint( int i, vec3 pos, vec4 rot ) {
      glUniform3fv( uniPos[i], 1, (GLfloat * )&pos );
      glUniform4fv( uniRot[i], 1, (GLfloat * )&rot );
   }

   bool Md5RenderState::StageBufferedMesh( md5BufferedMesh * bMesh ) {
      glBindBuffer( GL_ARRAY_BUFFER, bMesh->vBuf );
      glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, bMesh->iBuf );

      glVertexAttribPointer( varJoints, 4, GL_FLOAT, GL_FALSE,
                             sizeof( GLfloat ) * 4, bMesh->jIndex );
      glVertexAttribPointer( varBiases, 4, GL_FLOAT, GL_FALSE,
                             sizeof( GLfloat ) * 4, bMesh->biases );
      for ( int i = 0; i < 4; ++i ) {
         glVertexAttribPointer( varPositions + i, 4, GL_FLOAT, GL_FALSE,
                                sizeof( vec3 ), bMesh->positions[i] );
         glVertexAttribPointer( varNormals + i, 4, GL_FLOAT, GL_FALSE,
                                sizeof( vec4 ), bMesh->normals[i] );
      }
      return true;
   }

};
