#ifndef TRIENODE_HPP
#define TRIENODE_HPP

#include <string>

class TrieNode {
  private:
    char data;
    int freq;

  public:
    TrieNode* left;
    TrieNode* middle;
    TrieNode* right;

    TrieNode(const char& d) {
        data = d;
        left = middle = right = nullptr;
    }

    void setData(const char& d) { data = d; }

    void setFreq(const int& f) { freq = f; }

    char getData() { return data; }

    int getFreq() { return freq; }

    TrieNode* nextNode(const char& d) {
        if (int(d) == int(data)) {
            return middle;
        }

        return (int(d) < int(data)) ? left : right;
        // return new TrieNode('a');
    }
};

#endif