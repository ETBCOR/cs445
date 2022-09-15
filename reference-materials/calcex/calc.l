%{
#include "y.tab.h"
#include "symtbl.h"
#include <math.h>
%}

%%
([0-9]+|([0-9]*\.[0-9]+)([eE][+-]?[0-9]+)?) {
    yylval.dval = atof(yytext);
    return NUMBER;
    }

[ \t] ; /* ignore whitespace */

[A-Za-z][A-Za-z0-9]* {
    /* return symbol pointer */
    yylval.symptr = sym_lookup(yytext);
    return NAME;
    }

"$" { return 0; }
\n |
. { return yytext[0]; };
%%
int yywrap() { return 1; }
