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

md5SimpleBindPose * md5GetSimpleBindPose( const char * meshName ) {
   char meshFile[strlen( meshName ) + 9];
   FILE * md5;
   md5SimpleBindPose * out = malloc( sizeof( md5SimpleBindPose ) );
   md5MeshData         meshDat;

   vec4 tmp4;

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
   out->verts    = malloc( sizeof( vec4 ) * out->numVerts );
   out->norms    = malloc( sizeof( vec3 ) * out->numVerts );

   /* Pray for IEEE floating points. */
   memset( out->norms, 0, sizeof( vec3 ) * out->numVerts );

   /* FIXME: Only accounts for vertices with one weight. */
   for ( i = 0; i < out->numVerts; ++i ) {
      int wIdx  = meshDat.meshes[0].verts[i].firstWeight;
      md5Weight * weight = meshDat.meshes[0].weights + wIdx;
      md5Joint  * skel   = meshDat.joints + weight->joint;
      qtMul( out->verts + i, &( skel->orientation ), &( weight->position ) );
      qtConjugate( &tmp4, &( skel->orientation ) );
      qtMul( out->verts + i, out->verts + i, &tmp4 );
      v4Add( out->verts + i, out->verts + i, &( skel->position ) );
   }

   out->numIdx   = meshDat.meshes[0].numTris * 3;
   out->idxs     = malloc( sizeof( GLuint ) * out->numIdx );

   for ( i = 0; i < meshDat.meshes[0].numTris; i++ ) {
      vec3 A, B, tmp3;
      
      out->idxs[( i * 3 ) + 0] = meshDat.meshes[0].tris[i].vtx1;
      out->idxs[( i * 3 ) + 1] = meshDat.meshes[0].tris[i].vtx2;
      out->idxs[( i * 3 ) + 2] = meshDat.meshes[0].tris[i].vtx3;

      /* A and B are vectors in the direction of the edges of the triangle. */
      v3Sub( &A, &( out->verts[out->idxs[( i * 3 ) + 1]].xyz ),
                 &( out->verts[out->idxs[( i * 3 ) + 0]].xyz ) );

      v3Sub( &B, &( out->verts[out->idxs[( i * 3 ) + 2]].xyz ),
                 &( out->verts[out->idxs[( i * 3 ) + 0]].xyz ) );

      v3Add( out->norms + out->idxs[( i * 3 ) + 0],
             out->norms + out->idxs[( i * 3 ) + 0],
             v3Normalize( v3Cross( &tmp3, &B, &A ) ) );

      v3Add( out->norms + out->idxs[( i * 3 ) + 1],
             out->norms + out->idxs[( i * 3 ) + 1],
             &tmp3 );

      v3Add( out->norms + out->idxs[( i * 3 ) + 2],
             out->norms + out->idxs[( i * 3 ) + 2],
             &tmp3 );
   }

   for ( i = 0; i < out->numVerts; ++i ) {
      v3Normalize( out->norms + i );
   }

   return out;
}
