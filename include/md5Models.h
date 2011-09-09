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

/* md5Models.h */
#ifndef _MD5_MODELS_H_
#define _MD5_MODELS_H_

#include "vMath.h"
#include "libInclude.h"


typedef struct md5Joint {
   char      * name;
   int         parent;
   vec4        position,
               orientation;
} md5Joint;


typedef struct md5Vert {
   GLfloat     texU,
               texV;
   int         firstWeight,
               countWeight;
} md5Vert;

typedef struct md5Tri {
   int         vtx1,
               vtx2,
               vtx3;
} md5Tri;

typedef struct md5Weight {
   int         joint;
   GLfloat     weight;
   vec4        position;
} md5Weight;

typedef struct md5Mesh {
   char      * shader;

   int         numVerts;
   md5Vert   * verts;

   int         numTris;
   md5Tri    * tris; 

   int         numWeights;
   md5Weight * weights;
} md5Mesh;

typedef struct md5MeshData {
   int         numJoints;
   md5Joint  * joints;

   int         numMeshes;
   md5Mesh   * meshes;
} md5MeshData;


typedef struct md5SimpleBindPose {
   int      numVerts;
   vec4   * verts;
   vec3   * norms;

   int      numIdx;
   GLuint * idxs;
} md5SimpleBindPose;

md5SimpleBindPose * md5GetSimpleBindPose( const char * meshName );

#endif /* md5Models.h */
