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
   int         parent;
   vec3        position;
   vec4        orient;
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
   vec3        position;
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


// The md5BaseMesh structure defines everything needed to render the mesh
// with its bind-pose skeleton. All that needs to be changed to animate it
// is the position of the bones.
typedef struct md5BaseMesh {
   int numVerts; // The number of vertices making up the model.

   // Parens are to make these pointers to arrays of four elements each.
   GLfloat  (* jIndex)[4]; // The index of the joint associated with each of
                           // the weights associated with the given vertex.

   GLfloat  (* biases)[4]; // The bias of each weight. In order for correct
                           // behaviour, these should add up to one.

   // The parens here make them arrays of four pointers, instead of
   // pointers to arrays of four.
   vec3    * (positions[4]); // The offset of each of the ( up to ) four 
                             // weights associated with the given vertex.

   vec3    * (normals[4]);   // The normals of each of these weights in
                             // joint space.

   int numTris;      // The number of triangles in the mesh.

   GLuint * indices; // The indices of the vertices to be drawn, as per
                     // OpenGL vertex arrays.

   int numJoints;     // The number of joints in the skeleton.

   md5Joint * joints; // Data for each of the skeleton's joints.
} md5BaseMesh;

// If the mesh is stored in a buffer object, this special structure gives
// the names for the buffer objects ( vertex/index ), and the special pointers
// for use as vertex array offsets into the buffer
typedef struct md5BufferedMesh {
   GLuint vBuf, // OpenGL name for Vertex Buffer Object
          iBuf; // OpenGL name for Index Buffer Object

   GLvoid * jIndex,
          * biases,
          * positions[4],
          * normals[4];

   GLuint nIdx; // Number of indices
} md5BufferedMesh;

// Each frame is just an array of joints, each corresponding to a joint in the
// animated mesh.
typedef struct md5AnimFrame {
   md5Joint * joints;
} md5AnimFrame;

// This structure provides all of the information necessary to determine bone
// positions for a model at a given time.
typedef struct md5AnimData {
   float frameDur; // The number of milliseconds per frame.

   int numJoints;  // The number of joints in the model.

   md5Joint * baseFrame; // Joints in their base-frame position.

   int numFrames; // The number of frames in the model's animations.

   md5AnimFrame * frames; // A list of all frames.
} md5AnimData;

#endif /* md5Models.h */
