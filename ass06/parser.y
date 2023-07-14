%{
/********************************\
|*                              *|
|*    Class: CS445              *|
|*    Prof:  Michael Wilder     *|
|*                              *|
|*    Proj:  06                 *|
|*    Date:  Nov 20, 2022       *|
|*    By:    Ethan Corgatelli   *|
|*                              *|
\********************************/

#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "scanType.h"
#include "node.h"

#define YYERROR_VERBOSE

extern int yydebug;

extern int yylex();
extern int yylineno;
extern Node * AST;

extern void yyerror(const char *);

%}

%debug

/* possible "terminal" values */
%union {
    class TokenData * tokenData; // for terminals. yylex()'s $ vars give token data
    class Node * node;           // for nonterminals.
}

/* terminals */
%token <tokenData> STATIC BOOL CHAR INT
%token <tokenData> IF THEN ELSE WHILE FOR DO TO BY RETURN BREAK
%token <tokenData> AND OR PLUS MINUS EQ NEQ LEQ GEQ LESS GRTR
%token <tokenData> ASS ADDASS SUBASS MULASS DIVASS DEC INC NOT ASTR SLASH PERC QUEST
%token <tokenData> PARENL PARENR BRACKL BRACKR BRACEL BRACER
%token <tokenData> SEMCOL COL COMMA 
%token <tokenData> BOOLCONST ID NUMCONST CHARCONST STRINGCONST

/* nonterminals */
%type <tokenData> typeSpec assignop relop sumop mulop unaryop constant
%type <node> program declList decl varDecl scopedVarDecl varDeclList varDeclInit varDeclId funDecl parms parmList parmTypeList parmIdList parmId
%type <node> stmt stmtClosed stmtOpen expStmt compoundStmt localDecls stmtList iterStmtClosed iterStmtOpen iterRange returnStmt breakStmt
%type <node> exp simpleExp andExp unaryRelExp relExp sumExp mulExp unaryExp factor mutable immutable call args argList

/* start symbol */
%start program

%%
program
    : declList                                      {                                                       }
    ;
declList
    : declList decl                                 { AST->add($2);                                         }
    | decl                                          { AST->add($1); if ($1) $1->isRoot = true;              }
    ;
decl
    : varDecl                                       { $$ = $1;                                              }
    | funDecl                                       { $$ = $1;                                              }
    | error                                         { $$ = NULL;                                            }
    ;
varDecl
    : typeSpec varDeclList SEMCOL                   { $$ = $2;
                                                      if ($2) {
                                                          ((VarDecl *)$$)->setType($1->tokenstr, false);
                                                          ((VarDecl *)$$)->initS();
                                                      }
                                                      yyerrok;                                              }
    | error varDeclList SEMCOL                      { $$ = NULL; yyerrok;                                   }
    | typeSpec error SEMCOL                         { $$ = NULL; yyerrok;                                   }
    ;
scopedVarDecl
    : STATIC typeSpec varDeclList SEMCOL            { $$ = $3;
                                                      if ($3) {
                                                          ((VarDecl *)$$)->setType($2->tokenstr, true);
                                                          ((VarDecl *)$$)->initS();
                                                      }
                                                      yyerrok;                                              }
    | typeSpec varDeclList SEMCOL                   { $$ = $2; 
                                                      if ($2) ((VarDecl *)$$)->setType($1->tokenstr, false);
                                                      yyerrok;                                              }
    | error varDeclList SEMCOL                      { $$ = NULL; yyerrok;                                   }
    ;
varDeclList
    : varDeclList COMMA varDeclInit                 { if($$) $$->add($3); yyerrok;                          }
    | varDeclList COMMA error                       { $$ = NULL;                                            }
    | error                                         { $$ = NULL;                                            }
    | varDeclInit                                   { $$ = $1;                                              }
    ;
varDeclInit
    : varDeclId                                     { $$ = $1;                                              }
    | varDeclId COL simpleExp                       { $$ = $1;
                                                      if ($1) { ((VarDecl *)$$)->isInited = true;
                                                                $$->addChild(0, $3);              }         }
    | error COL simpleExp                           { $$ = NULL; yyerrok;                                   }
    ;
varDeclId
    : ID                                            { $$ = new VarDecl($1);                                 }
    | ID BRACKL NUMCONST BRACKR                     { $$ = new VarDecl($1, $3);                             }
    | ID BRACKL error                               { $$ = NULL;                                            }
    ;
typeSpec                                              /* uses terminal type TokenData */
    : BOOL                                          { $$ = $1;                                              }
    | CHAR                                          { $$ = $1;                                              }
    | INT                                           { $$ = $1;                                              }
    ;
funDecl
    : typeSpec ID PARENL parms PARENR compoundStmt  { $$ = new FunDecl($1, $2, $4, $6);                     }
    | typeSpec error                                { $$ = NULL;                                            }
    | typeSpec ID PARENL error                      { $$ = NULL;                                            }
    | ID PARENL parms PARENR compoundStmt           { $$ = new FunDecl($1, $3, $5);                         }
    | ID PARENL error                               { $$ = NULL;                                            }
    | ID PARENL parms PARENR error                  { $$ = NULL;                                            }
    ;
