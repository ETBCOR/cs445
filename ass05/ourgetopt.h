#ifndef _OURGETOPT_H_
#define _OURGETOPT_H_
// do not include in outGetopt.cpp
int ourGetopt( int, char **, char*);
extern char *optarg;                   /* option argument if : in opts */
//extern int optind = 1;                 /* next argv index              */
//extern int opterr = 1;                 /* show error message if not 0  */
extern int optopt;                     /* last option (export dubious) */
#endif
