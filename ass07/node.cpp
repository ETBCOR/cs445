#include "node.h"

static bool doInitChecking = true;
static FunDecl * curFun = NULL;
static bool returned = false;
static int loopDepth = 0;
static char * memstr;
static bool libGenDone = false;
static bool printExpComment = false;
static bool genStatics = false;
static std::vector<VarDecl *> localStatics;

static std::map<int , char *> opIdMap;

static void initOpIdMap ()
{
    opIdMap[AND]    = (char *)"AND";
    opIdMap[OR]     = (char *)"OR";
    opIdMap[EQ]     = (char *)"TEQ";
    opIdMap[NEQ]    = (char *)"TNE";
    opIdMap[LESS]   = (char *)"TLT";
    opIdMap[LEQ]    = (char *)"TLE";
    opIdMap[GRTR]   = (char *)"TGT";
    opIdMap[GEQ]    = (char *)"TGE";
    opIdMap[PLUS]   = (char *)"ADD";
    opIdMap[SLASH]  = (char *)"DIV";
    opIdMap[PERC]   = (char *)"MOD";
    opIdMap[MINUS]  = (char *)"SUB";
    opIdMap[ASTR]   = (char *)"MUL";
    opIdMap[BRACKL] = (char *)"";
    opIdMap[NOT]    = (char *)"";
    opIdMap[QUEST]  = (char *)"RND";

    opIdMap[ASS]    = (char *)"";
    opIdMap[ADDASS] = (char *)"ADD";
    opIdMap[SUBASS] = (char *)"SUB";
    opIdMap[MULASS] = (char *)"MUL";
    opIdMap[DIVASS] = (char *)"DIV";
    
}

static char * getOpIString(int opId)
{
    if (opIdMap.find(opId) == opIdMap.end()) {
        printf("ERROR(SYSTEM): getOpIString fails to find op %d\n", opId); 
        fflush(stdout);
        exit(1);
    }
    return opIdMap[opId];
}

static TokenData * buildTkn(char *);

Node::Node()
{
    sibling = NULL;
    semDone = false;
    linenum = -1;
    depth = 0;
    index = 1;
    size = 0;
    isRoot = false;
    isChild = false;
    isConst = false;
    isArray = false;
    isParm = false;
    isUnscoped = false;
    isInited = false;
    isGlobal = false;
    type = (char *)"undefined";
    memType = (char *)"None";
}

Node::Node(TokenData * tkn) : Node()
{
    token = tkn;
}

void Node::add(Node * n)
{
    if (!n)
        return;
    if (!sibling)
        sibling = n;
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
if (!semDone) {
    if (isRoot) {
        // add I/O functions to the table
        FunDecl * IO[7];
        Parm * pp[3];
        for (int i = 0; i < 3; i++) {
            pp[i] = new Parm(buildTkn((char *)"*dummy*"), false);
            pp[i]->isDummy = true;
        }

        // int input()
        IO[0] = new FunDecl(buildTkn((char *)"int"), buildTkn((char *)"input"), NULL, NULL);
        IO[0]->isRoot = true;
        // void output(int)
        pp[0]->type = (char *)"int";
        IO[1] = new FunDecl(buildTkn((char *)"output"), pp[0], NULL);
        // bool inputb()
        IO[2] = new FunDecl(buildTkn((char *)"bool"), buildTkn((char *)"inputb"), NULL, NULL);
        // void outputb(bool)
        pp[1]->type = (char *)"bool";
        IO[3] = new FunDecl(buildTkn((char *)"outputb"), pp[1], NULL);
        // char inputc()
        IO[4] = new FunDecl(buildTkn((char *)"char"), buildTkn((char *)"inputc"), NULL, NULL);
        // void outputc(char)
        pp[2]->type = (char *)"char";
        IO[5] = new FunDecl(buildTkn((char *)"outputc"), pp[2], NULL);
        // void outnl()
        IO[6] = new FunDecl(buildTkn((char *)"outnl"), NULL, NULL);

        VarDecl * firstDeclPtr = (VarDecl *)sibling;
        sibling = NULL;
        for (int i = 0; i < 7; i++) {
            IO[i]->isDummy = true;
            add(IO[i]);
        }
        IO[6]->add(firstDeclPtr);


        // perform semantic analysis
        updateLoc();
        semC(tbl);
        semS(tbl);


        // preform global semantic analysis
        VarDecl * m = (VarDecl *)tbl->lookupGlobal("main");
        bool validMain = (m && m->isFun && !m->child[0]);  // was main() defined?
        if (m && m->isFun)
            m->usageFlg = true;
        tbl->applyToAll(wrnUnused); // issue global scope usage warnings
        if (!validMain)
            throwErr(NO_MAIN, NULL);
    } else {
        semC(tbl);
        semS(tbl);
    }
    semDone = true;
}
}

void Node::semC(SymbolTable * tbl)
{
    for (int i = 0; i < MAXCHILDREN; i++)
        if (child[i])
            child[i]->sem(tbl);
}

void Node::semS(SymbolTable * tbl)
{
    if (sibling)
        sibling->sem(tbl);
}

void Node::gen(SymbolTable * tbl)
{
    gen(tbl, true);
}

