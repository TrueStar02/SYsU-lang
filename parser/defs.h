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

class Info
{
    public:
        Type type;
        int layer;
        bool isConst;
        std::vector<Info> args;
        Info(enum Type t = Void,int l = 0,bool ic = 0):
        type(t),layer(l),isConst(ic){}
};

typedef std::unordered_map<std::string,Info> SymT;
SymT symt;
Info curret;

class Tree
{
    public:
    bool isLeftVal;
    std::string kind;
    std::string name;
    std::string value;
    Info info;
    std::vector<Tree *> sons;
    Tree(bool isLeft,std::string kind="", std::string name="", std::string value=""): 
    isLeftVal(isLeft),kind(kind), name(name), value(value),info(Info()) {}
    bool addSon(bool isLeft,Tree* son)
    {   
        bool retval = 0;
        if(son != nullptr)
        {
            if( isLeft == 0 && son -> isLeftVal == 1 && son -> info.layer == 0)
            {
                auto ptr = new Tree(0,"ImplicitCastExpr");
                ptr->sons.emplace_back(son);
                ptr->info = son->info;
                sons.emplace_back(ptr); 
                retval = 0;
            }
            else
            {
                sons.emplace_back(son);
                retval = son -> isLeftVal;
                isLeftVal &= son -> isLeftVal;
            }
        }   
        else 
            sons.emplace_back(son);
        return retval;
    }
    void mergeSon(Tree* son)
    {   
        for(auto ptr:son -> sons)
            sons.emplace_back(ptr); 
        free(son);
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
    void SpreadType(Type type)
    {
        info.type = type;
        for(auto son:sons)
        {
            son -> info.type = type;
            yyerror(son->name);
            yyerror(son->info.layer);
            yyerror("\n");
            if(son -> sons.size())
                DeclAdjust(son);
            symt.insert(std::make_pair(son->name,son->info));
            yyerror("Test");
            yyerror(symt[son->name].type);
            yyerror(symt[son->name].layer);
            yyerror("\n");
            ;

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
    llvm::json::Value toJson() const 
    {
        llvm::json::Object tmp{
        {"kind", kind},
        {"name", name},
        {"value", value},
        {"type",GetType()},
        {"z-childs",GetChild()},
        {"lVal",(isLeftVal ? "True" : "False")},
        {"inner", llvm::json::Array{}}
        };
        for(auto&& it: sons) tmp.get("inner")->getAsArray()->push_back(it->toJson());
        return tmp;
    }
    void print(int depth=0) const 
    {
        yyerror("|");
        for(int i=0;i<depth;++i) 
            yyerror(" ");
        yyerror("-"+kind+" "+name+" "+value);
        for(auto&& it: sons)
        {
            yyerror("\n");
            it->print(depth+1);
        }
        if(!depth) yyerror("\n\n");
    }
    void GetParaType(Tree * ptr)
    {
        int s=0;
        for(auto son:ptr->sons)
        {
            ptr->info.args.emplace_back(son->info);
            s++;
        }    
        yyerror(s);
        yyerror("\n");
    }
    void AdjustParaType(Tree * ptr)
    {
        int size = ptr -> info.args.size();
        for(int i = 0;i < size;i++)
        {
            
            if(ptr -> info.args[i].layer > 0 )
            {
                auto tmp = new Tree(1,"ImplicitCastExpr");
                tmp -> sons.emplace_back(ptr -> sons[i + 1]);
                tmp -> isLeftVal = ptr -> sons[i + 1] -> isLeftVal;
                tmp -> info.type = ptr -> sons[i + 1] ->info.type;
                tmp -> info.layer = ptr -> sons[i + 1] ->info.layer;
                tmp -> info.isConst = ptr -> sons[i + 1] ->info.isConst;
                ptr -> sons[i + 1] = tmp; 
                yyerror("cast1\n");

                if(ptr -> info.args[i].type != ptr -> sons[i + 1] -> info.type)
                {
                    auto tmp = new Tree(1,"ImplicitCastExpr");
                    tmp -> sons.emplace_back(ptr -> sons[i + 1]);
                    tmp -> isLeftVal = ptr -> sons[i + 1] -> isLeftVal;
                    tmp -> info.type = ptr -> info.type;
                    tmp -> info.layer = ptr -> info.layer;
                    tmp -> info.isConst = ptr -> info.isConst;
                    ptr -> sons[i + 1] = tmp; 
                    yyerror("cast2\n");
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
                    yyerror("cast3\n");
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