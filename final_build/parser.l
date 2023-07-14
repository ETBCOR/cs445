%{
#include <string>
#include <float.h>
#include "scanType.h"
#include "node.h"
#include "parser.tab.h"

char * lastToken = NULL;
extern int errCount;
extern int wrnCount;

static std::string strFromCharP(char * c)
{
    int l = strlen(c); // lenght of string
    std::string s = std::string();

    // record the length of the string

    // loop through string (bounds are clipped by 1 on
    // each side so as to ignore the quote characters)
    for (int i = 1; i < l - 1; i++) {
        if (c[i] == '\\') {
            // escaped character

            if (i+1 < l) {
                // safe to peek next spot
                switch(c[i+1]) {
                    case 'n': s += '\n'; break;
                    case '0': s += '\0'; break;
                    case '\\': s += '\\'; break;
                    default: s += c[i+1];
                }
            }
            i++;
        } else
            // not an escaped character
            s += c[i];
    }
    return s;
}

static int cFromCharP(char * c)
{
    int l = strlen(c) - 2;
    char cc;
    if (l == 0) {
        // empty single quotes
        printf("Error(%d): EMpty character ''. Characters ignored.\n", yylineno);
        errCount++;
        return -1;
    } else if (l == 1) {
        // a literal single character
        cc = c[1];
    } else {
        // not a literal single character
        
        if (c[1] == '\\') {
            // this is an escaped character

            // if string is more than two characters, issue a warning
            if (l > 2) {
                printf("WARNING(%d): character is %d characters long and not a single character: '%s'.  The first char will be used.\n", yylineno, l, c);
                wrnCount++;
            }
            switch (c[2]) {
                case 'n': cc = '\n'; break;
                case '0': cc = '\0'; break;
                case '\\': cc = '\\'; break;
                default: cc = c[2];
            }
        } else {
            // strlen > one, but its not an escaped character
            printf("WARNING(%d): character is %d characters long and not a single character: '%s'.  The first char will be used.\n", yylineno, l, c);
            wrnCount++;
            cc = c[1];
        }
    }
    return (int)cc;
}
static int setValue(int tokenclass, char * tokenstr)
{
    yylval.tokenData = new TokenData;               // declare the token that will be returned
    yylval.tokenData->tokenclass = tokenclass;      // record the token's class
    yylval.tokenData->linenum = yylineno;           // record what line the token was found on
    yylval.tokenData->tokenstr = strdup(tokenstr);  // record the token's literal string

    int c;

    // determine what to do based on the class of token
    switch (tokenclass) {
        case ID:          yylval.tokenData->svalue = strdup(tokenstr); break;
        case STRINGCONST: yylval.tokenData->svalue = strFromCharP(tokenstr); break; 
        case NUMCONST:    yylval.tokenData->nvalue = atoi(tokenstr); break;
        case CHARCONST:   if ((c = cFromCharP(tokenstr)) != -1) yylval.tokenData->cvalue = (char)c; break; 
        case BOOLCONST:   yylval.tokenData->nvalue = strcmp(yytext, "true") == 0 ? 1 : 0; break;
    }

    lastToken = strdup(tokenstr);
    return tokenclass;
}
%}

%option noyywrap
%option nounput
%option yylineno

%%
[ \t] ; /* ignore whitespace */
\/\/.*$ ; /* ignore comments */

    /* variable types */
static                  { return setValue(STATIC, yytext);      }
bool                    { return setValue(BOOL,   yytext);      }
char                    { return setValue(CHAR,   yytext);      }
int                     { return setValue(INT,    yytext);      }

    /* keywords */
if                      { return setValue(IF,     yytext);      }
then                    { return setValue(THEN,   yytext);      }
else                    { return setValue(ELSE,   yytext);      }
while                   { return setValue(WHILE,  yytext);      }
for                     { return setValue(FOR,    yytext);      }
do                      { return setValue(DO,     yytext);      }
to                      { return setValue(TO,     yytext);      }
by                      { return setValue(BY,     yytext);      }
return                  { return setValue(RETURN, yytext);      }
break                   { return setValue(BREAK,  yytext);      }

    /* operators */
and                     { return setValue(AND,    yytext);      }
or                      { return setValue(OR,     yytext);      }
"+"                     { return setValue(PLUS,   yytext);      }
"-"                     { return setValue(MINUS,  yytext);      }
"=="                    { return setValue(EQ,     yytext);      }
"!="                    { return setValue(NEQ,    yytext);      }
"<="                    { return setValue(LEQ,    yytext);      }
">="                    { return setValue(GEQ,    yytext);      }
"<"                     { return setValue(LESS,   yytext);      }
">"                     { return setValue(GRTR,   yytext);      }
"="                     { return setValue(ASS,    yytext);      }
"+="                    { return setValue(ADDASS, yytext);      }
"-="                    { return setValue(SUBASS, yytext);      }
"*="                    { return setValue(MULASS, yytext);      }
"/="                    { return setValue(DIVASS, yytext);      }
"--"                    { return setValue(DEC,    yytext);      }
"++"                    { return setValue(INC,    yytext);      }
not                     { return setValue(NOT,    yytext);      }
"*"                     { return setValue(ASTR,   yytext);      }
"/"                     { return setValue(SLASH,  yytext);      }
"%"                     { return setValue(PERC,   yytext);      }
"?"                     { return setValue(QUEST,  yytext);      }

    /* parens/braces/brackets */
"("                     { return setValue(PARENL, yytext);      }
")"                     { return setValue(PARENR, yytext);      }
"["                     { return setValue(BRACKL, yytext);      }
"]"                     { return setValue(BRACKR, yytext);      }
"{"                     { return setValue(BRACEL, yytext);      }
"}"                     { return setValue(BRACER, yytext);      }

    /* separators */
";"                     { return setValue(SEMCOL, yytext);      }
":"                     { return setValue(COL,    yytext);      }
","                     { return setValue(COMMA,  yytext);      }

    /* CONST types */
true|false              { return setValue(BOOLCONST, yytext);   }
[A-Za-z][A-Za-z0-9]*    { return setValue(ID, yytext);          }
[0-9]+                  { return setValue(NUMCONST, yytext);    }
\'([^\'\\\n]|\\.)*\'    { return setValue(CHARCONST, yytext);   }
\"(\\.|[^\"\n])*\"      { return setValue(STRINGCONST, yytext); }
[^ \n\t]                { printf("ERROR(%d): Invalid or misplaced input character: '%c'. Character Ignored.\n", yylineno, yytext[0]); errCount++; }
.|\n                    ;
%%