void Node::gen(SymbolTable * tbl, bool doS)
{
    if (isRoot) {
        initOpIdMap();
        emitComment((char *)"C- compiler version", (char *)"07-20");
        emitComment((char *)"Built:", (char *)__TIMESTAMP__);
        emitComment((char *)"Author: Ethan Corgatelli");
        emitComment((char *)"File compiled:", basename(src));
        int frstInstr = emitSkip(1);

        for (VarDecl * v = (VarDecl *)sibling; v; v = (VarDecl *)v->sibling)
            if (v->isFun)
                v->gen(tbl, false);

        backPatchAJumpToHere(frstInstr, (char *)"Jump to init [backpatch]");
        emitComment((char *)"INIT");
        emitRM((char *)"LDA",1,gofs,0,(char *)"set first frame at end of globals");
        emitRM((char *)"ST", 1,0,1,(char *)"store old fp (point to self)");
        emitComment((char *)"INIT GLOBALS AND STATICS");

        genStatics = true;
        for (VarDecl * v = (VarDecl *)sibling; v; v = (VarDecl *)v->sibling)
            if (!v->isFun)
                v->gen(tbl, false);
        for (VarDecl * i : localStatics)
            i->gen(tbl, false);
        genStatics = false;

        emitComment((char *)"END INIT GLOBALS AND STATICS");
        emitRM((char *)"LDA",3,1,7,(char *)"Return address in ac");
        int mainLoc = ((FunDecl *)tbl->lookupGlobal((char *)"main"))->addr - emitSkip(0) - 1;
        emitRM((char *)"JMP",7,mainLoc,7,(char *)"Jump to main");
        emitRO((char *)"HALT",0,0,0,(char *)"DONE!");
        emitComment((char *)"END INIT");
        libGenDone = true;
    } else {
        genC(tbl);
        if (doS)
            genS(tbl);
    }
}

void Node::genC(SymbolTable * tbl)
{
    for (int i = 0; i < MAXCHILDREN; i++)
        if (child[i])
            child[i]->gen(tbl);
}

void Node::genS(SymbolTable * tbl)
{
    if (sibling)
        sibling->gen(tbl);
}

void Node::printSelf()
{
    if (isRoot)
        return;
    for (int i = 0; i < depth; i++)
        printf(".   ");
    if (isChild)
        printf("Child: %d  ", index);
    else
        printf("Sibling: %d  ", index);
}

void Node::print(bool ann, bool mem)
{
    for (int i = 0; i < MAXCHILDREN; i++)
        if (child[i])
            child[i]->print(ann, mem);
    if (sibling)
        sibling->print(ann, mem);
}

VarDecl::VarDecl(TokenData * tkn) : Node()
{
    token = tkn;
    linenum = tkn->linenum;
    name = strdup(tkn->tokenstr);
    isFun = false;
    isStatic = false;
    isDummy = false;
    arrSize = 0;
    usageFlg = false;
    usageWrnFlg = false;
    memSize = 1;
}

VarDecl::VarDecl(TokenData * tkn, TokenData * arrTkn) : VarDecl(tkn)
{
    isArray = true;
    arrSize = arrTkn->nvalue;
    memSize += arrSize;
}

void VarDecl::initS()
{
    isInited = true;
    if (sibling)
        ((VarDecl *)sibling)->initS();
}

void VarDecl::setType(char * varType, bool stat)
{
    type = strdup(varType);
    isStatic = stat;
    if (sibling)
        ((VarDecl *)sibling)->setType(varType, stat);
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
if (!semDone) {
    Node::semC(tbl);
    bool inserted = tbl->insert(name, this);
    if (!inserted)
        throwErr(DECL_DBL, this, (Node *)tbl->lookup(name));
    if (child[0]) {
        if (!child[0]->isConst)
            throwErr(INIT_NONCONST, this);
        if (strcmp(child[0]->type, type)
         && strcmp(child[0]->type, (char *)"undefined"))
            throwErr(INIT_MISMATCH, this, child[0]);
        if ((isArray && !child[0]->isArray)
         || (!isArray && child[0]->isArray))
            throwErr(INIT_ARR_MISMATCH, this, child[0]);
    }

    bool natGlob = (tbl->depth() == 1);
    if (isStatic && natGlob)
        memType = (char *)"Static";
    else if (isStatic)
        memType = (char *)"LocalStatic";
    else if (natGlob)
        memType = (char *)"Global";
    else
        memType = (char *)"Local";

    if (isStatic || natGlob) {
        memLoc = gofs - isArray;
        gofs -= memSize;
        isGlobal = true;
    } else {
        memLoc = fofs - isArray;
        fofs -= memSize;
    }

    Node::semS(tbl);
    semDone = true;
}
}

void VarDecl::gen(SymbolTable * tbl, bool doS)
{
    if (!isStatic || genStatics)
        tofs -= 1;
    if (isArray) {
        emitRM((char *)"LDC", 3, arrSize, 6, (char *)"load size of array",name);
        emitRM((char *)"ST", 3, memLoc + 1, !isGlobal, (char *)"save size of array",name);
    }
    if (child[0]) {
        if (!isStatic || genStatics)
            child[0]->gen(tbl);
        if (((Const *)child[0])->isStrConst && child[0]->isArray) {
            emitRM((char *)"LDA",3,child[0]->memLoc,0,(char *)"Load address of char array");
            emitRM((char *)"LDA",4,memLoc,1,(char *)"address of lhs");
            emitRM((char *)"LD",5,1,3,(char *)"size of rhs");
            emitRM((char *)"LD",6,1,4,(char *)"size of lhs");
            emitRO((char *)"SWP",5,6,6,(char *)"pick smallest size");
            emitRO((char *)"MOV",4,3,5,(char *)"array op =");
        } else if (!isStatic || genStatics)
            emitRM((char *)"ST",3,memLoc,!isGlobal,(char *)"Store variable",name);
        else
            localStatics.push_back((VarDecl *)this);
    }
    if (doS)
        Node::genS(tbl);
}

void VarDecl::print(bool ann, bool mem)
{
    if (mem) {
        memstr = (char *)malloc(64);
        sprintf(memstr, "[mem: %s loc: %d size: %d] ", memType, memLoc, memSize);
    }
    printSelf();
    printf(
        "Var: %s of %s%stype %s %s[line: %d]\n",
        name,
        isStatic ? "static " : "",
        isArray ? "array of " : "",
        type,
        mem ? memstr : "",
        linenum
    );
    Node::print(ann, mem);
}

FunDecl::FunDecl(TokenData * tkn, Node * prms, Node * stmt) : VarDecl(tkn)
{
    addChild(0, prms);
    addChild(1, stmt);
    type = (char *)"void";
    isFun = true;
}

