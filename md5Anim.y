
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
/*%parse-param{ md5AnimData * mdl }*/

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

md5Anim:
         "numFrames" "int"
         "numjoints" "int"
         "frameRate" "int"
         "numAnimatedComponents" "int"
         hierarchy bounds baseframe frame
         { printf( "numFrames %d\n numJoints %d\n frameRate %d\n numAnimatedComponents %d \n", $2, $4, $6, $8 );}
       ;       

hierarchy:
           "hierarchy" '{' hierarchySet '}' { printf( "hiererchy { \n"); }
         ;  

hierarchySet:
              hierarchySet "string" "int" "int" "int" 
              { printf( "%s %d %d %d\n", $2, $3, $4, $5); }

            | "string" "int" "int" "int" 
              { printf( "%s %d %d %d\n", $1, $2, $3, $4); }
            ;

bounds:         
        "bounds" '{' boundsSet '}' { printf( "} \n\n bounds { \n"); }
      ;

boundsSet: 
           boundsSet '('"real" "real" "real"')' '('"real" "real" "real"')'
           { printf( "( %f %f %f ) ( %f %f %f )", $3, $4, $5, $8, $9, $10 ); }

         | '('"real" "real" "real"')' '('"real" "real" "real"')'
           { printf( "( %f %f %f ) ( %f %f %f )", $2, $3, $4, $7, $8, $9 ); }
         ;

baseframe:
           "baseframe" '{' baseframeSet '}' { printf( "} \n\n bounds { \n"); }
         ;  

baseframeSet: 
              baseframeSet '(' "real" "real" "real" ')' 
                           '(' "real" "real" "real" ')'
              { printf( "( %f %f %f ) ( %f %f %f )", $3, $4, $5, $8, $9, $10 ); }

            | '(' "real" "real" "real" ')' '(' "real" "real" "real" ')'
              { printf( "( %f %f %f ) ( %f %f %f )", $2, $3, $4, $7, $8, $9 ); }
            ;         
         
frame:         
       "frame" "int" '{' frameSet '}' { printf( "} \n\nframe %d\n", $2); }
     ;  

frameSet: frameSet '<'"real"'>' { printf( "<%f>", $3 ); }
        |           
        ;
%%


/* md5Anim.y */
