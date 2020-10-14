#include<iostream>
using namespace std;
void Matrix_ADD(int **a,int **b,int **c,int n){//C=A+B
    for(register int i=0;i<n;i++){
        for(register int j=0;j<n;i++){
            c[i][j]=a[i][j]+b[i][j];
        }
    }
}
void Matrix_SUB(int **a,int **b,int **c,int n){//C=A-B
    for(register int i=0;i<n;i++){
        for(register int j=0;j<n;i++){
            c[i][j]=a[i][j]-b[i][j];
        }
    }
}
void Matrix_MUL(int **a,int **b,int **c,int n){//C=A*B
    for(register int i=0;i<n;i++){
		for (register int j= 0; j <n; j++){
			int sum = 0;
			for (register int m = 0; m <n; m++){
				sum = sum + a[i][m] * b[m][j];
			}
			c[i][j] = sum;
		}
	    }   
}
void Matrix_input(int **a ,int n){
    for(register int i=0;i<n;i++){
        for(register int j=0;j<n;j++){
           cin>>a[i][j];
        }  
    }
}
void Matrix_init(int **a,int n){
    for(int i=0;i<n;i++)
    a[i]=new int[n];
}
void Matrix_Print(int **a,int n){//打印
    for(register int i=0;i<n;i++){
		for (register int j= 0; j <n; j++){
	        cout<<a[i][j]<<" ";	
	    }
        cout<<endl;   
    }
}
void Matrix_copy(int **a,int **b,int n,int x,int y){//
    for(int i=0;i<=n/2-1;i++)
        for(int j=0;j<=n/2-1;j++){
            a[i+x][j+y]=b[i][j];
        }
}
void Matrix_Union(int n,int **a,int **a1,int **a2,int **a3,int **a4){//4个合并为一个
    Matrix_copy(a,a1,n,0,0);
    Matrix_copy(a,a2,n,0,n/2);
    Matrix_copy(a,a3,n,n/2,0);
    Matrix_copy(a,a4,n,n/2,n/2);
}
void Matrix_cut(int n,int **a,int **a1,int **a2,int **a3,int **a4){//一个分裂成四个
    Matrix_init(a1,n);
    Matrix_init(a2,n);
    Matrix_init(a3,n);
    Matrix_init(a4,n);
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++){
            if(i<=n/2-1&&j<=n/2-1)
                a1[i][j]=a[i][j];
            else if(i<=n/2-1&&j>n/2-1)
                a2[i][j-n/2]=a[i][j];
            else if(i>n/2-1&&j<=n/2-1)
                a3[i-n/2][j]=a[i][j];
            else
                a4[i-n/2][j-n/2]=a[i][j]; 
        }
}
void Matrix_MUL2(int **a,int **b,int **c,int n){//递归的方法
    if(n==2){
        Matrix_MUL(a,b,c,2);
    }else{
    int ** a1=new int *[n/2];int ** a2=new int *[n/2];
    int ** a3=new int *[n/2];int ** a4=new int *[n/2];
    Matrix_cut(n,a,a1,a2,a3,a4);
    cout<<"a1"<<endl;
    Matrix_Print(a1,n/2);
    cout<<"a2"<<endl;
    Matrix_Print(a2,n/2);
    cout<<"a3"<<endl;
    Matrix_Print(a3,n/2);
    cout<<"a4"<<endl;
    Matrix_Print(a4,n/2);
    int ** b1=new int *[n/2];int ** b2=new int *[n/2];
    int ** b3=new int *[n/2];int ** b4=new int *[n/2];
    Matrix_cut(n,b,b1,b2,b3,b4);
    cout<<"b1"<<endl;
    Matrix_Print(b1,n/2);
    cout<<"a2"<<endl;
    Matrix_Print(b2,n/2);
    cout<<"a3"<<endl;
    Matrix_Print(b3,n/2);
    cout<<"a4"<<endl;

    Matrix_Print(b4,n/2);
    int ** c1=new int *[n/2];int ** c2=new int *[n/2];
    int ** c3=new int *[n/2];int ** c4=new int *[n/2];
    int **temp1=new int*[n/2];int **temp2=new int*[n/2];
    Matrix_init(c1,n/2);Matrix_init(c2,n/2);
    Matrix_init(c3,n/2);Matrix_init(c4,n/2);
    Matrix_init(temp1,n/2);Matrix_init(temp2,n/2);
    Matrix_MUL2(a1,b1,temp1,n/2);
    Matrix_MUL2(a3,b2,temp2,n/2);
    Matrix_ADD(temp1,temp2,c1,n/2);

    Matrix_MUL2(a1,b3,temp1,n/2);
    Matrix_MUL2(a3,b4,temp2,n/2);
    Matrix_ADD(temp1,temp2,c3,n/2);
    
    Matrix_MUL2(a2,b1,temp1,n/2);
    Matrix_MUL2(a4,b2,temp2,n/2);
    Matrix_ADD(temp1,temp2,c2,n/2);
    
    Matrix_MUL2(a2,b3,temp1,n/2);
    Matrix_MUL2(a4,b4,temp2,n/2);
    Matrix_ADD(temp1,temp2,c4,n/2);

    //合成c
    Matrix_Union(n,c,c1,c2,c3,c4);
    }
}
int main(){
 int n;
cout<<"输入矩阵的维度n(n为2的k次方) :";
 cin>>n;
 int **a=new int*[n];
 int **b=new int*[n];
 int **c=new int*[n];
Matrix_init(a,n);
Matrix_init(b,n);
Matrix_init(c,n);
cout<<"输入矩阵A"<<endl;Matrix_input(a,n);
cout<<"输入矩阵B"<<endl;Matrix_input(b,n);
Matrix_MUL2(a,b,c,n);
cout<<"乘法结果"<<endl;
Matrix_Print(c,n);
return 0;   
}