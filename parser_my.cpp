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
			cout << "FIRST(" << cur_s<<")" ;
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
		int iter = 100;
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
			cout << "FOLLOW(" << cur_s<<")";
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
		cout << "显示table表：" << endl<<"  ";
		for (auto it1 = Vt_temp.begin(); it1 != Vt_temp.end(); it1++) {
				char a=*it1;
				cout<<setw(8)<<a;
				}
			cout<<endl;
		for (auto it = Vn.begin(); it != Vn.end(); it++) {
			cout<<*it<<" ";
			for (auto it1 = Vt_temp.begin(); it1 != Vt_temp.end(); it1++) {
				string TableLeft = "";
				TableLeft =TableLeft+ *it + *it1;
				if(Table[TableLeft]!="error"){
				string a=TableLeft.substr(0,1)+"->"+Table[TableLeft];
				cout<<setw(8)<<a;
				}else
				{
					cout<<setw(8)<<"error";
				}
				
				}
			cout<<endl;
			}
			
		}
	void print_stack(stack<char>Sign,string inputstring,int t){
		cout<<" 栈";
		int len=Sign.size();
		int len1=len;
		char a[100];
		int temp=0;
		while(len--){
			a[temp++]=Sign.top();
			Sign.pop();
		}
		while(temp>0){
			cout<<a[--temp]<<" ";
			Sign.push(a[temp]);
		}
		cout<<setw(20-len1*2)<<inputstring[t];
		for(int i=t+1;i<inputstring.length();i++)
			cout<<inputstring[i];
		cout<<endl;
	}
	bool AnalyzePredict(string inputstring){
		stack<char>Sign;
		Sign.push('#');
		Sign.push(S);
		int StringPtr = 0;
		inputstring+="#";
		print_stack(Sign,inputstring,StringPtr);
		char a = inputstring[StringPtr++];
		bool flag = true;
		while (flag) {
			char x = Sign.top();
			Sign.pop();
			//如果是终结符,直接移出符号栈
			if (is_terminal(x)) {
				if (x == a){
					print_stack(Sign,inputstring,StringPtr);
					a = inputstring[StringPtr++];
					}
				else
					return false;
			}
			else {
				//如果不是终结符，
				//如果是末尾符号
				if (x == '#') {
					if (x == a){
						flag = false;
					}	
					else
						return false;
				}
				else {
					string left = "";
					left += x;
					left += a;
					if (Table[left] != "error") {
						string right = Table[left];
						for (int i = right.length() - 1; i >= 0; i--) {
							Sign.push(right[i]);
						}
							print_stack(Sign,inputstring,StringPtr-1);
					}
					else {
						return false;
					}
				}
			}		
		}
		return true;
	}
	void remove_left_recursion(){
		string tempVn = "";
		for (auto it = Vn.begin(); it != Vn.end(); it++) {
			tempVn += *it;
		}
		for (int i = 0; i < tempVn.length(); i++) {
			char pi = tempVn[i];
			set<string>NewPRight;
			//对于所有的产生式的右部right
			for (auto it = P[pi].begin(); it != P[pi].end(); it++) {
				bool isget = 0;
				string right = *it;
				for (int j = 0; j < i; j++) {
					char pj = tempVn[j];
					//每一个产生式的右部如果等于一个非终结符
					if (pj == right[0]) {
						isget = 1;
						for (auto it1 = P[pj].begin(); it1 != P[pj].end(); it1++) {
							string s = *it1 + right.substr(1);
							NewPRight.insert(s);
							cout<<s<<endl;
						}
					}
				}
				if (isget == 0) {
					NewPRight.insert(right);
				}
			}
			for (int j = 0; j < i; j++) {
				char pj=tempVn[j];
				for (auto it = P[pi].begin(); it != P[pi].end(); it++) {
					string right = *it;
					if (right[0] == pj) {
						for (auto itpj = P[pj].begin(); itpj != P[pj].end(); itpj++) {
							string s = *itpj + right.substr(1);
							NewPRight.insert(s);
						}
					}
					else {
						NewPRight.insert(right);
					}
				}
			}
			if(i!=0)
				P[pi] = NewPRight;
			remove_left_gene(pi);
		}
	}

	void remove_left_gene(char c) {
		//c为原先的非终结符
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
			set<string>NewPRight;//加入了新非终结符NewVn的右部
			set<string>NewPNewVn;//新非终结符的右部
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
			for (auto temp:NewPNewVn)
				cout<<"添加产生式"<<NewVn<<"->"<<temp<<endl;
			P[c] = NewPRight;
			for (auto temp:NewPRight)
				cout<<"原先产生式变化为"<<c<<"->"<<temp<<endl;
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
	cout << "规格显示："<< endl;
	grammar->ShowByTogether();
	cout << endl;
	grammar->getFirst();
	cout << endl;
	grammar->getFollow();
	cout << endl;
	grammar->getTable();
	string temp;
	cout<<"输入预测的句子"<<endl;
	cin>>temp;
	//temp="aaaa";
	if(grammar->AnalyzePredict(temp))
			cout<<endl<<temp<<"  预测分析成功！"<<endl;
	else
			cout<<endl<<temp<<"  预测分析失败"<<endl;	
	return 0;
}