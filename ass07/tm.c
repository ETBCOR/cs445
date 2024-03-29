// // // // // // // // // // // // // // // // // // // // // // // // 
//
// File: tm.c
// The TM ("Tiny Machine") virtual machine
// Book: Compiler Construction: Principles and Practice
//
// Transmogrifier: Dr. Robert Heckendorn, University of Idaho (should be rewritten)

// v4.6a    R0=addr of top of Dmem, LIT loads at top of mem at minus addr for LIT instruction from R0
//           this sets up for indexing literals from global space like any other memory
// v4.5d   make C language compliant
// v4.5c   CO and COA commands put the differing chars or addresses in registers 
//           r and s which is more convenient
// v4.5b   Actually detect out of bounds on data memory and instr memory! Duh.
// v4.5    Modified Robert Heckendorn Nov 16, 2020
//           JMP, MOD instructions
// v4.4    Fixed problems with long long int so that 64 bit math works
// v4.3    Tag data memory with comment on last instruction that set that loc
// v4.2    Added some commands to make student life easier.
//         NEG  gives the negative of a number.
//         tests: SLT, SGT take 3 registers: r, s, t  SLT is:
//           if (reg[r]>=0) reg[r] = (reg[s]<reg[t] ? 1 : 0);
//           else reg[r] = (-reg[s] < -reg[t] ? 1 : 0);
//         useful for testing in a loop with an increment option
// v4.1    Modified Robert Heckendorn Apr 17, 2020
//         extend commands for dump of imem and dmem to dump all unused.
//         print instruction on step
// v4.0    Modified Robert Heckendorn Mar 7, 2020
//         compare memory instructions renmaed
//         dumped LDL and increment memory instructions
// v3.5b   Modified Robert Heckendorn May 7, 2016
//         Don't output usage message for unknown command error
// v3.5    Modified Robert Heckendorn Dec 20, 2015
//         Output instruction limit
// v3.4b   Modified Robert Heckendorn Nov 30, 2015
//          fixed bounds of RND and put in error for RND of 0
// v3.4    Modified Robert Heckendorn Nov 12, 2015
//          LIT and LDL instrucitions added
// v3.3    Modified Robert Heckendorn Oct 14, 2015
//          put in test and separate jump instructions
//          redid CMP instruction to be more useful and
//          added CMA instruction
// v3.1    Modified Robert Heckendorn Sep 3, 2015
//          changed all the jump instructions, changed
//          direction of MOV, SET instructions, added RND
// v3.01    Modified Robert Heckendorn Nov  20, 2013
//          Added array/character commands, incrementing
//           Load and Store, relaxed syntax, character
//           constants
//
// v2.7.1  Modified Robert Heckendorn Nov  13, 2009
//         bug fix
// v2.7 Modified Robert Heckendorn for CS445 Apr  6, 2006
//         empty line -> step, nicer tracing message
// v2.6 Modified Robert Heckendorn for CS445 Mar 20, 2006
//         Added OUTNL and modified OUTB and OUT instr.
// v2.5 Modified Robert Heckendorn for CS445 May 11, 2005
//         record at each data location the address of the
//         instruction that last assigned a value there
// v2.4 Modified Robert Heckendorn for CS445 Apr 22, 2004
//         fix eof bug, 'd' w/o args uses last arguments and
//         fixed some obscure bugs added 'u' command.
// v2.3 Modified Robert Heckendorn for CS445 Apr 28, 2003
//         marks memory as used or not, added 'e', -n on 'd'
// v2.2 Modified Robert Heckendorn for CS445 Apr 22, 2003
//         saves comments in imem, breakpoint, input break
//         abort limit and other fixes
// v2.1 Modified Robert Heckendorn for CS445 Apr 21, 2003
//         INB, OUTB
// v2.0 Modified Robert Heckendorn for CS445 Apr 11, 2003
//         =, load, more state reported
//
// v1.0 Kenneth C. Louden's original
//
// TO COMPILE: gcc tm.c -o tm
//

char *versionNumber =(char *)"TM version 4.6a";

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#define UP +1
#define DOWN -1
#define TRACE 1
#define NOTRACE 0
#define USED 1
#define UNUSED -1
#define READONLY -2

/******* const *******/
#define   IADDR_SIZE  10000	/* increase for large programs */
#define   DADDR_SIZE  10000	/* increase for large programs */
#define   NO_REGS 8
#define   PC_REG  7

#define   LINESIZE  200
#define   WORDSIZE  1000        /* maximum length of a word of text */
#define   DEFAULT_ABORT_LIMIT 50000
#define   DEFAULT_OUTPUT_LIMIT 1000

/******* type  *******/

// classes of op codes by format
typedef enum
{
    opclRR,			/* reg operands r, s, t */
    opclRA,			/* reg r, int d+s */
    opclLIT                     /* literal command */
} OPCLASS;

