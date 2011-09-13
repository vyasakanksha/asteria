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

/* vMath.h */
#ifndef _V_MATH_H_
#define _V_MATH_H_

#include "libInclude.h"

typedef GLfloat vec4 __attribute__(( vector_size ( sizeof( GLfloat ) * 4 ) ));
typedef GLfloat vec3 __attribute__(( vector_size ( sizeof( GLfloat ) * 4 ) ));
typedef GLfloat vec2 __attribute__(( vector_size ( sizeof( GLfloat ) * 4 ) ));


/* BE WARNED, THESE ARE WEIRD VECTOR TYPES. For example, writing to the .xyz *
 * field of a vec4_u can ( and will ) overwrite the .w field. If you are     *
 * using the unions ( i.e. anything with a '_u' at the end ), you MUST be    *
 * sure that you understand the GCC vector extensions and know what you are  *
 * doing.                                                                    */

typedef union vec2_u {

   struct {
      GLfloat x, y;
   };

   GLfloat arr[2];

   vec2 vec;

} vec2_u;

typedef union vec3_u {

   struct {
      GLfloat x, y, z;
   };

   GLfloat arr[3];

   vec3 vec;

   vec2 xy;

} vec3_u;

typedef union vec4_u {

   struct {
      GLfloat x, y, z, w;
   };

   GLfloat arr[4];

   vec4 vec;

   vec3 xyz;

   vec2 xy;

} vec4_u;


__attribute__ ((pure))
vec3 v3Cross( vec3 a, vec3 b );


__attribute__ ((pure))
GLfloat v2Dot( vec2 a, vec2 b );

__attribute__ ((pure))
GLfloat v3Dot( vec3 a, vec3 b );

__attribute__ ((pure))
GLfloat v4Dot( vec4 a, vec4 b );


__attribute__ ((pure))
static inline vec2 v2Scale( GLfloat s, vec2 v ) {
   return v * (vec2){ s, s, 1.0f, 1.0f };
}

__attribute__ ((pure))
static inline vec3 v3Scale( GLfloat s, vec3 v ) {
   return v * (vec3){ s, s, s, 1.0f };
}

__attribute__ ((pure))
static inline vec4 v4Scale( GLfloat s, vec4 v ) {
   return v * (vec4){ s, s, s, s };
}


__attribute__ ((pure))
vec2 v2Normalize( vec2 op );

__attribute__ ((pure))
vec3 v3Normalize( vec3 op );

__attribute__ ((pure))
vec4 v4Normalize( vec4 op );


__attribute__ ((pure))
vec3 qtRotate( vec4 q, vec3 v );

/* WARNING: Only works on unit quaternions. */
__attribute__ ((pure))
vec4 qtConjugate( vec4 op );

__attribute__ ((pure))
vec4 qtMkRot( GLfloat th, vec3 axis );

__attribute__ ((pure))
vec4 qtMul( vec4 a, vec4 b );

__attribute__ ((pure))
vec4 qtSlerp( vec4 a, vec4 b, GLfloat t );


/* output only valid up until next call. */
const char * v2Txt( vec2 v );

const char * v3Txt( vec2 v );

const char * v4Txt( vec2 v );

#endif /* vMath.h */
