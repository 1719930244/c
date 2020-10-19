// Lexical_Analysis.cpp : 定义控制台应用程序的入口点。
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "iostream"
using namespace std;
//词法分析程序
//首先定义种别码
/*
第一类：标识符   letter(letter | digit)*  无穷集
第二类：常数    (digit)+  无穷集
第三类：保留字(32)
auto       break    case     char        const      continue
default    do       double   else        enum       extern
float      for      goto     if          int        long
register   return   short    signed      sizeof     static
struct     switch   typedef  union       unsigned   void
volatile    while   //main 
第四类：界符  ‘/*’、‘//’、 () { } [ ] " "  '
第五类：运算符 <、<=、>、>=、=、+、-、*、/、^、
对所有可数符号进行编码：
<$,0>
<auto,1>
...
<while,32>
<+，33>
<-,34>
<*,35>
</,36>
<<,37>
<<=,38>
<>,39>
<>=,40>
<=,41>
<==,42>
<!=,43>
<;,44>
<(,45>
<),46>
<^,47>
<,,48>
<",49>
<',50>
<#,51>
<&,52>
<&&,53>
<|,54>
<||,55>
<%,56>
<~,57>
<<<,58>左移
<>>,59>右移
<[,60>
<],61>
<{,62>
<},63>
<\,64>
<.,65>
<?,66>
<:,67>
<!,68>
<常数99  ,数值>
<标识符100 ，标识符指针>
<字符串常量 ，101>
<+=,102>
<-=,103>
<*=,104>
</=,105>
<|=,106>
<&=,107
*/

/****************************************************************************************/
//全局变量，保留字表
static int cnt_line[100]={0};
static char reserveWord[32][20] = {
    "auto", "break", "case", "char", "const", "continue",
    "default", "do", "double", "else", "enum", "extern",
    "float", "for", "goto", "if", "int", "long",
    "register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void",
    "volatile", "while"
};
//界符运算符表,根据需要可以自行增加
static char operatorOrDelimiter[40][10] = {
    "+", "-", "*", "/", "<", "<=", ">", ">=", "=", "==",
    "!=", ";", "(", ")", "^", ",", "\"", "\'", "#", "&",
    "&&", "|", "||", "%", "~", "<<", ">>", "[", "]", "{",
    "}", "\\", ".", "\?", ":", "!","+=","-=","*=","/="
};

/****************************************************************************************/

/********查找是否是保留字*****************/
//是则返回种别码，否则返回-1
int searchReserve(char reserveWord[][20], char s[])
{
    for(register int i=0;i<32;i++)
        if(strcmp(reserveWord[i],s)==0){
        return i;
    }
    if(s[0]=='$')
    return 0;
    return -1;
}
/********查找保留字*****************/