// list of all of the op codes
typedef enum
{
    // RR instructions 
    opHALT,			// RR     halt, operands are ignored 
    opNOP,			// RR     no op, operands are ignored 
    opIN,			// RR     read integer into reg(r); s and t are ignored 
    opINB,			// RR     read bool into reg(r); s and t are ignored 
    opINC,			// RR     read char into reg(r); s and t are ignored 
    opOUT,			// RR     write integer from reg(r), s and t are ignored 
    opOUTB,			// RR     write bool from reg(r), s and t are ignored 
    opOUTC,			// RR     write char from reg(r), s and t are ignored 
    opOUTNL,			// RR     write newline regs r, s and t are ignored 
    opADD,			// RR     reg(r) = reg(s)+reg(t) 
    opSUB,			// RR     reg(r) = reg(s)-reg(t) 
    opMUL,			// RR     reg(r) = reg(s)*reg(t) 
    opDIV,			// RR     reg(r) = reg(s)/reg(t) 
    opMOD,			// RR     reg(r) = reg(s)%reg(t) 
    opAND,			// RR     reg(r) = reg(s)&reg(t) 
    opOR,			// RR     reg(r) = reg(s)|reg(t) 
    opXOR,			// RR     reg(r) = reg(s)^reg(t) 
    opNOT,			// RR     reg(r) = ~reg(s) 
    opNEG,			// RR     reg(r) = -reg(s) 
    opSWP,			// RR     reg[r] = min(reg[r], reg[s]), reg[s] = max(reg[r], reg[s])  
    opRND,			// RR     reg[r] = random(0, abs(reg[s])) 

    opTLT,			// RR     if reg(s)<reg(t) then reg(r) = 1  else reg(r) = 0 
    opSLT,			// RR     if (reg[r]>=0) reg[r] = (reg[s]<reg[t] ? 1 : 0); else reg[r] = (-reg[s] < -reg[t] ? 1 : 0);
    opTLE,			// RR     if reg(s)<=reg(t) then reg(r) = 1  else reg(r) = 0 
    opTGT,			// RR     if reg(s)>reg(t) then reg(r) = 1  else reg(r) = 0 
    opSGT,	                // RR     if (reg[r]<=0) reg[r] = (reg[s]>reg[t] ? 1 : 0); else reg[r] = (-reg[s] > -reg[t] ? 1 : 0);
    opTGE,			// RR     if reg(s)>=reg(t) then reg(r) = 1  else reg(r) = 0 
    opTEQ,			// RR     if reg(s)==reg(t) then reg(r) = 1  else reg(r) = 0 
    opTNE,			// RR     if reg(s)!=reg(t) then reg(r) = 1  else reg(r) = 0 

    opMOV,                      // RR     dMem[reg[r] + (0..reg[t]-1)] = dMem[reg[s] + (0..reg[t]-1)] 
    opSET,                      // RR     dMem[reg[r] + (0..reg[t]-1)] = reg[s] 
    opCO,                       // RR     compare memory instruction
    opCOA,                      // RR     compare memory instruction returning address
    opRRLim,			// limit of RR opcodes 

    // RA instructions 
    opLD,			// RM     reg(r) = mem(d+reg(s)) 
    opST,			// RM     mem(d+reg(s)) = reg(r) 
    opLDA,			// RA     reg(r) = d+reg(s) 
    opLDC,			// RA     reg(r) = d ; reg(t) is ignored 
    opJZR,			// RA     if reg(r)==0 then reg(7) = d+reg(s) 
    opJNZ,			// RA     if reg(r)!=0 then reg(7) = d+reg(s) 
    opJMP,                      // RA     reg(7) = d+reg(s) 

    opRALim,                    // limit of RA opcodes
    opLIT,                      // the special litteral op code

    opEND			// Limit of RA opcodes 
} OPCODE;

typedef enum
{
    srOKAY,
    srHALT,
    srIMEM_ERR,
    srDMEM_SET_ERR,
    srDMEM_RONLY_ERR,
    srDMEM_READ_ERR,
    srZERODIVIDE,
    srOUTPUTLIMIT_ERR
} STEPRESULT;

/* needs to do a better job of producing error messages */
char *stepResultTab[] = {
    (char *)"OK",
    (char *)"Halted",
    (char *)"ERROR: Instruction Memory Fault",
    (char *)"ERROR: Set Data Memory Range Fault",
    (char *)"ERROR: Set of Readonly Data Memory",
    (char *)"ERROR: Read Data Memory Range Fault",
    (char *)"ERROR: Division by 0",
    (char *)"ERROR: Output Instruction Limit Exceeded"
};


/* The structure for a instruction  */
typedef struct
{
    int iop;
    long long int iarg1;
    long long int iarg2;
    long long int iarg3;
    char *comment;
} INSTRUCTION;

/******** GLOBAL VARIABLES ********/
int iloc = 0;
int dloc = 0;
int promptflag = TRUE;
int traceflag = FALSE;
int icountflag = FALSE;
int abortLimit = DEFAULT_ABORT_LIMIT;
int outputLimit = DEFAULT_OUTPUT_LIMIT;
int stepcnt;
int pc, lastpc;
int savedbreakpoint, breakpoint;
char *emptyString = (char *)"";
char pgmName[WORDSIZE];
int instrCount = 0;
int outputInstrCount = 0;
int dmemStart = 0;
int dmemCount = 0;
int dmemDown = +1;
int imemStart = 0;
int imemCount = 0;
int imemDown = +1;

INSTRUCTION iMem[IADDR_SIZE];
int iMemTag[IADDR_SIZE];
long long int dMem[DADDR_SIZE];
int dMemTag[DADDR_SIZE];   // if >= 0 then last address modified, == -1 unused, == -2 read/only
char *dMemCmt[DADDR_SIZE];
long long int reg[NO_REGS];

char *opCodeTab[100];

void initOpCodeTab()
{
    opCodeTab[(int)opHALT] = (char *)"HALT";
    opCodeTab[(int)opNOP] = (char *)"NOP";
    opCodeTab[(int)opIN] = (char *)"IN";
    opCodeTab[(int)opINB] = (char *)"INB";
    opCodeTab[(int)opINC] = (char *)"INC";
    opCodeTab[(int)opOUT] = (char *)"OUT";
    opCodeTab[(int)opOUTB] = (char *)"OUTB";
    opCodeTab[(int)opOUTC] = (char *)"OUTC";
    opCodeTab[(int)opOUTNL] = (char *)"OUTNL";
    opCodeTab[(int)opADD] = (char *)"ADD";
    opCodeTab[(int)opSUB] = (char *)"SUB";
    opCodeTab[(int)opMUL] = (char *)"MUL";
    opCodeTab[(int)opDIV] = (char *)"DIV";
    opCodeTab[(int)opMOD] = (char *)"MOD";
    opCodeTab[(int)opAND] = (char *)"AND";
    opCodeTab[(int)opOR] = (char *)"OR";
    opCodeTab[(int)opXOR] = (char *)"XOR";
    opCodeTab[(int)opNOT] = (char *)"NOT";
    opCodeTab[(int)opNEG] = (char *)"NEG";
    opCodeTab[(int)opSWP] = (char *)"SWP";
    opCodeTab[(int)opRND] = (char *)"RND";
    opCodeTab[(int)opTLT] = (char *)"TLT";
    opCodeTab[(int)opSLT] = (char *)"SLT";
    opCodeTab[(int)opTLE] = (char *)"TLE";
    opCodeTab[(int)opTGT] = (char *)"TGT";
    opCodeTab[(int)opSGT] = (char *)"SGT";
    opCodeTab[(int)opTGE] = (char *)"TGE";
    opCodeTab[(int)opTEQ] = (char *)"TEQ";
    opCodeTab[(int)opTNE] = (char *)"TNE";
    opCodeTab[(int)opMOV] = (char *)"MOV";
    opCodeTab[(int)opSET] = (char *)"SET";
    opCodeTab[(int)opCO] = (char *)"CO";
    opCodeTab[(int)opCOA] = (char *)"COA";
    opCodeTab[(int)opRRLim] = (char *)"RRLim";
    opCodeTab[(int)opLD] = (char *)"LD";
    opCodeTab[(int)opST] = (char *)"ST";
    opCodeTab[(int)opLDA] = (char *)"LDA";
    opCodeTab[(int)opLDC] = (char *)"LDC";
    opCodeTab[(int)opJZR] = (char *)"JZR";
    opCodeTab[(int)opJNZ] = (char *)"JNZ";
    opCodeTab[(int)opJMP] = (char *)"JMP";
    opCodeTab[(int)opRALim] = (char *)"RALim";
    opCodeTab[(int)opLIT] = (char *)"LIT";
    opCodeTab[(int)opEND] = (char *)"END OF OPCODES";
}

