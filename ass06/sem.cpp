#include "sem.h"

static TokenData * buildTkn(char *);

void analyze(Node * tree, SymbolTable * tbl)
{
    // add I/O functions to the table
    // void output(int)
    Parm * pi = new Parm(buildTkn((char *)"*dummy*"), false);
    pi->type = (char *)"int";
    FunDecl * output = new FunDecl(buildTkn((char *)"output"), pi, NULL);
    output->usageFlg = true;
    tbl->insert(output->name, output);

    // void outputb(bool)
    Parm * pb = new Parm(buildTkn((char *)"*dummy*"), false);
    pb->type = (char *)"bool";
    FunDecl * outputb = new FunDecl(buildTkn((char *)"outputb"), pb, NULL);
    outputb->usageFlg = true;
    tbl->insert(outputb->name, outputb);

    // void outputc(char)
    Parm * pc = new Parm(buildTkn((char *)"*dummy*"), false);
    pc->type = (char *)"char";
    FunDecl * outputc = new FunDecl(buildTkn((char *)"outputc"), pc, NULL);
    outputc->usageFlg = true;
    tbl->insert(outputc->name, outputc);

    // int input()
    FunDecl * input = new FunDecl(buildTkn((char *)"int"), buildTkn((char *)"input"), NULL, NULL);
    input->usageFlg = true;
    tbl->insert(input->name, input);

    // bool inputb()
    FunDecl * inputb = new FunDecl(buildTkn((char *)"bool"), buildTkn((char *)"inputb"), NULL, NULL);
    inputb->usageFlg = true;
    tbl->insert(inputb->name, inputb);

    // char inputc()
    FunDecl * inputc = new FunDecl(buildTkn((char *)"char"), buildTkn((char *)"inputc"), NULL, NULL);
    inputc->usageFlg = true;
    tbl->insert(inputc->name, inputc);

    // void outnl()
    FunDecl * outnl = new FunDecl(buildTkn((char *)"outnl"), NULL, NULL);
    outnl->usageFlg = true;
    tbl->insert(outnl->name, outnl);


    // preform semantic analysis
    tree->sem(tbl);


    // preform global semantic analysis
    Node * m = (Node *)tbl->lookupGlobal("main");
    bool validMain = (m && ((VarDecl *)m)->isFun && !m->child[0]);  // was main() defined?
    if (m && ((VarDecl *)m)->isFun)
        ((VarDecl *)m)->usageFlg = true;
    tbl->applyToAll(wrnUnused); // issue global scope usage warnings
    if (!validMain)
        throwErr(NO_MAIN, NULL);
}

static TokenData * buildTkn(char * name)
{
    TokenData * t = new TokenData;
    t->linenum = -1;
    t->tokenstr = strdup(name);
    t->svalue = strdup(name);
    return t;
}
