int main(){
    int x = 0;
    int y = 0;
    while(x < 10)
    {   x++;
        y = 0;
        while(y < x)
        {
            y++;
        }
    }
    return 0;
}