#ifndef _MAIN_H_
#define _MAIN_H_

#include "ourgetopt.h"
#include "node.h"
#include "symbolTable.h"
#include "sem.h"
#include "yyerror.h"

extern FILE * yyin;
extern int optind;

Node * AST;

int errCount;
int wrnCount;

#endif /* _MAIN_H_ */
