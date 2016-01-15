#include <iostream>
#include <iomanip>
#include "Trie.h"

using std::cin;
using std::cout;
using std::endl;

char print();
void menu(Trie* t);
void help();
int main(int argc, char** argv) {
    Trie* t = new Trie();
    menu(t);    
    return 0;
}

void help() {
    cout << "=========================================" << endl;
    cout << "                英汉字典V2.0         \n" << endl;
    cout << "                支持三种查找方式               " << endl; 
    cout << "                     1.普通查找          " << endl;
    cout << "                     2.'.'任意字符替换查找          " << endl;
    cout << "                     3.'*'前缀查找          \n" << endl;
}

char print() {
    cout << "=========================================" << endl;
    cout << "                英汉字典V2.0         \n" << endl;
    cout << "                1.查询               " << endl; 
    cout << "                2.增加/修改               " << endl;
    cout << "                3.删除               " << endl;
    cout << "                4.退出               \n" << endl;
    cout << "                -> ";
    fflush(stdin);
    char ch = getchar();
    fflush(stdin);
    return ch;
    
}

void menu(Trie* t) {
    bool choose = true;
    string str1, str2;
    vector< pair<string, char*> > vec;
    int op;
    help();
    system("pause");
    system("cls"); 
    while(choose) {
        op = -1; 
        switch(print()) {
            case '1':
                cout << "输入单词："; 
                cin >> str1;
                vec = t->search(str1);
                if(vec.size()) {
                    for(vector< pair<string, char*> >::iterator it = vec.begin(); it != vec.end(); it++) {
                        cout << std::left << std::setw(16) << it->first << it->second << endl;
                    }
                }
                else { 
                    cout << "未找到" << endl;
                }
                break;
            case '2':
                cout << "输入单词  词义：" ; 
                cin >> str1 >> str2;
                op = t->push(str1, str2);
                break;
            case '3':
                cout << "输入单词：";
                cin >> str1;
                op = t->pop(str1);
                break;
            case '4':
                choose = false;
                break;
            default:
                fflush(stdin);
                break;
        }
        if(op == 1)
            cout << "操作成功" << endl;
        else if(!op)
            cout << "拼写错误或单词未找到" << endl; 
            
        system("pause");
        system("cls");
    }
}
