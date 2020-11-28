#include<iostream>
#include<algorithm>
using namespace std;
int flag1,flag2;
int maxSubSum1(int a[],int n){           //O(n3)
    int max=0,cnt;
    for(int i=0;i<n;i++){
        for(int j=i;j<n;j++){
            cnt=0;
            for(int k=i;k<=j;k++)
                cnt+=a[k];
                if(cnt>max){
                    max=cnt;
                    flag1=i;
                    flag2=j;
                }
        }
    }
    return max;
}
int maxSubSum2(int a[],int n){           //O(n2)
    int max=0,cnt;
    for(int i=0;i<n;i++){
        cnt=0;
        for(int j=i;j<n;j++){
            cnt+=a[j];
            if(cnt>max){
                max=cnt;
                flag1=i;
                flag2=j;
            }        
        }
    }
    return max;
}
int dp[1000];
int maxSubSum3(int a[],int n){           //O(n)
    int max=0;
    dp[0]=a[0];
    for(int i=1;i<n;i++){               //构造dp数组
        dp[i]=((dp[i-1]+a[i])>a[i]?dp[i-1]+a[i]:a[i]);
    }
    for(int i=0;i<n;i++)
        if(dp[i]>dp[max])
            max=i;
    for(int i=max;i>=0;i--)
        if(dp[i]<=0) {
            flag1=i+1;
            flag2=max;
            break;
        }
    return dp[max];
}
int main(){
    int n;
    cout<<"输入n: ";
    cin>>n;
    int *a=new int(n);
    //int a[]={0,-2,11,-4,13,-5,-2};
    cout<<"输入n个数字作为数组(第一个为0)"<<endl;
    for(int i=0;i<n;i++)
        cin>>a[i];
    cout<<endl<<"蛮力法1： 最大连续子序列的和为  "<<maxSubSum1(a,n)<<endl;
    for(int i=flag1;i<=flag2;i++)
        cout<<a[i]<<" ";
    cout<<endl<<"蛮力法2： 最大连续子序列的和为  "<<maxSubSum2(a,n)<<endl;
    for(int i=flag1;i<=flag2;i++)
        cout<<a[i]<<" ";
    cout<<endl<<"DP     ： 最大连续子序列的和为  "<<maxSubSum3(a,n)<<endl;
    for(int i=flag1;i<=flag2;i++)
        cout<<a[i]<<" ";
    cout<<endl;
    return 0;
}