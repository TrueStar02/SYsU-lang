
void add(int arr1[][3],int n)
{
    int x;
    for(x = 0;x < n;x++)
    {
        arr1[x][2] = arr1[x][0] + arr1[x][1];
    }
}

int main(){
    
    int arr[5][3] = {{3,7,0},{5,0,0},{8,1,0},{4,6,0},{9,2,0}};
    int y = 1;
    add(arr,5);

    return y;
}