
int feb(int i)
{
    if(i == 1 || i == 2)
        return 1;
    else
        return feb(i-2) + feb(i-1);
}
const int a = 8,b = 12;
int x;
int main(){
    
    
    int y = 3;

    x = y;

    return feb(a) + feb(b);
}