FunDecl::FunDecl(TokenData * retType, TokenData * tkn, Node * prms, Node * stmt) : FunDecl(tkn, prms, stmt)
{
    type = strdup(retType->tokenstr);
}

void FunDecl::sem(SymbolTable * tbl)
{
if (!semDone) {
    if (!tbl->insert(name, this))
        throwErr(DECL_DBL, this, (Node *)tbl->lookup(name));
    tbl->enter("Function");
    curFun = this;
    returned = false;
    memType = (char *)"Global";
    fofs -= 2;
    memLoc = 0;
    if (child[0])
        child[0]->sem(tbl);
    memSize = fofs;
    if (child[1])
        child[1]->sem(tbl);
    if (!isDummy && !returned && strcmp(type, (char *)"void"))
        throwWrn(NO_RETURN, this);
    tbl->applyToAll(wrnUnused);
    size = tbl->leave();
    Node::semS(tbl);
    semDone = true;
}
}

void FunDecl::gen(SymbolTable * tbl, bool doS)
{
    emitComment((char *)"");
    emitComment();
    emitComment((char *)"FUNCTION",name);
    addr = emitSkip(0);
    if (isDummy) {
        tofs = size;
        emitRM((char *)"ST", 3, -1, 1, (char *)"Store return address");
        if (name[0] == 'i') {
            if (strlen(name) == 5)
                emitRO((char *)"IN",2,2,2,(char *)"Grab int input");
            else if (name[5] == 'b')
                emitRO((char *)"INB",2,2,2,(char *)"Grab bool input");
            else if (name[5] == 'c')
                emitRO((char *)"INC",2,2,2,(char *)"Grab char input");
        } else if (name[0] == 'o') {
            if (strlen(name) == 5)
                emitRO((char *)"OUTNL",3,3,3,(char *)"Output a newline");
            else {
                emitRM((char *)"LD", 3, -2, 1, (char *)"Load parameter");
                if (strlen(name) == 6)
                    emitRO((char *)"OUT",3,3,3,(char *)"Output integer");
                else if (name[6] == 'b')
                    emitRO((char *)"OUTB",3,3,3,(char *)"Output bool");
                else if (name[6] == 'c')
                    emitRO((char *)"OUTC",3,3,3,(char *)"Output char");
            }
        }
    } else {
        // not a dummy function
        emitComment((char *)"TOFF set:", tofs = size);
        emitRM((char *)"ST",3,-1,1,(char *)"Store return address");
        if (child[1])
            child[1]->gen(tbl, true);
        emitComment((char *)"Add standard closing in case there is no return statement");
        emitRM((char *)"LDC",2,0,6,(char *)"Set return value to 0");
    }
    emitRM((char *)"LD",3,-1,1,(char *)"Load return address");
    emitRM((char *)"LD",1,0,1,(char *)"Adjust fp");
    emitRM((char *)"JMP",7,0,3,(char *)"Return");
    emitComment((char *)"END FUNCTION",name);
    if (doS)
        Node::genS(tbl);
}

void FunDecl::print(bool ann, bool mem)
{
    if (mem) {
        memstr = (char *)malloc(64);
        sprintf(memstr, "[mem: %s loc: %d size: %d] ", memType, memLoc, memSize);
    }
    printSelf();
    printf("Func: %s returns type %s %s[line: %d]\n",
               name, type, mem ? memstr : "", linenum);
    Node::print(ann, mem);
}

Parm::Parm(TokenData * tkn, bool arr) : VarDecl(tkn)
{
    isArray = arr;
    isParm = true;
    isInited = true;
}

void Parm::sem(SymbolTable * tbl)
{
if (!semDone) {
    Node::semC(tbl);
    if (!tbl->insert(name, this))
        throwErr(DECL_DBL, this, (Node *)tbl->lookup(name));

    memType = (char *)"Parameter";
    memLoc = fofs;
    fofs -= memSize;
    Node::semS(tbl);
    semDone = true;
}
}

void Parm::gen(SymbolTable * tbl, bool doS)
{
    Node::genC(tbl);
    if (doS)
        Node::genS(tbl);
}

void Parm::print(bool ann, bool mem)
{
    if (mem) {
        memstr = (char *)malloc(64);
        sprintf(memstr, "[mem: %s loc: %d size: %d] ", memType, memLoc, memSize);
    }
    printSelf();
    printf(
        "Parm: %s %sof type %s %s[line: %d]\n",
        name,
        isArray ? "of array " : "",
        type,
        mem ? memstr : "",
        linenum
    );
    Node::print(ann, mem);
}

CompoundStmt::CompoundStmt(int line, Node * decls, Node * stmt)
{
    linenum = line;
    addChild(0, decls->sibling);
    addChild(1, stmt->sibling);
}

void CompoundStmt::sem(SymbolTable * tbl)
{
if (!semDone) {
    if (!isUnscoped)
        tbl->enter("Compound");
    Node::semC(tbl);
    if (!isUnscoped)
        tbl->applyToAll(wrnUnused);
    memSize = fofs;
    if (!isUnscoped)
        size = tbl->leave();
    Node::semS(tbl);
    semDone = true;
}
}

void CompoundStmt::gen(SymbolTable * tbl, bool doS)
{
    int tofst = tofs;
    if (!isUnscoped) {
        tofs = memSize;
    }
    emitComment((char *)"COMPOUND");
    emitComment((char *)"TOFF set:", tofs);
    if (child[0])
        child[0]->gen(tbl);
    emitComment((char *)"Compound Body");
    for (Node * i = child[1]; i; i = i->sibling) {
        printExpComment = true;
        i->gen(tbl, false);
    }
    emitComment((char *)"TOFF set:", tofs = tofst);
    emitComment((char *)"END COMPOUND");
    if (doS)
        Node::genS(tbl);
}

void CompoundStmt::print(bool ann, bool mem)
{
    if (mem) {
        memstr = (char *)malloc(64);
        sprintf(memstr, "[mem: %s loc: %d size: %d] ", memType, memLoc, memSize);
    }
    printSelf();
    printf(
        "Compound %s[line: %d]\n",
        mem ? memstr : "", linenum
    );
    Node::print(ann, mem);
}

