%{
#include "y.tab.h"
#include "scanType.h"
#include <float.h>

int curline = 0;

static int setValue(int linenum, TokenClass tokenclass, char *tokenstr)
{
    yylval.tokenData = new TokenData;
    yylval.tokenData->linenum = linenum;
    yylval.tokenData->tokenstr = strdup(tokenstr);

    switch (tokenclass) {
        case ID_:
            yylval.tokenData->svalue = strdup(tokenstr);
            break;
        case NUMCONST_:
            yylval.tokenData->nvalue = atof(tokenstr);
            break;
        case CHARCONST_:
            if (strlen(tokenstr) > 1) {
                if (tokenstr[0] == '\\' && (tokenstr[1] == 'n' || tokenstr[1] == '0' || tokenstr[1] == '\\')) {
                    // this is an escaped character
                    if (strlen(tokenstr) > 2) {
                        printf("WARNING(%d): character is %d characters long and not a single character: ''%s''. The first char will be used.\n", linenum, strlen(tokenstr), tokenstr);
                    }
                    switch (tokenstr[1]) {
                        case 'n':
                            yylval.tokenData->cvalue = '\n';
                            break;
                        case '0':
                            yylval.tokenData->cvalue = '\0';
                            break;
                        case '\\':
                            yylval.tokenData->cvalue = '\\';
                            break;
                        default: printf("WARNING(%d): %c not found in switch.\n", linenum, tokenstr[1]);
                    }
                } else {
                    // strlen > one, but its not an escaped character
                    printf("WARNING(%d): character is %d characters long and not a single character: ''%s''. The first char will be used.\n", linenum, strlen(tokenstr), tokenstr);
                }
            } else {
                // strlen is one
                yylval.tokenData->cvalue = tokenstr[0];
            }
        case STRINGCONST_:
            yylval.tokenData->svalue = strdup(tokenstr);
            break;
        case BOOLCONST_:
            yylval.tokenData->nvalue = strcmp(yytext, "true") == 0 ? 1 : 0;
            break;
        default:
            printf("(setValue) could not parse type %d\n", tokenclass);
    }
}
%}

%%
[ \t] ; /* ignore whitespace */
\/\/.*$ ; /* ignore comments */
[A-Za-z][A-Za-z0-9]* { return setValue(curline, ID_, yytext); }
([0-9]+|([0-9]*\.[0-9]+)([eE][+-]?[0-9]+)?) { return setValue(curline, NUMCONST_, yytext); }
'[A-Za-z]|\\(n|0)'   { return setValue(curline, CHARCONST_, yytext); }
\"(\\.|[^\"])*\"     { return setValue(curline, STRINGCONST_, yytext); }
"true"|"false"       { return setValue(curline, BOOLCONST_, yytext); }
\n                   { curline++; }
.                    { printf("ERROR(%d): Invalid or misplaced input character: '%c'. Character Ignored.\n"); }
%%

// end
