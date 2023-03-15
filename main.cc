/* A Bison parser, made by GNU Bison 3.7.5.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30705

/* Bison version string.  */
#define YYBISON_VERSION "3.7.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 5 "/workspace/SYsU-lang/parser/parser.y"

#include "parser.hh"
#include <vector>
#include <memory>
#include <llvm/Support/JSON.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/raw_ostream.h>
#define yyerror(x)                                                             \
  do {                                                                         \
    llvm::errs() << (x);                                                       \
  } while (0)

namespace {
auto llvmin = llvm::MemoryBuffer::getFileOrSTDIN("-");
auto input = llvmin.get() -> getBuffer();
auto end = input.end(), it = input.begin();
auto wk_getline(char endline = "\n"[0]) {
  auto beg = it;
  while (it != end && *it != endline)
    ++it;
  auto len = it - beg;
  if (it != end && *it == endline)
    ++it;
  return llvm::StringRef(beg, len);
}
Tree* root;
} // namespace

// 以下树结构仅供参考，你可以随意修改或定义自己喜欢的形式
class Tree{
public:
  std::string kind;
  std::string name;
  std::string value;
  std::vector<Tree *> sons;
  Tree(std::string kind="", std::string name="", std::string value=""): kind(kind), name(name), value(value) {}
  void addSon(Tree* son){ sons.emplace_back(son); }
  void mergeSon(Tree* son){ for (auto ptr:son -> sons)sons.emplace_back(ptr); free(son);}
  llvm::json::Value toJson() const {
    llvm::json::Object tmp{
      {"kind", kind},
      {"name", name},
      {"value", value},
      {"inner", llvm::json::Array{}}
    };
    for(auto&& it: sons) tmp.get("inner")->getAsArray()->push_back(it->toJson());
    return tmp;
  }
  void print(int depth=0) const {
    yyerror("|");
    for(int i=0;i<depth;++i) yyerror(" ");
    yyerror("-"+kind+" "+name+" "+value);
    for(auto&& it: sons)
    {
      yyerror("\n");
      it->print(depth+1);
    }
    if(!depth) yyerror("\n\n");
  }
};

auto yylex() {
  auto tk = wk_getline();
  auto b = tk.find("'") + 1, e = tk.rfind("'");
  auto s = tk.substr(b, e - b).str(), t = tk.substr(0, tk.find(" ")).str();
  if (t == "numeric_constant") {
    yylval = new Tree("IntegerLiteral", "", s);
    return T_NUMERIC_CONSTANT;
  }
  if (t == "identifier") {
    yylval = new Tree("id", s);
    return T_IDENTIFIER;
  }
  if (t == "int")
    return T_INT;
  if (t == "float")
    return T_FLOAT;
  if(t == "long")
    return T_LONG;
  if(t == "char")
    return T_CHAR;
  if (t == "return")
    return T_RETURN;
  if (t == "semi")
    return T_SEMI;
  if (t == "l_paren")
    return T_L_PAREN;
  if (t == "r_paren")
    return T_R_PAREN;
  if (t == "l_brace")
    return T_L_BRACE;
  if (t == "r_brace")
    return T_R_BRACE;
  if (t == "const")
    return T_CONST;
  if (t == "comma")
    return T_COMMA;
  if (t == "l_square")
    return T_L_SQUARE;
  if (t == "r_square")
    return T_R_SQUARE;  
  if (t == "equal")
    return T_EQUAL;
  if (t == "void")
    return T_VOID;
  if (t == "if")
    return T_IF;;
  if (t == "else")
    return T_ELSE;
  if (t == "while")
    return T_WHILE;
  if (t == "do")
    return T_DO;
  if (t == "for")
    return T_FOR;
  if (t == "break")
    return T_BREAK;
  if (t == "continue")
    return T_CONTINUE;
  if (t == "tilde")
    return T_TILDE;
  if (t == "plus")
    return T_PLUS;
  if (t == "minus")
    return T_MINUS;  
  if (t == "exclaim")
    return T_EXCLAIM;
  if (t == "star")
    return T_STAR;
  if (t == "slash")
    return T_SLASH;
  if (t == "percent")
    return T_PERCENT;
  if (t == "less")
    return T_LESS;
  if (t == "greater")
    return T_GREATER;
  if (t == "lessequal")
    return T_LESSEQUAL;
  if (t == "greaterequal")
    return T_GREATEREQUAL;
  if (t == "equalequal")
    return T_EQUALEQUAL;
  if (t == "exclaimequal")
    return T_EXCLAIMEQUAL;
  if (t == "ampamp")
    return T_AMPAMP;
  if (t == "pipepipe")
    return T_PIPEPIPE;
  if (t == "amp")
    return T_AMP;
  if (t == "caret")
    return T_CARET;
  if (t == "pipe")
    return T_PIPE;
  if (t == "lessless")
    return T_LESSLESS;
  if (t == "greatergreater")
    return T_GREATERGREATER;
  if (t == "ellipsis")
    return T_ELLIPSIS;
  if (t == "string_literal")
    return T_STRING_LITERAL;
  if (t == "plusequal")
    return T_PLUSEQUAL;
  if (t == "minusequal")
    return T_MINUSEQUAL;
  if (t == "starequal")
    return T_STAREQUAL;
  if (t == "slashequal")
    return T_SLASHEQUAL;
  if (t == "percentequal")
    return T_PERCENTEQUAL;
  if (t == "ampequal")
    return T_AMPEQUAL;
  if (t == "caretequal")
    return T_CARETEQUAL;
  if (t == "pipeequal")
    return T_PIPEEQUAL;
  if (t == "lesslessequal")
    return T_LESSLESSEQUAL;
  if (t == "greatergreaterequal")
    return T_GREATERGREATEREQUAL;
  if (t == "plusplus")
    return T_PLUSPLUS;
  if (t == "minusminus")
    return T_MINUSMINUS; 
  return YYEOF;
}

int main() {
  yyparse();
  root->print();
  llvm::outs() << root->toJson() << "\n";
}

