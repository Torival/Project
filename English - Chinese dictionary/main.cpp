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
    cout << "                Ӣ���ֵ�V2.0         \n" << endl;
    cout << "                ֧�����ֲ��ҷ�ʽ               " << endl; 
    cout << "                     1.��ͨ����          " << endl;
    cout << "                     2.'.'�����ַ��滻����          " << endl;
    cout << "                     3.'*'ǰ׺����          \n" << endl;
}

char print() {
    cout << "=========================================" << endl;
    cout << "                Ӣ���ֵ�V2.0         \n" << endl;
    cout << "                1.��ѯ               " << endl; 
    cout << "                2.����/�޸�               " << endl;
    cout << "                3.ɾ��               " << endl;
    cout << "                4.�˳�               \n" << endl;
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
                cout << "���뵥�ʣ�"; 
                cin >> str1;
                vec = t->search(str1);
                if(vec.size()) {
                    for(vector< pair<string, char*> >::iterator it = vec.begin(); it != vec.end(); it++) {
                        cout << std::left << std::setw(16) << it->first << it->second << endl;
                    }
                }
                else { 
                    cout << "δ�ҵ�" << endl;
                }
                break;
            case '2':
                cout << "���뵥��  ���壺" ; 
                cin >> str1 >> str2;
                op = t->push(str1, str2);
                break;
            case '3':
                cout << "���뵥�ʣ�";
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
            cout << "�����ɹ�" << endl;
        else if(!op)
            cout << "ƴд����򵥴�δ�ҵ�" << endl; 
            
        system("pause");
        system("cls");
    }
}
