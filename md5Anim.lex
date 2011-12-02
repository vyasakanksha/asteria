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
#include "md5Anim.h"

static int lineno = 1;  /* To keep track of the line number for debugging */
void md5animerror( void *, const char * );
%}

%option noinput nounput

%%

"//".*                     { ; /* Ignore comments */                       }
MD5Version.*               { ; /* Ignore version */                        }
commandline.*              { ; /* Ignore ( internal Doom3 command ) */     }                        
numFrames                  { return TOK_NFRAMES;                           }
numJoints                  { return TOK_NJOINTS;                           }
frameRate                  { return TOK_FRATE;                             }
numAnimatedComponents      { return TOK_NANIM;                             }
hierarchy                  { return TOK_HIERARCHY;                         }  
bounds                     { return TOK_BOUNDS;                            }
baseframe                  { return TOK_BFRAME;                            }
frame                      { return TOK_FRAME;                             }
[(){}]                     { return yytext[0];                             }
"\n"                       { lineno++;                                     }
"-"?[0-9]+"."[0-9]+        { md5animlval.rVal = strtod( yytext, NULL );
                                    return TOK_FNUM;                       }
"-"?[0-9]+                 { md5animlval.zVal = strtod( yytext, NULL );
                                     return TOK_ZNUM;                      }
\"[^\n\"]*\"               { md5animlval.sVal = strdup( yytext ); 
                                                return TOK_STRING;         }

[\r\t ]                    { ; }
%%

/* I have no idea what this does, but without this function, everything breaks */
int md5animwrap( void )
{
   return 1;
}

/* Prints the error and associated string, if it occurs */
void md5animerror( void *, const char * err )
{
   fprintf( stderr, "md5anim error: %s at line <%d> \n", err, lineno );
}

/* md5Anim.lex */
