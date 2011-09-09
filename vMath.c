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

vec3 * v3Cross( vec3 * rop, const vec3 * a, const vec3 * b ) {
   GLfloat x, y, z;

   x = ( a->y * b->z ) - ( a->z * b->y );
   y = ( a->z * b->x ) - ( a->x * b->z );
   z = ( a->x * b->y ) - ( a->y * b->x );

   rop->x = x;
   rop->y = y;
   rop->z = z;

   return rop;
}


/* With reasonable optimization enabled, the nested calls will be inlined.   */
GLfloat v2Dot( const vec2 * a, const vec2 * b ) {
   return ( a->x * b->x ) + ( a->y * b->y );
}

GLfloat v3Dot( const vec3 * a, const vec3 * b ) {
   return v2Dot( &( a->xy ), &( b->xy ) ) + ( a->z * b->z );
}

GLfloat v4Dot( const vec4 * a, const vec4 * b ) {
   return v3Dot( &( a->xyz ), &( b->xyz ) ) + ( a->w * b->w );
}


vec2 * v2Scale( vec2 * rop, GLfloat s, const vec2 * v ) {
   GLfloat x, y;

   x = s * v->x;
   y = s * v->y;


   rop->x = x;
   rop->y = y;

   return rop;
}

vec3 * v3Scale( vec3 * rop, GLfloat s, const vec3 * v ) {
   GLfloat x, y, z;

   x = s * v->x;
   y = s * v->y;
   z = s * v->z;

   rop->x = x;
   rop->y = y;
   rop->z = z;

   return rop;
}

vec4 * v4Scale( vec4 * rop, GLfloat s, const vec4 * v ) {
   GLfloat x, y, z, w;

   x = s * v->x;
   y = s * v->y;
   z = s * v->z;
   w = s * v->w;

   rop->x = x;
   rop->y = y;
   rop->z = z;
   rop->w = w;

   return rop;
}


vec2 * v2Add( vec2 * rop, const vec2 * a, const vec2 * b ) {
   rop->x = a->x + b->x;
   rop->y = a->y + b->y;

   return rop;
}

vec3 * v3Add( vec3 * rop, const vec3 * a, const vec3 * b ) {
   rop->x = a->x + b->x;
   rop->y = a->y + b->y;
   rop->z = a->z + b->z;

   return rop;
}

vec4 * v4Add( vec4 * rop, const vec4 * a, const vec4 * b ) {
   rop->x = a->x + b->x;
   rop->y = a->y + b->y;
   rop->z = a->z + b->z;
   rop->w = a->w + b->w;

   return rop;
}

vec2 * v2Sub( vec2 * rop, const vec2 * a, const vec2 * b ) {
   rop->x = a->x - b->x;
   rop->y = a->y - b->y;

   return rop;
}

vec3 * v3Sub( vec3 * rop, const vec3 * a, const vec3 * b ) {
   rop->x = a->x - b->x;
   rop->y = a->y - b->y;
   rop->z = a->z - b->z;

   return rop;
}

vec4 * v4Sub( vec4 * rop, const vec4 * a, const vec4 * b ) {
   rop->x = a->x - b->x;
   rop->y = a->y - b->y;
   rop->z = a->z - b->z;
   rop->w = a->w - b->w;

   return rop;
}

vec2 * v2Normalize( vec2 * op ) {
   return v2Scale( op, 1.0f / sqrt( v2Dot( op, op ) ), op );
}

vec3 * v3Normalize( vec3 * op ) {
   return v3Scale( op, 1.0f / sqrt( v3Dot( op, op ) ), op );
}

vec4 * v4Normalize( vec4 * op ) {
   return v4Scale( op,  1.0f / sqrt( v4Dot( op, op ) ), op );
}


vec4 * qtMul( vec4 * rop, const vec4 * a, const vec4 * b ) {
   GLfloat w;
   vec3 v1, v2;

   w = a->w * b->w - v3Dot( &( a->xyz ), &( b->xyz ) );

   v3Scale( &v1, b->w, &( a->xyz ) );
   v3Scale( &v2, a->w, &( b->xyz ) );

   v3Add( &v1, &v1, &v2 );

   v3Cross( &v2, &( a->xyz ), &( b->xyz ) );

   v3Add( &( rop->xyz ), &v1, &v2 );
   rop->w = w;

   return rop;
}

vec4 * qtConjugate( vec4 * rop, const vec4 * op ) {
   rop->x = -op->x;
   rop->y = -op->y;
   rop->z = -op->z;
   rop->w =  op->w;

   return rop;
}

vec4 * qtMkRot( vec4 * rop, GLfloat th, const vec3 * axis ) {
   GLfloat x, y, z, w;

   x = sin( th / 2.0f ) * axis->x;
   y = sin( th / 2.0f ) * axis->y;
   z = sin( th / 2.0f ) * axis->z;
   w = cos( th / 2.0f );

   *rop = (vec4){ { x, y, z, w } };

   return rop;
}
