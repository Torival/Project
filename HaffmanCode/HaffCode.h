#ifndef __HAFFCODE__HH__
#define __HAFFCODE__HH__
#include <string>
#include <vector>
#include <map>

using namespace std; 
using std::vector;
using std::string;
using std::map;

//haffmantree的结点，在每次创建的时候初始化 
struct Node {
	int weight;
	int ch;
	int lchild, rchild, parent;
	Node(int weight, int ch = 0): lchild(-1), rchild(-1), parent(-1){ 
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
		//.txt -> .haff encode 
		map<char, string> entable; 
        vector<Node*> countCharacter(string &inputPath);
        void buildHuffTree(vector<Node*>& tree);
        void selectMin(vector<Node*>& tree, int& first, int& second);
        void buildHuffMap(vector<Node*>& tree, int cur, string str);
        string encodeFile(string &inputPath);
        
        //.haff -> .txt decode
        string decodeFile(string &inputPath);
};
#endif
