int main(){
    int x,y;
    for(x= 0;x < 10;x++)
    {   for(y = 0;y <= x;y++)
        {
            continue;
        }
        if(x > 8)
            break;
        
    }
    return x;
}