int main(){
    const int v = 2;
    int a,b = 5,c;
    if(v == 1)
    {   
        a = 7;
        if(a == 2 && b == 3)
        {
            a = 8;
        }
        else
        {
            a = a - 1;
            b = b + 1;
        }
    }
    else
    {
        c = 2;
        if(b == 5)
        {
            a = 4;
            if(c == 2 || a == 6)
                b = b + 1;
            c = c - 1;
        }
        else
            a = 3;
    }
        
    return a;
}

