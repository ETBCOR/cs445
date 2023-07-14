// 									
//  ourGetopt.c:	Derived from AT&T public domain source of ourGetopt(3),	
// 		modified for use with MS C 6.0 on MS DOS systems. For	
// 		unknown reasons the variable optopt is exported here.	
// 									
// 	Note that each option may occur more than once in the command	
// 	line, this may require special action like occurence counting.	
// 	Each option is indicated by a single character in opts string	
// 	followed by : if an option argument is required. So for "abo:"
// 	the following combinations are possible:			
// 		-a -b -o value	sets a, b, and argument value for o	
// 		-ab -o value	equivalent				
// 		-ab -ovalue	equivalent, but not recommended 	
// 		-abovalue	equivalent, but not recommended 	
// 		-a -- -b	sets only a, optind advanced to -b	
// 		-a - -b 	sets only a, optind stays at single -	
// 		-A		error message for A, returned as ?	
// 		-o		error message if no more arguments	
// 									
//  example use acode:							
// 	...								
// 	extern int ourGetopt( int, char **, char*);			
// 	...								
// 	int main( int argc, char *argv[] )				
// 	{								
// 		extern int   opterr;					
// 		extern int   optind;					
// 		extern char *optarg;					
// 		int c,	aset = 0,  bset = 0;				
// 		char *oarg = NULL;					
// 									
// 		while (( c = ourGetopt( argc, argv, (char *)"abo:" ) != -1 ) {
// 		switch ( c )						
// 		{	case 'a':                                     
// 				++aset; 	break;		
// 			case 'b':                                     
// 				++bset; 	break;		
// 			case 'o':                                     
// 				oarg = optarg;	break;		
// 			default:					
// 			       ...		return 1;		
//
//                case '?': NOT EXPLICITLY NEEDED WITH DEFAULT    
//		     
//                case ':': WILL NEVER HAPPEN, ':' NOT ALLOWED    
//
//                case '-': WILL NEVER HAPPEN, '-' NOT ALLOWED 
//              }                                                       
//            }
//              ...                                                     
//      }                                                               
//
//

#include	<string.h>
#include	<stdio.h>

#if	M_I8086 || M_I286 || MSDOS	/* test Microsoft C definitions */
#define 	SWITCH	'/'	/* /: only used for DOS */
#else
#define 	SWITCH	'-'	/* -: always recognized */
#endif

/* ------------ EXPORT variables -------------------------------------- */

char *optarg;			/* option argument if : in opts */
int optind = 1;			/* next argv index              */
int opterr = 1;			/* show error message if not 0  */
int optopt;			/* last option (export dubious) */

/* ------------ private section --------------------------------------- */

static int sp = 1;		/* offset within option word    */

static int badopt(char *name, char *text)
{
    if (opterr)			/* show error message if not 0      */
	fprintf(stderr, "%s: %s -- %c\n", name, text, optopt);

    return (int) '?';		/* ?: result for invalid option */
}

/* ------------ EXPORT function --------------------------------------- */

int ourGetopt(int argc, char **argv, char *opts)
{
    char *cp, ch;

    if (sp == 1) {
	if (argc <= optind || argv[optind][1] == '\0')
	    return EOF;		/* no more words or single '-'  */


	if ((ch = argv[optind][0]) != '-' && ch != SWITCH)
	    return EOF;		/* options must start with '-'  */

	if (!strcmp(argv[optind], "--")) {
	    ++optind;		/* to next word */
	    return EOF;		/* -- marks end */
	}
    }

    optopt = (int) (ch = argv[optind][sp]);	/* flag option  */

    if (ch == ':' || (cp = strchr(opts, ch)) == NULL) {
	if (argv[optind][++sp] == '\0') {
	    ++optind;
	    sp = 1;		/* to next word */
	}

	return badopt(argv[0], (char *)"illegal option");
    }

    if (*++cp == ':') {		/* ':' option requires argument */
	optarg = &argv[optind][sp + 1];	/* if same word */
	++optind;
	sp = 1;			/* to next word */

	if (*optarg == '\0') {	/* in next word */
	    if (argc <= optind)	/* no more word */
		return badopt(argv[0], (char *)"option requires an argument");

	    optarg = argv[optind++];	/* to next word */
	}
    }
    else {			/* flag option without argument */
	optarg = NULL;

	if (argv[optind][++sp] == '\0') {
	    optind++;
	    sp = 1;		/* to next word */
	}
    }

    return optopt;
}