char *niceStringIn(char *s)
{
    int len;
    char *t;

    len = strlen(s);
    t = (char *)malloc(len+1);

    unsigned char *tp = (unsigned char *)t;
    unsigned char *sp = (unsigned char *)s;
    for (; sp<(unsigned char *)s+len; sp++) {
        if (*sp=='\\') {
            sp++;
            if (*sp=='t') {
                *tp++ = '\t';
            }
            else if (*sp=='n') {
                *tp++ = '\n';
            }
            else if (*sp=='0') {
                *tp++ = '\0';
            }
            else {
                *tp++ = *sp;
            }
        }
        else if (*sp=='^') {
            sp++;
            *tp++ = (*sp) ^ 0x40;
        }
        else {
            *tp++ = *sp;
        }
    }
    *tp='\0';

    return t;
}

char *niceStringOut(char *s) 
{
    int len;
    char *t;

    len = strlen(s);
    t = (char *)malloc(3*len+1);

    unsigned char *tp = (unsigned char *)t;
    unsigned char *sp = (unsigned char *)s;
    for (; sp<(unsigned char *)s+len; sp++) {

        if (*sp=='\t') {
            *tp++ = '\\';
            *tp++ = 't';
           }
        else if (*sp=='\n') {
           *tp++ = '\\';
            *tp++ = 'n';
        }
        else if (*sp=='\0') {
           *tp++ = '\\';
            *tp++ = '0';
        }
        else if (*sp<' ') {
            *tp++ = '^';
            *tp++ = *sp;
        }
        else if (*sp==127) {
            *tp++ = '^';
            *tp++ = '?';
        }
        else if (*sp>127) {
            *tp++ = '\\';
           *tp++ = "0123456789abcdef"[(*sp)/16];
           *tp++ = "0123456789abcdef"[(*sp)%16];
           }
        else {
           *tp++ = *sp;
           }
    }
    *tp='\0';

    return t;
}


char *niceChar(int n) {
    if (n>=32 && n<127) {  // we can easily make this print nice chars for any range
        char s[2];
        s[0] = (char)n;
        s[1] = '\0';
        return niceStringOut(s);
    }
    else {
        return NULL;
    }
}


/* The ad hoc scanner's global state */
char in_Line[LINESIZE];
char *in_LinePtr;
int lineLen;
int inCol;
long long int num;
char word[WORDSIZE];
int wordset;  // bool that says if word was set last (truly horrible, needs total rewrite)
char ch;
/********************************************/

void printVersion()
{
    printf("%s (enter h for help)\n", versionNumber);
    printf("Data Addresses: 0-%d\n", DADDR_SIZE-1);
    printf("Instruction Addresses: 0-%d\n", IADDR_SIZE-1);
    printf("Instruction Execution Limit: %d\n", abortLimit);
    printf("Output Instruction Limit: %d\n", outputLimit);
    fflush(stdout);
}


STEPRESULT setDMem(int m, long long int value) {
//    printf("setDMem: %d %lld\n", m, value);
    if (dMemTag[m]==READONLY) {
        printf("ERROR(setDMem): instruction at addr %d attempting to set data memory marked as read only at loc: %d\n", pc, m);
        exit(1);
    }
    if (m<0 ||  m>=DADDR_SIZE) {
        printf("ERROR(setDMem): instruction at addr %d attempting to set out of bounds data memory at loc: %d\n", pc, m);
        exit(1);
    }

    dMem[m] = value;
    dMemTag[m] = pc;
    dMemCmt[m] = iMem[pc].comment;
    return srOKAY;
}



long long int getDMem(int m) {
    if (m<0 ||  m>=DADDR_SIZE) {
        printf("ERROR(getDMem): instruction at addr %d attempting to get out of bounds data memory at loc: %d\n", pc, m);
        
        exit(1);
    }
    else {
        return dMem[m];
    }
}


/********************************************/
int opClass(int c)
{
    if (c <= (int)opRRLim) return opclRR;
    else if (c <= (int)opRALim) return opclRA;
    else return opclLIT;
}



/********************************************/
void writeInstruction(int loc, int trace)
{
//DEBUG    printf("PC: %d  R7: %lld  loc: %d\n", pc, reg[7], loc);
    printf("%4d: ", loc);
    if ((loc >= 0) && (loc<IADDR_SIZE)) {
	printf("%4s%3lld,", opCodeTab[iMem[loc].iop], iMem[loc].iarg1);
	switch (opClass(iMem[loc].iop)) {
	case opclRR:
	    printf("%3lld, %1lld ", iMem[loc].iarg2, iMem[loc].iarg3);
	    if (trace) {
                printf(" | ");
                {
                    int i;
                    for (i=0; i<7; i++) printf(" r[%1d]:%-3lld", i, reg[i]);
                }
                printf(" | ");
	    }
	    break;
	case opclRA:
	    printf("%4lld(%1lld)", iMem[loc].iarg2, iMem[loc].iarg3);
	    if (trace) {
                long long int tmp;

                printf(" | ");
                {
                    int i;
                    for (i=0; i<7; i++) printf(" r[%1d]:%-3lld", i, reg[i]);
                }
/*   zzz   */
                tmp = iMem[loc].iarg2 + reg[iMem[loc].iarg3];
                if ((tmp >= 0) && (tmp<DADDR_SIZE)) {

                    printf(" m[%lld]:%-3lld",
                           iMem[loc].iarg2 + reg[iMem[loc].iarg3],
                           dMem[iMem[loc].iarg2 + reg[iMem[loc].iarg3]]);
                    printf(" | ");
                }
            }
	    break;
	}
        if (breakpoint == loc || savedbreakpoint == loc) printf(" %s", "<-[break]");
        if (reg[7] == loc && !trace) printf(" %s", "<-[pc]");
	printf(" %s\n", iMem[loc].comment);
    }
    fflush(stdout);
}				/* writeInstruction */



