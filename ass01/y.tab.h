#define STATIC 257
#define BOOL 258
#define CHAR 259
#define INT 260
#define IF 261
#define THEN 262
#define ELSE 263
#define WHILE 264
#define FOR 265
#define DO 266
#define TO 267
#define BY 268
#define RETURN 269
#define BREAK 270
#define AND 271
#define OR 272
#define EQ 273
#define NEQ 274
#define LEQ 275
#define GEQ 276
#define ADDASS 277
#define SUBASS 278
#define MULASS 279
#define DIVASS 280
#define DEC 281
#define INC 282
#define NOT 283
#define BOOLCONST 284
#define ID 285
#define NUMCONST 286
#define CHARCONST 287
#define STRINGCONST 288
#define CATCH 289
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union {
    struct TokenData *tokenData;
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
extern YYSTYPE yylval;
