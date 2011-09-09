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

typedef union vec2 {

   struct {
      GLfloat x, y;
   };

   GLfloat vec[2];

} vec2;

typedef union vec3 {

   struct {
      GLfloat x, y, z;
   };

   GLfloat vec[3];

   vec2 xy;

} vec3;

typedef union vec4 {

   struct {
      GLfloat x, y, z, w;
   };

   GLfloat vec[4];

   vec3 xyz;

   vec2 xy;

} vec4;


vec3 * v3Cross( vec3 * rop, const vec3 * a, const vec3 * b );


GLfloat v2Dot( const vec2 * a, const vec2 * b );

GLfloat v3Dot( const vec3 * a, const vec3 * b );

GLfloat v4Dot( const vec4 * a, const vec4 * b );


vec2 * v2Scale( vec2 * rop, GLfloat s, const vec2 * v );

vec3 * v3Scale( vec3 * rop, GLfloat s, const vec3 * v );

vec4 * v4Scale( vec4 * rop, GLfloat s, const vec4 * v );


vec2 * v2Add( vec2 * rop, const vec2 * a, const vec2 * b );

vec3 * v3Add( vec3 * rop, const vec3 * a, const vec3 * b );

vec4 * v4Add( vec4 * rop, const vec4 * a, const vec4 * b );


vec2 * v2Sub( vec2 * rop, const vec2 * a, const vec2 * b );

vec3 * v3Sub( vec3 * rop, const vec3 * a, const vec3 * b );

vec4 * v4Sub( vec4 * rop, const vec4 * a, const vec4 * b );


vec2 * v2Normalize( vec2 * op );

vec3 * v3Normalize( vec3 * op );

vec4 * v4Normalize( vec4 * op );


/* Quaternion Multiplication. */
vec4 * qtMul( vec4 * rop, const vec4 * a, const vec4 * b );

/* WARNING: Only works on unit quaternions. */
vec4 * qtConjugate( vec4 * rop, const vec4 * op );

vec4 * qtMkRot( vec4 * rop, GLfloat th, const vec3 * axis );

#endif /* vMath.h */
