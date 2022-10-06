#include "main.h"

int main (int argc, char *argv[])
{
    errCount = wrnCount = 0;

    AST = new Node();
    AST->index = -1;

    // read in flags
    int dOpt = 0, DOpt = 0, pOpt = 0, POpt = 0, hOpt = 0;
    int flg;
    while ((flg = ourGetopt(argc, argv, (char *)"dDpPh")) != EOF) {
        switch (flg) {
            case 'd': dOpt = 1; break;
            case 'D': DOpt = 1; break;
            case 'p': pOpt = 1; break;
            case 'P': POpt = 1; break;
            case 'h': hOpt = 1; break;
        }
    }

    // print the usage message if the h flag was raised
    if(hOpt) printf("usage: -c [options] [sourcefile]\noptions:\n-d          - turn on parser debugging\n-D          - turn on symbol table debugging\n-h          - print this usage messagea\n-p          - print the abstract syntax tree\n-P          - print the abstract syntax tree plus type information\n");

    // turn on parser debugging
    if(dOpt) yydebug = 1;

    // do parsing
    if (optind < argc) {
        // a file was specified on the command line, so read it
        yyin = fopen(argv[optind], "r");
        yyparse();
        fclose(yyin);
    } else {
        // no command line file was specified, so read from the input stream instead
        yyparse();
    }

    // check that the AST was generated
    if(!AST) {
        // if AST wasn't generated, print error mesasage and exit
        printf("ERROR: No abstract syntax tree was generated.\n");
        return -1;
    }

    // create the symbol table
    SymbolTable * syms = new SymbolTable();

    // set the symbol table's debug flag 
    syms->debug(DOpt);

    if(errCount == 0) {
        AST->updateLoc();

        //if(pOpt) AST->print(POpt);
        if(pOpt) AST->print();
        AST->sem(syms);
    }

    printf("Number of warnings: %d\nNumber of errors: %d\n", wrnCount, errCount);

    return 0;
}

