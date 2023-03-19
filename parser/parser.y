%code requires
{
#include<iostream>
class Tree;
class TranslationUnitTree;
class DeclTree;
class VarDeclTree;
class FuncDeclTree;
class StmtTree;
class DeclStmtTree;
class NullStmtTree;
class BlockStmtTree;
class IfStmtTree;
class LoopStmtTree;
class WhileStmtTree;
class DoStmtTree;
class ForStmtTree;
class JumpStmtTree;
class ContinueStmtTree;
class BreakStmtTree;
class ReturnStmtTree;
class ExprStmtTree;
class ExprTree;
class LiteralTree;
class ParenExprTree;
class InitListExprTree;
class ImplicitCastExprTree;
class ArraySubscriptExprTree;
class DeclRefTree;
class UnaryExprTree;
class BinaryExprTree;
class TernaryExprTree;
class FuncCallTree;
class Info;
class Layer;
enum OpType
{
    OP_PLUSPLUS,OP_MINUSMINUS,OP_PLUS,OP_MINUS,OP_EXCLAIM,OP_TILDE,
    OP_STAR,OP_SLASH,OP_PERCENT,OP_LESSLESS,OP_GREATERGREATER,
    OP_LESS,OP_GREATER,OP_LESSEQUAL,OP_GREATEREQUAL,OP_EQUALEQUAL,
    OP_EXCLAIMEQUAL,OP_AMP,OP_CARET,OP_PIPE,OP_AMPAMP,OP_PIPEPIPE,OP_COMMA,
    OP_EQUAL,OP_PLUSEQUAL,OP_MINUSEQUAL,OP_STAREQUAL,OP_SLASHEQUAL,OP_PERCENTEQUAL,
    OP_AMPEQUAL,OP_CARETEQUAL,OP_PIPEEQUAL,OP_LESSLESSEQUAL,OP_GREATERGREATEREQUAL
};
}
%{

#define yyerror(x)                                                             \
  do {                                                                         \
    llvm::errs() << (x);                                                       \
  } while (0)  
#include "parser.hh"
#include "defs.h"  


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


long long inum;
double fnum;
char number_buffer[25];

Info curinfo;
} 

auto yylex() {
  auto tk = wk_getline();
  auto b = tk.find("'") + 1, e = tk.rfind("'");
  auto s = tk.substr(b, e - b).str(), t = tk.substr(0, tk.find(" ")).str();
  if (t == "numeric_constant") {

    int branch;
    if(s.find('.') == std::string::npos)
    {
      if(s.length() > 1 && s[0] == '0')
      {
        if(tolower(s[1]) == 'x')
        {
          if(s.find('p') == std::string::npos && s.find('P') == std::string::npos)
            branch = 3;
          else 
            branch = 4;
        }
        else
          branch = 2;
      }
      else
      {
        if(s.find('e') == std::string::npos && s.find('E') == std::string::npos)
          branch = 1;
        else 
          branch = 4;
      }
    }
    else
      branch = 4;
    
    if(branch != 4)
    {
      if(branch != 1)
      {
        if(branch == 3)
          sscanf(s.c_str(),"%llx",&inum);
        else
          sscanf(s.c_str(),"%llo",&inum);
        sprintf(number_buffer,"%lld",inum);
          s = number_buffer;
      }
      else
        sscanf(s.c_str(),"%lld",&inum);
      yylval.tree = new LiteralTree("IntegerLiteral", "", s);  
      
      if(inum >= 0x80000000L || inum < -(0x80000000L))
        dynamic_cast<LiteralTree *>(yylval.tree) -> info.type = Longlong;
      else
        dynamic_cast<LiteralTree *>(yylval.tree) -> info.type = Int;
    }
    else 
    {
      sscanf(s.c_str(),"%lf",&fnum);
      sprintf(number_buffer,"%.6e",fnum);
      s = number_buffer;
      yylval.tree = new LiteralTree("FloatingLiteral", "", s); 
      dynamic_cast<LiteralTree *>(yylval.tree) -> info.type = Double; 
    }
  
    return T_NUMERIC_CONSTANT;
  }
  if (t == "identifier") {
    yylval.tree = new Tree("id", s);
    return T_IDENTIFIER;
  }
  if (t == "string_literal")
  {
    yylval.tree = new LiteralTree("StringLiteral", "",s);
    return T_STRING_LITERAL;
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
  root->accept(new Print);
  root->accept(new ToJson);

  llvm::outs() << root->temp << "\n";
}
%}
%union 
{ Tree* tree;
  Info * info;
  Layer * layer;
  OpType opcode;
}

