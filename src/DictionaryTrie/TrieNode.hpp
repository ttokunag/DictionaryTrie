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

    // TrieNode(const char& letter) {
    //     data = letter;
    //     freq = 0;
    //     left = middle = right = nullptr;
    // }

    TrieNode(const char& letter) {
        data = letter;
        freq = 0;
        left = middle = right = nullptr;
    }

    void setData(const char& d) { data = d; }

    void setFreq(const int& f) { freq = f; }

    char getData() { return data; }

    int getFreq() { return freq; }
};

#endif