/********************************************/
/* get the next character
*/
int getCh()
{
//    printf("LINE: \'%s\'  LINELEN: %d INCOL: %d\n", in_Line, lineLen, inCol);
    if (++inCol<lineLen) {
	ch = in_Line[inCol];
        return 1;
    }
    else {
	ch = ' ';
        return 0;
    }
}



/********************************************/
/* span a bunch of whitespace
*/
int nonBlank(void)
{
    while ((inCol<lineLen) &&
	   ((in_Line[inCol] == ' ') || (in_Line[inCol] == '\t'))) inCol++;
    if (inCol<lineLen) {
	ch = in_Line[inCol];
	return TRUE;
    }
    else {
	ch = ' ';
	return FALSE;
    }
}

/********************************************/
/* span a bunch of whitespace
*/
int uptoComment(void)
{
    while ((inCol<lineLen) && (in_Line[inCol] != '*')) inCol++;
    if (inCol<lineLen) {
	ch = in_Line[inCol];
	return TRUE;
    }
    else {
	ch = ' ';
	return FALSE;
    }
}


//  returns the numerical equivalent of a character in num.
//  returns success or failure in function value
//
void getCleanChar(void)
{
        getCh();
        if (ch == '\\') {
            getCh();
            if (ch == '0') num = '\0';
            else if (ch == 't') num = '\t';
            else if (ch == 'n') num = '\n';
            else if (ch == '\\') num = '\\';
            else if (ch == '\'') num = '\'';
            else num = ch;
        }
        else if (ch == '^') {
            getCh();
            num = ch;
            num ^= 0x40;
        }
        else {
            num = ch;
        }
}


// return a string in word[]
int getString(void)
{
    int i;
    int ok = FALSE;
    if (ch == '"') {
        i = 0;
        do {
            getCleanChar();
            word[i++] = ch;
        } while (ch != '"');
        word[i-1] = '\0';
        ok = TRUE;
    }

    return ok;
}

int getChar(void)
{
    int ok = FALSE;

    num = 0;
    if (ch == '\'') {
        getCleanChar();
        getCh();
        if (ch == '\'') {
            ok = TRUE;
            getCh();
        }
    }

    return ok;
}

//  returns the number in num.
// returns success or failure in function value
//
int getNum(void)
{
    int sign;
    long long int term;
    int ok = FALSE;

    num = 0;
    nonBlank();
    do {
	sign = 1;
	while ((ch == '+') || (ch == '-')) {
	    ok = FALSE;
	    if (ch == '-')
		sign = -sign;
	    getCh();
	}
	term = 0;
	while (isdigit(ch)) {
	    ok = TRUE;
	    term = term*10 + (ch - '0');
	    getCh();
	}
	num = num + (term*sign);
    }
    while ((ch == '+') || (ch == '-'));

//    printf("NUM: %d\n", num);
    return ok;
}				/* getNum */





/********************************************/
int getNumOrChar(void)
{
    nonBlank();
    if ((ch == '+') || (ch == '-') || isdigit(ch)) return getNum();
    else return getChar();
}


/********************************************/
int getWord(void)
{
    int temp = FALSE;
    int length = 0;
    if (nonBlank()) {
	while (isalnum(ch) || ch=='=' || ch=='?') {
	    if (length<WORDSIZE - 1)
		word[length++] = ch;
	    getCh();
	}
	word[length] = '\0';
	temp = (length != 0);
    }
    return temp;
}				/* getWord */


/********************************************/
int getBool(void)
{
    nonBlank();

    num = 1;
    if ((ch=='F') || (ch=='f') || (ch=='0')) num = 0;
    getWord();

    return TRUE;
}


/********************************************/
int skipCh(char c)
{
    int temp = FALSE;
    if (nonBlank() && (ch == c)) {
	getCh();
	temp = TRUE;
    }
    return temp;
}				/* skipCh */



/********************************************/
/* note this returns a duplicate string and not true or false */
char *getRemaining(void)
{
    skipCh(')');
    if (nonBlank()) return strdup(&in_Line[inCol]);
    return emptyString;
}



/********************************************/
int atEOL(void)
{
    return (!nonBlank());
}				/* atEOL */



/********************************************/
int error(char *msg, int lineNo, int instNo)
{
    printf("ERROR: Line %d", lineNo);
    if (instNo >= 0)
	printf(" (Address: %d)", instNo);
    printf("   %s\n", msg);
    return FALSE;
}				/* error */



/* clear registers and data memory */
void clearMachine()
{
    int regNo, loc;

    iloc = 0;
    dloc = 0;
    for (regNo = 0; regNo<NO_REGS; regNo++) reg[regNo] = 0;
    reg[0] = DADDR_SIZE - 1;   // v 4.6

    for (loc = 0; loc<DADDR_SIZE; loc++) {
	dMem[loc] = 0;
	dMemTag[loc] = UNUSED;
        dMemCmt[loc] = (char *)"";
    }
// NO LONGER starting v4.6   dMem[0] = DADDR_SIZE - 1;

    dmemStart = reg[0];
    dmemCount = 20;
    dmemDown = -1;
    imemStart = 0;
    imemCount = 20;
    imemDown = +1;
    instrCount = outputInstrCount = 0;
}

/* clear registers, data and instruction memory */
void fullClearMachine()
{
    int loc;

    /* clear registers and data memory */
    clearMachine();
    savedbreakpoint = breakpoint = -1;

    /* zero out instruction memory */
    for (loc = 0; loc<IADDR_SIZE; loc++) {
	iMem[loc].iop = opHALT;
	iMem[loc].iarg1 = 0;
	iMem[loc].iarg2 = 0;
	iMem[loc].iarg3 = 0;
	iMem[loc].comment = (char *)"* initially empty";
	iMemTag[loc] = UNUSED;
    }
}


