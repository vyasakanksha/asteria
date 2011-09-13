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

/* md5Calculations.c */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "md5Models.h"

#include <unistd.h>

extern int md5meshparse( md5MeshData * ); 
extern void md5meshset_in( FILE * );

/* This is the real loader function for md5mesh files. */
md5BaseMesh * md5LoadMesh( FILE * fp ) {
   md5BaseMesh * ret = malloc( sizeof( md5BaseMesh ) );
   md5MeshData meshDat;

   vec3 * bindPoseVerts, // We need to calculate the bind-pose vertex
            // locations in order to calculate the bind-pose normals.

        * bindPoseNorms; // We need to calculate the bind-pose normals
            // in order to calculate the weight normals.

   int i, j, k,

       numVerts = 0, // The total number of vertices

       numTris  = 0, // The total number of triangles in the mesh.

       meshBase = 0, // The beginning index of the current mesh in the
                     // overall vertex array ( i.e., if there are multiple
                     // meshes to be stored in the same vertex arrays, we
                     // need to add an offset to the .md5mesh indices ).

       idxBase  = 0; // Like meshBase, but for ret->indices

   char * pRef;

   // Call the parser to parse the input stream
   md5meshset_in( fp );
   if ( 0 != md5meshparse( &meshDat ) ) {
      // Parse failed! Bail out!
      free( ret );
      fclose( fp );
      return NULL;
   }

   // Count the total number of vertices and triangles in all meshes
   for ( i = 0; i < meshDat.numMeshes; ++i ) {
      numVerts += meshDat.meshes[i].numVerts;
      numTris  += meshDat.meshes[i].numTris;
   }
   bindPoseVerts = calloc( numVerts, sizeof( vec3 ) );
   bindPoseNorms = calloc( numVerts, sizeof( vec3 ) );

   pRef = malloc( ( sizeof( GLfloat ) + sizeof( vec3 ) ) * 8 * numVerts );
   printf( "Allocated %zd bytes for mesh...",
           ( sizeof( GLfloat ) + sizeof( vec3 ) ) * 8 * numVerts + numTris * 3 * sizeof( GLuint ) );

   // Distribute memory pointed to by pRef among the different fields of the
   // return struct. We could be a bit less verbose with the pointer arithmetic
   // here, but for the sake of clarity, we'll leave it explicit.
   *ret = (md5BaseMesh){
      .numVerts  = numVerts,
      .jIndex    = (GLfloat (*)[4])pRef,
      .biases    = (GLfloat (*)[4])( pRef + ( sizeof( GLfloat ) * 4 * numVerts ) ),
      .positions = {
         [0] = (vec3 *)( pRef + ( sizeof( GLfloat ) * 8 * numVerts )
                              + ( sizeof( vec3 ) * 0 * numVerts ) ),
         [1] = (vec3 *)( pRef + ( sizeof( GLfloat ) * 8 * numVerts )
                              + ( sizeof( vec3 ) * 1 * numVerts ) ),
         [2] = (vec3 *)( pRef + ( sizeof( GLfloat ) * 8 * numVerts )
                              + ( sizeof( vec3 ) * 2 * numVerts ) ),
         [3] = (vec3 *)( pRef + ( sizeof( GLfloat ) * 8 * numVerts )
                              + ( sizeof( vec3 ) * 3 * numVerts ) ),
      },
      .normals = {
         [0] = (vec3 *)( pRef + ( sizeof( GLfloat ) * 8 * numVerts )
                              + ( sizeof( vec3 ) * 4 * numVerts ) ),
         [1] = (vec3 *)( pRef + ( sizeof( GLfloat ) * 8 * numVerts )
                              + ( sizeof( vec3 ) * 5 * numVerts ) ),
         [2] = (vec3 *)( pRef + ( sizeof( GLfloat ) * 8 * numVerts )
                              + ( sizeof( vec3 ) * 6 * numVerts ) ),
         [3] = (vec3 *)( pRef + ( sizeof( GLfloat ) * 8 * numVerts )
                              + ( sizeof( vec3 ) * 7 * numVerts ) ),
      },
      .numTris   = numTris,
      .indices   = malloc( sizeof( GLuint ) * numTris * 3 ),

      .numJoints = meshDat.numJoints,
      .joints    = meshDat.joints
   };

   // This is the big important loop. For every vertex we fill out all of the
   // information that we have available. We will calculate the normals after
   // each mesh has been built.
   for ( i = 0; i < meshDat.numMeshes; ++i ) {

      // Get a pointer to the mesh we're working with
      md5Mesh * mesh = meshDat.meshes + i;

      // Get a pointer to the specific vertices we want to alter ( and their
      // normals ).
      vec3 * curVerts = bindPoseVerts + meshBase;
      vec3 * curNorms = bindPoseNorms + meshBase;

      // Pointer to the specific indices we want to alter.
      GLuint * curIdx = ret->indices + idxBase;

      // Calculate necessary information for each vertex. We can't yet
      // calculate weight normals, but we can do everything else. We also
      // build a list of bind-pose vertices to calculate the normals with.
      for ( j = 0; j < mesh->numVerts; ++j ) {

         // Register the ( up to 4 ) weights of this vertex.
         for ( k = 0; k < mesh->verts[j].countWeight && k < 4; ++k ) {
            int wIdx = mesh->verts[j].firstWeight + k;
            ret->jIndex[meshBase + j][k]    = mesh->weights[wIdx].joint;
            ret->biases[meshBase + j][k]    = mesh->weights[wIdx].weight;
            ret->positions[k][meshBase + j] = mesh->weights[wIdx].position;
         }

         // Mark all other weights as unused and make sure their bias is 0
         for ( ; k < 4; ++k ) {
            // Make sure unused weights don't screw with the shader.
            ret->jIndex[meshBase + j][k] = -1.0f; // -1 indicates this weight
                                                  // is unused
            ret->biases[meshBase + j][k] =  0.0f; // a bias of 0 ensures that
                                   // garbage data doesn't affect the vertex
            ret->positions[k][meshBase + j] = (vec3){ 0.0f, 0.0f, 0.0f, 0.0f };
         }

         // The vertices were initialized to 0 by calloc, so we can just
         // start accumulating.
         for ( k = 0; k < mesh->verts[j].countWeight; ++k ) {
            // Calculate the position of this weight
            vec3 pos = qtRotate( meshDat.joints[(int)ret->jIndex[j][k]].orient,
                                 ret->positions[k][j] )
                     + meshDat.joints[(int)ret->jIndex[j][k]].position;

            // Add the biased vector to the vertex's location
            curVerts[j] += v3Scale( ret->biases[j][k], pos );
         }
      }

      // Calculate vertex normals for the current bind-pose mesh.
      for ( j = 0; j < mesh->numTris; ++j ) {
         vec3 A, B;
         A = curVerts[mesh->tris[j].vtx3] - curVerts[mesh->tris[j].vtx1];
         B = curVerts[mesh->tris[j].vtx2] - curVerts[mesh->tris[j].vtx1];

         // Add this face's normal vector to all other normal vectors for
         // each of the face's vertices.
         curNorms[mesh->tris[j].vtx1] += v3Cross( A, B );
         curNorms[mesh->tris[j].vtx2] += v3Cross( A, B );
         curNorms[mesh->tris[j].vtx3] += v3Cross( A, B );

         // Record the indices for accessing the vertex arrays
         curIdx[( j * 3 ) + 0] = meshBase + mesh->tris[j].vtx1;
         curIdx[( j * 3 ) + 1] = meshBase + mesh->tris[j].vtx2;
         curIdx[( j * 3 ) + 2] = meshBase + mesh->tris[j].vtx3;
      }

      idxBase  += mesh->numTris * 3;
      meshBase += mesh->numVerts;
   }

   // This is the final step, go through and rotate bind-pose normals into
   // joint-space normals for each weight.
   for ( i = 0; i < numVerts; ++i ) {
      vec3 norm = v3Normalize( bindPoseNorms[i] );
      for ( j = 0; j < 4 && ret->jIndex[i][j] != -1; ++j ) {
         // Find the quaternion to rotate the normal around.
         vec4 conj = qtConjugate( meshDat.joints[(int)ret->jIndex[i][j]].orient );
         // Rotate it!
         ret->normals[j][i] = qtRotate( conj, norm );
      }
   }

   return ret;
}


