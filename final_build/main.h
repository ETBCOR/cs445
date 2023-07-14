#ifndef _MAIN_H_
#define _MAIN_H_

#include "ourgetopt.h"
#include "node.h"
#include "symbolTable.h"
#include "yyerror.h"

extern FILE * yyin;
extern int optind;

char * src;
Node * AST;
FILE * code;

int errCount;
int wrnCount;
int fofs = 0, gofs = 0, tofs = 0;
int breakLoc = 0;

#endif /* _MAIN_H_ */
