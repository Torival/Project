#include <algorithm>
#include <iostream>
#include <istream>
#include <fstream>
#include <cstring>
#include <vector>
#include "HaffCode.h"

using namespace std;

HaffCode::HaffCode(string &inputPath) {
    string haff = encode(inputPath);
    string out = decode(haff);
    cout << endl << "压缩后的路径为：" << haff << endl;
    cout << "解压后的路径为：" << out << endl;
}

HaffCode::~HaffCode() {
    
}

string HaffCode::encode(string &inputPath) {
    try{
        vector<Node*> tree = countCharacter(inputPath);
        buildHuffTree(tree);
        for(map<char, string>::iterator it = entable.begin(); it!=entable.end(); it++) {
            detable[it->second] = it->first;
            //测试输出huffman编码 
            cout << (char)it->first << " -> " << it->second << endl;
        }
        return encodeFile(inputPath);     
    } catch(const char * s) {
        cout << s << endl;
        exit(1);
    } 
}

string HaffCode::decode(string &inputPath) {
    if(!inputPath.length()) throw "Exception";
    return decodeFile(inputPath);
}

vector<Node*> HaffCode::countCharacter(string &inputPath) {
    ifstream in(inputPath.c_str(), ios::in);
    if(!inputPath.length()) throw "path is NULL";
    if(! in.is_open()) throw "Error opening file";
    
    int count[128] = {0};
    vector<Node*> vec;
    while(!in.eof()) {
        count[in.get()]++;
    }
    
    for(int i=0; i<128; i++) {
        if(count[i]) vec.push_back(new Node(count[i],i));
    }
    return vec;
}

void HaffCode::buildHuffTree(vector<Node*> &tree) { 
    int count = tree.size();
    int first, second;
    for(int i=0; i<count - 1; i++) {
        selectMin(tree, first, second); 
        tree.push_back(new Node((*tree[first]).weight + (*tree[second]).weight));
        (*tree[first]).parent = tree.size()-1;
        (*tree[second]).parent = tree.size()-1;
        (*tree[tree.size()-1]).lchild = first;
        (*tree[tree.size()-1]).rchild = second;
    }
    
    buildHuffMap(tree, tree.size()-1, "");
}

void HaffCode::selectMin(vector<Node*>& tree, int& first, int& second) {
    first = -1, second = -1;
    int flag = 0;
    for(int i = 0; i < tree.size(); i++) {      
        if((*tree[i]).parent == -1) {
            if(!flag) {
                if(first == -1) {         
                    first = i;
                } else {
                    second = i;
                    flag = 1;
                }
            } else { 
                //to deal 8, 5, 3
                if((*tree[first]).weight > (*tree[second]).weight)
                    std::swap(first, second);
                
                //select second min
                if((*tree[i]).weight < (*tree[second]).weight)
                    second = i;
                    
                //select first min
                if(second != i && (*tree[i]).weight < (*tree[first]).weight)      
                    first = i;
            }
        }
    }
}

void HaffCode::buildHuffMap(vector<Node*>& tree, int cur, string str) {
    if((*tree[cur]).lchild == -1 && (*tree[cur]).rchild == -1) {
        entable[(*tree[cur]).ch] = str;
        return ;
    }
    if((*tree[cur]).lchild != -1) buildHuffMap(tree, (*tree[cur]).lchild, str+"0");
    if((*tree[cur]).rchild != -1) buildHuffMap(tree, (*tree[cur]).rchild, str+"1");
}

string HaffCode::encodeFile(string &inputPath) {
    string outputPath = "./compression.haff";
    ofstream out(outputPath.c_str(), ios::out | ios ::binary);
    ifstream in(inputPath.c_str(), ios::in);
    if(out.fail() || in.fail()) throw "Open File Exception";
    
    unsigned char bit;
    string temp;
    int index = 0; 
        
    while(!in.eof()) {
        temp = entable[in.get()];
        while(temp.length()) {
            bit <<= 1;
            if(*temp.begin() == '1') bit |= 1;
            temp.erase(temp.begin());
            index++;
            if(index == 8) {
                out << bit;
                index = 0;
            }
        } 
    }
    if(index && index !=8) {
        bit <<= (8 - index);
        out << bit;
    }
    out.close();
    in.close();
    
    return outputPath;
}

string HaffCode::decodeFile(string &inputPath) {
    string outputPath = "./out.txt";
    ifstream in(inputPath.c_str(), ios::in | ios ::binary);
    ofstream out(outputPath.c_str(), ios::out);
    if(out.fail() || in.fail()) throw "Open File Exception";

    char buffer;
    string temp = "";
    int index = 0; 
    int count = 0;
    
    while(in.read(&buffer, 1)) { 
        for(int j=0; j<8; j++) {            
            if(detable.find(temp) != detable.end()) {
                out << detable[temp];
                temp = "", j--;
            } else {
                if((buffer >> (7 - j)) & 1) temp += "1";
                else temp += "0";
            }
        }
    }
    if(detable.find(temp) != detable.end()) out << detable[temp];
    out.close();
    in.close();
    
    return outputPath;
}
