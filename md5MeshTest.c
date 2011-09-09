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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "md5Models.h"

extern int md5meshparse( md5MeshData * );
extern void md5meshset_in( FILE * );

int main( int argc, char * argv[] ) {
   char * n;
   FILE * md5;

   md5MeshData mesh;

   if ( 2 != argc ) {
      fprintf( stderr, "usage: %s [*.md5mesh]\n",
                       ( ( n = strrchr( argv[0], '/' ) ) ? n : argv[0] ) );
      exit( 1 );
   } else if ( NULL == ( md5 = fopen( argv[1], "r" ) ) ) {
      fprintf( stderr, "failed to open %s: %s\n", argv[1], strerror( errno ) );
      exit( 1 );
   }
   md5meshset_in( md5 );

   if ( 0 == md5meshparse( &mesh ) ) {
      fprintf( stderr, "md5Mesh: Successful Parse!\n" );
   } else {
      fprintf( stderr, "md5Mesh: Failed to Parse!\n" );
      exit( 1 );
   }

   return 0;
}
