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

// gfxConfig.h
#ifndef _GFX_CONFIG_H_
#define _GFX_CONFIG_H_

#include "libInclude.h"

#ifdef __cplusplus
extern "C" {
#endif // C++

// This is the structure that holds all graphics configuration information.
// Right now it is kind of sparse, in the future we might add things such as
// AA mode and renderer-specific settings ( LoD, etc ).
typedef struct gfxConfigDat {
   int xRes, yRes;  // Screen resolution
   int fullscreen;  // Self explanatory, should we be in fullscreen mode?
} gfxConfigDat;

extern const char * gfxConfigFile;

extern gfxConfigDat gfxConfig;

void gfxLoadConfig( void );

#ifdef __cplusplus
}
#endif // C++

#endif // gfxConfig.h
