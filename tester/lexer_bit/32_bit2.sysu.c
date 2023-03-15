int main(){
    int a;
    int b,c,d,e;
    a = 5 | 7;
    b = 8 ^ 5;
    c = 9 & 8;
    d = 8 << 1;
    e = 5 >> 1;
    int res = a >> b | c ^ d & e;
    return res;
}