%code requires
{

class Tree;
class Info;

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
      yylval.tree = new Tree(0,"IntegerLiteral", "", s);  
      
      if(inum >= 0x80000000L || inum < -(0x80000000L))
        yylval.tree -> info.type = Longlong;
      else
        yylval.tree -> info.type = Int;
    }
    else 
    {
      sscanf(s.c_str(),"%lf",&fnum);
      sprintf(number_buffer,"%.6e",fnum);
      s = number_buffer;
      yylval.tree = new Tree(0,"FloatingLiteral", "", s); 
      yylval.tree -> info.type = Double; 
    }
  
    return T_NUMERIC_CONSTANT;
  }
  if (t == "identifier") {
    yylval.tree = new Tree(1,"id", s);
    return T_IDENTIFIER;
  }
  if (t == "string_literal")
  {
    yylval.tree = new Tree(0,"StringLiteral", "",s);
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
  root->print();
  llvm::outs() << root->toJson() << "\n";
}
%}
%union 
{ Tree* tree;
  Info * info;
  int layer;
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
  }
  

CompUnit: GlobalDecl {
    auto ptr = new Tree(0,"TranslationUnitDecl");
    ptr->mergeSon($<tree>1);
    $<tree>$ = ptr;
  }
| CompUnit GlobalDecl {
    auto ptr = $<tree>1;
    ptr -> mergeSon($<tree>2);
    $<tree>$ = ptr;
}
	

GlobalDecl: Decl {
    $<tree>$ = $<tree>1;
}
| FuncDef{
    auto ptr = new Tree(0,"Temp");
    ptr->addSon(0,$<tree>1);
    $<tree>$ = ptr;
}
| FuncDecl{
    auto ptr = new Tree(0,"Temp");
    ptr->addSon(0,$<tree>1);
    $<tree>$ = ptr;
};

FuncD: BType T_IDENTIFIER T_L_PAREN T_R_PAREN {
    auto ptr = new Tree(0,"FunctionDecl", $<tree>2->name);
    ptr -> info = *$<info>1;
    symt.insert(std::make_pair($<tree>2->name,*$<info>1));
    free($<tree>2);
    free($<info>1);
    curret  = ptr -> info;
    $<tree>$ = ptr;
}
|  BType T_IDENTIFIER T_L_PAREN FuncFParams T_R_PAREN {
    auto ptr = $<tree>4;
    ptr->name = $<tree>2->name;
    ptr -> info = *$<info>1;
    ptr -> GetParaType(ptr);
    yyerror("FuncD\n");
    yyerror(ptr -> info.args.size());
    yyerror("\n");
    symt.insert(std::make_pair($<tree>2->name,ptr -> info));
    free($<tree>2);
    free($<info>1);
    curret  = ptr -> info;
    $<tree>$ = ptr;
}
| T_VOID T_IDENTIFIER T_L_PAREN T_R_PAREN {
    auto ptr = new Tree(0,"FunctionDecl", $<tree>2->name);
    symt.insert(std::make_pair($<tree>2->name,Info()));
    free($<tree>2);
    $<tree>$ = ptr;
}
|  T_VOID T_IDENTIFIER T_L_PAREN FuncFParams T_R_PAREN {
    auto ptr = $<tree>4;
    ptr->name = $<tree>2->name;
    ptr -> GetParaType(ptr);
    symt.insert(std::make_pair($<tree>2->name,ptr -> info));
    free($<tree>2);
    $<tree>$ = ptr;
};

FuncDef: FuncD Block {
    auto ptr = $<tree>1;
    ptr->addSon(0,$<tree>2);
    yyerror("FuncDef\n");
    yyerror(ptr -> info.args.size());
    yyerror("\n");
    $<tree>$ = ptr;
};

FuncDecl: FuncD T_SEMI {
    $<tree>$ = $<tree>1;
};


FuncFParams : FuncFParam {
    auto ptr = new Tree(0,"FunctionDecl");
    ptr -> addSon(0,$<tree>1);
    $<tree>$ = ptr;
}
| FuncFParams T_COMMA FuncFParam {
    auto ptr = $<tree>1;
    ptr -> addSon(0,$<tree>3);
    $<tree>$ = ptr;
};

