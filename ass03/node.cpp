#include "node.h"

bool doInitChecking = true;

Node::Node()
{
    sibling = NULL;
    linenum = -1;
    depth = 0;
    index = 1;
    isRoot = false;
    isChild = false;
    isConst = false;
    isArray = false;
    isUnscoped = false;
    isInited = false;
    type = (char *)"undefined";
}

void Node::add(Node * n)
{
    if (!n) return;
    if (!sibling) sibling = n;
    else {
        Node * t = sibling;
        while (t->sibling) t = t->sibling;
        t->sibling = n;
    }
}

void Node::addChild(int c, Node * n)
{
    child[c] = n;
    if (n) {
        n->index = c;
        n->isChild = true;
    }
}

void Node::updateLoc()
{
    for (int i = 0; i < MAXCHILDREN; i++) if (child[i]) {
        child[i]->depth = depth + 1;
        child[i]->updateLoc();
    }

    if (sibling) {
        sibling->depth = depth;
        if (!isChild) sibling->index = index + 1;
        sibling->updateLoc();
    }
}

void Node::init(SymbolTable * tbl)
{
    for (int i = 0; i < MAXCHILDREN; i++) if (child[i]) child[i]->init(tbl);
    if (sibling) sibling->init(tbl);
}

void Node::sem(SymbolTable * tbl)
{
    semC(tbl);
    semS(tbl);
    if (sibling && sibling->isRoot) {
        Node * m = (Node *)tbl->lookupGlobal("main");
        if (!m || !((VarDecl *)m)->isFun || m->child[0])
            throwErr(NO_MAIN, NULL);
    }
}

void Node::semC(SymbolTable * tbl)
{
    for (int i = 0; i < MAXCHILDREN; i++) if (child[i]) child[i]->sem(tbl);
}

void Node::semS(SymbolTable * tbl)
{
    if (sibling) sibling->sem(tbl);
}

void Node::printSelf()
{
    if (isRoot) return; // root doesn't need special preamble
    for (int i = 0; i < depth; i++) printf(".   "); // print dots
    if (isChild) printf("Child: %d  ", index);
    else printf("Sibling: %d  ", index);
}

void Node::print(bool ann)
{
    for (int i = 0; i < MAXCHILDREN; i++)
        if (child[i]) child[i]->print(ann);
    if (sibling) sibling->print(ann);
}

VarDecl::VarDecl(TokenData * tkn) : Node()
{
    linenum = tkn->linenum;
    name = strdup(tkn->svalue);
    isFun = false;
    isStatic = false;
    arrSize = 0;
    usageFlg = false;
    usageWrnFlg = false;
}

VarDecl::VarDecl(TokenData * tkn, TokenData * arrTkn) : VarDecl(tkn)
{
    isArray = true;
    arrSize = arrTkn->nvalue;
}

void VarDecl::initS()
{
    isInited = true;
    if (sibling) ((VarDecl *)sibling)->initS();
}

void VarDecl::setType(char * varType, bool stat)
{
    type = strdup(varType);
    isStatic = stat;
    if (sibling) ((VarDecl *)sibling)->setType(varType, stat);
}

void VarDecl::init(SymbolTable * tbl)
{
    void * found = tbl->lookup(name);
    if (found) {
        VarDecl * v = (VarDecl *)found;
        v->isInited = true;
    }
}

void VarDecl::sem(SymbolTable * tbl)
{
    Node::semC(tbl);
    if (!tbl->insert(name, this)) throwErr(DECL_DBL, this, (Node *)tbl->lookup(name));
    Node::semS(tbl);
}

void VarDecl::print(bool ann)
{
    printSelf();
    printf(
        "Var: %s %sof type %s [line: %d]\n",
        name,
        //isStatic ? "static " : "",
        isArray ? "is array " : "",
        type, linenum
    );
    Node::print(ann);
}

