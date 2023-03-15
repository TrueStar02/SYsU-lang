int main(){
    int x = 10;
    int y = 0;
    while(x > 2)
    {
        x--;
        if(x == 8)
            continue;
        y++;
        if(y > 5)
            break;
    }
    return 0;
}