FuncFParam : BType T_IDENTIFIER {
    auto ptr = new Tree(0,"ParmVarDecl", $<tree>2->name);
    ptr -> info.type = $<info>1->type;
    free($<info>1);
    ptr -> info.layer = 0;
    symt.insert(std::make_pair($<tree>2->name,ptr->info));
    free($<tree>2);
    $<tree>$ = ptr;
}
|  BType T_IDENTIFIER T_L_SQUARE T_R_SQUARE
{
    auto ptr = new Tree(0,"ParmVarDecl", $<tree>2->name);
    ptr -> info.type = $<info>1->type;
    free($<info>1);
    ptr -> info.layer = 1;
    symt.insert(std::make_pair($<tree>2->name,ptr->info));
    free($<tree>2);
    $<tree>$ = ptr;
}
|  BType T_IDENTIFIER T_L_SQUARE T_R_SQUARE ConstDims
{
    auto ptr = new Tree(0,"ParmVarDecl", $<tree>2->name);
    ptr -> info.type = $<info>1->type;
    free($<info>1);
    ptr -> info.layer = $<layer>5 + 1;
    symt.insert(std::make_pair($<tree>2->name,ptr->info));
    free($<tree>2);
    $<tree>$ = ptr;
}
| T_CONST BType T_IDENTIFIER {
    auto ptr = new Tree(0,"ParmVarDecl", $<tree>3->name);
    ptr -> info.type = $<info>2->type;
    ptr -> info.isConst = 1;
    free($<info>2);
    ptr -> info.layer = 0;
    symt.insert(std::make_pair($<tree>3->name,ptr->info));
    free($<tree>3);
    $<tree>$ = ptr;
}
| T_CONST BType T_IDENTIFIER T_L_SQUARE T_R_SQUARE
{
    auto ptr = new Tree(0,"ParmVarDecl", $<tree>3->name);
    ptr -> info.type = $<info>2->type;
    ptr -> info.isConst = 1;
    free($<info>2);
    ptr -> info.layer = 1;
    symt.insert(std::make_pair($<tree>3->name,ptr->info));
    free($<tree>3);
    $<tree>$ = ptr;
}
| T_CONST BType T_IDENTIFIER T_L_SQUARE T_R_SQUARE ConstDims
{
    auto ptr = new Tree(0,"ParmVarDecl", $<tree>3->name);
    ptr -> info.type = $<info>2->type;
    ptr -> info.isConst = 1;
    free($<info>2);
    ptr -> info.layer = $<layer>5 + 1;
    symt.insert(std::make_pair($<tree>3->name,ptr->info));
    free($<tree>3);
    $<tree>$ = ptr;
};


FuncCall: T_IDENTIFIER T_L_PAREN T_R_PAREN{
    auto ptr = new Tree(0,"CallExpr");
    auto tp1 = new Tree(0,"DeclRefExpr", $<tree>1->name);
    auto tp2 = new Tree(0,"ImplicitCastExpr");
    tp2 -> addSon(0,tp1);
    ptr -> addSon(0,tp2);
    ptr -> info.type = symt[$<tree>1->name].type;
    free($<tree>1);
    $<tree>$ = ptr;
}
|  T_IDENTIFIER T_L_PAREN FuncRParams T_R_PAREN{
    auto ptr = $<tree>3;
    auto tp1 = new Tree(0,"DeclRefExpr", $<tree>1->name);
    auto tp2 = new Tree(0,"ImplicitCastExpr");
    tp2 -> addSon(0,tp1);
    ptr -> sons[0] = tp2;
    ptr -> info = symt[$<tree>1->name];
    yyerror("FuncCall\n");
    yyerror(ptr -> info.args.size());
    yyerror("\n");
    free($<tree>1);
    ptr -> AdjustParaType(ptr);
    $<tree>$ = ptr;
};

