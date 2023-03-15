int main(){
    const int v = 2;
    int a,b = 5;
    if(v == 1)
    {   
        a = 7;
        if(a == 2 && b == 3)
        {
            a = 8;
        }
        else
        {
            --a;
            ++b;
        }
    }
    else
    {
        if(b == 5)
        {
            a = 4;
        }
        else
            a = 3;
    }
        
    return a;
}

