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

/* libInclude.h */
#ifndef _LIB_INCLUDE_H_
#define _LIB_INCLUDE_H_

/* This file consolidates all of our big library includes into a single     *
 * header, allowing for quick and easy, bug-free includes. This is not a    *
 * trivial task. Additionally, this file allows cpp to avoid reading in     *
 * each and every include file when they have already been included, cpp    *
 * will instead see the #ifndef gaurds on this file, and skip over the      *
 * redundant includes.                                                      */

/* Prevent a collision between SDL and GLEW symbols. */
#define NO_SDL_GLEXT

/* OpenGL Extension Wrangler Library. */
#include <GL/glew.h>

/* OpenGL and GLU. */
#include <GL/gl.h>
#include <GL/glu.h>

/* Simple DirectMedia Layer, with support for OpenGL. */
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

#endif /* libInclude.h */
