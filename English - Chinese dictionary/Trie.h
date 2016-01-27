#ifndef __TRIE__HH__
#define __TRIE__HH__

#include <string>
#include <vector>
#include "TrieNode.h"

using namespace std;

class Trie {
    public:
        Trie(const char* path = NULL);
        ~Trie();
        int push(string word, string info);
        vector< pair<string, char*> > search(string word);      //pair把单词与翻译相关联 
        int pop(string word);
    private:
        void search(vector< pair<string, char*> > &vec, TrieNode* cur, string word, string str);
        void searchAll(vector< pair<string, char*> > &vec, TrieNode* cur, string str);
        void load(const char* filePath);
        void release(TrieNode* cur);
        TrieNode* root;
};

#endif 