IfStmt::IfStmt(TokenData * tkn, Node * cond, Node * stmt)
{
    token = tkn;
    linenum = tkn->linenum;
    addChild(0, cond);
    addChild(1, stmt);
}

IfStmt::IfStmt(TokenData * tkn, Node * cond, Node * stmt, Node * alt) : IfStmt(tkn, cond, stmt)
{
    addChild(2, alt);
}

void IfStmt::sem(SymbolTable * tbl)
{
if (!semDone) {
    tbl->enter("If");
    if (child[0]) {
        child[0]->sem(tbl);
        if (strcmp(child[0]->type, (char *)"bool")
         && strcmp(child[0]->type, (char *)"undefined"))
            throwErr(NONBOOL_COND, this, child[0]);
        if (child[0]->isArray)
            throwErr(ARR_COND, this);
    }
    if (child[1])
        child[1]->sem(tbl);
    if (child[2])
        child[2]->sem(tbl);
    tbl->applyToAll(wrnUnused);
    size = tbl->leave();
    Node::semS(tbl);
    semDone = true;
}
}

void IfStmt::gen(SymbolTable * tbl, bool doS)
{
    int tofst = tofs, ifLoc, elseLoc;
    emitComment((char *)"IF");
    tofs = size;
    printExpComment = false;
    child[0]->gen(tbl);
    ifLoc = emitSkip(1);
    emitComment((char *)"THEN");
    printExpComment = true;
    if(child[1])
        child[1]->gen(tbl);
    if(child[2]) {
        elseLoc = emitSkip(1);
        backPatchAJumpToHere((char *)"JZR",3,ifLoc,(char *)"Jump around the THEN if false [backpatch]");
        emitComment((char *)"ELSE");
        child[2]->gen(tbl);
        backPatchAJumpToHere(elseLoc,(char *)"Jump past if statement [backpatch]");
    } else
        backPatchAJumpToHere((char *)"JZR",3,ifLoc,(char *)"Jump around the THEN if false [backpatch]");
    emitComment((char *)"END IF");
    tofs = tofst;
    if (doS)
        Node::genS(tbl);
}

void IfStmt::print(bool ann, bool mem)
{
    printSelf();
    printf("If [line: %d]\n", linenum);
    Node::print(ann, mem);
}

WhileStmt::WhileStmt(TokenData * tkn, Node * cond, Node * stmt)
{
    token = tkn;
    linenum = tkn->linenum;
    addChild(0, cond);
    addChild(1, stmt);
}

void WhileStmt::sem(SymbolTable * tbl)
{
if (!semDone) {
    tbl->enter("While");
    loopDepth++;
    if (child[0]) {
        child[0]->sem(tbl);
        if (strcmp(child[0]->type, (char *)"bool")
         && strcmp(child[0]->type, (char *)"undefined"))
            throwErr(NONBOOL_COND, this, child[0]);
        if (child[0]->isArray)
            throwErr(ARR_COND, this);
    }
    if (child[1])
        child[1]->sem(tbl);
    tbl->applyToAll(wrnUnused);
    size = tbl->leave();
    loopDepth--;
    Node::semS(tbl);
    semDone = true;
}
}

void WhileStmt::gen(SymbolTable * tbl, bool doS)
{
    emitComment((char *)"WHILE");
    int loopLoc = emitSkip(0);
    printExpComment = false;
    child[0]->gen(tbl);
    emitRM((char *)"JNZ",3,1,7,(char *)"Jump to while part");
    int breakLocT = breakLoc;
    breakLoc = emitSkip(1);
    emitComment((char *)"DO");
    if (child[1])
        child[1]->gen(tbl);
    emitGotoAbs(loopLoc, (char *)"go to beginning of loop");
    backPatchAJumpToHere(breakLoc, (char *)"Jump past loop [backpatch]");
    breakLoc = breakLocT;
    emitComment((char *)"END WHILE");
    if (doS)
        Node::genS(tbl);
}

void WhileStmt::print(bool ann, bool mem)
{
    printSelf();
    printf("While [line: %d]\n", linenum);
    Node::print(ann, mem);
}

ForStmt::ForStmt(TokenData * tkn, TokenData * itr, Node * rng, Node * stmt)
{
    token = tkn;
    linenum = tkn->linenum;
    VarDecl * itrVar = new VarDecl(itr);
    itrVar->type = (char *)"int";
    itrVar->isInited = true;
    if (stmt)
        stmt->isUnscoped = true;
    addChild(0, itrVar);
    addChild(1, rng);
    addChild(2, stmt);
}

void ForStmt::sem(SymbolTable * tbl)
{
if (!semDone) {
    tbl->enter("For");
    loopDepth++;
    memLoc = 0;
    child[0]->sem(tbl);
    child[1]->sem(tbl);
    memSize = fofs;
    child[2]->sem(tbl);
    tbl->applyToAll(wrnUnused);
    size = tbl->leave();
    loopDepth--;
    Node::semS(tbl);
    semDone = true;
}
}

