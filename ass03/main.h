#ifndef MAIN_H
#define MAIN_H

#include "ourgetopt.h"
#include "node.h"
#include "symbolTable.h"

extern FILE * yyin;
extern int optind;

Node * AST;

int errCount;
int wrnCount;

#endif /* MAIN_H */