parms
    : parmList                                      { $$ = $1;                                              }
    | %empty                                        { $$ = NULL;                                            }
    ; 
parmList
    : parmList SEMCOL parmTypeList                  { if ($$) $$->add($3);                                  }
    | parmList SEMCOL error                         { $$ = NULL;                                            }
    | parmTypeList                                  { $$ = $1;                                              }
    | error                                         { $$ = NULL;                                            }
    ;
parmTypeList
    : typeSpec parmIdList                           { $$ = $2;
                                                      if ($2) ((VarDecl *)$$)->setType($1->tokenstr,false); }
    | typeSpec error                                { $$ = NULL;                                            }
    ;
parmIdList
    : parmIdList COMMA parmId                       { if ($$) $$->add($3); yyerrok;                         }
    | parmIdList COMMA error                        { $$ = NULL;                                            }
    | parmId                                        { $$ = $1;                                              }
    | error                                         { $$ = NULL;                                            }
    ;
parmId
    : ID                                            { $$ = new Parm($1);                                    }
    | ID BRACKL BRACKR                              { $$ = new Parm($1, true);                              }
    | error BRACKR                                  { $$ = NULL; yyerrok;                                   }
    ;
stmt
    : stmtClosed                                    { $$ = $1;                                              }
    | stmtOpen                                      { $$ = $1;                                              }
    ;
stmtClosed
    : IF simpleExp THEN stmtClosed ELSE stmtClosed  { $$ = new IfStmt($1, $2, $4, $6);                      }
    | IF error                                      { $$ = NULL;                                            }
    | IF error ELSE stmtClosed                      { $$ = NULL; yyerrok;                                   }
    | IF error THEN stmtClosed ELSE stmtClosed      { $$ = NULL; yyerrok;                                   }
    | iterStmtClosed                                { $$ = $1;                                              }
    | expStmt                                       { $$ = $1;                                              }
    | compoundStmt                                  { $$ = $1;                                              }
    | returnStmt                                    { $$ = $1;                                              }
    | breakStmt                                     { $$ = $1;                                              }
    ;
stmtOpen
    : IF simpleExp THEN stmt                        { $$ = new IfStmt($1, $2, $4);                          }
    | IF error THEN stmt                            { $$ = NULL; yyerrok;                                   }
    | IF simpleExp THEN stmtClosed ELSE stmtOpen    { $$ = new IfStmt($1, $2, $4, $6);                      }
    | IF error THEN stmtClosed ELSE stmtOpen        { $$ = NULL; yyerrok;                                   }
    | iterStmtOpen                                  { $$ = $1;                                              }
    ;
expStmt
    : exp SEMCOL                                    { $$ = $1;   yyerrok;                                   }
    | error SEMCOL                                  { $$ = NULL; yyerrok;                                   }
    | SEMCOL                                        { $$ = NULL; yyerrok;                                   }
    ;
compoundStmt
    : BRACEL localDecls stmtList BRACER             { $$ = new CompoundStmt($1->linenum, $2, $3); yyerrok;  }
    | BRACEL error stmtList BRACER                  { $$ = NULL; yyerrok;                                   }
    | BRACEL localDecls error BRACER                { $$ = NULL; yyerrok;                                   }
    ;
localDecls
    : localDecls scopedVarDecl                      { $$->add($2);                                          }
    | %empty                                        { $$ = new Node();                                      }
    ;
stmtList
    : stmtList stmt                                 { $$->add($2);                                          }
    | %empty                                        { $$ = new Node();                                      }
    ;
iterStmtClosed
    : WHILE simpleExp DO stmtClosed                 { $$ = new WhileStmt($1, $2, $4);                       }
    | WHILE error DO stmtClosed                     { $$ = NULL; yyerrok;                                   }
    | WHILE error                                   { $$ = NULL;                                            }
    | FOR ID ASS iterRange DO stmtClosed            { $$ = new ForStmt($1, $2, $4, $6);                     }
    | FOR ID ASS error DO stmtClosed                { $$ = NULL; yyerrok;                                   }
    | FOR error                                     { $$ = NULL;                                            }
    ;
iterStmtOpen
    : WHILE simpleExp DO stmtOpen                   { $$ = new WhileStmt($1, $2, $4);                       }
    | FOR ID ASS iterRange DO stmtOpen              { $$ = new ForStmt($1, $2, $4, $6);                     }
    ;
iterRange
    : simpleExp TO simpleExp                        { $$ = new IterRange($1->linenum, $1, $3);              }
    | simpleExp TO error                            { $$ = NULL;                                            }
    | simpleExp TO simpleExp BY simpleExp           { $$ = new IterRange($1->linenum, $1, $3, $5);          }
    | error BY error                                { $$ = NULL; yyerrok;                                   }
    | simpleExp TO simpleExp BY error               { $$ = NULL;                                            }
    ;
