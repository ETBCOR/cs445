%{
#include "symtbl.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
extern void yyerror(char*);
%}

%union {
    double dval;
    struct sym * symptr;
}

%token <symptr> NAME
%token <dval> NUMBER
%left '-' '+'
%left '*' '/'
%nonassoc UMINUS

%type <dval> expression
%%
statement_list
    : statement '\n'
    | statement_list statement '\n'
    ;

statement
    : NAME '=' expression { $1->value = $3; }
    | expression { printf("= %g\n", $1); }
    | '?' { printf("num-syms: %d\n", sym_count()); }
    ;

expression
    : expression '+' expression { $$ = $1 + $3; }
    | expression '-' expression { $$ = $1 - $3; }
    | expression '*' expression { $$ = $1 * $3; }
    | expression '/' expression {
        if ($3 == 0.0) /* need to check for div by 0 */
            yyerror("divide by zero");
        else
        $$ = $1 / $3;
        }
    | '-' expression %prec UMINUS { $$ = -$2; }
    | '(' expression ')' { $$ = $2; }
    | NUMBER
    | NAME { $$ = $1->value; }
    ;

%%

struct sym * sym_lookup_old(char * s)
{
    char * p;
    struct sym * sp;

    for (sp=sym_tbl; sp < &sym_tbl[NSYMS]; sp++)
    {
        if (sp->name && strcmp(sp->name, s) == 0)
            /* it's already here */
            return sp;

        if (sp->name)
            /* skip to next */
            continue;

        sp->name = strdup(s);
        return sp; 
    }
   
    yyerror("Too many symbols");
    exit(-1);
    return NULL; /* unreachable */
}

struct sym * sym_lookup(char * s)
{
    struct sym * p;
    struct sym * tmp;

    if (sym_tbl == NULL)
    {
        /* alloc new sym */
        tmp = malloc(sizeof(struct sym));
        tmp->name = strdup(s);
        tmp->value = 0.0;
        tmp->next = NULL;
        /* insert sym into list of syms */
        sym_tbl = tmp;
        /* return rslt */
        return tmp;
    }

    for (p=sym_tbl; p; p=p->next)
    {
        if (strcmp(p->name, s) == 0)
            return p;

        if (p->next == NULL)
        {
            /* alloc new sym */
            tmp = malloc(sizeof(struct sym));
            tmp->name = strdup(s);
            tmp->value = 0.0;
            tmp->next = NULL;
            /* insert sym into list of syms */
            p->next = tmp;
            /* return rslt */
            return tmp;
        }
    }

    
}

int sym_count(void)
{
    int n;
    struct sym * p;

    for (n=0,p=sym_tbl; p; p=p->next,n++)
        ;
    return n;
}

int main()
{
    sym_tbl = 0;

    yyparse();
    return 0;
}

void yyerror(char* s)
{
    printf("yyerror: \"%s\"\n", s);
}

