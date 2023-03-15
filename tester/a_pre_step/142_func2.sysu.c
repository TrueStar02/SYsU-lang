int foo(int a,int b)
{   int global;
    return a + b;
}
int main(){
    
    
    int res,x = 1,y = 3;
    res = foo(x,y);


    return res;
}