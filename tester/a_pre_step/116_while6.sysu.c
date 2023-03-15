int main(){
    int x = 0;
    int y = 0;
    while(x < 10)
    {   x = x + 1;
        if(x == 5)
            continue;
        y = 0;
        while(y < 10)
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
    }
    return 0;
}