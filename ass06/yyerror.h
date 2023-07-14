#ifndef _YYERROR_H_
#define _YYERROR_H_

extern int yylineno;          // line number of last token scanned in the scanner
extern char * lastToken;      // last token scanned in the scanner
extern int errCount;          // number of errors

void initErrorProcessing();    // WARNING: MUST be called before any errors occur (near top of main)!
void yyerror(const char *msg); // error routine called by Bison

#endif
