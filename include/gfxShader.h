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

/* gfxShader.h */
#ifndef _GFX_SHADER_H_
#define _GFX_SHADER_H_

#include "libInclude.h"

#ifdef __cplusplus
extern "C" {
#endif // C++

GLuint gfxMakeShader( const char * fName );

GLuint gfxMakeProgram( GLuint vtx, GLuint frg );

#ifdef __cplusplus
}
#endif // C++

#endif /* gfxShader.h */