returnStmt
    : RETURN SEMCOL                                 { $$ = new ReturnStmt($1->linenum);                     }
    | RETURN exp SEMCOL                             { $$ = new ReturnStmt($1->linenum, $2); yyerrok;        }
    | RETURN error SEMCOL                           { $$ = NULL; yyerrok;                                   }
    ;
breakStmt
    : BREAK SEMCOL                                  { $$ = new BreakStmt($1->linenum);                      }
    ;
exp
    : mutable assignop exp                          { $$ = new Assign($2, $1, $3);                          }
    | error assignop exp                            { $$ = NULL; yyerrok;                                   }
    | mutable assignop error                        { $$ = NULL;                                            }
    | mutable INC                                   { $$ = new Assign($2, $1);                              }
    | error INC                                     { $$ = NULL; yyerrok;                                   }
    | mutable DEC                                   { $$ = new Assign($2, $1);                              }
    | error DEC                                     { $$ = NULL; yyerrok;                                   }
    | simpleExp                                     { $$ = $1;                                              }
    ;
assignop
    :    ASS                                        { $$ = $1;                                              }
    | ADDASS                                        { $$ = $1;                                              }
    | SUBASS                                        { $$ = $1;                                              }
    | MULASS                                        { $$ = $1;                                              }
    | DIVASS                                        { $$ = $1;                                              }
    ;
simpleExp
    : simpleExp OR andExp                           { $$ = new Op($2, $1, $3);                              }
    | simpleExp OR error                            { $$ = NULL;                                            }
    | andExp                                        { $$ = $1;                                              }
    ;
andExp
    : andExp AND unaryRelExp                        { $$ = new Op($2, $1, $3);                              }
    | andExp AND error                              { $$ = NULL;                                            }
    | unaryRelExp                                   { $$ = $1;                                              }
    ;
unaryRelExp
    : NOT unaryRelExp                               { $$ = new Op($1, $2);                                  }
    | NOT error                                     { $$ = NULL;                                            }
    | relExp                                        { $$ = $1;                                              }
    ;
relExp
    : sumExp relop sumExp                           { $$ = new Op($2, $1, $3);                              }
    | sumExp relop error                            { $$ = NULL;                                            }
    | sumExp                                        { $$ = $1;                                              }
    ;
relop
    : LESS                                          { $$ = $1;                                              }
    | LEQ                                           { $$ = $1;                                              }
    | GRTR                                          { $$ = $1;                                              }
    | GEQ                                           { $$ = $1;                                              }
    | EQ                                            { $$ = $1;                                              }
    | NEQ                                           { $$ = $1;                                              }
    ;
sumExp
    : sumExp sumop mulExp                           { $$ = new Op($2, $1, $3);                              }
    | sumExp sumop error                            { $$ = NULL; yyerrok;                                   }
    | mulExp                                        { $$ = $1;                                              }
    ;
sumop
    : PLUS                                          { $$ = $1;                                              }
    | MINUS                                         { $$ = $1;                                              }
    ;
mulExp
    : mulExp mulop unaryExp                         { $$ = new Op($2, $1, $3);                              }
    | mulExp mulop error                            { $$ = NULL;                                            }
    | unaryExp                                      { $$ = $1;                                              }
mulop
    : ASTR                                          { $$ = $1;                                              }
    | SLASH                                         { $$ = $1;                                              }
    | PERC                                          { $$ = $1;                                              }
    ;
unaryExp
    : unaryop unaryExp                              { $$ = new Op($1, $2);                                  }
    | unaryop error                                 { $$ = NULL;                                            }
    | factor                                        { $$ = $1;                                              }
    ;
unaryop
    : MINUS                                         { $$ = $1;                                              }
    | ASTR                                          { $$ = $1;                                              }
    | QUEST                                         { $$ = $1;                                              }
    ;
factor
    : mutable                                       { $$ = $1;                                              }
    | immutable                                     { $$ = $1;                                              }
    ;
mutable
    : ID                                            { $$ = new Id($1);                                      }
    | ID BRACKL exp BRACKR                          { $$ = new Op($2, new Id($1), $3);                      }
    ;
immutable
    : PARENL exp PARENR                             { $$ = $2; yyerrok;                                     }
    | PARENL error                                  { $$ = NULL;                                            }
    | call                                          { $$ = $1;                                              }
    | constant                                      { $$ = new Const($1);                                   }
    ;
call
    : ID PARENL args PARENR                         { $$ = new Call($1, $3);                                }
    | error PARENL                                  { $$ = NULL; yyerrok;                                   }
    ;
args
    : argList                                       { $$ = $1;                                              }
    | %empty                                        { $$ = NULL;                                            }
    ;
argList
    : argList COMMA exp                             { if ($$) $$->add($3); yyerrok;                         }
    | argList COMMA error                           { $$ = NULL;                                            }
    | exp                                           { $$ = $1;                                              }
    ;
constant
    : NUMCONST                                      { $$ = $1;                                              }
    | CHARCONST                                     { $$ = $1;                                              }
    | STRINGCONST                                   { $$ = $1;                                              }
    | BOOLCONST                                     { $$ = $1;                                              }
    ;
%%
