#include <iostream>
#include <math.h>
using namespace std;
#define MAXN 10
#define MAXM 10
int m=4,n=5;
int v[MAXM][MAXN]={
                {0,0,0,0,0},
                {0,11,12,13,14,15},
                {0,0,5,10,15,20},
                {0,2,10,30,32,40},
                {0,20,21,22,23,24}
                };
int dp[MAXM][MAXN];
int pnum[MAXM][MAXN];
void plan(){
    int maxf,maxj;
    for(int j=0;j<n;j++){
        dp[m+1][j]=0;
    }
    for(int i=m;i>=1;i--){
        for(int s=1;s<=n;s++){
            maxf=0;
            maxj=0;
            for(int j=0;j<=s;j++){
                if((v[i][j]+dp[i+1][s-j])>=maxf){
                    maxf=v[i][j]+dp[i+1][s-j];
                    maxj=j;
                }
            }
            dp[i][s]=maxf;
            pnum[i][s]=maxj;
        }
    }
}

void displan(){
    int k,r,s;
    s=pnum[1][n];
    r=n-s;
    cout<<"最佳资源分配方案如下"<<endl;
    for(k=1;k<=m;k++){
        cout<<"第"<<k<<"项目分配"<<s<<"元"<<endl;
        s=pnum[k+1][r];
        r=r-s;
    }
    cout<<"该分配方案总盈利"<<dp[1][n]<<"元"<<endl;
}
int main(){
    plan();
    displan();
}