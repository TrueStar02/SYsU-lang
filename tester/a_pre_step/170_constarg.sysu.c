
int foo(int a[])
{
    a[1] = 3;
    return a[0];
    //return 0;
}

int main(){
    
    int ax[2] = {0,1};
    ax[1] = 2;

    return foo(ax);
}