#line 268 "main.cc"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.hh"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_T_NUMERIC_CONSTANT = 3,         /* T_NUMERIC_CONSTANT  */
  YYSYMBOL_T_IDENTIFIER = 4,               /* T_IDENTIFIER  */
  YYSYMBOL_T_INT = 5,                      /* T_INT  */
  YYSYMBOL_T_FLOAT = 6,                    /* T_FLOAT  */
  YYSYMBOL_T_LONG = 7,                     /* T_LONG  */
  YYSYMBOL_T_CHAR = 8,                     /* T_CHAR  */
  YYSYMBOL_T_RETURN = 9,                   /* T_RETURN  */
  YYSYMBOL_T_SEMI = 10,                    /* T_SEMI  */
  YYSYMBOL_T_L_PAREN = 11,                 /* T_L_PAREN  */
  YYSYMBOL_T_R_PAREN = 12,                 /* T_R_PAREN  */
  YYSYMBOL_T_L_BRACE = 13,                 /* T_L_BRACE  */
  YYSYMBOL_T_R_BRACE = 14,                 /* T_R_BRACE  */
  YYSYMBOL_T_CONST = 15,                   /* T_CONST  */
  YYSYMBOL_T_COMMA = 16,                   /* T_COMMA  */
  YYSYMBOL_T_L_SQUARE = 17,                /* T_L_SQUARE  */
  YYSYMBOL_T_R_SQUARE = 18,                /* T_R_SQUARE  */
  YYSYMBOL_T_EQUAL = 19,                   /* T_EQUAL  */
  YYSYMBOL_T_VOID = 20,                    /* T_VOID  */
  YYSYMBOL_T_IF = 21,                      /* T_IF  */
  YYSYMBOL_T_ELSE = 22,                    /* T_ELSE  */
  YYSYMBOL_T_WHILE = 23,                   /* T_WHILE  */
  YYSYMBOL_T_DO = 24,                      /* T_DO  */
  YYSYMBOL_T_FOR = 25,                     /* T_FOR  */
  YYSYMBOL_T_BREAK = 26,                   /* T_BREAK  */
  YYSYMBOL_T_CONTINUE = 27,                /* T_CONTINUE  */
  YYSYMBOL_T_TILDE = 28,                   /* T_TILDE  */
  YYSYMBOL_T_PLUS = 29,                    /* T_PLUS  */
  YYSYMBOL_T_MINUS = 30,                   /* T_MINUS  */
  YYSYMBOL_T_EXCLAIM = 31,                 /* T_EXCLAIM  */
  YYSYMBOL_T_STAR = 32,                    /* T_STAR  */
  YYSYMBOL_T_SLASH = 33,                   /* T_SLASH  */
  YYSYMBOL_T_PERCENT = 34,                 /* T_PERCENT  */
  YYSYMBOL_T_LESS = 35,                    /* T_LESS  */
  YYSYMBOL_T_GREATER = 36,                 /* T_GREATER  */
  YYSYMBOL_T_LESSEQUAL = 37,               /* T_LESSEQUAL  */
  YYSYMBOL_T_GREATEREQUAL = 38,            /* T_GREATEREQUAL  */
  YYSYMBOL_T_EQUALEQUAL = 39,              /* T_EQUALEQUAL  */
  YYSYMBOL_T_EXCLAIMEQUAL = 40,            /* T_EXCLAIMEQUAL  */
  YYSYMBOL_T_AMPAMP = 41,                  /* T_AMPAMP  */
  YYSYMBOL_T_PIPEPIPE = 42,                /* T_PIPEPIPE  */
  YYSYMBOL_T_AMP = 43,                     /* T_AMP  */
  YYSYMBOL_T_CARET = 44,                   /* T_CARET  */
  YYSYMBOL_T_PIPE = 45,                    /* T_PIPE  */
  YYSYMBOL_T_LESSLESS = 46,                /* T_LESSLESS  */
  YYSYMBOL_T_GREATERGREATER = 47,          /* T_GREATERGREATER  */
  YYSYMBOL_T_ELLIPSIS = 48,                /* T_ELLIPSIS  */
  YYSYMBOL_T_STRING_LITERAL = 49,          /* T_STRING_LITERAL  */
  YYSYMBOL_T_PLUSEQUAL = 50,               /* T_PLUSEQUAL  */
  YYSYMBOL_T_MINUSEQUAL = 51,              /* T_MINUSEQUAL  */
  YYSYMBOL_T_STAREQUAL = 52,               /* T_STAREQUAL  */
  YYSYMBOL_T_SLASHEQUAL = 53,              /* T_SLASHEQUAL  */
  YYSYMBOL_T_PERCENTEQUAL = 54,            /* T_PERCENTEQUAL  */
  YYSYMBOL_T_AMPEQUAL = 55,                /* T_AMPEQUAL  */
  YYSYMBOL_T_CARETEQUAL = 56,              /* T_CARETEQUAL  */
  YYSYMBOL_T_PIPEEQUAL = 57,               /* T_PIPEEQUAL  */
  YYSYMBOL_T_LESSLESSEQUAL = 58,           /* T_LESSLESSEQUAL  */
  YYSYMBOL_T_GREATERGREATEREQUAL = 59,     /* T_GREATERGREATEREQUAL  */
  YYSYMBOL_T_PLUSPLUS = 60,                /* T_PLUSPLUS  */
  YYSYMBOL_T_MINUSMINUS = 61,              /* T_MINUSMINUS  */
  YYSYMBOL_YYACCEPT = 62,                  /* $accept  */
  YYSYMBOL_Begin = 63,                     /* Begin  */
  YYSYMBOL_CompUnit = 64,                  /* CompUnit  */
  YYSYMBOL_GlobalDecl = 65,                /* GlobalDecl  */
  YYSYMBOL_FuncDef = 66,                   /* FuncDef  */
  YYSYMBOL_FuncFParams = 67,               /* FuncFParams  */
  YYSYMBOL_FuncFParam = 68,                /* FuncFParam  */
  YYSYMBOL_FuncCall = 69,                  /* FuncCall  */
  YYSYMBOL_FuncRParams = 70,               /* FuncRParams  */
  YYSYMBOL_Block = 71,                     /* Block  */
  YYSYMBOL_BlockItems = 72,                /* BlockItems  */
  YYSYMBOL_BlockItem = 73,                 /* BlockItem  */
  YYSYMBOL_Decl = 74,                      /* Decl  */
  YYSYMBOL_ConstDecl = 75,                 /* ConstDecl  */
  YYSYMBOL_VarDecl = 76,                   /* VarDecl  */
  YYSYMBOL_BType = 77,                     /* BType  */
  YYSYMBOL_ConstDefs = 78,                 /* ConstDefs  */
  YYSYMBOL_ConstDef = 79,                  /* ConstDef  */
  YYSYMBOL_VarDefs = 80,                   /* VarDefs  */
  YYSYMBOL_VarDef = 81,                    /* VarDef  */
  YYSYMBOL_ConstInitVal = 82,              /* ConstInitVal  */
  YYSYMBOL_ConstExp = 83,                  /* ConstExp  */
  YYSYMBOL_InitVal = 84,                   /* InitVal  */
  YYSYMBOL_LVal = 85,                      /* LVal  */
  YYSYMBOL_Stmt = 86,                      /* Stmt  */
  YYSYMBOL_Exp = 87,                       /* Exp  */
  YYSYMBOL_PostfixExp = 88,                /* PostfixExp  */
  YYSYMBOL_UnaryExp = 89,                  /* UnaryExp  */
  YYSYMBOL_PrimaryExp = 90,                /* PrimaryExp  */
  YYSYMBOL_MulExp = 91,                    /* MulExp  */
  YYSYMBOL_AddExp = 92,                    /* AddExp  */
  YYSYMBOL_ShiftExp = 93,                  /* ShiftExp  */
  YYSYMBOL_RelExp = 94,                    /* RelExp  */
  YYSYMBOL_EqExp = 95,                     /* EqExp  */
  YYSYMBOL_BAndExp = 96,                   /* BAndExp  */
  YYSYMBOL_BXorExp = 97,                   /* BXorExp  */
  YYSYMBOL_BOrExp = 98,                    /* BOrExp  */
  YYSYMBOL_LAndExp = 99,                   /* LAndExp  */
  YYSYMBOL_LOrExp = 100,                   /* LOrExp  */
  YYSYMBOL_AssignExp = 101,                /* AssignExp  */
  YYSYMBOL_AssignOp = 102                  /* AssignOp  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  21
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   277

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  62
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  41
/* YYNRULES -- Number of rules.  */
#define YYNRULES  111
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  202

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   316


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   264,   264,   269,   274,   281,   284,   292,   298,   305,
     311,   320,   325,   331,   338,   347,   357,   363,   369,   372,
     378,   383,   389,   392,   396,   399,   403,   408,   413,   414,
     415,   416,   418,   423,   429,   436,   441,   447,   453,   459,
     463,   467,   471,   477,   482,   486,   487,   488,   492,   498,
     505,   511,   517,   527,   531,   537,   540,   547,   550,   555,
     561,   564,   567,   572,   577,   582,   587,   592,   598,   601,
     606,   612,   613,   619,   625,   632,   633,   639,   646,   647,
     653,   660,   661,   667,   673,   679,   686,   687,   693,   700,
     701,   708,   709,   716,   717,   724,   725,   732,   733,   740,
     741,   747,   754,   755,   756,   757,   758,   759,   760,   761,
     762,   763
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "T_NUMERIC_CONSTANT",
  "T_IDENTIFIER", "T_INT", "T_FLOAT", "T_LONG", "T_CHAR", "T_RETURN",
  "T_SEMI", "T_L_PAREN", "T_R_PAREN", "T_L_BRACE", "T_R_BRACE", "T_CONST",
  "T_COMMA", "T_L_SQUARE", "T_R_SQUARE", "T_EQUAL", "T_VOID", "T_IF",
  "T_ELSE", "T_WHILE", "T_DO", "T_FOR", "T_BREAK", "T_CONTINUE", "T_TILDE",
  "T_PLUS", "T_MINUS", "T_EXCLAIM", "T_STAR", "T_SLASH", "T_PERCENT",
  "T_LESS", "T_GREATER", "T_LESSEQUAL", "T_GREATEREQUAL", "T_EQUALEQUAL",
  "T_EXCLAIMEQUAL", "T_AMPAMP", "T_PIPEPIPE", "T_AMP", "T_CARET", "T_PIPE",
  "T_LESSLESS", "T_GREATERGREATER", "T_ELLIPSIS", "T_STRING_LITERAL",
  "T_PLUSEQUAL", "T_MINUSEQUAL", "T_STAREQUAL", "T_SLASHEQUAL",
  "T_PERCENTEQUAL", "T_AMPEQUAL", "T_CARETEQUAL", "T_PIPEEQUAL",
  "T_LESSLESSEQUAL", "T_GREATERGREATEREQUAL", "T_PLUSPLUS", "T_MINUSMINUS",
  "$accept", "Begin", "CompUnit", "GlobalDecl", "FuncDef", "FuncFParams",
  "FuncFParam", "FuncCall", "FuncRParams", "Block", "BlockItems",
  "BlockItem", "Decl", "ConstDecl", "VarDecl", "BType", "ConstDefs",
  "ConstDef", "VarDefs", "VarDef", "ConstInitVal", "ConstExp", "InitVal",
  "LVal", "Stmt", "Exp", "PostfixExp", "UnaryExp", "PrimaryExp", "MulExp",
  "AddExp", "ShiftExp", "RelExp", "EqExp", "BAndExp", "BXorExp", "BOrExp",
  "LAndExp", "LOrExp", "AssignExp", "AssignOp", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316
};
#endif