FuncRParams : AssignExp {
    auto ptr = new Tree(0,"CallExpr");
    ptr -> addSon(0,nullptr); 
    ptr -> addSon(0,$<tree>1);
    $<tree>$ = ptr;
}
| FuncRParams T_COMMA AssignExp {
    auto ptr = $<tree>1;
    ptr -> addSon(0,$<tree>3);
    $<tree>$ = ptr;
};

Block: T_L_BRACE BlockItems T_R_BRACE {
    $<tree>$ = $<tree>2;
  }
| T_L_BRACE T_R_BRACE {
    auto ptr = new Tree(0,"CompoundStmt");
    $<tree>$ = ptr;
}
  ;

BlockItems: BlockItem {
    auto ptr = new Tree(0,"CompoundStmt");
    ptr -> addSon(1,$<tree>1);
    $<tree>$ = ptr;
}
| BlockItems BlockItem {
    auto ptr = $<tree>1;
    ptr -> addSon(1,$<tree>2);
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
    auto ptr = $<tree>3;
    ptr -> SpreadType($<info>2 -> type);
    free($<info>2);
    $<tree>$ = ptr;
};

VarDecl : BType VarDefs T_SEMI {

    auto ptr = $<tree>2;
    ptr -> SpreadType($<info>1 -> type);
    free($<info>1);
    $<tree>$ = ptr;
};

BType : T_INT{$<info>$ = new Info(Int);}
| T_LONG T_LONG {$<info>$ = new Info(Longlong);}
| T_CHAR {$<info>$ = new Info(Char);}
| T_FLOAT {$<info>$ = new Info(Float);};

ConstDefs : ConstDef {
    auto ptr = new Tree(0,"DeclStmt");
    ptr -> addSon(0,$<tree>1);
    $<tree>$ = ptr;
}
| ConstDefs T_COMMA ConstDef {
    auto ptr = $<tree>1;
    ptr -> addSon(0,$<tree>3);
    $<tree>$ = ptr;
};

ConstDef : T_IDENTIFIER T_EQUAL ConstInitVal {
    auto ptr = new Tree(0,"VarDecl", $<tree>1->name);
    ptr -> info.isConst = 1;
    ptr -> addSon(0,$<tree>3);
    free($<tree>1);
    $<tree>$ = ptr;
}
| T_IDENTIFIER ConstDims T_EQUAL ConstInitVal {
    auto ptr = new Tree(0,"VarDecl", $<tree>1->name);
    ptr -> info.isConst = 1;
    ptr -> info.layer = $<layer>2;
    yyerror("ArrDecl");
    yyerror(ptr -> info.layer);
    yyerror("\n");
    ptr -> addSon(0,$<tree>4);
    free($<tree>1);
    $<tree>$ = ptr;
};

VarDefs : VarDef {
    auto ptr = new Tree(0,"DeclStmt");
    ptr -> addSon(0,$<tree>1);
    $<tree>$ = ptr;
}
| VarDefs T_COMMA VarDef {
    auto ptr = $<tree>1;
    ptr -> addSon(0,$<tree>3);
    $<tree>$ = ptr;
};

VarDef : T_IDENTIFIER T_EQUAL InitVal {
    auto ptr = new Tree(0,"VarDecl", $<tree>1->name);
    ptr -> addSon(0,$<tree>3);
    free($<tree>1);
    $<tree>$ = ptr;
}
| T_IDENTIFIER {
    auto ptr = new Tree(0,"VarDecl", $<tree>1->name);
    free($<tree>1);
    $<tree>$ = ptr;
}
| T_IDENTIFIER ConstDims T_EQUAL InitVal {
    auto ptr = new Tree(0,"VarDecl", $<tree>1->name);
    ptr -> info.layer = $<layer>2;
    yyerror("ArrDecl");
    yyerror(ptr -> info.layer);
    yyerror("\n");
    ptr -> addSon(0,$<tree>4);
    free($<tree>1);
    $<tree>$ = ptr;
}
| T_IDENTIFIER ConstDims {
    auto ptr = new Tree(0,"VarDecl", $<tree>1->name);
    ptr -> info.layer = $<layer>2;
    yyerror("ArrDecl");
    yyerror(ptr -> info.layer);
    yyerror("\n");
    free($<tree>1);
    $<tree>$ = ptr;
};

