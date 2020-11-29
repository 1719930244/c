#include <iostream>
#include <string>
#include <fstream>
#include <set>
#include <map>
#include <iomanip>
#include <stack>
#include <string.h>
using namespace std;
const int maxnlen = 1e4;
class Grammar {
private:
	set<char>Vn;//non-terminal非终结符集合
	set<char>Vt; //terminal终结符集合
	char S;
	map<char, set<string> > P;//产生式规则集合
	map<char,set<char> >FIRST;//first集合
	map<char,set<char> >FOLLOW;//follow集合
	map<string, string>Table;//生成的驱动表
public:
	//构造函数
	Grammar(string filename) {
		Vn.clear();
		Vt.clear();
		P.clear();
		FIRST.clear();
		FOLLOW.clear();
		ifstream in(filename);
		if (!in.is_open()) {
			cout << "文法  文件打开失败" << endl;
			exit(1);
		}
		char *buffer = new char[maxnlen];
		in.getline(buffer, maxnlen, '#');
		string temps = "";
		//起始式的标志
		bool is_sethead = 0;
		//读取每个产生式为temp并加入
		for (int i = 0; i < strlen(buffer); i++) {
			if (buffer[i] == '\n' || buffer[i] == ' '|| (int)buffer[i]==13)
				continue;
			//其中每个；为一个产生式规则结束
			if (buffer[i] == ';') {
				if (!is_sethead) {
					this->setHead(temps[0]);
					is_sethead = 1;
				}
				this->add(temps);
				temps="";
			}
			else
				temps += buffer[i];
		}
		delete buffer;
		cout<<"非终结符集合Vn有："<<endl;
		for (set<char>::iterator it = Vn.begin(); it != Vn.end(); it++) {
			cout<<*it<<" ";
		}
		cout<<endl<<"终结符集合Vt有："<<endl;	
		for (set<char>::iterator it = Vt.begin(); it != Vt.end(); it++) {
			cout<<*it<<" ";
		}
		cout<<endl;	
		this->print();
	}
	bool is_Non_terminal(char s){
		if(Vn.find(s)!=Vn.end())
		return true;
		else
		return false;
	}
	
	bool is_terminal(char s){
		if(Vt.find(s)!=Vt.end())
		return true;
		else
		return false;
	}
	
	void setHead(char c) {
		S = c;
	}
	//加入一个产生式规则
	void add(string s) {
		char s1 = s[0];
		string s2="";
		int num = 0;
		for (int i = 0; i < s.length() ; i++) {
			if (s[i] == '>')
				num=i;
			if (num == 0)
				continue;
			if (i > num)
				s2 += s[i];
		}
		s2 += ';';
		//s1为非终结符
		Vn.insert(s1);
		string temp = "";
		//set<char>::iterator iter1 = s2.begin();
		for (int i = 0; i < s2.length() ; i++) {
			//char s : s2
			char s=s2[i];
			//对于产生式右部的每一个字符添加终结符，并将合并的产生式分开
			if (!isupper(s) && s != '|'&&s != ';'&&s!='@')
			Vt.insert(s);
			if (s == '|' || s == ';') {
				P[s1].insert(temp);
				temp = "";
			}
			else {
				temp += s;
			}
		}
	}
	
	void print() {
		cout << "当前分析所得到的文法为：" << endl;
		for (set<char>::iterator it = Vn.begin(); it != Vn.end(); it++) {
			char cur_s = *it;
			for (set<string>::iterator it1 = P[cur_s].begin(); it1 != P[cur_s].end(); it1++) {
				string cur_string = *it1;
				cout << cur_s << "->" << cur_string << endl;
			}
		}
	}
	