%token T_NUMERIC_CONSTANT
%token T_IDENTIFIER
%token T_INT
%token T_FLOAT
%token T_LONG
%token T_CHAR
%token T_RETURN
%token T_SEMI
%token T_L_PAREN
%token T_R_PAREN
%token T_L_BRACE
%token T_R_BRACE
%token T_CONST;
%token T_COMMA;
%token T_L_SQUARE;
%token T_R_SQUARE;  
%token T_EQUAL;
%token T_VOID;
%token T_IF;
%token T_ELSE;
%token T_WHILE;
%token T_DO;
%token T_FOR;
%token T_BREAK;
%token T_CONTINUE;
%token T_TILDE; 
%token T_PLUS;
%token T_MINUS;  
%token T_EXCLAIM;
%token T_STAR;
%token T_SLASH;
%token T_PERCENT;
%token T_LESS;
%token T_GREATER;
%token T_LESSEQUAL;
%token T_GREATEREQUAL;
%token T_EQUALEQUAL;
%token T_EXCLAIMEQUAL;
%token T_AMPAMP;
%token T_PIPEPIPE;
%token T_AMP;
%token T_CARET;
%token T_PIPE;
%token T_LESSLESS;
%token T_GREATERGREATER;
%token T_ELLIPSIS;
%token T_STRING_LITERAL;
%token T_PLUSEQUAL;
%token T_MINUSEQUAL;
%token T_STAREQUAL;
%token T_SLASHEQUAL;
%token T_PERCENTEQUAL;
%token T_AMPEQUAL;
%token T_CARETEQUAL;
%token T_PIPEEQUAL;
%token T_LESSLESSEQUAL;
%token T_GREATERGREATEREQUAL;
%token T_PLUSPLUS;
%token T_MINUSMINUS; 
%start Begin
%%
Begin: CompUnit {
    root = $<tree>1;
    //root -> SemAlysis();
  }
  

CompUnit: GlobalDecl {
    auto ptr = new TranslationUnitTree("TranslationUnitDecl");
    ptr->mergeSon($<tree>1);
    $<tree>$ = ptr;
  }
| CompUnit GlobalDecl {
    auto ptr = dynamic_cast<TranslationUnitTree *>($<tree>1);
    ptr -> mergeSon($<tree>2);
    $<tree>$ = ptr;
}
	

GlobalDecl: Decl {
    $<tree>$ = $<tree>1;
}
| FuncDef{
    auto ptr = new TranslationUnitTree("Temp");
    ptr->addSon($<tree>1);
    $<tree>$ = ptr;
}
| FuncDecl{
    auto ptr = new TranslationUnitTree("Temp");
    ptr->addSon($<tree>1);
    $<tree>$ = ptr;
};

FuncD: BType T_IDENTIFIER T_L_PAREN T_R_PAREN {
    auto ptr = new FuncDeclTree("FunctionDecl", $<tree>2->name);
    ptr -> info = *$<info>1;
    free($<tree>2);
    free($<info>1);
    $<tree>$ = ptr;
}
|  BType T_IDENTIFIER T_L_PAREN FuncFParams T_R_PAREN {
    auto ptr = dynamic_cast<FuncDeclTree *>($<tree>4);
    ptr -> name = $<tree>2->name;
    ptr -> info = *$<info>1;
    ptr -> GetParaType();
    free($<tree>2);
    free($<info>1);
    $<tree>$ = ptr;
}
| T_VOID T_IDENTIFIER T_L_PAREN T_R_PAREN {
    auto ptr = new FuncDeclTree("FunctionDecl", $<tree>2->name);
    free($<tree>2);
    $<tree>$ = ptr;
}
|  T_VOID T_IDENTIFIER T_L_PAREN FuncFParams T_R_PAREN {
    auto ptr = dynamic_cast<FuncDeclTree *>($<tree>4);
    ptr->name = $<tree>2->name;
    ptr -> GetParaType();
    free($<tree>2);
    $<tree>$ = ptr;
};

FuncDef: FuncD Block {
    auto ptr = dynamic_cast<FuncDeclTree *>($<tree>1);
    ptr -> body = dynamic_cast<BlockStmtTree *>($<tree>2);
    $<tree>$ = ptr;
};

