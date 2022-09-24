#ifndef TREE_H
#define TREE_H

#include <cstddef>
#include <stdio.h>
#include <string>
#include "scanType.h"
#include "parser.tab.h"

#define MAXCHILDREN 3

/* abstract AST node - for all nonterminals */
class Node
{ public:
    // tree connectivity
    Node * child[MAXCHILDREN];  // pointers to children of the node
    Node * sibling;             // sibling pointer

    int linenum;                // number of the relevant line
    int depth;                  // stores depth in tree for printing
    int index;                  // stores the index in child/sibling list

    bool isRoot;                // is this the root of the AST?
    bool isChild;               // used for printing
    bool isConst;               // is this a constant?
    bool isArray;               // is this node an array?
    
    char * type;                // for variable and expression nodes

    Node();                     // constructor (will be overriden)

    void add(Node *);           // adds the given node to the end of sibling list
    void addChild(int, Node *); // adds given node as child of this node at given index

    void updateLoc();           // recursively sets the depth and index for the whole tree

    void printSelf();           // prints the preamble for each line
    virtual void print();       // recursively prints the tree
};

/* variable nodes (and function nodes) */
class VarDecl: public Node
{ public:
    char * name;                            // stores the name of the variable or function

    bool isFun;                             // does this node represent a function (or a variable)?
    bool isStatic;                          // is this node's variable/function statically allocated?
    bool isInited;                          // has this node been initialized?

    int arrSize;                            // stores the size of the array (if the node is an array)

    VarDecl(TokenData *);                   // default constructor
    VarDecl(TokenData *, TokenData *);      // array constructor

    void setType(char *, bool);             // recursively sets type and isStatic of nodes in sibling list
    void init();                            // recursively sets isInited of nodes in sibling list

    virtual void print();
};

/* function nodes */
class FunDecl: public VarDecl
{ public:
    FunDecl(TokenData *, Node *, Node *);   // constructor for fun decls w/o type spec
    FunDecl(TokenData *, TokenData *,       // constructor for function declarations
                         Node *, Node *);   //                   with type specifier

    virtual void print();
};

class Parm: public VarDecl
{ public:
    Parm(TokenData *, bool arr = false);    // constructor for parameter nodes, takes ID and bool for isArray

    virtual void print();
};

class CompoundStmt: public Node
{ public:
    CompoundStmt(int, Node *, Node *);

    virtual void print();
};

class IfStmt: public Node
{ public:
    IfStmt(int, Node *, Node *);            // constructor for if statements (no else part)
    IfStmt(int, Node *, Node *, Node*);     // constructor for if/else statements

    virtual void print();
};

class WhileStmt: public Node
{ public:
    WhileStmt(int, Node *, Node*);          // constructor for while statements

    virtual void print();
};

class ForStmt: public Node
{ public:
    ForStmt(int, TokenData *, Node *, Node *); // constructor for for statements

    virtual void print();
};

class IterRange: public Node
{ public:
    IterRange(int, Node *, Node *);         // constructor for "to" ranges 
    IterRange(int, Node *, Node *, Node *); // constructor for to/by ranges

    virtual void print();
};

class ReturnStmt: public Node
{ public:
    ReturnStmt(int);                        // constructor for void return statements
    ReturnStmt(int, Node *);                // constructor for typed return statements

    virtual void print();
};

class BreakStmt: public Node
{ public:
    BreakStmt(int);                         // constructor for break statements

    virtual void print();
};

class Op: public Node
{ public:
    char * opName;                          // name of the operation (from token)
    int opId;                               // lex/yacc id of the operation

    Op(TokenData *, Node *);                // constructor for unary operations
    Op(TokenData *, Node *, Node *);        // constructor for binary operations

    virtual void print();
};

class Assign: public Op
{ public:
    Assign(TokenData *, Node *);             // constructor for unary assignments
    Assign(TokenData *, Node *, Node *);    // constructor for binary assignments

    virtual void print();
};

class Id: public VarDecl
{ public:
    Id(TokenData *);                        // constructor for identifiers

    virtual void print();
};

class Call: public Node
{ public:
    char * name;                            // stores the name of the function to be called

    Call(TokenData *, Node*);               // constructor for call "statement"

    virtual void print();
};

class Const: public Node
{ public:
    TokenData * token;                      // stores a constant's original token

    Const(TokenData *);                     // constructor for constants

    virtual void print();
};

#endif /* TREE_H */

