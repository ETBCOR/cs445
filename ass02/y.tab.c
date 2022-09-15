#ifndef lint
static const char yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93";
#endif

#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYPATCH 20130304

#define YYEMPTY        (-1)
#define yyclearin      (yychar = YYEMPTY)
#define yyerrok        (yyerrflag = 0)
#define YYRECOVERING() (yyerrflag != 0)

#define YYPREFIX "yy"

#define YYPURE 0

#line 2 "parser.y"
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
#line 24 "parser.y"
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union {
    struct TokenData *tokenData;
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
#line 51 "y.tab.c"

/* compatibility with bison */
#ifdef YYPARSE_PARAM
/* compatibility with FreeBSD */
# ifdef YYPARSE_PARAM_TYPE
#  define YYPARSE_DECL() yyparse(YYPARSE_PARAM_TYPE YYPARSE_PARAM)
# else
#  define YYPARSE_DECL() yyparse(void *YYPARSE_PARAM)
# endif
#else
# define YYPARSE_DECL() yyparse(void)
#endif

/* Parameters sent to lex. */
#ifdef YYLEX_PARAM
# define YYLEX_DECL() yylex(void *YYLEX_PARAM)
# define YYLEX yylex(YYLEX_PARAM)
#else
# define YYLEX_DECL() yylex(void)
# define YYLEX yylex()
#endif

/* Parameters sent to yyerror. */
#ifndef YYERROR_DECL
#define YYERROR_DECL() yyerror(const char *s)
#endif
#ifndef YYERROR_CALL
#define YYERROR_CALL(msg) yyerror(msg)
#endif

extern int YYPARSE_DECL();

#define STATIC 257
#define BOOL 258
#define CHAR 259
#define INT 260
#define IF 261
#define THEN 262
#define ELSE 263
#define WHILE 264
#define FOR 265
#define DO 266
#define TO 267
#define BY 268
#define RETURN 269
#define BREAK 270
#define AND 271
#define OR 272
#define PLUS 273
#define MINUS 274
#define EQ 275
#define NEQ 276
#define LEQ 277
#define GEQ 278
#define LESS 279
#define GRTR 280
#define ASS 281
#define ADDASS 282
#define SUBASS 283
#define MULASS 284
#define DIVASS 285
#define DEC 286
#define INC 287
#define NOT 288
#define ASTR 289
#define SLASH 290
#define PERC 291
#define QUEST 292
#define PARENL 293
#define PARENR 294
#define BRACEL 295
#define BRACER 296
#define BRACKL 297
#define BRACKR 298
#define SEMCOL 299
#define COL 300
#define COMMA 301
#define BOOLCONST 302
#define ID 303
#define NUMCONST 304
#define CHARCONST 305
#define STRINGCONST 306
#define YYERRCODE 256
static const short yylhs[] = {                           -1,
    0,    1,    1,    2,    2,    3,    7,    7,    6,    6,
    8,    8,    9,    9,    5,    5,    5,    4,    4,   11,
   11,   13,   13,   14,   15,   15,   16,   16,   17,   17,
   18,   18,   18,   18,   18,   18,   19,   19,   19,   21,
   21,   12,   26,   26,   27,   27,   20,   20,   24,   24,
   28,   28,   22,   22,   23,   25,   25,   25,   25,   30,
   30,   30,   30,   30,   10,   10,   31,   31,   32,   32,
   33,   33,   35,   35,   35,   35,   35,   35,   34,   34,
   36,   36,   37,   37,   38,   38,   38,   39,   39,   40,
   40,   40,   41,   41,   29,   29,   42,   42,   42,   43,
   45,   45,   46,   46,   44,   44,   44,   44,
};
static const short yylen[] = {                            2,
    1,    2,    1,    1,    1,    3,    4,    3,    3,    1,
    1,    3,    1,    4,    1,    1,    1,    6,    5,    1,
    0,    3,    1,    2,    3,    1,    1,    3,    1,    1,
    6,    1,    1,    1,    1,    1,    4,    6,    1,    2,
    1,    4,    2,    0,    2,    0,    4,    6,    4,    6,
    3,    5,    2,    3,    2,    3,    2,    2,    1,    1,
    1,    1,    1,    1,    3,    1,    3,    1,    2,    1,
    3,    1,    1,    1,    1,    1,    1,    1,    3,    1,
    1,    1,    3,    1,    1,    1,    1,    2,    1,    1,
    1,    1,    1,    1,    1,    4,    3,    1,    1,    4,
    1,    0,    3,    1,    1,    1,    1,    1,
};
static const short yydefred[] = {                         0,
   15,   16,   17,    0,    0,    0,    3,    4,    5,    0,
    0,    2,    0,    0,   10,    0,    0,    0,    0,   23,
    0,    0,    6,    0,    0,    0,    0,   26,    0,    0,
    0,    0,    0,    9,   90,    0,   91,   92,    0,  108,
    0,  105,  106,  107,    0,   93,    0,   68,   70,    0,
    0,   84,    0,   89,   94,   98,   99,    0,    0,   44,
   19,   22,    0,   14,   69,    0,    0,    0,    0,    0,
    0,    0,   81,   82,   77,   78,   74,   76,   73,   75,
    0,    0,   85,   86,   87,    0,   88,   28,   25,    0,
   18,   97,   60,   61,   62,   63,   64,   58,   57,    0,
  104,    0,    0,    0,    0,   67,    0,    0,   83,    0,
    0,   43,    0,   56,  100,    0,   96,    0,    0,    0,
    0,    0,    0,    0,   42,   41,   34,   45,   29,   30,
   32,   33,   35,   36,   39,    0,  103,    0,    8,    0,
    0,    0,   53,    0,   55,   40,    7,    0,    0,    0,
   54,   37,    0,   47,   49,    0,    0,    0,    0,    0,
   31,   38,    0,   48,   50,    0,    0,
};
static const short yydgoto[] = {                          5,
    6,    7,    8,    9,   17,   14,  112,   15,   16,   66,
   18,  127,   19,   20,   27,   28,  128,  129,  130,  131,
  132,  133,  134,  135,  136,   90,  113,  157,   46,  100,
   47,   48,   49,   50,   81,   82,   51,   86,   52,   53,
   54,   55,   56,   57,  102,  103,
};
static const short yysindex[] = {                      -254,
    0,    0,    0, -274,    0, -254,    0,    0,    0, -283,
 -177,    0, -193, -196,    0, -262, -264, -237, -229,    0,
 -177, -218,    0, -210,   85, -201, -204,    0, -199, -177,
 -181, -175, -178,    0,    0,   85,    0,    0,   85,    0,
 -179,    0,    0,    0, -145,    0, -139,    0,    0,  -10,
 -182,    0,  110,    0,    0,    0,    0, -159, -264,    0,
    0,    0, -199,    0,    0, -145, -168,   58,   85,   85,
   85,   85,    0,    0,    0,    0,    0,    0,    0,    0,
  110,  110,    0,    0,    0,  110,    0,    0,    0, -169,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   85,
    0, -156, -147, -141, -139,    0, -244, -182,    0, -177,
 -210,    0, -252,    0,    0,   85,    0, -210, -166,   85,
   85, -142,   77, -130,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, -123,    0, -151,    0, -251,
 -224,  -95,    0, -112,    0,    0,    0,  -99,  -99,   85,
    0,    0,  -75,    0,    0, -198,  -74,  -99,   85,  -99,
    0,    0, -213,    0,    0,   85, -145,
};
static const short yyrindex[] = {                         0,
    0,    0,    0,    0,    0,  191,    0,    0,    0,    0,
  -98,    0, -170,    0,    0, -117,    0,    0,  -97,    0,
  -98,    0,    0,    0,    0, -233, -215,    0,    0,    0,
    0,    0, -170,    0,    0,    0,    0,    0,    0,    0,
  -54,    0,    0,    0, -116,    0,   54,    0,    0, -195,
  -18,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, -236,    0,  146,  -93,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, -146,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  -92,    0,   62,    0,   37,   18,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, -125,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  -71,    0,    0,    0,  -67,
};
static const short yygindex[] = {                         0,
    0,  203,    0,    0,    2,  -83,    0,  186,    0,  -25,
  190,  -19,    0,  185,    0,  157,   86, -134, -133,    0,
    0,    0,    0,    0,  -36,    0,    0,    0,  -38,    0,
  167,  -29,    0,  158,    0,    0,  159,    0,  -30,    0,
    0,    0,    0,    0,    0,    0,
};
#define YYTABLESIZE 447
static const short yytable[] = {                         45,
   68,   10,   67,    1,    2,    3,   65,   10,  120,   61,
  148,  121,  122,  153,  154,  155,  123,  124,   11,   13,
   71,   35,   87,  161,  162,  164,  165,  119,   73,   74,
   68,   68,  101,  104,  138,   36,   37,   25,   26,   38,
   39,  149,  106,   91,   60,  125,  126,   71,    4,   40,
   41,   42,   43,   44,  166,  109,   29,   59,   71,   59,
   27,   68,   59,  114,   59,   27,   72,   27,  159,   30,
   72,   72,   72,   71,   68,   72,   72,   68,   24,  137,
    1,    2,    3,   24,   68,   32,  144,  110,    1,    2,
    3,  111,   33,   58,  140,  141,   59,   60,   72,   21,
   72,   22,   23,   72,   24,   72,   83,   84,   85,   68,
   68,  118,   63,   69,   46,   70,   22,   46,   46,   68,
   64,   68,   46,   46,  156,   92,   71,   46,   13,   13,
   13,   72,  139,  163,   24,   29,   88,  115,   29,   29,
  167,   46,   46,   29,   29,   46,   46,  147,   29,   24,
   46,   46,   46,  116,  117,   46,   46,   46,   46,   46,
  142,  120,   29,   29,  121,  122,   29,   29,  145,  123,
  124,   29,   29,   29,   35,  146,   29,   29,   29,   29,
   29,   11,   12,   11,   12,  150,  151,  158,   36,   37,
    1,  160,   38,   39,   51,   21,   20,   60,   52,  126,
  102,  101,   40,   41,   42,   43,   44,   95,   12,   34,
   31,   95,   95,   95,   62,   89,   95,   95,   95,   95,
   95,   95,   95,   95,   95,   95,   95,   95,   95,   95,
   95,   95,   95,  152,   95,   95,   95,  105,  107,   95,
  108,   95,    0,   80,   95,    0,   95,   80,   80,   80,
    0,    0,   80,   80,   80,   80,   80,   80,   80,   80,
   80,   80,   73,   74,   75,   76,   77,   78,   79,   80,
    0,    0,    0,    0,    0,   80,    0,   80,    0,   79,
   80,    0,   80,   79,   79,   79,    0,    0,   79,   79,
   79,   79,   79,   79,   79,   79,   79,   79,   71,    0,
    0,    0,   71,   71,   71,    0,    0,   71,   71,    0,
    0,   79,    0,   79,    0,   66,   79,    0,   79,   66,
   66,   66,    0,   65,    0,   66,    0,   65,   65,   65,
   71,    0,   71,   65,    0,   71,    0,   71,   93,   94,
   95,   96,   97,   98,   99,    0,    0,   66,    0,   66,
   35,    0,   66,    0,   66,   65,    0,   65,   35,    0,
   65,    0,   65,    0,   36,   37,    0,    0,   38,   39,
    0,    0,   36,   37,    0,  143,   38,   39,   40,   41,
   42,   43,   44,   35,    0,    0,   40,   41,   42,   43,
   44,    0,    0,    0,    0,    0,    0,    0,   37,    0,
    0,   38,   39,    0,    0,    0,    0,    0,    0,    0,
    0,   40,   41,   42,   43,   44,   93,   93,   93,   93,
   93,   93,   93,   93,   93,   93,    0,    0,    0,    0,
    0,    0,    0,    0,   93,   93,   93,    0,    0,   93,
    0,   93,    0,    0,   93,    0,   93,
};
static const short yycheck[] = {                         25,
   39,    0,   39,  258,  259,  260,   36,    6,  261,   29,
  262,  264,  265,  148,  149,  149,  269,  270,  293,  303,
  272,  274,   53,  158,  158,  160,  160,  111,  273,  274,
   69,   70,   69,   70,  118,  288,  289,  300,  303,  292,
  293,  266,   72,   63,  297,  298,  299,  272,  303,  302,
  303,  304,  305,  306,  268,   86,  294,  294,  272,  296,
  294,  100,  299,  100,  301,  299,  262,  301,  267,  299,
  266,  267,  268,  272,  113,  271,  272,  116,  294,  116,
  258,  259,  260,  299,  123,  304,  123,  257,  258,  259,
  260,   90,  303,  295,  120,  121,  301,  297,  294,  293,
  296,  295,  299,  299,  301,  301,  289,  290,  291,  148,
  149,  110,  294,  293,  261,  295,  295,  264,  265,  158,
  296,  160,  269,  270,  150,  294,  272,  274,  299,  300,
  301,  271,  299,  159,  301,  261,  296,  294,  264,  265,
  166,  288,  289,  269,  270,  292,  293,  299,  274,  301,
  297,  298,  299,  301,  296,  302,  303,  304,  305,  306,
  303,  261,  288,  289,  264,  265,  292,  293,  299,  269,
  270,  297,  298,  299,  274,  299,  302,  303,  304,  305,
  306,  299,  299,  301,  301,  281,  299,  263,  288,  289,
    0,  266,  292,  293,  266,  294,  294,  297,  266,  299,
  294,  294,  302,  303,  304,  305,  306,  262,    6,   24,
   21,  266,  267,  268,   30,   59,  271,  272,  273,  274,
  275,  276,  277,  278,  279,  280,  281,  282,  283,  284,
  285,  286,  287,  148,  289,  290,  291,   71,   81,  294,
   82,  296,   -1,  262,  299,   -1,  301,  266,  267,  268,
   -1,   -1,  271,  272,  273,  274,  275,  276,  277,  278,
  279,  280,  273,  274,  275,  276,  277,  278,  279,  280,
   -1,   -1,   -1,   -1,   -1,  294,   -1,  296,   -1,  262,
  299,   -1,  301,  266,  267,  268,   -1,   -1,  271,  272,
  273,  274,  275,  276,  277,  278,  279,  280,  262,   -1,
   -1,   -1,  266,  267,  268,   -1,   -1,  271,  272,   -1,
   -1,  294,   -1,  296,   -1,  262,  299,   -1,  301,  266,
  267,  268,   -1,  262,   -1,  272,   -1,  266,  267,  268,
  294,   -1,  296,  272,   -1,  299,   -1,  301,  281,  282,
  283,  284,  285,  286,  287,   -1,   -1,  294,   -1,  296,
  274,   -1,  299,   -1,  301,  294,   -1,  296,  274,   -1,
  299,   -1,  301,   -1,  288,  289,   -1,   -1,  292,  293,
   -1,   -1,  288,  289,   -1,  299,  292,  293,  302,  303,
  304,  305,  306,  274,   -1,   -1,  302,  303,  304,  305,
  306,   -1,   -1,   -1,   -1,   -1,   -1,   -1,  289,   -1,
   -1,  292,  293,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,  302,  303,  304,  305,  306,  271,  272,  273,  274,
  275,  276,  277,  278,  279,  280,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,  289,  290,  291,   -1,   -1,  294,
   -1,  296,   -1,   -1,  299,   -1,  301,
};
#define YYFINAL 5
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 306
#if YYDEBUG
static const char *yyname[] = {

"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"STATIC","BOOL","CHAR","INT","IF",
"THEN","ELSE","WHILE","FOR","DO","TO","BY","RETURN","BREAK","AND","OR","PLUS",
"MINUS","EQ","NEQ","LEQ","GEQ","LESS","GRTR","ASS","ADDASS","SUBASS","MULASS",
"DIVASS","DEC","INC","NOT","ASTR","SLASH","PERC","QUEST","PARENL","PARENR",
"BRACEL","BRACER","BRACKL","BRACKR","SEMCOL","COL","COMMA","BOOLCONST","ID",
"NUMCONST","CHARCONST","STRINGCONST",
};
static const char *yyrule[] = {
"$accept : program",
"program : declList",
"declList : declList decl",
"declList : decl",
"decl : varDecl",
"decl : funcDecl",
"varDecl : typeSpec varDeclList SEMCOL",
"scopedVarDecl : STATIC typeSpec varDeclList SEMCOL",
"scopedVarDecl : typeSpec varDeclList SEMCOL",
"varDeclList : varDeclList COMMA varDeclInit",
"varDeclList : varDeclInit",
"varDeclInit : varDeclId",
"varDeclInit : varDeclId COL simpleExp",
"varDeclId : ID",
"varDeclId : ID BRACEL NUMCONST BRACER",
"typeSpec : BOOL",
"typeSpec : CHAR",
"typeSpec : INT",
"funcDecl : typeSpec ID PARENL parms PARENR compoundStmt",
"funcDecl : ID PARENL parms PARENR compoundStmt",
"parms : parmList",
"parms :",
"parmList : parmList SEMCOL parmTypeList",
"parmList : parmTypeList",
"parmTypeList : typeSpec parmIdList",
"parmIdList : parmIdList COMMA parmId",
"parmIdList : parmId",
"parmId : ID",
"parmId : ID BRACEL BRACER",
"stmt : stmtClosed",
"stmt : stmtOpen",
"stmtClosed : IF simpleExp THEN stmtClosed ELSE stmtClosed",
"stmtClosed : iterStmtClosed",
"stmtClosed : expStmt",
"stmtClosed : compoundStmt",
"stmtClosed : returnStmt",
"stmtClosed : breakStmt",
"stmtOpen : IF simpleExp THEN stmt",
"stmtOpen : IF simpleExp THEN stmtClosed ELSE stmtOpen",
"stmtOpen : iterStmtOpen",
"expStmt : exp SEMCOL",
"expStmt : SEMCOL",
"compoundStmt : BRACKL localDecls stmtList BRACKR",
"localDecls : localDecls scopedVarDecl",
"localDecls :",
"stmtList : stmtList stmt",
"stmtList :",
"iterStmtClosed : WHILE simpleExp DO stmtClosed",
"iterStmtClosed : FOR ID ASS iterRange DO stmtClosed",
"iterStmtOpen : WHILE simpleExp DO stmtOpen",
"iterStmtOpen : FOR ID ASS iterRange DO stmtOpen",
"iterRange : simpleExp TO simpleExp",
"iterRange : simpleExp TO simpleExp BY simpleExp",
"returnStmt : RETURN SEMCOL",
"returnStmt : RETURN exp SEMCOL",
"breakStmt : BREAK SEMCOL",
"exp : mutable assignop exp",
"exp : mutable INC",
"exp : mutable DEC",
"exp : simpleExp",
"assignop : ASS",
"assignop : ADDASS",
"assignop : SUBASS",
"assignop : MULASS",
"assignop : DIVASS",
"simpleExp : simpleExp OR andExp",
"simpleExp : andExp",
"andExp : andExp AND unaryRelExp",
"andExp : unaryRelExp",
"unaryRelExp : NOT unaryRelExp",
"unaryRelExp : relExp",
"relExp : sumExp relop sumExp",
"relExp : sumExp",
"relop : LESS",
"relop : LEQ",
"relop : GRTR",
"relop : GEQ",
"relop : EQ",
"relop : NEQ",
"sumExp : sumExp sumop mulExp",
"sumExp : mulExp",
"sumop : PLUS",
"sumop : MINUS",
"mulExp : mulExp mulop unaryExp",
"mulExp : unaryExp",
"mulop : ASTR",
"mulop : SLASH",
"mulop : PERC",
"unaryExp : unaryop unaryExp",
"unaryExp : factor",
"unaryop : MINUS",
"unaryop : ASTR",
"unaryop : QUEST",
"factor : mutable",
"factor : immutable",
"mutable : ID",
"mutable : ID BRACEL exp BRACER",
"immutable : PARENL exp PARENR",
"immutable : call",
"immutable : constant",
"call : ID PARENL args PARENR",
"args : argList",
"args :",
"argList : argList COMMA exp",
"argList : exp",
"constant : NUMCONST",
"constant : CHARCONST",
"constant : STRINGCONST",
"constant : BOOLCONST",

};
#endif

int      yydebug;
int      yynerrs;

int      yyerrflag;
int      yychar;
YYSTYPE  yyval;
YYSTYPE  yylval;

/* define the initial stack-sizes */
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH  YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH  500
#endif
#endif

#define YYINITSTACKSIZE 500

typedef struct {
    unsigned stacksize;
    short    *s_base;
    short    *s_mark;
    short    *s_last;
    YYSTYPE  *l_base;
    YYSTYPE  *l_mark;
} YYSTACKDATA;
/* variables for the parser stack */
static YYSTACKDATA yystack;
#line 284 "parser.y"

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
#line 521 "y.tab.c"

#if YYDEBUG
#include <stdio.h>		/* needed for printf */
#endif

#include <stdlib.h>	/* needed for malloc, etc */
#include <string.h>	/* needed for memset */

/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(YYSTACKDATA *data)
{
    int i;
    unsigned newsize;
    short *newss;
    YYSTYPE *newvs;

    if ((newsize = data->stacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = (int) (data->s_mark - data->s_base);
    newss = (short *)realloc(data->s_base, newsize * sizeof(*newss));
    if (newss == 0)
        return -1;

    data->s_base = newss;
    data->s_mark = newss + i;

    newvs = (YYSTYPE *)realloc(data->l_base, newsize * sizeof(*newvs));
    if (newvs == 0)
        return -1;

    data->l_base = newvs;
    data->l_mark = newvs + i;

    data->stacksize = newsize;
    data->s_last = data->s_base + newsize - 1;
    return 0;
}

#if YYPURE || defined(YY_NO_LEAKS)
static void yyfreestack(YYSTACKDATA *data)
{
    free(data->s_base);
    free(data->l_base);
    memset(data, 0, sizeof(*data));
}
#else
#define yyfreestack(data) /* nothing */
#endif

#define YYABORT  goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR  goto yyerrlab

int
YYPARSE_DECL()
{
    int yym, yyn, yystate;
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")) != 0)
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = YYEMPTY;
    yystate = 0;

#if YYPURE
    memset(&yystack, 0, sizeof(yystack));
#endif

    if (yystack.s_base == NULL && yygrowstack(&yystack)) goto yyoverflow;
    yystack.s_mark = yystack.s_base;
    yystack.l_mark = yystack.l_base;
    yystate = 0;
    *yystack.s_mark = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = YYLEX) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
        {
            goto yyoverflow;
        }
        yystate = yytable[yyn];
        *++yystack.s_mark = yytable[yyn];
        *++yystack.l_mark = yylval;
        yychar = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;

    yyerror("syntax error");

    goto yyerrlab;

yyerrlab:
    ++yynerrs;

yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yystack.s_mark]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yystack.s_mark, yytable[yyn]);
#endif
                if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
                {
                    goto yyoverflow;
                }
                yystate = yytable[yyn];
                *++yystack.s_mark = yytable[yyn];
                *++yystack.l_mark = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yystack.s_mark);
#endif
                if (yystack.s_mark <= yystack.s_base) goto yyabort;
                --yystack.s_mark;
                --yystack.l_mark;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = YYEMPTY;
        goto yyloop;
    }

yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    if (yym)
        yyval = yystack.l_mark[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
    switch (yyn)
    {
    }
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yystack.s_mark = YYFINAL;
        *++yystack.l_mark = yyval;
        if (yychar < 0)
        {
            if ((yychar = YYLEX) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yystack.s_mark, yystate);
#endif
    if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
    {
        goto yyoverflow;
    }
    *++yystack.s_mark = (short) yystate;
    *++yystack.l_mark = yyval;
    goto yyloop;

yyoverflow:
    yyerror("yacc stack overflow");

yyabort:
    yyfreestack(&yystack);
    return (1);

yyaccept:
    yyfreestack(&yystack);
    return (0);
}
