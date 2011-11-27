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

/* md5Mesh.y */
%{
   #include <stdio.h>

   #include <math.h>

   #include "md5Structures.h"

   extern int md5meshlex( void );
   extern int md5mesherror( void * , const char * );

   static int jointIdx, meshIdx;
%}

%union {
   float  rVal;
   int    zVal;
   char * sVal;
}

%token            ENDOFFILE      0  "EOF"
%token            TOK_MD5VER        "MD5Version"
%token            TOK_CMDLINE       "commandline"
%token            TOK_NJOINT        "numJoints"
%token            TOK_NMESH         "numMeshes"
%token            TOK_JOINTS        "joints"
%token            TOK_MESH          "mesh"
%token            TOK_SHADER        "shader"
%token            TOK_NVERT         "numverts"
%token            TOK_VERT          "vert"
%token            TOK_NTRI          "numtris"
%token            TOK_TRI           "tri"
%token            TOK_NWEIGHT       "numweights"
%token            TOK_WEIGHT        "weight"
%token            TOK_RNUM          "float"
%token            TOK_ZNUM          "int"
%token            TOK_STRING        "string"

%error-verbose
%locations
%parse-param { md5MeshData * mdl }

%type <rVal> "float"
%type <zVal> "int" 
%type <sVal> "string"

%%

Md5File
   : "MD5Version" "int"
     "commandline" "string"
     "numJoints" "int"
     "numMeshes" "int" {
         mdl->numJoints = $6;
         mdl->joints    = malloc( sizeof( md5Joint ) * $6 );
         mdl->numMeshes = $8;
         mdl->meshes    = malloc( sizeof( md5Mesh )  * $8 );

         meshIdx = jointIdx = 0;

         /* Free extra memory given to the 'commandline' string. */
         free( $4 );
      }
     Joints
     MeshList
   ;

Joints
   : "joints" '{' JointSet '}'
   ;

JointSet
   : JointSet Joint
   | Joint 
   ;

Joint
   : "string" "int" '(' "float" "float" "float" ')'
                    '(' "float" "float" "float" ')' {
         GLfloat sq;
         free( $1 ); // Hoorray for unnecessary memory fragmentation!
         mdl->joints[jointIdx++] = (md5Joint){
            .parent     = $2,
            .position   = (vec3)((vec4_u){ { 
                  .x = $4,
                  .y = $5,
                  .z = $6,
                  .w = 0.0f
               }
            }).vec,
            .orient = (vec4)((vec4_u){ {
                  .x = $9,
                  .y = $10,
                  .z = $11,
                  .w = ( ( sq = ( $9 * $9 + $10 * $10 + $11 * $11 ) ) < 1.0f
                       ? -sqrt( 1.0f - sq )
                       : 0.0f ) /* If this is the case, something's weird. */
               }
            }).vec
         };
      }
   ;

MeshList
   : MeshList Mesh
   | Mesh
   ;

Mesh
   : "mesh" '{'
         "shader" "string"

         "numverts" "int" {
            mdl->meshes[meshIdx].numVerts   = $6;
            mdl->meshes[meshIdx].verts = malloc( sizeof( md5Vert ) * $6 );
         }
         VertSet

         "numtris" "int" {
            mdl->meshes[meshIdx].numTris    = $10;
            mdl->meshes[meshIdx].tris = malloc( sizeof( md5Tri ) * $10 );
         }
         TriSet

         "numweights" "int" {
            mdl->meshes[meshIdx].numWeights = $14;
            mdl->meshes[meshIdx].weights = malloc( sizeof( md5Weight ) * $14 );
         }
         WeightSet
     '}' {
         /* Increment the mesh index and set the shader name. */
         mdl->meshes[meshIdx++].shader = $4;
      }
   ;

VertSet
   : VertSet Vert
   | Vert
   ;

Vert
   : "vert" "int" '(' "float" "float" ')' "int" "int" {
      mdl->meshes[meshIdx].verts[$2] = (md5Vert){
         .texU = $4,
         .texV = $5,
         .firstWeight = $7,
         .countWeight = $8
      };
   }
   ;

TriSet
   : TriSet Tri
   | Tri
   ;

Tri
   : "tri" "int" "int" "int" "int" {
      mdl->meshes[meshIdx].tris[$2] = (md5Tri){
         .vtx1 = $3,
         .vtx2 = $4,
         .vtx3 = $5
      }
   }
   ;

WeightSet
   : WeightSet Weight
   | Weight
   ;

Weight
   : "weight" "int" "int" "float" '(' "float" "float" "float" ')' {
      mdl->meshes[meshIdx].weights[$2] = (md5Weight){
         .joint      = $3,
         .weight     = $4,
         .position   = (vec3)((vec4_u){ {
               .x = $6,
               .y = $7,
               .z = $8,
               .w = 0.0f
            }
         }).xyz
      }
   }

%%