ConstInitVal : ConstExp {
    $<tree>$ = $<tree>1;
}
| T_L_BRACE T_R_BRACE
{
    auto ptr = new Tree(0,"InitListExpr");
    auto tmp = new Tree(0,"array_filler: ImplicitValueInitExpr");
    ptr->addSon(0,tmp);
    $<tree>$ = ptr;
}
| T_L_BRACE ConstInitVals T_R_BRACE
{
    $<tree>$ = $<tree>2;
};

ConstInitVals : ConstInitVal {
    auto ptr = new Tree(0,"InitListExpr");
    ptr->addSon(0,$<tree>1);
    ptr->info = $<tree>1 -> info;
    $<tree>$ = ptr;
}
| ConstInitVals T_COMMA ConstInitVal {
    auto ptr = $<tree>1;
    ptr->addSon(0,$<tree>3);
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
    auto ptr = new Tree(0,"InitListExpr");
    auto tmp = new Tree(0,"array_filler: ImplicitValueInitExpr");
    ptr->addSon(0,tmp);
    $<tree>$ = ptr;
}
| T_L_BRACE InitVals T_R_BRACE
{
    $<tree>$ = $<tree>2;
};

InitVals : InitVal {
    auto ptr = new Tree(0,"InitListExpr");
    ptr->info = $<tree>1 -> info;
    ptr->addSon(0,$<tree>1);
    $<tree>$ = ptr;
}
| InitVals T_COMMA InitVal {
    auto ptr = $<tree>1;
    ptr->addSon(0,$<tree>3);
    $<tree>$ = ptr;
};

ConstDims :ConstDim {
    $<layer>$ = 1;
}
| ConstDims ConstDim {
    $<layer>$ = $<layer>1 + 1;
};

ConstDim : T_L_SQUARE ConstExp T_R_SQUARE {};


Dim : T_L_SQUARE LOrExp T_R_SQUARE {$<tree>$ = $<tree>2;};

LVal: T_IDENTIFIER {
    auto ptr = new Tree(1,"DeclRefExpr", $<tree>1->name);
    ptr -> info = symt[$<tree>1->name];
    yyerror("Script");
    yyerror(ptr -> info.layer);
    yyerror("\n");
    free($<tree>1);
    $<tree>$ = ptr;
}
| LVal Dim{
    auto ptr = new Tree(1,"ArraySubscriptExpr");
    auto tmp = new Tree(1,"ImplicitCastExpr");
    tmp -> addSon(1,$<tree>1);
    tmp -> info = $<tree>1 -> info;
    ptr -> addSon(1,tmp);
    ptr -> addSon(0,$<tree>2);
    ptr -> isLeftVal = 1;
    ptr -> info = $<tree>1 -> info;
    ptr -> info.layer --;
    yyerror("Script");
    yyerror(ptr -> info.layer);
    yyerror("\n");
    $<tree>$ = ptr;
}
;

