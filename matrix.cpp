#include<iostream>
using namespace std;
void Matrix_ADD(int **a,int **b,int **c,int n){//C=A+B
    for(register int i=0;i<n;i++){
        for(register int j=0;j<n;j++){
            c[i][j]=a[i][j]+b[i][j];

        }
    }
    
}
void Matrix_SUB(int **a,int **b,int **c,int n){//C=A-B
    for(register int i=0;i<n;i++){
        for(register int j=0;j<n;j++){
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
           //cin>>a[i][j];
           a[i][j]=1;
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
    int ** b1=new int *[n/2];int ** b2=new int *[n/2];
    int ** b3=new int *[n/2];int ** b4=new int *[n/2];
    Matrix_cut(n,b,b1,b2,b3,b4);
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
void Matrix_MUL3(int **a,int **b,int **c,int n){//使用Strassen方法
    if(n==2){
        Matrix_MUL(a,b,c,2);
        return;
    }
    int ** a11=new int *[n/2];int ** a12=new int *[n/2];
    int ** a21=new int *[n/2];int ** a22=new int *[n/2];
    Matrix_cut(n,a,a11,a21,a12,a22);
    int ** b11=new int *[n/2];int ** b12=new int *[n/2];
    int ** b21=new int *[n/2];int ** b22=new int *[n/2];
    Matrix_cut(n,b,b11,b21,b12,b22);
    int ** c11=new int *[n/2];int ** c12=new int *[n/2];
    int ** c21=new int *[n/2];int ** c22=new int *[n/2];
    Matrix_init(c11,n/2);
    Matrix_init(c12,n/2);
    Matrix_init(c21,n/2);
    Matrix_init(c22,n/2);    
    int **temp1=new int*[n/2];Matrix_init(temp1,n/2);    
    Matrix_SUB(b12,b22,temp1,n/2);
    
    int **temp2=new int*[n/2];Matrix_init(temp2,n/2);    
    Matrix_ADD(a11,a12,temp2,n/2);
    
    int **temp3=new int*[n/2];Matrix_init(temp3,n/2);    
    Matrix_ADD(a21,a22,temp3,n/2);
    
    int **temp4=new int*[n/2];Matrix_init(temp4,n/2);    
    Matrix_SUB(b12,b22,temp4,n/2);
    
    int **temp5=new int*[n/2];Matrix_init(temp5,n/2);    
    Matrix_ADD(a11,a22,temp5,n/2);
    
    int **temp6=new int*[n/2];Matrix_init(temp6,n/2);    
    Matrix_ADD(b11,b22,temp6,n/2);

    int **temp7=new int*[n/2];Matrix_init(temp7,n/2);    
    Matrix_SUB(a12,a22,temp7,n/2);
    
    int **temp8=new int*[n/2];Matrix_init(temp8,n/2);    
    Matrix_ADD(b21,b22,temp8,n/2);
 
    int **temp9=new int*[n/2];Matrix_init(temp9,n/2);    
    Matrix_SUB(a11,b21,temp9,n/2);

    int **temp10=new int*[n/2];Matrix_init(temp10,n/2);    
    Matrix_ADD(b11,b12,temp10,n/2);

    int **P1=new int*[n/2];Matrix_init(P1,n/2);    
    Matrix_MUL3(a11,temp1,P1,n/2);

    int **P2=new int*[n/2];Matrix_init(P2,n/2);    
    Matrix_MUL3(temp2,b22,P2,n/2);

    int **P3=new int*[n/2];Matrix_init(P3,n/2);    
    Matrix_MUL3(temp3,b11,P3,n/2);

    int **P4=new int*[n/2];Matrix_init(P4,n/2);    
    Matrix_MUL3(a22,temp4,P4,n/2);

    int **P5=new int*[n/2];Matrix_init(P5,n/2);    
    Matrix_MUL3(temp5,temp6,P5,n/2);

    int **P6=new int*[n/2];Matrix_init(P6,n/2);    
    Matrix_MUL3(temp7,temp8,P6,n/2);

    int **P7=new int*[n/2];Matrix_init(P7,n/2);    
    Matrix_MUL3(temp9,temp10,P7,n/2);
//求c11
    Matrix_ADD(P5,P4,c11,n/2);
    Matrix_SUB(c11,P2,c11,n/2);
    Matrix_ADD(c11,P6,c11,n/2);
//c12
    Matrix_ADD(P1,P2,c12,n/2);
//c21
    Matrix_ADD(P3,P4,c21,n/2);
 //c22
    Matrix_ADD(P5,P1,c22,n/2);
    Matrix_SUB(c22,P3,c22,n/2);
    Matrix_SUB(c22,P7,c22,n/2);   

    Matrix_Union(n,c,c11,c12,c21,c22);
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
Matrix_MUL(a,b,c,n);
cout<<"普通乘法结果"<<endl;
Matrix_Print(c,n);
Matrix_MUL2(a,b,c,n);
cout<<"递归乘法结果"<<endl;
Matrix_Print(c,n);
Matrix_MUL3(a,b,c,n);
cout<<"Strassen"<<endl;
Matrix_Print(c,n);

return 0;   
}