FuncDecl: FuncD T_SEMI {
    auto ptr = dynamic_cast<FuncDeclTree *>($<tree>1);
    ptr -> body = nullptr;
    $<tree>$ = ptr;
};


FuncFParams : FuncFParam {
    auto ptr = new FuncDeclTree(0,"FunctionDecl");
    ptr -> addSon(dynamic_cast<VarDeclTree *>($<tree>1));
    $<tree>$ = ptr;
}
| FuncFParams T_COMMA FuncFParam {
    auto ptr = dynamic_cast<FuncDeclTree *>($<tree>1);
    ptr -> addSon(dynamic_cast<VarDeclTree *>($<tree>3));
    $<tree>$ = ptr;
};

FuncFParam : BType T_IDENTIFIER {
    auto ptr = new VarDeclTree("ParmVarDecl", $<tree>2->name);
    ptr -> info.type = $<info>1->type;
    ptr -> initval = nullptr;
    free($<info>1);
    free($<tree>2);
    $<tree>$ = ptr;
}
|  BType T_IDENTIFIER T_L_SQUARE T_R_SQUARE
{
    auto ptr = new VarDeclTree("ParmVarDecl", $<tree>2->name);
    ptr -> info.type = $<info>1->type;
    ptr -> initval = nullptr;
    free($<info>1);
    ptr -> info.layer.emplace_back();
    free($<tree>2);
    $<tree>$ = ptr;
}
|  BType T_IDENTIFIER T_L_SQUARE T_R_SQUARE ConstDims
{
    auto ptr = new VarDeclTree("ParmVarDecl", $<tree>2->name);
    ptr -> info.type = $<info>1->type;
    ptr -> initval = nullptr;
    free($<info>1);
    ptr -> info.layer = std::move(*$<layer>5);
    ptr -> info.layer.emplace_back();
    free($<tree>2);
    $<tree>$ = ptr;
}
| T_CONST BType T_IDENTIFIER {
    auto ptr = new VarDeclTree("ParmVarDecl", $<tree>3->name);
    ptr -> info.type = $<info>2->type;
    ptr -> initval = nullptr;
    ptr -> info.isConst = 1;
    free($<info>2);
    free($<tree>3);
    $<tree>$ = ptr;
}
| T_CONST BType T_IDENTIFIER T_L_SQUARE T_R_SQUARE
{
    auto ptr = new VarDeclTree("ParmVarDecl", $<tree>3->name);
    ptr -> info.type = $<info>2->type;
    ptr -> initval = nullptr;
    ptr -> info.isConst = 1;
    free($<info>2);
    ptr -> info.layer.emplace_back();
    free($<tree>3);
    $<tree>$ = ptr;
}
| T_CONST BType T_IDENTIFIER T_L_SQUARE T_R_SQUARE ConstDims
{
    auto ptr = new VarDeclTree("ParmVarDecl", $<tree>3->name);
    ptr -> info.type = $<info>2->type;
    ptr -> initval = nullptr;
    ptr -> info.isConst = 1;
    free($<info>2);
    ptr -> info.layer = std::move(*$<layer>6);
    ptr -> info.layer.emplace_back();
    free($<tree>3);
    $<tree>$ = ptr;
};


FuncCall: T_IDENTIFIER T_L_PAREN T_R_PAREN{
    auto ptr = new FuncCallTree("CallExpr");
    auto tp1 = new DeclRefTree("DeclRefExpr", $<tree>1->name);
    auto tp2 = new ImplicitCastExprTree("ImplicitCastExpr");
    tp2 -> castkind = "FunctionToPointerDecay";
    tp2 -> cast = dynamic_cast<ExprTree *>(tp1);
    ptr -> iden = dynamic_cast<ExprTree *>(tp2);
    free($<tree>1);
    $<tree>$ = ptr;
}
|  T_IDENTIFIER T_L_PAREN FuncRParams T_R_PAREN{
    auto ptr = dynamic_cast<FuncCallTree *>($<tree>3);
    auto tp1 = new DeclRefTree("DeclRefExpr", $<tree>1->name);
    auto tp2 = new ImplicitCastExprTree("ImplicitCastExpr");
    tp2 -> castkind = "FunctionToPointerDecay";
    tp2 -> cast = dynamic_cast<ExprTree *>(tp1);
    ptr -> iden = dynamic_cast<ExprTree *>(tp2);
    free($<tree>1);
    $<tree>$ = ptr;
};

