#ifndef _ERROR_H_
#define _ERROR_H_

#include "node.h"

extern int errCount;
extern int wrnCount;

enum ErrCode {VAR_AS_FUN, ARR_MISMATCH, OP_LHS, OP_RHS, OPERAND_MISMATCH, ARR_NONINT_INDEX, ARR_UNINDEXED, NON_ARR_INDEX, ARR_RETURN, FUN_AS_VAR, DECL_DBL, DECL_NOT, OP_ARR, OP_NOT_ARR, OPERAND_UNARY, NO_MAIN};
enum WrnCode {VAR_UNUSED, VAR_UNINITED};

void throwErr (ErrCode, Node *, Node * = NULL);
void throwWrn (WrnCode, Node *, Node * = NULL);

void wrnUnused(std::string, void *);

#endif /* _ERROR_H_ */

