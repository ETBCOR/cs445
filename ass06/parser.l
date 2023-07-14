%{
#include <string>
#include <float.h>
#include "scanType.h"
#include "node.h"
#include "parser.tab.h"

char * lastToken = NULL;
extern int errCount;
extern int wrnCount;

static int setValue(int tokenclass, char * tokenstr)
{
    yylval.tokenData = new TokenData;               // declare the token that will be returned
    yylval.tokenData->tokenclass = tokenclass;      // record the token's class
    yylval.tokenData->linenum = yylineno;           // record what line the token was found on
    yylval.tokenData->tokenstr = strdup(tokenstr);  // record the token's literal string

    std::string s; // used to construct a stringconst
    int l; // used for storing the length of stringconst

    // determine what to do based on the class of token
    switch (tokenclass) {

        case ID:
            yylval.tokenData->svalue = strdup(tokenstr);
            break;

        case STRINGCONST:
            s = std::string();

            // record the length of the string
            l = strlen(tokenstr);

            // loop through string (bounds are clipped by 1
            // on each side so as to ignore the quote characters
            for (int i = 1; i < l - 1; i++) {
                if (tokenstr[i] == '\\') {
                    // escaped character

                    if (i+1 < l) {
                        // safe to peek next spot
                        switch(tokenstr[i+1]) {
                            case 'n': s += '\n'; break;
                            case '0': break;
                            case '\\': s += '\\'; break;
                            default: s += tokenstr[i+1];
                        }
                    }
                    i++;
                } else {
                    // not an escaped character
                    s += tokenstr[i];
                }
            }
            yylval.tokenData->svalue = new char[s.length()];

            // copy contents into token
            for (int i = 0; i < (int)s.length(); i++) {
                yylval.tokenData->svalue[i] = s[i];
            }

            // add terminating character to the string
            yylval.tokenData->svalue[s.length()] = '\0';
            break;
        
        case NUMCONST:
            yylval.tokenData->nvalue = atoi(tokenstr);
            break;

        case CHARCONST:
            l = strlen(tokenstr) - 2;
            if (l > 1) {
                // not a literal single character
                
                if (tokenstr[1] == '\\') {
                    // this is an escaped character

                    // if string is more than two characters, issue a warning
                    if (l > 2) {
                        printf("WARNING(%d): character is %d characters long and not a single character: '%s'.  The first char will be used.\n", yylineno, l, tokenstr);
                        wrnCount++;
                    }
                    switch (tokenstr[2]) {
                        case 'n': yylval.tokenData->cvalue = '\n'; break;
                        case '0': yylval.tokenData->cvalue = '\0'; break;
                        case '\\': yylval.tokenData->cvalue = '\\'; break;
                        default: yylval.tokenData->cvalue = tokenstr[2];
                    }
                } else {
                    // strlen > one, but its not an escaped character
                    printf("WARNING(%d): character is %d characters long and not a single character: '%s'.  The first char will be used.\n", yylineno, l, tokenstr);
                    wrnCount++;
                    yylval.tokenData->cvalue = tokenstr[1];
                }
            } else {
                // a literal single character
                yylval.tokenData->cvalue = tokenstr[1];
            }
            break; 

        case BOOLCONST:
            yylval.tokenData->nvalue = strcmp(yytext, "true") == 0 ? 1 : 0;
            break;
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
