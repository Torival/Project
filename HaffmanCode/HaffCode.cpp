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
    if(!inputPath.length()) throw "path is NULL";
    if(! in.is_open()) throw "Error opening file";
    
    int count[128] = {0};
    while(!in.eof()) {
        count[in.get()]++;
    }
    
    for(int i=0; i<128; i++) {
        if(count[i]) huffTree.push_back(new Node(count[i],i));
    }
}

void HaffCode::buildHuffTree() { 
    int count = huffTree.size();
    int first, second;
    for(int i=0; i<count - 1; i++) {
        selectMin(first, second); 
        huffTree.push_back(new Node((*huffTree[first]).weight + (*huffTree[second]).weight));
        (*huffTree[first]).parent = huffTree.size()-1;
        (*huffTree[second]).parent = huffTree.size()-1;
        (*huffTree[huffTree.size()-1]).lchild = first;
        (*huffTree[huffTree.size()-1]).rchild = second;
    }
    
}

void HaffCode::selectMin(int& first, int& second) {
    first = -1, second = -1;
    int flag = 0;
    for(int i = 0; i < huffTree.size(); i++) {      
        if((*huffTree[i]).parent == -1) {
            if(!flag) {
                if(first == -1) {         
                    first = i;
                } else {
                    second = i;
                    flag = 1;
                }
            } else { 
                //to deal 8, 5, 3
                if((*huffTree[first]).weight > (*huffTree[second]).weight)
                    std::swap(first, second);
                
                //select second min
                if((*huffTree[i]).weight < (*huffTree[second]).weight)
                    second = i;
                    
                //select first min
                if(second != i && (*huffTree[i]).weight < (*huffTree[first]).weight)      
                    first = i;
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
        temp = getBinary(in.get());
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
    char ch;
    string temp = "";
    
    while(in.read(&buffer, 1)) { 
        for(int j=0; j<8; j++) { 
           ch = getChar(temp);
           if(ch) {
                out << ch;
                temp = "";   
           } else {
                if((buffer >> (7 - j)) & 1) temp += "1";
                else temp += "0";
            }
        }
    }
    out.close();
    in.close();
    
    return outputPath;
}