void ForStmt::gen(SymbolTable * tbl, bool doS)
{
    child[0]->gen(tbl); // iter var
    emitComment((char *)"TOFF set:",tofs = memSize);
    emitComment((char *)"FOR");
    // range
    child[1]->child[0]->gen(tbl);
    emitRM((char *)"ST",3,tofs+3,1,(char *)"save starting value in index variable");
    printExpComment = false;
    child[1]->child[1]->gen(tbl);
    emitRM((char *)"ST",3,tofs+2,1,(char *)"save stop value");
    if (child[1]->child[2]) {
        printExpComment = true;
        child[1]->child[2]->gen(tbl);
    } else
        emitRM((char *)"LDC",3,1,6,(char *)"default increment by 1");
    emitRM((char *)"ST",3,tofs+1,1,(char *)"save step value");
    int loopLoc = emitSkip(0);
    emitRM((char *)"LD",4,tofs+3,1,(char *)"loop index");
    emitRM((char *)"LD",5,tofs+2,1,(char *)"stop value");
    emitRM((char *)"LD",3,tofs+1,1,(char *)"step value");
    emitRO((char *)"SLT",3,4,5,(char *)"Op <");
    emitRM((char *)"JNZ",3,1,7,(char *)"Jump to loop body");
    int breakLocT = breakLoc;
    breakLoc = emitSkip(1);
    if(child[2])
        child[2]->gen(tbl); // statement
    emitComment((char *)"Bottom of loop increment and jump");
    emitRM((char *)"LD",3,tofs+3,1,(char *)"Load index");
    emitRM((char *)"LD",5,tofs+1,1,(char *)"Load step");
    emitRO((char *)"ADD",3,3,5,(char *)"increment");
    emitRM((char *)"ST",3,tofs+3,1,(char *)"store back to index");
    emitGotoAbs(loopLoc, (char *)"go to beginning of loop");
    backPatchAJumpToHere(breakLoc, (char *)"Jump past loop [backpatch]");
    breakLoc = breakLocT;
    emitComment((char *)"END LOOP");
    if (doS)
        Node::genS(tbl);
}

void ForStmt::print(bool ann, bool mem)
{
    if (mem) {
        memstr = (char *)malloc(64);
        sprintf(memstr, "[mem: %s loc: %d size: %d] ", memType, memLoc, memSize);
    }
    printSelf();
    printf(
        "For %s[line: %d]\n",
        mem ? memstr : "",
        linenum
    );
    Node::print(ann, mem);
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
if (!semDone) {
    type = (char *)"int";
    fofs -= 2;
    memLoc = fofs;
    Node::semC(tbl);
    for (int i = 0; i < MAXCHILDREN; i++) if (child[i]) {
        if (strcmp(child[i]->type, (char *)"int")
         && strcmp(child[i]->type, (char *)"undefined"))
            throwErr(RANGE_TYPE, this, child[i], NULL, NULL, i+1);
        if (child[i]->isArray)
            throwErr(ARR_RANGE, this, NULL, NULL, NULL, i+1);
    }
    semDone = true;
}
}

void IterRange::gen(SymbolTable * tbl, bool doS) {
    tofs -= 2;
}

void IterRange::print(bool ann, bool mem)
{
    if (mem) {
        memstr = (char *)malloc(64);
        sprintf(memstr, "[mem: %s loc: %d size: %d] ", memType, memLoc, memSize);
    }
    printSelf();
    printf(
        "Range [line: %d]\n",
        linenum
    );
    Node::print(ann, mem);
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
if (!semDone) {
    Node::semC(tbl);
    if (child[0]) {
        if (!strcmp(curFun->type, (char *)"void"))
            throwErr(RETURN_VALUED, this, curFun);
        if (strcmp(child[0]->type, curFun->type)
         && strcmp(curFun->type, (char *)"void")
         && strcmp(curFun->type, (char *)"undefined")
         && strcmp(child[0]->type, (char *)"undefined"))
            throwErr(RETURN_MISMATCH, this, curFun);
        if (child[0]->isArray)
            throwErr(ARR_RETURN, this);
    } else {
        if (strcmp(curFun->type, (char *)"void"))
            throwErr(RETURN_UNVALUED, this, curFun);
    }
    returned = true;
    Node::semS(tbl);
    semDone = true;
}
}

void ReturnStmt::gen(SymbolTable * tbl, bool doS)
{
    emitComment((char *)"RETURN");
    if (child[0]) {
        printExpComment = false;
        child[0]->gen(tbl);
        emitRM((char *)"LDA",2,0,3,(char *)"Copy result to return register");
    }
    emitRM((char *)"LD",3,-1,1,(char *)"Load return address");
    emitRM((char *)"LD",1,0,1,(char *)"Adjust fp");
    emitRM((char *)"JMP",7,0,3,(char *)"Return");
    if (doS)
        Node::genS(tbl);
}

void ReturnStmt::print(bool ann, bool mem)
{
    printSelf();
    printf("Return [line: %d]\n", linenum);
    Node::print(ann, mem);
}

BreakStmt::BreakStmt(int line)
{
    linenum = line;
}

void BreakStmt::sem(SymbolTable * tbl)
{
if (!semDone) {
    Node::semC(tbl);
    if (loopDepth == 0)
        throwErr(LOOPLESS_BREAK, this);
    Node::semS(tbl);
    semDone = true;
}
}

void BreakStmt::gen(SymbolTable * tbl, bool doS)
{
    emitComment((char *)"BREAK");
    emitGotoAbs(breakLoc,(char *)"break");
    if (doS)
        Node::genS(tbl);
}

void BreakStmt::print(bool ann, bool mem)
{
    printSelf();
    printf("Break [line: %d]\n", linenum);
    Node::print(ann, mem);
}

Op::Op(TokenData * tkn, Node * lhs)
{
    addChild(0, lhs);
    linenum = tkn->linenum;
    opId = tkn->tokenclass;
    if (!strcmp(tkn->tokenstr, "-"))
        opName = (char *)"chsign";
    else if (!strcmp(tkn->tokenstr, "*"))
        opName = (char *)"sizeof";
    else
        opName = strdup(tkn->tokenstr);
}

Op::Op(TokenData * tkn, Node * lhs, Node * rhs) : Op(tkn, lhs)
{
    if (!strcmp(tkn->tokenstr, "="))
        opName = (char *)":=";
    else if (!strcmp(tkn->tokenstr, "=="))
        opName = (char*)"=";
    else if (!strcmp(tkn->tokenstr, "!="))
        opName = (char*)"><";
    else
        opName = strdup(tkn->tokenstr);
    addChild(1, rhs);
}

