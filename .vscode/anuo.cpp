#include <stdio.h>
#include <string.h>
void move(int n,char x,char y){
    printf("将盘片%d从%c搬到%c\n",n,x,y);
}
void fun(int n,char a ,char b,char c){
    if(n==1)
        move(n,a, c); 
    else{
        fun(n-1,a ,c,b);
        move(n,a,c);
        fun(n-1,b,a,c);
    }
}
int main(){
    int n;
    scanf("%d",&n);
    fun(n,'A','B','C');
    return 0;
}