Stmt: T_RETURN Exp T_SEMI {
    auto ptr = new Tree(0,"ReturnStmt");
    ptr->addSon(0,$<tree>2);
    ptr -> RetAdjust(ptr);
    $<tree>$ = ptr;
}
| T_RETURN T_SEMI {
    auto ptr = new Tree(0,"ReturnStmt");
    $<tree>$ = ptr;
}
| Block {$<tree>$ = $<tree>1;}
| Exp T_SEMI {$<tree>$ = $<tree>1;}
| T_SEMI {
    auto ptr = new Tree(0,"NullStmt");
    $<tree>$ = ptr;
}
| T_IF T_L_PAREN Exp T_R_PAREN Stmt {
    auto ptr = new Tree(0,"IfStmt");
    ptr->addSon(0,$<tree>3);
    ptr->addSon(0,$<tree>5);
    $<tree>$ = ptr;
} 
| T_IF T_L_PAREN Exp T_R_PAREN Stmt T_ELSE Stmt {
    auto ptr = new Tree(0,"IfStmt");
    ptr->addSon(0,$<tree>3);
    ptr->addSon(0,$<tree>5);
    ptr->addSon(0,$<tree>7);
    $<tree>$ = ptr;
}
| T_WHILE T_L_PAREN Exp T_R_PAREN Stmt {
    auto ptr = new Tree(0,"WhileStmt");
    ptr->addSon(0,$<tree>3);
    ptr->addSon(0,$<tree>5);
    $<tree>$ = ptr;
}
| T_DO Block T_WHILE T_L_PAREN Exp T_R_PAREN T_SEMI {
    auto ptr = new Tree(0,"DoStmt");
    ptr->addSon(0,$<tree>2);
    ptr->addSon(0,$<tree>5);
    $<tree>$ = ptr;
}
| T_FOR T_L_PAREN Exp T_SEMI Exp T_SEMI Exp T_R_PAREN Stmt {
    auto ptr = new Tree(0,"ForStmt");
    ptr->addSon(1,$<tree>3);
    auto nptr = new Tree(0,"");
    ptr ->addSon(0,nptr);
    ptr->addSon(1,$<tree>5);
    ptr->addSon(1,$<tree>7);
    ptr->addSon(0,$<tree>9);
    $<tree>$ = ptr;
}
| T_BREAK T_SEMI {
    auto ptr = new Tree(0,"BreakStmt");
    $<tree>$ = ptr;
}
| T_CONTINUE T_SEMI {
    auto ptr = new Tree(0,"ContinueStmt");
    $<tree>$ = ptr;
};


Exp: AssignExp {
    $<tree>$ = $<tree>1;
}
| Exp T_COMMA AssignExp {
    auto ptr = new Tree(0,"BinaryOperator");
    ptr->addSon(1,$<tree>1);
    ptr->addSon(1,$<tree>3);
    $<tree>$ = ptr;
}

PostfixExp: PrimaryExp  {
    $<tree>$ = $<tree>1;
}
| LVal T_PLUSPLUS {
    auto ptr = new Tree(1,"UnaryOperator");
    ptr->addSon(1,$<tree>1);
    ptr->info = $<tree>1 -> info;
    $<tree>$ = ptr;
}
| LVal T_MINUSMINUS {
    auto ptr = new Tree(1,"UnaryOperator");
    ptr->addSon(1,$<tree>1);
    ptr->info = $<tree>1 -> info;
    $<tree>$ = ptr;
};

UnaryExp: PostfixExp  {
    $<tree>$ = $<tree>1;
}
| FuncCall {
    $<tree>$ = $<tree>1;
}
|T_PLUS UnaryExp {
    auto ptr = new Tree(0,"UnaryOperator");
    ptr->addSon(0,$<tree>2);
    ptr->info = $<tree>2 -> info;
    $<tree>$ = ptr;
}
| T_MINUS UnaryExp {
    auto ptr = new Tree(0,"UnaryOperator");
    ptr->addSon(0,$<tree>2);
    ptr->info = $<tree>2 -> info;
    $<tree>$ = ptr;
}
| T_EXCLAIM UnaryExp {
    auto ptr = new Tree(0,"UnaryOperator");
    ptr->addSon(0,$<tree>2);
    ptr->info = $<tree>2 -> info;
    $<tree>$ = ptr;
}
| T_TILDE UnaryExp {
    auto ptr = new Tree(0,"UnaryOperator");
    ptr->addSon(0,$<tree>2);
    ptr->info = $<tree>2 -> info;
    $<tree>$ = ptr;
}
|T_PLUSPLUS LVal {
    auto ptr = new Tree(1,"UnaryOperator");
    ptr->addSon(1,$<tree>2);
    ptr->info = $<tree>2 -> info;
    $<tree>$ = ptr;
}
| T_MINUSMINUS LVal {
    auto ptr = new Tree(1,"UnaryOperator");
    ptr->addSon(1,$<tree>2);
    ptr->info = $<tree>2 -> info;
    $<tree>$ = ptr;
};