FuncRParams : AssignExp {
    auto ptr = new FuncCallTree("CallExpr");
    ptr -> addSon(dynamic_cast<ExprTree *>($<tree>1));
    $<tree>$ = ptr;
}
| FuncRParams T_COMMA AssignExp {
    auto ptr = dynamic_cast<FuncCallTree *>($<tree>1);
    ptr -> addSon(dynamic_cast<ExprTree *>($<tree>3));
    $<tree>$ = ptr;
};

Block: T_L_BRACE BlockItems T_R_BRACE {
    $<tree>$ = $<tree>2;
  }
| T_L_BRACE T_R_BRACE {
    auto ptr = new BlockStmtTree("CompoundStmt");
    $<tree>$ = ptr;
} ;

BlockItems: BlockItem {
    auto ptr = new BlockStmtTree("CompoundStmt");
    ptr -> addSon($<tree>1);
    $<tree>$ = ptr;
}
| BlockItems BlockItem {
    auto ptr = dynamic_cast<BlockStmtTree *>($<tree>1);
    ptr -> addSon($<tree>2);
    $<tree>$ = ptr;
};

BlockItem: Decl {
    $<tree>$ = $<tree>1;
}
| Stmt {
    $<tree>$ = $<tree>1; 
};

Decl: ConstDecl {
    $<tree>$ = $<tree>1;
}
| VarDecl {
    $<tree>$ = $<tree>1;
};

ConstDecl : T_CONST BType ConstDefs T_SEMI {
    auto ptr = dynamic_cast<DeclStmtTree *>($<tree>3);
    ptr -> SpreadType($<info>2 -> type);
    free($<info>2);
    $<tree>$ = ptr;
};

VarDecl : BType VarDefs T_SEMI {

    auto ptr = dynamic_cast<DeclStmtTree *>($<tree>2);
    ptr -> SpreadType($<info>1 -> type);
    free($<info>1);
    $<tree>$ = ptr;
};

BType : T_INT{$<info>$ = new Info(Int);}
| T_LONG T_LONG {$<info>$ = new Info(Longlong);}
| T_CHAR {$<info>$ = new Info(Char);}
| T_FLOAT {$<info>$ = new Info(Float);};

ConstDefs : ConstDef {
    auto ptr = new DeclStmtTree("DeclStmt");
    ptr -> addSon(dynamic_cast<VarDeclTree *>($<tree>1));
    $<tree>$ = ptr;
}
| ConstDefs T_COMMA ConstDef {
    auto ptr = dynamic_cast<DeclStmtTree *>($<tree>1);
    ptr -> addSon(dynamic_cast<VarDeclTree *>($<tree>3));
    $<tree>$ = ptr;
};

ConstDef : T_IDENTIFIER T_EQUAL ConstInitVal {
    auto ptr = new VarDeclTree(0,"VarDecl", $<tree>1->name);
    ptr -> info.isConst = 1;
    ptr -> initval = dynamic_cast<ExprTree *>($<tree>3);
    free($<tree>1);
    $<tree>$ = ptr;
}
| T_IDENTIFIER ConstDims T_EQUAL ConstInitVal {
    auto ptr = new VarDeclTree(0,"VarDecl", $<tree>1->name);
    ptr -> info.isConst = 1;
    ptr -> info.layer = std::move(*$<layer>2);
    ptr -> initval = dynamic_cast<ExprTree *>($<tree>4);
    free($<tree>1);
    $<tree>$ = ptr;
};

VarDefs : VarDef {
    auto ptr = new DeclStmtTree("DeclStmt");
    ptr -> addSon(dynamic_cast<VarDeclTree *>($<tree>1));
    $<tree>$ = ptr;
}
| VarDefs T_COMMA VarDef {
    auto ptr = dynamic_cast<DeclStmtTree *>($<tree>1);
    ptr -> addSon(dynamic_cast<VarDeclTree *>($<tree>3));
    $<tree>$ = ptr;
};

