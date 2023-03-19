#include <vector>

#include <cstdio>
#include <unordered_map>
#include <llvm/Support/JSON.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/raw_ostream.h>

const char * typestr[6]={"char","int","long long","float","double","void"};



enum Type
{
    Char,Int,Longlong,Float,Double,Void
};


const char * opstr[34]=
{   "++","--","+","-","!","~","*","/","%","<<",">>",
    "<",">","<=",">=","==","!=","&","^","|","&&","||",",",
    "=","+=","-=","*=","/=","%=","&=","^=","|=","<<=",">>="
};

class Layer
{
    public:
        int num = 0;
        std::vector<int> len;
        void emplace_back(int n = 0)
        {
            num++;
            len.emplace_back(n);
        }
        int pop_back()
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
        bool isConst;
        std::vector<Info> args;
        Info(enum Type t = Void,bool ic = 0):
        type(t),isConst(ic){}
};

typedef std::unordered_map<std::string,Info> SymT;
SymT symt;
Info curret;

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
};

class Tree
{
    public:
        std::string kind;
        std::string name;
        std::string value;
        llvm::json::Object * temp;
        Tree(std::string kind="", std::string name="", std::string value=""): 
            kind(kind), name(name), value(value) {} 
        bool forceRight(ExprTree *& ptr);
        virtual void accept( Visitor* visitor ) {}
        virtual void SematicAnalysis(){};
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
        ExprTree * expr;
};

class BlockStmtTree: public StmtTree
{
    public:
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
        ExprTree * cond;
        StmtTree * truestmt;
        StmtTree * falsestmt;

};

class LoopStmtTree: public StmtTree
{
    public:
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
        ExprTree * cond;
        StmtTree * stmt;
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
        StmtTree * stmt;
        ExprTree * cond;
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
        ExprTree * init;
        ExprTree * null;
        ExprTree * cond;
        ExprTree * incr;
        StmtTree * stmt;
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
        LoopStmtTree * loop;
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
        LoopStmtTree * loop;
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
        FuncDeclTree * func;
        ExprTree * expr;
};

class TranslationUnitTree: public Tree
{
    public:
        std::vector<Tree *> sons;
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
};

class DeclTree :public Tree
{
    public:
        Info info;
        DeclTree (std::string kind="", std::string name="", std::string value=""):
            Tree(kind,name,value){}

};

class VarDeclTree : public DeclTree
{
    public:
        ExprTree * initval;
        VarDeclTree (std::string kind="", std::string name="", std::string value=""):
            DeclTree(kind,name,value){}
        virtual void accept( Visitor* visitor )
        {
            visitor -> VisitVarDeclTree(this);
        }
};

class FuncDeclTree: public DeclTree
{
    public:    
        std::vector<VarDeclTree *> sons;
        BlockStmtTree * body;
        FuncDeclTree (std::string kind="", std::string name="", std::string value=""):
            DeclTree(kind,name,value){}
        virtual void accept( Visitor* visitor )
        {
            visitor -> VisitFuncDeclTree(this);
        }
        void GetParaType()
        {
            for(auto son:sons)
                info.args.emplace_back(son->info);   
        }
        bool addSon(VarDeclTree* son)
        {   
            bool retval = 0;
            sons.emplace_back(son);
            return retval;
        }
        
};




class ExprTree : public Tree
{
    public:
        Info info;
        bool isLeftVal;
        ExprTree (std::string kind="", std::string name="", std::string value=""):
            Tree(kind,name,value){}
};

class LiteralTree : public ExprTree
{
    public:
        LiteralTree(std::string kind="", std::string name="", std::string value=""):
            ExprTree(kind,name,value){}
        virtual void accept( Visitor* visitor )
        {
            visitor -> VisitLiteralTree(this);
        }
};

class ParenExprTree : public ExprTree
{
    public:
        ExprTree * son;
        ParenExprTree(std::string kind="", std::string name="", std::string value=""):
            ExprTree(kind,name,value){}
        virtual void accept( Visitor* visitor )
        {
            visitor -> VisitParenExprTree(this);
        }
        
};

class InitListExprTree :public ExprTree
{
    public:    
        InitListExprTree(std::string kind="", std::string name="", std::string value=""):
            ExprTree(kind,name,value){}
        virtual void accept( Visitor* visitor )
        {
            visitor -> VisitInitListExprTree(this);
        }
};

