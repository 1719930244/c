#include<iostream>
#include<algorithm>
using namespace std;
class assignment{
    public:
    int num;
    int time;
    int profit;
    int flag;//是否已经被调动
    double effiency;
    assignment(int a,int b,int c){
        num=a;
        time=b;
        profit=c;
        effiency=double(c/b);
        flag=0;
    }：
    assignment(){}
};
bool cmp(assignment a,assignment b){
    return a.effiency>b.effiency;
}
int main(){
    int n,t;
    cout<<"输入作业个数n：";
    cin>>n;
    assignment *list=new assignment[100]; 
    for(int i=0;i<n;i++){
        int t,p;
        cout<<"输入第"<<i<<"个作业的时间:"<<endl;
        cin>>t;
        cout<<"输入第"<<i<<"个作业的收益:"<<endl;
        cin>>p;
        list[i]=assignment(i,t,p);
    }    
    cout<<"输入时间t:"<<endl;
    cin>>t;
    sort(list,list+n,cmp);
    int nowtime=0;
    while(nowtime<t){
        for(int i=0;i<n;i++){
            if(list[i].flag==0&&(list[i].time+nowtime<=t)){
                list[i].flag=1;
                nowtime+=list[i].time;
                cout<<"作业"<<list[i].num<<" ";
                break;
            }
        }
    if(nowtime>t)
        break;
    }
    return 0;
}