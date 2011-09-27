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

#include "gfxModes.h"
#include "gfxConfig.h"

// Call this before doing anything in the 3D world.
void gfxEnter3DMode( void ) {
   // Aspect ratio
   GLfloat ratio = (GLfloat)gfxConfig.xRes / (GLfloat)gfxConfig.yRes;

   // Prepare a perspective projection.
   glMatrixMode( GL_PROJECTION );
   glLoadIdentity();
   gluPerspective( 45.0f, ratio, 0.01f, 100.0f );

   glMatrixMode( GL_MODELVIEW );

   // Turn on depth testing.
   glEnable( GL_DEPTH_TEST );

}

// Call this before drawing any part of the overlay.
void gfxEnterOverlayMode( void ) {
   glMatrixMode( GL_PROJECTION );
   glLoadIdentity();

   gluOrtho2D( 0.0f, 1.0f, 1.0f, 0.0f );

   glMatrixMode( GL_MODELVIEW );
}