FunDecl::FunDecl(TokenData * tkn, Node * prms, Node * stmt) : VarDecl(tkn)
{
    addChild(0, prms);
    addChild(1, stmt);
    if (stmt) stmt->isUnscoped = true;
    type = (char *)"void";
    isFun = true;
}

FunDecl::FunDecl(TokenData * retType, TokenData * tkn,
                              Node * prms, Node * stmt) : FunDecl(tkn, prms, stmt)
{
    type = strdup(retType->tokenstr);
}

void FunDecl::sem(SymbolTable * tbl)
{
    if (!tbl->insert(name, this)) throwErr(DECL_DBL, this, (Node *)tbl->lookup(name));
    tbl->enter("Function");
    Node::semC(tbl);
    tbl->applyToAll(wrnUnused);
    tbl->leave();
    Node::semS(tbl);
}

void FunDecl::print(bool ann)
{
    printSelf();
    printf( "Func: %s returns type %s [line: %d]\n",
                  name,           type,    linenum);
    Node::print(ann);
}

Parm::Parm(TokenData * tkn, bool arr) : VarDecl(tkn)
{
    isArray = arr;
    isInited = true;
}

void Parm::sem(SymbolTable * tbl)
{
    Node::semC(tbl);
    if (!tbl->insert(name, this)) throwErr(DECL_DBL, this, (Node *)tbl->lookup(name));
    Node::semS(tbl);
}

void Parm::print(bool ann)
{
    printSelf();
    printf(
        "Parm: %s %sof type %s [line: %d]\n",
        name,
        isArray ? "is array " : "",
        type, linenum
    );
    Node::print(ann);
}

CompoundStmt::CompoundStmt(int line, Node * decls, Node * stmt)
{
    linenum = line;
    addChild(0, decls->sibling);
    addChild(1, stmt->sibling);
}

void CompoundStmt::sem(SymbolTable * tbl)
{
    if (!isUnscoped) tbl->enter("Compound");
    Node::semC(tbl);
    if (!isUnscoped) {
        tbl->applyToAll(wrnUnused);
        tbl->leave();
    }
    Node::semS(tbl);
}

void CompoundStmt::print(bool ann)
{
    printSelf();
    printf("Compound [line: %d]\n", linenum);
    Node::print(ann);
}

IfStmt::IfStmt(int line, Node * cond, Node * stmt)
{
    linenum = line;
    addChild(0, cond);
    addChild(1, stmt);
}

IfStmt::IfStmt(int line, Node * cond, Node * stmt, Node * alt) : IfStmt(line, cond, stmt)
{
    addChild(2, alt);
}

void IfStmt::sem(SymbolTable * tbl)
{
    tbl->enter("If");
    if (child[0]) child[0]->sem(tbl);
    if (child[1]) child[1]->sem(tbl);
    if (child[2]) child[2]->sem(tbl);
    tbl->applyToAll(wrnUnused);
    tbl->leave();
    Node::semS(tbl);
}

void IfStmt::print(bool ann)
{
    printSelf();
    printf("If [line: %d]\n", linenum);
    Node::print(ann);
}

WhileStmt::WhileStmt(int line, Node * cond, Node * stmt)
{
    linenum = line;
    addChild(0, cond);
    addChild(1, stmt);
}

void WhileStmt::sem(SymbolTable * tbl)
{
    tbl->enter("While");
    Node::semC(tbl);
    tbl->applyToAll(wrnUnused);
    tbl->leave();
    Node::semS(tbl);
}

void WhileStmt::print(bool ann)
{
    printSelf();
    printf("While [line: %d]\n", linenum);
    Node::print(ann);
}

ForStmt::ForStmt(int line, TokenData * itr, Node * rng, Node * stmt)
{
    linenum = line;
    VarDecl * itrVar = new VarDecl(itr);
    itrVar->type = (char *)"int";
    itrVar->isInited = true;
    addChild(0, itrVar);
    addChild(1, rng);
    addChild(2, stmt);
}

