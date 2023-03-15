int main(){
    int x = 10;
    int y = 0;
    
    do{
        x = x - 1;
        if(x == 8)
            continue;
        y = y + 1;
        if(y > 5)
            break;
    }while(x > 2);
    return 0;
}