VarDef : T_IDENTIFIER T_EQUAL InitVal {
    auto ptr = new VarDeclTree(0,"VarDecl", $<tree>1->name);
    ptr -> initval = dynamic_cast<ExprTree *>($<tree>3);
    free($<tree>1);
    $<tree>$ = ptr;
}
| T_IDENTIFIER {
    auto ptr = new VarDeclTree(0,"VarDecl", $<tree>1->name);
    ptr -> initval = nullptr;
    free($<tree>1);
    $<tree>$ = ptr;
}
| T_IDENTIFIER ConstDims T_EQUAL InitVal {
    auto ptr = new VarDeclTree(0,"VarDecl", $<tree>1->name);
    ptr -> info.layer = std::move(*$<layer>2);
    ptr -> initval = dynamic_cast<ExprTree *>($<tree>4);
    free($<tree>1);
    $<tree>$ = ptr;
}
| T_IDENTIFIER ConstDims {
    auto ptr = new VarDeclTree("VarDecl", $<tree>1->name);
    ptr -> info.layer = std::move(*$<layer>2);
    ptr -> initval = nullptr;
    free($<tree>1);
    $<tree>$ = ptr;
};

ConstInitVal : ConstExp {
    $<tree>$ = $<tree>1;
}
| T_L_BRACE T_R_BRACE
{
    auto ptr = new InitListExprTree("InitListExpr");
    //auto tmp = new Tree(0,"array_filler: ImplicitValueInitExpr");
    //ptr->addSon(0,tmp);
    $<tree>$ = ptr;
}
| T_L_BRACE ConstInitVals T_R_BRACE
{
    $<tree>$ = $<tree>2;
};

ConstInitVals : ConstInitVal {
    auto ptr = new InitListExprTree("InitListExpr");
    //ptr->addSon(0,$<tree>1);
    //ptr->info = $<tree>1 -> info;
    $<tree>$ = ptr;
}
| ConstInitVals T_COMMA ConstInitVal {
    auto ptr = $<tree>1;
    //ptr->addSon(0,$<tree>3);
    $<tree>$ = ptr;
}


ConstExp : LOrExp {
    $<tree>$ = $<tree>1;
};

InitVal : LOrExp {
    $<tree>$ = $<tree>1;
}
| T_L_BRACE T_R_BRACE
{
    auto ptr = new InitListExprTree("InitListExpr");
    //auto tmp = new Tree(0,"array_filler: ImplicitValueInitExpr");
    //ptr->addSon(0,tmp);
    $<tree>$ = ptr;
}
| T_L_BRACE InitVals T_R_BRACE
{
    $<tree>$ = $<tree>2;
};

InitVals : InitVal {
    auto ptr = new InitListExprTree("InitListExpr");
    //ptr->info = $<tree>1 -> info;
    //ptr->addSon(0,$<tree>1);
    $<tree>$ = ptr;
}
| InitVals T_COMMA InitVal {
    auto ptr = $<tree>1;
    //ptr->addSon(0,$<tree>3);
    $<tree>$ = ptr;
};

ConstDims :ConstDim {
    auto ptr= new Layer;
    ptr->emplace_back();
    $<layer>$ = ptr;
}
| ConstDim ConstDims {
    
    auto ptr = $<layer>2;
    ptr -> emplace_back($<layer>1 -> len[0]);
    free($<layer>1);
    $<layer>$ = ptr;
};

ConstDim : T_L_SQUARE ConstExp T_R_SQUARE {};


Dim : T_L_SQUARE LOrExp T_R_SQUARE {$<tree>$ = $<tree>2;};

LVal: T_IDENTIFIER {
    auto ptr = new DeclRefTree("DeclRefExpr", $<tree>1->name);
    free($<tree>1);
    $<tree>$ = ptr;
}
| LVal Dim{
    auto ptr = new ArraySubscriptExprTree("ArraySubscriptExpr");
    auto tmp = new ImplicitCastExprTree("ImplicitCastExpr");
    tmp -> cast = dynamic_cast<ExprTree *>($<tree>1);
    tmp -> info = tmp -> cast -> info;
    tmp -> castkind = "ArrayToPointerDecay";
    ptr -> sym = dynamic_cast<ExprTree *>(tmp);
    ptr -> script = dynamic_cast<ExprTree *>($<tree>2);
    ptr -> isLeftVal = 1;
    ptr -> info = tmp -> cast -> info;
    ptr -> info.layer.pop_back();
    $<tree>$ = ptr;
}
;

