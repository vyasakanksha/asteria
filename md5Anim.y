
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
         "numJoints" "int" 
         "frameRate" "int" 
         "numAnimatedComponents" "int" 
         hierarchy bounds baseframe frame            
       ;       

hierarchy:
           "hierarchy" '{' hierarchySet '}' 
         ;  

hierarchySet:
              hierarchySet hierarchyValues
            | hierarchyValues 
            ;  
              
hierarchyValues:              
                 "string" "int" "int" "int" 
               ;
bounds:         
        "bounds" '{' boundsSet '}'
      ;

boundsSet: 
           boundsSet boundsValues
         | boundsValues 
         ;  
           
           
boundsValues:
              '('"real" "real" "real"')' '('"real" "real" "real"')'
            ;

baseframe:
           "baseframe" '{' baseframeSet '}' 
         ;

baseframeSet: baseframeSet baseframeValues
            | baseframeValues
            ;
              
baseframeValues:              
                 '(' "real" "real" "real" ')' '(' "real" "real" "real" ')'
               ;
         
frame:         
       frame frameSet
     | frameSet
     ;
       
frameSet: 
          "frame" "int" '{' frameValues '}' 
        ;     

frameValues: frameValues "real" 
           | /* epsilon */          
           ;
%%


/* md5Anim.y */
