#include <vector>
#include <stack>
#include <cstdio>
#include <iostream>
#include <unordered_map>
#include <llvm/Support/JSON.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>

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
class ImplicitValueInitExprTree;

const char * typestr[6]={"char","int","long long","float","double","void"};

enum OpType
{
    OP_PLUSPLUS,OP_MINUSMINUS,OP_PLUS,OP_MINUS,OP_EXCLAIM,OP_TILDE,
    OP_STAR,OP_SLASH,OP_PERCENT,OP_LESSLESS,OP_GREATERGREATER,
    OP_LESS,OP_GREATER,OP_LESSEQUAL,OP_GREATEREQUAL,OP_EQUALEQUAL,
    OP_EXCLAIMEQUAL,OP_AMP,OP_CARET,OP_PIPE,OP_AMPAMP,OP_PIPEPIPE,OP_COMMA,
    OP_EQUAL,OP_PLUSEQUAL,OP_MINUSEQUAL,OP_STAREQUAL,OP_SLASHEQUAL,OP_PERCENTEQUAL,
    OP_AMPEQUAL,OP_CARETEQUAL,OP_PIPEEQUAL,OP_LESSLESSEQUAL,OP_GREATERGREATEREQUAL
};

enum Type
{
    Char,Int,Longlong,Float,Double,Void
};

enum LiteralType
{
    Interger,Floating,String
};

#define yyerror(x)                                                             \
  do {                                                                         \
    llvm::errs() << (x);                                                       \
  } while (0) 

std::string castlookup(Type a,Type b)
{
    if(a < Float & b < Float)
        return "IntegralCast";
    if(a < Float & b >= Float)
        return "IntegralToFloating";
    if(a >= Float & b < Float)
        return "FloatingToIntegral";
    if(a >= Float & b >= Float)
        return "FloatingCast";
}

int twoType(Type from,Type to)
{
    return from << 4 + to;
}


const char * opstr[34]=
{   "++","--","+","-","!","~","*","/","%","<<",">>",
    "<",">","<=",">=","==","!=","&","^","|","&&","||",",",
    "=","+=","-=","*=","/=","%=","&=","^=","|=","<<=",">>="
};

class Layer
{
    public:
        int num = 0;
        std::vector<size_t> len;
        void emplace_back(int n = 0)
        {
            num++;
            len.emplace_back(n);
        }
        size_t pop_back()
        {
            int i = len.back();
            len.pop_back();
            num--;
            return i;
        }
};

class Info
{
    public:
        Type type;
        Layer layer;
        bool isPointer = 0;
        bool isConst;
        Info(enum Type t = Void,bool ic = 0):
        type(t),isConst(ic){}
};

typedef std::unordered_map<std::string,DeclTree *> SymT;

class SymTNode
{
    public:
        SymT symt;
        SymTNode * father;
        std::vector<SymTNode *> sons;
};

SymTNode * cursym;
FuncDeclTree * curfunc;
std::stack<LoopStmtTree *> curloops;

class Visitor
{
    public:
        virtual void VisitTranslationUnitTree(TranslationUnitTree *  translationUnitTree){}
        virtual void VisitVarDeclTree(VarDeclTree * varDeclTree){}
        virtual void VisitFuncDeclTree(FuncDeclTree * funcDeclTree){}
        virtual void VisitDeclStmtTree(DeclStmtTree * declStmtTree){}
        virtual void VisitNullStmtTree(NullStmtTree * nullStmtTree){}
        virtual void VisitExprStmtTree(ExprStmtTree * exprStmtTree){}
        virtual void VisitBlockStmtTree(BlockStmtTree * blockStmtTree){}
        virtual void VisitIfStmtTree(IfStmtTree * ifStmtTree){}
        virtual void VisitWhileStmtTree(WhileStmtTree * whileStmtTree){}
        virtual void VisitDoStmtTree(DoStmtTree * doStmtTree){}
        virtual void VisitForStmtTree(ForStmtTree * forStmtTree){}
        virtual void VisitContinueStmtTree(ContinueStmtTree * continueStmtTree){}
        virtual void VisitBreakStmtTree(BreakStmtTree * breakStmtTree){}
        virtual void VisitReturnStmtTree(ReturnStmtTree * returnStmtTree){}
        virtual void VisitLiteralTree(LiteralTree * lLiteralTree){}
        virtual void VisitParenExprTree(ParenExprTree * parenExprTree){}
        virtual void VisitInitListExprTree(InitListExprTree * initListExprTree){}
        virtual void VisitImplicitCastExprTree(ImplicitCastExprTree * implicitCastExprTree){}
        virtual void VisitArraySubscriptExprTree(ArraySubscriptExprTree * arraySubscriptExprTree){}
        virtual void VisitDeclRefTree(DeclRefTree * declRefTree){}
        virtual void VisitUnaryExprTree(UnaryExprTree * unaryExprTree){}
        virtual void VisitBinaryExprTree(BinaryExprTree * binaryExprTree){}
        virtual void VisitTernaryExprTree(TernaryExprTree * ternaryExprTree){}
        virtual void VisitFuncCallTree(FuncCallTree * funcCallTree){}
        virtual void VisitImplicitValueInitExprTree(ImplicitValueInitExprTree * implicitValueInitExprTree){}
};



class Tree
{
    public:
        std::string kind;
        std::string name;
        std::string value;
        long long lval;
        double dval;
        llvm::json::Object * temp;
        llvm::Value * ir;
        Tree(std::string kind="", std::string name="", std::string value=""): 
            kind(kind), name(name), value(value) {} 
        bool forceRight(ExprTree *& ptr);
        virtual void accept( Visitor* visitor ) {}
        virtual void SematicAnalysis(){};
        virtual std::string getinfo(){}
        virtual ~Tree(){}
};

class StmtTree : public Tree
{
    public:
        
        StmtTree (std::string kind="", std::string name= "", std::string value=""):
            Tree(kind,name,value){}

};

class DeclStmtTree : public StmtTree 
{
    public:
        std::vector<VarDeclTree *> sons;
        DeclStmtTree(std::string kind="", std::string name="", std::string value=""):
            StmtTree(kind,name,value){}
        virtual void accept( Visitor* visitor )
        {
            visitor -> VisitDeclStmtTree(this);
        }
        bool addSon(VarDeclTree* son)
        {   
            bool retval = 0;
            sons.emplace_back(son);
            return retval;
        }
        void SpreadType(Type type);   
        virtual void SematicAnalysis();  
};

class NullStmtTree: public StmtTree 
{
    public:
        NullStmtTree(std::string kind="", std::string name="", std::string value=""):
            StmtTree(kind,name,value){}
        virtual void accept( Visitor* visitor )
        {
            visitor -> VisitNullStmtTree(this);
        }
        virtual void SematicAnalysis(){}
};

class ExprStmtTree: public StmtTree 
{
    public:
        ExprStmtTree(std::string kind="", std::string name="", std::string value=""):
            StmtTree(kind,name,value){}
        virtual void accept( Visitor* visitor )
        {
            visitor -> VisitExprStmtTree(this);
        }
        virtual void SematicAnalysis();
        ExprTree * expr = nullptr;
};

class BlockStmtTree: public StmtTree
{
    public:
        SymTNode symtnode;
        bool processed = 0;
        BlockStmtTree(std::string kind="", std::string name="", std::string value=""):
            StmtTree(kind,name,value){}
        virtual void accept( Visitor* visitor )
        {
            visitor -> VisitBlockStmtTree(this);
        }
        std::vector<Tree *> sons;
        bool addSon(Tree* son)
        {   
            bool retval = 0;
            sons.emplace_back(son);
            return retval;
        }
        virtual void SematicAnalysis();

};

class IfStmtTree: public StmtTree
{
    public:
        IfStmtTree(std::string kind="", std::string name="", std::string value=""):
            StmtTree(kind,name,value){}
        virtual void accept( Visitor* visitor )
        {
            visitor -> VisitIfStmtTree(this);
        }
        ExprTree * cond = nullptr;
        StmtTree * truestmt = nullptr;
        StmtTree * falsestmt = nullptr;
        virtual void SematicAnalysis();

};

class LoopStmtTree: public StmtTree
{
    public:
        llvm::BasicBlock * breakB;
        llvm::BasicBlock * continueB;
        LoopStmtTree(std::string kind="", std::string name="", std::string value=""):
            StmtTree(kind,name,value){}
};

class WhileStmtTree: public LoopStmtTree
{
    public:
        WhileStmtTree(std::string kind="", std::string name="", std::string value=""):
            LoopStmtTree(kind,name,value){}
        virtual void accept( Visitor* visitor )
        {
            visitor -> VisitWhileStmtTree(this);
        }
        ExprTree * cond = nullptr;
        StmtTree * stmt = nullptr;
        virtual void SematicAnalysis();
};

class DoStmtTree: public LoopStmtTree
{
    public:
        DoStmtTree(std::string kind="", std::string name="", std::string value=""):
            LoopStmtTree(kind,name,value){}
        virtual void accept( Visitor* visitor )
        {
            visitor -> VisitDoStmtTree(this);
        }
        StmtTree * stmt = nullptr;
        ExprTree * cond = nullptr;
        virtual void SematicAnalysis();
};

class ForStmtTree: public LoopStmtTree
{
     public:
        ForStmtTree(std::string kind="", std::string name="", std::string value=""):
            LoopStmtTree(kind,name,value){}
        virtual void accept( Visitor* visitor )
        {
            visitor -> VisitForStmtTree(this);
        }    
        ExprTree * init = nullptr;
        ExprTree * null = nullptr;
        ExprTree * cond = nullptr;
        ExprTree * incr = nullptr;
        StmtTree * stmt = nullptr;
        virtual void SematicAnalysis();
};

class JumpStmtTree : public StmtTree
{
    public:
        JumpStmtTree(std::string kind="", std::string name="", std::string value=""):
            StmtTree(kind,name,value){}
};

class ContinueStmtTree: public JumpStmtTree
{
    public:
        ContinueStmtTree(std::string kind="", std::string name="", std::string value=""):
            JumpStmtTree(kind,name,value){}
        virtual void accept( Visitor* visitor )
        {
            visitor -> VisitContinueStmtTree(this);
        }
        LoopStmtTree * loop = nullptr;
        virtual void SematicAnalysis();
};

class BreakStmtTree: public JumpStmtTree
{
    public:
        BreakStmtTree(std::string kind="", std::string name="", std::string value=""):
            JumpStmtTree(kind,name,value){}
        virtual void accept( Visitor* visitor )
        {
            visitor -> VisitBreakStmtTree(this);
        }
        LoopStmtTree * loop = nullptr;
        virtual void SematicAnalysis();
};

class ReturnStmtTree: public JumpStmtTree
{
    public:
        ReturnStmtTree(std::string kind="", std::string name="", std::string value=""):
            JumpStmtTree(kind,name,value){}
        virtual void accept( Visitor* visitor )
        {
            visitor -> VisitReturnStmtTree(this);
        }
        FuncDeclTree * func = nullptr;
        ExprTree * expr = nullptr;
        void RetAdjust();
        virtual void SematicAnalysis();
};

class TranslationUnitTree: public Tree
{
    public:
        std::vector<Tree *> sons;
        SymTNode symtnode;
        TranslationUnitTree (std::string kind="", std::string name="", std::string value=""):
            Tree(kind,name,value){}
        virtual void accept( Visitor* visitor )
        {
            visitor -> VisitTranslationUnitTree(this);
        }
        
        bool addSon(Tree* son)
        {   
            bool retval = 0;
            sons.emplace_back(son);
            return retval;
        }
        
        void mergeSon(Tree* son)
        {   
            if(dynamic_cast<TranslationUnitTree *>(son))
            {
                auto ptr = dynamic_cast<TranslationUnitTree *>(son);
                for (auto ptrson:ptr -> sons)
                    sons.emplace_back(ptrson); 
            }
            
            else
            {
                auto ptr = dynamic_cast<DeclStmtTree *>(son);
                for (auto ptrson:ptr -> sons)
                sons.emplace_back(ptrson); 
            }
        }
        virtual void SematicAnalysis();
};

class DeclTree :public Tree
{
    public:
        Info info;
        DeclTree (std::string kind="", std::string name="", std::string value=""):
            Tree(kind,name,value){}
        virtual std::string getinfo()
        {
            std::string str;
            if(info.isConst)
                str += "const ";
            str += typestr[info.type];
            if(info.isPointer)
                str += '*';
            if(info.layer.num)
            {
                for(int num:info.layer.len)
                {
                    str += "[";
                    str += std::to_string(num);
                    str += "]";
                }
            }
            return str;  
        }
        

};

class VarDeclTree : public DeclTree
{
    public:
        ExprTree * initval = nullptr;
        bool isGlobal = 0;
        std::vector<ExprTree *> * dims = nullptr;
        VarDeclTree (std::string kind="", std::string name="", std::string value=""):
            DeclTree(kind,name,value){}
        void GiveInitialList();
        virtual void accept( Visitor* visitor )
        {
            visitor -> VisitVarDeclTree(this);
        }
        void DeclAdjust();
        virtual void SematicAnalysis();
};

class FuncDeclTree: public DeclTree
{
    public:    
        bool valen = 0;
        std::vector<VarDeclTree *> sons;
        FuncDeclTree * proto = nullptr;
        BlockStmtTree * body = nullptr;
        llvm::Function * funclr;
        FuncDeclTree (std::string kind="", std::string name="", std::string value=""):
            DeclTree(kind,name,value){}
        virtual void accept( Visitor* visitor )
        {
            visitor -> VisitFuncDeclTree(this);
        }
        bool addSon(VarDeclTree* son)
        {   
            bool retval = 0;
            sons.emplace_back(son);
            return retval;
        }
        virtual void SematicAnalysis();
        virtual std::string getinfo()
        {
            std::string str = DeclTree::getinfo();
            if(sons.size())
            {
                
                str += "(";
                for(auto son:sons)
                {
                    if(son -> info.isConst)
                        str += "const ";
                    str += typestr[son -> info.type];
                    if(son -> info.isPointer)
                        str += '*';
                    if(son -> info.layer.num)
                    {
                        for(int num1:son -> info.layer.len)
                        {
                            str += "[";
                            str += std::to_string(num1);
                            str += "]";
                        }
                    }
                    str += ",";
                }
                str[str.length() - 1] = ')';
            }
            else
                str += "()";
            //yyerror(str);
            return str;
        }
};

class ExprTree : public Tree
{
    public:
        Info info;
        bool isLeftVal;
        ExprTree (std::string kind="", std::string name="", std::string value=""):
            Tree(kind,name,value){}
        virtual void * GetValue(){ return nullptr;}
        virtual std::string getinfo()
        {
            std::string str;
            if(info.isConst)
                str += "const ";
            str += typestr[info.type];
            if(info.isPointer)
                str += '*';
            if(info.layer.num)
            {
                
                for(int num:info.layer.len)
                {
                    str += "[";
                    str += std::to_string(num);
                    str += "]";
                }
            }
            return str;         
        }
};

class LiteralTree : public ExprTree
{
    public:
        LiteralType lt;
        LiteralTree(std::string kind="", std::string name="", std::string value=""):
            ExprTree(kind,name,value){}
        virtual void accept( Visitor* visitor )
        {
            visitor -> VisitLiteralTree(this);
        }
        virtual void SematicAnalysis(){}
        virtual void * GetValue()
        {
            switch(info.type)
            {
                case Int:       return new int(lval);
                case Longlong:  return new long long(lval);
                case Double:    return new double(dval);
                default:        return nullptr;
            }
        }
};

class ParenExprTree : public ExprTree
{
    public:
        ExprTree * son = nullptr;
        ParenExprTree(std::string kind="", std::string name="", std::string value=""):
            ExprTree(kind,name,value){}
        virtual void accept( Visitor* visitor )
        {
            visitor -> VisitParenExprTree(this);
        }
        virtual void SematicAnalysis();
        virtual void * GetValue()
        {
            return son -> GetValue();
        }
};

class InitListExprTree :public ExprTree
{
    public:    
        InitListExprTree(std::string kind="", std::string name="", std::string value=""):
            ExprTree(kind,name,value){}
        std::vector<ExprTree *> sons;
        bool ok = 0;
        bool isGlobal = 0;
        virtual void accept( Visitor* visitor )
        {
            visitor -> VisitInitListExprTree(this);
        }
        bool addSon(ExprTree* son)
        {   
            sons.emplace_back(son);
            return 1;
        }
        void spreadToLeaves(Type type)
        {
            /*for(auto & ptr:sons)
            {
                auto nptr = dynamic_cast<InitListExprTree *>(ptr);
                if(nptr)
                    nptr -> spreadToLeaves(type);
                else
                {
                    forceRight(ptr);
                    if(type != ptr -> info.type)
                    {
                        auto tmp = new ImplicitCastExprTree("ImplicitCastExpr");
                        tmp->cast = ptr;
                        tmp->isLeftVal = ptr->isLeftVal;
                        tmp->info.type = type;
                        tmp->castkind = castlookup(ptr->info.type, type);
                        ptr = tmp;
                    }
                }

            }*/
        }
        void processInitList()
        {
            
        }
        virtual void SematicAnalysis();
};

class ImplicitValueInitExprTree : public ExprTree
{
    public:
        ImplicitValueInitExprTree(std::string kind="", std::string name="", std::string value=""):
            ExprTree(kind,name,value){}
        virtual void accept( Visitor* visitor )
        {
            visitor -> VisitImplicitValueInitExprTree(this);
        }
        virtual void SematicAnalysis(){}
};

class ImplicitCastExprTree : public ExprTree 
{
    public:
        ExprTree * cast = nullptr;
        std::string castkind;
        ImplicitCastExprTree(std::string kind="", std::string name="", std::string value=""):
            ExprTree(kind,name,value){}
        virtual void accept( Visitor* visitor )
        {
            visitor -> VisitImplicitCastExprTree(this);
        }
        virtual void SematicAnalysis();
        virtual void * GetValue()
        {

            switch(twoType(cast -> info.type,info.type))
            {
                case 0x12:  return new long long(*(int *)(cast -> GetValue()));  
                case 0x13:  return new float(*(int *)(cast -> GetValue()));
                case 0x14:  return new double(*(int *)(cast -> GetValue()));
                case 0x21:  return new int(*(long long *)(cast -> GetValue()));
                case 0x23:  return new float(*(long long *)(cast -> GetValue()));
                case 0x24:  return new double(*(long long *)(cast -> GetValue()));
                case 0x31:  return new int(*(float *)(cast -> GetValue()));
                case 0x33:  return new long long(*(float *)(cast -> GetValue()));
                case 0x34:  return new double(*(float *)(cast -> GetValue()));
                case 0x41:  return new int(*(double *)(cast -> GetValue()));
                case 0x42:  return new long long(*(double *)(cast -> GetValue()));
                case 0x43:  return new float(*(double *)(cast -> GetValue()));
                default:    return nullptr;
            }
        }
};

