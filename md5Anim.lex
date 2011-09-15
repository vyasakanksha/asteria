/* md5Anim.lex */

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

%{    
#include <stdlib.h>

int lineno = 1;  /* To keep track of the line number for debugging */
%}

%option noinput nounput

%%

"//".*                     { ; /* Ignore comments */                       }
MD5Version.*               { ; /* Ignore version */                        }
commandline.*              { ; /* Ignore ( internal Doom3 command ) */     }                        
numFrames                  { printf( "numFrames");
                                    /*return TOK_NFRAMES; */                          }
numJoints                  { printf( "numJoints" );
                                    /*return TOK_NJOINTS; */                          }
frameRate                  { printf( "frameRate");
                                    /*return TOK_FRATE;   */                          }
numAnimatedComponents      { printf( "numAnimatedComponenets");
                                    /*return TOK_NANIM;   */                          }
hierarchy                  { printf( "hierarchy" ); 
                                    /*return TOK_HIERARCHY; */                        }  
bounds                     { printf( "bounds" );
                                    /*return TOK_BOUNDS;    */                        }
baseFrame                  { printf( "baseFrame" );
                                    /*return TOK_BFRAME;    */                        }
frame                      { printf( "frame"     );
                                    /*return TOK_FRAME;     */                        }
[(){}]                     { printf( " %c ", yytext[0] );
                                    /*return yytext[0];     */                        }
"\n"                       { printf( "\n" ); lineno++;                               }
"-"?[0-9]+"."[0-9]+        { printf( " float " );  
                                    /*md5animlval.fval = strtod( yytext, NULL );
                                    return TOK_FNUM;  */                              }
"-"?[0-9]+                 { printf( " int " );
                                    /*md5animlval.zval = strtod( yytext, NULL );
                                     return TOK_ZNUM;  */                             }
\"[^\n\"]*\"               { printf( "string" ); 
                                    /*md5animlval.sval = strdup( yytext ); 
                                                return TOK_STRING; */                 }
%%

/* I have no idea what this does, but without this function, everything breaks */
int md5Animwrap( void )
{
   return 1;
}

/* Prints the error and associated string, if it occurs */
int md5animerror( void * nil, const * err )
{
   fprintf( stderr, "md5anim error: %s at line <%d> \n", err, lineno );
   return 0;
}

/* Temp function to test it the lexer works */
int main( int argc, char * argv[] )
{
   md5Animlex();
   return 0;
}

/* md5Anim.lex */