class ImplicitCastExprTree : public ExprTree 
{
    public:
        ExprTree * cast;
        std::string castkind;
        ImplicitCastExprTree(std::string kind="", std::string name="", std::string value=""):
            ExprTree(kind,name,value){}
        virtual void accept( Visitor* visitor )
        {
            visitor -> VisitImplicitCastExprTree(this);
        }
        
};

class ArraySubscriptExprTree:public ExprTree
{
    public:
        ExprTree * sym;
        ExprTree * script;
        ArraySubscriptExprTree(std::string kind="", std::string name="", std::string value=""):
                ExprTree(kind,name,value){}
        virtual void accept( Visitor* visitor )
        {
            visitor -> VisitArraySubscriptExprTree(this);
        }

};

class DeclRefTree : public ExprTree
{
    public:
        DeclTree * decl;
        DeclRefTree(std::string kind="", std::string name="", std::string value=""):
                ExprTree(kind,name,value){}
        virtual void accept( Visitor* visitor )
        {
            visitor -> VisitDeclRefTree(this);
        }
};

class UnaryExprTree : public ExprTree
{
    public:
        OpType opcode;
        bool isPostfix;
        ExprTree * son;
        UnaryExprTree(std::string kind="", std::string name="", std::string value=""):
                ExprTree(kind,name,value){}
        virtual void accept( Visitor* visitor )
        {
            visitor -> VisitUnaryExprTree(this);
        }
};

class BinaryExprTree : public ExprTree
{
    public:
        OpType opcode;
        ExprTree * left;
        ExprTree * right;
        BinaryExprTree(std::string kind="", std::string name="", std::string value=""):
                ExprTree(kind,name,value){}
        virtual void accept( Visitor* visitor )
        {
            visitor -> VisitBinaryExprTree(this);
        }
};

