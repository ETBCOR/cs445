#include "main.h"

int main (int argc, char *argv[])
{
    initErrorProcessing();

    errCount = wrnCount = 0;

    AST = new Node();
    AST->index = -1;

    // read in flags
    int dOpt = 0, DOpt = 0, pOpt = 0, POpt = 0, hOpt = 0, MOpt = 0, flg;
    while ((flg = ourGetopt(argc, argv, (char *)"dDpPhM")) != EOF) {
        switch (flg) {
            case 'd': dOpt = 1; break;
            case 'D': DOpt = 1; break;
            case 'p': pOpt = 1; break;
            case 'P': POpt = 1; break;
            case 'h': hOpt = 1; break;
            case 'M': MOpt = 1; break;
        }
    }

    src = strdup(argv[optind]);

    // print the usage message if the h flag was raised
    if (hOpt)
        printf("usage: -c [options] [sourcefile]\noptions:\n-d          - turn on parser debugging\n-D          - turn on symbol table debugging\n-h          - print this usage messagea\n-p          - print the abstract syntax tree\n-P          - print the abstract syntax tree plus type information\n");


    // turn on parser debugging
    if (dOpt)
        yydebug = 1;

    // do parsing
    if (optind < argc) {
        // a file was specified on the command line, so read it
        if ((yyin = fopen(src, "r"))) {
            yyparse();
            fclose(yyin);
        } else {
            printf("ERROR(ARGLIST): source file \"%s\" could not be opened.\n", src);
            errCount++;
        }
    } else {
        // no command line file was specified, so read from the input stream instead
        yyparse();
    }

    // check that the AST was generated
    if (!AST) {
        // if AST wasn't generated, print error mesasage and exit
        printf("ERROR: No abstract syntax tree was generated.\n");
        return -1;
    }

    // create the symbol table
    SymbolTable * syms = new SymbolTable();

    // set the symbol table's debug flag 
    syms->debug(DOpt);

    // do semantic analysis and AST printing
    if (errCount == 0) {
        AST->sem(syms);

        if ((POpt || MOpt) && errCount == 0)
            AST->print(true, MOpt);
        else if (pOpt)
            AST->print(false, MOpt);
    }
    if (MOpt && errCount == 0)
        printf("Offset for end of global space: %d\n", gofs);

    // do code generation
    int len = strlen(src);
    if (len <= 2 || src[len-3] != '.' || src[len-2] != 'c' || src[len-1] != '-') {
        printf("ERROR(ARGLIST): source file \"%s\" does not end in .c-\n", src);
        errCount++;
    }
    if (errCount == 0) {
        char * dest = strdup(src);
        dest[len-2] = 't';
        dest[len-1] = 'm';
        code = fopen(dest, "w+");
        AST->gen(syms);
        fclose(code);
    }
    printf("Number of warnings: %d\nNumber of errors: %d\n", wrnCount, errCount);
    return 0;
}

