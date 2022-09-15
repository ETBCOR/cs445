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
#line 23 "parser.y"
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
#line 50 "y.tab.c"

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
#define EQ 273
#define NEQ 274
#define LEQ 275
#define GEQ 276
#define ADDASS 277
#define SUBASS 278
#define MULASS 279
#define DIVASS 280
#define DEC 281
#define INC 282
#define NOT 283
#define BOOLCONST 284
#define ID 285
#define NUMCONST 286
#define CHARCONST 287
#define STRINGCONST 288
#define CATCH 289
#define YYERRCODE 256
static const short yylhs[] = {                           -1,
    0,    0,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,
};
static const short yylen[] = {                            2,
    2,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,
};
static const short yydefred[] = {                         0,
    3,    4,    5,    6,    7,    8,    9,   10,   11,   12,
   13,   14,   15,   16,   17,   18,   19,   20,   21,   22,
   23,   24,   25,   26,   27,   28,   29,   30,   31,   32,
   33,   34,   35,    0,    2,    1,
};
static const short yydgoto[] = {                         34,
   35,
};
static const short yysindex[] = {                      -257,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, -257,    0,    0,
};
static const short yyrindex[] = {                         0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,
};
static const short yygindex[] = {                         0,
   -1,
};
#define YYTABLESIZE 33
static const short yytable[] = {                          1,
    2,    3,    4,    5,    6,    7,    8,    9,   10,   11,
   12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
   22,   23,   24,   25,   26,   27,   28,   29,   30,   31,
   32,   33,   36,
};
static const short yycheck[] = {                        257,
  258,  259,  260,  261,  262,  263,  264,  265,  266,  267,
  268,  269,  270,  271,  272,  273,  274,  275,  276,  277,
  278,  279,  280,  281,  282,  283,  284,  285,  286,  287,
  288,  289,   34,
};
#define YYFINAL 34
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 289
#if YYDEBUG
static const char *yyname[] = {

"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"STATIC","BOOL","CHAR","INT","IF",
"THEN","ELSE","WHILE","FOR","DO","TO","BY","RETURN","BREAK","AND","OR","EQ",
"NEQ","LEQ","GEQ","ADDASS","SUBASS","MULASS","DIVASS","DEC","INC","NOT",
"BOOLCONST","ID","NUMCONST","CHARCONST","STRINGCONST","CATCH",
};
static const char *yyrule[] = {
"$accept : statementlist",
"statementlist : statementlist statement",
"statementlist : statement",
"statement : STATIC",
"statement : BOOL",
"statement : CHAR",
"statement : INT",
"statement : IF",
"statement : THEN",
"statement : ELSE",
"statement : WHILE",
"statement : FOR",
"statement : DO",
"statement : TO",
"statement : BY",
"statement : RETURN",
"statement : BREAK",
"statement : AND",
"statement : OR",
"statement : EQ",
"statement : NEQ",
"statement : LEQ",
"statement : GEQ",
"statement : ADDASS",
"statement : SUBASS",
"statement : MULASS",
"statement : DIVASS",
"statement : DEC",
"statement : INC",
"statement : NOT",
"statement : BOOLCONST",
"statement : ID",
"statement : NUMCONST",
"statement : CHARCONST",
"statement : STRINGCONST",
"statement : CATCH",

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
#line 77 "parser.y"

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
#line 285 "y.tab.c"

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
case 3:
#line 39 "parser.y"
	{ printf("Line %d Token: STATIC\n", yyval.tokenData->linenum); }
break;
case 4:
#line 40 "parser.y"
	{ printf("Line %d Token: BOOL\n", yyval.tokenData->linenum); }
break;
case 5:
#line 41 "parser.y"
	{ printf("Line %d Token: CHAR\n", yyval.tokenData->linenum); }
break;
case 6:
#line 42 "parser.y"
	{ printf("Line %d Token: INT\n", yyval.tokenData->linenum); }
break;
case 7:
#line 44 "parser.y"
	{ printf("Line %d Token: IF\n", yyval.tokenData->linenum); }
break;
case 8:
#line 45 "parser.y"
	{ printf("Line %d Token: THEN\n", yyval.tokenData->linenum); }
break;
case 9:
#line 46 "parser.y"
	{ printf("Line %d Token: ELSE\n", yyval.tokenData->linenum); }
break;
case 10:
#line 47 "parser.y"
	{ printf("Line %d Token: WHILE\n", yyval.tokenData->linenum); }
break;
case 11:
#line 48 "parser.y"
	{ printf("Line %d Token: FOR\n", yyval.tokenData->linenum); }
break;
case 12:
#line 49 "parser.y"
	{ printf("Line %d Token: DO\n", yyval.tokenData->linenum); }
break;
case 13:
#line 50 "parser.y"
	{ printf("Line %d Token: TO\n", yyval.tokenData->linenum); }
break;
case 14:
#line 51 "parser.y"
	{ printf("Line %d Token: BY\n", yyval.tokenData->linenum); }
break;
case 15:
#line 52 "parser.y"
	{ printf("Line %d Token: RETURN\n", yyval.tokenData->linenum); }
break;
case 16:
#line 53 "parser.y"
	{ printf("Line %d Token: BREAK\n", yyval.tokenData->linenum); }
break;
case 17:
#line 55 "parser.y"
	{ printf("Line %d Token: AND\n", yyval.tokenData->linenum); }
break;
case 18:
#line 56 "parser.y"
	{ printf("Line %d Token: OR\n", yyval.tokenData->linenum); }
break;
case 19:
#line 57 "parser.y"
	{ printf("Line %d Token: EQ\n", yyval.tokenData->linenum); }
break;
case 20:
#line 58 "parser.y"
	{ printf("Line %d Token: NEQ\n", yyval.tokenData->linenum); }
break;
case 21:
#line 59 "parser.y"
	{ printf("Line %d Token: LEQ\n", yyval.tokenData->linenum); }
break;
case 22:
#line 60 "parser.y"
	{ printf("Line %d Token: GEQ\n", yyval.tokenData->linenum); }
break;
case 23:
#line 61 "parser.y"
	{ printf("Line %d Token: ADDASS\n", yyval.tokenData->linenum); }
break;
case 24:
#line 62 "parser.y"
	{ printf("Line %d Token: SUBASS\n", yyval.tokenData->linenum); }
break;
case 25:
#line 63 "parser.y"
	{ printf("Line %d Token: MULASS\n", yyval.tokenData->linenum); }
break;
case 26:
#line 64 "parser.y"
	{ printf("Line %d Token: DIVASS\n", yyval.tokenData->linenum); }
break;
case 27:
#line 65 "parser.y"
	{ printf("Line %d Token: DEC\n", yyval.tokenData->linenum); }
break;
case 28:
#line 66 "parser.y"
	{ printf("Line %d Token: INC\n", yyval.tokenData->linenum); }
break;
case 29:
#line 67 "parser.y"
	{ printf("Line %d Token: NOT\n", yyval.tokenData->linenum); }
break;
case 30:
#line 69 "parser.y"
	{ printf("Line %d Token: BOOLCONST Value: %d  Input: %s\n", yyval.tokenData->linenum, yyval.tokenData->nvalue, yyval.tokenData->tokenstr); }
break;
case 31:
#line 70 "parser.y"
	{ printf("Line %d Token: ID Value: %s\n", yyval.tokenData->linenum, yyval.tokenData->svalue); }
break;
case 32:
#line 71 "parser.y"
	{ printf("Line %d Token: NUMCONST Value: %d  Input: %s\n", yyval.tokenData->linenum, yyval.tokenData->nvalue, yyval.tokenData->tokenstr); }
break;
case 33:
#line 72 "parser.y"
	{ printf("Line %d Token: CHARCONST Value: \'%c\'  Input: %s\n", yyval.tokenData->linenum, yyval.tokenData->cvalue, yyval.tokenData->tokenstr); }
break;
case 34:
#line 73 "parser.y"
	{ printf("Line %d Token: STRINGCONST Value: \"%s\"  Len: %ld  Input: %s\n", yyval.tokenData->linenum, yyval.tokenData->svalue, strlen(yyval.tokenData->svalue), yyval.tokenData->tokenstr); }
break;
case 35:
#line 74 "parser.y"
	{ printf("Line %d Token: %s\n", yyval.tokenData->linenum, yyval.tokenData->tokenstr); }
break;
#line 623 "y.tab.c"
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
