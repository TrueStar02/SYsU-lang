/* A Bison parser, made by GNU Bison 3.7.5.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSER_HH_INCLUDED
# define YY_YY_PARSER_HH_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 2 "/workspace/SYsU-lang/parser/parser.y"

class Tree;

#line 53 "parser.hh"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    T_NUMERIC_CONSTANT = 258,      /* T_NUMERIC_CONSTANT  */
    T_IDENTIFIER = 259,            /* T_IDENTIFIER  */
    T_INT = 260,                   /* T_INT  */
    T_FLOAT = 261,                 /* T_FLOAT  */
    T_LONG = 262,                  /* T_LONG  */
    T_CHAR = 263,                  /* T_CHAR  */
    T_RETURN = 264,                /* T_RETURN  */
    T_SEMI = 265,                  /* T_SEMI  */
    T_L_PAREN = 266,               /* T_L_PAREN  */
    T_R_PAREN = 267,               /* T_R_PAREN  */
    T_L_BRACE = 268,               /* T_L_BRACE  */
    T_R_BRACE = 269,               /* T_R_BRACE  */
    T_CONST = 270,                 /* T_CONST  */
    T_COMMA = 271,                 /* T_COMMA  */
    T_L_SQUARE = 272,              /* T_L_SQUARE  */
    T_R_SQUARE = 273,              /* T_R_SQUARE  */
    T_EQUAL = 274,                 /* T_EQUAL  */
    T_VOID = 275,                  /* T_VOID  */
    T_IF = 276,                    /* T_IF  */
    T_ELSE = 277,                  /* T_ELSE  */
    T_WHILE = 278,                 /* T_WHILE  */
    T_DO = 279,                    /* T_DO  */
    T_FOR = 280,                   /* T_FOR  */
    T_BREAK = 281,                 /* T_BREAK  */
    T_CONTINUE = 282,              /* T_CONTINUE  */
    T_TILDE = 283,                 /* T_TILDE  */
    T_PLUS = 284,                  /* T_PLUS  */
    T_MINUS = 285,                 /* T_MINUS  */
    T_EXCLAIM = 286,               /* T_EXCLAIM  */
    T_STAR = 287,                  /* T_STAR  */
    T_SLASH = 288,                 /* T_SLASH  */
    T_PERCENT = 289,               /* T_PERCENT  */
    T_LESS = 290,                  /* T_LESS  */
    T_GREATER = 291,               /* T_GREATER  */
    T_LESSEQUAL = 292,             /* T_LESSEQUAL  */
    T_GREATEREQUAL = 293,          /* T_GREATEREQUAL  */
    T_EQUALEQUAL = 294,            /* T_EQUALEQUAL  */
    T_EXCLAIMEQUAL = 295,          /* T_EXCLAIMEQUAL  */
    T_AMPAMP = 296,                /* T_AMPAMP  */
    T_PIPEPIPE = 297,              /* T_PIPEPIPE  */
    T_AMP = 298,                   /* T_AMP  */
    T_CARET = 299,                 /* T_CARET  */
    T_PIPE = 300,                  /* T_PIPE  */
    T_LESSLESS = 301,              /* T_LESSLESS  */
    T_GREATERGREATER = 302,        /* T_GREATERGREATER  */
    T_ELLIPSIS = 303,              /* T_ELLIPSIS  */
    T_STRING_LITERAL = 304,        /* T_STRING_LITERAL  */
    T_PLUSEQUAL = 305,             /* T_PLUSEQUAL  */
    T_MINUSEQUAL = 306,            /* T_MINUSEQUAL  */
    T_STAREQUAL = 307,             /* T_STAREQUAL  */
    T_SLASHEQUAL = 308,            /* T_SLASHEQUAL  */
    T_PERCENTEQUAL = 309,          /* T_PERCENTEQUAL  */
    T_AMPEQUAL = 310,              /* T_AMPEQUAL  */
    T_CARETEQUAL = 311,            /* T_CARETEQUAL  */
    T_PIPEEQUAL = 312,             /* T_PIPEEQUAL  */
    T_LESSLESSEQUAL = 313,         /* T_LESSLESSEQUAL  */
    T_GREATERGREATEREQUAL = 314,   /* T_GREATERGREATEREQUAL  */
    T_PLUSPLUS = 315,              /* T_PLUSPLUS  */
    T_MINUSMINUS = 316             /* T_MINUSMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef  Tree*  YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_PARSER_HH_INCLUDED  */