class ArraySubscriptExprTree:public ExprTree
{
    public:
        ExprTree * sym = nullptr;
        ExprTree * script = nullptr;
        ArraySubscriptExprTree(std::string kind="", std::string name="", std::string value=""):
                ExprTree(kind,name,value){}
        virtual void accept( Visitor* visitor )
        {
            visitor -> VisitArraySubscriptExprTree(this);
        }
        virtual void SematicAnalysis();
};

class DeclRefTree : public ExprTree
{
    public:
        DeclTree * decl = nullptr;
        DeclRefTree(std::string kind="", std::string name="", std::string value=""):
                ExprTree(kind,name,value){}
        virtual void accept( Visitor* visitor )
        {
            visitor -> VisitDeclRefTree(this);
        }
        virtual void SematicAnalysis();
        virtual void * GetValue()
        {
            VarDeclTree * temp = dynamic_cast<VarDeclTree *>(decl);
            return temp -> initval -> GetValue();
        }
};

class UnaryExprTree : public ExprTree
{
    public:
        OpType opcode;
        bool isPostfix;
        ExprTree * son = nullptr;
        UnaryExprTree(std::string kind="", std::string name="", std::string value=""):
                ExprTree(kind,name,value){}
        virtual void accept( Visitor* visitor )
        {
            visitor -> VisitUnaryExprTree(this);
        }
        virtual void SematicAnalysis();
        virtual void * GetValue()
        {
            switch(opcode)
            {
                case OP_PLUS:       return son -> GetValue();
                case OP_MINUS:
                {
                    switch(info.type)
                    {
                        case Int: 
                        {
                            int * ptr = (int *)(son -> GetValue());
                            *ptr = -(*ptr);
                            return ptr;
                        }
                        case Float:
                        {
                            float * ptr = (float *)(son -> GetValue());
                            *ptr = -(*ptr);
                            return ptr;
                        }
                        case Longlong: 
                        {
                            long long * ptr = (long long *)(son -> GetValue());
                            *ptr = -(*ptr);
                            return ptr;
                        }
                        case Double: 
                        {
                            double * ptr = (double *)(son -> GetValue());
                            *ptr = -(*ptr);
                            return ptr;
                        }
                        default:
                            return nullptr;
                    }
                }
                case OP_EXCLAIM:    
                {
                    switch(info.type)
                    {
                        case Int: 
                        {
                            int * ptr = (int *)(son -> GetValue());
                            *ptr = !(*ptr);
                            return ptr;
                        }
                        case Longlong: 
                        {
                            long long * ptr = (long long *)(son -> GetValue());
                            *ptr = !(*ptr);
                            return ptr;
                        }
                        default:
                            return nullptr;
                    }
                }
                case OP_TILDE:
                {
                    switch(info.type)
                    {
                        case Int: 
                        {
                            int * ptr = (int *)(son -> GetValue());
                            *ptr = ~(*ptr);
                            return ptr;
                        }
                        case Longlong: 
                        {
                            long long * ptr = (long long *)(son -> GetValue());
                            *ptr = ~(*ptr);
                            return ptr;
                        }
                        default:
                            return nullptr;
                    }
                }
                default:
                    return nullptr;
            }
        }
};

class BinaryExprTree : public ExprTree
{
    public:
        OpType opcode;
        ExprTree * left = nullptr;
        ExprTree * right = nullptr;
        BinaryExprTree(std::string kind="", std::string name="", std::string value=""):
                ExprTree(kind,name,value){}
        virtual void accept( Visitor* visitor )
        {
            visitor -> VisitBinaryExprTree(this);
        }
        void TryUpcast();
        void AssignAdjust();
        virtual void SematicAnalysis();
        virtual void * GetValue()
        {
            switch(opcode)
            {
                case OP_PLUS:           
                {
                    switch(info.type)
                    {
                        case Int:       return new int(*(int *)(left -> GetValue()) + *(int *)(right -> GetValue()));
                        case Longlong:  return new long long(*(long long *)(left -> GetValue()) + *(long long *)(right -> GetValue()));
                        case Float:     return new float(*(float *)(left -> GetValue()) + *(float *)(right -> GetValue()));
                        case Double:    return new double(*(double *)(left -> GetValue()) + *(double *)(right -> GetValue()));
                        default:        return nullptr;
                    }
                }
                case OP_MINUS:
                {
                    switch(info.type)
                    {
                        case Int:       return new int(*(int *)(left -> GetValue()) - *(int *)(right -> GetValue()));
                        case Longlong:  return new long long(*(long long *)(left -> GetValue()) - *(long long *)(right -> GetValue()));
                        case Float:     return new float(*(float *)(left -> GetValue()) - *(float *)(right -> GetValue()));
                        case Double:    return new double(*(double *)(left -> GetValue()) - *(double *)(right -> GetValue()));
                        default:        return nullptr;
                    }
                }
                case OP_STAR: 
                {
                    switch(info.type)
                    {
                        case Int:       return new int(*(int *)(left -> GetValue()) * *(int *)(right -> GetValue()));
                        case Longlong:  return new long long(*(long long *)(left -> GetValue()) * *(long long *)(right -> GetValue()));
                        case Float:     return new float(*(float *)(left -> GetValue()) * *(float *)(right -> GetValue()));
                        case Double:    return new double(*(double *)(left -> GetValue()) * *(double *)(right -> GetValue()));
                        default:        return nullptr;
                    }
                }
                case OP_SLASH:
                {
                    switch(info.type)
                    {
                        case Int:       return new int(*(int *)(left -> GetValue()) / *(int *)(right -> GetValue()));
                        case Longlong:  return new long long(*(long long *)(left -> GetValue()) / *(long long *)(right -> GetValue()));
                        case Float:     return new float(*(float *)(left -> GetValue()) / *(float *)(right -> GetValue()));
                        case Double:    return new double(*(double *)(left -> GetValue()) / *(double *)(right -> GetValue()));
                        default:        return nullptr;
                    }
                }
                case OP_PERCENT:
                {
                    switch(info.type)
                    {
                        case Int:       return new int(*(int *)(left -> GetValue()) % *(int *)(right -> GetValue()));
                        case Longlong:  return new long long(*(long long *)(left -> GetValue()) % *(long long *)(right -> GetValue()));
                        default:        return nullptr;
                    }
                }
                case OP_LESSLESS:
                {
                    switch(twoType(left -> info.type,right -> info.type))
                    {
                        case 0x11:      return new int(*(int *)(left -> GetValue()) << *(int *)(right -> GetValue()));
                        case 0x12:      return new int(*(int *)(left -> GetValue()) << *(long long *)(right -> GetValue()));
                        case 0x21:      return new long long(*(long long *)(left -> GetValue()) << *(int *)(right -> GetValue()));
                        case 0x22:      return new long long(*(long long *)(left -> GetValue()) << *(long long *)(right -> GetValue()));
                        default:        return nullptr;
                    }
                }
                case OP_GREATERGREATER:
                {
                    switch(twoType(left -> info.type,right -> info.type))
                    {
                        case 0x11:      return new int(*(int *)(left -> GetValue()) >> *(int *)(right -> GetValue()));
                        case 0x12:      return new int(*(int *)(left -> GetValue()) >> *(long long *)(right -> GetValue()));
                        case 0x21:      return new long long(*(long long *)(left -> GetValue()) >> *(int *)(right -> GetValue()));
                        case 0x22:      return new long long(*(long long *)(left -> GetValue()) >> *(long long *)(right -> GetValue()));
                        default:        return nullptr;
                    }
                }
                case OP_LESS:
                {
                    switch(left -> info.type)
                    {
                        case Int:       return new int(*(int *)(left -> GetValue()) < *(int *)(right -> GetValue()));
                        case Longlong:  return new int(*(long long *)(left -> GetValue()) < *(long long *)(right -> GetValue()));
                        case Float:     return new int(*(float *)(left -> GetValue()) < *(float *)(right -> GetValue()));
                        case Double:    return new int(*(double *)(left -> GetValue()) < *(double *)(right -> GetValue()));
                        default:        return nullptr;
                    }
                }
                case OP_GREATER:
                {
                    switch(left -> info.type)
                    {
                        case Int:       return new int(*(int *)(left -> GetValue()) > *(int *)(right -> GetValue()));
                        case Longlong:  return new int(*(long long *)(left -> GetValue()) > *(long long *)(right -> GetValue()));
                        case Float:     return new int(*(float *)(left -> GetValue()) > *(float *)(right -> GetValue()));
                        case Double:    return new int(*(double *)(left -> GetValue()) > *(double *)(right -> GetValue()));
                        default:        return nullptr;
                    }
                }
                case OP_LESSEQUAL:
                {
                    switch(left -> info.type)
                    {
                        case Int:       return new int(*(int *)(left -> GetValue()) <= *(int *)(right -> GetValue()));
                        case Longlong:  return new int(*(long long *)(left -> GetValue()) <= *(long long *)(right -> GetValue()));
                        case Float:     return new int(*(float *)(left -> GetValue()) <= *(float *)(right -> GetValue()));
                        case Double:    return new int(*(double *)(left -> GetValue()) <= *(double *)(right -> GetValue()));
                        default:        return nullptr;
                    }
                }
                case OP_GREATEREQUAL:
                {
                    switch(left -> info.type)
                    {
                        case Int:       return new int(*(int *)(left -> GetValue()) >= *(int *)(right -> GetValue()));
                        case Longlong:  return new int(*(long long *)(left -> GetValue()) >= *(long long *)(right -> GetValue()));
                        case Float:     return new int(*(float *)(left -> GetValue()) >= *(float *)(right -> GetValue()));
                        case Double:    return new int(*(double *)(left -> GetValue()) >= *(double *)(right -> GetValue()));
                        default:        return nullptr;
                    }
                }
                case OP_EQUALEQUAL:
                {
                    switch(left -> info.type)
                    {
                        case Int:       return new int(*(int *)(left -> GetValue()) == *(int *)(right -> GetValue()));
                        case Longlong:  return new int(*(long long *)(left -> GetValue()) == *(long long *)(right -> GetValue()));
                        case Float:     return new int(*(float *)(left -> GetValue()) == *(float *)(right -> GetValue()));
                        case Double:    return new int(*(double *)(left -> GetValue()) == *(double *)(right -> GetValue()));
                        default:        return nullptr;
                    }
                }
                case OP_EXCLAIMEQUAL:
                {
                    switch(left -> info.type)
                    {
                        case Int:       return new int(*(int *)(left -> GetValue()) != *(int *)(right -> GetValue()));
                        case Longlong:  return new int(*(long long *)(left -> GetValue()) != *(long long *)(right -> GetValue()));
                        case Float:     return new int(*(float *)(left -> GetValue()) != *(float *)(right -> GetValue()));
                        case Double:    return new int(*(double *)(left -> GetValue()) != *(double *)(right -> GetValue()));
                        default:        return nullptr;
                    }
                }
                case OP_AMP:
                {
                    switch(info.type)
                    {
                        case Int:       return new int(*(int *)(left -> GetValue()) & *(int *)(right -> GetValue()));
                        case Longlong:  return new long long(*(long long *)(left -> GetValue()) & *(long long *)(right -> GetValue()));
                        default:        return nullptr;
                    }
                }
                case OP_CARET:
                {
                    switch(info.type)
                    {
                        case Int:       return new int(*(int *)(left -> GetValue()) ^ *(int *)(right -> GetValue()));
                        case Longlong:  return new long long(*(long long *)(left -> GetValue()) ^ *(long long *)(right -> GetValue()));
                        default:        return nullptr;
                    }
                }
                case OP_PIPE:
                {
                    switch(info.type)
                    {
                        case Int:       return new int(*(int *)(left -> GetValue()) | *(int *)(right -> GetValue()));
                        case Longlong:  return new long long(*(long long *)(left -> GetValue()) | *(long long *)(right -> GetValue()));
                        default:        return nullptr;
                    }
                }
                case OP_AMPAMP:
                {
                    switch(twoType(left -> info.type,right -> info.type))
                    {
                        case 0x11:      return new int(*(int *)(left -> GetValue()) && *(int *)(right -> GetValue()));
                        case 0x12:      return new int(*(int *)(left -> GetValue()) && *(long long *)(right -> GetValue()));
                        case 0x13:      return new int(*(int *)(left -> GetValue()) && *(float *)(right -> GetValue()));
                        case 0x14:      return new int(*(int *)(left -> GetValue()) && *(double *)(right -> GetValue()));
                        case 0x21:      return new int(*(long long *)(left -> GetValue()) && *(int *)(right -> GetValue()));
                        case 0x22:      return new int(*(long long *)(left -> GetValue()) && *(long long *)(right -> GetValue()));
                        case 0x23:      return new int(*(long long *)(left -> GetValue()) && *(float *)(right -> GetValue()));
                        case 0x24:      return new int(*(long long *)(left -> GetValue()) && *(double *)(right -> GetValue()));
                        case 0x31:      return new int(*(float *)(left -> GetValue()) && *(int *)(right -> GetValue()));
                        case 0x32:      return new int(*(float *)(left -> GetValue()) && *(long long *)(right -> GetValue()));
                        case 0x33:      return new int(*(float *)(left -> GetValue()) && *(float *)(right -> GetValue()));
                        case 0x34:      return new int(*(float *)(left -> GetValue()) && *(double *)(right -> GetValue()));
                        case 0x41:      return new int(*(double *)(left -> GetValue()) && *(int *)(right -> GetValue()));
                        case 0x42:      return new int(*(double *)(left -> GetValue()) && *(long long *)(right -> GetValue()));
                        case 0x43:      return new int(*(double *)(left -> GetValue()) && *(float *)(right -> GetValue()));
                        case 0x44:      return new int(*(double *)(left -> GetValue()) && *(double *)(right -> GetValue()));
                        default:        return nullptr;
                    }
                }
                case OP_PIPEPIPE:
                {
                    switch(twoType(left -> info.type,right -> info.type))
                    {
                        case 0x11:      return new int(*(int *)(left -> GetValue()) || *(int *)(right -> GetValue()));
                        case 0x12:      return new int(*(int *)(left -> GetValue()) || *(long long *)(right -> GetValue()));
                        case 0x13:      return new int(*(int *)(left -> GetValue()) || *(float *)(right -> GetValue()));
                        case 0x14:      return new int(*(int *)(left -> GetValue()) || *(double *)(right -> GetValue()));
                        case 0x21:      return new int(*(long long *)(left -> GetValue()) || *(int *)(right -> GetValue()));
                        case 0x22:      return new int(*(long long *)(left -> GetValue()) || *(long long *)(right -> GetValue()));
                        case 0x23:      return new int(*(long long *)(left -> GetValue()) || *(float *)(right -> GetValue()));
                        case 0x24:      return new int(*(long long *)(left -> GetValue()) || *(double *)(right -> GetValue()));
                        case 0x31:      return new int(*(float *)(left -> GetValue()) || *(int *)(right -> GetValue()));
                        case 0x32:      return new int(*(float *)(left -> GetValue()) || *(long long *)(right -> GetValue()));
                        case 0x33:      return new int(*(float *)(left -> GetValue()) || *(float *)(right -> GetValue()));
                        case 0x34:      return new int(*(float *)(left -> GetValue()) || *(double *)(right -> GetValue()));
                        case 0x41:      return new int(*(double *)(left -> GetValue()) || *(int *)(right -> GetValue()));
                        case 0x42:      return new int(*(double *)(left -> GetValue()) || *(long long *)(right -> GetValue()));
                        case 0x43:      return new int(*(double *)(left -> GetValue()) || *(float *)(right -> GetValue()));
                        case 0x44:      return new int(*(double *)(left -> GetValue()) || *(double *)(right -> GetValue()));
                        default:        return nullptr;
                    }
                }
                case OP_COMMA:
                    return right -> GetValue();
                default:
                    return nullptr;
            }
            
        }
};

class TernaryExprTree : public ExprTree
{
    public:
        ExprTree * condexp = nullptr;
        ExprTree * trueexp = nullptr;
        ExprTree * falseexp = nullptr;
        TernaryExprTree(std::string kind="", std::string name="", std::string value=""):
                ExprTree(kind,name,value){}
        virtual void accept( Visitor* visitor )
        {
            visitor -> VisitTernaryExprTree(this);
        }
};

class FuncCallTree : public ExprTree
{
    public:
        ExprTree * iden = nullptr;
        std::vector<ExprTree *> para;
        FuncCallTree(std::string kind="", std::string name="", std::string value=""):
                ExprTree(kind,name,value){}
        virtual void accept( Visitor* visitor )
        {
            visitor -> VisitFuncCallTree(this);
        }
        bool addSon(ExprTree* son)
        {   
            bool retval = 0;
            para.emplace_back(son);
            return retval;
        }
        virtual void SematicAnalysis();
        void AdjustParaType();
};
bool Tree::forceRight(ExprTree *& ptr)
{
    if(ptr -> isLeftVal)
    {
        auto tmp = new ImplicitCastExprTree("ImplicitCastExpr");
        tmp -> castkind = "LValueToRValue";
        tmp -> info = ptr -> info;
        tmp -> isLeftVal = 0;
        tmp -> cast = ptr;
        ptr = tmp;
        return 1;
    }
    return 0;
}
void DeclStmtTree::SpreadType(Type type)
{
    for(auto son:sons)
        son -> info.type = type;
}

void InitListExprTree::SematicAnalysis()
{
    /*if(!sons.size())
    {
        sons.emplace_back(new ImplicitCastExprTree("array_filler: ImplicitValueInitExpr"));
        sons[0]->info = info;
        sons[0]->info.layer.pop_back();
    }
    else
    {
        spreadToLeaves(info.type);
        processInitList();
    }*/
       

}

void ExprStmtTree::SematicAnalysis()
{
    expr -> SematicAnalysis();
}

void DeclStmtTree::SematicAnalysis()
{
    for(auto son:sons)
        son -> SematicAnalysis();
}  

void BlockStmtTree::SematicAnalysis()
{
    if(!processed)
    {
        cursym -> sons.push_back(&symtnode);
        symtnode.father = cursym;
        cursym = &symtnode;
        processed = 1;
    }

    for(auto son:sons)
        son -> SematicAnalysis();

    cursym = symtnode.father;
}