int readInstructions(char *fileName)
{
    FILE *pgm;
    OPCODE op;
    long long int arg1, arg2, arg3;
    int loc, lineNo;
    char errorString[128];

    /* load program */
    if (*fileName!='\0') strcpy(pgmName, fileName);
    if (strchr(pgmName, '.') == NULL) strcat(pgmName, (char *)".tm");
    pgm = fopen(pgmName, "r");
    if (pgm == NULL) {
	printf("ERROR(readInstructions): file '%s' not found\n", pgmName);
	return FALSE;
    }
    printf("Loading file: %s\n", pgmName);

    /* clear the way for the new program */
    fullClearMachine();

    /* load program */
    lineNo = 0;
    loc = -1;   /* fist location to load is 0 */
    /* get line */
    fgets(in_Line, LINESIZE - 2, pgm);
    while (!feof(pgm)) {
        /* process line */
	inCol = 0;
	lineNo++;
	lineLen = strlen(in_Line) - 1;
	if (in_Line[lineLen] == '\n')
	    in_Line[lineLen] = '\0';
	else
	    in_Line[++lineLen] = '\0';

        /* process an instruction */
	if ((nonBlank()) && (in_Line[inCol] != '*')) {
            /* get address */
	    if (getNum()) {
                loc = num;

                /* colon after address */
                if (!skipCh(':')) {
                    return error((char *)"Missing colon", lineNo, loc);
                }
            }
            else {   /* if no address given then just increment counter */
                loc++;
            }
	    if (loc<0 || loc>=IADDR_SIZE) {
                printf("ERROR(readInstructions): at line %d attempting to set out of bounds instruction memory at loc: %d\n", lineNo, loc);
                exit(1);
            }

            /* get op code */
	    if (!getWord())
		return error((char *)"Missing opcode", lineNo, loc);

            { int opcnt;

                for (opcnt = 0; opcnt<(int)opEND; opcnt++) {
                    if (strncmp(opCodeTab[opcnt], word, 4) == 0) break;
                }
                if (opcnt>=(int)opEND) {
                    sprintf(errorString, (char *)"Illegal opcode: %s", word);
                    return error(errorString, lineNo, loc);
                }
		op = (OPCODE)opcnt;
            }

            /* process args to op code */
            arg1 = arg2 = arg3 = 0;
            if (op==opHALT || op==opNOP) {
                uptoComment();
            }
            else {
                switch (opClass(op)) {
                case opclRR:
                    /***********************************/
                    /* arg 1 */
                    if ((!getNum()) || (num<0) || (num >= NO_REGS))
                        return error((char *)"Bad first register", lineNo, loc);
                    arg1 = num;
                    if (!skipCh(','))
                        return error((char *)"Missing comma", lineNo, loc);

                    /* arg 2 */
                    if ((!getNum()) || (num<0) || (num >= NO_REGS))
                        return error((char *)"Bad second register", lineNo, loc);
                    arg2 = num;
                    if (!skipCh(','))
                        return error((char *)"Missing comma", lineNo, loc);

                    /* arg 3 */
                    if ((!getNum()) || (num<0) || (num >= NO_REGS))
                        return error((char *)"Bad third register", lineNo, loc);
                    arg3 = num;
                    break;

                case opclRA:
                    /***********************************/
                    /* arg 1 */
                    if (!getNum() || ((num<0) || (num >= NO_REGS)))
                        return error((char *)"Bad first register", lineNo, loc);
                    arg1 = num;
                    if (!skipCh(','))
                        return error((char *)"Missing comma", lineNo, loc);

                    /* arg 2 */
                    if (!getNumOrChar())
                        return error((char *)"Bad displacement", lineNo, loc);
                    arg2 = num;
                    if (!skipCh('(') && !skipCh(',')) {
                        if (op==opLDC) {
                            break;
                        }
                        return error((char *)"Missing left paren", lineNo, loc);
                    }

                    /* arg 3 */
                    if ((!getNum()) || (num<0) || (num >= NO_REGS))
                        return error((char *)"Bad second register", lineNo, loc);
                    arg3 = num;
                    break;
                case opclLIT:
                    nonBlank();
                    (wordset = getString()) || getNum() || getChar();
                    break;
                }
                
            }

            // set data memory with LIT instruction.
            // location is at loc offset from *top* of memory!
            if (op==opLIT) {
                int dloc;

                dloc = DADDR_SIZE - 1 - loc;
                if (wordset) {
                    int len, k;

                    len = strlen(word);
                    for (k=0; k<len; k++) {
                        setDMem(dloc-k, word[k]);
                        dMemTag[dloc-k] = READONLY;
                    }
                    setDMem(dloc+1, len);
                    dMemTag[dloc+1] = READONLY;
                }
                else {
                    setDMem(dloc, num);
                    dMemTag[dloc] = READONLY;
                }
            }
            // set instruction memory
            else {
                iMem[loc].iop = op;
                iMem[loc].iarg1 = arg1;
                iMem[loc].iarg2 = arg2;
                iMem[loc].iarg3 = arg3;
                iMem[loc].comment = getRemaining();
                iMemTag[loc] = USED;     /* correctly counts assignments to same loc  */
            }
	}

        /* get next line */
        fgets(in_Line, LINESIZE - 2, pgm);
    }
    return TRUE;
}				/* readInstructions */




/********************************************/

int outputLimitFail()
{
    outputInstrCount++;
    if (outputInstrCount>outputLimit && outputLimit!=0) return 1;
    return 0;
}


