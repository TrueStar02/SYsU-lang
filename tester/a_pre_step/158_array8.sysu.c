
int sort(int arr[],int n)
{
    int temp,x,y;
    int s = 0;
    for(x = 0;x < n;x++)
    for(y = x + 1;y < n;y++)
    {
        if(arr[x] > arr[y])
        {
            s++;
            temp = arr[x];
            arr[x] = arr[y];
            arr[y] = temp;
        }
    }
    return s;
}

int main(){
    
    int arr[10] = {3,7,5,0,8,1,4,6,9,2};
    int y = sort(arr,10);

    return y;
}