void ForStmt::sem(SymbolTable * tbl)
{
    tbl->enter("For");
    doInitChecking = false;
    child[0]->sem(tbl);
    doInitChecking = true;
    child[1]->sem(tbl);
    if (child[2]) child[2]->sem(tbl);
    tbl->applyToAll(wrnUnused);
    tbl->leave();
    Node::semS(tbl);
}

void ForStmt::print(bool ann)
{
    printSelf();
    printf("For [line: %d]\n", linenum);
    Node::print(ann);
}

IterRange::IterRange(int line, Node * strt, Node * end)
{
    linenum = line;
    addChild(0, strt);
    addChild(1, end);
}

IterRange::IterRange(int line, Node * strt, Node * end, Node * jmp) : IterRange(line, strt, end)
{
    addChild(2, jmp);
}

void IterRange::sem(SymbolTable * tbl)
{
    Node::semC(tbl);
    //tbl->applyToAll(wrnUnused);
}

void IterRange::print(bool ann)
{
    printSelf();
    printf("Range [line: %d]\n", linenum);
    Node::print(ann);
}

ReturnStmt::ReturnStmt(int line)
{
    linenum = line;
}

ReturnStmt::ReturnStmt(int line, Node * retVal) : ReturnStmt(line)
{
    addChild(0, retVal);
}

void ReturnStmt::sem(SymbolTable * tbl)
{
    Node::semC(tbl);
    if (child[0] && child[0]->isArray) throwErr(ARR_RETURN, this);
    Node::semS(tbl);
}

void ReturnStmt::print(bool ann)
{
    printSelf();
    printf("Return [line: %d]\n", linenum);
    Node::print(ann);
}

BreakStmt::BreakStmt(int line)
{
    linenum = line;
}

void BreakStmt::sem(SymbolTable * tbl)
{
    Node::semC(tbl);
    Node::semS(tbl);
}

void BreakStmt::print(bool ann)
{
    printSelf();
    printf("Break [line: %d]\n", linenum);
    Node::print(ann);
}

Op::Op(TokenData * tkn, Node * lhs)
{
   addChild(0, lhs);
   linenum = tkn->linenum;
   opId = tkn->tokenclass;
   if      (!strcmp(tkn->tokenstr, "-")) opName = (char *)"chsign";
   else if (!strcmp(tkn->tokenstr, "*")) opName = (char *)"sizeof";
   else                             opName = strdup(tkn->tokenstr);
}

Op::Op(TokenData * tkn, Node * lhs, Node * rhs) : Op(tkn, lhs)
{
    addChild(1, rhs);
    opName = strdup(tkn->tokenstr);
}

