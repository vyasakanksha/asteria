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

#include <tiffio.h>
#include <stdio.h>
#include <stdint.h>

#include "libInclude.h"
#include "gfxTexture.h"

GLint gfxTextureFromTiff( const char * path ) {
   TIFF * image;

   int w, h;

   GLubyte * imgDat;

   if ( strcmp( strrchr( path, '.' ), ".tiff" ) != 0 ) {
      fprintf( stderr, "Texture file must be a .tiff\n" );
      return -1;
   }

   image = TIFFOpen( path, "r" );

   TIFFGetField( image, TIFFTAG_IMAGEWIDTH, &w );
   TIFFGetField( image, TIFFTAG_IMAGELENGTH, &h );

   imgDat = malloc( sizeof( uint32_t ) * w * h );

   TIFFReadRGBAImage( image, w, h, (uint32 *)imgDat, 0 );

   TIFFClose( image );

   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
   glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );

   return gluBuild2DMipmaps( GL_TEXTURE_2D, GL_RGBA, w, h, GL_RGBA,
                             GL_UNSIGNED_INT_8_8_8_8, imgDat );
}
