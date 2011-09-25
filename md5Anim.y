
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
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License    *
 * for more details.                                                        *
 *                                                                          *
 * You should have received a copy of the GNU General Public License along  *
 * with Asteria. If not, see <http://www.gnu.org/licenses/>.                *
 *                                                                          *
 ****************************************************************************/


/* md5Anim.y */

%{
   #include <stdio.h>

   extern int md5animlex( void );
   extern void md5animerror( const char * );

   extern int baseFrameIdx, framesidx;
   extern int * jointFlags, * jointStartIdx;
%}

%token         ENDOFFILE         0  "EOF"
%token         TOK_NFRAMES          "numFrames" 
%token         TOK_NJOINTS          "numJoints"
%token         TOK_FRATE            "frameRate"
%token         TOK_NANIM            "numAnimatedComponents"
%token         TOK_HIERARCHY        "hierarchy"
%token         TOK_BOUNDS           "bounds"
%token         TOK_BFRAME           "baseframe"
%token         TOK_FRAME            "frame"
%token         TOK_FNUM             "real"
%token         TOK_ZNUM             "int"
%token         TOK_STRING           "string"


%error-verbose
%parse-param{ md5AnimData * anim }

%union
{
   float rVal;
   int zVal;
   char * sVal;
}

%type <rVal> "real"
%type <zVal> "int"
%type <sVal> "string"

%%

md5Anim
   : "numFrames" "int"  {
         anim->numframes = $2;
     }
     "numJoints" "int"  { 
        anim->numJoints = $5;
     }
     "frameRate" "int"  { 
        anim->frameDur = 1000/$8;
     }
     "numAnimatedComponents" "int" {
        anim->frames = malloc( sizeof( md5Joint ) * $11 );
        jointFlags = malloc( sizeof( int ) * $11 );
        jointStartIdx = malloc( sizeof( int ) * $11 );
        framesIdx = 0;
        baseFrameIdx = 0;
     }
     hierarchy bounds 
     { 
        framesIdx = 0;
     }
     
     baseframe frame 

   ;       

hierarchy
   : "hierarchy" '{' hierarchySet '}' 
   ;  

hierarchySet
   : hierarchySet hierarchyValues
   | hierarchyValues 
   ;  
              
hierarchyValues
   : "string" "int" "int" "int" {
      free( $1 ) // Don't need it!
      anim->frames[framesIdx++].parent = $2; 
      jointFlags[frameIdx] = $3;
      jointStartIdx[frameIdx] = $4;
   }

   ;

bounds
   : "bounds" '{' boundsSet '}'
   ;

boundsSet
   : boundsSet boundsValues
   | boundsValues 
   ;          
           
boundsValues
   : '('"real" "real" "real"')' '('"real" "real" "real"')' {
      /* deleting unnessasary memory */
      free( $2 );
      free( $3 );
      free( $4 );
      free( $7 );
      free( $8 );
      free( $9 );
   }

   ;

baseframe
   : "baseframe" '{' baseframeSet '}' 
   ;

baseframeSet
   : baseframeSet baseframeValues
   | baseframeValues
   ;
              
baseframeValues
   : '(' "real" "real" "real" ')' '(' "real" "real" "real" ')' {
      anim->baseFrames[baseFrameIdx++] = (md5Joint){
         .position = (vec3)((vec4_u)){ {
            .x = $2,
            .y = $3,
            .z = $4,
            .w = 0.0f
           }
        }.vec,
        .orient = (vec4)((vect4_u)){ {
           .x = $7,
           .y = $8,
           .z = $9,
           .w = 0.0f
          }
        }.vec
      };
   }
   ;
         
frame
   : frame frameSet
   | frameSet
   ;
       
frameSet
   : "frame" "int" '{' frameValues '}' 
   ;     

frameValues
   : frameValues "real" 
   | /* epsilon */          
   ;

%%


/* md5Anim.y */