void IfStmtTree::SematicAnalysis()
{
    cond -> SematicAnalysis();
    forceRight(cond);
    truestmt -> SematicAnalysis();
    if(falsestmt)
        falsestmt -> SematicAnalysis();
}

void DoStmtTree::SematicAnalysis()
{
    curloops.push(this);
    stmt -> SematicAnalysis();
    cond -> SematicAnalysis();
    forceRight(cond);
    curloops.pop();
}

void WhileStmtTree::SematicAnalysis()
{
    curloops.push(this);
    cond -> SematicAnalysis();
    forceRight(cond);
    stmt -> SematicAnalysis();
    curloops.pop();
}

void ForStmtTree::SematicAnalysis()
{
    curloops.push(this);
    init -> SematicAnalysis();
    cond -> SematicAnalysis();
    forceRight(cond);
    incr -> SematicAnalysis();
    stmt -> SematicAnalysis();
    curloops.pop();
}

void ContinueStmtTree::SematicAnalysis()
{
    loop = curloops.top();
}

void BreakStmtTree::SematicAnalysis()
{
    loop = curloops.top();
}

void ReturnStmtTree::RetAdjust()
{
    if(expr -> info.type != curfunc -> info.type)
    {
        auto tmp = new ImplicitCastExprTree("ImplicitCastExpr");
        tmp -> cast = expr;
        tmp -> isLeftVal = expr -> isLeftVal;
        tmp -> info.type = curfunc -> info.type;
        tmp -> info.layer = curfunc -> info.layer;
        tmp -> castkind = castlookup(expr -> info.type,curfunc -> info.type);
        expr = tmp;
    }
}
void ReturnStmtTree::SematicAnalysis()
{
    func = curfunc;
    if(expr)
    {
        expr -> SematicAnalysis();
        forceRight(expr);
        RetAdjust();
    }
}

void ParenExprTree::SematicAnalysis()
{
    son->SematicAnalysis();
    info = son->info;
    isLeftVal = son->isLeftVal;
}

void TranslationUnitTree::SematicAnalysis()
{
    cursym = &symtnode;
    symtnode.father = nullptr;
    for (auto son:sons)
        son -> SematicAnalysis();
}

void VarDeclTree::DeclAdjust()
{
    if (info.type != initval->info.type)
    {
        auto tmp = new ImplicitCastExprTree("ImplicitCastExpr");
        tmp->cast = initval;
        tmp->isLeftVal = initval->isLeftVal;
        tmp->info = info;
        tmp->castkind = (castlookup(initval->info.type, info.type));
        initval = tmp;
    }
}

void VarDeclTree::SematicAnalysis()
{
    if(dims)
    {
        for(auto ptr:(*dims))
        {
            ptr -> SematicAnalysis();
            if(ptr -> info.type == Int)
                info.layer.emplace_back(*(int *)(ptr -> GetValue()));
            else
                info.layer.emplace_back(*(long long *)(ptr -> GetValue()));

        }
        if(initval)
        {
            //GiveInitialList();
            //initval->SematicAnalysis();
        }
    }
    else if (initval)
    {
        initval->SematicAnalysis();
        forceRight(initval);
        DeclAdjust();
    }
    cursym->symt.insert(std::make_pair(name, this));
}

void VarDeclTree::GiveInitialList()
{
    auto ptr = dynamic_cast<InitListExprTree *>(initval);
    ptr -> info = info;
}

void FuncDeclTree::SematicAnalysis()
{
    cursym->symt.insert(std::make_pair(name, this));
    if (body)
    {
        curfunc = this;
        cursym->sons.push_back(&(body->symtnode));
        body->symtnode.father = cursym;
        cursym = &(body->symtnode);
        body->processed = 1;
        for (auto son : sons)
            son->SematicAnalysis();
        body->SematicAnalysis();
    }
}

void ImplicitCastExprTree::SematicAnalysis()
{
    cast->SematicAnalysis();
    info = cast->info;
}

void ArraySubscriptExprTree::SematicAnalysis()
{
    sym->SematicAnalysis();
    script->SematicAnalysis();
    forceRight(script);
    info = sym->info;
    info.layer.pop_back();
}

void DeclRefTree::SematicAnalysis()
{
    SymTNode *temp = cursym;
    while (cursym)
    {
        if (temp->symt.find(name) != temp->symt.end())
        {
            decl = temp->symt[name];
            break;
        }
        temp = temp->father;
    }
    info = decl->info;
    isLeftVal = 1;
}

void UnaryExprTree::SematicAnalysis()
{
    son->SematicAnalysis();
    if (opcode >= OP_PLUS)
        forceRight(son);
    info = son->info;
    isLeftVal = son->isLeftVal;
}

void BinaryExprTree::TryUpcast()
{
    if (left->info.type == right->info.type)
    {
        info = left->info;
    }
    else if (left->info.type > right->info.type)
    {
        auto tmp = new ImplicitCastExprTree("ImplicitCastExpr");
        tmp->cast = right;
        tmp->isLeftVal = right->isLeftVal;
        tmp->info = left->info;
        tmp->castkind = castlookup(right->info.type, left->info.type);
        right = tmp;
        info = left->info;
    }
    else
    {
        auto tmp = new ImplicitCastExprTree("ImplicitCastExpr");
        tmp->cast = left;
        tmp->isLeftVal = left->isLeftVal;
        tmp->info = right->info;
        tmp->castkind = castlookup(left->info.type, right->info.type);
        left = tmp;
        info = right->info;
    }
}
void BinaryExprTree::AssignAdjust()
{
    if (left->info.type != right->info.type)
    {
        auto tmp = new ImplicitCastExprTree("ImplicitCastExpr");
        tmp->cast = right;
        tmp->isLeftVal = right->isLeftVal;
        tmp->info = left->info;
        tmp->castkind = castlookup(right->info.type, left->info.type);
        right = tmp;
        
    }
    info = left->info;
}
void BinaryExprTree::SematicAnalysis()
{
    left->SematicAnalysis();
    right->SematicAnalysis();
    if (opcode <= OP_PIPE)
    {
        forceRight(left);
        forceRight(right);
        if(opcode != OP_LESSLESS && opcode != OP_GREATERGREATER)
            TryUpcast();
        else
            info.type = left -> info.type;
        if(opcode <= OP_EXCLAIMEQUAL && opcode >= OP_LESS)
            info.type = Int;
        isLeftVal = 0;
    }
    else if (opcode <= OP_PIPEPIPE)
    {
        forceRight(left);
        forceRight(right);
        info.type = Int;
        isLeftVal = 0;
    }
    else if (opcode == OP_COMMA)
    {
        info = right->info;
        isLeftVal = right->isLeftVal;
    }
    else
    {
        forceRight(right);
        AssignAdjust();
        isLeftVal = left->isLeftVal;
    }
}

void FuncCallTree::SematicAnalysis()
{
    iden->SematicAnalysis();
    info.type = iden->info.type;
    isLeftVal = 0;
    for (auto son : para)
    {
        son->SematicAnalysis();
    }
    AdjustParaType();
}
void FuncCallTree::AdjustParaType()
{
    auto funcdeclptr = dynamic_cast<FuncDeclTree * > ( dynamic_cast<DeclRefTree *>(dynamic_cast<ImplicitCastExprTree *>(iden)-> cast) -> decl);
    int size = funcdeclptr -> sons.size();
    for (int i = 0; i < size; i++)
    {

        if (funcdeclptr -> sons[i] -> info.layer.num > 0)
        {
            auto tmp = new ImplicitCastExprTree("ImplicitCastExpr");
            tmp->castkind = "FunctionToPointerDecay";
            tmp->cast = para[i];
            tmp->isLeftVal = para[i]->isLeftVal;
            tmp->info = para[i]->info;
            para[i] = tmp;

            if (funcdeclptr -> sons[i] -> info.type != para[i]->info.type)
            {
                    auto tmp = new ImplicitCastExprTree("ImplicitCastExpr");
                    tmp->castkind = castlookup(para[i]->info.type, funcdeclptr -> sons[i] -> info.type);
                    tmp->cast = para[i];
                    tmp->isLeftVal = para[i]->isLeftVal;
                    tmp->info = funcdeclptr -> sons[i] -> info;
                    para[i] = tmp;
            }

            else if (funcdeclptr -> sons[i] -> info.isConst & !(para[i]->info.isConst))
            {
                    auto tmp = new ImplicitCastExprTree("ImplicitCastExpr");
                    tmp->castkind = "NoOp";
                    tmp->cast = para[i];
                    tmp->isLeftVal = para[i]->isLeftVal;
                    tmp->info = funcdeclptr -> sons[i] -> info;
                    para[i] = tmp;
            }
        }
        else 
        {
            forceRight(para[i]);
            if (funcdeclptr -> sons[i] -> info.type != para[i]->info.type)
            {
                auto tmp = new ImplicitCastExprTree("ImplicitCastExpr");
                tmp->castkind = castlookup(para[i]->info.type,funcdeclptr -> sons[i] -> info.type);
                tmp->cast = para[i];
                tmp->isLeftVal = para[i]->isLeftVal;
                tmp->info = funcdeclptr -> sons[i] -> info;
                para[i] = tmp;
            }
        }
        
    }
}

class Print: public Visitor
{
    private:
        void printbasic(Tree * tree)
        {
            yyerror("|");
            for(int i = 0;i < depth; ++i) 
                yyerror(" ");
            yyerror("-" + tree -> kind + " " + tree -> name + " " + tree -> value + " ");
        }
    int depth = 0;
    public:
        virtual void VisitTranslationUnitTree(TranslationUnitTree *  translationUnitTree)
        {
            depth ++;
            printbasic(translationUnitTree);
            yyerror("\n");
            for(auto & it :translationUnitTree -> sons)
                it -> accept(this);
            depth --;
        }
        virtual void VisitVarDeclTree(VarDeclTree * varDeclTree)
        {
            depth ++;
            printbasic(varDeclTree);
            yyerror(varDeclTree -> getinfo());
            yyerror("\n");
            if(varDeclTree -> initval)
                varDeclTree -> initval -> accept(this);
            depth --;
        }
        virtual void VisitFuncDeclTree(FuncDeclTree * funcDeclTree)
        {
            depth ++;
            printbasic(funcDeclTree);
            yyerror(funcDeclTree -> getinfo());
            yyerror("\n");
            for(auto & it :funcDeclTree -> sons)
                it -> accept(this);
            if(funcDeclTree -> body)
                funcDeclTree -> body -> accept(this);
            depth --;
        }
        virtual void VisitDeclStmtTree(DeclStmtTree * declStmtTree)
        {
            depth ++;
            printbasic(declStmtTree);
            yyerror("\n");
            for(auto & it :declStmtTree -> sons)
                it -> accept(this);
            depth --;
        }
        virtual void VisitNullStmtTree(NullStmtTree * nullStmtTree)
        {
            depth ++;
            printbasic(nullStmtTree);
            yyerror("\n");
            depth --;
        }
        virtual void VisitExprStmtTree(ExprStmtTree * exprStmtTree)
        {
            exprStmtTree -> expr -> accept(this);
        }
        virtual void VisitBlockStmtTree(BlockStmtTree * blockStmtTree)
        {
            depth ++;
            printbasic(blockStmtTree);
            yyerror("\n");
            for(auto & it :blockStmtTree -> sons)
                it -> accept(this);
            depth --;
        }
        virtual void VisitIfStmtTree(IfStmtTree * ifStmtTree)
        {
            depth ++;
            printbasic(ifStmtTree);
            yyerror("\n");
            ifStmtTree -> cond -> accept(this);
            ifStmtTree -> truestmt -> accept(this);
            if(ifStmtTree -> falsestmt)
                ifStmtTree -> falsestmt -> accept(this);
            depth --;
        }
        virtual void VisitWhileStmtTree(WhileStmtTree * whileStmtTree)
        {
            depth ++;
            printbasic(whileStmtTree);
            yyerror("\n");
            whileStmtTree -> cond -> accept(this);
            whileStmtTree -> stmt -> accept(this);
            depth --;
        }
        virtual void VisitDoStmtTree(DoStmtTree * doStmtTree)
        {
            depth ++;
            printbasic(doStmtTree);
            yyerror("\n");
            doStmtTree -> stmt -> accept(this);
            doStmtTree -> cond -> accept(this);
            depth --;
        }
        virtual void VisitForStmtTree(ForStmtTree * forStmtTree)
        {
            depth ++;
            printbasic(forStmtTree);
            yyerror("\n");
            forStmtTree -> init -> accept(this);
            yyerror('\n');
            forStmtTree -> cond -> accept(this);
            forStmtTree -> incr -> accept(this);
            forStmtTree -> stmt -> accept(this);
            depth --;
        }
        virtual void VisitContinueStmtTree(ContinueStmtTree * continueStmtTree)
        {
            depth ++;
            printbasic(continueStmtTree);
            yyerror("\n");
            depth --;
        }
        virtual void VisitBreakStmtTree(BreakStmtTree * breakStmtTree)
        {
            depth ++;
            printbasic(breakStmtTree);
            yyerror("\n");
            depth --;            
        }
        virtual void VisitReturnStmtTree(ReturnStmtTree * returnStmtTree)
        {
            depth ++;
            printbasic(returnStmtTree);
            yyerror("\n");
            if(returnStmtTree -> expr)
                returnStmtTree -> expr -> accept(this);
            depth --;
        }
        virtual void VisitLiteralTree(LiteralTree * literalTree)
        {
            depth ++;
            printbasic(literalTree);
            yyerror(literalTree -> getinfo());
            yyerror("\n");
            depth --;  
        }
        virtual void VisitParenExprTree(ParenExprTree * parenExprTree)
        {
            depth ++;
            printbasic(parenExprTree);
            yyerror(parenExprTree -> getinfo());
            yyerror("\n");
            parenExprTree -> son -> accept(this);
            depth --;
        }
        virtual void VisitInitListExprTree(InitListExprTree * initListExprTree)
        {
            depth ++;
            printbasic(initListExprTree);
            yyerror(initListExprTree -> getinfo());
            yyerror("\n");
            for(auto & it :initListExprTree -> sons)
                it -> accept(this);
            depth --;  
        }
        virtual void VisitImplicitCastExprTree(ImplicitCastExprTree * implicitCastExprTree)
        {
            depth ++;
            printbasic(implicitCastExprTree);
            yyerror(implicitCastExprTree -> getinfo());
            yyerror(" ");
            yyerror(implicitCastExprTree -> castkind);
            yyerror("\n");
            implicitCastExprTree -> cast -> accept(this);
            depth --;
        }
        virtual void VisitArraySubscriptExprTree(ArraySubscriptExprTree * arraySubscriptExprTree)
        {
            depth ++;
            printbasic(arraySubscriptExprTree);
            yyerror(arraySubscriptExprTree -> getinfo());
            yyerror("\n");
            arraySubscriptExprTree -> sym -> accept(this);
            arraySubscriptExprTree -> script -> accept(this);
            depth --;
        }
        virtual void VisitDeclRefTree(DeclRefTree * declRefTree)
        {
            depth ++;
            printbasic(declRefTree);
            yyerror(declRefTree -> getinfo());
            yyerror("\n");
            depth --; 
        }
        virtual void VisitUnaryExprTree(UnaryExprTree * unaryExprTree)
        {
            depth ++;
            printbasic(unaryExprTree);
            yyerror(unaryExprTree -> getinfo());
            yyerror("\n");
            unaryExprTree -> son -> accept(this);
            depth --;
        }
        virtual void VisitBinaryExprTree(BinaryExprTree * binaryExprTree)
        {   
            depth ++;
            printbasic(binaryExprTree);
            yyerror(binaryExprTree -> getinfo());
            yyerror("\n");
            binaryExprTree -> left -> accept(this);
            binaryExprTree -> right -> accept(this);
            depth --;
        }
        virtual void VisitTernaryExprTree(TernaryExprTree * ternaryExprTree)
        {
            depth ++;
            printbasic(ternaryExprTree);
            yyerror(ternaryExprTree -> getinfo());
            yyerror("\n");
            ternaryExprTree -> condexp -> accept(this);
            ternaryExprTree -> trueexp -> accept(this);
            ternaryExprTree -> falseexp -> accept(this);
            depth --;
        }
        virtual void VisitFuncCallTree(FuncCallTree * funcCallTree)
        {
            depth ++;
            printbasic(funcCallTree);
            yyerror(funcCallTree -> getinfo());
            yyerror("\n");
            funcCallTree -> iden -> accept(this);
            for(auto & it :funcCallTree -> para)
                it -> accept(this);
            depth --;
        }
        virtual void VisitImplicitValueInitExprTree(ImplicitValueInitExprTree * implicitValueInitExprTree)
        {
            depth ++;
            printbasic(implicitValueInitExprTree);
            yyerror(implicitValueInitExprTree -> getinfo());
            yyerror("\n");
            depth --;
        }
};



llvm::json::Value tov(llvm::json::Object o){return o;}