STEPRESULT stepTM(void)
{
    INSTRUCTION currentinstruction;
    long long int r, s, t, d, m;
    int ok;

    pc = reg[PC_REG];
    if ((pc<0) || (pc>=IADDR_SIZE))
	return srIMEM_ERR;

    if (pc == breakpoint) {
	savedbreakpoint = breakpoint;
	breakpoint = -1;
	return srHALT;
    }
    breakpoint = savedbreakpoint;

    lastpc = pc;
    reg[PC_REG] = pc + 1;
    currentinstruction = iMem[pc];
    instrCount++;

    /* get the args to the instruction */
    if (opClass(currentinstruction.iop) == opclRR) {
        r = currentinstruction.iarg1;
        s = currentinstruction.iarg2;
        t = currentinstruction.iarg3;
    }
    else {  /* note s changes its position */
	r = currentinstruction.iarg1;
        d = currentinstruction.iarg2;
	s = currentinstruction.iarg3;
	m = currentinstruction.iarg2 + reg[s];
    }

    switch (currentinstruction.iop) {
	/* RR instructions */
    case opHALT:
        /***********************************/
	return srHALT;
	/* break; */

    case opNOP:
        break;

    case opIN:
        /***********************************/
	do {
	    if (promptflag) printf("Enter integer value: ");
	    fflush(stdin);
	    fflush(stdout);

            fgets(in_Line, LINESIZE - 2, stdin);
            {
                char *p;

                for (p=in_Line; *p; p++) {
                    if (*p=='\n') {
                        *p='\0';
                        break;
                    }
                }
                lineLen = p-in_Line;
            }

	    if (!promptflag) printf("entered: %s\n", in_Line);

	    inCol = 0;
	    ok = getNum();
	    if (!ok) {
		printf("Illegal value in input: \"%s\"\n", in_Line);
                exit(1);
            }
	    else {
		reg[r] = num;
            }
	}
	while (!ok);
	if (skipCh('#')) return srHALT;
	break;

    case opINB:
        /***********************************/
	if (promptflag) printf("Enter Boolean value: ");
	fflush(stdin);
	fflush(stdout);

	fgets(in_Line, LINESIZE - 2, stdin);
	{
	    char *p;

	    for (p=in_Line; *p; p++) {
		if (*p=='\n') {
		    *p='\0';
		    break;
		}
	    }
	    lineLen = p-in_Line;
	}

	if (!promptflag) printf("entered: %s\n", in_Line);

	inCol = 0;
	getBool();
	reg[r] = num;
	if (skipCh('#')) return srHALT;
	break;

    case opINC:
        /***********************************/
	fflush(stdin);
	fflush(stdout);

        while (inCol+1>=lineLen) {
            char *p;

	    if (promptflag) printf("Enter characters: ");
            fgets(in_Line, LINESIZE - 2, stdin);

            for (p=in_Line; *p; p++) {
                if (*p=='\n') {
//                    *p='\0';  // RH TEST
//                    *p='\n';
                    p++;  // include newline
                    break;
                }
            }
            lineLen = p-in_Line;
            inCol = -1;
        }

        if (getCh()) {
            reg[r] = ch;
        }

	break;

    case opOUT:
        if (outputLimitFail()) return srOUTPUTLIMIT_ERR;
	printf("%lld ", reg[r]);
        fflush(stdout);
	break;

    case opOUTB:
        if (outputLimitFail()) return srOUTPUTLIMIT_ERR;
	if (reg[r]) printf("T ");
	else printf("F ");
        fflush(stdout);
	break;

    case opOUTC:
        if (outputLimitFail()) return srOUTPUTLIMIT_ERR;
	printf("%c", (char)reg[r]);
        fflush(stdout);
	break;

    case opOUTNL:
        if (outputLimitFail()) return srOUTPUTLIMIT_ERR;
	printf("\n");
        fflush(stdout);
	break;

    case opADD:
	reg[r] = reg[s] + reg[t];
	break;

    case opSUB:
	reg[r] = reg[s] - reg[t];
	break;

    case opMUL:
	reg[r] = reg[s]*reg[t];
	break;

    case opDIV:
	if (reg[t] != 0)
	    reg[r] = reg[s]/reg[t];
	else
	    return srZERODIVIDE;
	break;

    case opMOD:
	if (reg[t] != 0) {
            long long int tmp;  // r may equal t

	    tmp = reg[s]%reg[t];
            if (tmp<0) tmp += llabs(reg[t]);  // always return a nonnegative answer
	    reg[r] = tmp;
        }
	else
	    return srZERODIVIDE;
	break;

    case opAND:
	reg[r] = reg[s]&reg[t];
	break;

    case opOR:
	reg[r] = reg[s]|reg[t];
	break;

    case opXOR:
	reg[r] = reg[s]^reg[t];
	break;

    case opNOT:
	reg[r] = ~reg[s];
	break;

    case opNEG:
	reg[r] = -reg[s];
	break;

    case opSWP:
        if (reg[r]>reg[s]) {
            long long int tmp;
            tmp = reg[r];
            reg[r] = reg[s];
            reg[s] = tmp;
        }
	break;

    case opRND:
	if (reg[s] != 0)
            reg[r] = random()%llabs(reg[s]);
	else
	    return srZERODIVIDE;
	break;

    case opMOV: {
        int raddr, saddr;
        int i;

        raddr = reg[r];
        saddr = reg[s];
        for (i=0; i<reg[t]; i++) {
            setDMem(raddr, getDMem(saddr));
            raddr--;
            saddr--;
        }
    }
        break;

    case opSET: {
        int raddr, svalue;
        int i;

        raddr = reg[r];
        svalue = reg[s];
        for (i=0; i<reg[t]; i++) {
            setDMem(raddr, svalue);
            raddr--;
        }
    }
        break;

    
    // find the first place that is different put the differing mem
    // values in r and s.   Memory is checked from high address
    // to low!  If the length of the comparison (stored in reg[t]) is
    // zero then r and s will be set to 0.
    case opCO: {
        int raddr, saddr;
	int i;

        raddr = reg[r];
        saddr = reg[s];
        if (reg[t]==0) {
            reg[r] = reg[s] = 0;
        }
        else {
            for (i=0; i<reg[t]; i++) {
                reg[r] = getDMem(raddr);
                reg[s] = getDMem(saddr);
                if (reg[r] != reg[s]) break;
                raddr--;
                saddr--;
            }
        }
    }
    break;

    // find the addresses of the first place that is different put the addresses of 
    // the differing mem values in r5 and r6
    case opCOA: {
        int raddr, saddr;
	int i;

        raddr = reg[r];
        saddr = reg[s];
        for (i=0; i<reg[t]; i++) {
            reg[r] = raddr;
            reg[s] = saddr;
            if (getDMem(raddr) != getDMem(saddr)) break;
            raddr--;
            saddr--;
        }
    }
        break;

        /*************** RA instructions ********************/
    case opLD:
	reg[r] = getDMem(m);
	break;
    case opST:
        setDMem(m, reg[r]);
	break;
    case opLDA:
	reg[r] = m;
	break;
    case opLDC:
	reg[r] = d;
	break;
    case opTLT:
        reg[r] = (reg[s]<reg[t] ? 1 : 0);
	break;
    case opSLT:
        if (reg[r]>=0) reg[r] = (reg[s]<reg[t] ? 1 : 0);
        else reg[r] = (-reg[s] < -reg[t] ? 1 : 0);
	break;
    case opTGT:
        reg[r] = (reg[s]>reg[t] ? 1 : 0);
	break;
    case opSGT:
        if (reg[r]>=0) reg[r] = (reg[s]>reg[t] ? 1 : 0);
        else reg[r] = (-reg[s] > -reg[t] ? 1 : 0);
	break;
    case opTLE:
        reg[r] = (reg[s]<=reg[t] ? 1 : 0);
	break;
    case opTGE:
        reg[r] = (reg[s]>=reg[t] ? 1 : 0);
	break;
    case opTEQ:
        reg[r] = (reg[s]==reg[t] ? 1 : 0);
	break;
    case opTNE:
        reg[r] = (reg[s]!=reg[t] ? 1 : 0);
	break;
    case opJZR:
	if (reg[r] == 0)
	    reg[PC_REG] = m;
	break;
    case opJNZ:
	if (reg[r] != 0)
	    reg[PC_REG] = m;
	break;
    case opJMP:
        reg[PC_REG] = m;
	break;

	/* end of legal instructions */
    }				/* case */
    return srOKAY;
}				/* stepTM */