PrimaryExp: T_NUMERIC_CONSTANT {
    $<tree>$ = $<tree>1;
}
| T_L_PAREN Exp T_R_PAREN {
    auto ptr = new Tree(1,"ParenExpr");
    ptr->addSon(1,$<tree>2);
    ptr->info = $<tree>2 -> info;
    $<tree>$ = ptr;
}
| LVal {
    
    $<tree>$ = $<tree>1;
}
| String_Literals {
  $<tree>$ = $<tree>1;
}

String_Literals :T_STRING_LITERAL{
     
    auto ptr = $<tree>1;
    ptr -> info.type = Char;
    ptr -> info.layer = 1;
    $<tree>$ = ptr;
}
| String_Literals T_STRING_LITERAL{
    auto ptr = $<tree>1;
    ptr -> value = ptr -> value.substr(0,ptr-> value.length()-1) + $<tree>2 -> value.substr(1);
    free($<tree>2);
    $<tree>$ = ptr;
};

MulExp : UnaryExp { $<tree>$ = $<tree>1; }
| MulExp T_STAR UnaryExp {
    auto ptr = new Tree(0,"BinaryOperator");
    ptr->addSon(0,$<tree>1);
    ptr->addSon(0,$<tree>3);
    ptr -> TryUpcast(ptr);
    $<tree>$ = ptr;
}
| MulExp T_SLASH UnaryExp {
    auto ptr = new Tree(0,"BinaryOperator");
    ptr->addSon(0,$<tree>1);
    ptr->addSon(0,$<tree>3);
    ptr -> TryUpcast(ptr);
    $<tree>$ = ptr;
}
| MulExp T_PERCENT UnaryExp {
    auto ptr = new Tree(0,"BinaryOperator");
    ptr->addSon(0,$<tree>1);
    ptr->addSon(0,$<tree>3);
    ptr -> TryUpcast(ptr);
    $<tree>$ = ptr;
};

AddExp : MulExp { $<tree>$ = $<tree>1; }
| AddExp T_PLUS MulExp {
    auto ptr = new Tree(0,"BinaryOperator");
    ptr->addSon(0,$<tree>1);
    ptr->addSon(0,$<tree>3);
    ptr -> TryUpcast(ptr);
    $<tree>$ = ptr;
}
| AddExp T_MINUS MulExp {
    auto ptr = new Tree(0,"BinaryOperator");
    ptr->addSon(0,$<tree>1);
    ptr->addSon(0,$<tree>3);
    ptr -> TryUpcast(ptr);
    $<tree>$ = ptr;
};

ShiftExp : AddExp { $<tree>$ = $<tree>1; }
| ShiftExp T_LESSLESS AddExp {
    auto ptr = new Tree(0,"BinaryOperator");
    ptr->addSon(0,$<tree>1);
    ptr->addSon(0,$<tree>3);
    ptr -> TryUpcast(ptr);
    $<tree>$ = ptr;
}
| ShiftExp T_GREATERGREATER AddExp {
    auto ptr = new Tree(0,"BinaryOperator");
    ptr->addSon(0,$<tree>1);
    ptr->addSon(0,$<tree>3);
    ptr -> TryUpcast(ptr);
    $<tree>$ = ptr;
};

RelExp : ShiftExp { $<tree>$ = $<tree>1; }
| RelExp T_LESS ShiftExp {
    auto ptr = new Tree(0,"BinaryOperator");
    ptr->addSon(0,$<tree>1);
    ptr->addSon(0,$<tree>3);
    ptr -> TryUpcast(ptr);
    $<tree>$ = ptr;
}
| RelExp T_GREATER ShiftExp {
    auto ptr = new Tree(0,"BinaryOperator");
    ptr->addSon(0,$<tree>1);
    ptr->addSon(0,$<tree>3);
    ptr -> TryUpcast(ptr);
    $<tree>$ = ptr;
}
| RelExp T_LESSEQUAL ShiftExp {
    auto ptr = new Tree(0,"BinaryOperator");
    ptr->addSon(0,$<tree>1);
    ptr->addSon(0,$<tree>3);
    ptr -> TryUpcast(ptr);
    $<tree>$ = ptr;
}
| RelExp T_GREATEREQUAL ShiftExp {
    auto ptr = new Tree(0,"BinaryOperator");
    ptr->addSon(0,$<tree>1);
    ptr->addSon(0,$<tree>3);
    ptr -> TryUpcast(ptr);
    $<tree>$ = ptr;
};