class ToJson:public Visitor
{
    public:
        virtual void VisitTranslationUnitTree(TranslationUnitTree *  translationUnitTree)
        {
           yyerror("a"); 
            translationUnitTree -> temp = new llvm::json::Object
            {
                {"kind", translationUnitTree -> kind},
                {"name", translationUnitTree -> name},
                {"value", translationUnitTree -> value},
                {"inner", llvm::json::Array{}}
            };
            for(auto& it: translationUnitTree -> sons) 
            {
                it -> accept(this);
                translationUnitTree -> temp ->get("inner")->getAsArray()->push_back(tov(*(it->temp)));
            }
                
        }
        virtual void VisitVarDeclTree(VarDeclTree * varDeclTree)
        {
            yyerror("b");
            varDeclTree -> temp = new llvm::json::Object
            {
                {"kind", varDeclTree -> kind},
                {"name", varDeclTree -> name},
                {"value", varDeclTree -> value},
                {"inner", llvm::json::Array{}}
            };
             if(varDeclTree -> initval)
            {
                varDeclTree -> initval -> accept(this);
                varDeclTree -> temp  -> get("inner")->getAsArray()->push_back(tov(*(varDeclTree -> initval -> temp)));
            }
        }
        virtual void VisitFuncDeclTree(FuncDeclTree * funcDeclTree)
        {
            yyerror("c");
            funcDeclTree -> temp = new llvm::json::Object
            {
                {"kind", funcDeclTree -> kind},
                {"name", funcDeclTree -> name},
                {"value", funcDeclTree -> value},
                {"inner", llvm::json::Array{}}
            };
            for(auto & it :funcDeclTree -> sons)
            {   
                it -> accept(this);
                funcDeclTree -> temp ->get("inner")->getAsArray()->push_back(tov(*(it->temp)));
            }
            
            if(funcDeclTree -> body)
            {   
                funcDeclTree -> body -> accept(this);
                funcDeclTree -> temp ->get("inner")->getAsArray()->push_back(tov(*(funcDeclTree -> body -> temp)));
            }
            
        }
        virtual void VisitDeclStmtTree(DeclStmtTree * declStmtTree)
        {
            yyerror("d");
            declStmtTree -> temp = new llvm::json::Object
            {
                {"kind", declStmtTree -> kind},
                {"name", declStmtTree -> name},
                {"value", declStmtTree -> value},
                {"inner", llvm::json::Array{}}
            };
            for(auto & it :declStmtTree -> sons)
            {   
                it -> accept(this);
                declStmtTree -> temp ->get("inner")->getAsArray()->push_back(tov(*(it->temp)));
            }
        }
        virtual void VisitNullStmtTree(NullStmtTree * nullStmtTree)
        {
            yyerror("e");
            nullStmtTree -> temp = new llvm::json::Object
            {
                {"kind", nullStmtTree -> kind},
                {"name", nullStmtTree -> name},
                {"value", nullStmtTree -> value},
                {"inner", llvm::json::Array{}}
            };
        }
        virtual void VisitExprStmtTree(ExprStmtTree * exprStmtTree)
        {
            yyerror("f");
            exprStmtTree -> expr -> accept(this);
            exprStmtTree -> temp = exprStmtTree -> expr -> temp;
        }
        virtual void VisitBlockStmtTree(BlockStmtTree * blockStmtTree)
        {
            yyerror("g");
            blockStmtTree -> temp = new llvm::json::Object
            {
                {"kind", blockStmtTree -> kind},
                {"name", blockStmtTree -> name},
                {"value", blockStmtTree -> value},
                {"inner", llvm::json::Array{}}
            };
            for(auto & it :blockStmtTree -> sons)
            {   
                it -> accept(this);
                blockStmtTree -> temp ->get("inner")->getAsArray()->push_back(tov(*(it->temp)));
            }
           
        }
        virtual void VisitIfStmtTree(IfStmtTree * ifStmtTree)
        {
            yyerror("h");
            ifStmtTree -> temp = new llvm::json::Object
            {
                {"kind", ifStmtTree -> kind},
                {"name", ifStmtTree -> name},
                {"value", ifStmtTree -> value},
                {"inner", llvm::json::Array{}}
            };
            ifStmtTree -> cond -> accept(this);
            ifStmtTree -> temp ->get("inner")->getAsArray()->push_back(tov(*(ifStmtTree -> cond->temp)));
            ifStmtTree -> truestmt -> accept(this);
            ifStmtTree -> temp ->get("inner")->getAsArray()->push_back(tov(*(ifStmtTree -> truestmt->temp)));
            if(ifStmtTree -> falsestmt)
            {   
                ifStmtTree -> falsestmt -> accept(this);
                ifStmtTree -> temp ->get("inner")->getAsArray()->push_back(tov(*(ifStmtTree -> falsestmt->temp)));
            }
            
        }
        virtual void VisitWhileStmtTree(WhileStmtTree * whileStmtTree)
        {
            yyerror("i");
            whileStmtTree -> temp = new llvm::json::Object
            {
                {"kind", whileStmtTree -> kind},
                {"name", whileStmtTree -> name},
                {"value", whileStmtTree -> value},
                {"inner", llvm::json::Array{}}
            };
            whileStmtTree -> cond -> accept(this);
            whileStmtTree -> temp ->get("inner")->getAsArray()->push_back(tov(*(whileStmtTree -> cond->temp)));
            whileStmtTree -> stmt -> accept(this);
            whileStmtTree -> temp ->get("inner")->getAsArray()->push_back(tov(*(whileStmtTree -> stmt->temp)));
        }
        virtual void VisitDoStmtTree(DoStmtTree * doStmtTree)
        {
            yyerror("j");
            doStmtTree -> temp = new llvm::json::Object
            {
                {"kind", doStmtTree -> kind},
                {"name", doStmtTree -> name},
                {"value", doStmtTree -> value},
                {"inner", llvm::json::Array{}}
            };
            
            doStmtTree -> stmt -> accept(this);
            
            doStmtTree -> temp ->get("inner")->getAsArray()->push_back(tov(*(doStmtTree -> stmt->temp)));
            
            doStmtTree -> cond -> accept(this);
            doStmtTree -> temp ->get("inner")->getAsArray()->push_back(tov(*(doStmtTree -> cond->temp)));
            
        }
        virtual void VisitForStmtTree(ForStmtTree * forStmtTree)
        {
            yyerror("k");
            forStmtTree -> temp = new llvm::json::Object
            {
                {"kind", forStmtTree -> kind},
                {"name", forStmtTree -> name},
                {"value", forStmtTree -> value},
                {"inner", llvm::json::Array{}}
            };
            forStmtTree -> init -> accept(this);
            forStmtTree -> temp ->get("inner")->getAsArray()->push_back(tov(*(forStmtTree -> init -> temp)));
            forStmtTree -> temp ->get("inner")->getAsArray()->push_back(llvm::json::Object{{}});
            forStmtTree -> cond -> accept(this);
            forStmtTree -> temp ->get("inner")->getAsArray()->push_back(tov(*(forStmtTree -> cond -> temp)));
            forStmtTree -> incr -> accept(this);
            forStmtTree -> temp ->get("inner")->getAsArray()->push_back(tov(*(forStmtTree -> incr -> temp)));
            forStmtTree -> stmt -> accept(this);
            forStmtTree -> temp ->get("inner")->getAsArray()->push_back(tov(*(forStmtTree -> stmt -> temp)));
        }
        virtual void VisitContinueStmtTree(ContinueStmtTree * continueStmtTree)
        {
            yyerror("l");
            continueStmtTree -> temp = new llvm::json::Object
            {
                {"kind", continueStmtTree -> kind},
                {"name", continueStmtTree -> name},
                {"value", continueStmtTree -> value},
                {"inner", llvm::json::Array{}}
            };
        }
        virtual void VisitBreakStmtTree(BreakStmtTree * breakStmtTree)
        {
            yyerror("m");
            breakStmtTree -> temp = new llvm::json::Object
            {
                {"kind", breakStmtTree -> kind},
                {"name", breakStmtTree -> name},
                {"value", breakStmtTree -> value},
                {"inner", llvm::json::Array{}}
            };          
        }
        virtual void VisitReturnStmtTree(ReturnStmtTree * returnStmtTree)
        {
            yyerror("n");
            returnStmtTree -> temp = new llvm::json::Object
            {
                {"kind", returnStmtTree -> kind},
                {"name", returnStmtTree -> name},
                {"value", returnStmtTree -> value},
                {"inner", llvm::json::Array{}}
            };   
            if(returnStmtTree -> expr)
            {
                returnStmtTree -> expr -> accept(this);
                //
                returnStmtTree -> temp ->  get("inner")-> getAsArray()->push_back(tov(*(returnStmtTree -> expr -> temp)));
            
            }
        }
        virtual void VisitLiteralTree(LiteralTree * literalTree)
        {
            yyerror("o");
            literalTree -> temp = new llvm::json::Object
            {
                {"kind", literalTree -> kind},
                {"name", literalTree -> name},
                {"value", literalTree -> value},
                {"inner", llvm::json::Array{}}
            };  
            
        }
        virtual void VisitParenExprTree(ParenExprTree * parenExprTree)
        {
            yyerror("p");
            parenExprTree -> temp = new llvm::json::Object
            {
                {"kind", parenExprTree -> kind},
                {"name", parenExprTree -> name},
                {"value", parenExprTree -> value},
                {"inner", llvm::json::Array{}}
            }; 
            parenExprTree -> son -> accept(this);
            parenExprTree -> temp ->get("inner")->getAsArray()->push_back(tov(*(parenExprTree -> son -> temp)));
        }
        virtual void VisitInitListExprTree(InitListExprTree * initListExprTree)
        {
            yyerror("q");
            initListExprTree -> temp = new llvm::json::Object
            {
                {"kind", initListExprTree -> kind},
                {"name", initListExprTree -> name},
                {"value", initListExprTree -> value},
                {"inner", llvm::json::Array{}}
            };  
            
        }
        virtual void VisitImplicitCastExprTree(ImplicitCastExprTree * implicitCastExprTree)
        {
            yyerror("r");
            implicitCastExprTree -> temp = new llvm::json::Object
            {
                {"kind", implicitCastExprTree -> kind},
                {"name", implicitCastExprTree -> name},
                {"value", implicitCastExprTree -> value},
                {"inner", llvm::json::Array{}}
            };  
            implicitCastExprTree -> cast -> accept(this);
            implicitCastExprTree -> temp ->get("inner")->getAsArray()->push_back(tov(*(implicitCastExprTree -> cast -> temp)));
        }
        virtual void VisitArraySubscriptExprTree(ArraySubscriptExprTree * arraySubscriptExprTree)
        {
            yyerror("s");
            arraySubscriptExprTree -> temp = new llvm::json::Object
            {
                {"kind", arraySubscriptExprTree -> kind},
                {"name", arraySubscriptExprTree -> name},
                {"value", arraySubscriptExprTree -> value},
                {"inner", llvm::json::Array{}}
            };  
            arraySubscriptExprTree -> sym -> accept(this);
            arraySubscriptExprTree -> temp ->get("inner")->getAsArray()->push_back(tov(*( arraySubscriptExprTree -> sym -> temp)));
            arraySubscriptExprTree -> script -> accept(this);
            arraySubscriptExprTree -> temp ->get("inner")->getAsArray()->push_back(tov(*(arraySubscriptExprTree -> script -> temp)));
        }
        virtual void VisitDeclRefTree(DeclRefTree * declRefTree)
        {
            yyerror("t");
            declRefTree -> temp = new llvm::json::Object
            {
                {"kind", declRefTree -> kind},
                {"name", declRefTree -> name},
                {"value", declRefTree -> value},
                {"inner", llvm::json::Array{}}
            }; 
        }
        virtual void VisitUnaryExprTree(UnaryExprTree * unaryExprTree)
        {
            yyerror("u");
            unaryExprTree -> temp = new llvm::json::Object
            {
                {"kind", unaryExprTree -> kind},
                {"name", unaryExprTree -> name},
                {"value", unaryExprTree -> value},
                {"inner", llvm::json::Array{}}
            }; 
            unaryExprTree -> son -> accept(this);
            unaryExprTree -> temp ->get("inner")->getAsArray()->push_back(tov(*(unaryExprTree -> son -> temp)));
        }
        virtual void VisitBinaryExprTree(BinaryExprTree * binaryExprTree)
        {   
            yyerror("v");
            binaryExprTree -> temp = new llvm::json::Object
            {
                {"kind", binaryExprTree -> kind},
                {"name", binaryExprTree -> name},
                {"value", binaryExprTree -> value},
                {"inner", llvm::json::Array{}}
            }; 
            binaryExprTree -> left -> accept(this);
            binaryExprTree -> temp ->get("inner")->getAsArray()->push_back(tov(*(binaryExprTree -> left -> temp)));
            binaryExprTree -> right -> accept(this);
            binaryExprTree -> temp ->get("inner")->getAsArray()->push_back(tov(*(binaryExprTree -> right -> temp)));
    
        }
        virtual void VisitTernaryExprTree(TernaryExprTree * ternaryExprTree)
        {
            yyerror("w");
            ternaryExprTree -> temp = new llvm::json::Object
            {
                {"kind", ternaryExprTree -> kind},
                {"name", ternaryExprTree -> name},
                {"value", ternaryExprTree -> value},
                {"inner", llvm::json::Array{}}
            }; 
            ternaryExprTree -> condexp -> accept(this);
            ternaryExprTree -> temp ->get("inner")->getAsArray()->push_back(tov(*(ternaryExprTree -> condexp -> temp)));
            ternaryExprTree -> trueexp -> accept(this);
            ternaryExprTree -> temp ->get("inner")->getAsArray()->push_back(tov(*(ternaryExprTree -> trueexp -> temp)));
            ternaryExprTree -> falseexp -> accept(this);
            ternaryExprTree -> temp ->get("inner")->getAsArray()->push_back(tov(*(ternaryExprTree -> falseexp -> temp)));
        }
        virtual void VisitFuncCallTree(FuncCallTree * funcCallTree)
        {
            yyerror("x");
            funcCallTree -> temp = new llvm::json::Object
            {
                {"kind", funcCallTree -> kind},
                {"name", funcCallTree -> name},
                {"value", funcCallTree -> value},
                {"inner", llvm::json::Array{}}
            }; 
            funcCallTree -> iden -> accept(this);
            funcCallTree -> temp ->get("inner")->getAsArray()->push_back(tov(*(funcCallTree -> iden -> temp)));
            for(auto & it :funcCallTree -> para)
            {   
                it -> accept(this);
                funcCallTree -> temp ->get("inner")->getAsArray()->push_back(tov(*(it->temp)));
            }
        }
        virtual void VisitImplicitValueInitExprTree(ImplicitValueInitExprTree * implicitValueInitExprTree)
        {
            yyerror("y");
            implicitValueInitExprTree -> temp = new llvm::json::Object
            {
                {"kind", implicitValueInitExprTree -> kind},
                {"name", implicitValueInitExprTree -> name},
                {"value", implicitValueInitExprTree -> value},
                {"inner", llvm::json::Array{}}
            };
        }
};

std::unordered_map<long long,DeclTree *> bigsymt;
char * nnn;

Info strToType(std::string str)
{
    size_t index = str.find('(');
    if(index != std::string::npos && str[index + 1] !='*')
        str = str.substr(0,index);
    
    Info info;
    if(str[0] == 'c' && str[1] =='o')
    {
        info.isConst = 1;
        str = str.substr(6);
    }

    index = str.rfind('[');
    while(index != std::string::npos)
    {
        info.layer.emplace_back(strtoll(str.c_str() + index + 1,&nnn,10));
        index = str.rfind('[',index - 1);
    }

    if(str.find('*') != std::string::npos)
        info.isPointer = 1;
    
    switch(str[0])
    {
        case 'i':   info.type = Int;        break;
        case 'c':   info.type = Char;       break;
        case 'l':
        case 'u':   info.type = Longlong;   break;
        case 'f':   info.type = Float;      break;
        case 'd':   info.type = Double;     break;
        case 'v':   info.type = Void;       break;
    }

    return info;
}

bool get_name(const llvm::json::Object *O,Tree * ptr)
{
    auto namep = O -> get("name");
    bool ret = (namep != nullptr);
    if(ret)
    {
        ptr -> name = namep -> getAsString() -> str();
    }
    return ret;
}

bool reg_id(const llvm::json::Object *O,DeclTree * ptr)
{
    auto idp = O -> get("id");
    bool ret = (idp != nullptr);
    if(ret)
    {
        bigsymt.insert(std::make_pair(strtoll((idp -> getAsString() -> str().c_str()) + 2,&nnn,16),ptr));
    }
    return ret;
}

bool get_type(const llvm::json::Object *O,ExprTree * ptr)
{
    auto typep = O -> get("type");
    bool ret = (typep != nullptr);
    if(ret)
    {
        if(auto qualtypep = typep -> getAsObject() -> get("qualType"))
        {
            ptr -> info = strToType(qualtypep -> getAsString() -> str());
        }  
        else 
            ret = 0;   
    }
    return ret;
}

bool get_type(const llvm::json::Object *O,DeclTree * ptr)
{
    auto typep = O -> get("type");
    bool ret = (typep != nullptr);
    if(ret)
    {
        if(auto qualtypep = typep -> getAsObject() -> get("qualType"))
        {
            ptr -> info = strToType(qualtypep -> getAsString() -> str());
        }  
        else 
            ret = 0;   
    }
    return ret;
}

bool get_vc(const llvm::json::Object *O,ExprTree * ptr)
{
    auto vcp = O->get("valueCategory");
    bool ret = (vcp != nullptr);
    if(ret)
    {
        ptr -> isLeftVal = (vcp -> getAsString() -> str() == "lvalue");
    }
    return ret;
}


StmtTree * covertStmt(Tree * tree)
{
    StmtTree * ptr = dynamic_cast<StmtTree *>(tree);
    if(ptr)
        return ptr;
    
    ExprStmtTree * ptr2 = new ExprStmtTree("ExprStmt");
    ptr2 -> expr = dynamic_cast<ExprTree *>(tree);
    return ptr2;
}


