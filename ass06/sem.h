#ifndef _SEM_H_
#define _SEM_H_

#include "node.h"
#include "error.h"
#include "scanType.h"
#include "symbolTable.h"

void analyze(Node *, SymbolTable *);    // performs semantic analysis

#endif /* _SEM_H_ */
