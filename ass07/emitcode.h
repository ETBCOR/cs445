#ifndef EMIT_CODE_H__
#define EMIT_CODE_H__

//
//  REGISTER DEFINES for optional use in calling the 
//  routines below.
//
#define GP   0	//  The global pointer
#define FP   1	//  The local frame pointer
#define RT   2	//  Return value
#define AC   3  //  Accumulator
#define AC1  4  //  Accumulator
#define AC2  5  //  Accumulator
#define AC3  6  //  Accumulator
#define PC   7	//  The program counter

//
//  No comment please...
//
#define NO_COMMENT (char *)""


//
//  The following functions were borrowed from Tiny compiler code generator
//
int emitWhereAmI();           // gives where the next instruction will be placed
int emitSkip(int howMany);    // emitSkip(0) tells you where the next instruction will be placed
void emitNewLoc(int loc);     // set the instruction counter back to loc

void emitComment();
void emitComment(char *c);
void emitComment(char *c, char *cc);
void emitComment(char *c, int n);

void emitGoto(int d, long long int s, char *c);
void emitGoto(int d, long long int s, char *c, char *cc);
void emitGotoAbs(int a, char *c);
void emitGotoAbs(int a, char *c, char *cc);

void emitRM(char *op, long long int r, long long int d, long long int s, char *c);
void emitRM(char *op, long long int r, long long int d, long long int s, char *c, char *cc);
void emitRMAbs(char *op, long long int r, long long int a, char *c);
void emitRMAbs(char *op, long long int r, long long int a, char *c, char *cc);

void emitRO(char *op, long long int r, long long int s, long long int t, char *c);
void emitRO(char *op, long long int r, long long int s, long long int t, char *c, char *cc);
void emitRO(char *op, long long int r, long long int s, long long int t, char *c, char *cc, char *ccc);

void backPatchAJumpToHere(int addr, char *comment);
void backPatchAJumpToHere(char *cmd, int reg, int addr, char *comment);

int emitStrLit(int goffset, char *s); // for char arrays

#endif