/********************************************/
void usage()
{
    printf("%s\n", versionNumber);
    printf("\nCommands are:\n");
    printf(" a(bortLimit <<n>>  Maximum number of instructions between halts (default is %d).\n", DEFAULT_ABORT_LIMIT);
    printf(" b(reakpoint <<n>>  Set a breakpoint for instr n.  No n means clear breakpoints.\n");
    printf(" c(lear             Reset TM for new execution of program\n");
    printf(" d(Mem <b <n>>      Print n dMem locations (counting down) starting at b (n can be negative to count up). No args means all used memory locations.\n");
    printf(" e(xecStats         Print execution statistics since last load or clear\n");
    printf(" g(o                Execute TM instructions until HALT\n");
    printf(" h(elp              Cause this list of commands to be printed\n");
    printf(" i(Mem <b <n>>      Print n iMem locations (counting up) starting at b.  No args means all used memory locations.\n");
    printf(" l(oad filename     Load filename into memory (default is last file)\n");
    printf(" n(ext              Print the next command that will be executed\n");
    printf(" o(utputLimit <<n>> Maximum combined number of calls to any output instruction (default is %d)\n", DEFAULT_OUTPUT_LIMIT);
    printf(" p(rint             Toggle printing of total number instructions executed ('go' only)\n");
    printf(" q(uit              Terminate TM\n");
    printf(" r(egs              Print the contents of the registers\n");
    printf(" s(tep <n>          Execute n (default 1) TM instructions\n");
    printf(" t(race             Toggle instruction tracing (printing) during execution\n");
    printf(" u(nprompt)         Unprompted for script input\n");
    printf(" v                  Print the version information\n");
    printf(" x(it               Terminate TM\n");
    printf(" = <r> <n>          Set register number r to value n (e.g. set the pc)\n");
    printf(" < <addr> <value>   Set dMem at addr to value\n");
    printf(" (empty line does a step)\n");
    printf("Also a # character placed after input will cause TM to halt\n  after processing the IN or INB commands (e.g. 34#  or f# )\n");
}



