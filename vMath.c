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

/* vMath.c */

#include "libInclude.h"
#include "vMath.h"

#include <math.h>

vec3 v3Cross( vec3 a, vec3  b ) {

   vec3_u u = { .vec = a }, 
          v = { .vec = b },
          ret;

   ret.x = ( u.y * v.z ) - ( u.z * v.y );
   ret.y = ( u.z * v.x ) - ( u.x * v.z );
   ret.z = ( u.x * v.y ) - ( u.y * v.x );

   return ret.vec;
}

GLfloat v2Dot( vec2 a, vec2 b ) {
   vec2_u tmp2 = { .vec = a * b };
   return tmp2.x + tmp2.y;
}

GLfloat v3Dot( vec3 a, vec3 b ) {
   vec3_u tmp3 = { .vec = a * b };
   return tmp3.x + tmp3.y + tmp3.z;
}

GLfloat v4Dot( vec4 a, vec4 b ) {
   vec4_u tmp4 = { .vec = a * b };
   return tmp4.x + tmp4.y + tmp4.z + tmp4.w;
}

vec2 v2Normalize( vec2 op ) {
   return v2Scale( 1.0f / sqrt( v2Dot( op, op ) ), op );
}

vec3 v3Normalize( vec3 op ) {
   return v3Scale( 1.0f / sqrt(  v3Dot( op, op ) ), op );
}

vec4 v4Normalize( vec4 op ) {
   return v4Scale( 1.0f / sqrt( v4Dot( op, op ) ), op );
}

vec3 v3LERP( vec3 a, vec3 b, GLfloat t ) {
   vec3 x, y;
   
   x = v3Scale( 1.0f - t, a );
   y = v3Scale( t, b );

   return x + y;
}

vec3 qtRotate( vec4 q, vec3 v ) {
   vec4_u qTmp = { .vec = q };

   vec4_u vTmp = { .vec = v };
   vTmp.w = 0.0f;
   v = vTmp.vec;

   /* Dot product of 3D components, the scalar component of the intermediate *
    * quaternion.                                                            */
   GLfloat dot = -v3Dot( qTmp.xyz, v );

   /* 3D component of quaternion conjugate. */
   vec3 conj   = v3Scale( -1.0f, qTmp.xyz );

   /* Vector portion of the intermediate quaternion. */
   vec3 intVec = v3Scale( qTmp.w, v ) + v3Cross( qTmp.xyz, v );

   return v3Scale( qTmp.w, intVec ) + v3Scale( dot, conj ) 
                                    + v3Cross( intVec, conj );
}

vec4 qtConjugate( vec4 op ) {
   vec4_u q = { .vec = op };
   q.w = -q.w;
   return -q.vec;
}

vec4 qtMkRot( GLfloat th, vec3 axis ) {
   vec3_u ax = { .vec = axis };
   GLfloat s;
   
   th *= M_PI / 180.0f;

   s = sin( th / 2.0f );

   return (vec4){ ax.x * s, ax.y * s, ax.z * s, cos( th / 2.0f ) };
}

vec4 qtMul( vec4 a, vec4 b ) {
   vec4_u a_u = { .vec = a },
          b_u = { .vec = b },
          res;

   GLfloat s;

   s       = a_u.w * b_u.w - v3Dot( a_u.xyz, b_u.xyz );
   res.vec = v3Scale( b_u.w, a_u.xyz ) 
           + v3Scale( a_u.w, b_u.xyz )
           + v3Cross( a_u.xyz, b_u.xyz );
   res.w   = s;

   return res.vec;
}

vec4 qtLERP( vec4 a, vec4 b, GLfloat t ) {
   vec4 x, y;

   GLfloat magCos = v4Dot( a, b );
   if ( magCos < 0.0f ) {
      a = -a;
   }

   x = v4Scale( 1.0f - t, a );
   y = v4Scale( t, b );

   return x + y;
}

vec4 qtSLERP( vec4 a, vec4 b, GLfloat t ) {
// account for cases where sin( theta ) small
// Look into different implementations / optimizations

   int theta;
   vec4 x, y;

   theta = acos( v4Dot( a, b ));

   x = v4Scale( sin( ( 1 - t ) * theta ) / sin( theta ), a);
   y = v4Scale( sin( t * theta ) / sin( theta ), b);

   return x + y;
}

vec4 qtNLERP( vec4 a, vec4 b , GLfloat t ) {
   return v4Normalize( qtLERP( a, b, t ));
}

const char * v2Txt( vec2 v ) {
   char * buf = malloc( 512 );
   vec2_u u = { .vec = v };
   snprintf( buf, 512, "{ .x = %f, .y = %f }", u.x, u.y );
   return buf;
}

const char * v3Txt( vec3 v ) {
   char * buf = malloc( 512 );
   vec3_u u = { .vec = v };
   snprintf( buf, 512, "{ .x = %f, .y = %f, .z = %f }", u.x, u.y, u.z );
   return buf;
}

const char * v4Txt( vec4 v ) {
   char * buf = malloc( 512 );
   vec4_u u = { .vec = v };
   snprintf( buf, 512, "{ .x = %f, .y = %f, .z = %f, .w = %f }",
                       u.x, u.y, u.z, u.w );
   return buf;
}
