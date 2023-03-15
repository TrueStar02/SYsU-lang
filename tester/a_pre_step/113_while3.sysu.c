int main(){
    int x = 10;
    int y = 0;
    while(x > 2)
    {
        x = x - 1;
        y = y + 1;
        if(y > 2)
            break;
    }
    return 0;
}