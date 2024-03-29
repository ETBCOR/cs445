%{
#include <string>
#include "y.tab.h"
#include "scanType.h"
#include <float.h>

int curline = 1;

static int setValue(int linenum, int tokenclass, char *tokenstr)
{
    yylval.tokenData = new TokenData;
    yylval.tokenData->linenum = linenum;
    yylval.tokenData->tokenstr = strdup(tokenstr);

    std::string s;
    int l;
    switch (tokenclass) {

        case ID:
            yylval.tokenData->svalue = strdup(tokenstr);
            break;

        case STRINGCONST:

            s = std::string();

            // record the length of the string
            l = strlen(tokenstr);

            // loop through string
            for (int i = 1; i < l - 1; i++) {
                if (tokenstr[i] == '\\') {
                    // escaped character

                    if (i+1 < l) {
                        // safe to peek next spot
                        switch(tokenstr[i+1]) {
                            case 'n':
                                s += '\n';
                                break;
                            case '0':
                                //s += '';
                                break;
                            case '\\':
                                s += '\\';
                                break;
                            default:
                                s += tokenstr[i+1];
                        }
                        
                    } else {
                        // "stuff\" edge case
                        printf("BIGOOPS in parser.l");
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
            yylval.tokenData->svalue[s.length()] = '\0';

            break;
        
        case NUMCONST:
            yylval.tokenData->nvalue = atof(tokenstr);
            break;

        case CHARCONST:

            l = strlen(tokenstr) - 2;
            if (l > 1) {
                // not a literal single character
                
                if (tokenstr[1] == '\\') {
                    // this is an escaped character

                    // if string is more than two characters, issue a warning
                    if (l > 2) {
                        printf("WARNING(%d): character is %d characters long and not a single character: '%s'.  The first char will be used.\n", linenum, l, tokenstr);
                    }
                    switch (tokenstr[2]) {
                        case 'n':
                            yylval.tokenData->cvalue = '\n';
                            break;
                        case '0':
                            yylval.tokenData->cvalue = '\0';
                            break;
                        case '\\':
                            yylval.tokenData->cvalue = '\\';
                            break;
                        default: 
                            yylval.tokenData->cvalue = tokenstr[2];
                    }
                } else {
                    // strlen > one, but its not an escaped character
                    printf("WARNING(%d): character is %d characters long and not a single character: '%s'.  The first char will be used.\n", linenum, l, tokenstr);
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

        case CATCH:
            break;

        //default:
            //printf("(setValue) could not parse type %d\n", tokenclass);
    }
    return tokenclass;
}
%}

%option noyywrap
%option nounput

%%
[ \t] ; /* ignore whitespace */
\/\/.*$ ; /* ignore comments */

    /* variable types */
static  { return setValue(curline, STATIC, yytext); }
bool    { return setValue(curline, BOOL, yytext); }
char    { return setValue(curline, CHAR, yytext); }
int     { return setValue(curline, INT, yytext); }

    /* keywords */
if      { return setValue(curline, IF, yytext); }
then    { return setValue(curline, THEN, yytext); }
else    { return setValue(curline, ELSE, yytext); }
while   { return setValue(curline, WHILE, yytext); }
for     { return setValue(curline, FOR, yytext); }
do      { return setValue(curline, DO, yytext); }
to      { return setValue(curline, TO, yytext); }
by      { return setValue(curline, BY, yytext); }
return  { return setValue(curline, RETURN, yytext); }
break   { return setValue(curline, BREAK, yytext); }

    /* operators */
and     { return setValue(curline, AND, yytext); }
or      { return setValue(curline, OR, yytext); }
"=="    { return setValue(curline, EQ, yytext); }
"!="    { return setValue(curline, NEQ, yytext); }
"<="    { return setValue(curline, LEQ, yytext); }
">="    { return setValue(curline, GEQ, yytext); }
"+="    { return setValue(curline, ADDASS, yytext); }
"-="    { return setValue(curline, SUBASS, yytext); }
"*="    { return setValue(curline, MULASS, yytext); }
"/="    { return setValue(curline, DIVASS, yytext); }
"--"    { return setValue(curline, DEC, yytext); }
"++"    { return setValue(curline, INC, yytext); }
not     { return setValue(curline, NOT, yytext); }

    /* CONST types */
true|false                              { return setValue(curline, BOOLCONST, yytext); }
[A-Za-z][A-Za-z0-9]*                    { return setValue(curline, ID, yytext); }
[0-9]+                                  { return setValue(curline, NUMCONST, yytext); }
\'([^\'\\\n]|\\.)+\'                    { return setValue(curline, CHARCONST, yytext); }
\"(\\.|[^\"\n])*\"                      { return setValue(curline, STRINGCONST, yytext); }
[\%\*\(\)\-\+\=\{\}\[\]\:\;\<\>\?\,\/]  { return setValue(curline, CATCH, yytext); }
\n                                      { curline++; }
.                                       { printf("ERROR(%d): Invalid or misplaced input character: '%c'. Character Ignored.\n", curline, yytext[0]); }
%%
