#include <iostream>
#include <math.h>
using namespace std;
const int X = 6;
const int Y = 5;
int x[5]={0};
int m[X][Y]={0};
int F[X][Y] = { 0 };
int max(int F[X][Y],int f[X][Y],int mm,int money){
	for (int i = 1; i <= mm; i++){
		for (int j = 0; j <= money; j++){
			F[i][j] = 0;
			m[i][j] = 0;
			for (int k = 0; k <= j; k++){
				if (F[i][j] < f[i][k] + F[i - 1][j - k]){
					F[i][j] = f[i][k] + F[i - 1][j - k];
					m[i][j]=k;}
			}
		}
	}
	return F[mm][money];
}

void jie(int mm, int money,int x[5],int F[X][Y]){
  for(int i=mm;i>=1;i--)
  {
    x[i]=m[i][money];
    money=money-x[i];
    cout<<"the "<<i<<"get--"<<x[i]<<endl;
  }
  
}
int main(int argc, char** argv){
	int f[X][Y] = {0,0,0,0,0,
				   0,11,0,2,20,
				   0,12,5,10,21,
				   0,13,10,30,22,
				   0,14,15,32,23 };
	cout<<"输入钱数m和投资个数n"<<endl;
    int m,n;
    cin>>m>>n;
	cout <<"最大收益为:"<<max(F, f, 4, money)<<endl;
	jie(4,money,x,F); 
	return 0;
}