Tree * CreateFromJson(const llvm::json::Object *O)
{
    
    if (O == nullptr)
        return nullptr;
    auto kind = O->get("kind")->getAsString();
    if(*kind == "TranslationUnitDecl")
    {
        yyerror("a");
        auto ptr = new TranslationUnitTree("TranslationUnitDecl");
        if(auto inner = O->getArray("inner"))
            for (const auto &it : *inner)
                if (auto P = it.getAsObject())
                {
                    if(auto temp = CreateFromJson(P))
                    {
                        if(temp -> kind == "VarDecl")
                        {
                            dynamic_cast<VarDeclTree *>(temp) -> isGlobal = 1;
                        }
                        
                        ptr -> sons.emplace_back(temp);
                    }
                
                }
        yyerror("A");
        return ptr;
    }
    else if(*kind == "FunctionDecl")
    {
        yyerror("b");
        auto ptr = new FuncDeclTree("FunctionDecl");
        curfunc = ptr;
        
        get_name(O,ptr);
        reg_id(O,ptr);
        get_type(O,ptr);

        if(auto predecl = O -> get("previousDecl"))
        {
            ptr -> proto = dynamic_cast<FuncDeclTree *>(bigsymt[strtoll((predecl -> getAsString() -> str().c_str()) + 2,&nnn,16)]);
        }

        if(auto inner = O->getArray("inner"))
            for (const auto &it : *inner)
                if (auto P = it.getAsObject())
                {
                    auto son = CreateFromJson(P);
                    if(son)
                    {
                        if(son -> kind == "ParmVarDecl")
                            ptr -> sons.emplace_back(dynamic_cast<VarDeclTree *>(son));
                        else if(son -> kind == "CompoundStmt")
                            ptr -> body = dynamic_cast<BlockStmtTree *>(son);
                    }
                    
                }
        yyerror("B");
        return ptr;
    }
    else if(*kind == "ParmVarDecl" || *kind == "VarDecl")
    {
        //array
        yyerror("c");
        auto ptr = new VarDeclTree(kind ->str());
        get_name(O,ptr);
        reg_id(O,ptr);
        get_type(O,ptr);
        
        if(auto inner = O->getArray("inner"))
            for (const auto &it : *inner)
                if (auto P = it.getAsObject())
                {
                    ptr -> initval = dynamic_cast<ExprTree *> (CreateFromJson(P));
                }
        yyerror("C");
        return ptr;
    }
    else if(*kind == "CallExpr")
    {
        yyerror("d");
        auto ptr = new FuncCallTree("CallExpr");
        get_type(O,ptr);

        get_vc(O,ptr);

        if(auto inner = O->getArray("inner"))
        {
            int index = 0;
            for (const auto &it : *inner)
                if (auto P = it.getAsObject())
                {
                    auto son = CreateFromJson(P);
                    if(index != 0)
                        ptr -> para.emplace_back(dynamic_cast<ExprTree *>(son));
                    else 
                        ptr -> iden = dynamic_cast<ExprTree *>(son);
                    index++;
                }
        }
        yyerror("D");    
        return ptr;
    }
    else if(*kind == "DeclRefExpr")
    {
        yyerror("e");
        auto ptr = new DeclRefTree("DeclRefExpr");
        get_type(O,ptr);

        get_vc(O,ptr);

        if(auto rdp = O->get("referencedDecl"))
        {
            if(auto rip = rdp -> getAsObject() -> get("id"))
            {
                ptr -> decl = bigsymt[strtoll((rip -> getAsString() -> str().c_str()) + 2,&nnn,16)];
            }     
        }
        yyerror("E");
        return ptr;
    }
    else if(*kind == "ImplicitCastExpr")
    {
        yyerror("f");
        auto ptr = new ImplicitCastExprTree("ImplicitCastExpr");
        get_type(O,ptr);

        get_vc(O,ptr);

        if(auto ckp = O->get("castKind"))
        {
            ptr -> castkind = ckp -> getAsString() -> str();
        }

        if(auto inner = O->getArray("inner"))
            for (const auto &it : *inner)
                if (auto P = it.getAsObject())
                {
                    ptr -> cast = dynamic_cast<ExprTree *> (CreateFromJson(P));
                }
        yyerror("F");
        return ptr;

    }
    else if(*kind == "CompoundStmt")
    {
        yyerror("g");
        auto ptr = new BlockStmtTree("CompoundStmt");
        if(auto inner = O->getArray("inner"))
            for (const auto &it : *inner)
                if (auto P = it.getAsObject())
                {
                    ptr -> sons.emplace_back(CreateFromJson(P));
                }
        yyerror("G");
        return ptr;

    }
    else if(*kind == "DeclStmt")
    {
        yyerror("h");
        auto ptr = new DeclStmtTree("DeclStmt");
        if(auto inner = O->getArray("inner"))
            for (const auto &it : *inner)
                if (auto P = it.getAsObject())
                {
                    ptr -> sons.emplace_back(dynamic_cast<VarDeclTree *>(CreateFromJson(P)));
                }
        yyerror("H");
        return ptr;
    }
    else if(*kind == "InitListExpr")
    {
        //Not checked yet
        yyerror("i");
        auto ptr = new InitListExprTree("InitListExpr");
        get_type(O,ptr);
        get_vc(O,ptr);
        if(auto inner = O->getArray("inner"))
        {   
            for (const auto &it : *inner)
                if (auto P = it.getAsObject())
                {
                    ptr -> sons.emplace_back(dynamic_cast<ExprTree *>(CreateFromJson(P)));
                }
        }
        else if(auto inner = O->getArray("array_filler"))
        {   
            for (const auto &it : *inner)
                if (auto P = it.getAsObject())
                {
                    ptr -> sons.emplace_back(dynamic_cast<ExprTree *>(CreateFromJson(P)));
                }
        }
        yyerror("I");
        return ptr;
    }
    else if(*kind == "ImplicitValueInitExpr")
    {
        yyerror("j");
        auto ptr = new ImplicitValueInitExprTree("ImplicitValueInitExpr");
        get_type(O,ptr);
        get_vc(O,ptr);
        yyerror("J");
        return ptr;
    }
    else if(*kind == "ArraySubscriptExpr")
    {
        yyerror("k");
        auto ptr = new ArraySubscriptExprTree("ArraySubscriptExpr");
        get_type(O,ptr);
        get_vc(O,ptr);
        if(auto inner = O->getArray("inner"))
        {
            int index = 0;
            for (const auto &it : *inner)
                if (auto P = it.getAsObject())
                {
                    if(index == 0)
                        ptr -> sym = dynamic_cast<ExprTree *>(CreateFromJson(P));
                    else if(index == 1)
                        ptr -> script = dynamic_cast<ExprTree *>(CreateFromJson(P));
                    index++;
                }
        }
        yyerror("K");    
        return ptr;
    }
    else if(*kind == "IntegerLiteral")
    {
        yyerror("l");
        auto ptr = new LiteralTree("IntegerLiteral");
        ptr -> lt = Interger;
        get_type(O,ptr);
        get_vc(O,ptr);
        if(auto vp = O->get("value"))
        {
            ptr -> lval = strtoll(vp -> getAsString() -> str().c_str(),&nnn,10);
        }
        yyerror("L");
        return ptr;
    }
    else if(*kind == "FloatingLiteral")
    {
        yyerror("m");
        auto ptr = new LiteralTree("FloatingLiteral");
        ptr -> lt = Floating;
        get_type(O,ptr);
        get_vc(O,ptr);
        if(auto vp = O->get("value"))
        {
            ptr -> dval = strtod(vp -> getAsString() -> str().c_str(),&nnn);
        }
        yyerror("M");
        return ptr;
    } 
    else if(*kind == "StringLiteral")
    {
        yyerror("n");
        auto ptr = new LiteralTree("StringLiteral");
        ptr -> lt = String;
        get_type(O,ptr);
        get_vc(O,ptr);
        if(auto vp = O->get("value"))
        {
            ptr -> value = vp -> getAsString() -> str();
        }
        yyerror("N");
        return ptr;
    } 
    else if(*kind == "ReturnStmt")
    {
        yyerror("o");
        auto ptr = new ReturnStmtTree("ReturnStmt");
        if(auto inner = O->getArray("inner"))
            for (const auto &it : *inner)
                if (auto P = it.getAsObject())
                {
                    ptr -> expr = dynamic_cast<ExprTree *>(CreateFromJson(P));
                }
        ptr -> func = curfunc;
        yyerror("O");
        return ptr;
    }
    else if(*kind == "NullStmt")
    {
        yyerror("p");
        yyerror("P");
        return new NullStmtTree("NullStmt");
    }
    else if(*kind == "IfStmt")
    {
        yyerror("q");
        auto ptr = new IfStmtTree("IfStmt");
        if(auto inner = O->getArray("inner"))
        {   
            int index = 0;
            for (const auto &it : *inner)
                if (auto P = it.getAsObject())
                {
                    if(index == 0)
                        ptr -> cond = dynamic_cast<ExprTree *>(CreateFromJson(P));
                    else if(index == 1)
                        ptr -> truestmt = covertStmt(CreateFromJson(P));
                    else
                        ptr -> falsestmt = covertStmt(CreateFromJson(P));
                    index++;
                }
        }
        yyerror("Q");
        return ptr;
    }
    else if(*kind == "WhileStmt")
    {
        yyerror("r");
        auto ptr = new WhileStmtTree("WhileStmt");
        curloops.push(ptr);

        if(auto inner = O->getArray("inner"))
        {   
            int index = 0;
            for (const auto &it : *inner)
                if (auto P = it.getAsObject())
                {
                    if(index == 0)
                        ptr -> cond = dynamic_cast<ExprTree *>(CreateFromJson(P));
                    else if(index == 1)
                        ptr -> stmt = covertStmt(CreateFromJson(P));
                    
                    index++;
                }
        }

        curloops.pop();
        yyerror("R");
        return ptr;
    }
    else if(*kind == "DoStmt")
    {
        yyerror("s");
        auto ptr = new DoStmtTree("DoStmt");
        curloops.push(ptr);

        if(auto inner = O->getArray("inner"))
        {   
            int index = 0;
            for (const auto &it : *inner)
                if (auto P = it.getAsObject())
                {
                    if(index == 1)
                        ptr -> cond = dynamic_cast<ExprTree *>(CreateFromJson(P));
                    else if(index == 0)
                        ptr -> stmt = covertStmt(CreateFromJson(P));
                    
                    index++;
                }
        }

        curloops.pop();
        yyerror("S");
        return ptr;
    }
    else if(*kind == "ForStmt")
    {
        yyerror("t");
        auto ptr = new ForStmtTree("ForStmt");
        curloops.push(ptr);

        if(auto inner = O->getArray("inner"))
        {   
            int index = 0;
            for (const auto &it : *inner)
                if (auto P = it.getAsObject())
                {
                    if(index == 0)
                        ptr -> init = dynamic_cast<ExprTree *>(CreateFromJson(P));
                    else if(index == 2)
                        ptr -> cond = dynamic_cast<ExprTree *>(CreateFromJson(P));
                    else if(index == 3)
                        ptr -> incr= dynamic_cast<ExprTree *>(CreateFromJson(P));
                    else if(index == 4)
                        ptr -> stmt = covertStmt(CreateFromJson(P));
                    
                    index++;
                }
        }

        curloops.pop();
        yyerror("T");
        return ptr;
    }
    else if(*kind == "BreakStmt")
    {
        yyerror("u");
        auto ptr = new BreakStmtTree("BreakStmt");
        ptr -> loop = curloops.top();
        yyerror("U");
        return ptr;
    }
    else if(*kind == "ContinueStmt")
    {
        yyerror("v");
        auto ptr = new ContinueStmtTree("ContinueStmt");
        ptr -> loop = curloops.top();
        yyerror("V");
        return ptr;
    }
    else if(*kind == "UnaryOperator")
    {
        yyerror("w");
        auto ptr = new UnaryExprTree("UnaryOperator");
        get_type(O,ptr);

        get_vc(O,ptr);

        if(auto ipf = O->get("isPostfix"))
        {
            ptr -> isPostfix = *(ipf -> getAsBoolean());
        }

        if(auto op = O->get("opcode"))
        {
            
            std::string str = op -> getAsString() -> str() ;
            if(str == "++")
                ptr -> opcode = OP_PLUSPLUS;
            else if(str == "--")
                ptr -> opcode = OP_MINUSMINUS;
            else if(str == "+")
                ptr -> opcode = OP_PLUS;
            else if(str == "-")
                ptr -> opcode = OP_MINUS;
            else if(str == "!")
                ptr -> opcode = OP_EXCLAIM;
            else if(str == "~")
                ptr -> opcode = OP_TILDE;
        }
        if(auto inner = O->getArray("inner"))
            for (const auto &it : *inner)
                if (auto P = it.getAsObject())
                {
                    ptr -> son = dynamic_cast<ExprTree *> (CreateFromJson(P));
                }
        yyerror("W");
        return ptr;

    }
    else if(*kind == "ParenExpr")
    {
        yyerror("w");
        auto ptr = new ParenExprTree("ParenExpr");
        get_type(O,ptr);

        get_vc(O,ptr);


        if(auto inner = O->getArray("inner"))
            for (const auto &it : *inner)
                if (auto P = it.getAsObject())
                {
                    ptr -> son = dynamic_cast<ExprTree *> (CreateFromJson(P));
                }
        yyerror("W");
        return ptr;

    }
    else if(*kind == "BinaryOperator")
    {
        yyerror("x");
        auto ptr = new BinaryExprTree("BinaryOperator");
        get_type(O,ptr);

        get_vc(O,ptr);
        if(auto op = O->get("opcode"))
        {
            
            std::string str = op -> getAsString() -> str() ;
            if(str == "*")
                ptr -> opcode = OP_STAR;
            else if(str == "/")
                ptr -> opcode = OP_SLASH;
            else if(str == "%")
                ptr -> opcode = OP_PERCENT;
            else if(str == "+")
                ptr -> opcode = OP_PLUS;
            else if(str == "-")
                ptr -> opcode = OP_MINUS;
            else if(str == "<<")
                ptr -> opcode = OP_LESSLESS;
            else if(str == ">>")
                ptr -> opcode = OP_GREATERGREATER;
            else if(str == "<")
                ptr -> opcode = OP_LESS;
            else if(str == ">")
                ptr -> opcode = OP_GREATER;
            else if(str == "<=")
                ptr -> opcode = OP_LESSEQUAL;
            else if(str == ">=")
                ptr -> opcode = OP_GREATEREQUAL;
            else if(str == "==")
                ptr -> opcode = OP_EQUALEQUAL;
            else if(str == "!=")
                ptr -> opcode = OP_EXCLAIMEQUAL;
            else if(str == "&")
                ptr -> opcode = OP_AMP;
            else if(str == "^")
                ptr -> opcode = OP_CARET;
            else if(str == "|")
                ptr -> opcode = OP_PIPE;
            else if(str == "&&")
                ptr -> opcode = OP_AMPAMP;
            else if(str == "||")
                ptr -> opcode = OP_PIPEPIPE;
            else if(str == ",")
                ptr -> opcode = OP_COMMA;
            else if(str == "=")
                ptr -> opcode = OP_EQUAL;
        }
        if(auto inner = O->getArray("inner"))
        {
            int index = 0;
            for (const auto &it : *inner)
                if (auto P = it.getAsObject())
                {
                    if(index == 0)
                        ptr -> left = dynamic_cast<ExprTree *> (CreateFromJson(P));
                    else if(index == 1)
                        ptr -> right = dynamic_cast<ExprTree *> (CreateFromJson(P));
                    index++;
                }
        }
        yyerror("X");    
        return ptr;

    }
    else if(*kind == "CompoundAssignOperator")
    {
        yyerror("y");
        auto ptr = new BinaryExprTree("CompoundAssignOperator");
        get_type(O,ptr);

        get_vc(O,ptr);
        if(auto op = O->get("opcode"))
        {
            std::string str = op -> getAsString() -> str() ;
            if(str == "*=")
                ptr -> opcode = OP_STAREQUAL;
            else if(str == "/=")
                ptr -> opcode = OP_SLASHEQUAL;
            else if(str == "%=")
                ptr -> opcode = OP_PERCENTEQUAL;
            else if(str == "+=")
                ptr -> opcode = OP_PLUSEQUAL;
            else if(str == "-=")
                ptr -> opcode = OP_MINUSEQUAL;
            else if(str == "<<=")
                ptr -> opcode = OP_LESSLESSEQUAL;
            else if(str == ">>=")
                ptr -> opcode = OP_GREATERGREATEREQUAL;
            else if(str == "&=")
                ptr -> opcode = OP_AMPEQUAL;
            else if(str == "^=")
                ptr -> opcode = OP_CARETEQUAL;
            else if(str == "|=")
                ptr -> opcode = OP_PIPEEQUAL;
        }
        

        if(auto inner = O->getArray("inner"))
        {
            int index = 0;
            for (const auto &it : *inner)
                if (auto P = it.getAsObject())
                {
                    if(index == 0)
                        ptr -> left = dynamic_cast<ExprTree *> (CreateFromJson(P));
                    else if(index == 1)
                        ptr -> right = dynamic_cast<ExprTree *> (CreateFromJson(P));
                    index++;
                }
        }
        yyerror("Y"); 
        return ptr;
    }
    else
    {
        yyerror("z");
        return nullptr;
    }

}

llvm::LLVMContext TheContext;
llvm::Module TheModule("-", TheContext);
llvm::IRBuilder<> Builder(TheContext);

llvm::Type * BasicType(Type type)
{
    switch(type)
    {
        case Int:       return llvm::Type::getInt32Ty(TheContext);
        case Longlong:  return llvm::Type::getInt64Ty(TheContext);
        case Char:      return llvm::Type::getInt8Ty(TheContext);
        case Float:
        case Double:    return llvm::Type::getFloatTy(TheContext);
        case Void:      return llvm::Type::getVoidTy(TheContext);
    }
}


llvm::Type * toLLVMType(Info & info)
{
    llvm::Type * type = BasicType(info.type);

    for(int i =  0; i <info.layer.len.size();i++)
    {
        type = llvm::ArrayType::get(type,info.layer.len[i]);
    }
    
    if(info.isPointer)
    {
        return type->getPointerTo();
    }
    else
        return type;
}

static llvm::AllocaInst *CreateEntryBlockAlloca(llvm::Function *TheFunction,llvm::Type * Ty,llvm::Value * ArraySize ,llvm::StringRef VarName) 
{
    llvm::IRBuilder<> TmpB(&TheFunction->getEntryBlock(),TheFunction->getEntryBlock().begin());
    return TmpB.CreateAlloca(Ty, ArraySize, VarName);
}

void getZeroConstInitList(std::vector<llvm::Constant *> & elements,int start,int fill,Type type)
{
    switch(type)
    {
        case Int:
        {
            for(int j = 0;j < fill;j++)
            {
                elements[start + j] = llvm::ConstantInt::get(TheContext, llvm::APInt(32,0,1));
            }
            break;  
        }      
        case Longlong:
        {
            for(int j = 0;j < fill;j++)
            {
                elements[start + j] = llvm::ConstantInt::get(TheContext, llvm::APInt(64,0,1));
            }
            break;
        }           
        case Float:
        case Double:
        {
            for(int j = 0;j < fill;j++)
            {
                elements[start + j] = llvm::ConstantFP::get(TheContext, llvm::APFloat(0.0));
            }
            break;
        }    
    }
}

void fillConstInitList(std::vector<llvm::Constant *> & elements,InitListExprTree * initlist,std::vector<size_t> & steps,int st_index,std::vector<size_t> & sizes,int sz_index,int start)
{
    int step = steps[st_index];
    int size = sizes[sz_index];
    int i,fill,j;
    int sonnum = initlist -> sons.size();
    
    bool last = (step == 1);

    if(dynamic_cast<ImplicitValueInitExprTree *>(initlist -> sons[0]))
    {
        fill = step * (size - sonnum + 1);
        i = 1;
    }
    else
    {
        fill = 0;
        i = 0;
    }
    
    for(j = 0;i < sonnum;i++,j++)
    {
        if(last)
        {
            void * p =initlist -> sons[i] -> GetValue();
            switch(initlist -> sons[i] -> info.type)
            {
                case Int:  
                    elements[start + j] = llvm::ConstantInt::get(TheContext,llvm::APInt(32,*(int *)p,1));
                    break;
                case Longlong:
                    elements[start + j] = llvm::ConstantInt::get(TheContext,llvm::APInt(64,*(long long *)p,1));
                    break;
                case Float:
                    elements[start + j] = llvm::ConstantFP::get(TheContext,llvm::APFloat(*(float *)p));
                    break;
                case Double:
                    elements[start + j] = llvm::ConstantFP::get(TheContext,llvm::APFloat(*(double *)p));
                    break;
            }
        }
        else
        {
            fillConstInitList(elements,dynamic_cast<InitListExprTree *>(initlist -> sons[i]),steps,st_index - 1,sizes,sz_index - 1,start + step * j);
        }
    }
    getZeroConstInitList(elements,start + j * step,fill,initlist -> info.type);
}