int doCommand(void)
{
    char cmd;
    int i;
    int printcnt;
    int stepResult;
    int loc;

    stepcnt = 0;
    do {
	if (promptflag) printf("Enter command: ");
	fflush(stdin);
	fflush(stdout);

	fgets(in_Line, LINESIZE - 2, stdin);
	if (feof(stdin)) {
	    word[0] = 'q';
	    word[1] = '\0';
	    break;
	}

	{
	    char *p;

	    for (p=in_Line; *p; p++) {
		if (*p=='\n') {
		    *p='\0';
		    break;
		}
	    }
	    lineLen = p-in_Line;
	}
	inCol = 0;
    }
    while ((lineLen>0) && !getWord());

    if (lineLen==0) {
        word[0] = 's';
        word[1] = '\0';
    }

    if (! promptflag) printf("command: %s\n", in_Line);

    cmd = word[0];
    switch (cmd) {
    case 'l':
        /***********************************/
	if (!getWord()) *word = '\0';
	readInstructions(word);
	break;

    case 't':
        /***********************************/
	traceflag = !traceflag;
	printf("Tracing now ");
	if (traceflag)
	    printf("on.\n");
	else
	    printf("off.\n");
	break;

        /***********************************/
    case 'u':
//        printf("\n");
	promptflag = FALSE;
	break;

        /***********************************/
    case 'v':
        printVersion();
	break;

    case '?':
    case 'h':
        /***********************************/
	usage();
	break;

    case 'p':
        /***********************************/
	icountflag = !icountflag;
	printf("Printing instruction count now ");
	if (icountflag)
	    printf("on.\n");
	else
	    printf("off.\n");
	break;

    case 'a':
        /***********************************/
        if (getNum()) {
	    abortLimit = llabs(num);
        }
	else {
	    abortLimit = 0;
	    printf("Abort limit turned off.\n");
        }
	break;

    case 'o':
        /***********************************/
        if (getNum()) {
	    outputLimit = llabs(num);
        }
	else {
	    outputLimit = 0;
	    printf("Output limit turned off.\n");
        }
	break;

    case 's':
        /***********************************/
	if (atEOL())
	    stepcnt = 1;
	else if (getNum())
	    stepcnt = llabs(num);
	else
	    printf("Step count?\n");
        if (! traceflag) writeInstruction(reg[7], NOTRACE);
        break;

    case 'e':
        /***********************************/
    { int cnt;
            printf("EXEC STAT: Number of instructions executed: %d\n", instrCount);
            printf("EXEC STAT: Number of output instructions executed: %d\n", outputInstrCount);

	    cnt = 0;
	    for (i = 0; i<IADDR_SIZE; i++) if (iMemTag[i]==USED) cnt++;
	    printf("EXEC STAT: Instruction memory used: %d\n", cnt);

	    cnt = 0;
	    for (i = 0; i<IADDR_SIZE; i++) if (dMemTag[i]>=0) cnt++;
	    printf("EXEC STAT: Data memory touched: %d\n", cnt);

	    cnt = 0;
	    for (i = 0; i<IADDR_SIZE; i++) if (dMemTag[i]==READONLY) cnt++;
	    printf("EXEC STAT: Read only memory: %d\n", cnt);
    }
    break;

    case 'g':
        /***********************************/
	stepcnt = 1;
	break;

    case 'r':
        /***********************************/
	for (i = 0; i<NO_REGS; i++) {
	    printf("r[%1d]: %-4lld   ", i, reg[i]);
	    if ((i%4) == 3) printf("\n");
	}
	break;

    case '=':
        /***********************************/
	if (getNum()) {
	    loc = num;
	    if (getNum()) {
		if (loc<0 || loc>=NO_REGS) printf("%d is not a legal register number\n", loc);
		else reg[loc] = num;
	    }
	    else printf("Register value?\n");
	}
        else printf("Register number?\n");
        break;

        /***********************************/
    case 'n':
	iloc = reg[PC_REG];
	if ((iloc >= 0) && (iloc<IADDR_SIZE)) writeInstruction(iloc, TRACE);
	break;

    case 'i':
        /***********************************/
    {
        int usedonly;

        usedonly = 1;
        imemStart = 0;
        imemCount = IADDR_SIZE;
        dmemDown = 1;
        if (getNum()) {
            usedonly = 0;
            imemStart = num;
            if (getNum()) {
                imemDown = +1;
                if (num<0) imemDown = -1;
                imemCount = llabs(num);
            }
        }
        iloc = imemStart;
        printcnt = imemCount;

        for (i=0; i<printcnt; i++, iloc+=imemDown) {
            iloc = (DADDR_SIZE + iloc) % DADDR_SIZE;
            if (! usedonly || iMemTag[iloc]!=UNUSED) {
                writeInstruction(iloc, NOTRACE);
            }
        }
    }
    break;
        
    case 'd':
        /***********************************/
    { 
        int usedonly;

        usedonly = 1;
        dmemStart = DADDR_SIZE-1;
        dmemCount = DADDR_SIZE;
        dmemDown = -1;
        if (getNum()) {
            usedonly = 0;
            dmemStart = num;
            if (getNum()) {
                dmemDown = +1;
                if (num<0) dmemDown = -1;
                dmemCount = llabs(num);
            }
        }
        dloc = dmemStart;
        printcnt = dmemCount;
        printf("%5s: %5s", "addr", "value");
        printf("    %s\n", "instr that last assigned this loc");
        for (i=0; i<printcnt; i++, dloc+=dmemDown) {
            char *c;

            dloc = (DADDR_SIZE + dloc) % DADDR_SIZE;
            if (! usedonly || dMemTag[dloc]!=UNUSED) {
                c = niceChar(dMem[dloc]);
                if (c) printf("%5d: %5lld '%s'", dloc, dMem[dloc], c);
                else printf("%5d: %5lld %3s", dloc, dMem[dloc], "");

                if (dMemTag[dloc]>=0)
                    printf("    %3d %s\n", dMemTag[dloc], dMemCmt[dloc]);
                else if (dMemTag[dloc]==UNUSED) printf("    %s\n", "unused");
                else printf("    %s\n", "readOnly");
            }
        }
    }
    break;

    case '<':
            if (getNum()) {
                dloc = num;
                getNum();
            }
            if (dloc >= 0 && dloc<DADDR_SIZE) {
                dMem[dloc] = num;
            }
            break;

    case 'b':
	if (atEOL()) {
	    savedbreakpoint = breakpoint = -1;
	}
	else if (getNum())
	    savedbreakpoint = breakpoint = llabs(num);
	else
	    printf("Breakpoint location?\n");
	break;


    case 'c':
        /***********************************/
	clearMachine();
	lastpc = 0;
        stepcnt = 0;
	break;

    case 'q':
    case 'x':
	return FALSE;		/* break; */

    default:
	printf("ERROR: TM Command %c unknown.\n", cmd);
//	usage();
	break;
    }				/* case */

    stepResult = srOKAY;
    if (stepcnt>0) {
	if (cmd == 'g') {
            outputInstrCount = stepcnt = 0;
//	    stepcnt = 0;
	    while ((stepResult == srOKAY) && ((abortLimit==0) || (stepcnt<abortLimit))) {
		iloc = reg[PC_REG];
		stepResult = stepTM();
		if (traceflag) writeInstruction(iloc, TRACE);
		stepcnt++;
	    }
	    if ((stepcnt>=abortLimit) && (abortLimit!=0)) {
		stepResult = srHALT;
		printf("Abort limit reached! (limit = %d) (see 'a' command in help).\n", abortLimit);
	    }
	    if (icountflag)
		printf("Number of instructions executed = %d\n", stepcnt);
	}
	else {
	    while ((stepcnt>0) && (stepResult == srOKAY)) {
		iloc = reg[PC_REG];
		stepResult = stepTM();
		if (traceflag) writeInstruction(iloc, TRACE);
		stepcnt--;
	    }
	}

	printf("\nStatus: %s\n", stepResultTab[stepResult]);
	if (stepResult!=srOKAY) {
	    printf("Last executed cmd: ");
	    writeInstruction(lastpc, TRACE);
	}
	printf("PC was %d, PC is now %lld\n", lastpc, reg[PC_REG]);
    }
    return TRUE;
}				/* doCommand */



/********************************************/
/* E X E C U T I O N   B E G I N S   H E R E */
/********************************************/

int main(int argc, char *argv[])
{
    srandom(getpid()*332+1);
    initOpCodeTab();

    //printVersion();

    /* guarantee a full clear even if the file load fails */
    fullClearMachine();

    /* read the program if supplied as an argument */
    if (argc == 2) readInstructions(argv[1]);

    /* do stuff */
    while (doCommand());

    printf("Bye.\n");

    return 0;
}
