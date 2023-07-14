#include "error.h"

void throwErr (ErrCode err, Node * n1, Node * n2, Node * n3, Node * n4, int i)
{
    switch (err) {
        case VAR_AS_FUN:
            printf("ERROR(%d): '%s' is a simple variable and cannot be called.\n",
                   n1->linenum, ((VarDecl *)n1)->name); break;
        case ARR_MISMATCH:
            printf("ERROR(%d): '%s' requires both operands be arrays or not but lhs is%s an array and rhs is%s an array.\n",
                   n1->linenum, ((Op *)n1)->opName, n1->child[0]->isArray ? "" : " not", n1->child[1]->isArray ? "" : " not"); break;
        case OP_LHS:
            printf("ERROR(%d): '%s' requires operands of type %s but lhs is of type %s.\n",
                   n1->linenum, ((Op *)n1)->opName, n1->type, n1->child[0]->type); break;
        case OP_RHS:
            printf("ERROR(%d): '%s' requires operands of type %s but rhs is of type %s.\n",
                   n1->linenum, ((Op *)n1)->opName, n1->type, n1->child[1]->type); break;
        case OPERAND_MISMATCH:
            printf("ERROR(%d): '%s' requires operands of the same type but lhs is type %s and rhs is type %s.\n",
                   n1->linenum, ((Op *)n1)->opName, n1->child[0]->type, n1->child[1]->type); break;
        case ARR_NONINT_INDEX:
            printf("ERROR(%d): Array '%s' should be indexed by type int but got type %s.\n",
                   n1->linenum, ((VarDecl *)n1->child[0])->name, n1->child[1]->type); break;
        case ARR_UNINDEXED:
            printf("ERROR(%d): Array index is the unindexed array '%s'.\n",
                   n1->linenum, ((VarDecl *)n1->child[1])->name); break;
        case LOOPLESS_BREAK:
            printf("ERROR(%d): Cannot have a break statement outside of loop.\n",
                   n1->linenum); break;
        case NON_ARR_INDEX:
            printf("ERROR(%d): Cannot index nonarray '%s'.\n",
                   n1->linenum, ((VarDecl *)n1->child[0])->name); break;
        case ARR_RETURN:
            printf("ERROR(%d): Cannot return an array.\n",
                   n1->linenum); break;
        case ARR_COND:
            printf("ERROR(%d): Cannot use array as test condition in %s statement.\n",
                   n1->linenum, n1->token->tokenstr); break;
        case ARR_RANGE:
            printf("ERROR(%d): Cannot use array in position %d in range of for statement.\n",
                   n1->linenum, i); break;
        case FUN_AS_VAR:
            printf("ERROR(%d): Cannot use function '%s' as a variable.\n",
                   n1->linenum, ((VarDecl *)n1)->name); break;
        case PARM_TYPE:
            printf("ERROR(%d): Expecting type %s in parameter %i of call to '%s' declared on line %d but got type %s.\n",
                   n1->linenum, n2->type, i, ((FunDecl* )n4)->name, n4->linenum, n3->type); break;
        case RANGE_TYPE:
            printf("ERROR(%d): Expecting type %s in position %d in range of for statement but got type %s.\n",
                   n1->linenum, n1->type, i, n2->type); break;
        case NONBOOL_COND:
            printf("ERROR(%d): Expecting Boolean test condition in %s statement but got type %s.\n",
                   n1->linenum, n1->token->tokenstr, n2->type); break;
        case PARM_NONARR:
            printf("ERROR(%d): Expecting array in parameter %i of call to '%s' declared on line %d.\n",
                   n1->linenum, i, ((FunDecl *)n2)->name, n2->linenum); break;
        case RETURN_VALUED:
            printf("ERROR(%d): Function '%s' at line %d is expecting no return value, but return has a value.\n",
            n1->linenum, ((FunDecl *)n2)->name, n2->linenum); break;
        case RETURN_UNVALUED:
            printf("ERROR(%d): Function '%s' at line %d is expecting to return type %s but return has no value.\n",
                   n1->linenum, ((FunDecl *)n2)->name, n2->linenum, n2->type); break;
        case RETURN_MISMATCH:
            printf("ERROR(%d): Function '%s' at line %d is expecting to return type %s but returns type %s.\n",
                   n1->linenum, ((FunDecl *)n2)->name, n2->linenum, n2->type, n1->child[0]->type); break;
        case INIT_NONCONST:
            printf("ERROR(%d): Initializer for variable '%s' is not a constant expression.\n",
                   n1->linenum, ((VarDecl *)n1)->name); break;
        case INIT_MISMATCH:
            printf("ERROR(%d): Initializer for variable '%s' of type %s is of type %s\n",
                   n1->linenum, ((VarDecl *)n1)->name, n1->type, n2->type); break;
        case INIT_ARR_MISMATCH:
            printf("ERROR(%d): Initializer for variable '%s' requires both operands be arrays or not but variable is%s an array and rhs is%s an array.\n",
                   n1->linenum, ((VarDecl *)n1)->name, n1->isArray ? "" : " not", n2->isArray ? "" : " not"); break;
        case PARM_ARR:
            printf("ERROR(%d): Not expecting array in parameter %i of call to '%s' declared on line %d.\n",
                   n1->linenum, i, ((FunDecl *)n2)->name, n2->linenum); break;
        case DECL_DBL:
            printf("ERROR(%d): Symbol '%s' is already declared at line %d.\n",
                   n1->linenum, ((VarDecl *)n1)->name, n2->linenum); break;
        case DECL_NOT:
            printf("ERROR(%d): Symbol '%s' is not declared.\n",
                   n1->linenum, ((VarDecl *)n1)->name); break;
        case OP_ARR:
            printf("ERROR(%d): The operation '%s' does not work with arrays.\n",
                   n1->linenum, ((Op *)n1)->opName); break;
        case OP_NONARR:
            printf("ERROR(%d): The operation '%s' only works with arrays.\n",
                   n1->linenum, ((Op *)n1)->opName); break;
        case PARM_FEW:
            printf("ERROR(%d): Too few parameters passed for function '%s' declared on line %d.\n",
                   n1->linenum, ((FunDecl *)n2)->name, n2->linenum); break;
        case PARM_MANY:
            printf("ERROR(%d): Too many parameters passed for function '%s' declared on line %d.\n",
                   n1->linenum, ((FunDecl *)n2)->name, n2->linenum); break;
        case OPERAND_UNARY:
            printf("ERROR(%d): Unary '%s' requires an operand of type %s but was given type %s.\n",
                   n1->linenum, ((Op *)n1)->opName, n1->type, n1->child[0]->type); break;
        case NO_MAIN:
            printf("ERROR(LINKER): A function named 'main' with no parameters must be defined.\n"); break;
    }
    errCount++;
}