void useLocalInitList(llvm::Value * arr,InitListExprTree * initlist,std::vector<size_t> & sizes,int sz_index,bool zeroFill,Type type = Void)
{
    int size = sizes[sz_index];
    int i,fill,j;
    int sonnum;
    
    bool last = (sz_index == 0);

    if(zeroFill)
    {
        sonnum = 0;
        i = 0;
    }
    else if(dynamic_cast<ImplicitValueInitExprTree *>(initlist -> sons[0]))
    {
        sonnum = initlist -> sons.size();
        i = 1;
    }
    else
    {
        sonnum = initlist -> sons.size();
        i = 0;
    }

    for(j = 0;i < sonnum;i++,j++)
    {
        llvm::Value * initpos = Builder.CreateInBoundsGEP(arr, {llvm::ConstantInt::get(TheContext, llvm::APInt(32, 0)), llvm::ConstantInt::get(TheContext, llvm::APInt(32, j))});
        if(last)
        { 
            Builder.CreateStore(initlist -> sons[i] -> ir,initpos);
        }
        else
        {
            useLocalInitList(initpos,dynamic_cast<InitListExprTree *>(initlist -> sons[i]),sizes,sz_index - 1,0,type);
        }
    }

    for(;j < size;j++)
    {
        llvm::Value * initpos = Builder.CreateInBoundsGEP(arr, {llvm::ConstantInt::get(TheContext, llvm::APInt(32, 0)), llvm::ConstantInt::get(TheContext, llvm::APInt(32, j))});
        
        if(last)
        { 
            yyerror(type);
            switch(type)
            {
                case Int:       Builder.CreateStore(llvm::ConstantInt::get(TheContext, llvm::APInt(32,0,1)),initpos);break;
                case Longlong:  Builder.CreateStore(llvm::ConstantInt::get(TheContext, llvm::APInt(64,0,1)),initpos);break;
                case Float:       
                case Double:    Builder.CreateStore(llvm::ConstantFP::get(TheContext, llvm::APFloat(0.0)),initpos);break;
            }            
        }
        else
        {
            useLocalInitList(initpos,nullptr,sizes,sz_index - 1,1,type);
        }
    }
}

