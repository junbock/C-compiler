char a;
char fun(){
    static char b;
    a=b+1;
    return b;
}
int main(){
    a="a";
    a=fun();
    return 0;
}
