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
%token <tokenData> AND OR PLUS MINUS EQ NEQ LEQ GEQ LESS GRTR ASS ADDASS SUBASS MULASS DIVASS DEC INC NOT ASTR SLASH PERC QUEST
%token <tokenData> PARENL PARENR BRACEL BRACER BRACKL BRACKR
%token <tokenData> SEMCOL COL COMMA 
%token <tokenData> BOOLCONST ID NUMCONST CHARCONST STRINGCONST

%%
program
    : declList
    ;

declList
    : declList decl | decl
    ;

decl
    : varDecl | funcDecl
    ;

varDecl
    : typeSpec varDeclList SEMCOL
    ;

scopedVarDecl
    : STATIC typeSpec varDeclList SEMCOL
    | typeSpec varDeclList SEMCOL
    ;

varDeclList
    : varDeclList COMMA varDeclInit
    | varDeclInit
    ;

varDeclInit
    : varDeclId
    | varDeclId COL simpleExp
    ;

varDeclId
    : ID
    | ID BRACEL NUMCONST BRACER
    ;

typeSpec
    : BOOL
    | CHAR
    | INT
    ;

funcDecl
    : typeSpec ID PARENL parms PARENR compoundStmt
    | ID PARENL parms PARENR compoundStmt
    ;

parms
    : parmList
    | /* empty */
    ;

parmList
    : parmList SEMCOL parmTypeList
    | parmTypeList
    ;

parmTypeList
    : typeSpec parmIdList
    ;

parmIdList
    : parmIdList COMMA parmId
    | parmId
    ;

parmId
    : ID
    | ID BRACEL BRACER
    ;

stmt
    : stmtClosed
    | stmtOpen
    ;

stmtClosed
    : IF simpleExp THEN stmtClosed ELSE stmtClosed
    | iterStmtClosed
    | expStmt
    | compoundStmt
    | returnStmt
    | breakStmt
    ;

stmtOpen
    : IF simpleExp THEN stmt
    | IF simpleExp THEN stmtClosed ELSE stmtOpen
    | iterStmtOpen
    ;

expStmt
    : exp SEMCOL
    | SEMCOL
    ;

compoundStmt
    : BRACKL localDecls stmtList BRACKR
    ;

localDecls
    : localDecls scopedVarDecl
    | /* empty */
    ;

stmtList
    : stmtList stmt
    | /* empty */
    ;

iterStmtClosed
    : WHILE simpleExp DO stmtClosed
    | FOR ID ASS iterRange DO stmtClosed
    ;

iterStmtOpen
    : WHILE simpleExp DO stmtOpen
    | FOR ID ASS iterRange DO stmtOpen
    ;

iterRange
    : simpleExp TO simpleExp
    | simpleExp TO simpleExp BY simpleExp
    ;

returnStmt
    : RETURN SEMCOL
    | RETURN exp SEMCOL
    ;

breakStmt
   : BREAK SEMCOL
   ;

exp
    : mutable assignop exp
    | mutable INC
    | mutable DEC
    | simpleExp
    ;

assignop
    : ASS
    | ADDASS
    | SUBASS
    | MULASS
    | DIVASS
    ;

simpleExp
    : simpleExp OR andExp
    | andExp
    ;

andExp
    : andExp AND unaryRelExp
    | unaryRelExp
    ;

unaryRelExp
    : NOT unaryRelExp
    | relExp
    ;

relExp
    : sumExp relop sumExp
    | sumExp
    ;

relop
    : LESS
    | LEQ
    | GRTR
    | GEQ
    | EQ
    | NEQ
    ;

sumExp
    : sumExp sumop mulExp
    | mulExp
    ;

sumop
    : PLUS
    | MINUS
    ;

mulExp
    : mulExp mulop unaryExp
    | unaryExp
    ;

mulop
    : ASTR
    | SLASH
    | PERC
    ;

unaryExp
    : unaryop unaryExp
    | factor
    ;

unaryop
    : MINUS
    | ASTR
    | QUEST
    ;

factor
    : mutable
    | immutable
    ;

mutable
    : ID
    | ID BRACEL exp BRACER
    ;

immutable
    : PARENL exp PARENR
    | call
    | constant
    ;

call
    : ID PARENL args PARENR
    ;

args
    : argList
    | /* empty */
    ;

argList
    : argList COMMA exp
    | exp
    ;

constant
    : NUMCONST
    | CHARCONST
    | STRINGCONST
    | BOOLCONST
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
