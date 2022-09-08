%{
#include "scanType.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

extern int yylex();
extern FILE *yyin;
extern int curline;
extern void yyerror(char*);
%}

%union {
    struct TokenData *tokenData;
}

%token <TokenData> ID
%token <TokenData> NUMCONST
%token <TokenData> CHARCONST
%token <TokenData> STRINGCONST
%token <TokenData> BOOLCONST

%%
statement
    : ID
    | NUMCONST
    | CHARCONST
    | STRINGCONST
    | BOOLCONST
    ;
%%
extern int yydebug;
int main (int argc, char *argv[]) {
    if (argc > 1) {
        if ((yyin = fopen(argv[1], "r"))) {
            // file opened successfully
        }
        else {
            // failed to open file
            printf("ERROR: failed to open \'%s\'\n", argv[1]);
            exit(1);
        }
    }

    // parse
    yyparse();

    return 0;
}
