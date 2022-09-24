#include "node.h"
#include <string.h>

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
    if (child) {
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
        if (!isChild) sibling->index = index + 1;
        sibling->updateLoc();
    }
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
    isInited = false;
    arrSize = 0;
}

VarDecl::VarDecl(TokenData * tkn, TokenData * arrTkn) : VarDecl(tkn)
{
    isArray = true;
    arrSize = arrTkn->nvalue;
}

void VarDecl::setType(char * varType, bool stat)
{
    type = strdup(varType);
    isStatic = stat;
    if (sibling) ((VarDecl *)sibling)->setType(varType, stat);
}

void VarDecl::init()
{
    isInited = true;
    if (sibling) ((VarDecl *)sibling)->init();
}

void VarDecl::print()
{
    printSelf();
    printf(
        "Var %s: %s%stype %s [line: %d]\n",
        name,
        isStatic ? "static " : "",
        isArray ? "array of " : "",
        type, linenum
    );
    Node::print();
}

FunDecl::FunDecl(TokenData * tkn, Node * prms, Node * stmt) : VarDecl(tkn)
{
    addChild(0, prms);
    addChild(1, stmt);
    type = (char *)"void";
    isFun = true;
}

FunDecl::FunDecl(TokenData * retType, TokenData * tkn,
                              Node * prms, Node * stmt) : FunDecl(tkn, prms, stmt)
{
    type = strdup(retType->tokenstr);
}

void FunDecl::print()
{
    printSelf();
    printf( "Func %s: returns type %s [line: %d]\n",
                 name,            type,    linenum);
    Node::print();
}

Parm::Parm(TokenData * tkn, bool arr) : VarDecl(tkn)
{
    isArray = arr;
    isInited = true;
}

void Parm::print()
{
    printSelf();
    printf(
        "Parm %s: %stype %s [line: %d]\n",
        name,
        isArray ? "array of " : "",
        type, linenum
    );
    Node::print();
}

CompoundStmt::CompoundStmt(int line, Node * decls, Node * stmt)
{
    linenum = line;
    addChild(0, decls/*->sibling*/);
    addChild(1, stmt/*->sibling*/);
}

void CompoundStmt::print()
{
    printSelf();
    printf("Compound [line %d]\n", linenum);
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

void IfStmt::print()
{
    printSelf();
    printf("If [line: %d]\n");
    Node::print();
}

WhileStmt::WhileStmt(int line, Node * cond, Node * stmt)
{
    linenum = line;
    addChild(0, cond);
    addChild(1, stmt);
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
    itrVar->isInited = true;
    addChild(0, itrVar);
    addChild(1, rng);
    addChild(2, stmt);
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

void ReturnStmt::print()
{
    printSelf();
    printf("Return [line %d]\n", linenum);
    Node::print();
}

BreakStmt::BreakStmt(int line)
{
    linenum = line;
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
   opName = strdup(tkn->tokenstr);
   opId = tkn->tokenclass;
}

Op::Op(TokenData * tkn, Node * lhs, Node * rhs) : Op(tkn, lhs)
{
    addChild(1, rhs);
}

void Op::print()
{
    printSelf();
    bool def = strcmp(type, "undefined");
    printf(
        "Op %s : %stype %s%s[line: %d]\n",
        opName,
        def ? "" : "undefined ",
        def ? type : "",
        def ? (char *)" " : "",
        linenum
    );
    Node::print();
}

Assign::Assign(TokenData * tkn, Node * lhs) : Op(tkn, lhs) {}

Assign::Assign(TokenData * tkn, Node * lhs, Node * rhs) : Op(tkn, lhs, rhs) {}

void Assign::print()
{
    printSelf();
    bool def = strcmp(type, "undefined");
    printf(
        "Assign %s : %s%stype %s%s[line: %d]\n",
        opName,
        def ? "" : "undefined ",
        isArray ? "array of" : "",
        def ? type : "",
        def ? (char *)" " : "",
        linenum
    );
    Node::print();
}

Id::Id(TokenData * tkn) : VarDecl(tkn) {}

void Id::print()
{
    printSelf();
    printf(
        "Id %s: %s%s%stype %s [line: %d]\n",
        name,
        isStatic ? "static " : "",
        isArray ? "array of " : "",
        strcmp(type, "undefined") ? "" : "undefined",
        type, linenum
    );
    Node::print();
}

Call::Call(TokenData * tkn, Node * args)
{
    addChild(0, args);
    linenum = tkn->linenum;
    type = (char *)"undefined";
    name = strdup(tkn->tokenstr);
}

void Call::print()
{
    printSelf();
    bool def = strcmp(type, "undefined");
    printf(
        "Call %s: %stype %s%s[line: %d]\n",
        name,
        def ? "" : "undefined ",
        def ? type : "",
        def ? (char *)" " : "",
        linenum
    );
    Node::print();
}

Const::Const(TokenData * tkn)
{
    isConst = true;
    token = tkn;
    switch(token->tokenclass) {
        case BOOLCONST:   type = (char *)"bool"; break;
        case NUMCONST:    type = (char *)"int";  break;
        case CHARCONST:   type = (char *)"char";
        case STRINGCONST: isArray = true;        break;
    }
}

void Const::print()
{
    printSelf();
    printf("Const ");
    switch(token->tokenclass) {
        case BOOLCONST:   printf(token->nvalue ? "true" : "false"); break;
        case NUMCONST:    printf("%d", token->nvalue);              break;
        case CHARCONST:   printf("'%c'", token->cvalue);            break;
        case STRINGCONST: printf("\"%s\"", token->svalue);          break; 
    }
    printf(" [line: %d]\n", linenum);
    Node::print();
}

