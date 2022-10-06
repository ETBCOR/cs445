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
}

void Node::semC(SymbolTable * tbl)
{
    for (int i = 0; i < MAXCHILDREN; i++) if (child[i]) {
        child[i]->sem(tbl);
    }
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

void Node::print()
{
    for (int i = 0; i < MAXCHILDREN; i++)
        if (child[i]) child[i]->print();
    if (sibling) sibling->print();
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
    if (!tbl->insert(name, this)) throwErr(DECL_DBL, this);
    Node::semS(tbl);
}

void VarDecl::print()
{
    printSelf();
    printf(
        "Var: %s %sof type %s [line: %d]\n",
        name,
        //isStatic ? "static " : "",
        isArray ? "is array " : "",
        type, linenum
    );
    Node::print();
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
    if (!tbl->insert(name, this)) throwErr(DECL_DBL, this);
    tbl->enter("Function");
    Node::semC(tbl);
    tbl->applyToAll(wrnUnused);
    tbl->leave();
    Node::semS(tbl);
}

void FunDecl::print()
{
    printSelf();
    printf( "Func: %s returns type %s [line: %d]\n",
                  name,           type,    linenum);
    Node::print();
}

Parm::Parm(TokenData * tkn, bool arr) : VarDecl(tkn)
{
    isArray = arr;
    isInited = true;
}

void Parm::sem(SymbolTable * tbl)
{
    Node::semC(tbl);
    if (!tbl->insert(name, this)) throwErr(DECL_DBL, this);
    Node::semS(tbl);
}

void Parm::print()
{
    printSelf();
    printf(
        "Parm: %s %sof type %s [line: %d]\n",
        name,
        isArray ? "is array " : "",
        type, linenum
    );
    Node::print();
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

void CompoundStmt::print()
{
    printSelf();
    printf("Compound [line: %d]\n", linenum);
    Node::print();
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
    Node::semC(tbl);

    tbl->leave();
    Node::semS(tbl);
}

void IfStmt::print()
{
    printSelf();
    printf("If [line: %d]\n", linenum);
    Node::print();
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

void WhileStmt::print()
{
    printSelf();
    printf("While [line: %d]\n", linenum);
    Node::print();
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
    Node::semC(tbl);
    tbl->applyToAll(wrnUnused);
    tbl->leave();
    Node::semS(tbl);
}

void ForStmt::print()
{
    printSelf();
    printf("For [line: %d]\n", linenum);
    Node::print();
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
    Node::semS(tbl);
}

void IterRange::print()
{
    printSelf();
    printf("Range [line: %d]\n", linenum);
    Node::print();
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
    Node::semS(tbl);
}

void ReturnStmt::print()
{
    printSelf();
    printf("Return [line: %d]\n", linenum);
    Node::print();
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

void BreakStmt::print()
{
    printSelf();
    printf("Break [line: %d]\n", linenum);
    Node::print();
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
            break;
        case OR:
            break;
        case EQ:
            break;
        case NEQ:
            break;
        case LESS:
            break;
        case LEQ:
            break;
        case GRTR:
            break;
        case GEQ:
            break;
        case ASS:
            break;
        case ADDASS:
            break;
        case SUBASS:
            break;
        case MULASS:
            break;
        case DIVASS:
            break;
        case PLUS:
            break;
        case MINUS:
            break;
        case ASTR:
            break;
        case SLASH:
            break;
        case PERC:
            break;
        case BRACKL:
            break;
        case DEC:
            break;
        case INC:
            break;
        case NOT:
            break;
        case QUEST:
            break;
    }

    Node::semS(tbl);
}

void Op::print()
{
    printSelf();
    printf( "Op: %s [line: %d]\n",
               opName,   linenum);
    Node::print();
}

Assign::Assign(TokenData * tkn, Node * lhs) : Op(tkn, lhs) {}

Assign::Assign(TokenData * tkn, Node * lhs, Node * rhs) : Op(tkn, lhs, rhs) {}

void Assign::sem(SymbolTable * tbl)
{
    doInitChecking = false;
    child[0]->sem(tbl);
    doInitChecking = true;
    if(child[1]) child[1]->sem(tbl);

    child[0]->init(tbl);
    Node::semS(tbl);
}

void Assign::print()
{
    printSelf();
    printf( "Assign: %s [line: %d]\n",
                   opName,   linenum);
    Node::print();
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
            type = strdup(v->type);
            isArray = v->isArray;
            if (isInited) v->isInited = true;
            isFun = v->isFun;
            isStatic = v->isStatic;
            v->usageFlg = true;
            if (doInitChecking && !v->isInited && !v->usageWrnFlg) throwWrn(VAR_UNINITED, this);
        }
    }
    Node::sem(tbl);
}

void Id::print()
{
    printSelf();
    printf( "Id: %s [line: %d]\n",
                name,    linenum);
    Node::print();
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
    Node::semC(tbl);



    Node::semS(tbl);
}

void Call::print()
{
    printSelf();
    printf( "Call: %s [line: %d]\n",
                  name,    linenum);
    Node::print();
}

Const::Const(TokenData * tkn)
{
    linenum = tkn->linenum;
    isConst = true;
    token = tkn;
    switch (token->tokenclass) {
        case BOOLCONST:   type = (char *)"bool"; break;
        case NUMCONST:    type = (char *)"int";  break;
        case CHARCONST:   type = (char *)"char";
        case STRINGCONST: isArray = true;        break;
    }
}

void Const::sem(SymbolTable * tbl)
{
    Node::semC(tbl);



    Node::semS(tbl);
}

void Const::print()
{
    printSelf();
    printf("Const ");
    switch (token->tokenclass) {
        case BOOLCONST:   printf(token->nvalue ? "true" : "false"); break;
        case NUMCONST:    printf("%d", token->nvalue);              break;
        case CHARCONST:   printf("'%c'", token->cvalue);            break;
        case STRINGCONST: printf("is array \"%s\"", token->svalue);       break; 
    }
    printf(" [line: %d]\n", linenum);
    Node::print();
}