#define YYPACT_NINF (-179)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      13,     6,  -179,    10,  -179,   242,    41,    47,    13,  -179,
    -179,  -179,  -179,  -179,    46,    14,    53,  -179,  -179,    80,
      61,  -179,  -179,    94,  -179,  -179,    46,   140,    99,    57,
    -179,   189,    79,  -179,    74,    30,  -179,   116,    79,  -179,
      80,    74,    90,  -179,    83,    79,    79,    79,    79,    79,
     129,   129,  -179,  -179,   -46,  -179,  -179,  -179,   126,    -3,
     133,   216,   159,   110,   130,    93,   143,   139,   162,  -179,
      74,   242,  -179,  -179,  -179,   139,  -179,  -179,    74,    32,
     185,   107,   139,  -179,  -179,  -179,  -179,  -179,  -179,  -179,
    -179,  -179,  -179,    79,    79,    79,    79,    79,    79,    79,
      79,    79,    79,    79,    79,    79,    79,    79,    79,    79,
      79,    67,  -179,  -179,   194,   204,    74,   210,   190,   215,
    -179,   203,  -179,  -179,  -179,    64,  -179,  -179,  -179,  -179,
     113,  -179,    79,  -179,  -179,  -179,  -179,  -179,  -179,  -179,
    -179,  -179,  -179,    79,  -179,    79,  -179,  -179,  -179,   126,
     126,    -3,    -3,   133,   133,   133,   133,   216,   216,   159,
     110,   130,    93,   143,  -179,    65,    79,    79,   236,    79,
    -179,  -179,  -179,  -179,  -179,  -179,    79,   139,   139,  -179,
    -179,   114,   120,   249,    75,  -179,    28,    28,    79,    79,
     239,  -179,   138,    95,    28,   252,    79,  -179,  -179,   166,
      28,  -179
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,    31,     0,    30,     0,     0,     0,     2,     3,
       6,     5,    24,    25,     0,     0,     0,    29,    28,     0,
       0,     1,     4,    38,    35,    27,     0,     0,     0,     0,
      32,     0,     0,    36,     0,     0,    11,     0,     0,    26,
       0,     0,     0,    68,    42,     0,     0,     0,     0,     0,
       0,     0,    61,    37,    70,    60,    71,    57,    75,    78,
      81,    86,    89,    91,    93,    95,    97,    41,     0,     7,
       0,     0,    13,    34,    39,    40,    33,     9,     0,     0,
      70,     0,    99,    55,    65,    62,    63,    64,    42,    66,
      67,    58,    59,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    47,    19,     0,     0,     0,     0,     0,     0,
      45,     0,    20,    22,    23,     0,     8,    12,    10,    14,
       0,    16,     0,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,     0,    69,     0,    72,    73,    74,    76,
      77,    79,    80,    82,    83,    84,    85,    87,    88,    90,
      92,    94,    96,    98,    44,     0,     0,     0,     0,     0,
      53,    54,    18,    21,    46,    15,     0,   100,   101,    56,
      43,     0,     0,     0,     0,    17,     0,     0,     0,     0,
      48,    50,     0,     0,     0,     0,     0,    49,    51,     0,
       0,    52
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -179,  -179,  -179,   257,  -179,   235,   196,  -179,  -179,   -30,
    -179,   147,   -56,  -179,  -179,    -2,  -179,   229,  -179,   244,
    -179,  -179,  -179,   -45,  -178,   -32,  -179,    68,  -179,     7,
     104,   155,   115,   165,   167,   164,   168,   163,   -31,   -77,
    -179
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     7,     8,     9,    10,    35,    36,    52,   130,   120,
     121,   122,    11,    12,    13,    14,    29,    30,    15,    24,
      73,    74,    53,    54,   124,   125,    55,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    82,    83,
     143
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      80,    67,   131,    19,    69,    89,    90,    75,   190,   191,
      16,    77,   123,    81,    91,    92,   197,    17,     1,     2,
       3,     4,   201,    80,    25,    37,    96,    97,     5,    37,
      26,    43,    44,     6,    80,    43,    44,   111,   112,    45,
     126,    68,    70,    45,   129,    20,    71,    21,   128,   114,
      23,   115,   116,   117,   118,   119,    46,    47,    48,    49,
      46,    47,    48,    49,    27,   123,    80,    39,   179,    37,
      43,    44,    31,    40,   174,   180,    80,   164,    45,   165,
     145,   145,    43,    44,    28,   189,   168,    68,    50,    51,
      45,   145,    50,    51,    79,    46,    47,    48,    49,   185,
      80,   177,    78,   149,   150,   196,    71,    46,    47,    48,
      49,   145,   178,    32,    84,    85,    86,    87,    38,   144,
      72,    80,    80,   145,    80,   175,   186,    50,    51,   176,
     145,    80,   187,    88,   181,   182,   145,   184,   108,    50,
      51,    80,    80,    80,    80,    18,     2,     3,     4,    80,
     195,    80,    34,   106,   145,    80,   192,   193,    93,    94,
      95,   146,   147,   148,   199,    43,    44,    18,     2,     3,
       4,   111,   112,    45,   107,    68,   113,     5,   200,    98,
      99,   110,   145,   114,   109,   115,   116,   117,   118,   119,
      46,    47,    48,    49,    18,     2,     3,     4,   104,   105,
     170,    41,   151,   152,   132,   166,    43,    44,    18,     2,
       3,     4,   111,   112,    45,   167,    68,   172,     5,   157,
     158,   169,    50,    51,   114,   171,   115,   116,   117,   118,
     119,    46,    47,    48,    49,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,    91,    92,    18,     2,     3,
       4,   100,   101,   102,   103,   153,   154,   155,   156,   183,
     188,   194,   198,    50,    51,    22,    42,   127,   173,    76,
      33,   159,   161,   163,   160,     0,     0,   162
};

