#include "ourgetopt.h"
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>

// this program reads in a command line
// aflg gets set if there is a -a option
// bflg gets set if there is a -b option
// zflg gets set if there is a -z option
//
// a and b options are mutually exclusive
// z prints out when it is discovered
// o option takes a file name and prints out when it is discovered
// extraneous words are considered files are printed out
//
// this program was "borrowed" and modified from the Sun Solaris man
// page for educational purposes only.
//
//  compile:  g++ ourGetopt.cpp
//  test: a.out  ali -a bob -z -z carol -z -o don ella -o frida gill
//
// Robert Heckendorn  Mar 6, 2006
//
int main(int argc, char **argv)
{
    int c;
    extern char *optarg;
    extern int optind;
    int aflg, bflg, zflg;
    int errflg;
    char *ofile;

    aflg = bflg = zflg = errflg = 0;
    ofile = NULL;

    while (1) {

        // hunt for a string of options
        while ((c = ourGetopt(argc, argv, (char *)"abo:z")) != EOF)
            switch (c) {
            case        'a':
                if (bflg) errflg = 1;
                else aflg = 1;
                break;
            case        'b':
                if (aflg) errflg = 1;
                else bflg = 1;
                break;
            case        'o':
		if (ofile) {
		    printf("-o option can be used only once.  Last use is used.\n");
		}
                ofile = strdup(optarg);
                printf("ofile = %s\n", ofile);
                break;
            case        'z':
		zflg = 1;
                printf("option Z!\n");
                break;
            case        '?':
                errflg = 1;
            }

        // report any errors or usage request
        if (errflg) {
            (void)fprintf(stderr, "usage: cmd [-a|-b] [-z] [-o <filename>] files...\n");
            exit(2);
        }

        // pick off a nonoption
        if (optind < argc) {
            (void)printf("file: %s\n", argv[optind]);
            optind++;
        }
        else {
            break;
        }
    }

    if (aflg) printf("option 'a' was found\n");
    if (bflg) printf("option 'b' was found\n");
    if (zflg) printf("option 'z' was found\n");
    if (ofile) printf("option 'o' was found for file %s\n", ofile);

    return 0;
}
