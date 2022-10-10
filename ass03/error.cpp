#include "error.h"

void throwErr (ErrCode err, Node * node, Node * found)
{
    switch (err) {
        case VAR_AS_FUN:
            printf("ERROR(%d): '%s' is a simple variable and cannot be called.\n", node->linenum, ((VarDecl *)node)->name);
            break;
        case ARR_MISMATCH:
            printf("ERROR(%d): '%s' requires both operands be arrays or not but lhs is%s an array and rhs is%s an array.\n", node->linenum, ((Op *)node)->opName, node->child[0]->isArray ? "" : " not", node->child[1]->isArray ? "" : " not");
            break;
        case OP_LHS:
            printf("ERROR(%d): '%s' requires operands of type %s but lhs is of type %s.\n", node->linenum, ((Op *)node)->opName, node->type, node->child[0]->type);
            break;
        case OP_RHS:
            printf("ERROR(%d): '%s' requires operands of type %s but rhs is of type %s.\n", node->linenum, ((Op *)node)->opName, node->type, node->child[1]->type);
            break;
        case OPERAND_MISMATCH:
            printf("ERROR(%d): '%s' requires operands of the same type but lhs is type %s and rhs is type %s.\n", node->linenum, ((Op *)node)->opName, node->child[0]->type, node->child[1]->type);
            break;
        case ARR_NONINT_INDEX:
            printf("ERROR(%d): Array '%s' should be indexed by type int but got type %s.\n", node->linenum, ((VarDecl *)node->child[0])->name, node->child[1]->type);
            break;
        case ARR_UNINDEXED:
            printf("ERROR(%d): Array index is the unindexed array '%s'.\n", node->linenum, ((VarDecl *)node->child[1])->name);
            break;
        case NON_ARR_INDEX:
            printf("ERROR(%d): Cannot index nonarray '%s'.\n", node->linenum, ((VarDecl *)node->child[0])->name);
            break;
        case ARR_RETURN:
            printf("ERROR(%d): Cannot return an array.\n", node->linenum);
            break;
        case FUN_AS_VAR:
            printf("ERROR(%d): Cannot use function '%s' as a variable.\n", node->linenum, ((VarDecl *)node)->name);
            break;
        case DECL_DBL:
            printf("ERROR(%d): Symbol '%s' is already declared at line %d.\n", node->linenum, ((VarDecl *)node)->name, found->linenum);
            break;
        case DECL_NOT:
            printf("ERROR(%d): Symbol '%s' is not declared.\n", node->linenum, ((VarDecl *)node)->name);
            break;
        case OP_ARR:
            printf("ERROR(%d): The operation '%s' does not work with arrays.\n", node->linenum, ((Op *)node)->opName);
            break;
        case OP_NOT_ARR:
            printf("ERROR(%d): The operation '%s' only works with arrays.\n", node->linenum, ((Op *)node)->opName);
            break;
        case OPERAND_UNARY:
            printf("ERROR(%d): Unary '%s' requires an operand of type %s but was given type %s.\n", node->linenum, ((Op *)node)->opName, node->type, node->child[0]->type);
            break;
        case NO_MAIN:
            printf("ERROR(LINKER): A function named 'main()' must be defined.\n");
            break;
    }
    errCount++;
}

void throwWrn (WrnCode wrn, Node * node, Node * found)
{
    switch (wrn) {
        case VAR_UNUSED:
            printf("WARNING(%d): The variable '%s' seems not to be used.\n", node->linenum, ((VarDecl *)node)->name);
            ((VarDecl *)node)->usageWrnFlg = true;
            break;
        case VAR_UNINITED:
            printf("WARNING(%d): Variable '%s' may be uninitialized when used here.\n", node->linenum, ((VarDecl *)node)->name);
            ((VarDecl *)found)->usageWrnFlg = true;
            break;
    }
    wrnCount++;
}

void wrnUnused(std::string symbol, void * node_)
{
    VarDecl * node = (VarDecl *) node_;
    if(!node->isFun && !node->usageFlg && !node->usageWrnFlg) throwWrn(VAR_UNUSED, node);
}

