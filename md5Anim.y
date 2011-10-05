
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
   #include <math.h>
   #include "md5Models.h"

   extern int md5animlex( void );
   extern void md5animerror( void *, const char * );

   static int *jointFlags, baseFrameIdx, frameIdx, jointIdx, bit;

   static void assignJointBit( md5Joint * joint, int bit, float val )
   {
      vec3_u pos    = { .vec = joint->position };
      vec4_u orient = { .vec = joint->orient   };
      switch ( bit ) {
         case 1: 
            pos.x = val;
            break;
         case 2:
            pos.y = val;
            break;
         case 4:
            pos.z = val;
            break;
         case 8:
            orient.x = val;
            break;
         case 16:
            orient.y = val;
            break;
         case 32:
            orient.z = val;
            break;
      }
      joint->position = pos.vec;
      joint->orient   = orient.vec;
   }
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
         anim->numFrames = $2;
         anim->frames = malloc( sizeof( md5AnimFrame ) * $2 );
     }
     "numJoints" "int"  { 
        int i;
        anim->numJoints = $5;
        anim->baseFrame = malloc( sizeof( md5Joint ) * $5 );
        for( i = 0; i < anim->numFrames; i++) {
         anim->frames[i].joints = malloc( sizeof( md5Joint ) * $5 );
        }
     }
     "frameRate" "int"  { 
        anim->frameDur = 1000/$8;
     }
     "numAnimatedComponents" "int" {
        jointFlags = malloc( sizeof( int ) * $11 );
     }
     hierarchy bounds baseframe frame 

   ;       

hierarchy
   : "hierarchy" '{' hierarchySet '}' 
   ;  

hierarchySet
   : hierarchySet {
      frameIdx = 0;
      baseFrameIdx = 0;
   } hierarchyValues
   | hierarchyValues 
   ;  
              
hierarchyValues
   : "string" "int" "int" "int" {
      free( $1 ); // Don't need it!
      anim->baseFrame[baseFrameIdx++].parent = $2; 
      jointFlags[frameIdx++] = $3;
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
   : '('"real" "real" "real"')' '('"real" "real" "real"')' 
   ;

baseframe
   : "baseframe" '{' baseframeSet '}' 
   ;

baseframeSet
   : baseframeSet {
        baseFrameIdx = 0;
   } baseframeValues
   | baseframeValues
   ;
              
baseframeValues
   : '(' "real" "real" "real" ')' '(' "real" "real" "real" ')' {
      GLfloat sq;
      anim->baseFrame[baseFrameIdx++] = (md5Joint){
         .position = (vec3)((vec4_u){ {
            .x = $2,
            .y = $3,
            .z = $4,
            .w = 0.0f
           }
        }).vec,
        .orient = (vec4)((vec4_u){ {
           .x = $7,
           .y = $8,
           .z = $9,
           .w = (( sq = ( $7 * $7  +  $8 * $8  +  $9 * $9 )) < 1.0f 
                ? -sqrt( 1.0f - sq ) : 0.0f ) 
          }
        }).vec
      };
   }
   ;
         
frame
   : frame frameSet
   | frameSet
   ;
       
frameSet
   : "frame" "int" { 
      jointIdx = 0; bit = 1, frameIdx = $2; 
   } '{' frameValues '}' 
   ;     

frameValues
   : frameValues "real" {
      while (( bit & jointFlags[jointIdx] ) == 0 ) {
         if( bit > 32 ) {
            bit = 1;
            jointIdx++;
         }
         assignJointBit( &anim->frames[frameIdx].joints[jointIdx], bit, 0.0f );
         bit = bit << 1;
      }
      assignJointBit( &anim->frames[frameIdx].joints[jointIdx], bit, $2 );
   }
   | /* epsilon */          
   ;

%%


/* md5Anim.y */
