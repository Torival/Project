#ifndef __HAFFCODE__HH__
#define __HAFFCODE__HH__
#include <string>
#include <vector>

using namespace std; 
using std::vector;
using std::string;

//haffmantree的结点，在每次创建的时候初始化 
struct Node {
	int weight;
	char ch;
	int left, right, parent;
	Node(int weight, char ch = 0): left(-1), right(-1), parent(-1){ 
        this->weight = weight;
        this->ch = ch;
    }
};
 
class HaffCode {
	public:
		HaffCode(string &inputPath);
		~HaffCode();
		string decode(string &inputPath);
		string encode(string &inputPath);
	private:
        vector<Node*> huffTree;
        
        //build huffTree
        void countCharacter(string &inputPath);
        void buildHuffTree();
        void selectMin(int& first, int& second);
        
        //.txt -> .huff encode
        string getBinary(char ch);
        string encodeFile(string &inputPath);
        
        //.huff -> .txt decode
        char getChar(string str);
        string decodeFile(string &inputPath);
};
#endif
