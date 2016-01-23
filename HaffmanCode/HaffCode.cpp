#include <algorithm>
#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include "HaffCode.h"

using namespace std;

HaffCode::HaffCode(string &inputPath) {
    string haff = encode(inputPath);
    string out = decode(haff);
}

HaffCode::~HaffCode() {
    
}

string HaffCode::encode(string &inputPath) {
    try{
        countCharacter(inputPath);
        buildHuffTree();
        return encodeFile(inputPath);     
    } catch(const char * s) {
        cerr << s << endl;
        exit(1);
    } 
}

string HaffCode::decode(string &inputPath) {
    if(!inputPath.length()) throw "Exception";
    return decodeFile(inputPath);
}

void HaffCode::countCharacter(string &inputPath) {
    ifstream in(inputPath.c_str(), ios::in);
    if(! in.is_open()) throw "Error opening file";
    
    int count[128] = {0};
    while(!in.eof()) {
        count[in.get()]++;
    }
    
    for(int i=0; i<128; i++) {
        if(count[i]) huffTree.push_back(new Node(count[i],i));
        else huffTree.push_back(NULL);
    }
}

void HaffCode::buildHuffTree() { 
    int count = 0;
    int size = huffTree.size();
    int first, second;
    
    for(int i = 0; i< size; i++) 
        if(huffTree[i]) count++;
        
    for(int i=0; i<count - 1; i++) {
        selectMin(first, second); 
        huffTree.push_back(new Node((*huffTree[first]).weight + (*huffTree[second]).weight));
        (*huffTree[first]).parent = huffTree.size()-1;
        (*huffTree[second]).parent = huffTree.size()-1;
        (*huffTree[huffTree.size()-1]).left = first;
        (*huffTree[huffTree.size()-1]).right = second;
    }
}

void HaffCode::selectMin(int& first, int& second) {
    first = -1, second = -1;
    for(int i = 0; i < huffTree.size(); i++) {      
        if(huffTree[i] && (*huffTree[i]).parent == -1) {
            if(first == -1 || second == -1) {
                if(first == -1) {         
                    first = i;
                } else {
                    second = i;
                }
            } else { 
                //to deal 8, 5, 3
                if((*huffTree[first]).weight > (*huffTree[second]).weight)
                    std::swap(first, second);
                
                //select second min
                if((*huffTree[i]).weight < (*huffTree[second]).weight)
                    second = i;                    
            }
        }
    }
}

string HaffCode::encodeFile(string &inputPath) {
    string outputPath = "./compression.haff";
    ofstream out(outputPath.c_str(), ios::out | ios ::binary);
    ifstream in(inputPath.c_str(), ios::in);
    if(in.fail()) throw "Open File Exception";
    
    unsigned char bit;
    string temp;
    int index = 0; 
        
    while(!in.eof()) {
        temp = "";
        temp = getBinary(in.get());
        for(int i = 0; i < temp.length(); i++) {
            bit <<= 1, index++;
            if(temp[i] == '1') bit |= 1;
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
    char ch;
    string temp = "";
    
    while(in.read(&buffer, 1)) { 
        for(int j = 0; j < 8; j++) { 
           if((ch = getChar(temp))) {
                out << ch;
                temp = "";   
           }
           if((buffer >> (7 - j)) & 1) temp += "1";
           else temp += "0";
        }
    }
    out.close();
    in.close();
    
    return outputPath;
}

string HaffCode::getBinary(char ch){
    if(ch < 0) return "";
    
    string result = "";
    int cur = ch;
    int parent = 0;
    
    while((parent = (*huffTree[cur]).parent) != -1) {
        if((*huffTree[parent]).right == cur) result += "1";
        else result += "0";
        cur = parent;
    }
    std::reverse(result.begin(), result.end());
    return result;
}

char HaffCode::getChar(string str) {
    int index = huffTree.size() - 1;
    int len = str.length();
    
    for(int i = 0; i < len; i++) {
        if(str[i] == '1') index = (*huffTree[index]).right;
        else index = (*huffTree[index]).left;
    }
    
    return (*huffTree[index]).ch;
}