Stmt: T_RETURN Exp T_SEMI {
    auto ptr = new ReturnStmtTree("ReturnStmt");
    ptr -> expr = dynamic_cast<ExprTree *>($<tree>2);
    $<tree>$ = ptr;
}
| T_RETURN T_SEMI {
    auto ptr = new ReturnStmtTree("ReturnStmt");
    ptr -> expr = nullptr;
    $<tree>$ = ptr;
}
| Block {$<tree>$ = $<tree>1;}
| Exp T_SEMI {
    auto ptr = new ExprStmtTree("ExprStmt"); 
    ptr -> expr = dynamic_cast<ExprTree *>($<tree>1);
    $<tree>$ = ptr;
}
| T_SEMI {
    auto ptr = new Tree("NullStmt");
    $<tree>$ = ptr;
}
| T_IF T_L_PAREN Exp T_R_PAREN Stmt {
    auto ptr = new IfStmtTree("IfStmt");
    ptr -> cond = dynamic_cast<ExprTree *>($<tree>3);
    ptr -> truestmt = dynamic_cast<StmtTree *>($<tree>5);
    $<tree>$ = ptr;
} 
| T_IF T_L_PAREN Exp T_R_PAREN Stmt T_ELSE Stmt {
    auto ptr = new IfStmtTree("IfStmt");
    ptr -> cond = dynamic_cast<ExprTree *>($<tree>3);
    ptr -> truestmt = dynamic_cast<StmtTree *>($<tree>5);
    ptr -> falsestmt = dynamic_cast<StmtTree *>($<tree>7);
    $<tree>$ = ptr;
}
| T_WHILE T_L_PAREN Exp T_R_PAREN Stmt {
    auto ptr = new WhileStmtTree("WhileStmt");
    ptr -> cond = dynamic_cast<ExprTree *>($<tree>3);
    ptr -> stmt = dynamic_cast<StmtTree *>($<tree>5);
    $<tree>$ = ptr;
}
| T_DO Block T_WHILE T_L_PAREN Exp T_R_PAREN T_SEMI {
    auto ptr = new DoStmtTree("DoStmt");
    ptr -> stmt = dynamic_cast<StmtTree *>($<tree>2);
    ptr -> cond = dynamic_cast<ExprTree *>($<tree>5);
    $<tree>$ = ptr;
}
| T_FOR T_L_PAREN Exp T_SEMI Exp T_SEMI Exp T_R_PAREN Stmt {
    auto ptr = new ForStmtTree(0,"ForStmt");
    ptr -> init = dynamic_cast<ExprTree *>($<tree>3);
    auto nptr = new ExprTree(0,"");
    ptr -> null = dynamic_cast<ExprTree *>(nptr);
    ptr -> cond = dynamic_cast<ExprTree *>($<tree>5);
    ptr -> incr = dynamic_cast<ExprTree *>($<tree>7);
    ptr -> stmt = dynamic_cast<StmtTree *>($<tree>9);
    $<tree>$ = ptr;
}
| T_BREAK T_SEMI {
    auto ptr = new BreakStmtTree("BreakStmt");
    $<tree>$ = ptr;
}
| T_CONTINUE T_SEMI {
    auto ptr = new ContinueStmtTree("ContinueStmt");
    $<tree>$ = ptr;
};


Exp: AssignExp {
    $<tree>$ = $<tree>1;
}
| Exp T_COMMA AssignExp {
    auto ptr = new BinaryExprTree("BinaryOperator");
    ptr -> left = dynamic_cast<ExprTree *>($<tree>1);
    ptr -> right = dynamic_cast<ExprTree *>($<tree>3);
    ptr -> opcode = OP_COMMA;
    $<tree>$ = ptr;
}

PostfixExp: PrimaryExp  {
    $<tree>$ = $<tree>1;
}
| LVal PostfixOp {
    auto ptr = new UnaryExprTree("UnaryOperator");
    ptr -> son = dynamic_cast<ExprTree *>($<tree>1);
    ptr -> isPostfix = 1;
    ptr -> opcode = $<opcode>2;
    ptr -> info = ptr -> son -> info;
    $<tree>$ = ptr;
};

PostfixOp:T_PLUSPLUS{$<opcode>$ = OP_PLUSPLUS;}
| T_MINUSMINUS {$<opcode>$ = OP_MINUSMINUS;};

UnaryExp: PostfixExp  {
    $<tree>$ = $<tree>1;
}
| FuncCall {
    $<tree>$ = $<tree>1;
}
| UnaryOp UnaryExp {
    auto ptr = new UnaryExprTree("UnaryOperator");
    ptr -> son = dynamic_cast<ExprTree *>($<tree>2);
    ptr->isPostfix = 0;
    ptr->opcode = $<opcode>1;
    ptr -> info = ptr -> son -> info;
    $<tree>$ = ptr;
}
|  PostfixOp LVal{
    auto ptr = new UnaryExprTree("UnaryOperator");
    ptr -> son = dynamic_cast<ExprTree *>($<tree>2);
    ptr->isPostfix = 0;
    ptr->opcode = $<opcode>1;
    ptr -> info = ptr -> son -> info;
    $<tree>$ = ptr;
};