EqExp : RelExp { $<tree>$ = $<tree>1; }
| EqExp T_EQUALEQUAL RelExp {
    auto ptr = new Tree(0,"BinaryOperator");
    ptr->addSon(0,$<tree>1);
    ptr->addSon(0,$<tree>3);
    ptr -> TryUpcast(ptr);
    $<tree>$ = ptr;
}
| EqExp T_EXCLAIMEQUAL RelExp {
    auto ptr = new Tree(0,"BinaryOperator");
    ptr->addSon(0,$<tree>1);
    ptr->addSon(0,$<tree>3);
    ptr -> TryUpcast(ptr);
    $<tree>$ = ptr;
};

BAndExp : EqExp { $<tree>$ = $<tree>1; }
| BAndExp T_AMP EqExp {
    auto ptr = new Tree(0,"BinaryOperator");
    ptr->addSon(0,$<tree>1);
    ptr->addSon(0,$<tree>3);
    ptr -> TryUpcast(ptr);
    $<tree>$ = ptr;
};

BXorExp : BAndExp { $<tree>$ = $<tree>1; }
| BXorExp T_CARET BAndExp {
    auto ptr = new Tree(0,"BinaryOperator");
    ptr->addSon(0,$<tree>1);
    ptr->addSon(0,$<tree>3);
    ptr -> TryUpcast(ptr);
    $<tree>$ = ptr;
};

BOrExp : BXorExp { $<tree>$ = $<tree>1; }
| BOrExp T_PIPE BXorExp {
    auto ptr = new Tree(0,"BinaryOperator");
    ptr->addSon(0,$<tree>1);
    ptr->addSon(0,$<tree>3);
    ptr -> TryUpcast(ptr);
    $<tree>$ = ptr;
};

LAndExp : BOrExp { $<tree>$ = $<tree>1; }
| LAndExp T_AMPAMP BOrExp {
    auto ptr = new Tree(0,"BinaryOperator");
    ptr->addSon(0,$<tree>1);
    ptr->addSon(0,$<tree>3);
    ptr -> info.type = Int;
    $<tree>$ = ptr;
};

LOrExp : LAndExp { $<tree>$ = $<tree>1; }
| LOrExp T_PIPEPIPE LAndExp {
    auto ptr = new Tree(0,"BinaryOperator");
    ptr->addSon(0,$<tree>1);
    ptr->addSon(0,$<tree>3);
    ptr -> info.type = Int;
    $<tree>$ = ptr;
};

AssignExp : LOrExp {$<tree>$ = $<tree>1 ;}
| LVal T_EQUAL LOrExp {
    auto ptr = new Tree(0,"BinaryOperator");
    ptr->addSon(1,$<tree>1);
    ptr->addSon(0,$<tree>3);
    ptr -> AssignAdjust(ptr);
    $<tree>$ = ptr;
}
| LVal AssignOp LOrExp {
    auto ptr = new Tree(0,"CompoundAssignOperator");
    ptr->addSon(1,$<tree>1);
    ptr->addSon(0,$<tree>3);
    ptr -> AssignAdjust(ptr);
    $<tree>$ = ptr;
};

AssignOp : T_PLUSEQUAL {}
| T_MINUSEQUAL {}
| T_STAREQUAL {}
| T_SLASHEQUAL {}
| T_PERCENTEQUAL {}
| T_AMPEQUAL {}
| T_CARETEQUAL {}
| T_PIPEEQUAL {}
| T_LESSLESSEQUAL {}
| T_GREATERGREATEREQUAL {};

%%