/*********************判断是否为字母********************/
bool IsLetter(char letter)
{//注意C语言允许下划线也为标识符的一部分可以放在首部或其他地方
    if (letter >= 'a'&&letter <= 'z' || letter >= 'A'&&letter <= 'Z'|| letter=='_')
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*********************判断是否为字母********************/


/*****************判断是否为数字************************/
bool IsDigit(char digit)
{
    if (digit >= '0'&&digit <= '9')
    {
        return true;
    }
    else
    {
        return false;
    }
}
/*****************判断是否为数字************************/

void Print_error(int lines,int place,string error){
    FILE *fp;
    if ((fp = fopen("error.o", "a")) == NULL)
    {//打开源程序
        cout << "can't open this file";
        exit(0);
    }
    char c[50];
    strcpy(c,error.c_str());
    fprintf(fp, "( 错误行数：%d ,错误列数： %d ,错误内容： %s)\n",lines,place,c);
    fclose(fp);
}
/********************编译预处理，取出无用的字符和注释**********************/
void filterResource(char r[], int &pProject)
{
    char tempString[10000];
    int count=0,lines = 0;
    for (register int i = 0; i <= pProject; i++){//自己实现
        if(r[i]=='\r'||r[i]=='\t'){
           continue;
        }
        else if(r[i]=='\n'){
            cnt_line[lines++]=count-1;
            continue;
        }
        else if(r[i]=='/'&&r[i+1]=='/'){
                cnt_line[lines++]=count-1;
                i++;
                while(r[i]!='\n'&&r[i]!='$'){
                    i++;
                }
                
                continue;
            }
        else if(r[i]=='/'&&r[i+1]=='*'){
                i+=2;
                while(r[i]!='*'||r[i+1]!='/'){
                    i++;
                    if(r[i]=='$'){
                            tempString[count++]=r[i];
                            Print_error(lines,count-cnt_line[lines-1],"注释不对");
                            break;
                    }
                }
                i++;
                continue;
            }
        
        else{
            tempString[count++]=r[i];
        }
    }
    tempString[count] = '\0';
    pProject=count;
    strcpy(r, tempString);//产生净化之后的源程序
}
/********************编译预处理，取出无用的字符和注释**********************/


/****************************分析子程序，算法核心***********************/
void GetToken(int &syn, char resourceProject[], char token[], int &pProject,int &line)
{//根据DFA的状态转换图设计
    int i, count = 0;//count用来做token[]的指示器，收集有用字符
    char ch;//作为判断使用
    while(pProject>=cnt_line[line])
        line++;
    for (i = 0; i<50; i++){//每次收集前先清零
        token[i] = '\0';
    }
    ch = resourceProject[pProject];
    while (ch == ' '){//过滤空格，防止程序因识别不了空格而结束
        pProject++;
        ch = resourceProject[pProject];
    }
    if(ch =='\"'){
        token[count++]=ch;
        pProject++;
        ch = resourceProject[pProject];
        while(ch!='\"'){
            token[count++]=ch;
            pProject++;
            ch = resourceProject[pProject];
        }
        ch = resourceProject[pProject];
        token[count++]=ch;
        pProject++;
        syn=101;
        token[count]='\0';
        return ;
    }
    if (IsLetter(resourceProject[pProject])){	
		//开头为字母，读入后面的字符，判断是否为标识符，并将字符串存储在token中，自己实现。
        token[count++]=resourceProject[pProject++];
        while((IsLetter(resourceProject[pProject])||IsDigit(resourceProject[pProject]))){
            token[count++]=resourceProject[pProject++];
        }
		token[count]= '\0';
		int syn_new=searchReserve(reserveWord,token);
        int syn_now=syn;
        if (syn_new == -1){
            syn = 100;//标识符种别码
        }
        else{
            syn=syn_new;
        }
        
    }
    else if (IsDigit(resourceProject[pProject]))
    {//首字符为数字
        if(resourceProject[pProject]=='0'&&resourceProject[pProject+1]=='x'){
            pProject+=2;
            token[count++]='0';
            token[count++]='x';
        }
        while (IsDigit(resourceProject[pProject])||
        ((resourceProject[pProject]>='a')&&(resourceProject[pProject]<='f'))){//后跟数字
            token[count++] = resourceProject[pProject];//收集
            pProject++;
        }//多读了一个字符既是下次将要开始的指针位置
        if(resourceProject[pProject]=='.'&&IsDigit(resourceProject[pProject+1])){
            token[count++] = resourceProject[pProject++];
            while (IsDigit(resourceProject[pProject])){
                token[count++] = resourceProject[pProject];
                pProject++;
            }   
        }
        token[count] = '\0';
        syn = 99;//常数种别码
        return;
    }
    else if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == ';' || ch == '(' || ch == ')' || ch == '^'
        || ch == ',' || ch == '\"' || ch == '\'' || ch == '~' || ch == '#' || ch == '%' || ch == '['
        || ch == ']' || ch == '{' || ch == '}' || ch == '\\' || ch == '.' || ch == '\?' || ch == ':')
    {//若为运算符或者界符，查表得到结果
        if(ch == '+'&&resourceProject[pProject+1]=='='){
            token[0] = resourceProject[pProject++];
            token[1]=resourceProject[pProject++];
            token[2] = '\0';//形成单字符串
            syn=102;
            return ;
        }
        if(ch == '-'&&resourceProject[pProject+1]=='='){
            token[0] = resourceProject[pProject++];
            token[1]=resourceProject[pProject++];
            token[2] = '\0';//形成单字符串
            syn=103;
            return ;
        }
        if(ch == '*'&&resourceProject[pProject+1]=='='){
            token[0] = resourceProject[pProject++];
            token[1]=resourceProject[pProject++];
            token[2] = '\0';//形成单字符串
            syn=104;
            return ;
        }
        if(ch == '/'&&resourceProject[pProject+1]=='='){
            token[0] = resourceProject[pProject++];
            token[1]=resourceProject[pProject++];
            token[2] = '\0';//形成单字符串
            syn=105;
            return ;
        }
        
        token[0] = ch;
        token[1] = '\0';//形成单字符串
        for (i = 0; i<36; i++)
        {//查运算符界符表
            if (strcmp(token, operatorOrDelimiter[i]) == 0)
            {
                syn = 33 + i;//获得种别码，使用了一点技巧，使之呈线性映射
                break;//查到即推出
            }
        }
        pProject++;//指针下移，为下一扫描做准备
        return;
    }
    else  if (resourceProject[pProject] == '<')
    {//<,<=,<<
        pProject++;//后移，超前搜索
        if (resourceProject[pProject] == '=')
        {
            syn = 38;
        }
        else if (resourceProject[pProject] == '<')
        {//左移
            pProject--;
            syn = 58;
        }
        else
        {
            pProject--;
            syn = 37;
        }
        pProject++;//指针下移
        return;
    }
    else  if (resourceProject[pProject] == '>')
    {//>,>=,>>
        pProject++;
        if (resourceProject[pProject] == '=')
        {
            syn = 40;
        }
        else if (resourceProject[pProject] == '>')
        {
            syn = 59;
        }
        else
        {
            pProject--;
            syn = 39;
        }
        pProject++;
        return;
    }
    else  if (resourceProject[pProject] == '=')
    {//=.==
        pProject++;
        if (resourceProject[pProject] == '=')
        {
            syn = 42;
        }
        else
        {
            pProject--;
            syn = 41;
        }
        pProject++;
        return;
    }
    else  if (resourceProject[pProject] == '!')
    {//!,!=
        pProject++;
        if (resourceProject[pProject] == '=')
        {
            syn = 43;
        }
        else
        {
            syn = 68;
            pProject--;
        }
        pProject++;
        return;
    }
    else  if (resourceProject[pProject] == '&')
    {//&,&&
        pProject++;
        if(ch == '&'&&resourceProject[pProject+1]=='='){
            token[0] = resourceProject[pProject++];
            token[1]=resourceProject[pProject++];
            token[2] = '\0';//形成单字符串
            syn=107;
            return ;
        }
        
        if (resourceProject[pProject] == '&')
        {
            syn = 53;
        }
        else
        {
            pProject--;
            syn = 52;
        }
        pProject++;
        return;
    }
    else  if (resourceProject[pProject] == '|')
    {//|,||
        pProject++;
        if(ch == '|'&&resourceProject[pProject+1]=='='){
            token[0] = resourceProject[pProject++];
            token[1]=resourceProject[pProject++];
            token[2] = '\0';//形成单字符串
            syn=106;
            return ;
        }        
        if (resourceProject[pProject] == '|')
        {
            syn = 55;
        }
        else
        {
            pProject--;
            syn = 54;
        }
        pProject++;
        return;
    }
    else  if (resourceProject[pProject] == '$')
   {//结束符
        syn = 0;//种别码为0
    } 
    else{//不能被以上词法分析识别，则出错。
        Print_error(line,pProject-cnt_line[line-1],"字符不能被识别");
        cout<<"字符无法识别： "<<ch<<endl;
        pProject++;
    }
}
void Print(char s[],int n){
    int l=0;
    cout<<"第"<<l<<"行 : ";
    for(register int i=0;i<n;i++){
    cout<<s[i];
    while(i==cnt_line[l]){
        cout<<endl;
        l++;
        cout<<"第"<<l<<"行"<<endl;
    }
}

}

