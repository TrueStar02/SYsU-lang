int main(){
    int a,d,e;
    int b,c;
    a = 5|7;
    b = 8^5;
    c = 9&8;
    d = 8 << 1;
    e = 5 >> 1;
    int res = a | b + c - d ^ e || a ^ b & c - d * e;
    return res;
}