#ifndef SCANTYPE_H
#define SCANTYPE_H

struct TokenData {
   int tokenclass; // token class
   int linenum;    // line where found
   char *tokenstr; // what string was actually read
   char cvalue;    // any character value
   int nvalue;     // any numeric value or Boolean value
   char *svalue;   // any string value e.g. an id
};

#endif /* SCANTYPE_H */