UnaryOp:T_PLUS {$<opcode>$ = OP_PLUS;}
| T_MINUS {$<opcode>$ = OP_MINUS;}
| T_EXCLAIM {$<opcode>$ = OP_EXCLAIM;}
| T_TILDE {$<opcode>$ = OP_TILDE;};


PrimaryExp: T_NUMERIC_CONSTANT {
    $<tree>$ = $<tree>1;
}
| T_L_PAREN Exp T_R_PAREN {
    auto ptr = new ParenExprTree("ParenExpr");
    ptr -> son = dynamic_cast<ExprTree *>($<tree>2);
    ptr -> info = ptr -> son -> info;
    $<tree>$ = ptr;
}
| LVal {
    
    $<tree>$ = $<tree>1;
}
| String_Literals {
  $<tree>$ = $<tree>1;
}

String_Literals :T_STRING_LITERAL{
     
    auto ptr = dynamic_cast<LiteralTree *>($<tree>1);
    ptr -> info.type = Char;
    ptr -> info.layer.emplace_back(ptr -> value.length()+1);
    $<tree>$ = ptr;
}
| String_Literals T_STRING_LITERAL{
    auto ptr = $<tree>1;
    ptr -> value = ptr -> value.substr(0,ptr-> value.length()-1) + $<tree>2 -> value.substr(1);
    free($<tree>2);
    $<tree>$ = ptr;
};

MulExp : UnaryExp { $<tree>$ = $<tree>1; }
| MulExp MulOp UnaryExp {
    auto ptr = new BinaryExprTree("BinaryOperator");
    ptr -> left = dynamic_cast<ExprTree *>($<tree>1);
    ptr -> right = dynamic_cast<ExprTree *>($<tree>3);
    ptr -> opcode = $<opcode>2;
    $<tree>$ = ptr;
}

MulOp : T_STAR{$<opcode>$ = OP_STAR;}
| T_SLASH{$<opcode>$ = OP_SLASH;}
| T_PERCENT{$<opcode>$ = OP_PERCENT;};

AddExp : MulExp { $<tree>$ = $<tree>1; }
| AddExp AddOp MulExp {
    auto ptr = new BinaryExprTree("BinaryOperator");
    ptr -> left = dynamic_cast<ExprTree *>($<tree>1);
    ptr -> right = dynamic_cast<ExprTree *>($<tree>3);
    ptr -> opcode = $<opcode>2;
    $<tree>$ = ptr;
};

AddOp : T_PLUS{$<opcode>$ = OP_PLUS;}
| T_MINUS{$<opcode>$ = OP_MINUS;};

ShiftExp : AddExp { $<tree>$ = $<tree>1; }
| ShiftExp ShiftOp AddExp {
    auto ptr = new BinaryExprTree("BinaryOperator");
    ptr -> left = dynamic_cast<ExprTree *>($<tree>1);
    ptr -> right = dynamic_cast<ExprTree *>($<tree>3);
    ptr -> opcode = $<opcode>2;
    $<tree>$ = ptr;
};

ShiftOp : T_LESSLESS{$<opcode>$ = OP_LESSLESS;}
| T_GREATERGREATER{$<opcode>$ = OP_GREATERGREATER;};

RelExp : ShiftExp { $<tree>$ = $<tree>1; }
| RelExp RelOp ShiftExp {
    auto ptr = new BinaryExprTree("BinaryOperator");
    ptr -> left = dynamic_cast<ExprTree *>($<tree>1);
    ptr -> right = dynamic_cast<ExprTree *>($<tree>3);
    ptr -> opcode = $<opcode>2;
    $<tree>$ = ptr;
};

RelOp :T_LESS{$<opcode>$ = OP_LESS;}
| T_GREATER{$<opcode>$ = OP_GREATER;} 
| T_LESSEQUAL{$<opcode>$ = OP_LESSEQUAL;}
| T_GREATEREQUAL{$<opcode>$ = OP_GREATEREQUAL;};