class ToLLVM:public Visitor
{
    public:
        virtual void VisitTranslationUnitTree(TranslationUnitTree *  translationUnitTree)
        {
            
            for(auto ptr:translationUnitTree -> sons)
                ptr -> accept(this);                
        }
        virtual void VisitVarDeclTree(VarDeclTree * varDeclTree)
        {
            if(varDeclTree -> info.layer.len.size())
            {
                if(varDeclTree -> isGlobal)
                {
                    llvm::Constant *initval;
                    if (varDeclTree->initval)
                    {
                        InitListExprTree *initlist = dynamic_cast<InitListExprTree *>(varDeclTree->initval);
                        initlist->isGlobal = 1;
                        initlist->accept(this);
                        initval = llvm::dyn_cast<llvm::Constant>(initlist->ir);
                    }
                    else
                    {
                        size_t tot = 1;
                        for (int i : varDeclTree->info.layer.len)
                            tot *= i;
                        std::vector<llvm::Constant *> elements(tot);

                        getZeroConstInitList(elements, 0, tot, varDeclTree->info.type);
                        initval = llvm::ConstantArray::get(llvm::ArrayType::get(BasicType(varDeclTree->info.type), tot), elements);
                    }

                    varDeclTree->ir = new llvm::GlobalVariable(TheModule, toLLVMType(varDeclTree->info), varDeclTree->info.isConst, llvm::GlobalValue::ExternalLinkage, initval, varDeclTree->name);
                }
                else
                {
                    llvm::Function *TheFunction = Builder.GetInsertBlock()->getParent();
                    llvm::AllocaInst *Alloca = CreateEntryBlockAlloca(TheFunction,toLLVMType(varDeclTree -> info),nullptr, varDeclTree -> name);; 
                    if(varDeclTree -> initval)
                    {
                        InitListExprTree * initListExprTree = dynamic_cast<InitListExprTree *>(varDeclTree -> initval);
                        initListExprTree -> accept(this);
                        useLocalInitList(Alloca,initListExprTree,initListExprTree->info.layer.len,initListExprTree->info.layer.len.size() - 1,0,varDeclTree -> info.type);           
                    }
                    varDeclTree -> ir = Alloca;
                }
            }
            else 
            {
                if(varDeclTree -> isGlobal)
                {
                    llvm::Constant * initValue = nullptr;
                    if(varDeclTree -> initval)
                    {
                        varDeclTree -> initval -> accept(this);
                        initValue = llvm::dyn_cast<llvm::Constant>((varDeclTree->initval->ir));
                    }
                    if(!initValue)
                    {
                        switch(varDeclTree -> info.type)
                        {
                            case Int:
                                initValue = llvm::ConstantInt::get(TheContext, llvm::APInt(32,0,1));
                                break;
                            case Longlong:
                                initValue = llvm::ConstantInt::get(TheContext, llvm::APInt(64,0,1));
                                break;
                            case Float:
                            case Double:
                                initValue = llvm::ConstantFP::get(TheContext, llvm::APFloat(0.0));
                                break;
                        }
                    }  
                    varDeclTree -> ir = new llvm::GlobalVariable(TheModule, toLLVMType(varDeclTree -> info), varDeclTree -> info.isConst, llvm::GlobalValue::ExternalLinkage, initValue, varDeclTree -> name);
                }
                else
                {
                    llvm::Function *TheFunction = Builder.GetInsertBlock()->getParent();
                    llvm::AllocaInst *Alloca = CreateEntryBlockAlloca(TheFunction,toLLVMType(varDeclTree -> info),nullptr, varDeclTree -> name);; 
                    if(varDeclTree -> initval)
                    {
                        varDeclTree -> initval -> accept(this);
                        Builder.CreateStore(varDeclTree -> initval -> ir, Alloca);
                    }
                    varDeclTree -> ir = Alloca;
                }
            }
                
            
        }
        virtual void VisitFuncDeclTree(FuncDeclTree * funcDeclTree)
        {
            llvm::Function * TheF;
            if(!(funcDeclTree -> body) || !(TheF = TheModule.getFunction(funcDeclTree -> name)))
            {
                std::vector<llvm::Type *> ArgType;
                for(auto son:funcDeclTree -> sons)
                    ArgType.emplace_back(toLLVMType(son->info));
                llvm::FunctionType * FT = llvm::FunctionType::get(toLLVMType(funcDeclTree -> info),ArgType,funcDeclTree -> valen);
                TheF = llvm::Function::Create(FT,llvm::Function::ExternalLinkage,funcDeclTree -> name,TheModule);
                int index = 0;
                for(auto &arg:TheF -> args())
                {
                    arg.setName(funcDeclTree -> sons[index] -> name);
                    index++;
                }
                if(!(funcDeclTree -> body))
                {
                    funcDeclTree -> funclr = TheF;
                    return;
                }
                    
                    
            }
            if(!(funcDeclTree -> funclr = TheF))
                return;

            llvm::BasicBlock * BB = llvm::BasicBlock::Create(TheContext,"entry",TheF);
            Builder.SetInsertPoint(BB);

            int index = 0;
            for(auto &arg:TheF -> args())
            {
                funcDeclTree -> sons[index] -> accept(this);
                //llvm::AllocaInst *Alloca = CreateEntryBlockAlloca(TheF, toLLVMType(funcDeclTree -> sons[index] -> info),nullptr,arg.getName());
                Builder.CreateStore(&arg, funcDeclTree -> sons[index] -> ir);
                index++;
            }
            
            
            funcDeclTree -> body -> accept(this);
            
            llvm::verifyFunction(*TheF);
            
            if(funcDeclTree -> info.type == Void)
            {
                Builder.CreateRetVoid();
                llvm::BasicBlock * uselessB = llvm::BasicBlock::Create(TheContext, "useless", TheF);
                Builder.SetInsertPoint(uselessB);
            }

            Builder.CreateUnreachable();            
        }
        virtual void VisitDeclStmtTree(DeclStmtTree * declStmtTree)
        {
            for(auto & son:declStmtTree -> sons)
                son -> accept(this);
        }
        virtual void VisitNullStmtTree(NullStmtTree * nullStmtTree)
        {
            //No ops
        }
        virtual void VisitExprStmtTree(ExprStmtTree * exprStmtTree)
        {
            exprStmtTree -> expr -> accept(this);
            exprStmtTree ->ir = exprStmtTree -> expr -> ir;
        }
        virtual void VisitBlockStmtTree(BlockStmtTree * blockStmtTree)
        {
            for(auto & son:blockStmtTree -> sons)
                son -> accept(this);
        }
        virtual void VisitIfStmtTree(IfStmtTree * ifStmtTree)
        {
            ifStmtTree -> cond -> accept(this);
            llvm::Value * res;
            switch (ifStmtTree -> cond -> info.type)
            {
                case Int:
                    res = Builder.CreateICmpNE(ifStmtTree -> cond -> ir, llvm::ConstantInt::get(TheContext, llvm::APInt(32,0,1)), "ifcond");
                    break;
                case Longlong:
                    res = Builder.CreateICmpNE(ifStmtTree -> cond -> ir, llvm::ConstantInt::get(TheContext, llvm::APInt(64,0,1)), "ifcond");
                    break;
                case Char:    
                    res = Builder.CreateICmpNE(ifStmtTree -> cond -> ir, llvm::ConstantInt::get(TheContext, llvm::APInt(82,0,1)), "ifcond");
                    break;
                case Float:
                case Double:
                    res = Builder.CreateFCmpONE(ifStmtTree -> cond -> ir, llvm::ConstantFP::get(TheContext, llvm::APFloat(0.0)), "ifcond");
                    break;
            }
            
            llvm::Function *TheFunction = Builder.GetInsertBlock() -> getParent();

            llvm::BasicBlock *ThenBB = llvm::BasicBlock::Create(TheContext, "then", TheFunction);
            llvm::BasicBlock *ElseBB = llvm::BasicBlock::Create(TheContext, "else");
            llvm::BasicBlock *MergeBB = llvm::BasicBlock::Create(TheContext, "ifcont");
            Builder.CreateCondBr(res, ThenBB, ElseBB);
            
            Builder.SetInsertPoint(ThenBB);
            ifStmtTree -> truestmt -> accept(this);
            Builder.CreateBr(MergeBB);
            ThenBB = Builder.GetInsertBlock();
            
            TheFunction->getBasicBlockList().push_back(ElseBB);
            Builder.SetInsertPoint(ElseBB);
            if(ifStmtTree -> falsestmt)
                ifStmtTree -> falsestmt -> accept(this);
            Builder.CreateBr(MergeBB);
            ElseBB = Builder.GetInsertBlock();

            TheFunction->getBasicBlockList().push_back(MergeBB);
            Builder.SetInsertPoint(MergeBB);
        }
        virtual void VisitWhileStmtTree(WhileStmtTree * whileStmtTree)
        {
            llvm::Function *TheFunction = Builder.GetInsertBlock() -> getParent();
            llvm::BasicBlock * condB = llvm::BasicBlock::Create(TheContext, "loopcond", TheFunction);
            llvm::BasicBlock * bodyB = llvm::BasicBlock::Create(TheContext, "loopbody");
            llvm::BasicBlock * afterB = llvm::BasicBlock::Create(TheContext, "afterloop");

            whileStmtTree -> breakB = afterB;
            whileStmtTree -> continueB = condB;

            Builder.CreateBr(condB);

            Builder.SetInsertPoint(condB);
            whileStmtTree -> cond -> accept(this);
            llvm::Value * res;
            switch (whileStmtTree -> cond -> info.type)
            {
                case Int:
                    res = Builder.CreateICmpNE(whileStmtTree -> cond -> ir, llvm::ConstantInt::get(TheContext, llvm::APInt(32,0,1)), "whilecond");
                    break;
                case Longlong:
                    res = Builder.CreateICmpNE(whileStmtTree -> cond -> ir, llvm::ConstantInt::get(TheContext, llvm::APInt(64,0,1)), "whilecond");
                    break;
                case Char:    
                    res = Builder.CreateICmpNE(whileStmtTree -> cond -> ir, llvm::ConstantInt::get(TheContext, llvm::APInt(8,0,1)), "whilecond");
                    break;
                case Float:
                case Double:
                    res = Builder.CreateFCmpONE(whileStmtTree -> cond -> ir, llvm::ConstantFP::get(TheContext, llvm::APFloat(0.0)), "whilecond");
                    break;
            }
            Builder.CreateCondBr(res, bodyB,afterB);

            TheFunction->getBasicBlockList().push_back(bodyB);
            Builder.SetInsertPoint(bodyB);
            whileStmtTree -> stmt -> accept(this);
            Builder.CreateBr(condB);

            TheFunction->getBasicBlockList().push_back(afterB);
            Builder.SetInsertPoint(afterB);
        }
        virtual void VisitDoStmtTree(DoStmtTree * doStmtTree)
        {
            llvm::Function *TheFunction = Builder.GetInsertBlock() -> getParent();
            llvm::BasicBlock * condB = llvm::BasicBlock::Create(TheContext, "loopcond");
            llvm::BasicBlock * bodyB = llvm::BasicBlock::Create(TheContext, "loopbody", TheFunction);
            llvm::BasicBlock * afterB = llvm::BasicBlock::Create(TheContext, "afterloop");

            doStmtTree -> breakB = afterB;
            doStmtTree -> continueB = condB;

            Builder.CreateBr(bodyB);

            Builder.SetInsertPoint(bodyB);
            doStmtTree -> stmt -> accept(this);
            Builder.CreateBr(condB);

            TheFunction->getBasicBlockList().push_back(condB);
            Builder.SetInsertPoint(condB);
            doStmtTree -> cond -> accept(this);
            llvm::Value * res;
            switch (doStmtTree -> cond -> info.type)
            {
                case Int:
                    res = Builder.CreateICmpNE(doStmtTree -> cond -> ir, llvm::ConstantInt::get(TheContext, llvm::APInt(32,0,1)), "docond");
                    break;
                case Longlong:
                    res = Builder.CreateICmpNE(doStmtTree -> cond -> ir, llvm::ConstantInt::get(TheContext, llvm::APInt(64,0,1)), "docond");
                    break;
                case Char:    
                    res = Builder.CreateICmpNE(doStmtTree -> cond -> ir, llvm::ConstantInt::get(TheContext, llvm::APInt(8,0,1)), "docond");
                    break;
                case Float:
                case Double:
                    res = Builder.CreateFCmpONE(doStmtTree -> cond -> ir, llvm::ConstantFP::get(TheContext, llvm::APFloat(0.0)), "docond");
                    break;
            }
            Builder.CreateCondBr(res, bodyB,afterB);


            TheFunction->getBasicBlockList().push_back(afterB);
            Builder.SetInsertPoint(afterB);

            
        }
        virtual void VisitForStmtTree(ForStmtTree * forStmtTree)
        {
            llvm::Function *TheFunction = Builder.GetInsertBlock() -> getParent();
            llvm::BasicBlock * condB = llvm::BasicBlock::Create(TheContext, "loopcond", TheFunction);
            llvm::BasicBlock * bodyB = llvm::BasicBlock::Create(TheContext, "loopbody");
            llvm::BasicBlock * incrB = llvm::BasicBlock::Create(TheContext, "loopincr");
            llvm::BasicBlock * afterB = llvm::BasicBlock::Create(TheContext, "afterloop");

            forStmtTree -> breakB = afterB;
            forStmtTree -> continueB = incrB;

            forStmtTree -> init -> accept(this);
            Builder.CreateBr(condB);

            Builder.SetInsertPoint(condB);
            forStmtTree -> cond -> accept(this);
            llvm::Value * res;
            switch (forStmtTree -> cond -> info.type)
            {
                case Int:
                    res = Builder.CreateICmpNE(forStmtTree -> cond -> ir, llvm::ConstantInt::get(TheContext, llvm::APInt(32,0,1)), "forcond");
                    break;
                case Longlong:
                    res = Builder.CreateICmpNE(forStmtTree -> cond -> ir, llvm::ConstantInt::get(TheContext, llvm::APInt(64,0,1)), "forcond");
                    break;
                case Char:    
                    res = Builder.CreateICmpNE(forStmtTree -> cond -> ir, llvm::ConstantInt::get(TheContext, llvm::APInt(8,0,1)), "forcond");
                    break;
                case Float:
                case Double:
                    res = Builder.CreateFCmpONE(forStmtTree -> cond -> ir, llvm::ConstantFP::get(TheContext, llvm::APFloat(0.0)), "forcond");
                    break;
            }
            Builder.CreateCondBr(res, bodyB,afterB);

            TheFunction->getBasicBlockList().push_back(bodyB);
            Builder.SetInsertPoint(bodyB);
            forStmtTree -> stmt -> accept(this);
            Builder.CreateBr(incrB);

            TheFunction->getBasicBlockList().push_back(incrB);
            Builder.SetInsertPoint(incrB);
            forStmtTree -> incr -> accept(this);
            Builder.CreateBr(condB);

            TheFunction->getBasicBlockList().push_back(afterB);
            Builder.SetInsertPoint(afterB);
        }
        virtual void VisitContinueStmtTree(ContinueStmtTree * continueStmtTree)
        {
            Builder.CreateBr(continueStmtTree -> loop -> continueB);
            llvm::Function *TheFunction = Builder.GetInsertBlock() -> getParent();
            llvm::BasicBlock * uselessB = llvm::BasicBlock::Create(TheContext, "useless", TheFunction);
            Builder.SetInsertPoint(uselessB);
        }
        virtual void VisitBreakStmtTree(BreakStmtTree * breakStmtTree)
        {
            Builder.CreateBr(breakStmtTree -> loop -> breakB);
            llvm::Function *TheFunction = Builder.GetInsertBlock() -> getParent();
            llvm::BasicBlock * uselessB = llvm::BasicBlock::Create(TheContext, "useless", TheFunction);
            Builder.SetInsertPoint(uselessB);
        }
        virtual void VisitReturnStmtTree(ReturnStmtTree * returnStmtTree)
        {
            if(returnStmtTree ->expr)
            {
                returnStmtTree -> expr -> accept(this);
                Builder.CreateRet(returnStmtTree -> expr -> ir);
            }
            else
                Builder.CreateRetVoid();
            llvm::Function *TheFunction = Builder.GetInsertBlock() -> getParent();
            llvm::BasicBlock * uselessB = llvm::BasicBlock::Create(TheContext, "useless", TheFunction);
            Builder.SetInsertPoint(uselessB);
        }
        virtual void VisitLiteralTree(LiteralTree * literalTree)
        { 
            switch(literalTree -> lt)
            {
                case Interger:  
                    if(literalTree -> info.type == Int)
                        literalTree ->ir = llvm::ConstantInt::get(TheContext,llvm::APInt(32,literalTree -> lval,1));
                    else if(literalTree -> info.type == Longlong)
                        literalTree ->ir = llvm::ConstantInt::get(TheContext,llvm::APInt(64,literalTree -> lval,1));
                    break;
                case Floating:
                    literalTree -> ir = llvm::ConstantFP::get(TheContext,llvm::APFloat(literalTree -> dval));
                    break;
                case String:
                {
                    size_t len = literalTree -> value.length() + 1;
                    std::vector<llvm::Constant *> elements(len);
                    for(int i = 0;i < len - 1;i++)
                        elements[i] = llvm::ConstantInt::get(TheContext,llvm::APInt(8,literalTree -> value[i],1));
                    elements[len - 1] = llvm::ConstantInt::get(TheContext,llvm::APInt(8,0,1));
                    llvm::Constant * initval = llvm::ConstantArray::get(llvm::ArrayType::get(llvm::Type::getInt8Ty(TheContext),len),elements);
                    literalTree -> ir = new llvm::GlobalVariable(TheModule, llvm::ArrayType::get(llvm::Type::getInt8Ty(TheContext),len), 1, llvm::GlobalValue::ExternalLinkage, initval);;
                    break;
                }  
            }
        }
        virtual void VisitParenExprTree(ParenExprTree * parenExprTree)
        {
            parenExprTree -> son -> accept(this);
            parenExprTree -> ir = parenExprTree -> son -> ir;
        }
        virtual void VisitInitListExprTree(InitListExprTree * initListExprTree)
        {

            if(initListExprTree -> isGlobal)
            {
                std::vector<size_t> steps(initListExprTree->info.layer.len.size() + 1);
                steps[0] = 1;
                for (int i = 1, j = 0; j < initListExprTree->info.layer.len.size() ; i++, j++)
                {
                    steps[i] = steps[i - 1] * initListExprTree -> info.layer.len[j];
                }

                std::vector<llvm::Constant *> elements(steps[initListExprTree->info.layer.len.size()]);
                fillConstInitList(elements,initListExprTree,steps,initListExprTree->info.layer.len.size() - 1,initListExprTree->info.layer.len,initListExprTree->info.layer.len.size() - 1,0);
                initListExprTree -> ir = llvm::ConstantArray::get(llvm::ArrayType::get(BasicType(initListExprTree -> info.type),steps[initListExprTree->info.layer.len.size()]),elements);
            }
            else
            {
                for(auto son:initListExprTree -> sons)
                    son -> accept(this);
            }
        }
        virtual void VisitImplicitCastExprTree(ImplicitCastExprTree * implicitCastExprTree)
        {
            implicitCastExprTree -> cast -> accept(this);
            if(implicitCastExprTree -> castkind == "IntegralCast")
            {
                if(implicitCastExprTree -> info.type == Int)
                    implicitCastExprTree -> ir = Builder.CreateTrunc(implicitCastExprTree -> cast -> ir,llvm::Type::getInt32Ty(TheContext),"LonglongToInt");
                else
                    implicitCastExprTree -> ir = Builder.CreateSExt(implicitCastExprTree -> cast -> ir,llvm::Type::getInt64Ty(TheContext),"IntToLonglong");
            }
            else if(implicitCastExprTree -> castkind == "IntegralToFloating")
                implicitCastExprTree -> ir = Builder.CreateSIToFP(implicitCastExprTree -> cast -> ir,llvm::Type::getFloatTy(TheContext),"IntegralToFloat");
            else if(implicitCastExprTree -> castkind == "FloatingToIntegral")
            {
                if(implicitCastExprTree -> info.type == Int)
                    implicitCastExprTree -> ir = Builder.CreateFPToSI(implicitCastExprTree -> cast -> ir,llvm::Type::getInt32Ty(TheContext),"FloatToInt");
                else
                    implicitCastExprTree -> ir = Builder.CreateFPToSI(implicitCastExprTree -> cast -> ir,llvm::Type::getInt64Ty(TheContext),"FloatToLonglong");
            }
            else if(implicitCastExprTree -> castkind == "LValueToRValue")
                implicitCastExprTree -> ir = Builder.CreateLoad(implicitCastExprTree -> cast -> ir);
            else
                implicitCastExprTree -> ir = implicitCastExprTree -> cast -> ir;
        }
        virtual void VisitArraySubscriptExprTree(ArraySubscriptExprTree * arraySubscriptExprTree)
        {
            arraySubscriptExprTree -> sym -> accept(this);
            arraySubscriptExprTree -> script -> accept(this);
            ImplicitCastExprTree * p;
            if((p = dynamic_cast<ImplicitCastExprTree *>(arraySubscriptExprTree -> sym)) && (p -> castkind == "ArrayToPointerDecay"))
                arraySubscriptExprTree -> ir = Builder.CreateInBoundsGEP(arraySubscriptExprTree -> sym -> ir, {llvm::ConstantInt::get(TheContext, llvm::APInt(32, 0)), arraySubscriptExprTree -> script -> ir});
            else
                arraySubscriptExprTree -> ir = Builder.CreateInBoundsGEP(arraySubscriptExprTree -> sym -> ir, {arraySubscriptExprTree -> script -> ir});
        }
        virtual void VisitDeclRefTree(DeclRefTree * declRefTree)
        {
                declRefTree -> ir = declRefTree -> decl -> ir;
        }
        virtual void VisitUnaryExprTree(UnaryExprTree * unaryExprTree)
        {
            unaryExprTree -> son -> accept(this);
            if(!(unaryExprTree -> son -> ir))
            {
                unaryExprTree -> ir = nullptr;
                return;
            }
            
            switch(unaryExprTree -> opcode)
            {
                case OP_TILDE:      
                    unaryExprTree -> ir = Builder.CreateNot(unaryExprTree -> son -> ir,"rev");
                    break;
                case OP_MINUS:      
                    if(unaryExprTree -> info.type == Int || unaryExprTree -> info.type == Longlong)          
                        unaryExprTree -> ir = Builder.CreateNeg(unaryExprTree -> son -> ir,"neg");
                    else
                        unaryExprTree -> ir = Builder.CreateFNeg(unaryExprTree -> son -> ir,"fneg");
                    break;
                case OP_PLUS:   
                    unaryExprTree -> ir = unaryExprTree -> son -> ir;
                    break;  
                case OP_EXCLAIM:
                {
                    size_t len = (unaryExprTree -> info.type == Int) ? 32 :(unaryExprTree -> info.type == Char ? 8 : 64);
                    llvm::Value * temp = Builder.CreateICmpEQ(unaryExprTree -> son -> ir,llvm::ConstantInt::get(TheContext, llvm::APInt(len,0,1)),"not");
                    unaryExprTree -> ir = Builder.CreateZExt(temp,llvm::Type::getInt32Ty(TheContext));
                    break;
                }
                case OP_PLUSPLUS:
                {
                    unaryExprTree -> son -> accept(this);
                    size_t len = (unaryExprTree -> info.type == Int) ? 32 :(unaryExprTree -> info.type == Char ? 8 : 64);
                    llvm::Value * lv = Builder.CreateLoad(unaryExprTree -> son -> ir);
                    llvm::Value * one = llvm::ConstantInt::get(TheContext,llvm::APInt(len,1,1));
                    llvm::Value * temp = Builder.CreateAdd(lv,one,"plusplus");
                    Builder.CreateStore(temp,unaryExprTree -> son -> ir);
                    break;
                }
                case OP_MINUSMINUS:
                {
                    unaryExprTree -> son -> accept(this);
                    size_t len = (unaryExprTree -> info.type == Int) ? 32 :(unaryExprTree -> info.type == Char ? 8 : 64);
                    llvm::Value * lv = Builder.CreateLoad(unaryExprTree -> son -> ir);
                    llvm::Value * one = llvm::ConstantInt::get(TheContext,llvm::APInt(len,1,1));
                    llvm::Value * temp = Builder.CreateSub(lv,one,"minusminus");
                    Builder.CreateStore(temp,unaryExprTree -> son -> ir);
                    break;
                }
                default:
                    unaryExprTree -> ir = nullptr;
                    break;
            }

        }
        virtual void VisitBinaryExprTree(BinaryExprTree * binaryExprTree)
        {            
            if(binaryExprTree -> info.type == Int || binaryExprTree -> info.type == Longlong)
            {
                switch(binaryExprTree -> opcode)
                {
                    case OP_STAR:
                    {
                        binaryExprTree -> left -> accept(this);
                        binaryExprTree -> right -> accept(this);
                        if(!(binaryExprTree -> left -> ir) || !(binaryExprTree -> right -> ir))
                        {
                            binaryExprTree -> ir = nullptr;
                            return;
                        }
                        binaryExprTree -> ir = Builder.CreateMul(binaryExprTree -> left -> ir,binaryExprTree -> right -> ir,"mul");
                        break;
                    }
                    case OP_SLASH:
                    {
                        binaryExprTree -> left -> accept(this);
                        binaryExprTree -> right -> accept(this);
                        if(!(binaryExprTree -> left -> ir) || !(binaryExprTree -> right -> ir))
                        {
                            binaryExprTree -> ir = nullptr;
                            return;
                        }
                        binaryExprTree -> ir = Builder.CreateSDiv(binaryExprTree -> left -> ir,binaryExprTree -> right -> ir,"div");
                        break;
                    } 
                    case OP_PERCENT:
                    {
                        binaryExprTree -> left -> accept(this);
                        binaryExprTree -> right -> accept(this);
                        if(!(binaryExprTree -> left -> ir) || !(binaryExprTree -> right -> ir))
                        {
                            binaryExprTree -> ir = nullptr;
                            return;
                        }
                        binaryExprTree -> ir = Builder.CreateSRem(binaryExprTree -> left -> ir,binaryExprTree -> right -> ir,"rem");
                        break;
                    }
                        
                    case OP_PLUS:
                    {
                        binaryExprTree -> left -> accept(this);
                        binaryExprTree -> right -> accept(this);
                        if(!(binaryExprTree -> left -> ir) || !(binaryExprTree -> right -> ir))
                        {
                            binaryExprTree -> ir = nullptr;
                            return;
                        }
                        binaryExprTree -> ir = Builder.CreateAdd(binaryExprTree -> left -> ir,binaryExprTree -> right -> ir,"add");
                        break;
                    }
                        
                    case OP_MINUS:
                    {
                        binaryExprTree -> left -> accept(this);
                        binaryExprTree -> right -> accept(this);
                        if(!(binaryExprTree -> left -> ir) || !(binaryExprTree -> right -> ir))
                        {
                            binaryExprTree -> ir = nullptr;
                            return;
                        }
                        binaryExprTree -> ir = Builder.CreateSub(binaryExprTree -> left -> ir,binaryExprTree -> right -> ir,"sub");
                        break;
                    }    
                    case OP_LESSLESS:
                    {
                        binaryExprTree -> left -> accept(this);
                        binaryExprTree -> right -> accept(this);
                        if(!(binaryExprTree -> left -> ir) || !(binaryExprTree -> right -> ir))
                        {
                            binaryExprTree -> ir = nullptr;
                            return;
                        }
                        binaryExprTree -> ir = Builder.CreateShl(binaryExprTree -> left -> ir,binaryExprTree -> right -> ir,"shl");
                        break;
                    }          
                    case OP_GREATERGREATER:
                    {
                        binaryExprTree -> left -> accept(this);
                        binaryExprTree -> right -> accept(this);
                        if(!(binaryExprTree -> left -> ir) || !(binaryExprTree -> right -> ir))
                        {
                            binaryExprTree -> ir = nullptr;
                            return;
                        }
                        binaryExprTree -> ir = Builder.CreateLShr(binaryExprTree -> left -> ir,binaryExprTree -> right -> ir,"shr");
                        break;
                    }   
                    case OP_LESS:
                    {
                        binaryExprTree -> left -> accept(this);
                        binaryExprTree -> right -> accept(this);
                        if(!(binaryExprTree -> left -> ir) || !(binaryExprTree -> right -> ir))
                        {
                            binaryExprTree -> ir = nullptr;
                            return;
                        }
                        llvm::Value * temp;
                        if(binaryExprTree -> left -> info.type < Float)
                            temp = Builder.CreateICmpSLT(binaryExprTree -> left -> ir,binaryExprTree -> right -> ir,"less");
                        else
                            temp = Builder.CreateFCmpULT(binaryExprTree -> left -> ir,binaryExprTree -> right -> ir,"less");
                        binaryExprTree -> ir = Builder.CreateZExt(temp,llvm::Type::getInt32Ty(TheContext),"bool_32");
                        break;
                    }                       
                    case OP_GREATER:
                    {
                        binaryExprTree -> left -> accept(this);
                        binaryExprTree -> right -> accept(this);
                        if(!(binaryExprTree -> left -> ir) || !(binaryExprTree -> right -> ir))
                        {
                            binaryExprTree -> ir = nullptr;
                            return;
                        }
                        llvm::Value * temp;
                        if(binaryExprTree -> left -> info.type < Float)
                            temp = Builder.CreateICmpSGT(binaryExprTree -> left -> ir,binaryExprTree -> right -> ir,"greater");
                        else
                            temp = Builder.CreateFCmpUGT(binaryExprTree -> left -> ir,binaryExprTree -> right -> ir,"greater");
                        binaryExprTree -> ir = Builder.CreateZExt(temp,llvm::Type::getInt32Ty(TheContext),"bool_32");
                        break;
                    }                        
                    case OP_LESSEQUAL:
                    {
                        binaryExprTree -> left -> accept(this);
                        binaryExprTree -> right -> accept(this);
                        if(!(binaryExprTree -> left -> ir) || !(binaryExprTree -> right -> ir))
                        {
                            binaryExprTree -> ir = nullptr;
                            return;
                        }
                        llvm::Value * temp;
                        if(binaryExprTree -> left -> info.type < Float)
                            temp = Builder.CreateICmpSLE(binaryExprTree -> left -> ir,binaryExprTree -> right -> ir,"lesse");
                        else
                            temp = Builder.CreateFCmpULE(binaryExprTree -> left -> ir,binaryExprTree -> right -> ir,"lesse");
                        binaryExprTree -> ir = Builder.CreateZExt(temp,llvm::Type::getInt32Ty(TheContext),"bool_32");
                        break;
                    }                       
                    case OP_GREATEREQUAL:
                    {
                        binaryExprTree -> left -> accept(this);
                        binaryExprTree -> right -> accept(this);
                        if(!(binaryExprTree -> left -> ir) || !(binaryExprTree -> right -> ir))
                        {
                            binaryExprTree -> ir = nullptr;
                            return;
                        }
                        llvm::Value * temp;
                        if(binaryExprTree -> left -> info.type < Float)
                            temp = Builder.CreateICmpSGE(binaryExprTree -> left -> ir,binaryExprTree -> right -> ir,"greatere");
                        else
                            temp = Builder.CreateFCmpUGE(binaryExprTree -> left -> ir,binaryExprTree -> right -> ir,"greatere");
                        binaryExprTree -> ir = Builder.CreateZExt(temp,llvm::Type::getInt32Ty(TheContext),"bool_32");
                        break;
                    }                        
                    case OP_EQUALEQUAL:
                    {
                        binaryExprTree -> left -> accept(this);
                        binaryExprTree -> right -> accept(this);
                        if(!(binaryExprTree -> left -> ir) || !(binaryExprTree -> right -> ir))
                        {
                            binaryExprTree -> ir = nullptr;
                            return;
                        }
                        llvm::Value * temp;
                        if(binaryExprTree -> left -> info.type < Float)
                            temp = Builder.CreateICmpEQ(binaryExprTree -> left -> ir,binaryExprTree -> right -> ir,"eq");
                        else
                            temp = Builder.CreateFCmpUEQ(binaryExprTree -> left -> ir,binaryExprTree -> right -> ir,"eq");
                        binaryExprTree -> ir = Builder.CreateZExt(temp,llvm::Type::getInt32Ty(TheContext),"bool_32");
                        break;
                    }                        
                    case OP_EXCLAIMEQUAL:
                    {
                        binaryExprTree -> left -> accept(this);
                        binaryExprTree -> right -> accept(this);
                        if(!(binaryExprTree -> left -> ir) || !(binaryExprTree -> right -> ir))
                        {
                            binaryExprTree -> ir = nullptr;
                            return;
                        }
                        llvm::Value * temp;
                        if(binaryExprTree -> left -> info.type < Float)
                            temp = Builder.CreateICmpNE(binaryExprTree -> left -> ir,binaryExprTree -> right -> ir,"neq");
                        else
                            temp = Builder.CreateFCmpUNE(binaryExprTree -> left -> ir,binaryExprTree -> right -> ir,"neq");
                        binaryExprTree -> ir = Builder.CreateZExt(temp,llvm::Type::getInt32Ty(TheContext),"bool_32");
                        break;
                    }                        
                    case OP_AMP:
                    {
                        binaryExprTree -> left -> accept(this);
                        binaryExprTree -> right -> accept(this);
                        if(!(binaryExprTree -> left -> ir) || !(binaryExprTree -> right -> ir))
                        {
                            binaryExprTree -> ir = nullptr;
                            return;
                        }
                        binaryExprTree -> ir = Builder.CreateAnd(binaryExprTree -> left -> ir,binaryExprTree -> right -> ir,"and");
                        break;
                    }
                        
                    case OP_CARET:
                    {
                        binaryExprTree -> left -> accept(this);
                        binaryExprTree -> right -> accept(this);
                        if(!(binaryExprTree -> left -> ir) || !(binaryExprTree -> right -> ir))
                        {
                            binaryExprTree -> ir = nullptr;
                            return;
                        }
                        binaryExprTree -> ir = Builder.CreateXor(binaryExprTree -> left -> ir,binaryExprTree -> right -> ir,"xor");
                        break;
                    }                       
                    case OP_PIPE:
                    {
                        binaryExprTree -> left -> accept(this);
                        binaryExprTree -> right -> accept(this);
                        if(!(binaryExprTree -> left -> ir) || !(binaryExprTree -> right -> ir))
                        {
                            binaryExprTree -> ir = nullptr;
                            return;
                        }
                        binaryExprTree -> ir = Builder.CreateOr(binaryExprTree -> left -> ir,binaryExprTree -> right -> ir,"or");
                        break;
                    }                       
                    case OP_AMPAMP:  //No short circuit
                    {
                        binaryExprTree -> left -> accept(this);
                        if(!(binaryExprTree -> left -> ir))
                        {
                            binaryExprTree -> ir = nullptr;
                            return;
                        }
                        llvm::Value * lres;
                        switch (binaryExprTree -> left -> info.type)
                        {
                            case Int:
                                lres = Builder.CreateICmpNE(binaryExprTree -> left -> ir, llvm::ConstantInt::get(TheContext, llvm::APInt(32,0,1)), "and_left");
                                break;
                            case Longlong:
                                lres = Builder.CreateICmpNE(binaryExprTree -> left -> ir, llvm::ConstantInt::get(TheContext, llvm::APInt(64,0,1)), "and_left");
                                break;
                            case Char:    
                                lres = Builder.CreateICmpNE(binaryExprTree -> left -> ir, llvm::ConstantInt::get(TheContext, llvm::APInt(8,0,1)), "and_left");
                                break;
                            case Float:
                            case Double:
                                lres = Builder.CreateFCmpONE(binaryExprTree -> left -> ir, llvm::ConstantFP::get(TheContext, llvm::APFloat(0.0)), "and_left");
                                break;
                        }
                        
                        llvm::Function *TheFunction = Builder.GetInsertBlock() -> getParent();
                        llvm::BasicBlock * CurBlock = Builder.GetInsertBlock();

                        llvm::BasicBlock *TrueBB = llvm::BasicBlock::Create(TheContext, "and_not_decided", TheFunction);
                        llvm::BasicBlock *MergeBB = llvm::BasicBlock::Create(TheContext, "andcont");
                        Builder.CreateCondBr(lres, TrueBB, MergeBB);

                        Builder.SetInsertPoint(TrueBB);
                        binaryExprTree -> right -> accept(this);
                        llvm::Value * rres;
                        switch (binaryExprTree -> right -> info.type)
                        {
                            case Int:
                                rres = Builder.CreateICmpNE(binaryExprTree -> right -> ir, llvm::ConstantInt::get(TheContext, llvm::APInt(32,0,1)), "and_right");
                                break;
                            case Longlong:
                                rres = Builder.CreateICmpNE(binaryExprTree -> right -> ir, llvm::ConstantInt::get(TheContext, llvm::APInt(64,0,1)), "and_right");
                                break;
                            case Char:    
                                rres = Builder.CreateICmpNE(binaryExprTree -> right -> ir, llvm::ConstantInt::get(TheContext, llvm::APInt(8,0,1)), "and_right");
                                break;
                            case Float:
                            case Double:
                                rres = Builder.CreateFCmpONE(binaryExprTree -> right -> ir, llvm::ConstantFP::get(TheContext, llvm::APFloat(0.0)), "and_right");
                                break;
                        }
                        Builder.CreateBr(MergeBB);
                        TrueBB = Builder.GetInsertBlock();

                        TheFunction->getBasicBlockList().push_back(MergeBB);
                        Builder.SetInsertPoint(MergeBB);

                        llvm::PHINode *PN = Builder.CreatePHI(llvm::Type::getInt1Ty(TheContext), 2, "andtmp");
                        PN->addIncoming(lres, CurBlock);
                        PN->addIncoming(rres, TrueBB);
                        binaryExprTree -> ir = Builder.CreateZExt(PN,llvm::Type::getInt32Ty(TheContext),"bool_32");
                        break;
                    }               
                    case OP_PIPEPIPE:  
                    {
                        binaryExprTree -> left -> accept(this);
                        if(!(binaryExprTree -> left -> ir))
                        {
                            binaryExprTree -> ir = nullptr;
                            return;
                        }
                        llvm::Value * lres;
                        switch (binaryExprTree -> left -> info.type)
                        {
                            case Int:
                                lres = Builder.CreateICmpNE(binaryExprTree -> left -> ir, llvm::ConstantInt::get(TheContext, llvm::APInt(32,0,1)), "or_left");
                                break;
                            case Longlong:
                                lres = Builder.CreateICmpNE(binaryExprTree -> left -> ir, llvm::ConstantInt::get(TheContext, llvm::APInt(64,0,1)), "or_left");
                                break;
                            case Char:    
                                lres = Builder.CreateICmpNE(binaryExprTree -> left -> ir, llvm::ConstantInt::get(TheContext, llvm::APInt(8,0,1)), "or_left");
                                break;
                            case Float:
                            case Double:
                                lres = Builder.CreateFCmpONE(binaryExprTree -> left -> ir, llvm::ConstantFP::get(TheContext, llvm::APFloat(0.0)), "or_left");
                                break;
                        }
                        
                        llvm::Function *TheFunction = Builder.GetInsertBlock() -> getParent();
                        llvm::BasicBlock * CurBlock = Builder.GetInsertBlock();

                        llvm::BasicBlock *FalseBB = llvm::BasicBlock::Create(TheContext, "or_not_decided", TheFunction);
                        llvm::BasicBlock *MergeBB = llvm::BasicBlock::Create(TheContext, "orcont");
                        Builder.CreateCondBr(lres, MergeBB, FalseBB);

                        Builder.SetInsertPoint(FalseBB);
                        binaryExprTree -> right -> accept(this);
                        llvm::Value * rres;
                        switch (binaryExprTree -> right -> info.type)
                        {
                            case Int:
                                rres = Builder.CreateICmpNE(binaryExprTree -> right -> ir, llvm::ConstantInt::get(TheContext, llvm::APInt(32,0,1)), "or_right");
                                break;
                            case Longlong:
                                rres = Builder.CreateICmpNE(binaryExprTree -> right -> ir, llvm::ConstantInt::get(TheContext, llvm::APInt(64,0,1)), "or_right");
                                break;
                            case Char:    
                                rres = Builder.CreateICmpNE(binaryExprTree -> right -> ir, llvm::ConstantInt::get(TheContext, llvm::APInt(8,0,1)), "or_right");
                                break;
                            case Float:
                            case Double:
                                rres = Builder.CreateFCmpONE(binaryExprTree -> right -> ir, llvm::ConstantFP::get(TheContext, llvm::APFloat(0.0)), "or_right");
                                break;
                        }
                        Builder.CreateBr(MergeBB);
                        FalseBB = Builder.GetInsertBlock();

                        TheFunction->getBasicBlockList().push_back(MergeBB);
                        Builder.SetInsertPoint(MergeBB);

                        llvm::PHINode *PN = Builder.CreatePHI(llvm::Type::getInt1Ty(TheContext), 2, "ortmp");
                        PN->addIncoming(lres, CurBlock);
                        PN->addIncoming(rres, FalseBB);
                        binaryExprTree -> ir = Builder.CreateZExt(PN,llvm::Type::getInt32Ty(TheContext),"bool_32");
                        break;
                    }
                    case OP_COMMA:
                    {
                        binaryExprTree -> left -> accept(this);
                        binaryExprTree -> right -> accept(this);
                        if(!(binaryExprTree -> left -> ir) || !(binaryExprTree -> right -> ir))
                        {
                            binaryExprTree -> ir = nullptr;
                            return;
                        }
                        binaryExprTree -> ir = binaryExprTree -> right -> ir;
                        break;
                    }
                    case OP_EQUAL:
                    {
                        binaryExprTree -> left -> accept(this);

                        llvm::Value * left = binaryExprTree -> left -> ir;
                        binaryExprTree -> right -> accept(this);
                        Builder.CreateStore(binaryExprTree -> right -> ir,left);
                        break;
                    }
                    case OP_PLUSEQUAL:
                    {
                        binaryExprTree -> left -> accept(this);
                        binaryExprTree -> right -> accept(this);
                        llvm::Value * lv = Builder.CreateLoad(binaryExprTree -> left -> ir);
                        llvm::Value * temp = Builder.CreateAdd(lv,binaryExprTree -> right -> ir,"add");
                        Builder.CreateStore(temp,binaryExprTree -> left -> ir);
                        break;
                    }
                    case OP_MINUSEQUAL:
                    {
                        binaryExprTree -> left -> accept(this);
                        binaryExprTree -> right -> accept(this);
                        llvm::Value * lv = Builder.CreateLoad(binaryExprTree -> left -> ir);
                        llvm::Value * temp = Builder.CreateSub(lv,binaryExprTree -> right -> ir,"sub");
                        Builder.CreateStore(temp,binaryExprTree -> left -> ir);
                        break;
                    }
                    case OP_STAREQUAL:
                    {
                        binaryExprTree -> left -> accept(this);
                        binaryExprTree -> right -> accept(this);
                        llvm::Value * lv = Builder.CreateLoad(binaryExprTree -> left -> ir);
                        llvm::Value * temp = Builder.CreateMul(lv,binaryExprTree -> right -> ir,"mul");
                        Builder.CreateStore(temp,binaryExprTree -> left -> ir);
                        break;
                    }
                    case OP_SLASHEQUAL:
                    {
                        binaryExprTree -> left -> accept(this);
                        binaryExprTree -> right -> accept(this);
                        llvm::Value * lv = Builder.CreateLoad(binaryExprTree -> left -> ir);
                        llvm::Value * temp = Builder.CreateSDiv(lv,binaryExprTree -> right -> ir,"div");
                        Builder.CreateStore(temp,binaryExprTree -> left -> ir);
                        break;
                    }
                    case OP_PERCENTEQUAL:
                    {
                        binaryExprTree -> left -> accept(this);
                        binaryExprTree -> right -> accept(this);
                        llvm::Value * lv = Builder.CreateLoad(binaryExprTree -> left -> ir);
                        llvm::Value * temp = Builder.CreateSRem(lv,binaryExprTree -> right -> ir,"rem");
                        Builder.CreateStore(temp,binaryExprTree -> left -> ir);
                        break;
                    }
                    case OP_AMPEQUAL:
                    {
                        binaryExprTree -> left -> accept(this);
                        binaryExprTree -> right -> accept(this);
                        llvm::Value * lv = Builder.CreateLoad(binaryExprTree -> left -> ir);
                        llvm::Value * temp = Builder.CreateAnd(lv,binaryExprTree -> right -> ir,"and");
                        Builder.CreateStore(temp,binaryExprTree -> left -> ir);
                        break;
                    }
                    case OP_CARETEQUAL:
                    {
                        binaryExprTree -> left -> accept(this);
                        binaryExprTree -> right -> accept(this);
                        llvm::Value * lv = Builder.CreateLoad(binaryExprTree -> left -> ir);
                        llvm::Value * temp = Builder.CreateXor(lv,binaryExprTree -> right -> ir,"xor");
                        Builder.CreateStore(temp,binaryExprTree -> left -> ir);
                        break;
                    }
                    case OP_PIPEEQUAL:
                    {
                        binaryExprTree -> left -> accept(this);
                        binaryExprTree -> right -> accept(this);
                        llvm::Value * lv = Builder.CreateLoad(binaryExprTree -> left -> ir);
                        llvm::Value * temp = Builder.CreateOr(lv,binaryExprTree -> right -> ir,"or");
                        Builder.CreateStore(temp,binaryExprTree -> left -> ir);
                        break;
                    }
                    case OP_LESSLESSEQUAL:
                    {
                        binaryExprTree -> left -> accept(this);
                        binaryExprTree -> right -> accept(this);
                        llvm::Value * lv = Builder.CreateLoad(binaryExprTree -> left -> ir);
                        llvm::Value * temp = Builder.CreateShl(lv,binaryExprTree -> right -> ir,"shl");
                        Builder.CreateStore(temp,binaryExprTree -> left -> ir);
                        break;
                    }
                    case OP_GREATERGREATEREQUAL:
                    {
                        binaryExprTree -> left -> accept(this);
                        binaryExprTree -> right -> accept(this);
                        llvm::Value * lv = Builder.CreateLoad(binaryExprTree -> left -> ir);
                        llvm::Value * temp = Builder.CreateLShr(lv,binaryExprTree -> right -> ir,"shr");
                        Builder.CreateStore(temp,binaryExprTree -> left -> ir);
                        break;
                    }
                }
            }
            else
            {
                switch(binaryExprTree -> opcode)
                {
                    case OP_STAR:
                    {
                        binaryExprTree -> left -> accept(this);
                        binaryExprTree -> right -> accept(this);
                        if(!(binaryExprTree -> left -> ir) || !(binaryExprTree -> right -> ir))
                        {
                            binaryExprTree -> ir = nullptr;
                            return;
                        }
                        binaryExprTree -> ir = Builder.CreateFMul(binaryExprTree -> left -> ir,binaryExprTree -> right -> ir,"fmul");
                        break;
                    }
                    case OP_SLASH:
                    {
                        binaryExprTree -> left -> accept(this);
                        binaryExprTree -> right -> accept(this);
                        if(!(binaryExprTree -> left -> ir) || !(binaryExprTree -> right -> ir))
                        {
                            binaryExprTree -> ir = nullptr;
                            return;
                        }
                        binaryExprTree -> ir = Builder.CreateFDiv(binaryExprTree -> left -> ir,binaryExprTree -> right -> ir,"fdiv");
                        break;
                    } 
                    case OP_PLUS:
                    {
                        binaryExprTree -> left -> accept(this);
                        binaryExprTree -> right -> accept(this);
                        if(!(binaryExprTree -> left -> ir) || !(binaryExprTree -> right -> ir))
                        {
                            binaryExprTree -> ir = nullptr;
                            return;
                        }
                        binaryExprTree -> ir = Builder.CreateFAdd(binaryExprTree -> left -> ir,binaryExprTree -> right -> ir,"fadd");
                        break;
                    } 
                    case OP_MINUS:
                    {
                        binaryExprTree -> left -> accept(this);
                        binaryExprTree -> right -> accept(this);
                        if(!(binaryExprTree -> left -> ir) || !(binaryExprTree -> right -> ir))
                        {
                            binaryExprTree -> ir = nullptr;
                            return;
                        }
                        binaryExprTree -> ir = Builder.CreateFSub(binaryExprTree -> left -> ir,binaryExprTree -> right -> ir,"fsub");
                        break;
                    }     
                    case OP_COMMA:
                    {
                        binaryExprTree -> left -> accept(this);
                        binaryExprTree -> right -> accept(this);
                        if(!(binaryExprTree -> left -> ir) || !(binaryExprTree -> right -> ir))
                        {
                            binaryExprTree -> ir = nullptr;
                            return;
                        }
                        binaryExprTree -> ir = binaryExprTree -> right -> ir;
                        break;
                    }
                    case OP_EQUAL:
                    {
                        binaryExprTree -> left -> accept(this);

                        llvm::Value * left = binaryExprTree -> left -> ir;
                        binaryExprTree -> right -> accept(this);
                        Builder.CreateStore(binaryExprTree -> right -> ir,left);
                        break;
                    }
                    case OP_PLUSEQUAL:
                    {
                        binaryExprTree -> left -> accept(this);
                        binaryExprTree -> right -> accept(this);
                        llvm::Value * lv = Builder.CreateLoad(binaryExprTree -> left -> ir);
                        llvm::Value * temp = Builder.CreateFAdd(lv,binaryExprTree -> right -> ir,"add");
                        Builder.CreateStore(temp,binaryExprTree -> left -> ir);
                        break;
                    }
                    case OP_MINUSEQUAL:
                    {
                        binaryExprTree -> left -> accept(this);
                        binaryExprTree -> right -> accept(this);
                        llvm::Value * lv = Builder.CreateLoad(binaryExprTree -> left -> ir);
                        llvm::Value * temp = Builder.CreateFSub(lv,binaryExprTree -> right -> ir,"sub");
                        Builder.CreateStore(temp,binaryExprTree -> left -> ir);
                        break;
                    }
                    case OP_STAREQUAL:
                    {
                        binaryExprTree -> left -> accept(this);
                        binaryExprTree -> right -> accept(this);
                        llvm::Value * lv = Builder.CreateLoad(binaryExprTree -> left -> ir);
                        llvm::Value * temp = Builder.CreateFMul(lv,binaryExprTree -> right -> ir,"mul");
                        Builder.CreateStore(temp,binaryExprTree -> left -> ir);
                        break;
                    }
                    case OP_SLASHEQUAL:
                    {
                        binaryExprTree -> left -> accept(this);
                        binaryExprTree -> right -> accept(this);
                        llvm::Value * lv = Builder.CreateLoad(binaryExprTree -> left -> ir);
                        llvm::Value * temp = Builder.CreateFDiv(lv,binaryExprTree -> right -> ir,"div");
                        Builder.CreateStore(temp,binaryExprTree -> left -> ir);
                        break;
                    }
                }
            }
        }            
        virtual void VisitTernaryExprTree(TernaryExprTree * ternaryExprTree)
        {
            
        }
        virtual void VisitFuncCallTree(FuncCallTree * funcCallTree)
        {
            ImplicitCastExprTree * impl = dynamic_cast<ImplicitCastExprTree *> (funcCallTree ->iden);
            DeclRefTree * ref = dynamic_cast<DeclRefTree *>(impl -> cast);
            llvm::Function * F = dynamic_cast<FuncDeclTree *>(ref -> decl) -> funclr; 

            std::vector<llvm::Value *> ArgsV;
            llvm::Value * Arg;
            bool useGEP;
            ImplicitCastExprTree * p;
            for (unsigned i = 0, e = funcCallTree -> para.size(); i != e; ++i)
            {
                funcCallTree -> para[i] -> accept(this);
                useGEP = funcCallTree -> para[i] -> info.isPointer && !((p = dynamic_cast<ImplicitCastExprTree *>(funcCallTree -> para[i])) && p -> castkind == "LValueToRValue" && dynamic_cast<DeclRefTree *>(p -> cast));
                if(useGEP)
                    Arg = Builder.CreateInBoundsGEP(funcCallTree -> para[i] -> ir, {llvm::ConstantInt::get(TheContext, llvm::APInt(32, 0)), llvm::ConstantInt::get(TheContext, llvm::APInt(32, 0))});
                else
                    Arg = funcCallTree -> para[i] -> ir;
                ArgsV.emplace_back(Arg);
            }

            funcCallTree -> ir = Builder.CreateCall(F, ArgsV);
        }
        virtual void VisitImplicitValueInitExprTree(ImplicitValueInitExprTree * implicitValueInitExprTree)
        {
            
        }
};