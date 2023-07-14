#ifndef _SCANTYPE_H_
#define _SCANTYPE_H_

class TokenData
{
public:
   int tokenclass;  // token class uses flex/yacc token numbers
   int linenum;     // line where found
   char * tokenstr; // what string was actually read
   int nvalue;      // any numeric value or Boolean value
   char cvalue;     // any character value
   char * svalue;   // any string value e.g. an id
};

#endif /* _SCANTYPE_H_ */