class TernaryExprTree : public ExprTree
{
    public:
        ExprTree * condexp;
        ExprTree * trueexp;
        ExprTree * falseexp;
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
        ExprTree * iden;
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



class Print: public Visitor
{
    private:
        void printbasic(Tree * tree)
        {
            yyerror("|");
            for(int i = 0;i < depth; ++i) 
                yyerror(" ");
            yyerror("-" + tree -> kind + " " + tree -> name + " " + tree -> value);
            yyerror("\n");
        }
    int depth = 0;
    public:
        virtual void VisitTranslationUnitTree(TranslationUnitTree *  translationUnitTree)
        {
            depth ++;
            printbasic(translationUnitTree);
            for(auto && it :translationUnitTree -> sons)
                it -> accept(this);
            depth --;
        }
        virtual void VisitVarDeclTree(VarDeclTree * varDeclTree)
        {
            depth ++;
            printbasic(varDeclTree);
            if(varDeclTree -> initval)
                varDeclTree -> initval -> accept(this);
            depth --;
        }
        virtual void VisitFuncDeclTree(FuncDeclTree * funcDeclTree)
        {
            depth ++;
            printbasic(funcDeclTree);
            for(auto && it :funcDeclTree -> sons)
                it -> accept(this);
            if(funcDeclTree -> body)
                funcDeclTree -> body -> accept(this);
            depth --;
        }
        virtual void VisitDeclStmtTree(DeclStmtTree * declStmtTree)
        {
            depth ++;
            printbasic(declStmtTree);
            for(auto && it :declStmtTree -> sons)
                it -> accept(this);
            depth --;
        }
        virtual void VisitNullStmtTree(NullStmtTree * nullStmtTree)
        {
            depth ++;
            printbasic(nullStmtTree);
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
            for(auto && it :blockStmtTree -> sons)
                it -> accept(this);
            depth --;
        }
        virtual void VisitIfStmtTree(IfStmtTree * ifStmtTree)
        {
            depth ++;
            printbasic(ifStmtTree);
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
            whileStmtTree -> cond -> accept(this);
            whileStmtTree -> stmt -> accept(this);
            depth --;
        }
        virtual void VisitDoStmtTree(DoStmtTree * doStmtTree)
        {
            depth ++;
            printbasic(doStmtTree);
            doStmtTree -> stmt -> accept(this);
            doStmtTree -> cond -> accept(this);
            depth --;
        }
        virtual void VisitForStmtTree(ForStmtTree * forStmtTree)
        {
            depth ++;
            printbasic(forStmtTree);
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
            depth --;
        }
        virtual void VisitBreakStmtTree(BreakStmtTree * breakStmtTree)
        {
            depth ++;
            printbasic(breakStmtTree);
            depth --;            
        }
        virtual void VisitReturnStmtTree(ReturnStmtTree * returnStmtTree)
        {
            depth ++;
            printbasic(returnStmtTree);
            if(returnStmtTree -> expr)
                returnStmtTree -> expr -> accept(this);
            depth --;
        }
        virtual void VisitLiteralTree(LiteralTree * literalTree)
        {
            depth ++;
            printbasic(literalTree);
            depth --;  
        }
        virtual void VisitParenExprTree(ParenExprTree * parenExprTree)
        {
            depth ++;
            printbasic(parenExprTree);
            parenExprTree -> son -> accept(this);
            depth --;
        }
        virtual void VisitInitListExprTree(InitListExprTree * initListExprTree)
        {
            depth ++;
            printbasic(initListExprTree);
            depth --;  
        }
        virtual void VisitImplicitCastExprTree(ImplicitCastExprTree * implicitCastExprTree)
        {
            depth ++;
            printbasic(implicitCastExprTree);
            implicitCastExprTree -> cast -> accept(this);
            depth --;
        }
        virtual void VisitArraySubscriptExprTree(ArraySubscriptExprTree * arraySubscriptExprTree)
        {
            depth ++;
            printbasic(arraySubscriptExprTree);
            arraySubscriptExprTree -> sym -> accept(this);
            arraySubscriptExprTree -> script -> accept(this);
            depth --;
        }
        virtual void VisitDeclRefTree(DeclRefTree * declRefTree)
        {
            depth ++;
            printbasic(declRefTree);
            depth --; 
        }
        virtual void VisitUnaryExprTree(UnaryExprTree * unaryExprTree)
        {
            depth ++;
            printbasic(unaryExprTree);
            unaryExprTree -> son -> accept(this);
            depth --;
        }
        virtual void VisitBinaryExprTree(BinaryExprTree * binaryExprTree)
        {   
            depth ++;
            printbasic(binaryExprTree);
            binaryExprTree -> left -> accept(this);
            binaryExprTree -> right -> accept(this);
            depth --;
        }
        virtual void VisitTernaryExprTree(TernaryExprTree * ternaryExprTree)
        {
            depth ++;
            printbasic(ternaryExprTree);
            ternaryExprTree -> condexp -> accept(this);
            ternaryExprTree -> trueexp -> accept(this);
            ternaryExprTree -> falseexp -> accept(this);
            depth --;
        }
        virtual void VisitFuncCallTree(FuncCallTree * funcCallTree)
        {
            depth ++;
            printbasic(funcCallTree);
            funcCallTree -> iden -> accept(this);
            for(auto && it :funcCallTree -> para)
                it -> accept(this);
            depth --;
        }
};

/*

llvm::json::Value toJson() const 
    {
        llvm::json::Object tmp{
        {"kind", kind},
        {"name", name},
        {"value", value},
        {"type",GetType()},
        //{"opcode",opstr[opcode]},
        {"z-childs",GetChild()},
        {"lVal",(isLeftVal ? "True" : "False")},
        {"inner", llvm::json::Array{}}
        };
        for(auto&& it: sons) tmp.get("inner")->getAsArray()->push_back(it->toJson());
        return tmp;
    }

*/

class ToJson:public Visitor
{
    public:
        virtual void VisitTranslationUnitTree(TranslationUnitTree *  translationUnitTree)
        {
            translationUnitTree -> temp = new llvm::json::Object
            {
                {"kind", translationUnitTree -> kind},
                {"name", translationUnitTree -> name},
                {"value", translationUnitTree -> value},
                {"inner", llvm::json::Array{}}
            };
            for(auto&& it: translationUnitTree -> sons) 
            {
                it -> accept(this);
                translationUnitTree -> temp -> get("inner")->getAsArray()->push_back(std::move(*(it->temp)));
            }
                
        }
        virtual void VisitVarDeclTree(VarDeclTree * varDeclTree)
        {
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
                varDeclTree -> temp -> get("inner")->getAsArray()->push_back(std::move(*(varDeclTree -> initval -> temp)));
            }
        }
        virtual void VisitFuncDeclTree(FuncDeclTree * funcDeclTree)
        {
            funcDeclTree -> temp = new llvm::json::Object
            {
                {"kind", funcDeclTree -> kind},
                {"name", funcDeclTree -> name},
                {"value", funcDeclTree -> value},
                {"inner", llvm::json::Array{}}
            };
            for(auto && it :funcDeclTree -> sons)
            {   
                it -> accept(this);
                funcDeclTree -> temp -> get("inner")->getAsArray()->push_back(std::move(*(it->temp)));
            }
            if(funcDeclTree -> body)
            {   
                funcDeclTree -> body -> accept(this);
                funcDeclTree -> temp -> get("inner")->getAsArray()->push_back(std::move(*(funcDeclTree -> body -> temp)));
            }
        }
        virtual void VisitDeclStmtTree(DeclStmtTree * declStmtTree)
        {
            declStmtTree -> temp = new llvm::json::Object
            {
                {"kind", declStmtTree -> kind},
                {"name", declStmtTree -> name},
                {"value", declStmtTree -> value},
                {"inner", llvm::json::Array{}}
            };
            for(auto && it :declStmtTree -> sons)
            {   
                it -> accept(this);
                declStmtTree -> temp -> get("inner")->getAsArray()->push_back(std::move(*(it->temp)));
            }
        }
        virtual void VisitNullStmtTree(NullStmtTree * nullStmtTree)
        {
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
            exprStmtTree -> expr -> accept(this);
            exprStmtTree -> temp = exprStmtTree -> expr -> temp;
        }
        virtual void VisitBlockStmtTree(BlockStmtTree * blockStmtTree)
        {
            blockStmtTree -> temp = new llvm::json::Object
            {
                {"kind", blockStmtTree -> kind},
                {"name", blockStmtTree -> name},
                {"value", blockStmtTree -> value},
                {"inner", llvm::json::Array{}}
            };
            for(auto && it :blockStmtTree -> sons)
            {   
                it -> accept(this);
                blockStmtTree -> temp -> get("inner")->getAsArray()->push_back(std::move(*(it->temp)));
            }
           
        }
        virtual void VisitIfStmtTree(IfStmtTree * ifStmtTree)
        {
            ifStmtTree -> temp = new llvm::json::Object
            {
                {"kind", ifStmtTree -> kind},
                {"name", ifStmtTree -> name},
                {"value", ifStmtTree -> value},
                {"inner", llvm::json::Array{}}
            };
            ifStmtTree -> cond -> accept(this);
            ifStmtTree -> temp -> get("inner")->getAsArray()->push_back(std::move(*(ifStmtTree -> cond->temp)));
            ifStmtTree -> truestmt -> accept(this);
            ifStmtTree -> temp -> get("inner")->getAsArray()->push_back(std::move(*(ifStmtTree -> truestmt->temp)));
            if(ifStmtTree -> falsestmt)
            {   
                ifStmtTree -> falsestmt -> accept(this);
                ifStmtTree -> temp -> get("inner")->getAsArray()->push_back(std::move(*(ifStmtTree -> falsestmt->temp)));
            }
            
        }
        virtual void VisitWhileStmtTree(WhileStmtTree * whileStmtTree)
        {
            whileStmtTree -> temp = new llvm::json::Object
            {
                {"kind", whileStmtTree -> kind},
                {"name", whileStmtTree -> name},
                {"value", whileStmtTree -> value},
                {"inner", llvm::json::Array{}}
            };
            whileStmtTree -> cond -> accept(this);
            whileStmtTree -> temp -> get("inner")->getAsArray()->push_back(std::move(*(whileStmtTree -> cond->temp)));
            whileStmtTree -> stmt -> accept(this);
            whileStmtTree -> temp -> get("inner")->getAsArray()->push_back(std::move(*(whileStmtTree -> stmt->temp)));
        }
        virtual void VisitDoStmtTree(DoStmtTree * doStmtTree)
        {
            doStmtTree -> temp = new llvm::json::Object
            {
                {"kind", doStmtTree -> kind},
                {"name", doStmtTree -> name},
                {"value", doStmtTree -> value},
                {"inner", llvm::json::Array{}}
            };
            doStmtTree -> stmt -> accept(this);
            doStmtTree -> temp -> get("inner")->getAsArray()->push_back(std::move(*(doStmtTree -> cond->temp)));
            doStmtTree -> cond -> accept(this);
            doStmtTree -> temp -> get("inner")->getAsArray()->push_back(std::move(*(doStmtTree -> cond->temp)));
        }
        virtual void VisitForStmtTree(ForStmtTree * forStmtTree)
        {
            forStmtTree -> temp = new llvm::json::Object
            {
                {"kind", forStmtTree -> kind},
                {"name", forStmtTree -> name},
                {"value", forStmtTree -> value},
                {"inner", llvm::json::Array{}}
            };
            forStmtTree -> init -> accept(this);
            forStmtTree -> temp -> get("inner")->getAsArray()->push_back(std::move(*(forStmtTree -> init -> temp)));
            forStmtTree -> temp -> get("inner")->getAsArray()->push_back(llvm::json::Object{{}});
            forStmtTree -> cond -> accept(this);
            forStmtTree -> temp -> get("inner")->getAsArray()->push_back(std::move(*(forStmtTree -> cond -> temp)));
            forStmtTree -> incr -> accept(this);
            forStmtTree -> temp -> get("inner")->getAsArray()->push_back(std::move(*(forStmtTree -> incr -> temp)));
            forStmtTree -> stmt -> accept(this);
            forStmtTree -> temp -> get("inner")->getAsArray()->push_back(std::move(*(forStmtTree -> stmt -> temp)));
        }
        virtual void VisitContinueStmtTree(ContinueStmtTree * continueStmtTree)
        {
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
                returnStmtTree -> temp -> get("inner")->getAsArray()->push_back(std::move(*(returnStmtTree -> expr -> temp)));
            }
        }
        virtual void VisitLiteralTree(LiteralTree * literalTree)
        {
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
            parenExprTree -> temp = new llvm::json::Object
            {
                {"kind", parenExprTree -> kind},
                {"name", parenExprTree -> name},
                {"value", parenExprTree -> value},
                {"inner", llvm::json::Array{}}
            }; 
            parenExprTree -> son -> accept(this);
            parenExprTree -> temp -> get("inner")->getAsArray()->push_back(std::move(*(parenExprTree -> son -> temp)));
        }
        virtual void VisitInitListExprTree(InitListExprTree * initListExprTree)
        {
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
            implicitCastExprTree -> temp = new llvm::json::Object
            {
                {"kind", implicitCastExprTree -> kind},
                {"name", implicitCastExprTree -> name},
                {"value", implicitCastExprTree -> value},
                {"inner", llvm::json::Array{}}
            };  
            implicitCastExprTree -> cast -> accept(this);
            implicitCastExprTree -> temp -> get("inner")->getAsArray()->push_back(std::move(*(implicitCastExprTree -> cast -> temp)));
        }
        virtual void VisitArraySubscriptExprTree(ArraySubscriptExprTree * arraySubscriptExprTree)
        {
            arraySubscriptExprTree -> temp = new llvm::json::Object
            {
                {"kind", arraySubscriptExprTree -> kind},
                {"name", arraySubscriptExprTree -> name},
                {"value", arraySubscriptExprTree -> value},
                {"inner", llvm::json::Array{}}
            };  
            arraySubscriptExprTree -> sym -> accept(this);
            arraySubscriptExprTree -> temp -> get("inner")->getAsArray()->push_back(std::move(*( arraySubscriptExprTree -> sym -> temp)));
            arraySubscriptExprTree -> script -> accept(this);
            arraySubscriptExprTree -> temp -> get("inner")->getAsArray()->push_back(std::move(*(arraySubscriptExprTree -> script -> temp)));
        }
        virtual void VisitDeclRefTree(DeclRefTree * declRefTree)
        {
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
            unaryExprTree -> temp = new llvm::json::Object
            {
                {"kind", unaryExprTree -> kind},
                {"name", unaryExprTree -> name},
                {"value", unaryExprTree -> value},
                {"inner", llvm::json::Array{}}
            }; 
            unaryExprTree -> son -> accept(this);
            unaryExprTree -> temp -> get("inner")->getAsArray()->push_back(std::move(*(unaryExprTree -> son -> temp)));
        }
        virtual void VisitBinaryExprTree(BinaryExprTree * binaryExprTree)
        {   
            binaryExprTree -> temp = new llvm::json::Object
            {
                {"kind", binaryExprTree -> kind},
                {"name", binaryExprTree -> name},
                {"value", binaryExprTree -> value},
                {"inner", llvm::json::Array{}}
            }; 
            binaryExprTree -> left -> accept(this);
            binaryExprTree -> temp -> get("inner")->getAsArray()->push_back(std::move(*(binaryExprTree -> left -> temp)));
            binaryExprTree -> right -> accept(this);
            binaryExprTree -> temp -> get("inner")->getAsArray()->push_back(std::move(*(binaryExprTree -> right -> temp)));
    
        }
        virtual void VisitTernaryExprTree(TernaryExprTree * ternaryExprTree)
        {
            ternaryExprTree -> temp = new llvm::json::Object
            {
                {"kind", ternaryExprTree -> kind},
                {"name", ternaryExprTree -> name},
                {"value", ternaryExprTree -> value},
                {"inner", llvm::json::Array{}}
            }; 
            ternaryExprTree -> condexp -> accept(this);
            ternaryExprTree -> temp -> get("inner")->getAsArray()->push_back(std::move(*(ternaryExprTree -> condexp -> temp)));
            ternaryExprTree -> trueexp -> accept(this);
            ternaryExprTree -> temp -> get("inner")->getAsArray()->push_back(std::move(*(ternaryExprTree -> trueexp -> temp)));
            ternaryExprTree -> falseexp -> accept(this);
            ternaryExprTree -> temp -> get("inner")->getAsArray()->push_back(std::move(*(ternaryExprTree -> falseexp -> temp)));
        }
        virtual void VisitFuncCallTree(FuncCallTree * funcCallTree)
        {
            funcCallTree -> temp = new llvm::json::Object
            {
                {"kind", funcCallTree -> kind},
                {"name", funcCallTree -> name},
                {"value", funcCallTree -> value},
                {"inner", llvm::json::Array{}}
            }; 
            funcCallTree -> iden -> accept(this);
            funcCallTree -> temp -> get("inner")->getAsArray()->push_back(std::move(*(funcCallTree -> iden -> temp)));
            for(auto && it :funcCallTree -> para)
            {   
                it -> accept(this);
                funcCallTree -> temp -> get("inner")->getAsArray()->push_back(std::move(*(it->temp)));
            }
        }
};

