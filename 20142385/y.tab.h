/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    IDENTIFIER = 258,
    TYPE_IDENTIFIER = 259,
    FLOAT_CONSTANT = 260,
    INTEGER_CONSTANT = 261,
    CHARACTER_CONSTANT = 262,
    STRING_LITERAL = 263,
    PLUS = 264,
    MINUS = 265,
    AMP = 266,
    BARBAR = 267,
    AMPAMP = 268,
    SEMICOLON = 269,
    LSS = 270,
    GTR = 271,
    LEQ = 272,
    GEQ = 273,
    EQL = 274,
    NEQ = 275,
    LP = 276,
    RP = 277,
    LB = 278,
    RB = 279,
    LR = 280,
    RR = 281,
    PERIOD = 282,
    COMMA = 283,
    EXCL = 284,
    STAR = 285,
    SLASH = 286,
    PERCENT = 287,
    ASSIGN = 288,
    COLON = 289,
    AUTO_SYM = 290,
    STATIC_SYM = 291,
    ENUM_SYM = 292,
    IF_SYM = 293,
    ELSE_SYM = 294,
    WHILE_SYM = 295,
    CONTINUE_SYM = 296,
    BREAK_SYM = 297,
    RETURN_SYM = 298
  };
#endif
/* Tokens.  */
#define IDENTIFIER 258
#define TYPE_IDENTIFIER 259
#define FLOAT_CONSTANT 260
#define INTEGER_CONSTANT 261
#define CHARACTER_CONSTANT 262
#define STRING_LITERAL 263
#define PLUS 264
#define MINUS 265
#define AMP 266
#define BARBAR 267
#define AMPAMP 268
#define SEMICOLON 269
#define LSS 270
#define GTR 271
#define LEQ 272
#define GEQ 273
#define EQL 274
#define NEQ 275
#define LP 276
#define RP 277
#define LB 278
#define RB 279
#define LR 280
#define RR 281
#define PERIOD 282
#define COMMA 283
#define EXCL 284
#define STAR 285
#define SLASH 286
#define PERCENT 287
#define ASSIGN 288
#define COLON 289
#define AUTO_SYM 290
#define STATIC_SYM 291
#define ENUM_SYM 292
#define IF_SYM 293
#define ELSE_SYM 294
#define WHILE_SYM 295
#define CONTINUE_SYM 296
#define BREAK_SYM 297
#define RETURN_SYM 298

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
