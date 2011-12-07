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

   #include "altio.h"

   #include "md5Anim.h"

   #define YY_INPUT( buf, res, size )                                         \
   ( res ) = yyget_extra( yyscanner )->Read( ( buf ), ( size ) )
   #define YY_EXTRA_TYPE alt::Reader *
   #define YY_USER_ACTION {                                                   \
      yylloc->first_column = yylloc->last_column;                             \
      yylloc->last_column += strlen( yytext );                               \
   }


%}

%option reentrant noyywrap bison-bridge bison-locations noinput nounput

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
"\n"                       { yylloc->first_line++;                          }
"-"?[0-9]+"."[0-9]+        { yylval->rVal = strtod( yytext, NULL );
                                    return TOK_FNUM;                       }
"-"?[0-9]+                 { yylval->zVal = strtod( yytext, NULL );
                                     return TOK_ZNUM;                      }
\"[^\n\"]*\"               { yylval->sVal = strdup( yytext ); 
                                                return TOK_STRING;         }

[\r\t ]                    { ; }

%%

/* Prints the error and associated string, if it occurs */
void md5animerror( YYLTYPE * loc, void * , void * , const char * err )
{
   fprintf( stderr, "md5anim error: %s at line <%d> \n", err,
   loc->first_line );
}

/* md5Anim.lex */
