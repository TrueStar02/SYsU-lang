int main(){
    const int v = 2;
    int a = 4 , b = 1;
    if(v + a == 5)
    {
        b = 4;
    }    
    else if(v + a == 7 - b)
    {
        
        b = 2;
       
    }
    else
    {
        b = b + 3;
        a = a + 1;
        if(a + b == 5 - v)
        {
            b -= v;
        }
        else
            a -= v;

    }
        
    return a + b;
}