void Op::sem(SymbolTable * tbl)
{
    Node::semC(tbl);
    switch (opId) {
        case AND:
        case OR:
            type = (char *)"bool";
            if (strcmp(child[0]->type, type)
             && strcmp(child[0]->type, (char *)"undefined"))
                throwErr(OP_LHS, this);
            if (strcmp(child[1]->type, type)
             && strcmp(child[1]->type, (char *)"undefined"))
                throwErr(OP_RHS, this);
            if (child[0]->isArray || child[1]->isArray) throwErr(OP_ARR, this);
            break;
        case EQ:
        case NEQ:
        case LESS:
        case LEQ:
        case GRTR:
        case GEQ:
            type = (char *)"bool";
            if (strcmp(child[0]->type, child[1]->type)
             && strcmp(child[0]->type, (char *)"undefined")
             && strcmp(child[1]->type, (char *)"undefined"))
                throwErr(OPERAND_MISMATCH, this);
            if ((child[0]->isArray && !child[1]->isArray)
             || (child[1]->isArray && !child[0]->isArray))
                throwErr(ARR_MISMATCH, this);
            break;
        case PLUS:
        case SLASH:
        case PERC:
            type = (char *)"int";
            if (strcmp(child[0]->type, type)
             && strcmp(child[0]->type, (char *)"undefined"))
                throwErr(OP_LHS, this);
            if (strcmp(child[1]->type, type)
             && strcmp(child[1]->type, (char *)"undefined"))
                throwErr(OP_RHS, this);
            if (child[0]->isArray || child[1]->isArray) throwErr(OP_ARR, this);
            break;
        case MINUS:
            type = (char *)"int";
            if (child[1]) {
                if (strcmp(child[0]->type, type)
                 && strcmp(child[0]->type, (char *)"undefined"))
                    throwErr(OP_LHS, this);
                if (strcmp(child[1]->type, type)
                 && strcmp(child[1]->type, (char *)"undefined"))
                    throwErr(OP_RHS, this);
                if (child[0]->isArray || child[1]->isArray) throwErr(OP_ARR, this);
            } else {
                if (strcmp(child[0]->type, type)
                 && strcmp(child[0]->type, (char *)"undefined"))
                    throwErr(OPERAND_UNARY, this);
                if (child[0]->isArray) throwErr(OP_ARR, this);
            }
            break;
        case ASTR:
            type = (char *)"int";
            if(child[1]) {
                if (strcmp(child[0]->type, type)
                 && strcmp(child[0]->type, (char *)"undefined"))
                    throwErr(OP_LHS, this);
                if (strcmp(child[1]->type, type)
                 && strcmp(child[1]->type, (char *)"undefined"))
                    throwErr(OP_RHS, this);
            } else {
                if (!child[0]->isArray
                 && strcmp(child[0]->type, (char *)"undefined"))
                    throwErr(OP_NOT_ARR, this);
            }
            break;
        case BRACKL:
            type = strdup(child[0]->type);
            if (!child[0]->isArray) throwErr(NON_ARR_INDEX, this);
            if (strcmp(child[1]->type, (char *)"int")) throwErr(ARR_NONINT_INDEX, this);
            if (child[1]->isArray) throwErr(ARR_UNINDEXED, this);
            break;
        case NOT:
            type = (char *)"bool";
            if (strcmp(child[0]->type, type)
             && strcmp(child[0]->type, (char *)"undefined"))
                throwErr(OPERAND_UNARY, this);
            if (child[0]->isArray) throwErr(OP_ARR, this);
            break;
        case QUEST:
            type = (char *)"int";
            if (strcmp(child[0]->type, type)
             && strcmp(child[0]->type, (char *)"undefined"))
                throwErr(OPERAND_UNARY, this);
            if (child[0]->isArray) throwErr(OP_ARR, this);
            break;
    }

    Node::semS(tbl);
}

void Op::print(bool ann)
{
    printSelf();
    printf("Op: %s%s%s [line: %d]\n",
           opName,
           ann ? " of type " : "",
           ann ? type : "",
           linenum);
    Node::print(ann);
}

Assign::Assign(TokenData * tkn, Node * lhs) : Op(tkn, lhs) {}

Assign::Assign(TokenData * tkn, Node * lhs, Node * rhs) : Op(tkn, lhs, rhs) {}

void Assign::sem(SymbolTable * tbl)
{
    switch (opId) {
        case ASS:
            doInitChecking = false;
            child[0]->sem(tbl);
            doInitChecking = true;
            child[1]->sem(tbl);
            type = strdup(child[0]->type);
            if (strcmp(child[0]->type, child[1]->type)
             && strcmp(child[0]->type, (char *)"undefined")
             && strcmp(child[1]->type, (char *)"undefined"))
                throwErr(OPERAND_MISMATCH, this);
            if ((child[0]->isArray && !child[1]->isArray)
             || (child[1]->isArray && !child[0]->isArray))
            throwErr(ARR_MISMATCH, this);
            child[0]->init(tbl);
            break;
        case ADDASS:
        case SUBASS:
        case MULASS:
        case DIVASS:
            type = (char *)"int";
            doInitChecking = false;
            child[0]->sem(tbl);
            doInitChecking = true;
            child[1]->sem(tbl);
            if (strcmp(child[0]->type, type)
             && strcmp(child[0]->type, (char *)"undefined"))
                throwErr(OP_LHS, this);
            if (strcmp(child[1]->type, type)
             && strcmp(child[1]->type, (char *)"undefined"))
                throwErr(OP_RHS, this);
            if (child[0]->isArray || child[1]->isArray) throwErr(OP_ARR, this);
            child[0]->init(tbl);
            break;
        case INC:
        case DEC:
            Node::semC(tbl);
            type = (char *)"int";
            if (strcmp(child[0]->type, type)
             && strcmp(child[0]->type, (char *)"undefined"))
                throwErr(OPERAND_UNARY, this);
            if (child[0]->isArray) throwErr(OP_ARR, this);
            child[0]->sem(tbl);
            break;
    }
    Node::semS(tbl);
}