void Op::sem(SymbolTable * tbl)
{
if (!semDone) {
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
            if (child[0]->isArray || child[1]->isArray)
                throwErr(OP_ARR, this);
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
            if (child[0]->isArray || child[1]->isArray)
                throwErr(OP_ARR, this);
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
                if (child[0]->isArray || child[1]->isArray)
                    throwErr(OP_ARR, this);
            } else {
                if (strcmp(child[0]->type, type)
                 && strcmp(child[0]->type, (char *)"undefined"))
                    throwErr(OPERAND_UNARY, this);
                if (child[0]->isArray)
                    throwErr(OP_ARR, this);
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
                if (child[0]->isArray || child[1]->isArray)
                    throwErr(OP_ARR, this);
            } else if (!child[0]->isArray && strcmp(child[0]->type, (char *)"undefined"))
                throwErr(OP_NONARR, this);
            break;
        case BRACKL:
            type = strdup(child[0]->type);
            if (!child[0]->isArray)
                throwErr(NON_ARR_INDEX, this);
            if (strcmp(child[1]->type, (char *)"int")
             && strcmp(child[1]->type, (char *)"undefined"))
                throwErr(ARR_NONINT_INDEX, this);
            if (child[1]->isArray)
                throwErr(ARR_UNINDEXED, this);
            break;
        case NOT:
            type = (char *)"bool";
            if (strcmp(child[0]->type, type)
             && strcmp(child[0]->type, (char *)"undefined"))
                throwErr(OPERAND_UNARY, this);
            if (child[0]->isArray)
                throwErr(OP_ARR, this);
            break;
        case QUEST:
            type = (char *)"int";
            if (strcmp(child[0]->type, type)
             && strcmp(child[0]->type, (char *)"undefined"))
                throwErr(OPERAND_UNARY, this);
            if (child[0]->isArray)
                throwErr(OP_ARR, this);
            break;
    }
    if ((!child[1] && child[0]->isConst && opId != QUEST)
     || (child[1] && child[0]->isConst && child[1]->isConst))
        isConst = true;
    Node::semS(tbl);
    semDone = true;
}
}

void Op::gen(SymbolTable * tbl, bool doS)
{
    if (printExpComment) {
        emitComment((char *)"EXPRESSION");
        printExpComment = false;
    }
    child[0]->gen(tbl);
    if (child[1]) {
        emitRM((char *)"ST",3,tofs,1,(char *)"Push left side");
        emitComment((char *)"TOFF dec:",--tofs);
        child[1]->gen(tbl);
        emitComment((char *)"TOFF inc:",++tofs);
        emitRM((char *)"LD",4,tofs,1,(char*)"Pop left into ac1");
    }
    switch(opId) {
        case EQ:    
        case NEQ:   
        case LESS:  
        case LEQ:   
        case GRTR:  
        case GEQ:   
            if (child[0]->isArray) {
                emitRM((char *)"LD",5,1,3,(char *)"AC2 <- |RHS|");
                emitRM((char *)"LD",6,1,4,(char *)"AC3 <- |LHS|");
                emitRM((char *)"LDA",2,0,5,(char *)"R2 <- |RHS|");
                emitRO((char *)"SWP",5,6,6,(char *)"pick smallest size");
                emitRM((char *)"LD",6,1,4,(char *)"AC3 <- |LHS|");
                emitRO((char *)"CO",4,3,5,(char *)"setup array compare  LHS vs RHS");
                emitRO((char *)"TNE",5,4,3,(char *)"if not equal then test (AC1, AC)");
                emitRM((char *)"JNZ",5,2,7,(char *)"jump not equal");
                emitRM((char *)"LDA",3,0,2,(char *)"AC1 <- |RHS|");
                emitRM((char *)"LDA",4,0,6,(char *)"AC <- |LHS|");
            }
        case AND:
        case OR:    
        case PLUS:  
        case SLASH: 
        case PERC:  
            emitRO(getOpIString(opId),3,4,3,(char *)"Op",opName,(char *)"");
            break;
        case MINUS: 
            if (child[1])
                emitRO(getOpIString(opId),3,4,3,(char *)"Op",opName,(char *)"");
            else
                emitRO((char *)"NEG",3,3,3,(char *)"Op",opName,(char *)"");
            break;
        case ASTR:  
            if (child[1])
                emitRO(getOpIString(opId),3,4,3,(char *)"Op",opName,(char *)"");
            else
                emitRM((char *)"LD",3,1,3,(char *)"Load array size");
            break;
        case BRACKL:
            emitRO((char *)"SUB",3,4,3,(char *)"compute location from index");
            emitRM((char *)"LD",3,0,3,(char *)"Load array element");
            break;
        case NOT:   
            emitRM((char *)"LDC",4,1,6,(char *)"Load 1");
            emitRO((char *)"XOR",3,3,4,(char *)"Op XOR to get logical not");
            break;
        case QUEST:
            emitRO(getOpIString(opId),3,3,3,(char *)"Op",opName,(char *)"");
            break;
    }
    if (doS)
        Node::genS(tbl);
}

void Op::print(bool ann, bool mem)
{
    printSelf();
    printf("Op: %s%s%s [line: %d]\n",
           opName,
           ann ? " of type " : "",
           ann ? type : "",
           linenum);
    Node::print(ann, mem);
}

Assign::Assign(TokenData * tkn, Node * lhs) : Op(tkn, lhs) {}

Assign::Assign(TokenData * tkn, Node * lhs, Node * rhs) : Op(tkn, lhs, rhs) {}

void Assign::sem(SymbolTable * tbl)
{
if (!semDone) {
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
            isArray = (child[0]->isArray && child[1]->isArray);
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
            if (child[0]->isArray || child[1]->isArray)
                throwErr(OP_ARR, this);
            child[0]->init(tbl);
            break;
        case INC:
        case DEC:
            child[0]->sem(tbl);
            type = (char *)"int";
            if (strcmp(child[0]->type, type)
             && strcmp(child[0]->type, (char *)"undefined"))
                throwErr(OPERAND_UNARY, this);
            if (child[0]->isArray)
                throwErr(OP_ARR, this);
            break;
    }
    Node::semS(tbl);
    semDone = true;
}
}

