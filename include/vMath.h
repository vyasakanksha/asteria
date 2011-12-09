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

#ifdef __cplusplus
extern "C" {
#endif // C++

typedef GLfloat vec4 __attribute__(( vector_size ( sizeof( GLfloat ) * 4 ) ));
typedef GLfloat vec3 __attribute__(( vector_size ( sizeof( GLfloat ) * 4 ) ));
typedef GLfloat vec2 __attribute__(( vector_size ( sizeof( GLfloat ) * 4 ) ));


/* BE WARNED, THESE ARE WEIRD VECTOR TYPES. For example, writing to the .xyz *
 * field of a vec4_u can ( and will ) overwrite the .w field. If you are     *
 * using the unions ( i.e. anything with a '_u' at the end ), you MUST be    *
 * sure that you understand the GCC vector extensions and know what you are  *
 * doing.                                                                    */

// Union that represents a 2D vector.
typedef union vec2_u {

   struct {
      GLfloat x, y;
   };

   GLfloat arr[2];

   vec2 vec;

} vec2_u;

// Union type for 3D vector. BE WARNED: This is just a 4D vector in disguise!
typedef union vec3_u {

   struct {
      GLfloat x, y, z;
   };

   GLfloat arr[3];

   vec3 vec;

   vec2 xy;

} vec3_u;

// Union type for 4D vector.
typedef union vec4_u {

   struct {
      GLfloat x, y, z, w;
   };

   GLfloat arr[4];

   vec4 vec;

   vec3 xyz;

   vec2 xy;

} vec4_u;


// Returns the cross product of two 3D vectors.
__attribute__ ((pure))
vec3 v3Cross( vec3 a, vec3 b );


// Returns the dot product of two 2D vectors.
__attribute__ ((pure))
GLfloat v2Dot( vec2 a, vec2 b );

// Returns the dot product of two 3D vectors.
__attribute__ ((pure))
GLfloat v3Dot( vec3 a, vec3 b );

// Returns the dot product of two 4D vectors.
__attribute__ ((pure))
GLfloat v4Dot( vec4 a, vec4 b );

// Returns the result of multiplying the scalar 's' by the vector 'v'.
__attribute__ ((pure))
static inline vec2 v2Scale( GLfloat s, vec2 v ) {
   return v * (vec2){ s, s, 1.0f, 1.0f };
}

// Returns the result of multiplying the scalar 's' by the vector 'v'.
__attribute__ ((pure))
static inline vec3 v3Scale( GLfloat s, vec3 v ) {
   return v * (vec3){ s, s, s, 1.0f };
}

// Returns the result of multiplying the scalar 's' by the vector 'v'.
__attribute__ ((pure))
static inline vec4 v4Scale( GLfloat s, vec4 v ) {
   return v * (vec4){ s, s, s, s };
}

// Returns a vector with magnitude 1, pointing in the same direction as 'op'.
__attribute__ ((pure))
vec2 v2Normalize( vec2 op );

// Returns a vector with magnitude 1, pointing in the same direction as 'op'.
__attribute__ ((pure))
vec3 v3Normalize( vec3 op );

__attribute__ ((pure))
vec3 v3LERP( vec3 a, vec3 b, GLfloat t );

// Returns a vector with magnitude 1, pointing in the same direction as 'op'.
__attribute__ ((pure))
vec4 v4Normalize( vec4 op );

// Returns 'v' rotated by the quaternion 'q'.
__attribute__ ((pure))
vec3 qtRotate( vec4 q, vec3 v );

// Returns the conjugate of the quaternion 'op'.
// WARNING: Only works on unit quaternions.
__attribute__ ((pure))
vec4 qtConjugate( vec4 op );

// Returns a quaternion that describes a rotation by 'th' degrees around the
// vector 'axis'
__attribute__ ((pure))
vec4 qtMkRot( GLfloat th, vec3 axis );

// 'a' and 'b' are quaternions. Returns the quaternion product ab.
__attribute__ ((pure))
vec4 qtMul( vec4 a, vec4 b );

// Returns a Spherical Linear intERPolation of the two quaternions.
__attribute__ ((pure))
vec4 qtSLERP( vec4 a, vec4 b, GLfloat t );

// Returns a Linear intERPolation of the two quaternions.
__attribute__ ((pure))
vec4 qtLERP( vec4 a, vec4 b, GLfloat t );

// Returns a Normalized Linear intERPolation of the two quaternions.
__attribute__ ((pure))
vec4 qtNLERP( vec4 a, vec4 b , GLfloat t );


// FIXME: These are HORRENDOUS memory leakers! They are just here for
// debugging and should _NEVER_ be used in real code.
const char * v2Txt( vec2 v );

const char * v3Txt( vec2 v );

const char * v4Txt( vec2 v );

#ifdef __cplusplus
}
#endif // C++

#endif /* vMath.h */