void throwWrn (WrnCode wrn, Node * n1, Node * n2)
{
    switch (wrn) {
        case NO_RETURN:
            printf("WARNING(%d): Expecting to return type %s but function '%s' has no return statement.\n",
                   n1->linenum, n1->type, ((FunDecl *)n1)->name); break;
        case FUN_UNUSED:
            printf("WARNING(%d): The function '%s' seems not to be used.\n",
                   n1->linenum, ((VarDecl*)n1)->name); break;
            ((VarDecl *)n1)->usageWrnFlg = true;
        case PARM_UNUSED:
            printf("WARNING(%d): The parameter '%s' seems not to be used.\n",
                   n1->linenum, ((VarDecl*)n1)->name); break;
            ((VarDecl *)n1)->usageWrnFlg = true;
        case VAR_UNUSED:
            printf("WARNING(%d): The variable '%s' seems not to be used.\n",
                   n1->linenum, ((VarDecl *)n1)->name);
            ((VarDecl *)n1)->usageWrnFlg = true;
            break;
        case VAR_UNINITED:
            printf("WARNING(%d): Variable '%s' may be uninitialized when used here.\n",
                   n1->linenum, ((VarDecl *)n1)->name);
            ((VarDecl *)n2)->usageWrnFlg = true;
            break;
    }
    wrnCount++;
}

void wrnUnused(std::string symbol, void * node_)
{
    VarDecl * node = (VarDecl *) node_;
    if (!node->usageFlg && !node->usageWrnFlg) {
        if (node->isParm)
            throwWrn(PARM_UNUSED, node);
        else if (node->isFun)
            throwWrn(FUN_UNUSED, node);
        else throwWrn(VAR_UNUSED, node);
    }
}

