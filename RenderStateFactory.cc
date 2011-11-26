#include "altio.h"

#include "libInclude.h"
#include "RenderStateFactory.h"

using namespace alt;

namespace asteria {

   RenderStateFactory::RenderStateFactory( RenderContext * rc )
   : renderContext( rc ) {}


   // Make an Md5RenderState based on the names of the Md5 Shaders. This is a
   // factory method and not part of the constructor because there is really no
   // reason for Md5State to know how to do all of this junk. Maybe I'm being
   // too OOP, but the distinction seems logical to me.
   Md5RenderState *
   RenderStateFactory::Md5FromShaderNames( const String & vtx,
                                           const String & frg ) {

      GLuint varJoints, varBiases, varPositions, varNormals;
      GLuint uniPos[Md5RenderState::MaxJoints],
             uniRot[Md5RenderState::MaxJoints];
      GLuint shaderVtx, shaderFrag, shaderProg;

      GLint err;

      // FIXME: We don't want to be opening files here, we want to be using the
      // resource manager to look up resources. Make sure this gets fixed.
      File vtxF( vtx ), frgF( frg );

      // Build our shader program from the two shaders who's names we have.
      shaderVtx  = renderContext->MakeShader( vtx, vtxF );
      shaderFrag = renderContext->MakeShader( frg, frgF );

      // Link the shaders into a program.
      shaderProg = renderContext->MakeProgram( shaderVtx, shaderFrag );

      // Get the indices of each of the varying variables ( the vertex
      // attributes ).
      varJoints    = glGetAttribLocation( shaderProg, "wJoints" );
      varBiases    = glGetAttribLocation( shaderProg, "wBiases" );
      varPositions = glGetAttribLocation( shaderProg, "wPositions" );
      varNormals   = glGetAttribLocation( shaderProg, "wNormals" );

      // The OR of anything with '-1' is still '-1'. So we can test if any
      // of these things went wrong by ORing them all together.
      err = shaderVtx | shaderFrag   | shaderProg | varJoints
          | varBiases | varPositions | varNormals ;

      for ( int i = 0; i < Md5RenderState::MaxJoints; ++i ) {

         // Query the OpenGL 'names' of the uniform variables that hold
         // bone positions and orientations.
         String varName = SPrintf( "jPos[%d]", i );
         uniPos[i] = glGetUniformLocation( shaderProg, varName );
         varName = SPrintf( "jQuat[%d]", i );
         uniRot[i] = glGetUniformLocation( shaderProg, varName );

         // Same as the big OR above.
         err |= uniPos[i] | uniRot[i];
      }

      if ( err < 0 ) {
         FPrintf( Stderr, "Unable to load shader and identify variables." );
         return NULL;
      }

      return new Md5RenderState( varJoints, varBiases, varPositions, varNormals,
                                 uniPos, uniRot,
                                 shaderVtx, shaderFrag, shaderProg );

   }

};
