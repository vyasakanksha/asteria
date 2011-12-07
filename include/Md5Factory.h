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

#ifndef _ASTERIA_MD5_FACTORY_H_
#define _ASTERIA_MD5_FACTORY_H_

#include "altio.h"
#include "alt++/String.h"

#include "Md5Model.h"
#include "Md5RenderState.h"

namespace asteria {

   class Md5Factory {
      public:
         Md5Factory( Md5RenderState * rS );

         Md5Model * FromName( const alt::String & name );

      private:
         Md5RenderState * renderState;

         md5AnimData     * loadAnim( alt::Reader & r );
         md5BaseMesh     * loadMesh( alt::Reader & r );
         md5BufferedMesh * bufferMesh( md5BaseMesh * mesh );

   };

};

#endif // Md5ModelFactory.h
