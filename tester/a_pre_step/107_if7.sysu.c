int main(){
    const int v = 2;
    int a = 4 , b = 1;
    if(v + a == 5)
    {
        b = 4;
    }    
    else
    {
        if(v + a == 7 - b)
            b = 2;
        b++;
    }
        
    return a + b;
}

