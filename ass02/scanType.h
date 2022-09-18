#ifndef SCANTYPE_H
#define SCANTYPE_H

class TokenData
{
public:
   int tokenclass; // token class uses flex/yacc token numbers
   int linenum;    // line where found
   char *tokenstr; // what string was actually read
   int nvalue;     // any numeric value or Boolean value
   char cvalue;    // any character value
   char *svalue;   // any string value e.g. an id
};

#endif /* SCANTYPE_H */
