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
      glUniform4fv( uniPos[i], 1, (GLfloat * )&rot );
   }

};
