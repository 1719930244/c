#include<iostream>
#include<algorithm>
using namespace std;
class assignment{
    public:
    int num;
    double time, profit,effiency;
    int flag;//是否已经被调动
   assignment(int a,double b,double c){
        num=a;
        time=b;
        profit=c;
        effiency=(double)(c/b);
        flag=0;
    };
    assignment(){}
};
bool cmp(assignment a,assignment b){
    return a.effiency>b.effiency;
}
void print(assignment *list,int n){
    for(int i=0;i<n;i++)
    cout<<"第"<<list[i].num<<"个作业的效率为"<<list[i].effiency<<" 时间为"<<list[i].time<<endl;
}
int main(){
    int n,t;
    cout<<"输入作业个数n：";
    cin>>n;
    assignment *list=new assignment[100]; 
    for(int i=0;i<n;i++){
        double t,p;
        cout<<"输入第"<<i<<"个作业的时间:"<<endl;
        cin>>t;
        cout<<"输入第"<<i<<"个作业的收益:"<<endl;
        cin>>p;
        list[i]=assignment(i,t,p);
    }    
    cout<<"输入时间t:"<<endl;
    cin>>t;
    sort(list,list+n,cmp);
    print(list,n);
    int nowtime=0,cnt=0;
    cout<<"调度顺序为 "<<endl;
    while(nowtime<t){
        for(int i=0;i<n;i++){
            if(list[i].flag==0&&(list[i].time+nowtime<=t)){
                list[i].flag=1;
                nowtime+=list[i].time;
                cout<<"作业"<<list[i].num<<" ";
                cnt++;
                break;
            }
        }
        cout<<endl;
        if(nowtime>t)
          break;
        if(cnt>n)
         break;
         cnt++;
    }
    return 0;
}