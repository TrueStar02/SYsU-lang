int main(){
    int x = 0;
    int y = 0;
    const int v = 3;
    do
    {   x = x + 1;
        if(x == 5)
            continue;
        y = 0;
        while(y < 10 || 10 >= v)
        {
            
            
            if(y > x)
                break;
            else
            {
                
                if(y == 5)
                {   
                    y = y + 1;
                    continue;
                }  
                y = y + 1;  
            }
        }
    }while(x < 10 && v < 10);
    return 8 | v * y ^ x;
}