md5BufferedMesh * md5BufferMesh( md5BaseMesh * mesh ) {
   md5BufferedMesh * ret = malloc( sizeof( md5BufferedMesh ) );
   int i;

   glGenBuffers( 1, &( ret->vBuf ) );
   glGenBuffers( 1, &( ret->iBuf ) );

   ret->jIndex = (GLvoid * )0;
   ret->biases = (GLvoid * )( (char * )mesh->biases 
                            - (char * )mesh->jIndex );

   for ( i = 0; i < 4; ++i ) {
      ret->positions[i] = (GLvoid * )( (char * )mesh->positions[i] 
                                     - (char * )mesh->jIndex );
   }

   for ( i = 0; i < 4; ++i ) {
      ret->normals[i] = (GLvoid * )( (char * )mesh->normals[i]
                                   - (char * )mesh->jIndex );
   }

   glBindBuffer( GL_ARRAY_BUFFER, ret->vBuf );
   glBufferData(
         GL_ARRAY_BUFFER, 
         ( sizeof( GLfloat ) + sizeof( vec3 ) ) * 24 * mesh->numTris,
         mesh->jIndex,
         GL_STATIC_DRAW );

   glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ret->iBuf );
   glBufferData(
         GL_ELEMENT_ARRAY_BUFFER, 
         sizeof( GLuint ) * mesh->numTris * 3,
         mesh->indices,
         GL_STATIC_DRAW );

   return ret;
}
