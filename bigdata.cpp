#include<iostream>
#include<string>
#define _GLIBCXX_USE_CXX11_ABI 1
using namespace std;
void input(char *a,int n){
     
    cout<<"输入二进制整数"<<endl;
    cin>>a;
    return a;
}
void cut(string a,string &b,string &c){
    int n=a.length();
    for(int i=0;i<n/2;i++)
        b.insert('1');
    for(int i=0;i<n/2;i++)
        c[i]=a[i+n/2];
}

int main(){
    string a=new string();
    a=input();
    a.insert(1,'1');
//  b=input();
    cout<<a<<endl;;
    cout<<a1;
    return 0;
}