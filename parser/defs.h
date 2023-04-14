#include <vector>
#include <stack>
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
const char * opstr[34]=
{   "++","--","+","-","!","~","*","/","%","<<",">>",
    "<",">","<=",">=","==","!=","&","^","|","&","||",",",
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
        ExprTree * expr;
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
        ExprTree * cond;
        StmtTree * truestmt;
        StmtTree * falsestmt;
        virtual void SematicAnalysis();

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
        StmtTree * stmt;
        ExprTree * cond;
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
        ExprTree * init;
        ExprTree * null;
        ExprTree * cond;
        ExprTree * incr;
        StmtTree * stmt;
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
        LoopStmtTree * loop;
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
        LoopStmtTree * loop;
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
        FuncDeclTree * func;
        ExprTree * expr;
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
        ExprTree * initval;
        std::vector<ExprTree *> * dims;
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
        BlockStmtTree * body;
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
        virtual long long GetValue(){}
        virtual std::string getinfo()
        {
            std::string str;
            if(info.isConst)
                str += "const ";
            str += typestr[info.type];
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
        LiteralTree(std::string kind="", std::string name="", std::string value=""):
            ExprTree(kind,name,value){}
        virtual void accept( Visitor* visitor )
        {
            visitor -> VisitLiteralTree(this);
        }
        virtual void SematicAnalysis(){}
        virtual long long GetValue()
        {
            char * end;
            return strtoll(value.c_str(),&end,10);
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
        virtual void SematicAnalysis();
        virtual long long GetValue()
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
        virtual void accept( Visitor* visitor )
        {
            visitor -> VisitInitListExprTree(this);
        }
        bool addSon(ExprTree* son)
        {   
            sons.emplace_back(son);
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
        virtual void SematicAnalysis();
        virtual long long GetValue()
        {
            return cast -> GetValue();
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
        virtual void SematicAnalysis();
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
        virtual void SematicAnalysis();
        virtual long long GetValue()
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
        ExprTree * son;
        UnaryExprTree(std::string kind="", std::string name="", std::string value=""):
                ExprTree(kind,name,value){}
        virtual void accept( Visitor* visitor )
        {
            visitor -> VisitUnaryExprTree(this);
        }
        virtual void SematicAnalysis();
        virtual long long GetValue()
        {
            long long res,temp = son -> GetValue();
            switch(opcode)
            {
                case OP_PLUS:       res = temp;break;
                case OP_MINUS:      res = -temp;break;
                case OP_EXCLAIM:    res = !temp;break;
                case OP_TILDE:      res = ~temp;break;
            }
            return res;
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
        void TryUpcast();
        void AssignAdjust();
        virtual void SematicAnalysis();
        virtual long long GetValue()
        {
            long long res,val1 = left -> GetValue(),val2 = right -> GetValue();
            switch(opcode)
            {
                case OP_PLUS:           res = val1 + val2;break;
                case OP_MINUS:          res = val1 - val2;break;
                case OP_STAR:           res = val1 * val2;break;
                case OP_SLASH:          res = val1 / val2;break;
                case OP_PERCENT:        res = val1 % val2;break;
                case OP_LESSLESS:       res = val1 << val2;break;
                case OP_GREATERGREATER: res = val1 >> val2;break;
                case OP_LESS:           res = val1 < val2;break;
                case OP_GREATER:        res = val1 > val2;break;
                case OP_LESSEQUAL:      res = val1 <= val2;break;
                case OP_GREATEREQUAL:   res = val1 >= val2;break;
                case OP_EQUALEQUAL:     res = val1 == val2;break;
                case OP_EXCLAIMEQUAL:   res = val1 != val2;break;
                case OP_AMP:            res = val1 & val2;break;
                case OP_CARET:          res = val1 ^ val2;break;
                case OP_PIPE:           res = val1 | val2;break;
                case OP_AMPAMP:         res = val1 && val2;break;
                case OP_PIPEPIPE:       res = val1 || val2;break;
            }
            return res;
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
            info.layer.emplace_back(ptr -> GetValue());
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
        TryUpcast();
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
    
    
    
};
*/