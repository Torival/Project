#ifndef __TRIENODE__HH__
#define __TRIENODE__HH__

#include <vector>

using namespace std;

class TrieNode {
    public:
        TrieNode(char ch = 0);
        TrieNode* insert(TrieNode* cur);
        TrieNode* toNext(char ch);
        
        char ch;
        char* info; 
        vector<TrieNode*> next;
};

#endif 
