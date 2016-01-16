#include <fstream>
#include <iostream>
#include <cstring>
#include "Trie.h"

using namespace std;

Trie::Trie(const char* path) {
    root = new TrieNode();
    try{
        load(path);
    } catch(const char * s) {
        cerr << s << endl;
        exit(1);
    }
}

Trie::~Trie() {
    release(root);
}

int Trie::push(string word, string info) {
    if(!word.length()) return 0;
    TrieNode* cur = root;
    for(int i=0; i<word.length(); i++) {
        if(word[i] < 'a' || word[i] > 'z') return 0;
        if(!cur->toNext(word[i])) {
            cur = cur->insert(new TrieNode(word[i]));
        } else {
            cur = cur->toNext(word[i]);        
        }  
    }
    
    cur->info = new char[info.length()+1];
    strcpy(cur->info, info.c_str());
    return 1;
}

vector< pair<string, char*> > Trie::search(string word) {
    vector< pair<string, char*> > vec;
    search(vec, root, word, "");
    return vec;
}

void Trie::search(vector< pair<string, char*> > &vec, TrieNode* cur, string word, string str) {
    if(!word.length()) return ;
    char temp = word[0];
    word.erase(word.begin());
    if('a' <= temp && temp <= 'z') {
        if(!cur->toNext(temp)) return ;
        str += temp;
        cur = cur->toNext(temp);
        if(!word.length() && cur->info) vec.push_back(pair<string, char*>(str, cur->info)); 
    } else if(temp == '.') {
        for(int i=0; i<cur->next.size(); i++) {
            if(cur->next[i]->info && !word.length()) vec.push_back(pair<string, char*>(str + cur->next[i]->ch, cur->next[i]->info)); 
            search(vec,cur->next[i], word, str+cur->next[i]->ch);
        }
        return ;
    } else if(temp == '*') {
        for(int i=0; i<cur->next.size(); i++) {
            searchAll(vec,cur->next[i],str);
        }
        return ;
    }
    search(vec, cur, word, str);
}
void Trie::searchAll(vector< pair<string, char*> > &vec, TrieNode* cur, string str) {
    if(!cur) return;
    if(cur->info) vec.push_back(pair<string, char*>(str+cur->ch, cur->info));
    
    for(int i=0; i<cur->next.size(); i++) {
        searchAll(vec, cur->next[i], str+cur->ch);
    }
}

int Trie::pop(string word) {
    if(!word.length()) return 0;
        
    TrieNode* cur = root;
    for(int i=0; i<word.length(); i++) {
        cur = cur->toNext(word[i]);
        if(!cur) return 0;
    }
    if(!cur->info) return 0;
    else {
        delete cur->info;
        cur->info = NULL;
        return 1;
    }
}

void Trie::load(const char* filePath) {
    if(!filePath)
        filePath =  "./dictionary.txt";
        //filePath =  "./test.txt";
    ifstream in(filePath, ios::in);
    if(in.fail()) throw "NotFoundFileException";
    
    string word, info;
    string::iterator it;
    while(getline(in, word, ' ')) {
        getline(in, info);
        it = info.begin();
        while(*it == ' ') info.erase(it);
        this->push(word, info);
    }
    in.close();
}

void Trie::release(TrieNode* cur) {
    if(!cur) return ;
    
    for(int i=0; i<cur->next.size(); i++) {
        release(cur->next[i]);
    }
    delete cur;
}