static  char IDentifierTbl[1000][50] = {""};//标识符表
static int cnt=0;
void insert(char token[]){//插入标识符表
    bool flag=false;
    for(register int i=0;i<=cnt;i++)
        if(strcmp(IDentifierTbl[i],token)==0)
            flag=true;
         if(flag==false){
        strcpy(IDentifierTbl[cnt++],token);
    }
}  
    
int main(){
    //打开一个文件，读取其中的源程序
    char resourceProject[10000];
    char token[50] = { 0 };
    int syn = -1, syn_last=-1,i;//初始化
    int pProject = 0;//源程序指针
    FILE *fp, *fp1;
    if ((fp = fopen("testHard.c", "r")) == NULL)
    {//打开源程序
        cout << "can't open this file";
        exit(0);
    }
    resourceProject[pProject] = fgetc(fp);//读取一个字符
    while (resourceProject[pProject] != '$')
    {//将源程序读入resourceProject[]数组
        pProject++;
        resourceProject[pProject] = fgetc(fp);
    }
    resourceProject[++pProject] = '\0';
    fclose(fp);
    cout << endl << "源程序为:" << endl; 
    //对源程序进行过滤
    filterResource(resourceProject, pProject);
    cout << endl << "过滤之后的程序:" << endl;
    Print(resourceProject, pProject);
    int line=0;
    pProject = 0;//从头开始读
    
    if ((fp1 = fopen("test_compile.txt", "w+")) == NULL){
        cout << "can't open this file";
    }

    while (syn != 0){
        //启动扫描
        GetToken(syn, resourceProject, token, pProject,line);
        if((syn_last==99)&&(syn==100)){
            string error="连续俩标识符或常量错误";
            Print_error(line,pProject-cnt_line[line-1],error);
            }
        syn_last=syn;
        while(pProject==cnt_line[line]){
            line++;
            cout<<endl<<"第"<<line<<"行"<<endl;
        }
        if (syn == 100){
        //判断是否在已有标识符表中，自己实现
        insert(token);
        printf("( 标识符 ,%s)", token);
        fprintf(fp1, "( 标识符 ,%s)\n", token);
        }
        else if (syn >= 1 && syn <= 32)
        {//保留字
            printf("( %s ,--)", reserveWord[syn]);
            fprintf(fp1, "( %s ,--)\n", reserveWord[syn]);
        }
        else if (syn == 99)
        {//const 常数
            printf("( 常数 ,%s)", token);
            fprintf(fp1, "( 常数 ,%s)\n", token);
        }
        else if (syn == 101)
        {//const 常数
            printf("( 字符串常量 ,%s)", token);
            fprintf(fp1, "( 字符串常量 ,%s)\n", token);
        } 
        else if (syn >= 33 && syn <= 68)
        {
            printf("( %s ,--)", operatorOrDelimiter[syn - 33]);
            fprintf(fp1, "( %s ,--)\n", operatorOrDelimiter[syn - 33]);
        }
        else if(syn>=102&&syn<=107){
            printf("( %s ,--)", operatorOrDelimiter[syn - 66]);
            fprintf(fp1, "( %s ,--)\n", operatorOrDelimiter[syn - 66]);
        }
    }
    cout<<endl;
 for (i = 0; i<cnt; i++)
    {//插入标识符表中
        printf("第%d个标识符：  %s\n", i, IDentifierTbl[i]);
        fprintf(fp1, "第%d个标识符：  %s\n", i, IDentifierTbl[i]);
    }

   fclose(fp1);
    return 0;
}