void Assign::gen(SymbolTable * tbl, bool doS)
{
    if (printExpComment) {
        emitComment((char *)"EXPRESSION");
        printExpComment = false;
    }
    Id * lhs;
    if (child[1] && child[0]->isArray && child[1]->isArray) {
        // array to array assignment
        lhs = (Id *)child[0];
        child[1]->gen(tbl);
        emitRM((char *)"LDA",4,lhs->memLoc,1,(char *)"address of lhs");
        emitRM((char *)"LD",5,1,3,(char *)"size of rhs");
        emitRM((char *)"LD",6,1,4,(char *)"size of lhs");
        emitRO((char *)"SWP",5,6,6,(char *)"pick smallest size");
        emitRO((char *)"MOV",4,3,5,(char *)"array op =");
    } else if (child[0]->child[0]) {
        // lhs is a dereferenced array
        lhs = (Id *)child[0]->child[0];
        child[0]->child[1]->gen(tbl);
        if (child[1]) {
            // binary
            emitRM((char *)"ST",3,tofs,1,(char *)"Push index");
            emitComment((char *)"TOFF dec:",--tofs);
            child[1]->gen(tbl);
            emitComment((char *)"TOFF inc:",++tofs);
            emitRM((char *)"LD",4,tofs,1,(char *)"Pop index");
        } //else {} // do nothing if unary
        if (strcmp(lhs->memType, (char *)"Parameter"))
            // not parameter
            emitRM((char *)"LDA",5,lhs->memLoc,!lhs->isGlobal,(char *)"Load address of base of array",lhs->name);
        else
            // parameter
            emitRM((char *)"LD",5,lhs->memLoc,1,(char *)"Load address of base of array",lhs->name);
        emitRO((char *)"SUB",5,5,(child[1]?4:3),(char *)"Compute offset of value");
        if (opId != ASS)
            emitRM((char *)"LD",(child[1]?4:3),0,5,(char *)"load lhs variable",lhs->name);
        if (opId == ADDASS || opId == SUBASS || opId == MULASS || opId == DIVASS)
            emitRO(getOpIString(opId),3,4,3,(char *)"op",opName,(char *)"");
        else if (opId == INC)
            emitRM((char *)"LDA",3,1,3,(char *)"increment value of",lhs->name);
        else if (opId == DEC)
            emitRM((char *)"LDA",3,-1,3,(char *)"decrement value of",lhs->name);
        emitRM((char *)"ST",3,0,5,(char *)"Store variable",lhs->name);
    } else {
        // not array
        lhs = (Id *)child[0];
        if (child[1]) {
            // binary
            child[1]->gen(tbl);
            if (opId != ASS)
                emitRM((char *)"LD",4,lhs->memLoc,!lhs->isGlobal,(char *)"load lhs variable",lhs->name);
        } else {
            // unary
            if (opId != ASS)
                emitRM((char *)"LD",3,lhs->memLoc,!lhs->isGlobal,(char *)"load lhs variable",lhs->name);
            if (opId == INC)
                emitRM((char *)"LDA",3,1,3,(char *)"increment value of",lhs->name);
            else if (opId == DEC)
                emitRM((char *)"LDA",3,-1,3,(char *)"decrement value of",lhs->name);
        }
        if (opId == ADDASS || opId == SUBASS || opId == MULASS || opId == DIVASS)
            emitRO(getOpIString(opId),3,4,3,(char *)"op",opName,(char *)"");
        emitRM((char *)"ST",3,lhs->memLoc,!lhs->isGlobal,(char *)"Store variable",lhs->name);
    }
    if (doS)
        Node::genS(tbl);
}

void Assign::print(bool ann, bool mem)
{
    printSelf();
    printf(
        "Assign: %s%s%s%s [line: %d]\n",
        opName,
        isArray ? " of array" : "",
        ann ? " of type " : "", 
        ann ? type : "",
        linenum
    );
    Node::print(ann, mem);
}

Id::Id(TokenData * tkn) : VarDecl(tkn) {}

void Id::sem(SymbolTable * tbl)
{
if (!semDone) {
    void * found = tbl->lookup(name);
    if (!found)
        throwErr(DECL_NOT, this);
    else {
        VarDecl * v = (VarDecl *)found;
        v->usageFlg = true;
        if (v->isFun)
            throwErr(FUN_AS_VAR, this);
        else {
            isArray = v->isArray;
            if (isInited)
                v->isInited = true;
            isGlobal = v->isGlobal;
            type = strdup(v->type);
            isStatic = v->isStatic;
            memType = v->memType ? strdup(v->memType) : NULL;
            memLoc = v->memLoc;
            memSize = v->memSize;
            if (doInitChecking
             && !v->isInited
             && !v->usageWrnFlg)
                throwWrn(VAR_UNINITED, this, v);
        }
    }
    Node::sem(tbl);
    semDone = true;
}
}

void Id::gen(SymbolTable * tbl, bool doS)
{
    if (!isArray)
        emitRM((char *)"LD",3,memLoc,!isGlobal,(char *)"Load variable",name);
    else {
        if (strcmp(memType, (char *)"Parameter"))
            // not parameter
            emitRM((char *)"LDA",3,memLoc,!isGlobal,(char *)"Load address of base of array",name);
        else
            // parameter
            emitRM((char *)"LD",3,memLoc,1,(char *)"Load address of base of array",name);
    }
    if (doS)
        Node::genS(tbl);
}

void Id::print(bool ann, bool mem)
{
    if (mem) {
        memstr = (char *)malloc(64);
        sprintf(memstr, "[mem: %s loc: %d size: %d] ", memType, memLoc, memSize);
    }
    printSelf();
    printf(
        "Id: %s%s%s%s%s%s %s[line: %d]\n",
        name,
        (isArray || isStatic || ann) ? " of " : "",
        isStatic ? "static " : "",
        isArray ? "array of " : "",
        ann ? "type " : "",
        ann ? type : "",
        mem ? memstr : "",
        linenum
    );
    Node::print(ann, mem);
}