/*class Tree
{
    public:
        
    
    OpType opcode;
    
    Info info;
    
    isLeftVal(isLeft),kind(kind), name(name), value(value),info(Info()) {}
    bool addSon(bool isLeft,Tree* son)
    {   
        bool retval = 0;
        if(son != nullptr)
        {
           
            sons.emplace_back(son);
            retval = son -> isLeftVal;
            isLeftVal &= son -> isLeftVal;
        }   
        else 
            sons.emplace_back(son);
        return retval;
    }
    
    void TryUpcast(Tree * ptr)
    {
        if(ptr -> sons[0] -> info.type == ptr -> sons[1] -> info.type)
        {
            ptr -> info.type = ptr -> sons[0] -> info.type;
            ptr -> info.layer = ptr -> sons[0] -> info.layer;
        }
        else if(ptr -> sons[0] -> info.type > ptr -> sons[1] -> info.type)
        {
            auto tmp = new Tree(0,"ImplicitCastExpr");
            tmp -> sons.emplace_back(ptr -> sons[1]);
            tmp -> isLeftVal = ptr -> sons[1] -> isLeftVal;
            tmp -> info.type = ptr -> sons[0] -> info.type;
            tmp -> info.layer = ptr -> sons[0] -> info.layer;
            ptr -> sons[1] = tmp; 
            ptr -> info.type = ptr -> sons[0] -> info.type;
            ptr -> info.layer = ptr -> sons[0] -> info.layer;
        }
        else
        {
            auto tmp = new Tree(0,"ImplicitCastExpr");
            tmp->sons.emplace_back(ptr -> sons[0]);
            tmp -> isLeftVal = ptr -> sons[0] -> isLeftVal;
            tmp -> info.type = ptr -> sons[1] -> info.type;
            tmp -> info.layer = ptr -> sons[1] -> info.layer;
            ptr -> sons[0] = tmp; 
            ptr -> info.type = ptr -> sons[1] -> info.type;
            ptr -> info.layer = ptr -> sons[1] -> info.layer;
        }
    }
    void AssignAdjust(Tree * ptr)
    {
        if(ptr -> sons[0] -> info.type != ptr -> sons[1] -> info.type)
        {
            auto tmp = new Tree(0,"ImplicitCastExpr");
            tmp -> sons.emplace_back(ptr -> sons[1]);
            tmp -> isLeftVal = ptr -> sons[1] -> isLeftVal;
            tmp -> info.type = ptr -> sons[0] -> info.type;
            tmp -> info.layer = ptr -> sons[0] -> info.layer;
            ptr -> sons[1] = tmp; 
            ptr -> info.type = ptr -> sons[0] -> info.type;
            ptr -> info.layer = ptr -> sons[0] -> info.layer;
        }
    }
    void DeclAdjust(Tree * ptr)
    {
        if(ptr -> info.type != ptr -> sons[0] -> info.type)
        {
            auto tmp = new Tree(0,"ImplicitCastExpr");
            tmp -> sons.emplace_back(ptr -> sons[0]);
            tmp -> isLeftVal = ptr -> sons[0] -> isLeftVal;
            tmp -> info.type = ptr -> info.type;
            tmp -> info.layer = ptr -> info.layer;
            ptr -> sons[0] = tmp; 
        }
    }
    void RetAdjust(Tree * ptr)
    {
        if(curret.type != ptr -> sons[0] -> info.type)
        {
            auto tmp = new Tree(0,"ImplicitCastExpr");
            tmp -> sons.emplace_back(ptr -> sons[0]);
            tmp -> isLeftVal = ptr -> sons[0] -> isLeftVal;
            tmp -> info.type = curret.type;
            tmp -> info.layer = curret.layer;
            ptr -> sons[0] = tmp; 
        }
    }
    
    std::string GetType() const 
    {   
        std::string s;
        if(info.isConst)
            s += "const ";
        s += typestr[info.type];
        if(info.args.size())
        {
            s += '(';
            for(auto para : info.args)
            {   
                if(para.isConst)
                    s += "const "; 
                s += typestr[para.type];
                s += ',';
            }
            s += ')';
        }
        return s;
    }

    std::string GetChild() const {return std::string(sons.size(),'*');}
    
    
    void AdjustParaType(Tree * ptr)
    {
        int size = ptr -> info.args.size();
        for(int i = 0;i < size;i++)
        {
            
            if(ptr -> info.args[i].layer.num > 0 )
            {
                auto tmp = new Tree(1,"ImplicitCastExpr");
                tmp -> sons.emplace_back(ptr -> sons[i + 1]);
                tmp -> isLeftVal = ptr -> sons[i + 1] -> isLeftVal;
                tmp -> info.type = ptr -> sons[i + 1] ->info.type;
                tmp -> info.layer = ptr -> sons[i + 1] ->info.layer;
                tmp -> info.isConst = ptr -> sons[i + 1] ->info.isConst;
                ptr -> sons[i + 1] = tmp; 

                if(ptr -> info.args[i].type != ptr -> sons[i + 1] -> info.type)
                {
                    auto tmp = new Tree(1,"ImplicitCastExpr");
                    tmp -> sons.emplace_back(ptr -> sons[i + 1]);
                    tmp -> isLeftVal = ptr -> sons[i + 1] -> isLeftVal;
                    tmp -> info.type = ptr -> info.type;
                    tmp -> info.layer = ptr -> info.layer;
                    tmp -> info.isConst = ptr -> info.isConst;
                    ptr -> sons[i + 1] = tmp; 
                }
                
                else if(ptr -> info.args[i].isConst && !(ptr -> sons[i + 1] -> info.isConst))
                {
                    auto tmp = new Tree(1,"ImplicitCastExpr");
                    tmp -> sons.emplace_back(ptr -> sons[i + 1]);
                    tmp -> isLeftVal = ptr -> sons[i + 1] -> isLeftVal;
                    tmp -> info.type = ptr -> info.type;
                    tmp -> info.layer = ptr -> info.layer;
                    tmp -> info.isConst = 1;
                    ptr -> sons[i + 1] = tmp; 
                }
            }
            if(ptr -> info.args[i].type != ptr -> sons[i + 1] -> info.type)
            {
                auto tmp = new Tree(0,"ImplicitCastExpr");
                tmp -> sons.emplace_back(ptr -> sons[i + 1]);
                tmp -> isLeftVal = ptr -> sons[i + 1] -> isLeftVal;
                tmp -> info.type = ptr -> info.type;
                tmp -> info.layer = ptr -> info.layer;
                tmp -> info.isConst = ptr -> info.isConst;
                ptr -> sons[i + 1] = tmp; 
            }
            
        }
    }
};
*/