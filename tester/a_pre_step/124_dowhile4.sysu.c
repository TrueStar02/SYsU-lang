int main(){
    int x = 0;
    int y = 0;
    do
    {   x = x + 1;
        y = 0;
        do
        {
            y = y + 1;
        }while(y < x);
    }while(x < 10);
    return 0;
}