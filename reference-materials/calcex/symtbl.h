#ifndef SYMTBL_H
#define SYMTBL_H __DATE__" "__TIME__

#define NSYMS   (32)

struct sym {
    char * name;
    double value;
    struct sym * next;
} * sym_tbl;

int sym_count(void);
struct sym * sym_lookup(char *);

#endif /* SYMTBL_H */
