%{
#include <cstdio>
#include <iostream>
using namespace std;
#define YY_DECL extern "C" int yylex()

#include "tecplot_loader.tab.h"  // to get the token types that we return

%}

%option caseless

%%
[ \t\n]           ;
TITLE { return TITLE; }
VARIABLES { return VARIABLES; }
ZONE { return ZONE; }
=            { return EQUALS; }
N { return N; }
E { return E; }
F { return F; }
ET { return ET; }
POINT { return POINT; }
FEPOINT { return FEPOINT; }
BLOCK { return BLOCK; }
FEBLOCK { return FEBLOCK; }
TRIANGLE { return TRIANGLE; }
BRICK { return BRICK; }
QUADRILATERAL { return QUADRILATERAL; }
TETRAHEDRON { return TETRAHEDRON; }
,	{ return COMA; }
\"(\\.|[^"])*\" {
    yylval.sval = (char*)calloc(strlen(yytext)-1, sizeof(char));
    strncpy(yylval.sval, &yytext[1], strlen(yytext)-2);
    return STRING;
}
[-+]?[0-9]*\.[0-9]+([eE][-+]?[0-9]+)? { yylval.fval = atof(yytext); return FLOAT; }
[-+]?[0-9]+ { yylval.ival = atof(yytext); return INTEGER; }
.                ;
%%
