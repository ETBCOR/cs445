#ifndef _NODE_H_
#define _NODE_H_

#include <string.h>
#include <stdio.h>
#include <map>
#include <iostream>
#include <vector>
#include "scanType.h"
#include "parser.tab.h"
#include "symbolTable.h"
#include "error.h"
#include "emitcode.h"

#define MAXCHILDREN 3

extern char * src;

extern int fofs;
extern int gofs;
extern int tofs;
extern int breakLoc;

class Node
{ public:
    TokenData * token;                      // stores the token that generated this node
    Node * child[MAXCHILDREN];              // pointers to children of the node
    Node * sibling;                         // sibling pointer

    bool semDone;                           // is semantic analysis done at this node (and children)?
    int linenum;                            // number of the relevant line
    int depth;                              // stores depth in tree for printing
    int index;                              // stores the index in child/sibling list
    int size;

    bool isRoot;                            // is this the root of the AST?
    bool isChild;                           // used for printing
    bool isConst;                           // is this a constant?
    bool isArray;                           // is this node an array?
    bool isParm;                            // is this node a parameter?
    bool isUnscoped;                        // is this node unscoped (e.g. "if(true) x++;")?
    bool isInited;                          // has this node been initialized?
    bool isGlobal;                          // is this node global?

    char * type;                            // for variable and expression nodes

    char * memType;
    int memLoc;
    int memSize;

    Node();                                 // constructor (will be overriden)
    Node(TokenData *);                      // constructor that stores the token

    void add(Node *);                       // adds the given node to the end of sibling list
    void addChild(int, Node *);             // adds given node as child of this node at given index

    void updateLoc();                       // recursively sets the depth and index for the whole tree
    virtual void init(SymbolTable *);       // initialzies symbols into the table
    virtual void sem(SymbolTable *);        // do error/warning checking and semantic analysis
    void semC(SymbolTable *);               // do semantic analysis on children
    void semS(SymbolTable *);               // do semantic analysis on sibling
    
    void gen(SymbolTable *);                // do code generation (assuming sibling should be tried next)
    virtual void gen(SymbolTable *, bool);  // do code generation (bool tells whether to go onto next sibling)
    void genC(SymbolTable *);               // do code generation of children
    void genS(SymbolTable *);               // do code generation of sibling

    void printSelf();                       // prints the preamble for each line
    virtual void print(bool, bool);         // recursively prints the tree. bools are for extra type info and memory info respecitvly
};

class VarDecl: public Node
{ public:
    char * name;                            // stores the name of the variable or function

    bool isFun;                             // does this node represent a function (or a variable)?
    bool isStatic;                          // is this node's variable/function statically allocated?
    bool isDummy;                           // is this a dummy function/paramater for IO?

    int arrSize;                            // stores the size of the array (if the node is an array)

    bool usageFlg;                          // set to true when var is found to have been referenced
    bool usageWrnFlg;                       // set to true when a "var unused" message has been sent for this var

    VarDecl(TokenData *);                   // default constructor
    VarDecl(TokenData *, TokenData *);      // array constructor

    void initS();                           // recursively sets isInited of nodes in sibling list
    void setType(char *, bool);             // recursively sets type and isStatic of nodes in sibling list

    virtual void init(SymbolTable *);
    virtual void sem(SymbolTable *);
    virtual void gen(SymbolTable *, bool);
    virtual void print(bool, bool);
};

class FunDecl: public VarDecl
{ public:
    int addr;                               // address of function (for calls)

    FunDecl(TokenData *, Node *, Node *);   // constructor for fun decls w/o type spec
    FunDecl(TokenData *, TokenData *,       // constructor for function declarations
                         Node *, Node *);   //                   with type specifier

    virtual void sem(SymbolTable *);
    virtual void gen(SymbolTable *, bool);
    virtual void print(bool, bool);
};

class Parm: public VarDecl
{ public:
    Parm(TokenData *, bool arr = false);    // constructor for parameter nodes, takes ID and bool for isArray

    virtual void sem(SymbolTable *);
    virtual void gen(SymbolTable *, bool);
    virtual void print(bool, bool);
};

class CompoundStmt: public Node
{ public:
    CompoundStmt(int, Node *, Node *);

    virtual void sem(SymbolTable *);
    virtual void gen(SymbolTable *, bool);
    virtual void print(bool, bool);
};

class IfStmt: public Node
{ public:
    IfStmt(TokenData *, Node *, Node *);            // constructor for if statements (no else part)
    IfStmt(TokenData *, Node *, Node *, Node*);     // constructor for if/else statements

    virtual void sem(SymbolTable *);
    virtual void gen(SymbolTable *, bool);
    virtual void print(bool, bool);
};

class WhileStmt: public Node
{ public:
    WhileStmt(TokenData *, Node *, Node*);          // constructor for while statements

    virtual void sem(SymbolTable *);
    virtual void gen(SymbolTable *, bool);
    virtual void print(bool, bool);
};

class ForStmt: public Node
{ public:
    ForStmt(TokenData *, TokenData *, Node *, Node *);  // constructor for for statements

    virtual void sem(SymbolTable *);
    virtual void gen(SymbolTable *, bool);
    virtual void print(bool, bool);
};

class IterRange: public Node
{ public:
    IterRange(int, Node *, Node *);         // constructor for "to" ranges 
    IterRange(int, Node *, Node *, Node *); // constructor for to/by ranges

    virtual void sem(SymbolTable *);
    virtual void gen(SymbolTable *, bool);
    virtual void print(bool, bool);
};

class ReturnStmt: public Node
{ public:
    ReturnStmt(int);                        // constructor for void return statements
    ReturnStmt(int, Node *);                // constructor for typed return statements

    virtual void sem(SymbolTable *);
    virtual void gen(SymbolTable *, bool);
    virtual void print(bool, bool);
};

class BreakStmt: public Node
{ public:
    BreakStmt(int);                         // constructor for break statements

    virtual void sem(SymbolTable *);
    virtual void gen(SymbolTable *, bool);
    virtual void print(bool, bool);
};

class Op: public Node
{ public:
    char * opName;                          // name of the operation (from token)
    int opId;                               // lex/yacc id of the operation

    Op(TokenData *, Node *);                // constructor for unary operations
    Op(TokenData *, Node *, Node *);        // constructor for binary operations

    virtual void sem(SymbolTable *);
    virtual void gen(SymbolTable *, bool);
    virtual void print(bool, bool);
};

class Assign: public Op
{ public:
    Assign(TokenData *, Node *);            // constructor for unary assignments
    Assign(TokenData *, Node *, Node *);    // constructor for binary assignments

    virtual void sem(SymbolTable *);
    virtual void gen(SymbolTable *, bool);
    virtual void print(bool, bool);
};

class Id: public VarDecl
{ public:
    Id(TokenData *);                        // constructor for identifiers

    virtual void sem(SymbolTable *);
    virtual void gen(SymbolTable *, bool);
    virtual void print(bool, bool);
};

class Call: public Node
{ public:
    char * name;                            // stores the name of the function to be called

    Call(TokenData *, Node*);               // constructor for call "statement"

    virtual void sem(SymbolTable *);
    virtual void gen(SymbolTable *, bool);
    virtual void print(bool, bool);
};

class Const: public Node
{ public:
    TokenData * token;                      // stores a constant's original token
    bool isStrConst;

    Const(TokenData *);                     // constructor for constants

    virtual void sem(SymbolTable *);
    virtual void gen(SymbolTable *, bool);
    virtual void print(bool, bool);
};

#endif /* _NODE_H_ */

