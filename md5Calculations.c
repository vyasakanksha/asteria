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

/* Bison generated header. */
#include "md5Mesh.h"

extern int md5meshparse( md5MeshData * ); 
extern void md5meshset_in( FILE * );

md5BaseMesh * md5LoadMesh( FILE * fp ) {
   md5BaseMesh * ret = malloc( sizeof( md5BaseMesh ) );
   md5MeshData meshDat;

   vec3 * bindPoseVerts, * bindPoseNorms;

   md5meshset_in( fp );

   if ( 0 != md5meshparse( &meshDat ) ) {
      free( ret );
      fclose( fp );
      return NULL;
   }

   // TODO: Calculate bind-pose vertices.
   //     : Calculate bind-pose normals.
   //     : Transpose normals back into joint local space.
   //     : Generate  vertex arrays to be passed to the md5 shader.
}

md5SimpleBindPose * md5GetSimpleBindPose( const char * meshName ) {
   char meshFile[strlen( meshName ) + 9];
   FILE * md5;
   md5SimpleBindPose * out = malloc( sizeof( md5SimpleBindPose ) );
   md5MeshData         meshDat;

   int i;

   strcpy( meshFile, meshName );
   strcat( meshFile, ".md5mesh" );

   if ( NULL == ( md5 = fopen( meshFile, "r" ) ) ) {
      fprintf( stderr, "Could not open '%s': %s\n",
                       meshFile, strerror( errno ) );
      return NULL;
   }

   md5meshset_in( md5 );

   if ( 0 != md5meshparse( &meshDat ) ) {
      fprintf( stderr, "parsing '%s' failed: %s\n",
                        meshFile, strerror( errno ) );
   }

   /* FIXME: Only accounts for models with a single mesh. */
   out->numVerts = meshDat.meshes[0].numVerts;
   out->verts    = malloc( sizeof( vec3 ) * out->numVerts );
   out->norms    = malloc( sizeof( vec3 ) * out->numVerts );

   /* Pray for IEEE floating points. */
   memset( out->norms, 0, sizeof( vec3 ) * out->numVerts );

   /* FIXME: Only accounts for vertices with one weight. */
   for ( i = 0; i < out->numVerts; ++i ) {
      int wIdx  = meshDat.meshes[0].verts[i].firstWeight;
      md5Weight * weight = meshDat.meshes[0].weights + wIdx;
      md5Joint  * skel   = meshDat.joints + weight->joint;
      out->verts[i] = qtRotate( skel->orientation, weight->position );
      out->verts[i] += skel->position;
   }

   out->numIdx   = meshDat.meshes[0].numTris * 3;
   out->idxs     = malloc( sizeof( GLuint ) * out->numIdx );

   for ( i = 0; i < meshDat.meshes[0].numTris; i++ ) {
      vec3 A, B;

      out->idxs[( i * 3 ) + 0] = meshDat.meshes[0].tris[i].vtx1;
      out->idxs[( i * 3 ) + 1] = meshDat.meshes[0].tris[i].vtx2;
      out->idxs[( i * 3 ) + 2] = meshDat.meshes[0].tris[i].vtx3;

      A = out->verts[out->idxs[( i * 3 ) + 1]]
        - out->verts[out->idxs[( i * 3 ) + 0]];

      B = out->verts[out->idxs[( i * 3 ) + 2]]
        - out->verts[out->idxs[( i * 3 ) + 0]];

      out->norms[out->idxs[( i * 3 ) + 0]] += v3Cross( B, A );
      out->norms[out->idxs[( i * 3 ) + 1]] += v3Cross( B, A );
      out->norms[out->idxs[( i * 3 ) + 2]] += v3Cross( B, A );

   }

   for ( i = 0; i < out->numVerts; ++i ) {
      out->norms[i] = v3Normalize( out->norms[i] );
   }

   return out;
}