	void getFirst() {
		FIRST.clear();
		//判断迭代次数
		int iter = 4;
		while (iter--) {
			for (set<char>::iterator it = Vn.begin(); it != Vn.end(); it++) {
				char cur_s = *it;
				for (set<string>::iterator it1 = P[cur_s].begin(); it1 != P[cur_s].end(); it1++) {
					string cur_string = *it1;
					//分cur_string[0]是终结符，非终结符，和非终结符的first集是否有@
					for(int i= 0;i<cur_string.length();i++){
						if( is_terminal(cur_string[i])||cur_string[i]=='@'){
							FIRST[cur_s].insert(cur_string[i]);
							break;
						}	
						else if(is_Non_terminal(cur_string[i])){
							for(auto temp:FIRST[cur_string[i]]){
								if(temp!='@')
								FIRST[cur_s].insert(temp);
							}
							if(FIRST[cur_string[i]].find('@')!=FIRST[cur_string[i]].end()){
								if(i==cur_string.length()-1)
									FIRST[cur_s].insert('@');	
								continue;
							}
							else
								break;
						}
						else
							{
							cout<<cur_string[i]<< " is error !"<<endl;
						}
					}				
				}
		}
	}
		cout << "FIRST集为："<< endl;
		for (set<char>::iterator it = Vn.begin(); it != Vn.end();it++) {
			char cur_s = *it;
			cout << "FIRST()   " << cur_s ;
			for (set<char>::iterator it1 = FIRST[cur_s].begin(); it1 != FIRST[cur_s].end(); it1++) {
				 cout<<"       " << *it1 ;
			}
			cout << endl;
		}
	}
	void getFollow(){
		FOLLOW.clear();
		FOLLOW[S].insert('#');
		//判断迭代次数
		int iter = 5;
		while (iter--) {
			for (set<char>::iterator it = Vn.begin(); it != Vn.end(); it++) {
				char cur_s = *it;
				/*  第一步：B->Ac,将c加入到A的follow集
					第二步：B->AC,将C的first集加到A的follow集
				    第三步：遍历C的first去除@，加到A的follow集
					第四步，AC/ACK为最后两个或者三个 
							B->AC
							B->ACK(K的first集含有@) 
							将B的follow集加入到C的follow集
					*/	
				for (set<string>::iterator it1 = P[cur_s].begin(); it1 != P[cur_s].end(); it1++) {
					string cur_string = *it1;							
					for (int i = 0; i < cur_string.length() - 1; i++) {
						if(is_Non_terminal(cur_string[i])&&is_terminal(cur_string[i+1])){
							FOLLOW[cur_string[i]].insert(cur_string[i+1]);
						}
						if(is_Non_terminal(cur_string[i])&&is_Non_terminal(cur_string[i+1])){
							for(auto temp:FIRST[cur_string[i+1]]){
								if(temp!='@')
								FOLLOW[cur_string[i]].insert(temp);
						}
					}
				}
					int end_index=cur_string.length()-1;
					if(is_Non_terminal(cur_string[end_index])){
							for(auto temp:FOLLOW[cur_s]){
								FOLLOW[cur_string[end_index]].insert(temp);
							}
						//当end_index所在非终结符的first集中存在@时
						while(FIRST[cur_string[end_index]].find('@')!=FIRST[cur_string[end_index]].end()){
							if(end_index-1<0||is_terminal(cur_string[end_index-1]))
								break;
							for(auto temp:FOLLOW[cur_s]){
								FOLLOW[cur_string[end_index-1]].insert(temp);
							}
							end_index--;
						}
					}
			}
		}
	}
		cout << "FOLLOW集为："<< endl;
		for (set<char>::iterator it = Vn.begin(); it != Vn.end(); it++) {
			char cur_s = *it;
			cout << "FOLLOW()  " << cur_s;
			for (set<char>::iterator it1 = FOLLOW[cur_s].begin(); it1 != FOLLOW[cur_s].end(); it1++) {
				cout << "       " << *it1;
			}
			cout << endl;
		}
	}
	
	void getTable() {
		set<char>Vt_temp;
		//int i = 0; i < s2.length() ; i++
		//set<char>::iterator iter1;
		for (set<char>::iterator iter1 =Vt.begin(); iter1!=Vt.end();iter1++ ) {
			Vt_temp.insert(*iter1);
		}
		Vt_temp.insert('#');
		for (auto it = Vn.begin(); it != Vn.end(); it++) {
			char cur_s = *it;
			for (auto it1 = P[cur_s].begin(); it1 != P[cur_s].end(); it1++) {
				string cur_string = *it1;
				if (is_Non_terminal(cur_string[0])) {
					char first_s = cur_string[0];
					for (auto it2 = FIRST[first_s].begin(); it2 != FIRST[first_s].end(); it2++) {
						string TableLeft = "";
						TableLeft = TableLeft +cur_s + *it2;
						Table[TableLeft] = cur_string;
					}
				}
				else {
					string TableLeft = "";
					TableLeft = TableLeft+ cur_s + cur_string[0];
					Table[TableLeft] = cur_string;
				}	
			}
			if (FIRST[cur_s].count('@') > 0) {
				for (auto it1 = FOLLOW[cur_s].begin(); it1 != FOLLOW[cur_s].end(); it1++) {
					string TableLeft = "";
					TableLeft =TableLeft+ cur_s + *it1;
					Table[TableLeft] = "@";
				}
			}
		}
		for (auto it = Vn.begin(); it != Vn.end(); it++) {
			for (auto it1 = Vt_temp.begin(); it1 != Vt_temp.end(); it1++) {
				string TableLeft = "";
				TableLeft =TableLeft+ *it + *it1;
				if (!Table.count(TableLeft)) {
					Table[TableLeft] = "error";
				}
			}
		}
		
		/*请编程实现以下功能
		***************************************************************************************				
			显示Table，例如格式打印：cout << *it << "->" <<  Table[iter];
		*/
		cout << "显示table表：" << endl << endl<<"  ";
		for (auto it1 = Vt_temp.begin(); it1 != Vt_temp.end(); it1++) {
				char a=*it1;
				cout<<setw(15)<<a;
				}
			cout<<endl;
		for (auto it = Vn.begin(); it != Vn.end(); it++) {
			cout<<*it<<" ";
			for (auto it1 = Vt_temp.begin(); it1 != Vt_temp.end(); it1++) {
				string TableLeft = "";
				TableLeft =TableLeft+ *it + *it1;
				string a=TableLeft.substr(0,1)+"->"+Table[TableLeft];
				cout<<setw(15)<<a;
				}
			cout<<endl;
			}
			
		}
		