EqExp : RelExp { $<tree>$ = $<tree>1; }
| EqExp EqOp RelExp {
    auto ptr = new BinaryExprTree("BinaryOperator");
    ptr -> left = dynamic_cast<ExprTree *>($<tree>1);
    ptr -> right = dynamic_cast<ExprTree *>($<tree>3);
    ptr -> opcode = $<opcode>2;
    $<tree>$ = ptr;
};

EqOp :T_EQUALEQUAL{$<opcode>$ = OP_EQUALEQUAL;}
| T_EXCLAIMEQUAL{$<opcode>$ = OP_EXCLAIMEQUAL;}

BAndExp : EqExp { $<tree>$ = $<tree>1; }
| BAndExp T_AMP EqExp {
    auto ptr = new BinaryExprTree("BinaryOperator");
    ptr -> left = dynamic_cast<ExprTree *>($<tree>1);
    ptr -> right = dynamic_cast<ExprTree *>($<tree>3);
    ptr -> opcode = OP_AMP;
    $<tree>$ = ptr;
};

BXorExp : BAndExp { $<tree>$ = $<tree>1; }
| BXorExp T_CARET BAndExp {
    auto ptr = new BinaryExprTree("BinaryOperator");
    ptr -> left = dynamic_cast<ExprTree *>($<tree>1);
    ptr -> right = dynamic_cast<ExprTree *>($<tree>3);
    ptr -> opcode = OP_CARET;
    $<tree>$ = ptr;
};

BOrExp : BXorExp { $<tree>$ = $<tree>1; }
| BOrExp T_PIPE BXorExp {
    auto ptr = new BinaryExprTree("BinaryOperator");
    ptr -> left = dynamic_cast<ExprTree *>($<tree>1);
    ptr -> right = dynamic_cast<ExprTree *>($<tree>3);
    ptr -> opcode = OP_PIPE;
    $<tree>$ = ptr;
};

LAndExp : BOrExp { $<tree>$ = $<tree>1; }
| LAndExp T_AMPAMP BOrExp {
    auto ptr = new BinaryExprTree("BinaryOperator");
    ptr -> left = dynamic_cast<ExprTree *>($<tree>1);
    ptr -> right = dynamic_cast<ExprTree *>($<tree>3);
    ptr -> opcode = OP_AMPAMP;
    $<tree>$ = ptr;
};

LOrExp : LAndExp { $<tree>$ = $<tree>1; }
| LOrExp T_PIPEPIPE LAndExp {
    auto ptr = new BinaryExprTree("BinaryOperator");
    ptr -> left = dynamic_cast<ExprTree *>($<tree>1);
    ptr -> right = dynamic_cast<ExprTree *>($<tree>3);
    ptr -> opcode = OP_PIPEPIPE;
    $<tree>$ = ptr;
};

AssignExp : LOrExp {$<tree>$ = $<tree>1 ;}
| LVal T_EQUAL LOrExp {
    auto ptr = new BinaryExprTree("BinaryOperator");
    ptr -> left = dynamic_cast<ExprTree *>($<tree>1);
    ptr -> right = dynamic_cast<ExprTree *>($<tree>3);
    ptr -> opcode = OP_EQUAL;
    $<tree>$ = ptr;
}
| LVal AssignOp LOrExp {
    auto ptr = new BinaryExprTree(0,"CompoundAssignOperator");
    ptr -> left = dynamic_cast<ExprTree *>($<tree>1);
    ptr -> right = dynamic_cast<ExprTree *>($<tree>3);
    ptr -> opcode = $<opcode>2;
    $<tree>$ = ptr;
};

AssignOp : T_PLUSEQUAL {$<opcode>$ = OP_PLUSEQUAL;}
| T_MINUSEQUAL {$<opcode>$ = OP_MINUSEQUAL;}
| T_STAREQUAL {$<opcode>$ = OP_STAREQUAL;}
| T_SLASHEQUAL {$<opcode>$ = OP_SLASHEQUAL;}
| T_PERCENTEQUAL {$<opcode>$ = OP_PERCENTEQUAL;}
| T_AMPEQUAL {$<opcode>$ = OP_AMPEQUAL;}
| T_CARETEQUAL {$<opcode>$ = OP_CARETEQUAL;}
| T_PIPEEQUAL {$<opcode>$ = OP_PIPEEQUAL;}
| T_LESSLESSEQUAL {$<opcode>$ = OP_LESSLESSEQUAL;}
| T_GREATERGREATEREQUAL {$<opcode>$ = OP_GREATERGREATEREQUAL;};

%%