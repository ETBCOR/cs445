%{
/********************************\
|*                              *|
|*    Class: CS445              *|
|*    Prof:  Michael Wilder     *|
|*                              *|
|*    Proj:  01                 *|
|*    By:    Ethan Corgatelli   *|
|*    Date:  Sep 9, 2022        *|
|*                              *|
\********************************/
#include "scanType.h"
#include <string>
#include <stdio.h>
#include <stdlib.h>

extern int yylex();
extern FILE *yyin;
extern int curline;
extern void yyerror(const char*);
%}

%union {
    struct TokenData *tokenData;
}

%token <tokenData> STATIC BOOL CHAR INT
%token <tokenData> IF THEN ELSE WHILE FOR DO TO BY RETURN BREAK
%token <tokenData> AND OR EQ NEQ LEQ GEQ ADDASS SUBASS MULASS DIVASS DEC INC NOT
%token <tokenData> BOOLCONST ID NUMCONST CHARCONST STRINGCONST
%token <tokenData> CATCH

%type <tokenData> statement

%%
statementlist : statementlist statement | statement

statement
    : STATIC        { printf("Line %d Token: STATIC\n", $$->linenum); }
    | BOOL          { printf("Line %d Token: BOOL\n", $$->linenum); }
    | CHAR          { printf("Line %d Token: CHAR\n", $$->linenum); }
    | INT           { printf("Line %d Token: INT\n", $$->linenum); }

    | IF            { printf("Line %d Token: IF\n", $$->linenum); }
    | THEN          { printf("Line %d Token: THEN\n", $$->linenum); }
    | ELSE          { printf("Line %d Token: ELSE\n", $$->linenum); }
    | WHILE         { printf("Line %d Token: WHILE\n", $$->linenum); }
    | FOR           { printf("Line %d Token: FOR\n", $$->linenum); }
    | DO            { printf("Line %d Token: DO\n", $$->linenum); }
    | TO            { printf("Line %d Token: TO\n", $$->linenum); }
    | BY            { printf("Line %d Token: BY\n", $$->linenum); }
    | RETURN        { printf("Line %d Token: RETURN\n", $$->linenum); }
    | BREAK         { printf("Line %d Token: BREAK\n", $$->linenum); }

    | AND           { printf("Line %d Token: AND\n", $$->linenum); }
    | OR            { printf("Line %d Token: OR\n", $$->linenum); }
    | EQ            { printf("Line %d Token: EQ\n", $$->linenum); }
    | NEQ           { printf("Line %d Token: NEQ\n", $$->linenum); }
    | LEQ           { printf("Line %d Token: LEQ\n", $$->linenum); }
    | GEQ           { printf("Line %d Token: GEQ\n", $$->linenum); }
    | ADDASS        { printf("Line %d Token: ADDASS\n", $$->linenum); }
    | SUBASS        { printf("Line %d Token: SUBASS\n", $$->linenum); }
    | MULASS        { printf("Line %d Token: MULASS\n", $$->linenum); }
    | DIVASS        { printf("Line %d Token: DIVASS\n", $$->linenum); }
    | DEC           { printf("Line %d Token: DEC\n", $$->linenum); }
    | INC           { printf("Line %d Token: INC\n", $$->linenum); }
    | NOT           { printf("Line %d Token: NOT\n", $$->linenum); }

    | BOOLCONST     { printf("Line %d Token: BOOLCONST Value: %d  Input: %s\n", $$->linenum, $$->nvalue, $$->tokenstr); }
    | ID            { printf("Line %d Token: ID Value: %s\n", $$->linenum, $$->svalue); }
    | NUMCONST      { printf("Line %d Token: NUMCONST Value: %d  Input: %s\n", $$->linenum, $$->nvalue, $$->tokenstr); }
    | CHARCONST     { printf("Line %d Token: CHARCONST Value: \'%c\'  Input: %s\n", $$->linenum, $$->cvalue, $$->tokenstr); }
    | STRINGCONST   { printf("Line %d Token: STRINGCONST Value: \"%s\"  Len: %ld  Input: %s\n", $$->linenum, $$->svalue, strlen($$->svalue), $$->tokenstr); }
    | CATCH         { printf("Line %d Token: %s\n", $$->linenum, $$->tokenstr); }
    ;
%%

extern int yydebug;
int main (int argc, char *argv[]) {

    // read the file
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

extern void yyerror(const char *msg) {
    printf("ERROR(%d): %s\n", curline, msg);
}