Call::Call(TokenData * tkn, Node * args)
{
    token = tkn;
    linenum = tkn->linenum;
    addChild(0, args);
    type = (char *)"undefined";
    name = strdup(tkn->tokenstr);
}

void Call::sem(SymbolTable * tbl)
{
if (!semDone) {
    void * found = tbl->lookup(name);
    if (!found) {
        throwErr(DECL_NOT, this);
        Node::semC(tbl);
    } else {
        VarDecl * v = (VarDecl *)found;
        v->usageFlg = true;
        if (!v->isFun)
            throwErr(VAR_AS_FUN, this);
        else {
            type = strdup(v->type);
            FunDecl * f = (FunDecl *)v;

            Node * formal = child[0];
            Node * actual = f->child[0];
            int i = 0;
            do {
                if (i++ != 0) {
                    formal = formal->sibling;
                    actual = actual->sibling;
                }
                if (!formal && actual)
                    throwErr(PARM_FEW, this, f);
                else if (formal && !actual)
                    throwErr(PARM_MANY, this, f);
                if (formal) {
                    formal->sem(tbl);
                    if (actual) {
                        if (strcmp(formal->type, actual->type)
                         && strcmp(formal->type, (char *)"undefined"))
                            throwErr(PARM_TYPE, this, actual, formal, f, i);
                        if (!formal->isArray && actual->isArray)
                            throwErr(PARM_NONARR, this, f, NULL, NULL, i);
                        else if (formal->isArray && !actual->isArray)
                            throwErr(PARM_ARR, this, f, NULL, NULL, i);
                        formal->isParm = true;
                    }
                }
            } while (formal && actual);
            if (formal && formal->sibling)
                formal->sibling->sem(tbl);
        }
    }
    Node::semS(tbl);
    semDone = true;
}
}

void Call::gen(SymbolTable * tbl, bool doS)
{
    if (printExpComment) {
        emitComment((char *)"EXPRESSION");
        printExpComment = false;
    }
    FunDecl * f = (FunDecl *)tbl->lookupGlobal(name);
    emitComment((char *)"CALL",name);
    emitRM((char *)"ST",1,tofs,1,(char *)"Store fp in ghost frame for",name);
    int tofst = tofs;
    emitComment((char *)"TOFF dec:",--tofs);
    emitComment((char *)"TOFF dec:",--tofs);
    int in = 1;
    for (Node * i = child[0]; i; i = i->sibling) {
        emitComment((char *)"Param", in++);
        i->gen(tbl, false);
        emitRM((char *)"ST",3,tofs,1,(char *)"Push parameter");
        emitComment((char *)"TOFF dec:",--tofs);
    }
    tofs = tofst;
    emitComment((char *)"Param end",name);
    emitRM((char *)"LDA",1,tofs,1,(char *)"Ghost frame becomes new active frame");
    emitRM((char *)"LDA",3,1,7,(char *)"Return address in ac");
    int fLoc = f->addr - emitSkip(0) - 1;
    emitRM((char *)"JMP",7,fLoc,7,(char *)"CALL",f->name);
    emitRM((char *)"LDA",3,0,2,(char *)"Save the result in ac");
    emitComment((char *)"Call end",name);
    emitComment((char *)"TOFF set:",tofs);
    if (doS)
        Node::genS(tbl);
}

void Call::print(bool ann, bool mem)
{
    printSelf();
    printf("Call: %s%s%s [line: %d]\n",
           name,
           ann ? " of type " : "",
           ann ? type : "",
           linenum);
    Node::print(ann, mem);
}

Const::Const(TokenData * tkn)
{
    linenum = tkn->linenum;
    isConst = true;
    token = tkn;
    isStrConst = false;
    switch (token->tokenclass) {
        case BOOLCONST:   type = (char *)"bool"; break;
        case NUMCONST:    type = (char *)"int";  break;
        case CHARCONST:   type = (char *)"char"; break;
        case STRINGCONST: type = (char *)"char";
            isStrConst = true;
            isArray = true;
            memType = (char *)"Global";
            memSize = tkn->svalue.length() + 1;
            break;
    }
}

void Const::sem(SymbolTable * tbl)
{
if (!semDone) {
    if (isArray) {
        memLoc = gofs - 1;
        gofs -= memSize;
    }
    Node::semS(tbl);
    semDone = true;
}
}

void Const::gen(SymbolTable * tbl, bool doS)
{
    switch (token->tokenclass) {
        case BOOLCONST:   emitRM((char *)"LDC",3,token->nvalue,6,(char *)"Load Boolean constant");   break;
        case NUMCONST:    emitRM((char *)"LDC",3,token->nvalue,6,(char *)"Load integer constant");   break;
        case CHARCONST:   emitRM((char *)"LDC",3,(int)token->cvalue,6,(char *)"Load char constant"); break;
        case STRINGCONST: emitStrLit(memLoc, (char *)token->svalue.c_str());
            if (isParm)
                emitRM((char *)"LDA",3,memLoc,0,(char *)"Load address of char arrayy");
            break;
    }
    if (doS)
        Node::genS(tbl);
}

void Const::print(bool ann, bool mem)
{
    if (mem) {
        memstr = (char *)malloc(64);
        sprintf(memstr, "[mem: %s loc: %d size: %d] ", memType, memLoc, memSize);
    }
    printSelf();
    printf("Const ");
    switch (token->tokenclass) {
        case BOOLCONST: printf(token->nvalue ? "true" : "false");   break;
        case NUMCONST:    printf("%d", token->nvalue);              break;
        case CHARCONST:   printf("'%c'", token->cvalue);            break;
        case STRINGCONST: std::cout << "\"" << token->svalue << "\" of array"; break;
    }
    printf("%s%s %s[line: %d]\n",
           ann ? " of type " : "",
           ann ? type : "",
           (mem && isArray) ? memstr : "",
           linenum);
    Node::print(ann, mem);
}

static TokenData * buildTkn(char * name)
{
    TokenData * t = new TokenData;
    t->linenum = -1;
    t->tokenstr = strdup(name);
    t->svalue = strdup(name);
    return t;
}
