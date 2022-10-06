#include "error.h"

void throwErr (ErrCode err, Node * node)
{
    switch (err) {
        case SIMPLE_CALL:
            printf("ERROR(%d): '%s' is a simple variable and cannot be called.\n", node->linenum, ((VarDecl *)node)->name);
            break;
        case ARR_MISMATCH: {
            Op * o = (Op *)node;
            printf("ERROR(%d): '%s' requires both operands be arrays or not but lhs is%s an array and rhs is%s an array.\n", o->linenum, o->opName, o->child[0]->type, o->child[1]->type);
            break;
        }
        case OP_LHS: {
            Op * o = (Op *)node;
            printf("ERROR(%d): '%s' requires operands of %s but lhs is of %s.\n", o->linenum, o->opName, o->type, o->child[0]->type);
            break;
        }
        case OP_RHS: {
            Op * o = (Op *)node;
            printf("ERROR(%d): '%s' requires operands of %s but rhs is of %s.\n", o->linenum, o->opName, o->type, o->child[1]->type);
            break;
        }
        case OPERAND_MISMATCH: {
            Op * o = (Op *)node;
            printf("ERROR(%d): '%s' requires operands of the same type but lhs is %s and rhs is %s.\n", o->linenum, o->opName, o->child[0]->type, o->child[1]->type);
            break;
        }
        case ARR_NONINT_INDEX: {
            Op * o = (Op *)node;
            printf("ERROR(%d): Array '%s' should be indexed by type int but got %s.\n", o->linenum, ((VarDecl *)o->child[0])->name, o->child[1]->type);
            break;
        }
        case ARR_UNINDEXED: {
            Op * o = (Op *)node;
            printf("ERROR(%d): Array index is the unindexed array '%s'.\n", o->linenum, ((VarDecl *)o->child[1])->name);
            break;
        }
        case NON_ARR_INDEX:
            printf("ERROR(%d): Cannot index nonarray '%s'.\n", node->linenum);
            break;
        case ARR_RETURN:
            printf("ERROR(%d): Cannot return an array.\n", node->linenum);
            break;
        case FUN_AS_VAR:
            printf("ERROR(%d): Cannot use function '%s' as a variable.\n", node->linenum, ((VarDecl *)node)->name);
            break;
        case DECL_DBL:
            printf("ERROR(%d): Symbol '%s' is already declared at line %d.\n", node->linenum, ((VarDecl *)node)->name, -1);
            break;
        case DECL_NOT:
            printf("ERROR(%d): Symbol '%s' is not declared.\n", node->linenum, ((VarDecl *)node)->name);
            break;
        case OP_ARR:
            printf("ERROR(%d): The operation '%s' does not work with arrays.\n", node->linenum);
            break;
        case OP_NOT_ARR:
            printf("ERROR(%d): The operation '%s' only works with arrays.\n", node->linenum);
            break;
        case OPERAND_UNARY:
            printf("ERROR(%d): Unary '%s' requires an operand of %s but was given %s.\n", node->linenum);
            break;
        case NO_MAIN:
            printf("ERROR(LINKER): A function named 'main()' must be defined.\n");
            break;
    }
    errCount++;
}

void throwWrn (WrnCode wrn, Node * node)
{
    switch (wrn) {
        case VAR_UNUSED: {
            VarDecl * v = (VarDecl *) node;
            printf("WARNING(%d): The variable '%s' seems not to be used.\n", v->linenum, v->name);
            v->usageWrnFlg = true;
            break;
        }
        case VAR_UNINITED: {
            VarDecl * v = (VarDecl *) node;
            printf("WARNING(%d): Variable '%s' may be uninitialized when used here.\n", v->linenum, v->name);
            v->usageWrnFlg = true;
            break;
        }
    }
    wrnCount++;
}

void wrnUnused(std::string symbol, void * node_)
{
    VarDecl * node = (VarDecl *) node_;
    if(!node->isFun && !node->usageFlg && !node->usageWrnFlg) throwWrn(VAR_UNUSED, node);
}

