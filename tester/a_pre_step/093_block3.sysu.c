int main(){
    
    int x ;
    {
        int a,b=2;
        
        {
            a = 1;
            b += a;
        }
        x = x + a;
    }
    return x;
}

