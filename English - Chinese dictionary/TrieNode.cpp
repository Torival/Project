#include <string>
#include <iostream>
#include "TrieNode.h"

using namespace std;

TrieNode::TrieNode(char c): info(NULL), ch(c), next(0) {
}

TrieNode* TrieNode::insert(TrieNode* cur) {
    if(!cur) throw "NULL Pointer";
    
    int i = 0;
    for(; i < next.size(); i++) {
        if(next[i]->ch == (*cur).ch) return next[i];
        if(next[i]->ch > (*cur).ch) break;
    }
    next.insert(next.begin()+i,cur);
    return next[i];
}

TrieNode* TrieNode::toNext(char ch) {
    if(!this || !ch) throw "NULL Pointer";
    
    int low = 0, high = next.size()-1;
    int mid;
    while(low <= high) {
        mid = (low + high) >> 1;
        if(next[mid]->ch == ch) return next[mid];
        else if(next[mid]->ch < ch) low = ++mid;
        else high = --mid;
    }
    return NULL;
}
