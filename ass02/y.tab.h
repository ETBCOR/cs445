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
#define PLUS 273
#define MINUS 274
#define EQ 275
#define NEQ 276
#define LEQ 277
#define GEQ 278
#define LESS 279
#define GRTR 280
#define ASS 281
#define ADDASS 282
#define SUBASS 283
#define MULASS 284
#define DIVASS 285
#define DEC 286
#define INC 287
#define NOT 288
#define ASTR 289
#define SLASH 290
#define PERC 291
#define QUEST 292
#define PARENL 293
#define PARENR 294
#define BRACEL 295
#define BRACER 296
#define BRACKL 297
#define BRACKR 298
#define SEMCOL 299
#define COL 300
#define COMMA 301
#define BOOLCONST 302
#define ID 303
#define NUMCONST 304
#define CHARCONST 305
#define STRINGCONST 306
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
