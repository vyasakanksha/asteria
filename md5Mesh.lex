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

/* md5Mesh.lex */
%{
   #include <stdlib.h>

   #include "md5Mesh.h"

   void md5mesherror( void * , const char * );
   static int lineno = 1;
%}

%option noinput nounput

%%

"MD5Version"            { return TOK_MD5VER;    }
"commandline"           { return TOK_CMDLINE;   }
"numJoints"             { return TOK_NJOINT;    }
"numMeshes"             { return TOK_NMESH;     }
"joints"                { return TOK_JOINTS;    }
"mesh"                  { return TOK_MESH;      }
"shader"                { return TOK_SHADER;    }
"numverts"              { return TOK_NVERT;     }
"vert"                  { return TOK_VERT;      }
"numtris"               { return TOK_NTRI;      }
"tri"                   { return TOK_TRI;       }
"numweights"            { return TOK_NWEIGHT;   }
"weight"                { return TOK_WEIGHT;    }

"-"?[0-9]+"."[0-9]+     {
      md5meshlval.rVal = strtof( yytext, NULL );
      return TOK_RNUM;
   }
"-"?[0-9]+                  {
      md5meshlval.zVal = strtol( yytext, NULL, 10 );
      return TOK_ZNUM;
   }
"\""[^\n\"]+"\""        {
      md5meshlval.sVal = strdup( yytext );
      return TOK_STRING;
   }

"//".*                  { ; }

[(){}]                  { return yytext[0]; }

[ \t]                   { ; }

"\n"                    { lineno++; }

%%

void md5mesherror( void * nil, const char * err ) {
   fprintf( stderr, "md5Mesh: %d error: %s\n",
                    lineno,
                    err );
}

int yywrap( void ) {
   return 1;
}
