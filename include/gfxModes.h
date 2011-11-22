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

// gfxModes.h
#ifndef _GFX_MODES_H_
#define _GFX_MODES_H_

#ifdef __cplusplus
extern "C" {
#endif

// These functions are designed to be called once each per frame. First we
// enter 3D Mode and draw the scene, then we enter Overlay Mode and draw
// anything that isn't a part of the 3D world.

void gfxEnter3DMode( void );

void gfxEnterOverlayMode( void );

#ifdef __cplusplus
}
#endif // C++

#endif // gfxModes.h