static const yytype_int16 yycheck[] =
{
      45,    32,    79,     5,    34,    50,    51,    38,   186,   187,
       4,    41,    68,    45,    60,    61,   194,     7,     5,     6,
       7,     8,   200,    68,    10,    27,    29,    30,    15,    31,
      16,     3,     4,    20,    79,     3,     4,     9,    10,    11,
      70,    13,    12,    11,    12,     4,    16,     0,    78,    21,
       4,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      28,    29,    30,    31,    11,   121,   111,    10,   145,    71,
       3,     4,    11,    16,    10,    10,   121,    10,    11,   111,
      16,    16,     3,     4,     4,    10,   116,    13,    60,    61,
      11,    16,    60,    61,    11,    28,    29,    30,    31,   176,
     145,   132,    12,    96,    97,    10,    16,    28,    29,    30,
      31,    16,   143,    19,    46,    47,    48,    49,    19,    12,
       4,   166,   167,    16,   169,    12,    12,    60,    61,    16,
      16,   176,    12,     4,   166,   167,    16,   169,    45,    60,
      61,   186,   187,   188,   189,     5,     6,     7,     8,   194,
      12,   196,    12,    43,    16,   200,   188,   189,    32,    33,
      34,    93,    94,    95,   196,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    44,    13,    14,    15,    12,    46,
      47,    42,    16,    21,    41,    23,    24,    25,    26,    27,
      28,    29,    30,    31,     5,     6,     7,     8,    39,    40,
      10,    12,    98,    99,    19,    11,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    11,    13,    14,    15,   104,
     105,    11,    60,    61,    21,    10,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,     5,     6,     7,
       8,    35,    36,    37,    38,   100,   101,   102,   103,    23,
      11,    22,    10,    60,    61,     8,    31,    71,   121,    40,
      26,   106,   108,   110,   107,    -1,    -1,   109
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     5,     6,     7,     8,    15,    20,    63,    64,    65,
      66,    74,    75,    76,    77,    80,     4,     7,     5,    77,
       4,     0,    65,     4,    81,    10,    16,    11,     4,    78,
      79,    11,    19,    81,    12,    67,    68,    77,    19,    10,
      16,    12,    67,     3,     4,    11,    28,    29,    30,    31,
      60,    61,    69,    84,    85,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,    13,    71,
      12,    16,     4,    82,    83,   100,    79,    71,    12,    11,
      85,    87,   100,   101,    89,    89,    89,    89,     4,    85,
      85,    60,    61,    32,    33,    34,    29,    30,    46,    47,
      35,    36,    37,    38,    39,    40,    43,    44,    45,    41,
      42,     9,    10,    14,    21,    23,    24,    25,    26,    27,
      71,    72,    73,    74,    86,    87,    71,    68,    71,    12,
      70,   101,    19,    50,    51,    52,    53,    54,    55,    56,
      57,    58,    59,   102,    12,    16,    89,    89,    89,    91,
      91,    92,    92,    93,    93,    93,    93,    94,    94,    95,
      96,    97,    98,    99,    10,    87,    11,    11,    71,    11,
      10,    10,    14,    73,    10,    12,    16,   100,   100,   101,
      10,    87,    87,    23,    87,   101,    12,    12,    11,    10,
      86,    86,    87,    87,    22,    12,    10,    86,    10,    87,
      12,    86
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    62,    63,    64,    64,    65,    65,    66,    66,    66,
      66,    67,    67,    68,    69,    69,    70,    70,    71,    71,
      72,    72,    73,    73,    74,    74,    75,    76,    77,    77,
      77,    77,    78,    78,    79,    80,    80,    81,    81,    82,
      83,    84,    85,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    86,    87,    87,    88,    88,    88,
      89,    89,    89,    89,    89,    89,    89,    89,    90,    90,
      90,    91,    91,    91,    91,    92,    92,    92,    93,    93,
      93,    94,    94,    94,    94,    94,    95,    95,    95,    96,
      96,    97,    97,    98,    98,    99,    99,   100,   100,   101,
     101,   101,   102,   102,   102,   102,   102,   102,   102,   102,
     102,   102
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     2,     1,     1,     5,     6,     5,
       6,     1,     3,     2,     3,     4,     1,     3,     3,     2,
       1,     2,     1,     1,     1,     1,     4,     2,     1,     2,
       1,     1,     1,     3,     3,     2,     3,     3,     1,     1,
       1,     1,     1,     3,     2,     1,     2,     1,     5,     7,
       5,     7,     9,     2,     2,     1,     3,     1,     2,     2,
       1,     1,     2,     2,     2,     2,     2,     2,     1,     3,
       1,     1,     3,     3,     3,     1,     3,     3,     1,     3,
       3,     1,     3,     3,     3,     3,     1,     3,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
# ifndef YY_LOCATION_PRINT
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* Begin: CompUnit  */
#line 264 "/workspace/SYsU-lang/parser/parser.y"
                {
    root = yyvsp[0];
  }
#line 1536 "main.cc"
    break;

  case 3: /* CompUnit: GlobalDecl  */
#line 269 "/workspace/SYsU-lang/parser/parser.y"
                     {
    auto ptr = new Tree("TranslationUnitDecl");
    ptr->mergeSon(yyvsp[0]);
    yyval = ptr;
  }
#line 1546 "main.cc"
    break;

  case 4: /* CompUnit: CompUnit GlobalDecl  */
#line 274 "/workspace/SYsU-lang/parser/parser.y"
                      {
    auto ptr = yyvsp[-1];
    ptr -> mergeSon(yyvsp[0]);
    yyval = ptr;
}
#line 1556 "main.cc"
    break;

  case 5: /* GlobalDecl: Decl  */
#line 281 "/workspace/SYsU-lang/parser/parser.y"
                 {
    yyval = yyvsp[0];
}
#line 1564 "main.cc"
    break;

  case 6: /* GlobalDecl: FuncDef  */
#line 284 "/workspace/SYsU-lang/parser/parser.y"
          {
    auto ptr = new Tree("Temp");
    ptr->addSon(yyvsp[0]);
    yyval = ptr;
}
#line 1574 "main.cc"
    break;

  case 7: /* FuncDef: T_INT T_IDENTIFIER T_L_PAREN T_R_PAREN Block  */
#line 292 "/workspace/SYsU-lang/parser/parser.y"
                                                      {
    auto ptr = new Tree("FunctionDecl", yyvsp[-3]->name);
    free(yyvsp[-3]);
    ptr->addSon(yyvsp[0]);
    yyval = ptr;
}
#line 1585 "main.cc"
    break;

  case 8: /* FuncDef: T_INT T_IDENTIFIER T_L_PAREN FuncFParams T_R_PAREN Block  */
#line 298 "/workspace/SYsU-lang/parser/parser.y"
                                                            {
    auto ptr = yyvsp[-2];
    ptr->name = yyvsp[-4]->name;
    free(yyvsp[-4]);
    ptr->addSon(yyvsp[0]);
    yyval = ptr;
}
#line 1597 "main.cc"
    break;

  case 9: /* FuncDef: T_VOID T_IDENTIFIER T_L_PAREN T_R_PAREN Block  */
#line 305 "/workspace/SYsU-lang/parser/parser.y"
                                                {
    auto ptr = new Tree("FunctionDecl", yyvsp[-3]->name);
    free(yyvsp[-3]);
    ptr->addSon(yyvsp[0]);
    yyval = ptr;
}
#line 1608 "main.cc"
    break;

  case 10: /* FuncDef: T_VOID T_IDENTIFIER T_L_PAREN FuncFParams T_R_PAREN Block  */
#line 311 "/workspace/SYsU-lang/parser/parser.y"
                                                             {
    auto ptr = yyvsp[-2];
    ptr->name = yyvsp[-4]->name;
    free(yyvsp[-4]);
    ptr->addSon(yyvsp[0]);
    yyval = ptr;
}
#line 1620 "main.cc"
    break;

  case 11: /* FuncFParams: FuncFParam  */
#line 320 "/workspace/SYsU-lang/parser/parser.y"
                         {
    auto ptr = new Tree("FunctionDecl");
    ptr -> addSon(yyvsp[0]);
    yyval = ptr;
}
#line 1630 "main.cc"
    break;

  case 12: /* FuncFParams: FuncFParams T_COMMA FuncFParam  */
#line 325 "/workspace/SYsU-lang/parser/parser.y"
                                 {
    auto ptr = yyvsp[-2];
    ptr -> addSon(yyvsp[0]);
    yyval = ptr;
}
#line 1640 "main.cc"
    break;

  case 13: /* FuncFParam: BType T_IDENTIFIER  */
#line 331 "/workspace/SYsU-lang/parser/parser.y"
                                {
    auto ptr = new Tree("ParmVarDecl", yyvsp[0]->name);
    free(yyvsp[0]);
    yyval = ptr;
}
#line 1650 "main.cc"
    break;

  case 14: /* FuncCall: T_IDENTIFIER T_L_PAREN T_R_PAREN  */
#line 338 "/workspace/SYsU-lang/parser/parser.y"
                                          {
    auto ptr = new Tree("CallExpr");
    auto tp1 = new Tree("DeclRefExpr", yyvsp[-2]->name);
    free(yyvsp[-2]);
    auto tp2 = new Tree("ImplicitCastExpr");
    tp2 -> addSon(tp1);
    ptr -> addSon(tp2);
    yyval = ptr;
}
#line 1664 "main.cc"
    break;

  case 15: /* FuncCall: T_IDENTIFIER T_L_PAREN FuncRParams T_R_PAREN  */
#line 347 "/workspace/SYsU-lang/parser/parser.y"
                                               {
    auto ptr = yyvsp[-1];
    auto tp1 = new Tree("DeclRefExpr", yyvsp[-3]->name);
    free(yyvsp[-3]);
    auto tp2 = new Tree("ImplicitCastExpr");
    tp2 -> addSon(tp1);
    ptr -> sons[0] = tp2;
    yyval = ptr;
}
#line 1678 "main.cc"
    break;

  case 16: /* FuncRParams: AssignExp  */
#line 357 "/workspace/SYsU-lang/parser/parser.y"
                        {
    auto ptr = new Tree("CallExpr");
    ptr -> addSon(nullptr); 
    ptr -> addSon(yyvsp[0]);
    yyval = ptr;
}
#line 1689 "main.cc"
    break;

  case 17: /* FuncRParams: FuncRParams T_COMMA AssignExp  */
#line 363 "/workspace/SYsU-lang/parser/parser.y"
                                {
    auto ptr = yyvsp[-2];
    ptr -> addSon(yyvsp[0]);
    yyval = ptr;
}
#line 1699 "main.cc"
    break;

  case 18: /* Block: T_L_BRACE BlockItems T_R_BRACE  */
#line 369 "/workspace/SYsU-lang/parser/parser.y"
                                      {
    yyval = yyvsp[-1];
  }
#line 1707 "main.cc"
    break;

  case 19: /* Block: T_L_BRACE T_R_BRACE  */
#line 372 "/workspace/SYsU-lang/parser/parser.y"
                      {
    auto ptr = new Tree("CompoundStmt");
    yyval = ptr;
}
#line 1716 "main.cc"
    break;

  case 20: /* BlockItems: BlockItem  */
#line 378 "/workspace/SYsU-lang/parser/parser.y"
                      {
    auto ptr = new Tree("CompoundStmt");
    ptr -> addSon(yyvsp[0]);
    yyval = ptr;
}
#line 1726 "main.cc"
    break;

  case 21: /* BlockItems: BlockItems BlockItem  */
#line 383 "/workspace/SYsU-lang/parser/parser.y"
                       {
    auto ptr = yyvsp[-1];
    ptr -> addSon(yyvsp[0]);
    yyval = ptr;
}
#line 1736 "main.cc"
    break;

  case 22: /* BlockItem: Decl  */
#line 389 "/workspace/SYsU-lang/parser/parser.y"
                {
    yyval = yyvsp[0];
}
#line 1744 "main.cc"
    break;

  case 23: /* BlockItem: Stmt  */
#line 392 "/workspace/SYsU-lang/parser/parser.y"
       {
    yyval = yyvsp[0]; 
}
#line 1752 "main.cc"
    break;

  case 24: /* Decl: ConstDecl  */
#line 396 "/workspace/SYsU-lang/parser/parser.y"
                {
    yyval = yyvsp[0];
}
#line 1760 "main.cc"
    break;

  case 25: /* Decl: VarDecl  */
#line 399 "/workspace/SYsU-lang/parser/parser.y"
          {
    yyval = yyvsp[0];
}
#line 1768 "main.cc"
    break;

  case 26: /* ConstDecl: T_CONST BType ConstDefs T_SEMI  */
#line 403 "/workspace/SYsU-lang/parser/parser.y"
                                           {
    yyval = yyvsp[-1];
}
#line 1776 "main.cc"
    break;

  case 27: /* VarDecl: VarDefs T_SEMI  */
#line 408 "/workspace/SYsU-lang/parser/parser.y"
                         {

    //$$ = $2;
}
#line 1785 "main.cc"
    break;

  case 28: /* BType: T_INT  */
#line 413 "/workspace/SYsU-lang/parser/parser.y"
             {}
#line 1791 "main.cc"
    break;

  case 29: /* BType: T_LONG T_LONG  */
#line 414 "/workspace/SYsU-lang/parser/parser.y"
                {}
#line 1797 "main.cc"
    break;

  case 30: /* BType: T_CHAR  */
#line 415 "/workspace/SYsU-lang/parser/parser.y"
         {}
#line 1803 "main.cc"
    break;

  case 31: /* BType: T_FLOAT  */
#line 416 "/workspace/SYsU-lang/parser/parser.y"
          {}
#line 1809 "main.cc"
    break;

  case 32: /* ConstDefs: ConstDef  */
#line 418 "/workspace/SYsU-lang/parser/parser.y"
                     {
    auto ptr = new Tree("DeclStmt");
    ptr -> addSon(yyvsp[0]);
    yyval = ptr;
}
#line 1819 "main.cc"
    break;

  case 33: /* ConstDefs: ConstDefs T_COMMA ConstDef  */
#line 423 "/workspace/SYsU-lang/parser/parser.y"
                             {
    auto ptr = yyvsp[-2];
    ptr -> addSon(yyvsp[0]);
    yyval = ptr;
}
#line 1829 "main.cc"
    break;

  case 34: /* ConstDef: T_IDENTIFIER T_EQUAL ConstInitVal  */
#line 429 "/workspace/SYsU-lang/parser/parser.y"
                                             {
    auto ptr = new Tree("VarDecl", yyvsp[-2]->name);
    free(yyvsp[-2]);
    ptr -> addSon(yyvsp[0]);
    yyval = ptr;
}
#line 1840 "main.cc"
    break;

  case 35: /* VarDefs: BType VarDef  */
#line 436 "/workspace/SYsU-lang/parser/parser.y"
                      {
    /*auto ptr = new Tree("DeclStmt");
    ptr -> addSon($1);
    $$ = ptr;*/
}
#line 1850 "main.cc"
    break;

  case 36: /* VarDefs: VarDefs T_COMMA VarDef  */
#line 441 "/workspace/SYsU-lang/parser/parser.y"
                         {
    auto ptr = yyvsp[-2];
    ptr -> addSon(yyvsp[0]);
    yyval = ptr;
}
#line 1860 "main.cc"
    break;

  case 37: /* VarDef: T_IDENTIFIER T_EQUAL InitVal  */
#line 447 "/workspace/SYsU-lang/parser/parser.y"
                                      {
    auto ptr = new Tree("VarDecl", yyvsp[-2]->name);
    free(yyvsp[-2]);
    ptr -> addSon(yyvsp[0]);
    yyval = ptr;
}
#line 1871 "main.cc"
    break;

  case 38: /* VarDef: T_IDENTIFIER  */
#line 453 "/workspace/SYsU-lang/parser/parser.y"
               {
    auto ptr = new Tree("VarDecl", yyvsp[0]->name);
    free(yyvsp[0]);
    yyval = ptr;
}
#line 1881 "main.cc"
    break;

  case 39: /* ConstInitVal: ConstExp  */
#line 459 "/workspace/SYsU-lang/parser/parser.y"
                        {
    yyval = yyvsp[0];
}
#line 1889 "main.cc"
    break;

  case 40: /* ConstExp: LOrExp  */
#line 463 "/workspace/SYsU-lang/parser/parser.y"
                  {
    yyval = yyvsp[0];
}
#line 1897 "main.cc"
    break;

  case 41: /* InitVal: LOrExp  */
#line 467 "/workspace/SYsU-lang/parser/parser.y"
                 {
    yyval = yyvsp[0];
}
#line 1905 "main.cc"
    break;

  case 42: /* LVal: T_IDENTIFIER  */
#line 471 "/workspace/SYsU-lang/parser/parser.y"
                   {
    auto ptr = new Tree("DeclRefExpr", yyvsp[0]->name);
    free(yyvsp[0]);
    yyval = ptr;
}
#line 1915 "main.cc"
    break;

  case 43: /* Stmt: T_RETURN Exp T_SEMI  */
#line 477 "/workspace/SYsU-lang/parser/parser.y"
                          {
    auto ptr = new Tree("ReturnStmt");
    ptr->addSon(yyvsp[-1]);
    yyval = ptr;
}
#line 1925 "main.cc"
    break;

  case 44: /* Stmt: T_RETURN T_SEMI  */
#line 482 "/workspace/SYsU-lang/parser/parser.y"
                  {
    auto ptr = new Tree("ReturnStmt");
    yyval = ptr;
}
#line 1934 "main.cc"
    break;

  case 45: /* Stmt: Block  */
#line 486 "/workspace/SYsU-lang/parser/parser.y"
        {yyval = yyvsp[0];}
#line 1940 "main.cc"
    break;

  case 46: /* Stmt: Exp T_SEMI  */
#line 487 "/workspace/SYsU-lang/parser/parser.y"
             {yyval = yyvsp[-1];}
#line 1946 "main.cc"
    break;

  case 47: /* Stmt: T_SEMI  */
#line 488 "/workspace/SYsU-lang/parser/parser.y"
         {
    auto ptr = new Tree("NullStmt");
    yyval = ptr;
}
#line 1955 "main.cc"
    break;

  case 48: /* Stmt: T_IF T_L_PAREN Exp T_R_PAREN Stmt  */
#line 492 "/workspace/SYsU-lang/parser/parser.y"
                                    {
    auto ptr = new Tree("IfStmt");
    ptr->addSon(yyvsp[-2]);
    ptr->addSon(yyvsp[0]);
    yyval = ptr;
}
#line 1966 "main.cc"
    break;

  case 49: /* Stmt: T_IF T_L_PAREN Exp T_R_PAREN Stmt T_ELSE Stmt  */
#line 498 "/workspace/SYsU-lang/parser/parser.y"
                                                {
    auto ptr = new Tree("IfStmt");
    ptr->addSon(yyvsp[-4]);
    ptr->addSon(yyvsp[-2]);
    ptr->addSon(yyvsp[0]);
    yyval = ptr;
}
#line 1978 "main.cc"
    break;

  case 50: /* Stmt: T_WHILE T_L_PAREN Exp T_R_PAREN Stmt  */
#line 505 "/workspace/SYsU-lang/parser/parser.y"
                                       {
    auto ptr = new Tree("WhileStmt");
    ptr->addSon(yyvsp[-2]);
    ptr->addSon(yyvsp[0]);
    yyval = ptr;
}
#line 1989 "main.cc"
    break;

  case 51: /* Stmt: T_DO Block T_WHILE T_L_PAREN Exp T_R_PAREN T_SEMI  */
#line 511 "/workspace/SYsU-lang/parser/parser.y"
                                                    {
    auto ptr = new Tree("DoStmt");
    ptr->addSon(yyvsp[-5]);
    ptr->addSon(yyvsp[-2]);
    yyval = ptr;
}
#line 2000 "main.cc"
    break;

  case 52: /* Stmt: T_FOR T_L_PAREN Exp T_SEMI Exp T_SEMI Exp T_R_PAREN Stmt  */
#line 517 "/workspace/SYsU-lang/parser/parser.y"
                                                           {
    auto ptr = new Tree("ForStmt");
    ptr->addSon(yyvsp[-6]);
    auto nptr = new Tree("");
    ptr ->addSon(nptr);
    ptr->addSon(yyvsp[-4]);
    ptr->addSon(yyvsp[-2]);
    ptr->addSon(yyvsp[0]);
    yyval = ptr;
}
#line 2015 "main.cc"
    break;

  case 53: /* Stmt: T_BREAK T_SEMI  */
#line 527 "/workspace/SYsU-lang/parser/parser.y"
                 {
    auto ptr = new Tree("BreakStmt");
    yyval = ptr;
}
#line 2024 "main.cc"
    break;

  case 54: /* Stmt: T_CONTINUE T_SEMI  */
#line 531 "/workspace/SYsU-lang/parser/parser.y"
                    {
    auto ptr = new Tree("ContinueStmt");
    yyval = ptr;
}
#line 2033 "main.cc"
    break;

  case 55: /* Exp: AssignExp  */
#line 537 "/workspace/SYsU-lang/parser/parser.y"
               {
    yyval = yyvsp[0];
}
#line 2041 "main.cc"
    break;

  case 56: /* Exp: Exp T_COMMA AssignExp  */
#line 540 "/workspace/SYsU-lang/parser/parser.y"
                        {
    auto ptr = new Tree("BinaryOperator");
    ptr->addSon(yyvsp[-2]);
    ptr->addSon(yyvsp[0]);
    yyval = ptr;
}
#line 2052 "main.cc"
    break;

  case 57: /* PostfixExp: PrimaryExp  */
#line 547 "/workspace/SYsU-lang/parser/parser.y"
                        {
    yyval = yyvsp[0];
}
#line 2060 "main.cc"
    break;

  case 58: /* PostfixExp: LVal T_PLUSPLUS  */
#line 550 "/workspace/SYsU-lang/parser/parser.y"
                  {
    auto ptr = new Tree("UnaryOperator");
    ptr->addSon(yyvsp[-1]);
    yyval = ptr;
}
#line 2070 "main.cc"
    break;

  case 59: /* PostfixExp: LVal T_MINUSMINUS  */
#line 555 "/workspace/SYsU-lang/parser/parser.y"
                    {
    auto ptr = new Tree("UnaryOperator");
    ptr->addSon(yyvsp[-1]);
    yyval = ptr;
}
#line 2080 "main.cc"
    break;

  case 60: /* UnaryExp: PostfixExp  */
#line 561 "/workspace/SYsU-lang/parser/parser.y"
                      {
    yyval = yyvsp[0];
}
#line 2088 "main.cc"
    break;

  case 61: /* UnaryExp: FuncCall  */
#line 564 "/workspace/SYsU-lang/parser/parser.y"
           {
    yyval = yyvsp[0];
}
#line 2096 "main.cc"
    break;

  case 62: /* UnaryExp: T_PLUS UnaryExp  */
#line 567 "/workspace/SYsU-lang/parser/parser.y"
                 {
    auto ptr = new Tree("UnaryOperator");
    ptr->addSon(yyvsp[0]);
    yyval = ptr;
}
#line 2106 "main.cc"
    break;

  case 63: /* UnaryExp: T_MINUS UnaryExp  */
#line 572 "/workspace/SYsU-lang/parser/parser.y"
                   {
    auto ptr = new Tree("UnaryOperator");
    ptr->addSon(yyvsp[0]);
    yyval = ptr;
}
#line 2116 "main.cc"
    break;

  case 64: /* UnaryExp: T_EXCLAIM UnaryExp  */
#line 577 "/workspace/SYsU-lang/parser/parser.y"
                     {
    auto ptr = new Tree("UnaryOperator");
    ptr->addSon(yyvsp[0]);
    yyval = ptr;
}
#line 2126 "main.cc"
    break;

  case 65: /* UnaryExp: T_TILDE UnaryExp  */
#line 582 "/workspace/SYsU-lang/parser/parser.y"
                   {
    auto ptr = new Tree("UnaryOperator");
    ptr->addSon(yyvsp[0]);
    yyval = ptr;
}
#line 2136 "main.cc"
    break;

  case 66: /* UnaryExp: T_PLUSPLUS LVal  */
#line 587 "/workspace/SYsU-lang/parser/parser.y"
                 {
    auto ptr = new Tree("UnaryOperator");
    ptr->addSon(yyvsp[0]);
    yyval = ptr;
}
#line 2146 "main.cc"
    break;

  case 67: /* UnaryExp: T_MINUSMINUS LVal  */
#line 592 "/workspace/SYsU-lang/parser/parser.y"
                    {
    auto ptr = new Tree("UnaryOperator");
    ptr->addSon(yyvsp[0]);
    yyval = ptr;
}
#line 2156 "main.cc"
    break;

  case 68: /* PrimaryExp: T_NUMERIC_CONSTANT  */
#line 598 "/workspace/SYsU-lang/parser/parser.y"
                               {
    yyval = yyvsp[0];
}
#line 2164 "main.cc"
    break;

  case 69: /* PrimaryExp: T_L_PAREN Exp T_R_PAREN  */
#line 601 "/workspace/SYsU-lang/parser/parser.y"
                          {
    auto ptr = new Tree("ParenExpr");
    ptr->addSon(yyvsp[-1]);
    yyval = ptr;
}
#line 2174 "main.cc"
    break;

  case 70: /* PrimaryExp: LVal  */
#line 606 "/workspace/SYsU-lang/parser/parser.y"
       {
    auto ptr = new Tree("ImplicitCastExpr");
    ptr->addSon(yyvsp[0]);
    yyval = ptr;
}
#line 2184 "main.cc"
    break;

  case 71: /* MulExp: UnaryExp  */
#line 612 "/workspace/SYsU-lang/parser/parser.y"
                  { yyval = yyvsp[0]; }
#line 2190 "main.cc"
    break;

  case 72: /* MulExp: MulExp T_STAR UnaryExp  */
#line 613 "/workspace/SYsU-lang/parser/parser.y"
                         {
    auto ptr = new Tree("BinaryOperator");
    ptr->addSon(yyvsp[-2]);
    ptr->addSon(yyvsp[0]);
    yyval = ptr;
}
#line 2201 "main.cc"
    break;

  case 73: /* MulExp: MulExp T_SLASH UnaryExp  */
#line 619 "/workspace/SYsU-lang/parser/parser.y"
                          {
    auto ptr = new Tree("BinaryOperator");
    ptr->addSon(yyvsp[-2]);
    ptr->addSon(yyvsp[0]);
    yyval = ptr;
}
#line 2212 "main.cc"
    break;

  case 74: /* MulExp: MulExp T_PERCENT UnaryExp  */
#line 625 "/workspace/SYsU-lang/parser/parser.y"
                            {
    auto ptr = new Tree("BinaryOperator");
    ptr->addSon(yyvsp[-2]);
    ptr->addSon(yyvsp[0]);
    yyval = ptr;
}
#line 2223 "main.cc"
    break;

  case 75: /* AddExp: MulExp  */
#line 632 "/workspace/SYsU-lang/parser/parser.y"
                { yyval = yyvsp[0]; }
#line 2229 "main.cc"
    break;

  case 76: /* AddExp: AddExp T_PLUS MulExp  */
#line 633 "/workspace/SYsU-lang/parser/parser.y"
                       {
    auto ptr = new Tree("BinaryOperator");
    ptr->addSon(yyvsp[-2]);
    ptr->addSon(yyvsp[0]);
    yyval = ptr;
}
#line 2240 "main.cc"
    break;

  case 77: /* AddExp: AddExp T_MINUS MulExp  */
#line 639 "/workspace/SYsU-lang/parser/parser.y"
                        {
    auto ptr = new Tree("BinaryOperator");
    ptr->addSon(yyvsp[-2]);
    ptr->addSon(yyvsp[0]);
    yyval = ptr;
}
#line 2251 "main.cc"
    break;

  case 78: /* ShiftExp: AddExp  */
#line 646 "/workspace/SYsU-lang/parser/parser.y"
                  { yyval = yyvsp[0]; }
#line 2257 "main.cc"
    break;

  case 79: /* ShiftExp: ShiftExp T_LESSLESS AddExp  */
#line 647 "/workspace/SYsU-lang/parser/parser.y"
                             {
    auto ptr = new Tree("BinaryOperator");
    ptr->addSon(yyvsp[-2]);
    ptr->addSon(yyvsp[0]);
    yyval = ptr;
}
#line 2268 "main.cc"
    break;

  case 80: /* ShiftExp: ShiftExp T_GREATERGREATER AddExp  */
#line 653 "/workspace/SYsU-lang/parser/parser.y"
                                   {
    auto ptr = new Tree("BinaryOperator");
    ptr->addSon(yyvsp[-2]);
    ptr->addSon(yyvsp[0]);
    yyval = ptr;
}
#line 2279 "main.cc"
    break;

  case 81: /* RelExp: ShiftExp  */
#line 660 "/workspace/SYsU-lang/parser/parser.y"
                  { yyval = yyvsp[0]; }
#line 2285 "main.cc"
    break;

  case 82: /* RelExp: RelExp T_LESS ShiftExp  */
#line 661 "/workspace/SYsU-lang/parser/parser.y"
                         {
    auto ptr = new Tree("BinaryOperator");
    ptr->addSon(yyvsp[-2]);
    ptr->addSon(yyvsp[0]);
    yyval = ptr;
}
#line 2296 "main.cc"
    break;

  case 83: /* RelExp: RelExp T_GREATER ShiftExp  */
#line 667 "/workspace/SYsU-lang/parser/parser.y"
                            {
    auto ptr = new Tree("BinaryOperator");
    ptr->addSon(yyvsp[-2]);
    ptr->addSon(yyvsp[0]);
    yyval = ptr;
}
#line 2307 "main.cc"
    break;

  case 84: /* RelExp: RelExp T_LESSEQUAL ShiftExp  */
#line 673 "/workspace/SYsU-lang/parser/parser.y"
                              {
    auto ptr = new Tree("BinaryOperator");
    ptr->addSon(yyvsp[-2]);
    ptr->addSon(yyvsp[0]);
    yyval = ptr;
}
#line 2318 "main.cc"
    break;

  case 85: /* RelExp: RelExp T_GREATEREQUAL ShiftExp  */
#line 679 "/workspace/SYsU-lang/parser/parser.y"
                                 {
    auto ptr = new Tree("BinaryOperator");
    ptr->addSon(yyvsp[-2]);
    ptr->addSon(yyvsp[0]);
    yyval = ptr;
}
#line 2329 "main.cc"
    break;

  case 86: /* EqExp: RelExp  */
#line 686 "/workspace/SYsU-lang/parser/parser.y"
               { yyval = yyvsp[0]; }
#line 2335 "main.cc"
    break;

  case 87: /* EqExp: EqExp T_EQUALEQUAL RelExp  */
#line 687 "/workspace/SYsU-lang/parser/parser.y"
                            {
    auto ptr = new Tree("BinaryOperator");
    ptr->addSon(yyvsp[-2]);
    ptr->addSon(yyvsp[0]);
    yyval = ptr;
}
#line 2346 "main.cc"
    break;

  case 88: /* EqExp: EqExp T_EXCLAIMEQUAL RelExp  */
#line 693 "/workspace/SYsU-lang/parser/parser.y"
                              {
    auto ptr = new Tree("BinaryOperator");
    ptr->addSon(yyvsp[-2]);
    ptr->addSon(yyvsp[0]);
    yyval = ptr;
}
#line 2357 "main.cc"
    break;

  case 89: /* BAndExp: EqExp  */
#line 700 "/workspace/SYsU-lang/parser/parser.y"
                { yyval = yyvsp[0]; }
#line 2363 "main.cc"
    break;

  case 90: /* BAndExp: BAndExp T_AMP EqExp  */
#line 701 "/workspace/SYsU-lang/parser/parser.y"
                      {
    auto ptr = new Tree("BinaryOperator");
    ptr->addSon(yyvsp[-2]);
    ptr->addSon(yyvsp[0]);
    yyval = ptr;
}
#line 2374 "main.cc"
    break;

  case 91: /* BXorExp: BAndExp  */
#line 708 "/workspace/SYsU-lang/parser/parser.y"
                  { yyval = yyvsp[0]; }
#line 2380 "main.cc"
    break;

  case 92: /* BXorExp: BXorExp T_CARET BAndExp  */
#line 709 "/workspace/SYsU-lang/parser/parser.y"
                          {
    auto ptr = new Tree("BinaryOperator");
    ptr->addSon(yyvsp[-2]);
    ptr->addSon(yyvsp[0]);
    yyval = ptr;
}
#line 2391 "main.cc"
    break;

  case 93: /* BOrExp: BXorExp  */
#line 716 "/workspace/SYsU-lang/parser/parser.y"
                 { yyval = yyvsp[0]; }
#line 2397 "main.cc"
    break;

  case 94: /* BOrExp: BOrExp T_PIPE BXorExp  */
#line 717 "/workspace/SYsU-lang/parser/parser.y"
                        {
    auto ptr = new Tree("BinaryOperator");
    ptr->addSon(yyvsp[-2]);
    ptr->addSon(yyvsp[0]);
    yyval = ptr;
}
#line 2408 "main.cc"
    break;

  case 95: /* LAndExp: BOrExp  */
#line 724 "/workspace/SYsU-lang/parser/parser.y"
                 { yyval = yyvsp[0]; }
#line 2414 "main.cc"
    break;

  case 96: /* LAndExp: LAndExp T_AMPAMP BOrExp  */
#line 725 "/workspace/SYsU-lang/parser/parser.y"
                          {
    auto ptr = new Tree("BinaryOperator");
    ptr->addSon(yyvsp[-2]);
    ptr->addSon(yyvsp[0]);
    yyval = ptr;
}
#line 2425 "main.cc"
    break;

  case 97: /* LOrExp: LAndExp  */
#line 732 "/workspace/SYsU-lang/parser/parser.y"
                 { yyval = yyvsp[0]; }
#line 2431 "main.cc"
    break;

  case 98: /* LOrExp: LOrExp T_PIPEPIPE LAndExp  */
#line 733 "/workspace/SYsU-lang/parser/parser.y"
                            {
    auto ptr = new Tree("BinaryOperator");
    ptr->addSon(yyvsp[-2]);
    ptr->addSon(yyvsp[0]);
    yyval = ptr;
}
#line 2442 "main.cc"
    break;

  case 99: /* AssignExp: LOrExp  */
#line 740 "/workspace/SYsU-lang/parser/parser.y"
                   {yyval = yyvsp[0] ;}
#line 2448 "main.cc"
    break;

  case 100: /* AssignExp: LVal T_EQUAL LOrExp  */
#line 741 "/workspace/SYsU-lang/parser/parser.y"
                      {
    auto ptr = new Tree("BinaryOperator");
    ptr->addSon(yyvsp[-2]);
    ptr->addSon(yyvsp[0]);
    yyval = ptr;
}
#line 2459 "main.cc"
    break;

  case 101: /* AssignExp: LVal AssignOp LOrExp  */
#line 747 "/workspace/SYsU-lang/parser/parser.y"
                       {
    auto ptr = new Tree("CompoundAssignOperator");
    ptr->addSon(yyvsp[-2]);
    ptr->addSon(yyvsp[0]);
    yyval = ptr;
}
#line 2470 "main.cc"
    break;

  case 102: /* AssignOp: T_PLUSEQUAL  */
#line 754 "/workspace/SYsU-lang/parser/parser.y"
                       {}
#line 2476 "main.cc"
    break;

  case 103: /* AssignOp: T_MINUSEQUAL  */
#line 755 "/workspace/SYsU-lang/parser/parser.y"
               {}
#line 2482 "main.cc"
    break;

  case 104: /* AssignOp: T_STAREQUAL  */
#line 756 "/workspace/SYsU-lang/parser/parser.y"
              {}
#line 2488 "main.cc"
    break;

  case 105: /* AssignOp: T_SLASHEQUAL  */
#line 757 "/workspace/SYsU-lang/parser/parser.y"
               {}
#line 2494 "main.cc"
    break;

  case 106: /* AssignOp: T_PERCENTEQUAL  */
#line 758 "/workspace/SYsU-lang/parser/parser.y"
                 {}
#line 2500 "main.cc"
    break;

  case 107: /* AssignOp: T_AMPEQUAL  */
#line 759 "/workspace/SYsU-lang/parser/parser.y"
             {}
#line 2506 "main.cc"
    break;

  case 108: /* AssignOp: T_CARETEQUAL  */
#line 760 "/workspace/SYsU-lang/parser/parser.y"
               {}
#line 2512 "main.cc"
    break;

  case 109: /* AssignOp: T_PIPEEQUAL  */
#line 761 "/workspace/SYsU-lang/parser/parser.y"
              {}
#line 2518 "main.cc"
    break;

  case 110: /* AssignOp: T_LESSLESSEQUAL  */
#line 762 "/workspace/SYsU-lang/parser/parser.y"
                  {}
#line 2524 "main.cc"
    break;

  case 111: /* AssignOp: T_GREATERGREATEREQUAL  */
#line 763 "/workspace/SYsU-lang/parser/parser.y"
                        {}
#line 2530 "main.cc"
    break;


#line 2534 "main.cc"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 765 "/workspace/SYsU-lang/parser/parser.y"