	/*
		每一次分析一个输入串
		Sign为符号栈,出栈字符为x
		输入字符串当前字符为a
	*/
	bool AnalyzePredict(string inputstring){
		stack<char>Sign;
		Sign.push('#');
		Sign.push(S);
		int StringPtr = 0;
		char a = inputstring[StringPtr++];
		bool flag = true;
		while (flag) {
			char x = Sign.top();
			Sign.pop();
			//如果是终结符,直接移出符号栈
			if (Vt.count(x)) {
				if (x == a)a = inputstring[StringPtr++];
				else
					return false;
			}
			else {
				/*请编程实现以下功能
				***************************************************************************************
				*/
				//第一步：如果不是终结符，如果是末尾符号
				
				//第二步：如果是非终结符，需要移进操作




				
				
			}		
		}
		return true;
	}
	/*
		消除左递归
	*/
	void remove_left_recursion(){
		string tempVn = "";
		for (auto it = Vn.begin(); it != Vn.end(); it++) {
			tempVn += *it;
		}
		
		for (int i = 0; i < tempVn.length(); i++) {
			char pi = tempVn[i];
			/*请编程实现消除左递归的功能
				***************************************************************************************
			*/















			remove_left_gene(pi);
		}
	}
	/*
		提取左因子
	*/
	void remove_left_gene(char c) {
		char NewVn;
		for (int i = 0; i < 26; i++) {
			NewVn = i + 'A';
			if (!Vn.count(NewVn)) {
				break;
			}
		}
		bool isaddNewVn = 0;
		for (auto it = P[c].begin(); it != P[c].end(); it++) {
			string right = *it;
			
			if (right[0] == c) {
				isaddNewVn = 1;
				
				break;
			}
		}
		if (isaddNewVn) {
			set<string>NewPRight;
			set<string>NewPNewVn;
			for (auto it = P[c].begin(); it != P[c].end(); it++) {
				string right = *it;
				if (right[0] != c) {
					right += NewVn;
					NewPRight.insert(right);
				}
				else {
					right = right.substr(1);
					right += NewVn;
					NewPNewVn.insert(right);
				}
			}
			Vn.insert(NewVn);
			NewPNewVn.insert("@");
			P[NewVn] = NewPNewVn;
			P[c] = NewPRight;
		}
	}
	void ShowByTogether() {
		for (auto it = Vn.begin(); it != Vn.end(); it++) {
			cout << *it << "->";
			char c = *it;
			for (auto it1 = P[c].begin(); it1 != P[c].end(); it1++) {
				if (it1 == P[c].begin())cout << *it1;
				else
					cout << "|" << *it1;
					
			}
			cout << endl;
		}
	}
};
int main() {
	/*
	文法测试
	E->T|E+T;
	T->F|T*F;
	F->i|(E);
	A->+TA|@;
	B->*FB|@;
	E->TA;
	F->(E)|i;
	T->FB;
	直接将上面两个测试样例放在parse_test1.txt和parse_test2.txt中
	*/
	string filename_gramer = "parse_test2.txt";
	Grammar *grammar=new Grammar(filename_gramer);
	cout << "/-------------------------没有消除左递归-----------------------------/" << endl;
	cout << "规格显示："<<endl;
	grammar->ShowByTogether();
	cout << endl;
	grammar->getFirst();
	cout << endl;
	grammar->getFollow();
	cout << endl;
	grammar->getTable();
	cout << "/-------------------------已经消除左递归-----------------------------/" << endl;
	grammar->remove_left_recursion();
	cout << "规格显示：";
	cout << endl;
	grammar->ShowByTogether();
	cout << endl;
	grammar->getFirst();
	cout << endl;
	grammar->getFollow();
	cout << endl;
	grammar->getTable();
	cout << "/--------------------------------------------------------------------end/" <<endl;
	return 0;
}