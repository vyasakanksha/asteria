
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

   #include "altio.h"

   #include "md5Anim.h"

   #include "md5Structures.h"

   extern int md5animlex( YYSTYPE * , YYLTYPE * , void * );
   extern void md5animerror( YYLTYPE *, void *, void *, const char * );

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


%pure_parser
%locations

%error-verbose

%parse-param{void * scan}
%parse-param{md5AnimData * anim}
%lex-param{void * scan}

%union
{
   float  rVal;
   int    zVal;
   char * sVal;
}

%type <rVal> "real"
%type <zVal> "int"
%type <sVal> "string"

%%

md5Anim
   : "numFrames" "int"  {
         anim->numFrames = $2;
         anim->frames = new md5AnimFrame[$2];
     }
     "numJoints" "int"  { 
        int i;
        anim->numJoints = $5;
        anim->baseFrame = new md5Joint[$5];
        for( i = 0; i < anim->numFrames; i++) {
         anim->frames[i].joints = new md5Joint[$5];
        }
     }
     "frameRate" "int"  { 
        anim->frameRate = $8;
     }
     "numAnimatedComponents" "int" {
        jointFlags = new int[$11];
     }
     hierarchy bounds baseframe frame
   ;       

hierarchy
   : "hierarchy" {
      baseFrameIdx = 0;
   } '{' hierarchySet '}' 
   ;  

hierarchySet
   : hierarchySet hierarchyValues
   | hierarchyValues 
   ;  
              
hierarchyValues
   : "string" "int" "int" "int" {
      free( $1 ); // Don't need it!
      anim->baseFrame[baseFrameIdx].parent = $2; 
      jointFlags[baseFrameIdx] = $3;
      baseFrameIdx++;
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
   : "baseframe" {
        baseFrameIdx = 0;
   }
   '{' baseframeSet '}' 
   ;

baseframeSet
   : baseframeSet baseframeValues
   | baseframeValues
   ;
              
baseframeValues
   : '(' "real" "real" "real" ')' '(' "real" "real" "real" ')' {
      GLfloat sq;
      anim->baseFrame[baseFrameIdx] = (md5Joint){
         .parent   = anim->baseFrame[baseFrameIdx].parent,
         .position = (vec3)((vec4_u){ {
            .x = $2,
            .y = $3,
            .z = $4,
            .w = 0.0f
           }
        }).xyz,
        .orient = (vec4)((vec4_u){ {
           .x = $7,
           .y = $8,
           .z = $9,
           .w = ( ( sq = ( $7 * $7  +  $8 * $8  +  $9 * $9 ) ) < 1.0f 
                ? -sqrt( 1.0f - sq )
                : 0.0f )
          }
        }).vec
      };
      baseFrameIdx++;
   }
   ;
         
frame
   : frame frameSet
   | frameSet
   ;
       
frameSet
   : "frame" "int" {
      int i;
      jointIdx = 0; bit = 1, frameIdx = $2;
      for( i = 0; i < anim->numJoints; i++ ) {
         anim->frames[$2].joints[i].parent   = anim->baseFrame[i].parent;
         anim->frames[$2].joints[i].position = anim->baseFrame[i].position;
         anim->frames[$2].joints[i].orient   = anim->baseFrame[i].orient;
      }
   } '{' frameValues '}' {
      for ( int i = 0; i < anim->numJoints; i++ ) {
         GLfloat t;
         vec4_u v = { .vec = anim->frames[$2].joints[i].orient };

         t = v3Dot( v.xyz, v.xyz );
         v.w = ( t < 1.0f
               ? -sqrt( 1.0 - t )
               : 0.0f );

         anim->frames[$2].joints[i].orient = v.vec;

         v.vec = anim->frames[$2].joints[i].position;
         v.w = 0.0f;
         anim->frames[$2].joints[i].position = v.vec; 
      }
   }
   ;     

frameValues
   : frameValues "real" {
      while ( ( bit & jointFlags[jointIdx] ) == 0 ) {
         // assignJointBit( &anim->frames[frameIdx].joints[jointIdx], bit, 0.0f );
         bit = bit << 1;
         if( bit > 32 ) {
            bit = 1;
            jointIdx++;
         }
      }  
      assignJointBit( &anim->frames[frameIdx].joints[jointIdx], bit, $2 );
      bit = bit << 1;
      if( bit > 32 ) {
         bit = 1;
         jointIdx++;
      }
   }
   | /* epsilon */          
   ;

%%


/* md5Anim.y */