void Assign::print(bool ann)
{
    printSelf();
    printf("Assign: %s%s%s [line: %d]\n",
           opName,
           ann ? " of type " : "", 
           ann ? type : "",
           linenum);
    Node::print(ann);
}

Id::Id(TokenData * tkn) : VarDecl(tkn) {}

void Id::sem(SymbolTable * tbl)
{
    void * found = tbl->lookup(name);
    if (!found) throwErr(DECL_NOT, this);
    else {
        VarDecl * v = (VarDecl *)found;
        if (v->isFun) throwErr(FUN_AS_VAR, this);
        else {
            isArray = v->isArray;
            if (isInited) v->isInited = true;
            type = strdup(v->type);
            isStatic = v->isStatic;
            v->usageFlg = true;
            if (doInitChecking && !v->isInited && !v->usageWrnFlg)
                throwWrn(VAR_UNINITED, this, v);
        }
    }
    Node::sem(tbl);
}

void Id::print(bool ann)
{
    printSelf();
    printf("Id: %s%s%s [line: %d]\n",
           name,
           ann ? " of type " : "",
           ann ? type : "",
           linenum);
    Node::print(ann);
}

Call::Call(TokenData * tkn, Node * args)
{
    addChild(0, args);
    linenum = tkn->linenum;
    type = (char *)"undefined";
    name = strdup(tkn->tokenstr);
}

void Call::sem(SymbolTable * tbl)
{
    void * found = tbl->lookup(name);
    if (!found) throwErr(DECL_NOT, this);
    else {
        VarDecl * v = (VarDecl *)found;
        if (!v->isFun) throwErr(VAR_AS_FUN, this);
        else {
            type = strdup(v->type);
            v->usageFlg = true;
        }
    }
    Node::sem(tbl);
}

void Call::print(bool ann)
{
    printSelf();
    printf("Call: %s%s%s [line: %d]\n",
           name,
           ann ? " of type " : "",
           ann ? type : "",
           linenum);
    Node::print(ann);
}

Const::Const(TokenData * tkn)
{
    linenum = tkn->linenum;
    isConst = true;
    token = tkn;
    switch (token->tokenclass) {
        case BOOLCONST:   type = (char *)"bool"; break;
        case NUMCONST:    type = (char *)"int";  break;
        case CHARCONST:   type = (char *)"char"; break;
        case STRINGCONST: type = (char *)"char"; isArray = true; break;
    }
}

void Const::sem(SymbolTable * tbl)
{
    Node::semC(tbl);



    Node::semS(tbl);
}

void Const::print(bool ann)
{
    printSelf();
    printf("Const ");
    switch (token->tokenclass) {
        case BOOLCONST:   printf(token->nvalue ? "true" : "false"); break;
        case NUMCONST:    printf("%d", token->nvalue);              break;
        case CHARCONST:   printf("'%c'", token->cvalue);            break;
        case STRINGCONST: printf("is array \"%s\"", token->svalue);       break; 
    }
    printf("%s%s [line: %d]\n",
           ann ? " of type " : "",
           ann ? type : "",
           linenum);
    Node::print(ann);
}

